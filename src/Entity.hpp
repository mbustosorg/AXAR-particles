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

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "Particle.h"
#include <string.h>

using namespace std;

class Entity {

public:
	
	Entity(string symbol, string name, string sector, string industry, string headquarters, double latitude, double longitude);
	~Entity();
	
	void updateMarketData(string lastTradeDate, double lastTrade, double divYield, double peRatio, double capitalization);
	void updateParticle(Particle particle);
	vec3 sphericalLocation;
	
	Particle mParticle;
	string mExchange;
	string mName;
	string mIndustry;
	string mSymbol;
	string mSector;
	string mHeadquarters;

	Color mColor;
	double mLatitude;
	double mLongitude;
	double mCapitalization;
	
private:
	
	string mLastTradeDate;
	double mLastTrade;
	double mDivYield;
	double mPeRatio;
	
};

#endif
