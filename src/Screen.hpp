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

using namespace ci;
using namespace ci::app;
using namespace std;

class Screen {
	
public:
	
	Screen();
	~Screen();
	//Screen(Screen *nextScreen);
	
	virtual void setup();
	virtual void update();
	virtual void draw();
	virtual void setCamera(RCamera* camera);

protected:

	RCamera* mCam;
	
	vector<Particle> particles;
	vector<Particle> particleHeads;
	
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
	
	Particle tempData[PARTICLE_VECTOR_LENGTH];
	Particle tempHeadData[PARTICLE_HEAD_VECTOR_LENGTH];

	void setupBuffers(gl::VaoRef* vaos, gl::VboRef* vbos, vector<Particle> particles);

private:
	
	void performProgramUpdate(gl::GlslProgRef mUpdateProg, gl::VboRef mBuffer, gl::VaoRef mAttributes, int drawType, int count);
	void render(gl::GlslProgRef mRenderProg, gl::VaoRef mAttributes, int drawType, int count);
	void updateHeadParticle(int index, int total, Particle* current,
							float theta, float theta_offset,
							float phi, float phi_offset,
							float radius, float radius_offset,
							float rotZ, float rotSpeed);
	
};

#endif