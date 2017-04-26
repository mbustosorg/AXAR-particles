//
//  Camera.cpp
//  openGlTest
//
//  Created by Mauricio Bustos on 3/15/17.
//
//

#include "RCamera.hpp"
#include "cinder/Easing.h"

RCamera::RCamera() {
	
}

void RCamera::trigger() {
	cameraMode++;
	cameraMode = cameraMode % 5;
	cameraModeBase = getElapsedSeconds();
}

void RCamera::update() {
	cameraTick++;
	cameraTick++;
	cameraTick = cameraTick % 720;
	float x = DEFAULT_DISTANCE * sin(cameraTick / 360.0 * M_PI);
	//float y = DEFAULT_DISTANCE * cos(cameraTick / 180.0 * M_PI);
	float z = DEFAULT_DISTANCE * cos(cameraTick / 360.0 * M_PI);
	mEye.x = x;
	float difference = cameraMode * 100.0 - (cameraMode - 1) * 100.0;
	if ((getElapsedSeconds() - cameraModeBase) / 10.0f < 1.0f) {
		mEye.y = easeInOutCubic((getElapsedSeconds() - cameraModeBase) / 10.0f) * 2000.0;
	}
	mEye.z = z;
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 5.0f, 20000.0f );
	mCam.lookAt( mEye, mCenter, mUp );
}


