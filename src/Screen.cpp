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

#include "Screen.hpp"

Screen::Screen() {
	
}

Screen::~Screen() {
	
}

void Screen::setup() {
	
}

void Screen::setCamera(RCamera *camera) {
	mCam = camera;
}

void Screen::setEntities(unordered_map<string, Entity*> entities) {
	mEntities = entities;
	Num_Particles = mEntities.size();
	Num_Lines = mEntities.size() * TRAIL_LENGTH;
	Num_Triangles = mEntities.size() * 4 * 2;
	Particle_Vector_Length = Num_Lines * 2;
	Particle_Head_Vector_Length = Num_Triangles * 3 * 2;
}

void Screen::render(gl::GlslProgRef mRenderProg, gl::VaoRef mAttributes, int drawType, int count) {
	gl::ScopedGlslProg render(mRenderProg);
	gl::ScopedVao vao(mAttributes);
	gl::context()->setDefaultShaderVars();
	gl::drawArrays(drawType, 0, count);
}

void Screen::setupBuffers(gl::VaoRef* vaos, gl::VboRef* vbos, vector<Particle> particleData) {
	
	// Create particle buffers on GPU and copy data into the first buffer.
	// Mark as static since we only write from the CPU once.
	vbos[mSourceIndex] = gl::Vbo::create(GL_ARRAY_BUFFER, particleData.size() * sizeof(Particle), particleData.data(), GL_DYNAMIC_DRAW);
	vbos[mDestinationIndex] = gl::Vbo::create(GL_ARRAY_BUFFER, particleData.size() * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);
	
	for(int i = 0; i < 2; ++i)
	{	// Describe the particle layout for OpenGL.
		vaos[i] = gl::Vao::create();
		gl::ScopedVao vao(vaos[i]);
		
		// Define attributes as offsets into the bound particle buffer
		gl::ScopedBuffer buffer(vbos[i]);
		gl::enableVertexAttribArray(0);
		gl::enableVertexAttribArray(1);
		gl::enableVertexAttribArray(2);
		gl::enableVertexAttribArray(3);
		gl::enableVertexAttribArray(4);
		gl::enableVertexAttribArray(5);
		gl::enableVertexAttribArray(6);
		gl::enableVertexAttribArray(7);
		gl::enableVertexAttribArray(8);
		gl::enableVertexAttribArray(9);
		gl::enableVertexAttribArray(10);
		gl::enableVertexAttribArray(11);
		
		gl::vertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, pos));
		gl::vertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, color));
		gl::vertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, index));
		gl::vertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, delay));
		
		gl::vertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, translation));
		gl::vertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, translation) + 4 * sizeof(GLfloat)));
		gl::vertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, translation) + 8 * sizeof(GLfloat)));
		gl::vertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, translation) + 12 * sizeof(GLfloat)));
		
		gl::vertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, rotation));
		gl::vertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, rotation) + 4 * sizeof(GLfloat)));
		gl::vertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, rotation) + 8 * sizeof(GLfloat)));
		gl::vertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, rotation) + 12 * sizeof(GLfloat)));
	}
}

void Screen::performProgramUpdate(gl::GlslProgRef mUpdateProg, gl::VboRef mBuffer, gl::VaoRef mAttributes, int drawType, int count) {
	// Update particles on the GPU
	gl::ScopedGlslProg prog(mUpdateProg);
	gl::ScopedState rasterizer(GL_RASTERIZER_DISCARD, true);	// turn off fragment stage
	
	// Bind the source data (Attributes refer to specific buffers).
	gl::ScopedVao source(mAttributes);
	// Bind destination as buffer base.
	gl::bindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mBuffer);
	gl::beginTransformFeedback(drawType);
	// Draw source into destination, performing our vertex transformations.
	gl::drawArrays(drawType, 0, (int)count);
	gl::endTransformFeedback();
}

void Screen::update() {
	performProgramUpdate(mParticleUpdateProg, mParticleBuffer[mDestinationIndex], mAttributes[mSourceIndex], GL_LINES, (int)Num_Lines * 2);
	performProgramUpdate(mParticleHeadUpdateProg, mParticleHeadBuffer[mDestinationIndex], mAttributesHead[mSourceIndex], GL_TRIANGLES, (int)Num_Triangles * 3);
	
	// Swap source and destination for next loop
	std::swap(mSourceIndex, mDestinationIndex);
}

void Screen::draw() {
	
	Particle tempData[particles.size()];
	mParticleBuffer[mDestinationIndex]->bind();
	size_t bufferSize = mParticleBuffer[mDestinationIndex]->getSize();
	mParticleBuffer[mDestinationIndex]->getBufferSubData(0, bufferSize, &tempData);
	for (int i = 0; i < particles.size(); i++) {
	 particles[i] = tempData[i];
	}
	particles.at(0).pos.x = particles.at(0).pos.x + 1000.0;
	mParticleBuffer[mDestinationIndex]->bind();
	
	gl::clear(Color(0, 0, 0));
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	render(mParticleRenderProg, mAttributes[mSourceIndex], GL_LINES, (int)Num_Lines * 2);
	render(mParticleHeadRenderProg, mAttributesHead[mSourceIndex], GL_TRIANGLES, (int)Num_Triangles * 3);
	
}

