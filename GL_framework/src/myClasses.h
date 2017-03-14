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

extern float *partVerts;


enum solverMethod { euler, verlet };
struct coords { float x;float y;float z; };
enum emiterType {font, cascada};
extern float gravity;

class Particle {
	friend class particleManager;
	
	vec3 velocity;
	vec3 position;

	float mass;
	vec3 force;
	vec3 acc;

	float elasticCoef;
	float frictionCoef;	

	float lifeCounter;

	
public:
	float particleLife;
	Particle(vec3 pos, vec3 initAcc, float laMassa, float eC, float fC);
	
	void Move(float dt);
	void DetectWall(coords n, int d, float dt);
	void DetectSphere(coords pos, float radius, float dt);
	
};


class particleManager {

	float spawnCounter;
	
public:
	int emitterRate;
	coords pos1; //posicio del emissor
	coords pos2; //posicio del emissor per cascada
	coords dir; //vector de velocitat inicial de les particules
	emiterType eType; //tipus d'emissor
	float particleLife;
	solverMethod partsMethod;
	float elasticCoef;
	float frictionCoef;
	float fontAngle; //l'angle amb que surten les particules a la font

	coords wallNormals[6];
	int wallDs[6];
	std::vector<Particle> particles;
	void SpawnParticles(emiterType spawnType);
	void Update(float dt);
	
};


