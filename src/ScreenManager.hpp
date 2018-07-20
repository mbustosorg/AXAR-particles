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

#ifndef ScreenManager_hpp
#define ScreenManager_hpp

#include <stdio.h>
#include <list>
#include "Dashboard.hpp"
#include "FinancialData.hpp"
#include "Orbit.hpp"
#include "Geographic.hpp"
#include "IndustryOrbit.hpp"

class ScreenManager {
	
public:
	
	ScreenManager();
	
	void update();
	void draw();
	void setCamera(RCamera* camera);
	void setDashboard(Dashboard* dashboard);
private:
	
	RCamera* mCam;
	Dashboard* mDashboard;
	unordered_map<string, Entity*> mEntities;
	
	Screen* currentScreen;
	double timeStamp;
	char* mFileRoot = std::getenv("DATA_ROOT");

};

#endif
