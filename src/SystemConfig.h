/*
 
 Copyright (C) 2017 Mauricio Bustos (m@bustos.org)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

#ifndef SystemConfig_h
#define SystemConfig_h

const int NUM_PARTICLES = 1001;
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
