//
//  Dashboard.hpp
//  openGlTest
//
//  Created by Mauricio Bustos on 3/24/17.
//
//

#ifndef Dashboard_hpp
#define Dashboard_hpp

#include <stdio.h>
#include "RCamera.hpp"

class Dashboard {

public:
	
	Dashboard(RCamera *cam);
	
	void displayMessage(string message, float x, float y);
	
private:
	
	RCamera *mCam;
	
	Font mFont = Font("Helvetica Neue", 52.0f);
	
};

#endif /* Dashboard_hpp */
