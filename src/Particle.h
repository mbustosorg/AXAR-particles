//
//  Particle.h
//  AXAR_ParticleApp
//
//  Created by Mauricio Bustos on 4/28/17.
//
//

#ifndef Particle_h
#define Particle_h

#include "cinder/gl/gl.h"

using namespace cinder;

struct Particle
{
	vec4	pos;
	vec4	ppos;
	vec4	home;
	ColorA  color;
	float	damping;
	float	index;
	mat4	translation;
	mat4	rotation;
};

#endif
