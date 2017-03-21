#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "myClasses.h"
#include <time.h>
#include <stdio.h>
/*
TASK
1. Simulation of the whole mesh using your solver of choice (Euler or Verlet). (4pt)
	- particle-spring mesh of 14 x 18 points
	- mass equal to 1 kg
	- the mesh should be animated except for two at the top, which should have a fixed position (points 0 and 13).
	- mesh such that it is horizontal at a initial height and make the position
	- runs for 20 seconds, it should reinitialize

2. Collision with walls and ground planes. (2pt)

3. Collision with the Sphere. (2pt)
	- random location
	- radius of the sphere random.

4. Make the parameters tweakable from the GUI. (2pt)
	 Constant and damping term of direct-link springs (stretch).
	 Constant and damping term of diagonal-link springs (shear).
	 Constant and damping term of second-link springs (bend).
	 Max % of accepted ellongation of links. //LO DE CORREGIR QUE CAU MOLT A SACO, EL RUBBERY DEFORMATION
	 Initial Rest distance of the springs between the points of the mesh. //la longitud?
*/

particleManager pM;
Esfera esfera;

bool show_test_window = false;
void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}


void PhysicsInit() {
	for (int i = 0; i < 18;i++) {
		for (int j = 0; j < 14;j++) {
			if ((i == 0 && j == 0) || (i == 0 && j == 13)) {
				Particle temp({ -3 + 0.3*j, 8, -3 + i*0.3 }, 1, 0.3, 0.3, true);
				pM.particles.push_back(temp);
				partVerts[j * 3 + 0] = -3 + 0.3*j;
				partVerts[j * 3 + 1] = 8;
				partVerts[j * 3 + 2] = -3 + i*0.3;
			}
			else {
				Particle temp({ -3 + 0.3*j, 5, -3 + i*0.3 }, 1, 0.3, 0.3, false);
				pM.particles.push_back(temp);
			}			
		}			
	}
	
	//La esfera
	srand(time(NULL));

	float min = 0.5;
	float max = 3;

	esfera.radius = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));

	min = -4 + esfera.radius;
	max = 4 - esfera.radius;

	esfera.position.x = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	esfera.position.y = 1;
	esfera.position.z = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	Sphere::updateSphere(esfera.position, esfera.radius);

	//els murs
	pM.wallNormals[0] = { 0,1,0 };
	pM.wallNormals[1] = { 0,-1,0 };
	pM.wallNormals[2] = { 1,0,0 };
	pM.wallNormals[3] = { -1,0,0 };
	pM.wallNormals[4] = { 0,0,1 };
	pM.wallNormals[5] = { 0,0,-1 };
	pM.wallDs[0] = 0;
	pM.wallDs[1] = 10;
	pM.wallDs[2] = -5;
	pM.wallDs[3] = 5;
	pM.wallDs[4] = 5;
	pM.wallDs[5] = -5;
}
void PhysicsUpdate(float dt) {
	
	//detectar esfera
	for (int i = 0;i < pM.particles.size();i++)
		pM.particles[i].DetectSphere(esfera.position, esfera.radius, dt);
	//actualizar pos particles
		pM.Update(dt);
	
}
void PhysicsCleanup() {
	//TODO
}