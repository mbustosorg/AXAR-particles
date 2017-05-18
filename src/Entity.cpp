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

#include "Entity.hpp"
#include "SystemConfig.h"
#include "cinder/Rand.h"

Entity::Entity(string symbol, string name, string sector, string industry, string headquarters, double latitude, double longitude) {
	mSymbol = symbol;
	mName = name;
	mSector = sector;
	mIndustry = industry;
	mHeadquarters = headquarters;
	mLatitude = latitude;
	mLongitude = longitude;
	float randLatitude = Rand::randFloat(mLatitude - 0.5, mLatitude + 0.5) * M_PI / 180.0;
	float randLongitude = Rand::randFloat(mLongitude - 0.5, mLongitude + 0.5) * M_PI / 180.0;
	float x = -SPHERE_RADIUS * cos(randLatitude) * cos(randLongitude);
	float y = SPHERE_RADIUS * sin(randLatitude);
	float z = SPHERE_RADIUS * cos(randLatitude) * sin(randLongitude);
	sphericalLocation = vec3(x, y, z);

	mColor = Colors[SectorIndices[mSector]];
}

Entity::~Entity() {
	
}

void Entity::updateMarketData(string lastTradeDate, double lastTrade, double divYield, double peRatio) {
	mLastTradeDate = lastTradeDate;
	mLastTrade = lastTrade;
	mDivYield = divYield;
	mPeRatio = peRatio;
}

void Entity::updateParticle(Particle particle) {
	mParticle = particle;
}
