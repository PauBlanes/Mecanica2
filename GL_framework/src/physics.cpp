#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "myClasses.h"
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
bool show_test_window = false;
static bool Play_simulation = true;
static bool Reset = false;
static float resertTime=20;
float Second;
//Gravity
static float GravityAccel[3] = { 0.0f, -9.81f,0.0f };
//k
static float kStretch[2] = {1000.00,50.00};
static float kShear[2] = { 1000.00,50.00 };
static float kBend[2] = { 1000.00,50.00 };
static float linkDistance;
static float MaxElogation;
static bool Elongation = true;
//collisions
static bool collisions = true;
static bool sphereCollisions = true;
static float ElasticCoeff=0.5;
static float FrictionCoeff=0.1;
void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		Second += 1 / ImGui::GetIO().Framerate;

		//TODO
		//for play & stop
		ImGui::Checkbox("Play simulation", &Play_simulation);
		//for resert
		if (ImGui::Button("Reset simulation")) {
			Reset ^= 1;
		}
		//Resert Time
		ImGui::DragFloat("Resert Time", &resertTime, 20); 
		ImGui::InputFloat3("Gravity Accel", GravityAccel);
		//Spring parameters
		if (ImGui::CollapsingHeader("Spring parameters")){
			//k-stretch
			ImGui::InputFloat2("k-stretch", kStretch);
			//k-shear
			ImGui::InputFloat2("k-shear", kShear);
			//k-bend
			ImGui::InputFloat2("k-bend", kBend);
			//Particles Link Distance
			ImGui::DragFloat("Particles Link Distance", &linkDistance, 0.5);
			//Max Elongation per cent
			ImGui::DragFloat("Max Elongation per cent", &resertTime, 0.05);
			//boolean use Elongation Correction
			ImGui::Checkbox("Use Elongation Corrections", &Elongation);
		}
		//Collisions
		if (ImGui::CollapsingHeader("Collisions")) {
			//Use collisions
			ImGui::Checkbox("Use Collisions", &collisions);
			//Use Sphere collider
			ImGui::Checkbox("Use Sphere Collider", &sphereCollisions);
			//Elastic Coefficion
			ImGui::DragFloat("Elastic Coefficion", &ElasticCoeff, 0.5);
			//Friction Coefficion
			ImGui::DragFloat("Friction Coefficion", &FrictionCoeff, 0.1);
		}
	}
	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

float RandomFloat(float min, float max) {
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

void PhysicsInit() {
	//particles
	for (int i = 0; i < 18;i++) {
		for (int j = 0; j < 14;j++) {
			Particle temp({ -3 + 0.5*j, 5, -3 + i*0.5 }, 1, 0.5, 0.5);
			pM.particles.push_back(temp);
		}
	}
	//sphere

}
void PhysicsUpdate(float dt) {
	if (Play_simulation) {
		pM.Update(dt);
	}
	if (Reset) {
		Second = 0;
		Second += 1 / ImGui::GetIO().Framerate;
		for (int i = 0; i < pM.particles.size(); i++) {
			pM.particles.clear(); //netejem l'array i com que el dels vertex nomes fem update tenint en compte el tamany d'aquest dons no tindrem problemes.
		}
		PhysicsInit();
		Reset = false;
		Play_simulation = true;
	}

	//TODO
}
void PhysicsCleanup() {
	//TODO
}