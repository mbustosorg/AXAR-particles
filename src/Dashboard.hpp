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

#ifndef Dashboard_hpp
#define Dashboard_hpp

#include <stdio.h>
#include "RCamera.hpp"
#include "cinder/Font.h"

const int FontSizes = 300;

class Dashboard {

public:
	
	Dashboard(RCamera *cam);
	
	void displayMessage(string message, float x, float y, float fontSize, Color color, bool inEye);
	
private:
	
	bool mCursorOn;
	double mMessageStartTime;
	string mLastMessage;
	RCamera *mCam;
	Font mFont[FontSizes];
	gl::Texture2dRef axaLogo;
	
};

#endif
