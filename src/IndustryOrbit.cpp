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

#include "IndustryOrbit.hpp"
#include "SystemConfig.h"
#include "cinder/Rand.h"

IndustryOrbit::IndustryOrbit() {
	screenTime = 2.0f;
}

void IndustryOrbit::setSectorWeights(unordered_map<int, double> *sectorWeights) {
	mSectorWeights = sectorWeights;
}

void IndustryOrbit::updateHeadParticle(Entity* entity, int index, Particle* current,
							   vec3 offsetVector, vec3 rotationAxis,
							   float rotZ, float rotSpeed, vec4 center) {
	
	current->pos = center + vec4(entity->mSphericalLocation + offsetVector, 1.0);
	current->color = entity->mColor;
	current->index = index;
	current->translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	current->rotation = rotate(rotSpeed, rotationAxis);
}

void IndustryOrbit::restart() {
	
	vector<Particle> *tempVector = mPrevScreen->currentPositions();
	mParticles.swap(*tempVector);
	vector<Particle>().swap(*tempVector);
	delete(tempVector);
	
	int particleId = 0;
	for (unordered_map<string, Entity*>::iterator entity = mEntities.begin(); entity != mEntities.end(); ++entity) {
		
		Entity* currentEntity = entity->second;
		
		vec3 center = vec3(0.75 * SPHERE_RADIUS * cos(SectorIndices.at(currentEntity->mSector) * M_PI / (float)SectorIndices.size() * 2.0),
						   0.75 * SPHERE_RADIUS * sin(SectorIndices.at(currentEntity->mSector) * M_PI / (float)SectorIndices.size() * 2.0), 0.0);
		
		vec3 sphericalLocation = (currentEntity->mSphericalLocation * (float)mSectorWeights->at(SectorIndices.at(currentEntity->mSector))) * vec3(0.75, 0.75, 0.75);
		
		for (int i = particleId * (TRAIL_LENGTH * 2); i < (particleId + 1) * (TRAIL_LENGTH * 2); i = i + TRAIL_LENGTH * 2)	{
			
			float rotSpeed = -Rand::randFloat(ROTATION_SPEED - 0.008f, ROTATION_SPEED + 0.008f);
			float rotZ = Rand::randFloat(-1.5f, 1.5f);
			vec3 flatSphericalLocation = vec3(entity->second->mSphericalLocation.z, 0.0, -entity->second->mSphericalLocation.x);
			vec3 rotationAxis = vec3(vec4(flatSphericalLocation, 1.0) * rotate(-rotZ, sphericalLocation));
			
			for (int j = 0; j < TRAIL_LENGTH; j++) {
				auto &p = mParticles.at(i + j * 2);
				//p.pos = vec4(sphericalLocation, 1.0);
				//p.sphericalPosition = vec4(sphericalLocation, 1.0);
				p.color = entity->second->mColor;
				p.index = i;
				p.delay = (float) j;
				p.translation = translate(mat4(1.0f), center);
				p.rotation = rotate(rotSpeed, rotationAxis);
				
				auto &pNext = mParticles.at(i + j * 2 + 1);
				//pNext.pos = vec4(sphericalLocation, 1.0);
				//pNext.sphericalPosition = vec4(sphericalLocation, 1.0);
				pNext.color = entity->second->mColor;
				pNext.index = i;
				pNext.delay = (float) j + 1.0f;
				pNext.translation = translate(mat4(1.0f), center);
				pNext.rotation = rotate(rotSpeed, rotationAxis);
			}
		}
		particleId++;
	}
	
	setupBuffers(mAttributes, mParticleBuffer, &mParticles);
	setupBuffers(mAttributesHead, mParticleHeadBuffer, &mParticleHeads);

}

void IndustryOrbit::setup() {
	
	mParticles.assign(Num_Lines * 2, Particle());
	mParticleHeads.assign(Num_Triangles * 3, Particle());
/*
	particleHeads.assign(Num_Triangles * 3, Particle());
	
	int particleId = 0;
	for (unordered_map<string, Entity*>::iterator entity = mEntities.begin(); entity != mEntities.end(); ++entity) {
		
		Entity* currentEntity = entity->second;
		
		vec3 center = vec3(0.75 * SPHERE_RADIUS * cos(SectorIndices.at(currentEntity->mSector) * M_PI / (float)SectorIndices.size() * 2.0),
						   0.75 * SPHERE_RADIUS * sin(SectorIndices.at(currentEntity->mSector) * M_PI / (float)SectorIndices.size() * 2.0), 0.0);
		
		vec3 sphericalLocation = (entity->second->mSphericalLocation * (float)mSectorWeights->at(SectorIndices.at(currentEntity->mSector))) * vec3(0.75, 0.75, 0.75);
		
		for (int i = particleId * (TRAIL_LENGTH * 2); i < (particleId + 1) * (TRAIL_LENGTH * 2); i = i + TRAIL_LENGTH * 2)	{
			
			float rotSpeed = -Rand::randFloat(ROTATION_SPEED - 0.008f, ROTATION_SPEED + 0.008f);
			float rotZ = Rand::randFloat(-1.5f, 1.5f);
			vec3 flatSphericalLocation = vec3(entity->second->mSphericalLocation.z, 0.0, -entity->second->mSphericalLocation.x);
			vec3 rotationAxis = vec3(vec4(flatSphericalLocation, 1.0) * rotate(-rotZ, sphericalLocation));

			for (int j = 0; j < TRAIL_LENGTH; j++) {
				auto &p = particles.at(i + j * 2);
				p.pos = vec4(sphericalLocation, 1.0);
				p.sphericalPosition = vec4(sphericalLocation, 1.0);
				p.color = entity->second->mColor;
				p.index = i;
				p.delay = (float) j;
				p.translation = translate(mat4(1.0f), center);
				p.rotation = rotate(rotSpeed, rotationAxis);
				
				auto &pNext = particles.at(i + j * 2 + 1);
				pNext.pos = vec4(sphericalLocation, 1.0);
				pNext.sphericalPosition = vec4(sphericalLocation, 1.0);
				pNext.color = entity->second->mColor;
				pNext.index = i;
				pNext.delay = (float) j + 1.0f;
				pNext.translation = translate(mat4(1.0f), center);
				pNext.rotation = rotate(rotSpeed, rotationAxis);
			}
		}
		particleId++;
	}
 */
	loadUpdateProgram("industryOrbit.vs");
}

