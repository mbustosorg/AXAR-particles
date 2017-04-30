//
//  ScreenManager.cpp
//  AXAR_ParticleApp
//
//  Created by Mauricio Bustos on 4/28/17.
//
//

#include "ScreenManager.hpp"

ScreenManager::ScreenManager() {
	orbit.setup();
}

void ScreenManager::update() {
	orbit.update();
}

void ScreenManager::draw() {
	orbit.draw();
}
