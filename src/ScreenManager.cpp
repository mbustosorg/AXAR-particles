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
#include "FinancialData.hpp"
#include "cinder/app/AppBAse.h"

using namespace ci::app;

ScreenManager::ScreenManager() {

	FinancialData marketData("sap500");
	industryOrbit.setSectorWeights(&marketData.mSectorWeights);
	
	mEntities = marketData.mEntities;
	marketData.loadQuotes();
	
	screens.push_back(&geo);
	screens.push_back(&orbit);
	screens.push_back(&industryOrbit);
	
	std::for_each(screens.begin(), screens.end(), [this](Screen *n){ n->setEntities(mEntities); n->setup(); });
	currentScreen = screens.begin();
	timeStamp = getElapsedSeconds();
}

void ScreenManager::setCamera(RCamera *camera) {
	mCam = camera;
	std::for_each(screens.begin(), screens.end(), [this](Screen *n){ n->setCamera(mCam); });
}

void ScreenManager::update() {
	if (getElapsedSeconds() - timeStamp > (*currentScreen)->screenTime) {
		++currentScreen;
		if (currentScreen == screens.end()) {
			currentScreen = screens.begin();
		}
		timeStamp = getElapsedSeconds();
	}
	(*currentScreen)->update();
}

void ScreenManager::draw() {
	(*currentScreen)->draw();
}
