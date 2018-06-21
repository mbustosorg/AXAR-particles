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

#include <algorithm>
#include "Dashboard.hpp"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBAse.h"
#include "SystemConfig.h"

Dashboard::Dashboard(RCamera *cam) {
	mCam = cam;
	for (int i = 1; i < FontSizes; i++) {
		mFont[i] = Font("Courier New Bold", (float) i);
	}
	axaLogo = gl::Texture2d::create(loadImage(loadAsset("smallAXA2.png")));
	
	mMessageStartTime = 0.0f;
	mLastMessage = "";
	mCursorOn = false;
}

void Dashboard::displayMessage(string message, float x, float y, float fontSize, Color color, bool inEye) {
	
	gl::pushModelMatrix();
	
	gl::color(color);

	vec3 eye = mCam->mCurrentEye;
	float prop = 1.0f;
	if (mCam->mTarget) {
		prop = length(mCam->mCurrentEye) / mDefaultEyeDistance;
	}
	
	// Compute elevation angle
	vec3 hTarget = vec3(eye.x, 0.0f, eye.z);
	double eDotproduct = dot(eye, hTarget) / (length(eye) * length(hTarget));
	double eRotation = acos(eDotproduct);

	// Move to eye
	gl::translate(vec3(eye.x, eye.y, eye.z) * 0.5f * prop / (float) cos(eRotation));
	gl::rotate(M_PI, vec3(0.0f, 0.0f, 1.0f));
	gl::rotate(eRotation, vec3(eye.z, 0.0f, eye.x));
	
	// Compute rotation angle from start point which is Z axis
	vec3 start = vec3(0.0f, 0.0f, -1.0f);
	vec3 target = vec3(eye.x, 0.0f, eye.z);
	double dotproduct = dot(start, target) / (length(start) * length(target));
	double rotation = acos(dotproduct);
	if (eye.x > 0.0) {
		if (eye.z > 0.0) gl::rotate(rotation, mCam->mUp);
		else gl::rotate(2 * M_PI + rotation, mCam->mUp);
	} else {
		if (eye.z > 0.0) gl::rotate(-rotation, mCam->mUp);
		else gl::rotate(2 * M_PI - rotation, mCam->mUp);
	}
	if (!inEye) {
		gl::draw(axaLogo, vec2(x + 2900, y));
	}
	
	if (message != mLastMessage) {
		mMessageStartTime = getElapsedSeconds();
		mLastMessage = message;
	}
	
	float time = getElapsedSeconds() - mMessageStartTime;
	float fraction = time - int(time);
	bool bottomHalf = fraction < 0.25 || (fraction > 0.5 && fraction < 0.75);
	string cursor = "_";
	if (bottomHalf) cursor = "";
	
	string portion = message.substr(0, std::min(message.length(), size_t(time * 8.0f))) + cursor;
	TextLayout simple;
	simple.setColor( Color( 0.9f, 0.9f, 0.9f ) );
	simple.setFont(mFont[int(fontSize)]);
	size_t pos = portion.find(mDelimiter);
	if (pos != string::npos) {
		simple.addLine(portion.substr(0, pos));
		simple.addLine(portion.substr(pos + 1, portion.length() - 1));
	} else {
		simple.addLine(portion);
	}
	gl::Texture2dRef mSimpleTexture = gl::Texture2d::create(simple.render(true, false));
	gl::draw(mSimpleTexture, vec2(x * prop, y * prop));
	
	gl::popModelMatrix();
	
}
