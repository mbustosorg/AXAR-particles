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

ScreenManager::ScreenManager() {
	
	mEntities = new list<Entity>;
	for (int i = 0; i < NUM_PARTICLES; i++) {
		mEntities->push_back(Entity("test", "test"));
	}
	geo.setEntities(mEntities);
	//orbit.setup();
	geo.setup();
}

void ScreenManager::setCamera(RCamera *camera) {
	mCam = camera;
	orbit.setCamera(mCam);
	geo.setCamera(mCam);
}

void ScreenManager::update() {
	//orbit.update();
	geo.update();
}

void ScreenManager::draw() {
	//orbit.draw();
	geo.draw();
}
