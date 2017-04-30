//
//  Screen.cpp
//  AXAR_ParticleApp
//
//  Created by Mauricio Bustos on 4/24/17.
//
//

#include "Screen.hpp"

Screen::Screen() {
	
}

Screen::~Screen() {
	
}

void Screen::setup() {
	
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
		gl::enableVertexAttribArray(12);
		gl::enableVertexAttribArray(13);
		
		gl::vertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, pos));
		gl::vertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, color));
		gl::vertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, ppos));
		gl::vertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, home));
		gl::vertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, damping));
		gl::vertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, index));
		
		gl::vertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, translation));
		gl::vertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, translation) + 4 * sizeof(GLfloat)));
		gl::vertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, translation) + 8 * sizeof(GLfloat)));
		gl::vertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, translation) + 12 * sizeof(GLfloat)));
		
		gl::vertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, rotation));
		gl::vertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, rotation) + 4 * sizeof(GLfloat)));
		gl::vertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, rotation) + 8 * sizeof(GLfloat)));
		gl::vertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(offsetof(Particle, rotation) + 12 * sizeof(GLfloat)));
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
	gl::drawArrays(drawType, 0, count);
	gl::endTransformFeedback();
}

void Screen::update() {
	performProgramUpdate(mParticleUpdateProg, mParticleBuffer[mDestinationIndex], mAttributes[mSourceIndex], GL_LINES, NUM_LINES * 2);
	performProgramUpdate(mParticleHeadUpdateProg, mParticleHeadBuffer[mDestinationIndex], mAttributesHead[mSourceIndex], GL_TRIANGLES, NUM_TRIANGLES * 3);
	
	// Swap source and destination for next loop
	std::swap(mSourceIndex, mDestinationIndex);
}

void Screen::draw() {
	
	gl::clear(Color(0, 0, 0));
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	render(mParticleRenderProg, mAttributes[mSourceIndex], GL_LINES, NUM_LINES * 2);
	render(mParticleHeadRenderProg, mAttributesHead[mSourceIndex], GL_TRIANGLES, NUM_TRIANGLES * 3);
	
}

