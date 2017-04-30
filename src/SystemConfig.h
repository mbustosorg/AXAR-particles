//
//  SystemConfig.h
//  AXAR_ParticleApp
//
//  Created by Mauricio Bustos on 4/28/17.
//
//

#ifndef SystemConfig_h
#define SystemConfig_h

const int NUM_PARTICLES = 501;
const int TRAIL_LENGTH = 25;
const int SPHERE_RADIUS = 2500;
const int NUM_LINES = NUM_PARTICLES * TRAIL_LENGTH;
const int NUM_TRIANGLES = NUM_PARTICLES * 4 * 2;
const int PARTICLE_VECTOR_LENGTH = NUM_LINES * 2;
const int PARTICLE_HEAD_VECTOR_LENGTH = NUM_TRIANGLES * 3 * 2;
const float SECTION_ARC_LENGTH = 0.04f;

const float HEAD_DEPTH = 10.0f;
const float HEAD_ARC_WIDTH = 0.005f;

const float ROTATION_SPEED = 0.01f;

#endif /* SystemConfig_h */
