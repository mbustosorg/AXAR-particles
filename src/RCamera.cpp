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

#include "RCamera.hpp"
#include "cinder/Easing.h"
#include "cinder/app/AppBase.h"

using namespace ci::app;

RCamera::RCamera() {
	
}

void RCamera::trigger() {
	cameraMode++;
	cameraMode = cameraMode % 5;
	cameraModeBase = getElapsedSeconds();
}

void RCamera::update() {
	cameraTick--;
	cameraTick = cameraTick % 720;
	double x = DEFAULT_DISTANCE * sin(cameraTick / 360.0 * M_PI);
	//float y = DEFAULT_DISTANCE * cos(cameraTick / 180.0 * M_PI);
	double z = DEFAULT_DISTANCE * cos(cameraTick / 360.0 * M_PI);
	mEye.x = x;
	double difference = cameraMode * 100.0 - (cameraMode - 1) * 100.0;
	if ((getElapsedSeconds() - cameraModeBase) / 10.0f < 1.0f) {
		mEye.y = easeInOutCubic((getElapsedSeconds() - cameraModeBase) / 10.0f) * 2000.0;
	}
	mEye.z = z;
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 5.0f, 20000.0f );
	mCam.lookAt( mEye, mCenter, mUp );
}


