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
	 Max % of accepted ellongation of links.
	 Initial Rest distance of the springs between the points of the mesh.
*/

particleManager pM;

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
	//TODO
}
void PhysicsUpdate(float dt) {
	//TODO
}
void PhysicsCleanup() {
	//TODO
}