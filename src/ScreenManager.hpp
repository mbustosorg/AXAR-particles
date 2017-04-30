//
//  ScreenManager.hpp
//  AXAR_ParticleApp
//
//  Created by Mauricio Bustos on 4/28/17.
//
//

#ifndef ScreenManager_hpp
#define ScreenManager_hpp

#include <stdio.h>
#include "Orbit.hpp"

class ScreenManager {
	
public:
	
	ScreenManager();
	
	void update();
	void draw();
	
private:
	
	Orbit orbit;
	
};

#endif
