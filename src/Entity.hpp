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
	
	Entity(string symbol, string name, string sector, string similarsGroup, string headquarterCountry, int index);
	~Entity();
	
	void updateMarketData(string lastTradeDate, double lastTrade, double divYield, double peRatio, double capitalization);
	void updateLatitudeLongitude(double latitude, double longitude);
	void setPosition(vec3 position);
	
	int mParticleIndex;
	string mName;
	string mSimilarsGroup;
	string mSymbol;
	string mSector;
	string mHeadquarterCountryISO;
	string mHeadquarterCountryName;
	string mAddress;
	string mCity;
	string mPostalCode;
	string mCompanyXrf;
	string mCompanyZone;
	string mListingXrf;
	string mListingZone;
	string mExchange;
	string mTicker;
	string mSedol;
	
	Color mColor;
	double mLatitude;
	double mLongitude;
	double mRandLatitude;
	double mRandLongitude;
	double mUsdCapitalization;
	double mShares;
	double mWeight;
	vec3 mSphericalLocation;
	vec3 mPosition;
	
private:
	
	string mLastTradeDate;
	double mLastTrade;
	double mDivYield;
	double mPeRatio;
	
};

#endif
