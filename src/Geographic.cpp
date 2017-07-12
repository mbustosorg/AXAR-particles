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

#include "Geographic.hpp"
#include "cinder/Easing.h"
#include "cinder/app/AppBAse.h"

using namespace ci::app;

Geographic::Geographic(unordered_map<string, Entity*> entities, string universe) {
	screenTime = 20.0f;
	mName = "Geographic";
	mUniverse = universe;
	setEntities(entities);
	setup();
}

void Geographic::setup() {
	mParticles.assign(Num_Lines * 2, Particle());
	mParticleHeads.assign(Num_Triangles * 3, Particle());
	
	int index = 0;
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		auto entity = i->second;
		maxWeight = max(maxWeight, entity->mWeight);
		entity->mParticleIndex = index;
		index++;
	}
	
	restartTime = fmod(getElapsedFrames() / 60.0f, 1000.f);
	
	Screen::setup();
	loadUpdateProgram("industryOrbit.vs");
}

void Geographic::restart() {
	//vector<Particle> *tempVector = mPrevScreen->currentPositions();
	//mParticles.swap(*tempVector);
	//vector<Particle>().swap(*tempVector);
	//delete(tempVector);
	
	restartTime = fmod(getElapsedFrames() / 60.0f, 1000.f);
}

void Geographic::update() {
	
}

void Geographic::displayMessage(Dashboard *dashboard) {
	float deltaTime = (getElapsedSeconds() - mScreenStartTime) / 5.0;
	if (deltaTime < 1.0) {
		dashboard->displayMessage(mUniverse, -2000.0f, 900.0f, 200, Color(deltaTime, deltaTime, deltaTime));
	} else if (deltaTime < 2.0) {
		dashboard->displayMessage(mUniverse, -2000.0f, 900.0f, 200, Color(1.0 - deltaTime / 2.0f, 1.0 - deltaTime / 2.0f, 1.0 - deltaTime / 2.0f));
	}
}

void Geographic::draw()
{
	gl::clear();
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	gl::setMatrices(mCam->mCam);
	
	const float rotationTime = 4.5f;
	const float rotationOffset = 0.03f;
	
	float currentTime = fmod(getElapsedFrames() / 60.0f, 1000.f);
	
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		
		Entity* entity = i->second;
		
		if (entity->mWeight > 0.0001) {
			auto &p = mParticles.at(entity->mParticleIndex * TRAIL_LENGTH * 2);
			
			float rotation = 0;
			float startTime = entity->mParticleIndex * rotationOffset;
			if (currentTime > (startTime + restartTime)) rotation = (currentTime - (startTime + restartTime)) / rotationTime;
			if (rotation > 1.0) rotation = 1.0;
			
			int shapeIndex = int((entity->mWeight / maxWeight * rotation) * (RadiusSteps - 1));
			auto shape = mShapes[shapeIndex];
			
			float angle = easeOutBack(rotation);
			gl::ScopedModelMatrix scpModelMatrix;
			vec3 translation = entity->mSphericalLocation - vec3(p.pos);
			gl::translate(vec3(p.pos) + translation * angle);
			gl::color(entity->mColor);
			
			shape->draw();
		}
	}
}
