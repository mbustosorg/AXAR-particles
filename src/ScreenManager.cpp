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
#include "Poco/SortedDirectoryIterator.h"
#include "Poco/JSON/Parser.h"
#include "Poco/FileStream.h"

using namespace ci::app;

ScreenManager::ScreenManager() {
	
	std::string fileRoot = string(mFileRoot);
	Poco::SortedDirectoryIterator it(fileRoot);
	Poco::SortedDirectoryIterator end;
	string latestDataDirectory = "";
	while (it != end)
	{
		if (it->isDirectory()) latestDataDirectory = it.name();
		++it;
	}
	
	Poco::Path p(true);
	p.assign(mFileRoot);
	p.pushDirectory(latestDataDirectory);
	p.setFileName("metadata.json");
	Poco::File json(p.toString());
	Poco::Dynamic::Var result;
	Poco::JSON::Parser parser;
	Poco::FileInputStream x(p.toString());
	result = parser.parse(x);

	p.setFileName("");
	Poco::DirectoryIterator files(p);
	
	// For each benchmark, create two loops from GEO->ORBIT->INDUSTRY_ORBIT that feeds into the next benchmark.
	// Sew the last INDUSTRY_ORBIT into the first GEO
	
	Orbit *firstOrbit = NULL;
	Orbit *lastOrbit = NULL;
	IndustryOrbit *lastIndustryOrbit = NULL;

	while (files != end)
	{
		if (files.name().find("json")) {

			Poco::JSON::Array::Ptr children = result.extract<Poco::JSON::Array::Ptr>();
			for (int i = 0; i < children->size(); i++) {
				Poco::JSON::Object::Ptr object = children->getObject(i);
				string code = object->getValue<string>("BENCHMARK_CODE");
				string name = object->getValue<string>("BENCHMARK_NAME");
				
				FinancialData *marketData = new FinancialData(code, name, latestDataDirectory);

				Geographic *geo = new Geographic(marketData, name);
				Orbit *orbit = new Orbit(marketData, name);
				IndustryOrbit *industryOrbit = new IndustryOrbit(marketData, name);
				industryOrbit->setSectorWeights(&(marketData->mSectorWeights));
				
				if (lastIndustryOrbit != NULL) {
					lastIndustryOrbit->setOrder(lastOrbit, geo);
					geo->setOrder(lastIndustryOrbit, orbit);
				}

				Geographic *geo2 = new Geographic(marketData, name);
				lastOrbit = new Orbit(marketData, name);
				lastIndustryOrbit = new IndustryOrbit(marketData, name);
				lastIndustryOrbit->setSectorWeights(&(marketData->mSectorWeights));
				
				if (currentScreen == NULL) currentScreen = geo;
				if (firstOrbit == NULL) firstOrbit = orbit;
				
				orbit->setOrder(geo, industryOrbit);
				industryOrbit->setOrder(orbit, geo2);
				geo2->setOrder(industryOrbit, lastOrbit);
				lastOrbit->setOrder(geo2, lastIndustryOrbit);
			}
		}
		++files;
	}
	lastIndustryOrbit->setOrder(lastOrbit, currentScreen);
	currentScreen->setOrder(lastIndustryOrbit, firstOrbit);

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
