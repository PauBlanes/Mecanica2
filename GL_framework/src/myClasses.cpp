
#include "myClasses.h"


int NumPrticles = 0;
float gravity = -9.8;
Particle::Particle(vec3 pos, float laMassa, float eC, float fC) {
	
		
	position = pos;	

	velocity = {0,0,0};
	mass = laMassa;

	//calculem les forces inicials
	acc.y = gravity;
	force = mass*acc;
	
	//posem els coeficients
	elasticCoef = eC;
	frictionCoef = fC;

	
};
void Particle::Move(float dt) {
	
				
		//noves posicions
		position += velocity*dt;
		
		//calculem velocitats
		velocity += dt*(force / mass);	
	
		//calculem forces
		force = mass*acc;
	
}
void Particle::DetectWall(vec3 n, int d, float dt) {
	
	//calculem quina seria la seva seguent posicio
	vec3 posCreuada = position + dt*velocity;
	
	//si estan una a cada banda del pla fem el rebot
	if ((dot(n,position) + d) * (dot(n,posCreuada) + d) <= 0) {
		
		//calculem la nova posicio
		position = posCreuada - 2 * (dot(n, posCreuada) + d)*n;

		//calculem la nova velocitat
		float VperN = dot(velocity, n); // v*n
		//elasticidad
		velocity += -(1 + elasticCoef)*(n*VperN);		
		//friccion
		vec3 vN = VperN*n;
		velocity += -frictionCoef * (velocity - vN); //-u*vT	

	}
}
void Particle::DetectSphere(vec3 centreEsfera, float radius, float dt) {
	//calculem quina seria la seva seguent posicio
	vec3 posCreuada = position + dt*velocity;;
	
	vec3 distVector = posCreuada - centreEsfera;
	float dist = length(distVector);
		if (dist < radius) {			
			//trobar punt d'interseccio
			vec3 l = normalize(velocity); //normalitzem velocitat per fer la linia que surt de PosActual i va en dir de la velocitat
			float d = -dot(l, (position - centreEsfera)) + sqrt((dot(l, (position - centreEsfera)))*(dot(l, (position - centreEsfera))) - ((length(position - centreEsfera))*(length(position - centreEsfera))) + (radius*radius));
			vec3 intersectionPoint = position + l*d;
			//vector interseccio-centre sera la normal del pla
			vec3 n = intersectionPoint - centreEsfera;

			//elasticitat
			float VperN = dot (n, velocity); // v*n
			velocity += -(1 + elasticCoef)*(n*VperN);
			
			//friccion
			vec3 vN = VperN*n;			
			velocity += -frictionCoef * (velocity - vN); //velocity = velocity -u*vT
						
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




