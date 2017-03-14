
#include "myClasses.h"

float gravity;
int NumPrticles = 0;
Particle::Particle(vec3 pos, vec3 initAcc, float laMassa, float eC, float fC) {
	
		
	position = pos;	

	velocity = {0,0,0};
	mass = laMassa;

	//calculem la forca
	acc.x = initAcc.x;
	acc.y = gravity + initAcc.y;
	acc.z = initAcc.z;

	force.x = mass*acc.x;
	force.y = mass*acc.y;
	force.z = mass*acc.z;

	elasticCoef = eC;
	frictionCoef = fC;

	
};
void Particle::Move(float dt) {
	
				
		//noves posicions
		position.x += dt*velocity.x;
		position.y += dt*velocity.y;
		position.z += dt*velocity.z;
		
		//sumem acceleracions que ens dona l'emissor o en el cas d'un camp de for鏰
		velocity.x += dt*(force.x / mass);
		velocity.y += dt*(force.y / mass);
		velocity.z += dt*(force.z / mass);
				
	
	
	//recalculem forces per si han canviat
	force.x = mass*acc.x;
	force.y = mass*acc.y;
	force.z = mass*acc.z;
}
void Particle::DetectWall(vec3 n, int d, float dt) {
	//calculem quina seria la seva seguent posicio
	vec3 posCreuada = { 0,0,0 };
	//colisio per metode euler
	//AMB PUNT I PUNT PROJECTAT CALCULAR LA POS ON ESTARA DESPRES DEL REBOT
	posCreuada.x = position.x + dt*velocity.x;
	posCreuada.y = position.y + dt*velocity.y;
	posCreuada.z = position.z + dt*velocity.z;

	//el rebot
	if ((n.x*position.x + n.y*position.y + n.z*position.z + d) * (n.x*posCreuada.x + n.y*posCreuada.y + n.z*posCreuada.z + d) <= 0) {
		//std::cout << "colisio" << std::endl;
		float VperN = (n.x*velocity.x) + (n.y*velocity.y) + (n.z*velocity.z); // v*n
		//elasticidad
		velocity.x += -(1 + elasticCoef)*(n.x*VperN);
		velocity.y += -(1 + elasticCoef)*(n.y*VperN);
		velocity.z += -(1 + elasticCoef)*(n.z*VperN);
		//friccion
		vec3 vN;
		vN.x = VperN*n.x;
		vN.y = VperN*n.y;
		vN.z = VperN*n.z;
		velocity.x += -frictionCoef * (velocity.x - vN.x); //-u*vT
		velocity.y += -frictionCoef * (velocity.y - vN.y);
		velocity.z += -frictionCoef * (velocity.z - vN.z);

	}
}
void Particle::DetectSphere(vec3 Pos, float radius, float dt) {
	//calculem quina seria la seva seguent posicio
	vec3 posCreuada = {0,0,0};
	//colisio per metode euler
		posCreuada.x = position.x + dt*velocity.x;
		posCreuada.y = position.y + dt*velocity.y;
		posCreuada.z = position.z + dt*velocity.z;
		vec3 distVector = { posCreuada.x - Pos.x, posCreuada.y - Pos.y, posCreuada.z - Pos.z };
		float dist = sqrt((distVector.x*distVector.x)+ (distVector.y*distVector.y)+ (distVector.z*distVector.z));
		if (dist < radius) {
			//std::cout << "CollShpere" << std::endl;
			//trobar punt d'interseccio -- wikipedia line-sphere intersection
			//vector interseccio-centre
			//normalitzar i aixo es la normal
			vec3 n = { distVector.x / dist, distVector.y / dist, distVector.z / dist };
			float VperN = (n.x*velocity.x) + (n.y*velocity.y) + (n.z*velocity.z); // v*n
																				  //elasticidad
			velocity.x += -(1 + elasticCoef)*(n.x*VperN);
			velocity.y += -(1 + elasticCoef)*(n.y*VperN);
			velocity.z += -(1 + elasticCoef)*(n.z*VperN);
			//friccion
			vec3 vN;
			vN.x = VperN*n.x;
			vN.y = VperN*n.y;
			vN.z = VperN*n.z;
			velocity.x += -frictionCoef * (velocity.x - vN.x); //-u*vT
			velocity.y += -frictionCoef * (velocity.y - vN.y);
			velocity.z += -frictionCoef * (velocity.z - vN.z);
			
		}
		
}



//MANAGER
void particleManager::Update(float dt) {
	
	//actualitzar el array de vertexs
	for (int i = 0; i < particles.size(); ++i) {
		
		particles[i].Move(dt);
		
		particles[i].elasticCoef = elasticCoef;
		particles[i].frictionCoef = frictionCoef;

		partVerts[i * 3 + 0] = particles[i].position.x;
		partVerts[i * 3 + 1] = particles[i].position.y;
		partVerts[i * 3 + 2] = particles[i].position.z;
				

	}	

	LilSpheres::updateParticles(0, particles.size(), partVerts);

	
}




