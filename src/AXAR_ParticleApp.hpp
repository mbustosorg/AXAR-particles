//
//  AXAR_ParticleApp.h
//  AXAR_ParticleApp
//
//  Created by Mauricio Bustos on 4/28/17.
//
//

#ifndef AXAR_ParticleApp_hpp
#define AXAR_ParticleApp_hpp

#include "RCamera.hpp"
#include "Dashboard.hpp"
#include "ScreenManager.hpp"

class AXAR_ParticleApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
	
	// Mouse state suitable for passing as uniforms to update program
	bool			mMouseDown = false;
	float			mMouseForce = 0.0f;
	vec3			mMousePos = vec3(0, 0, 0);
	
	RCamera			mCamera;
	Dashboard		*mDashboard = new Dashboard(&mCamera);
	ScreenManager   screenManager;
	
};

#endif
