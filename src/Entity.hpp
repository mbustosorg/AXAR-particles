//
//  Entity.hpp
//  openGlTest
//
//  Created by Mauricio Bustos on 3/15/17.
//
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "Particle.h"
#include <string.h>

using namespace std;

class Entity {

public:
	
	Entity(string name, string industry);
	~Entity();
	
	void updateParticle(Particle particle);
	
private:
	
	Particle mParticle;
	string name;
	string industry;
	
};

#endif
