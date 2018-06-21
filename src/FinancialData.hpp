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

#ifndef FinancialData_hpp
#define FinancialData_hpp

#include "Entity.hpp"

#include "Poco/Dynamic/Var.h"

#include <stdio.h>
#include <string.h>

using namespace std;

const string GooglePlacesApiKey = std::getenv("GOOGLE_PLACES_API_KEY"); 

class FinancialData {

public:
	FinancialData();
	FinancialData(string benchmark, string descriptiveName, string date);
	
	void updateLatLon(Entity *entity);
	
	unordered_map<string, Entity*> mEntities;
	unordered_map<int, double> mSectorWeights;
	unordered_map<string, double> mCountryCounts;
	double mTotalCap = 0.0;
	
private:
	
	void writeJson();
	Poco::Dynamic::Var retrieveQuery(string query);
	string processedFile();
	string unprocessedFile();
	string mBenchmark;
	string mDate;
	string mDescriptiveName;
	
};

#endif
