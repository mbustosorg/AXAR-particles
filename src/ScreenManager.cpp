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
	
	Geographic *sap500Geo2 = new Geographic(sap500MarketData->mEntities, "S&P 500");
	Orbit *sap500Orbit2 = new Orbit(sap500MarketData->mEntities, "S&P 500");
	IndustryOrbit *sap500IndustryOrbit2 = new IndustryOrbit(sap500MarketData->mEntities, "S&P 500");
	sap500IndustryOrbit2->setSectorWeights(&(sap500MarketData->mSectorWeights));
	
	FinancialData *mscwxlMarketData = new FinancialData("mscwxl", "MSCI World", "20161230");
	Geographic *mscwxlGeo = new Geographic(mscwxlMarketData->mEntities, "MSCI World");
	Orbit *mscwxlOrbit = new Orbit(mscwxlMarketData->mEntities, "MSCI World");
	IndustryOrbit *mscwxlIndustryOrbit = new IndustryOrbit(mscwxlMarketData->mEntities, "MSCI World");
	mscwxlIndustryOrbit->setSectorWeights(&(mscwxlMarketData->mSectorWeights));

	Geographic *mscwxlGeo2 = new Geographic(mscwxlMarketData->mEntities, "MSCI World");
	Orbit *mscwxlOrbit2 = new Orbit(mscwxlMarketData->mEntities, "MSCI World");
	IndustryOrbit *mscwxlIndustryOrbit2 = new IndustryOrbit(mscwxlMarketData->mEntities, "MSCI World");
	mscwxlIndustryOrbit2->setSectorWeights(&(mscwxlMarketData->mSectorWeights));

	FinancialData *msceurMarketData = new FinancialData("mscief", "MSCI Europe", "20161230");
	Geographic *msceurGeo = new Geographic(msceurMarketData->mEntities, "MSCI Europe");
	Orbit *msceurOrbit = new Orbit(msceurMarketData->mEntities, "MSCI Europe");
	IndustryOrbit *msceurIndustryOrbit = new IndustryOrbit(msceurMarketData->mEntities, "MSCI Europe");
	msceurIndustryOrbit->setSectorWeights(&(msceurMarketData->mSectorWeights));

	Geographic *msceurGeo2 = new Geographic(msceurMarketData->mEntities, "MSCI Europe");
	Orbit *msceurOrbit2 = new Orbit(msceurMarketData->mEntities, "MSCI Europe");
	IndustryOrbit *msceurIndustryOrbit2 = new IndustryOrbit(msceurMarketData->mEntities, "MSCI Europe");
	msceurIndustryOrbit2->setSectorWeights(&(msceurMarketData->mSectorWeights));

	sap500Geo->setOrder(msceurIndustryOrbit, sap500Orbit);
	sap500Orbit->setOrder(sap500Geo, sap500IndustryOrbit);
	sap500IndustryOrbit->setOrder(sap500Orbit, sap500Geo2);
	
	sap500Geo2->setOrder(sap500IndustryOrbit, sap500Orbit2);
	sap500Orbit2->setOrder(sap500Geo2, sap500IndustryOrbit2);
	sap500IndustryOrbit2->setOrder(sap500Orbit2, mscwxlGeo);
	
	mscwxlGeo->setOrder(sap500IndustryOrbit2, mscwxlOrbit);
	mscwxlOrbit->setOrder(mscwxlGeo, mscwxlIndustryOrbit);
	mscwxlIndustryOrbit->setOrder(mscwxlOrbit, mscwxlGeo2);
	
	mscwxlGeo2->setOrder(mscwxlIndustryOrbit, mscwxlOrbit2);
	mscwxlOrbit2->setOrder(mscwxlGeo2, mscwxlIndustryOrbit2);
	mscwxlIndustryOrbit2->setOrder(mscwxlOrbit2, msceurGeo);
	
	msceurGeo->setOrder(msceurIndustryOrbit, msceurOrbit);
	msceurOrbit->setOrder(msceurGeo, msceurIndustryOrbit);
	msceurIndustryOrbit->setOrder(msceurOrbit, msceurGeo2);
	
	msceurGeo2->setOrder(msceurIndustryOrbit, msceurOrbit2);
	msceurOrbit2->setOrder(msceurGeo2, msceurIndustryOrbit2);
	msceurIndustryOrbit2->setOrder(msceurOrbit2, sap500Geo);
	
	currentScreen = sap500Geo;
	
	timeStamp = getElapsedSeconds();
}

void ScreenManager::setCamera(RCamera *camera) {
	mCam = camera;
	currentScreen->setCamera(mCam);
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
	}
	currentScreen->update();
}

void ScreenManager::draw() {
	currentScreen->draw();
	currentScreen->displayMessage(mDashboard);
}
