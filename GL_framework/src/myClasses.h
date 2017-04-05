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

	vec3 fElasticFromRight = { 0,0,0 };
	vec3 fElasticFromLeft = { 0,0,0 };
	vec3 fElasticFromUp = { 0,0,0 };
	vec3 fElasticFromDown = { 0,0,0 };

	vec3 fShearUpR = { 0,0,0 };
	vec3 fShearUpL = { 0,0,0 };
	vec3 fShearDownR = { 0,0,0 };
	vec3 fShearDownL = { 0,0,0 };

	float mass; //hola
	vec3 force;
	vec3 acc;

	float elasticCoef;
	float frictionCoef;	

	
	
public:
	
	bool isAgarre;

	Particle(vec3 pos, float laMassa, float eC, float fC, float agarre);
	
	void Move(float dt);
	void DetectWall(vec3 n, int d, float dt);
	void DetectSphere(vec3 centreEsfera, float radius, float dt);
	
};


class particleManager {

		
public:	
	float elasticCoef;
	float frictionCoef;
	
	float ke;
	float kd;
	float lHorizontal;
	float lVertical;
	float maxSeparation;

	vec3 wallNormals[6];
	int wallDs[6];
	std::vector<Particle> particles;
	void Update(float dt);
	void CalculateForces();
};

struct Esfera {
	vec3 position;
	float radius;
	
};


