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

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <string>
#include "cinder/app/App.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define DEFAULT_DISTANCE 4500.0

class RCamera {
	
public:
	
	RCamera();
	
	void trigger();
	void focusOn(vec3* target, Color* color);
	void update();
	
	CameraPersp mCam;
	vec3 mEye = vec3(0.0f, 200.0f, 0.0f);
	vec3 mCurrentEye = vec3(0.0, 0.0, 0.0);
	vec3 mCenter = vec3(0.0, 0.0, 0.0);
	vec3 mUp = vec3(0.0f, 1.0f, 0.0f);
	float cameraTick = 0.0f;
	
	vec3* mTarget = NULL;
	Color* mTargetColor = NULL;
	float mEase = 0.0f;
	
private:
	
	int cameraMode = 0;
	double cameraModeBase = 0;
	
	double mTargetSetTime = 0.0;
	
};
#endif
