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
#include "spdlog/spdlog.h"

using namespace ci::app;

ScreenManager::ScreenManager() {
	
	FinancialData *sap500MarketData = new FinancialData("sap500", "S&P 500", "20161230");
	Geographic *sap500Geo = new Geographic(sap500MarketData->mEntities, "S&P 500");
	Orbit *sap500Orbit = new Orbit(sap500MarketData->mEntities, "S&P 500");
	IndustryOrbit *sap500IndustryOrbit = new IndustryOrbit(sap500MarketData->mEntities, "S&P 500");
	sap500IndustryOrbit->setSectorWeights(&(sap500MarketData->mSectorWeights));
	
	FinancialData *mscwxlMarketData = new FinancialData("mscwxl", "MSCI World", "20161230");
	Geographic *mscwxlGeo = new Geographic(mscwxlMarketData->mEntities, "MSCI World");
	Orbit *mscwxlOrbit = new Orbit(mscwxlMarketData->mEntities, "MSCI World");
	IndustryOrbit *mscwxlIndustryOrbit = new IndustryOrbit(mscwxlMarketData->mEntities, "MSCI World");
	mscwxlIndustryOrbit->setSectorWeights(&(mscwxlMarketData->mSectorWeights));
	
	FinancialData *msceurMarketData = new FinancialData("mscief", "MSCI Europe", "20161230");
	Geographic *msceurGeo = new Geographic(msceurMarketData->mEntities, "MSCI Europe");
	Orbit *msceurOrbit = new Orbit(msceurMarketData->mEntities, "MSCI Europe");
	IndustryOrbit *msceurIndustryOrbit = new IndustryOrbit(msceurMarketData->mEntities, "MSCI Europe");
	msceurIndustryOrbit->setSectorWeights(&(msceurMarketData->mSectorWeights));
	
	sap500Geo->setOrder(msceurIndustryOrbit, sap500Orbit);
	sap500Orbit->setOrder(sap500Geo, sap500IndustryOrbit);
	sap500IndustryOrbit->setOrder(sap500Orbit, mscwxlGeo);
	
	mscwxlGeo->setOrder(mscwxlIndustryOrbit, mscwxlOrbit);
	mscwxlOrbit->setOrder(mscwxlGeo, mscwxlIndustryOrbit);
	mscwxlIndustryOrbit->setOrder(mscwxlOrbit, msceurGeo);
	
	msceurGeo->setOrder(msceurIndustryOrbit, msceurOrbit);
	msceurOrbit->setOrder(msceurGeo, msceurIndustryOrbit);
	msceurIndustryOrbit->setOrder(msceurOrbit, sap500Geo);
	
	currentScreen = sap500Geo;
	
	timeStamp = getElapsedSeconds();
}

void ScreenManager::setCamera(RCamera *camera) {
	mCam = camera;
	currentScreen->setCamera(mCam);
	//Screen *next = currentScreen->mNextScreen;
	for (Screen* currentIter = currentScreen->mNextScreen; currentIter != currentScreen; currentIter = currentIter->mNextScreen) {
		currentIter->setCamera(mCam);
	}
}

void ScreenManager::setDashboard(Dashboard *dashboard) {
	mDashboard = dashboard;
}

void ScreenManager::update() {
	
	if (getElapsedSeconds() - timeStamp > currentScreen->screenTime) {
		currentScreen = currentScreen->mNextScreen;
		spdlog::get("particleApp")->info("Running screen {} - {}", currentScreen->mName, currentScreen->mUniverse);
		currentScreen->restart();
		timeStamp = getElapsedSeconds();
		currentScreen->setScreenStartTime(timeStamp);
	}
	currentScreen->update();
}

void ScreenManager::draw() {

	currentScreen->draw();
	currentScreen->displayMessage(mDashboard);
}
