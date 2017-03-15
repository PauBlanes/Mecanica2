#pragma once

#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <vector>
#include <iostream>

#include "imgui\imgui.h"
#include <time.h>

//Boolean variables allow to show/hide the primitives
extern bool renderSphere;
extern bool renderParticles;
using namespace glm;
struct coords { float x; float y; float z; };

namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.05f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);	

}
namespace Sphere {
	extern void setupSphere(glm::vec3 pos = glm::vec3(0.f, 1.f, 0.f), float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}

namespace ClothMesh {
	extern void setupClothMesh();
	extern void cleanupClothMesh();
	extern void updateClothMesh(float* array_data);
	extern void drawClothMesh();
}

extern float* partVerts;
extern float gravity;

class Particle {
	friend class particleManager;
	
	vec3 velocity;
	vec3 position;

	float mass; //hola
	vec3 force;
	vec3 acc;

	float elasticCoef;
	float frictionCoef;	

	
	
public:
	
	Particle(vec3 pos, float laMassa, float eC, float fC);
	
	void Move(float dt);
	void DetectWall(vec3 n, int d, float dt);
	void DetectSphere(vec3 centreEsfera, float radius, float dt);
	
};


class particleManager {

		
public:	
	float elasticCoef;
	float frictionCoef;
		
	vec3 wallNormals[6];
	int wallDs[6];
	std::vector<Particle> particles;
	void Update(float dt);
	
};


