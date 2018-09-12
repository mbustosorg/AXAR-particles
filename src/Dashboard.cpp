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
		mFont[i] = Font("Courier New", (float) i);
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
	
	// Compute elevation angle
	vec3 hTarget = vec3(eye.x, 0.0f, eye.z);
	double elevation = acos(dot(eye, hTarget) / (length(eye) * length(hTarget)));
	// Compute eye offset
	vec3 eyeOffset = eye - ((eye / length(eye)) * 2000.0f);
	// Compute axis rotation
	vec3 start = vec3(1.0f, 0.0f, 0.0f);
	vec3 target = vec3(eye.x, 0.0f, eye.z);
	double dotproduct = dot(start, target) / (length(start) * length(target));
	double rotation = acos(dotproduct);
	if (eye.y < 0.0) elevation = -elevation;
	if (eye.z < 0.0) rotation = -rotation;

	gl::translate(eyeOffset);
	gl::rotate(-M_PI / 2.0 - rotation, vec3(0.0f, 1.0f, 0.0f));
	gl::rotate(M_PI, vec3(0.0f, 0.0f, 1.0f));
	gl::rotate(-elevation, vec3(1.0f, 0.0f, 0.0f));

	if (!inEye) {
		gl::draw(axaLogo, vec2((WINDOW_WIDTH / 4 - axaLogo.get()->getWidth() / 2) / 0.5f, (WINDOW_HEIGHT / 4 - axaLogo.get()->getHeight() / 2) / 0.5f));
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
	gl::draw(mSimpleTexture, vec2((-WINDOW_WIDTH / 4) / 0.5f, (WINDOW_HEIGHT / 4) / 0.5f - 220));

	gl::popModelMatrix();
	
}
