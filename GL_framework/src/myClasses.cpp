
#include "myClasses.h"


int NumPrticles = 0;

float gravity = -9.8;
float* partVerts = new float[LilSpheres::maxParticles * 3];

Particle::Particle(vec3 pos, float laMassa, float eC, float fC, float agarre) {
	
	isAgarre = agarre;

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
			//trobem el punt d'interseccio
			vec3 l = normalize(velocity); //normalitzem velocitat per fer la recta que surt de PosActual i va en dir de la velocitat
			float distIntersec = -dot(l, (position - centreEsfera)) - sqrt((dot(l, (position - centreEsfera)))*(dot(l, (position - centreEsfera))) - ((length(position - centreEsfera))*(length(position - centreEsfera))) + (radius*radius));
			vec3 intersectionPoint = position + l*distIntersec;
			//comprovem si el punt d'interseccio que hem calculat esta entre pos i posCreuada
			if (length(position - intersectionPoint) + length(intersectionPoint - posCreuada) != length(position - posCreuada))
				distIntersec = -dot(l, (position - centreEsfera)) + sqrt((dot(l, (position - centreEsfera)))*(dot(l, (position - centreEsfera))) - ((length(position - centreEsfera))*(length(position - centreEsfera))) + (radius*radius));
			
			//vector interseccio-centre sera la normal del pla
			vec3 n = normalize(intersectionPoint - centreEsfera);

			//calcular d del pla i pos de rebot
			float d = -dot(n, intersectionPoint);
			position = posCreuada - 2 * (dot(n, posCreuada) + d)*n;

			//elasticitat			
			velocity += -(1 + elasticCoef)*(n*dot(n, velocity));
			
			//friccion
			vec3 vN = dot(n, velocity)*n;
			velocity += -frictionCoef * (velocity - vN); //velocity = velocity -u*vT
						
		}		
}



//MANAGER
void particleManager::Update(float dt) {
	
	//actualitzar el array de vertexs
	for (int i = 0; i < particles.size(); ++i) {
		
		if (particles[i].isAgarre == false) {
			//colisio murs
			for (int j = 0; j < 6;j++) {
				particles[i].DetectWall(wallNormals[j], wallDs[j], dt);
			}
			//colisio esfera
			
			particles[i].Move(dt);

			particles[i].elasticCoef = elasticCoef;
			particles[i].frictionCoef = frictionCoef;

			partVerts[i * 3 + 0] = particles[i].position.x;
			partVerts[i * 3 + 1] = particles[i].position.y;
			partVerts[i * 3 + 2] = particles[i].position.z;
		}
		
	}	
	ClothMesh::updateClothMesh(partVerts);
	//LilSpheres::updateParticles(0, particles.size(), partVerts);
	
}







