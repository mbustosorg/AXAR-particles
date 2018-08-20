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

#ifndef AXAR_ParticleApp_hpp
#define AXAR_ParticleApp_hpp

#include "RCamera.hpp"
#include "Dashboard.hpp"
#include "ScreenManager.hpp"
#include "cinder/app/App.h"
#include "spdlog/spdlog.h"

using namespace cinder::app;

class AXAR_ParticleApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void drawAxes();
	
	// Mouse state suitable for passing as uniforms to update program
	bool			mMouseDown = false;
	float			mMouseForce = 0.0f;
	vec3			mMousePos = vec3(0, 0, 0);
	
	RCamera			mCamera;
	Dashboard		*mDashboard = new Dashboard(&mCamera);
	ScreenManager   *mScreenManager;
	
	std::shared_ptr<spdlog::logger> console;
	
};

#endif
