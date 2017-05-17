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

Geographic::Geographic() {
	
}

void Geographic::setEntities(list<Entity> *entities) {
	mEntities = entities;
}

void Geographic::setup() {
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef	shader = gl::getStockShader( lambert );
	
	for (list<Entity>::iterator i = mEntities->begin(); i != mEntities->end(); ++i) {
		auto sphere = geom::Sphere().subdivisions(60).radius(20.0f);
		mShapes.push_back(gl::Batch::create(sphere, shader));
	}
}

void Geographic::update() {
	
}

void Geographic::draw()
{
	gl::clear();
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	gl::setMatrices(mCam->mCam);
	
	const float delay = 0.25f;
	const float rotationTime = 20.5f;
	const float rotationOffset = 0.1f;
	const float totalTime = delay + rotationTime + mEntities->size() * rotationOffset;
	
	float time = fmod(getElapsedFrames() / 30.0f, 1000.f);//totalTime);
	
	list<gl::BatchRef>::iterator shapes = mShapes.begin();
	int index = 0;
	for (list<Entity>::iterator i = mEntities->begin(); i != mEntities->end(); ++i) {
		float rotation = 0;
		float startTime = index * rotationOffset;
		float endTime = startTime + rotationTime;
		if (time > startTime) rotation = (time - startTime) / rotationTime;
		if (rotation > 1.0) rotation = 1.0;
		//if (time > startTime && time < endTime) rotation = (time - startTime) / rotationTime;

		//float angle = easeInOutQuint(rotation);
		//float angle = easeOutBounce(rotation);
		float angle = easeOutBack(rotation);
		gl::ScopedModelMatrix scpModelMatrix;
		gl::translate((*i).sphericalLocation() * angle);
		gl::color(index / float(mEntities->size()), 1 - index / float(mEntities->size()), 1 - index / float(mEntities->size()));
		gl::color(Color(CM_HSV, lmap<float>(index, 0.0f, mEntities->size(), 1.0f, 0.0f), 1.0f, 1.0f));
		(*shapes)->draw();
		++shapes;
		++index;
	}
}
