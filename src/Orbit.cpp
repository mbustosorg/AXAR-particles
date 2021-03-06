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

#include "Orbit.hpp"
#include "cinder/Rand.h"

Orbit::Orbit(FinancialData* financialData, string universe) {
	setEntities(financialData->mEntities);
	if (universe == "MSCI World") {
		screenTime = ORB_MSCIW_LENGTH;
		mFocusTimes = new TargetFocusTimes(randomEntityIndex(ORB_MSCIW_ENTITY_COUNT), ORB_MSCIW_START, ORB_MSCIW_END);
	} else {
		screenTime = ORB_LENGTH;
		mFocusTimes = new TargetFocusTimes(randomEntityIndex(ORB_ENTITY_COUNT), ORB_START, ORB_END);
	}
	mName = "Orbit";
	mUniverse = universe;
	setup();
}

void Orbit::updateHeadParticle(Entity* entity, int index, Particle* current,
							   vec3 offsetVector, vec3 rotationAxis,
							   float rotZ, float rotSpeed) {
	current->pos = vec4(entity->mSphericalLocation + offsetVector, 1.0);
	current->color = entity->mColor;
	current->index = index;
	current->translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	current->rotation = rotate(rotSpeed, rotationAxis);
}

void Orbit::restart() {
	setup();
	mRestartTime = timeStamp();
	mTarget = NULL;
	if (mUniverse == "MSCI World") {
		mFocusTimes->restart((int)mEntities.size(), randomEntityIndex(ORB_MSCIW_ENTITY_COUNT));
	} else {
		mFocusTimes->restart((int)mEntities.size(), randomEntityIndex(ORB_ENTITY_COUNT));
	}
}

void Orbit::displayMessage(Dashboard *dashboard) {
	float deltaTime = (timeStamp() - mRestartTime) / MESSAGE_FADE_SECONDS;
	if (deltaTime < 1.0) {
		dashboard->displayMessage(mUniverse, DEFAULT_TEXT_X, DEFAULT_TEXT_Y, UNIVERSE_FONT_SIZE, Color(deltaTime, deltaTime, deltaTime), false);
	} else if (deltaTime < 2.0) {
		dashboard->displayMessage(mUniverse, DEFAULT_TEXT_X, DEFAULT_TEXT_Y, UNIVERSE_FONT_SIZE, Color(2.0 - deltaTime, 2.0 - deltaTime, 2.0 - deltaTime), false);
	}
	if (mCam->mTarget) {
		std::locale underscore_locale(std::locale(), new underscore_numpunct());
		std::stringstream capitalization;
		capitalization.imbue(underscore_locale);
		capitalization << std::setprecision(0) << std::fixed << mTarget->mUsdCapitalization;
		string targetName = mTarget->mName + "\n" + capitalization.str();
		dashboard->displayMessage(targetName, DEFAULT_TEXT_X, DEFAULT_TEXT_Y, ENTITY_FONT_SIZE, Color(200.0, 200.0, 200.0), true);
	}
}

void Orbit::setup() {
	
	mParticles.assign(Num_Lines * 2, Particle());
	mParticleHeads.assign(Num_Triangles * 3, Particle());
	
	const float Head_Length = 10.0f;
	const float Head_Width = 10.0f;
	
	for (unordered_map<string, Entity*>::iterator entity = mEntities.begin(); entity != mEntities.end(); ++entity) {
		
		int particleId = entity->second->mParticleIndex;
		vec3 sphericalLocation = entity->second->mSphericalLocation;
		
		for (int i = particleId * (TRAIL_LENGTH * 2); i < (particleId + 1) * (TRAIL_LENGTH * 2); i = i + TRAIL_LENGTH * 2)	{
			
			float rotSpeed = -Rand::randFloat(ROTATION_SPEED - 0.018f, ROTATION_SPEED + 0.018f);
			float rotZ = Rand::randFloat(-1.5f, 1.5f);
			vec3 flatSphericalLocation = vec3(sphericalLocation.z, 0.0, -sphericalLocation.x);
			vec3 rotationAxis = vec3(vec4(flatSphericalLocation, 1.0) * rotate(-rotZ, sphericalLocation));
			
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24    )), vec3(Head_Length, 0.0f, 00.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 1)), vec3(0.0f, Head_Width, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 2)), vec3(0.0f, 0.0f, Head_Width), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 3)), vec3(Head_Length, 0.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 4)), vec3(0.0f, -Head_Width, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 5)), vec3(0.0f, 0.0f, Head_Width), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 6)), vec3(Head_Length, 0.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 7)), vec3(0.0f, Head_Width, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 8)), vec3(0.0f, 0.0f, -Head_Width), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 9)), vec3(Head_Length, 0.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 10)), vec3(0.0f, -Head_Width, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 11)), vec3(0.0f, 0.0f, -Head_Width), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 12)), vec3(-Head_Length, 0.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 13)), vec3(0.0f, -Head_Width, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 14)), vec3(0.0f, 0.0f, Head_Width), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 15)), vec3(-Head_Length, 0.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 16)), vec3(0.0f, Head_Width, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 17)), vec3(0.0f, 0.0f, Head_Width), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 18)), vec3(-Head_Length, 0.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 19)), vec3(0.0f, Head_Width, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 20)), vec3(0.0f, 0.0f, -Head_Width), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 21)), vec3(-Head_Length, 0.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 22)), vec3(0.0f, -Head_Width, 0.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(mParticleHeads.at(particleId * 24 + 23)), vec3(0.0f, 0.0f, -Head_Width), rotationAxis, rotZ, rotSpeed);
			
			for (int j = 0; j < TRAIL_LENGTH; j++) {
				auto &p = mParticles.at(i + j * 2);
				p.pos = vec4(sphericalLocation, 1.0);
				p.sphericalPosition = vec4(sphericalLocation, 1.0);
				p.color = entity->second->mColor;
				p.index = i;
				p.delay = (float) j;
				p.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
				p.rotation = rotate(rotSpeed, rotationAxis);
				
				auto &pNext = mParticles.at(i + j * 2 + 1);
				pNext.pos = vec4(sphericalLocation, 1.0);
				pNext.sphericalPosition = vec4(sphericalLocation, 1.0);
				pNext.color = entity->second->mColor;
				pNext.index = i;
				pNext.delay = (float) j + 1.0f;
				pNext.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
				pNext.rotation = rotate(rotSpeed, rotationAxis);
			}
		}
		particleId++;
	}
	Screen::setup();
	loadUpdateProgram("orbitUpdate.vs");
}

