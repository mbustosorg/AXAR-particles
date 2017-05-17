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
#include "Orbit.hpp"
#include "Geographic.hpp"

class ScreenManager {
	
public:
	
	ScreenManager();
	
	void update();
	void draw();
	void setCamera(RCamera* camera);
	
private:
	
	RCamera* mCam;
	list<Entity>* mEntities;
	
	list<Screen*> screens;
	list<Screen*>::iterator currentScreen;
	double timeStamp;
	
	Orbit orbit;
	Geographic geo;
	
};

#endif
