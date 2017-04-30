//
//  Orbit.hpp
//  AXAR_ParticleApp
//
//  Created by Mauricio Bustos on 4/28/17.
//
//

#ifndef Orbit_hpp
#define Orbit_hpp

#include <stdio.h>
#include "Screen.hpp"

class Orbit : public Screen {
	
public:
	Orbit();
	
	void setup();

private:
	
	void updateHeadParticle(int index, int total, Particle* current,
							float theta, float thetaOffset,
							float phi, float phiOffset,
							float radius, float radiusOffset,
							float rotZ, float rotSpeed);
	
};

#endif
