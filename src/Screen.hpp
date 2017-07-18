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

#ifndef Screen_hpp
#define Screen_hpp

#include <stdio.h>
#include "cinder/gl/gl.h"
#include "Particle.h"
#include "SystemConfig.h"
#include "RCamera.hpp"
#include "Entity.hpp"
#include "Dashboard.hpp"

using namespace ci;
using namespace std;

class Screen {
	
public:
	
	Screen();
	~Screen();
	
	virtual void setOrder(Screen *prevScreen, Screen *nextScreen);
	virtual void setup();
	virtual void update();
	virtual void restart();
	virtual void draw();
	virtual void setCamera(RCamera* camera);
	virtual void setEntities(unordered_map<string, Entity*> entities);
	virtual void displayMessage(Dashboard *dashboard);
	
	void setScreenStartTime(float startTime);
	
	vector<Particle>* currentPositions();
	Screen* mPrevScreen;
	Screen* mNextScreen;

	string mName = "";
	string mUniverse = "";
	
	float screenTime = 2.0; // Number of seconds to play this screen
	vec3 *mTargetLocation; // Target point for camera

protected:
	
	void loadUpdateProgram(string programName);

	RCamera* mCam;
	
	vector<Particle> mParticles;
	vector<Particle> mParticleHeads;
	unordered_map<string, Entity*> mEntities;
	
	gl::GlslProgRef mParticleRenderProg;
	gl::GlslProgRef mParticleUpdateProg;
	gl::GlslProgRef mParticleHeadRenderProg;
	gl::GlslProgRef mParticleHeadUpdateProg;
	
	// Descriptions of particle data layout.
	gl::VaoRef		mAttributes[2];
	// Buffers holding raw particle data on GPU.
	gl::VboRef		mParticleBuffer[2];
	// Descriptions of particle data layout.
	gl::VaoRef		mAttributesHead[2];
	// Buffers holding raw particle data on GPU.
	gl::VboRef		mParticleHeadBuffer[2];
	
	// Current source and destination buffers for transform feedback.
	// Source and destination are swapped each frame after update.
	std::uint32_t	mSourceIndex		= 0;
	std::uint32_t	mDestinationIndex	= 1;
	
	//Particle* tempData;
	//Particle* tempHeadData;

	void setupBuffers(gl::VaoRef* vaos, gl::VboRef* vbos, vector<Particle> *particles);
	
	unsigned long Num_Particles = 0;
	unsigned long Num_Lines = 0;
	unsigned long Num_Triangles = 0;
	unsigned long Particle_Vector_Length = 0;
	unsigned long Particle_Head_Vector_Length = 0;

	gl::BatchRef mShapes[RadiusSteps];
	
	float mScreenStartTime;

private:
	
	void performProgramUpdate(gl::GlslProgRef mUpdateProg, gl::VboRef mBuffer, gl::VaoRef mAttributes, int drawType, int count);
	void render(gl::GlslProgRef mRenderProg, gl::VaoRef mAttributes, int drawType, int count);
	void updateHeadParticle(int index, unsigned long total, Particle* current,
							float theta, float theta_offset,
							float phi, float phi_offset,
							float radius, float radius_offset,
							float rotZ, float rotSpeed);
	
};

#endif
