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
#include "spdlog/spdlog.h"


using namespace ci::app;

RCamera::RCamera() {
	double x = DEFAULT_DISTANCE * sin(cameraTick / 360.0 * M_PI);
	//double y = DEFAULT_DISTANCE * cos(cameraTick / 180.0 * M_PI);
	double z = DEFAULT_DISTANCE * cos(cameraTick / 360.0 * M_PI);
	mEye = vec3(x, 2000.0f, z);
	mCurrentEye = vec3(x, 2000.0f, z);
}

void RCamera::trigger() {
	cameraMode++;
	cameraMode = cameraMode % 5;
	cameraModeBase = getElapsedSeconds();
}

void RCamera::update() {
	
	mCam.setPerspective(60.0f, getWindowAspectRatio(), 5.0f, 20000.0f);

	cameraTick -= 0.5f;
	if (cameraTick > 720.0f || cameraTick < -720.0f) cameraTick = 0.0f;

	double x = DEFAULT_DISTANCE * sin(cameraTick / 360.0 * M_PI);
	//double y = DEFAULT_DISTANCE * cos(cameraTick / 180.0 * M_PI);
	double z = DEFAULT_DISTANCE * cos(cameraTick / 360.0 * M_PI);
	mEye = vec3(x, 2000.0f, z);

	double delta = (getElapsedSeconds() - mTargetSetTime);
	double deltaFactor = 5.0f;
	if (mTarget) {
		if (delta / deltaFactor < 1.0f) {
			mCurrentEye = mEye + (*mTarget * 2.0f - mEye) * easeInOutCubic(delta / deltaFactor);
		} else {
			mCurrentEye = *mTarget * 2.0f;
		}
	} else {
		if (delta / deltaFactor < 1.0f) {
			mCurrentEye = mCurrentEye + (mEye - mCurrentEye) * easeInOutCubic(delta / deltaFactor);
		} else {
			mCurrentEye = mEye;
		}
	}
	mCam.lookAt(mCurrentEye, mCenter, mUp);
}

void RCamera::focusOn(vec3* target) {
	
	if (target) spdlog::get("particleApp")->info("focusOn");
	else spdlog::get("particleApp")->info("focusOf");

	mTargetSetTime = getElapsedSeconds();
	mTarget = target;
	
}

