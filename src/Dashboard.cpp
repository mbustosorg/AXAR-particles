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

#include "Dashboard.hpp"
#include "cinder/gl/gl.h"

Dashboard::Dashboard(RCamera *cam) {
	mCam = cam;
	
	for (int i = 1; i < FontSizes; i++) {
		mFont[i] = Font("Helvetica Neue", (float) i);
	}
	
}

void Dashboard::displayMessage(string message, float x, float y, float fontSize, Color color) {
	
	gl::pushModelMatrix();
	
	vec3 eye = mCam->mEye;
	
	// Compute elevation angle
	vec3 hTarget = vec3(eye.x, 0.0f, eye.z);
	double eDotproduct = dot(eye, hTarget) / (length(eye) * length(hTarget));
	double eRotation = acos(eDotproduct);

	// Move to eye
	gl::translate(vec3(eye.x, eye.y, eye.z) * 0.5f / (float) cos(eRotation));
	
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

	TextLayout simple;
	//simple.setColor(color);
	gl::color(color);
	
	//simple.clear(ColorA(0.1f,0.1f,0.1f,0.7f));
	simple.setColor( Color( 0.9f, 0.9f, 0.9f ) );
	simple.setFont(Font("Helvetica Neue", fontSize));
	//simple.setFont(mFont[(int) fontSize]);
	simple.addLine(message);
	//simple.addLine("Rot: " + to_string(eRotation));
	//simple.addLine("Dot: " + to_string(eDotproduct));
	//simple.addLine("EX: " + to_string(mCam->mEye.x));
	//simple.addLine("EY: " + to_string(mCam->mEye.y));
	//simple.addLine("EZ: " + to_string(mCam->mEye.z));
	gl::Texture2dRef mSimpleTexture = gl::Texture2d::create(simple.render(true, false));
	
	//gl::drawSolidRect(Rectf(50.0f, 200.0f, -50.0f, -200.0f));
	
	gl::draw(mSimpleTexture, vec2(x, y));
	
	//gl::drawStrokeCh drawString(message, vec2(0.0f, 0.0f), Color::white(), mFont);
	
	gl::popModelMatrix();
	
}
