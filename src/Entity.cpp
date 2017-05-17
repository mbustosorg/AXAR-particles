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

#include "Entity.hpp"
#include "SystemConfig.h"
#include "cinder/Rand.h"

Entity::Entity(string name, string industry) {
	mLatitude = Rand::randFloat(-180.0f, 180.0f);
	mLongitude = Rand::randFloat(-180.0f, 180.0f);
}

Entity::~Entity() {
	
}

void Entity::updateParticle(Particle particle) {
	mParticle = particle;
}

vec3 Entity::sphericalLocation() {
	float latitude = Rand::randFloat(mLatitude - 0.01, mLatitude + 0.01) * M_PI / 180.0;
	float longitude = Rand::randFloat(mLongitude - 0.01, mLongitude + 0.01) * M_PI / 180.0;
	float x = -SPHERE_RADIUS * cos(latitude) * cos(longitude);
	float y = -SPHERE_RADIUS * sin(latitude);
	float z = SPHERE_RADIUS * cos(latitude) * sin(longitude);
	return vec3(x, y, z);
}
