
#include "myClasses.h"


int NumPrticles = 0;

float gravity = -5.f;
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
	
	if (!isAgarre) {
		//noves posicions
		position += velocity*dt;

		//calculem velocitats
		velocity += dt*(force / mass);

		//calculem forces
		force = fElasticFromLeft + fElasticFromRight + fElasticFromDown + fElasticFromUp
				+ fShearUpL + fShearUpR + fShearDownL + fShearDownR;
		force += (mass*acc);
	}
}
void Particle::DetectWall(vec3 n, int d, float dt) {
	
	//calculem quina seria la seva seguent posicio
	vec3 posCreuada = position + dt*velocity;
	
	//si estan una a cada banda del pla fem el rebot
	if ((dot(n,position) + d) * (dot(n,posCreuada) + d) <= 0) {
		
		//calculem la nova posicio
		position = posCreuada - (1 + elasticCoef) * (dot(n, posCreuada) + d)*n;

		//calculem la nova velocitat
		float VperN = dot(velocity, n); // v*n
		//elasticidad
		velocity += -(1 + elasticCoef)*(n*VperN);		
		//friccion
		vec3 vN = dot(velocity, n)*n;
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
	//correccions de distancia
	for (int i = 0; i < particles.size(); ++i) {
		if (i % 14 != 13 && !particles[i].isAgarre) { //si no es de la columna de la dreta
			if (length(particles[i].position - particles[i + 1].position) > (lHorizontal + lHorizontal*maxSeparation)) { //si estan massa separats
				if (particles[i+1].isAgarre)
					particles[i].position -= normalize(particles[i].position - particles[i + 1].position)*(length(particles[i].position - particles[i + 1].position) - (lHorizontal));
				else {
					particles[i].position -= normalize(particles[i].position - particles[i + 1].position)*((length(particles[i].position - particles[i + 1].position) - (lHorizontal))/2);
					particles[i+1].position += normalize(particles[i].position - particles[i + 1].position)*((length(particles[i].position - particles[i + 1].position) - (lHorizontal)) / 2);
				}				
			}
		}
		if (i % 14 != 0 && !particles[i].isAgarre) { //si no es de la columna de la esquerra
			if (length(particles[i].position - particles[i - 1].position) > (lHorizontal + lHorizontal*maxSeparation)) { //si estan massa separats
				if (particles[i - 1].isAgarre)
					particles[i].position -= normalize(particles[i].position - particles[i - 1].position)*(length(particles[i].position - particles[i - 1].position) - (lHorizontal));
				else {
					particles[i].position -= normalize(particles[i].position - particles[i - 1].position)*((length(particles[i].position - particles[i - 1].position) - (lHorizontal)) / 2);
					particles[i - 1].position += normalize(particles[i].position - particles[i - 1].position)*((length(particles[i].position - particles[i - 1].position) - (lHorizontal)) / 2);
				}
			}
		}
		if (i < 17 * 14 && !particles[i].isAgarre) { //si no es de la ultima fila
			if (length(particles[i].position - particles[i + 14].position) > (lVertical + lVertical*maxSeparation)) { //si estan massa separats
				if (particles[i + 14].isAgarre)
					particles[i].position += normalize(particles[i].position - particles[i + 14].position)*(length(particles[i].position - particles[i + 14].position) - (lVertical));
				else {
					particles[i].position += normalize(particles[i].position - particles[i + 14].position)*((length(particles[i].position - particles[i + 14].position) - (lVertical)) / 2);
					particles[i + 14].position -= normalize(particles[i].position - particles[i + 14].position)*((length(particles[i].position - particles[i + 14].position) - (lVertical)) / 2);
				}
			}
		}
		if (i > 13 && !particles[i].isAgarre) { //si no es de la primera fila
			if (length(particles[i].position - particles[i - 14].position) > (lVertical + lVertical*maxSeparation)) { //si estan massa separats
				if (particles[i - 14].isAgarre)
					particles[i].position -= normalize(particles[i].position - particles[i - 14].position)*(length(particles[i].position - particles[i - 14].position) - (lVertical));
				else {
					particles[i].position -= normalize(particles[i].position - particles[i - 14].position)*((length(particles[i].position - particles[i - 14].position) - (lVertical)) / 2);
					particles[i - 14].position += normalize(particles[i].position - particles[i - 14].position)*((length(particles[i].position - particles[i - 14].position) - (lVertical)) / 2);
				}
			}
		}
	}	
	
}

void particleManager::CalculateForces() {

	

	for (int i = 0;i < 18 * 14;i++) {
		//Veïns estructurals
		if (i % 14 != 13) { //si no es de la columna de la dreta
			particles[i].fElasticFromRight = -(ke*(length(particles[i].position - particles[i + 1].position) - lHorizontal) /* - (ke (||P1-P2|| - L12)) + */
				+ kd*(particles[i].velocity - particles[i + 1].velocity) * ((particles[i].position - particles[i + 1].position) / (length(particles[i].position - particles[i + 1].position)))); //+ kd(v1-v2)*P1-P2/||P1-P2||)
			particles[i].fElasticFromRight *= (particles[i].position - particles[i + 1].position) / (length(particles[i].position - particles[i + 1].position)); // (...) * P1-P2/||P1-P2||
			
		}
		if (i < 17 * 14) { //si no es de la ultima fila
			particles[i].fElasticFromDown = -(ke*(length(particles[i].position - particles[i + 14].position) - lVertical) /* - (ke (||P1-P2|| - L12)) + */
				+ kd*(particles[i].velocity - particles[i + 14].velocity) * ((particles[i].position - particles[i + 14].position) / (length(particles[i].position - particles[i + 14].position)))); //+ kd(v1-v2)*P1-P2/||P1-P2||)
			particles[i].fElasticFromDown *= (particles[i].position - particles[i + 14].position) / (length(particles[i].position - particles[i + 14].position)); // (...) * P1-P2/||P1-P2||
		}
		if (i % 14 != 0) { //si no es de la columna de la esquerra
			particles[i].fElasticFromLeft = particles[i-1].fElasticFromRight *-1.f;
			
		}
			
		if (i > 13) { //si no es de la primera fila
			particles[i].fElasticFromUp = particles[i - 14].fElasticFromDown * -1.f;
		}
			
		//Veïns de shear
		/*
		}*/
		//if(i % 14 != 13&& i < 17 * 14&& i % 14 != 0&& i > 13){
			
			if (i % 14 != 13&& i < (17 * 14)) { //si no es de la columna de la dreta i si no es de la ultima fila

				particles[i].fShearDownR = -(ke*(length(particles[i].position - particles[i + 15].position) - lHorizontal) 
					+ kd*(particles[i].velocity - particles[i + 15].velocity) * ((particles[i].position - particles[i + 15].position) / (length(particles[i].position - particles[i + 15].position))));
				particles[i].fShearDownR *= (particles[i].position - particles[i + 15].position) / (length(particles[i].position - particles[i + 15].position));

			}
			if (i < (17 * 14) && i % 14 != 0) { //si no es de la ultima fila i si no es de la columna de la esquerra
				particles[i].fShearDownL = -(ke*(length(particles[i].position - particles[i + 13].position) - lVertical)
					+ kd*(particles[i].velocity - particles[i + 13].velocity) * ((particles[i].position - particles[i + 13].position) / (length(particles[i].position - particles[i + 13].position))));
				particles[i].fShearDownL *= (particles[i].position - particles[i + 13].position) / (length(particles[i].position - particles[i + 13].position));
			}
			if (i % 14 != 0 && i > 13) { //si no es de la columna de la esquerra i si no es de la primera fila
				particles[i].fShearUpL = particles[i - 15].fShearDownR *-1.f;

			}

			if (i > 13&& i % 14 != 13) { //si no es de la primera fila I si no es de la columna de la dreta 
				particles[i].fShearUpR = particles[i - 13].fShearDownL * -1.f;
			}
		//}
/*		if (i == 1) {
			particles[i].fElasticFromRight = -(ke*(length(particles[i].position - particles[i + 13].position) - lHorizontal)
				+ kd*(particles[i].velocity - particles[i + 13].velocity) * ((particles[i].position - particles[i + 13].position) / (length(particles[i].position - particles[i + 13].position))));
			particles[i].fElasticFromRight *= (particles[i].position - particles[i + 13].position) / (length(particles[i].position - particles[i + 13].position));
		}
		if (i==14){
			particles[i].fElasticFromUp = particles[i - 13].fElasticFromDown * -1.f;
		}
		if (i == 14 * 17) {
			particles[i].fElasticFromRight = -(ke*(length(particles[i].position - particles[i + 13].position) - lHorizontal)
				+ kd*(particles[i].velocity - particles[i + 13].velocity) * ((particles[i].position - particles[i + 13].position) / (length(particles[i].position - particles[i + 13].position))));
			particles[i].fElasticFromRight *= (particles[i].position - particles[i + 13].position) / (length(particles[i].position - particles[i + 13].position));
		}
		if (i == 14*17) {
			particles[i].fElasticFromUp = particles[i - 13].fElasticFromDown * -1.f;
		}

		if (i % 14 == 13 && i > 17 * 14 && i % 14 == 0 && i < 13) {


		}*/
	}
}







