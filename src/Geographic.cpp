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

#include "Geographic.hpp"
#include "cinder/Easing.h"
#include "cinder/app/AppBAse.h"

using namespace ci::app;

Geographic::Geographic() {
	screenTime = 25.0f;
}

void Geographic::setup() {
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef	shader = gl::getStockShader(lambert);

	mParticles.assign(Num_Lines * 2, Particle());
	mParticleHeads.assign(Num_Triangles * 3, Particle());
	
	int index = 0;
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		i->second->mParticleIndex = index;
		auto sphere = geom::Sphere().subdivisions(60).radius(20.0f);
		mShapes.push_back(gl::Batch::create(sphere, shader));
		index++;
	}

	restartTime = fmod(getElapsedFrames() / 60.0f, 1000.f);
	loadUpdateProgram("industryOrbit.vs");
}

void Geographic::restart() {
	vector<Particle> *tempVector = mPrevScreen->currentPositions();
	mParticles.swap(*tempVector);
	vector<Particle>().swap(*tempVector);
	delete(tempVector);
	
	restartTime = fmod(getElapsedFrames() / 60.0f, 1000.f);
}

void Geographic::update() {
	
}

void Geographic::draw()
{
	gl::clear();
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	gl::setMatrices(mCam->mCam);
	
	const float rotationTime = 1.5f;
	const float rotationOffset = 0.1f;
	
	float currentTime = fmod(getElapsedFrames() / 60.0f, 1000.f);
	
	list<gl::BatchRef>::iterator shapes = mShapes.begin();
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		
		Entity* entity = i->second;
		auto &p = mParticles.at(entity->mParticleIndex * TRAIL_LENGTH * 2);
		
		float rotation = 0;
		float startTime = entity->mParticleIndex * rotationOffset;
		if (currentTime > (startTime + restartTime)) rotation = (currentTime - (startTime + restartTime)) / rotationTime;
		if (rotation > 1.0) rotation = 1.0;

		float angle = easeOutBack(rotation);
		gl::ScopedModelMatrix scpModelMatrix;
		vec3 translation = entity->mSphericalLocation - vec3(p.pos);
		gl::translate(vec3(p.pos) + translation * angle);
		gl::color(entity->mColor);

		(*shapes)->draw();
		++shapes;
	}
	
	//Screen::draw();
}
