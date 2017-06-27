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

#include "ScreenManager.hpp"
#include "cinder/app/AppBAse.h"

using namespace ci::app;

ScreenManager::ScreenManager() {
	
	marketData = new FinancialData("sap500", "20161230");

	industryOrbit.setSectorWeights(&(marketData->mSectorWeights));
	
	mEntities = marketData->mEntities;
	
	geo.setOrder(&industryOrbit, &orbit);
	orbit.setOrder(&geo, &industryOrbit);
	industryOrbit.setOrder(&orbit, &geo);

	currentScreen = &geo;
	currentScreen->setEntities(mEntities);
	currentScreen->setup();
	for (Screen* currentIter = currentScreen->mNextScreen; currentIter != currentScreen; currentIter = currentIter->mNextScreen) {
		currentIter->setEntities(mEntities);
		currentIter->setup();
	}
	timeStamp = getElapsedSeconds();
}

void ScreenManager::setCamera(RCamera *camera) {
	mCam = camera;
	currentScreen->setCamera(mCam);
	for (Screen* currentIter = currentScreen->mNextScreen; currentIter != currentScreen; currentIter = currentIter->mNextScreen) {
		currentIter->setCamera(mCam);
	}
}

void ScreenManager::update() {
	
	if (getElapsedSeconds() - timeStamp > currentScreen->screenTime) {
		currentScreen = currentScreen->mNextScreen;
		currentScreen->restart();
		timeStamp = getElapsedSeconds();
	}
	currentScreen->update();
}

void ScreenManager::draw() {
	currentScreen->draw();
}
