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


#ifndef IndustryOrbit_hpp
#define IndustryOrbit_hpp

#include <stdio.h>
#include "Screen.hpp"
#include "FinancialData.hpp"

class IndustryOrbit : public Screen {
	
public:
	IndustryOrbit(FinancialData* financialData, string universe);
	
	void setup();
	void restart();
	void update();
	void displayMessage(Dashboard *dashboard);
	void setSectorWeights(unordered_map<int, double> *sectorWeights);
	
private:
	
	void updateHeadParticle(Entity* entity, int index, Particle* current,
							vec3 offsetVector, vec3 rotationAxis,
							float rotZ, float rotSpeed, vec4 center);
	
	unordered_map<int, double> *mSectorWeights;
	float mTransitionFactor = 0.0;
	
};

#endif
