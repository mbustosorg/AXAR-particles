//
//  Camera.hpp
//  openGlTest
//
//  Created by Mauricio Bustos on 3/15/17.
//
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <string>
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define DEFAULT_DISTANCE 4500.0

class RCamera {
	
public:
	
	RCamera();
	
	void trigger();
	void update();
	
	CameraPersp mCam;
	vec3 mEye = vec3( 0.0f, 200.0f, 0.0f );
	vec3 mCenter = vec3 (0.0, 0.0, 0.0);
	vec3 mUp = vec3( 0.0f, 1.0f, 0.0f);
	int cameraTick = 0;
	
private:
	
	int cameraMode = 0;
	float cameraModeBase = 0;
	
};
#endif /* Camera_hpp */
