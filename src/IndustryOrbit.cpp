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

IndustryOrbit::IndustryOrbit(FinancialData* financialData, string universe) {
	setEntities(financialData->mEntities);
	if (universe == "MSCI World") {
		screenTime = IND_MSCIW_LENGTH;
		mFocusTimes = new TargetFocusTimes(randomEntityIndex(IND_MSCIW_ENTITY_COUNT), IND_MSCIW_START, IND_MSCIW_END);
	} else {
		screenTime = IND_LENGTH;
		mFocusTimes = new TargetFocusTimes(randomEntityIndex(IND_ENTITY_COUNT), IND_START, IND_END);
	}
	mName = "Industry Orbit";
	mUniverse = universe;
	setup();
}

void IndustryOrbit::setup() {
	mParticles.assign(Num_Lines * 2, Particle());
	mParticleHeads.assign(Num_Triangles * 3, Particle());
	Screen::setup();
	loadUpdateProgram("industryOrbit.vs");
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
	
	if (mPrevScreen->mCurrentPositions->size() != mCurrentPositions->size()) {
		mCurrentPositions->assign(Num_Lines * 2, Particle());
	}
	vector<Particle> *tempVector = mPrevScreen->mCurrentPositions;
	mParticles.swap(*tempVector);
	
	mRestartTime = timeStamp();
	if (mUniverse == "MSCI World") {
		mFocusTimes->restart((int)mEntities.size(), randomEntityIndex(IND_MSCIW_ENTITY_COUNT));
	} else {
		mFocusTimes->restart((int)mEntities.size(), randomEntityIndex(IND_ENTITY_COUNT));
	}

	mTransitionFactor = 0.0f;
	
	for (unordered_map<string, Entity*>::iterator entity = mEntities.begin(); entity != mEntities.end(); ++entity) {
		
		Entity* currentEntity = entity->second;
		int particleId = currentEntity->mParticleIndex;
		
		vec3 center = vec3(0.75 * SPHERE_RADIUS * cos((float)SectorIndices.at(currentEntity->mSector) / (float)(SectorIndices.size()) * M_PI * 2.0),
						   0.75 * SPHERE_RADIUS * sin((float)SectorIndices.at(currentEntity->mSector) / (float)(SectorIndices.size()) * M_PI * 2.0), 0.0);
		
		vec3 sphericalLocation = (currentEntity->mSphericalLocation * (float)mSectorWeights->at(SectorIndices.at(currentEntity->mSector))) * vec3(0.75, 0.75, 0.75);
		
		for (int i = particleId * (TRAIL_LENGTH * 2); i < (particleId + 1) * (TRAIL_LENGTH * 2); i = i + TRAIL_LENGTH * 2)	{
			
			float rotSpeed = -Rand::randFloat(ROTATION_SPEED - 0.008f, ROTATION_SPEED + 0.008f);
			float rotZ = Rand::randFloat(-1.5f, 1.5f);
			vec3 flatSphericalLocation = vec3(entity->second->mSphericalLocation.z, 0.0, -entity->second->mSphericalLocation.x);
			vec3 rotationAxis = vec3(vec4(flatSphericalLocation, 1.0) * rotate(-rotZ, sphericalLocation));
			
			for (int j = 0; j < TRAIL_LENGTH; j++) {
				auto &p = mParticles.at(i + j * 2);
				//p.pos = vec4(sphericalLocation, 1.0);
				p.startPosition = p.pos;
				p.sphericalPosition = vec4(sphericalLocation, 1.0);
				p.color = entity->second->mColor;
				p.index = i + j * 2;
				p.delay = (float) (i / 32 + j * 2);
				p.translation = translate(mat4(1.0f), center);
				p.rotation = rotate(rotSpeed, rotationAxis);

				auto &pNext = mParticles.at(i + j * 2 + 1);
				//pNext.pos = vec4(sphericalLocation, 1.0);
				pNext.startPosition = pNext.pos;
				pNext.sphericalPosition = vec4(sphericalLocation, 1.0);
				pNext.color = entity->second->mColor;
				pNext.index = i + j * 2 + 1;
				pNext.delay = (float) (i / 32 + j * 2 + 2.0f) ;
				pNext.translation = translate(mat4(1.0f), center);
				pNext.rotation = rotate(rotSpeed, rotationAxis);

			}
		}
		particleId++;
	}
	setupBuffers(mAttributes, mParticleBuffer, &mParticles);
	setupBuffers(mAttributesHead, mParticleHeadBuffer, &mParticleHeads);
}

void IndustryOrbit::update() {
	mTransitionFactor += 1.0f;
	mParticleUpdateProg.get()->uniform("transitionFactor", mTransitionFactor);
	Screen::update();
}


