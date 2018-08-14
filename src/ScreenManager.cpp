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
#include "cinder/app/AppBase.h"
#include "spdlog/spdlog.h"
#include "Poco/DirectoryIterator.h"

using namespace ci::app;

ScreenManager::ScreenManager() {
	
	std::string fileRoot = string(mFileRoot);
	Poco::DirectoryIterator it(fileRoot);
	Poco::DirectoryIterator end;
	string latestDataDirectory = "";
	while (it != end)
	{
		if (it->isDirectory()) latestDataDirectory = it.name();
		++it;
	}
	
	FinancialData *mscwxlMarketData = new FinancialData("mscwxl", "MSCI World", latestDataDirectory);
	Geographic *mscwxlGeo = new Geographic(mscwxlMarketData, "MSCI World");
	Orbit *mscwxlOrbit = new Orbit(mscwxlMarketData, "MSCI World");
	IndustryOrbit *mscwxlIndustryOrbit = new IndustryOrbit(mscwxlMarketData, "MSCI World");
	mscwxlIndustryOrbit->setSectorWeights(&(mscwxlMarketData->mSectorWeights));

	Geographic *mscwxlGeo2 = new Geographic(mscwxlMarketData, "MSCI World");
	Orbit *mscwxlOrbit2 = new Orbit(mscwxlMarketData, "MSCI World");
	IndustryOrbit *mscwxlIndustryOrbit2 = new IndustryOrbit(mscwxlMarketData, "MSCI World");
	mscwxlIndustryOrbit2->setSectorWeights(&(mscwxlMarketData->mSectorWeights));

	FinancialData *sap500MarketData = new FinancialData("sap500", "S&P 500", latestDataDirectory);
	Geographic *sap500Geo = new Geographic(sap500MarketData, "S&P 500");
	Orbit *sap500Orbit = new Orbit(sap500MarketData, "S&P 500");
	IndustryOrbit *sap500IndustryOrbit = new IndustryOrbit(sap500MarketData, "S&P 500");
	sap500IndustryOrbit->setSectorWeights(&(sap500MarketData->mSectorWeights));
	
	Geographic *sap500Geo2 = new Geographic(sap500MarketData, "S&P 500");
	Orbit *sap500Orbit2 = new Orbit(sap500MarketData, "S&P 500");
	IndustryOrbit *sap500IndustryOrbit2 = new IndustryOrbit(sap500MarketData, "S&P 500");
	sap500IndustryOrbit2->setSectorWeights(&(sap500MarketData->mSectorWeights));
	
	FinancialData *msceurMarketData = new FinancialData("mscief", "MSCI Europe", latestDataDirectory);
	Geographic *msceurGeo = new Geographic(msceurMarketData, "MSCI Europe");
	Orbit *msceurOrbit = new Orbit(msceurMarketData, "MSCI Europe");
	IndustryOrbit *msceurIndustryOrbit = new IndustryOrbit(msceurMarketData, "MSCI Europe");
	msceurIndustryOrbit->setSectorWeights(&(msceurMarketData->mSectorWeights));

	Geographic *msceurGeo2 = new Geographic(msceurMarketData, "MSCI Europe");
	Orbit *msceurOrbit2 = new Orbit(msceurMarketData, "MSCI Europe");
	IndustryOrbit *msceurIndustryOrbit2 = new IndustryOrbit(msceurMarketData, "MSCI Europe");
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
	
	currentScreen = mscwxlGeo;
	
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
	currentScreen->setDashboard(mDashboard);
	for (Screen* currentIter = currentScreen->mNextScreen; currentIter != currentScreen; currentIter = currentIter->mNextScreen) {
		currentIter->setDashboard(mDashboard);
	}
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
