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

#ifndef Geographic_hpp
#define Geographic_hpp

#include <stdio.h>
#include "Screen.hpp"
#include "Entity.hpp"
#include "Dashboard.hpp"
#include "FinancialData.hpp"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include <list>

class Geographic : public Screen {
	
public:
	Geographic(FinancialData* financialData, string universe);
	
	void setup();
	void restart();
	void update();
	void draw();
	void displayMessage(Dashboard *dashboard);
	
private:
	
	void updateHeadParticle(int index, int total, Particle* current,
							float theta, float thetaOffset,
							float phi, float phiOffset,
							float radius, float radiusOffset,
							float rotZ, float rotSpeed);
	int sphereCount = 1;
	double maxWeight = 0.0;
	bool mBorrowParticles = false;
	double mUniverseCap = 0.0;
	double mCountryCount = 0.0;
	
	const float rotationTime = 4.5f;
	const float rotationOffset = 0.005f;

};

#endif
