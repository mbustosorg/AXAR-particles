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
#include "SystemConfig.h"
#include "cinder/Easing.h"

using namespace ci::app;

Geographic::Geographic(FinancialData* financialData, string universe) {
	setEntities(financialData->mEntities);
	mName = "Geographic";
	mUniverse = universe;
	mUniverseCap = financialData->mTotalCap;
	mCountryCount = financialData->mCountryCounts.size();
	if (universe == "MSCI World") {
		screenTime = GEO_MSCIW_LENGTH;
		mFocusTimes = new TargetFocusTimes(randomEntityIndex(GEO_MSCIW_ENTITY_COUNT), GEO_MSCIW_START, GEO_MSCIW_END);
	} else {
		screenTime = GEO_LENGTH;
		mFocusTimes = new TargetFocusTimes(randomEntityIndex(GEO_ENTITY_COUNT), GEO_START, GEO_END);
	}
	setup();
	
	std::locale underscore_locale(std::locale(), new underscore_numpunct());
	std::stringstream buffer;
	buffer.imbue(underscore_locale);
	buffer << std::setprecision(0) << std::fixed << mUniverseCap;
	mUniverse = mUniverse + "\n" + buffer.str();
}

void Geographic::setup() {
	mParticles.assign(Num_Lines * 2, Particle());
	mParticleHeads.assign(Num_Triangles * 3, Particle());
	
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		auto entity = i->second;
		maxWeight = max(maxWeight, entity->mWeight);
	}
	
	mRestartTime = timeStamp();
	
	Screen::setup();
	loadUpdateProgram("industryOrbit.vs");
}

void Geographic::restart() {
	if (mPrevScreen->mCurrentPositions->size() == mParticles.size()) {
		vector<Particle> *tempVector = mPrevScreen->mCurrentPositions;
		mParticles.swap(*tempVector);
	}
	mRestartTime = timeStamp();
	if (mUniverse == "MSCI World") {
		mFocusTimes->restart((int)mEntities.size(), randomEntityIndex(GEO_MSCIW_ENTITY_COUNT));
	} else {
		mFocusTimes->restart((int)mEntities.size(), randomEntityIndex(GEO_ENTITY_COUNT));
	}
}

void Geographic::update() {
	float currentTime = timeStamp();
	mTarget = NULL;
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		Entity* entity = i->second;
		if (entity->mWeight > MINIMUM_WEIGHT) {
			float rotation = 0.0f;
			float startTime = entity->mParticleIndex * rotationOffset;
			if (currentTime > (startTime + mRestartTime)) rotation = (currentTime - (startTime + mRestartTime)) / rotationTime;
			if (rotation > 1.0) rotation = 1.0;
			if (mFocusTimes->active() && mFocusTimes->newFocusTrigger(timeStamp() - mRestartTime) && entity->mParticleIndex == mFocusTimes->focusIndex()) {
				mTarget = entity;
			}
		}
	}
	Screen::updateTargetView();
}

void Geographic::displayMessage(Dashboard *dashboard) {
	
	float deltaTime = (timeStamp() - mRestartTime) / MESSAGE_FADE_SECONDS;
	if (deltaTime < 1.0) {
		dashboard->displayMessage(mUniverse, DEFAULT_TEXT_X, DEFAULT_TEXT_Y, UNIVERSE_FONT_SIZE, Color(deltaTime, deltaTime, deltaTime), false);
	} else if (deltaTime < 2.0) {
		dashboard->displayMessage(mUniverse, DEFAULT_TEXT_X, DEFAULT_TEXT_Y, UNIVERSE_FONT_SIZE, Color(2.0 - deltaTime, 2.0 - deltaTime, 2.0 - deltaTime), false);
	}
	if (mTarget) {
		std::locale underscore_locale(std::locale(), new underscore_numpunct());
		std::stringstream latitude;
		latitude.imbue(underscore_locale);
		latitude << std::setprecision(4) << std::fixed << mTarget->mLatitude;
		std::stringstream longitude;
		longitude.imbue(underscore_locale);
		longitude << std::setprecision(4) << std::fixed << mTarget->mLongitude;
		string targetName = mTarget->mName + "\n" + latitude.str() + ", " + longitude.str();
		dashboard->displayMessage(targetName, DEFAULT_TEXT_X, DEFAULT_TEXT_Y, ENTITY_FONT_SIZE, Color(200.0, 200.0, 200.0), true);
	}
}

void Geographic::draw()
{
	gl::clear();
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	gl::setMatrices(mCam->mCam);
	
	float currentTime = timeStamp();
	
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		
		Entity* entity = i->second;
		
		if (entity->mWeight > MINIMUM_WEIGHT) {
			auto &p = mParticles.at(entity->mParticleIndex * TRAIL_LENGTH * 2);
			
			float rotation = 0.0f;
			float startTime = entity->mParticleIndex * rotationOffset;
			if (currentTime > (startTime + mRestartTime)) rotation = (currentTime - (startTime + mRestartTime)) / rotationTime;
			if (rotation > 1.0) rotation = 1.0;
			
			int shapeIndex = int((entity->mWeight / maxWeight * rotation) * (RadiusSteps - 1));
			auto shape = mShapes[shapeIndex];
			
			float angle = easeOutBack(rotation);
			gl::ScopedModelMatrix scpModelMatrix;
			vec3 translation = entity->mSphericalLocation - vec3(p.pos);
			if (mFocusTimes->active() && mFocusTimes->newFocusTrigger(timeStamp() - mRestartTime) && entity->mParticleIndex == mFocusTimes->focusIndex()) {
				mTarget = entity;
			}
			entity->setPosition(vec3(p.pos) + translation * angle);
			gl::translate(vec3(p.pos) + translation * angle);
			gl::color(entity->mColor);
			
			shape->draw();
		}
	}
}
