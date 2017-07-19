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
	mTargetLocation = new vec3(0.0f);
}

Screen::~Screen() {
	
}

void Screen::setOrder(Screen* prevScreen, Screen* nextScreen) {
	mPrevScreen = prevScreen;
	mNextScreen = nextScreen;
}

void Screen::setup() {
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef	shader = gl::getStockShader(lambert);
	
	for (int i = 0; i < RadiusSteps; i++) {
		auto sphere = geom::Sphere().subdivisions(15).radius(MinRadius + float(i));
		mShapes[i] = gl::Batch::create(sphere, shader);
	}
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

void Screen::setupBuffers(gl::VaoRef* vaos, gl::VboRef* vbos, vector<Particle> *particleData) {
	
	// Create particle buffers on GPU and copy data into the first buffer.
	// Mark as static since we only write from the CPU once.
	vbos[mSourceIndex] = gl::Vbo::create(GL_ARRAY_BUFFER, particleData->size() * sizeof(Particle), particleData->data(), GL_DYNAMIC_DRAW);
	vbos[mDestinationIndex] = gl::Vbo::create(GL_ARRAY_BUFFER, particleData->size() * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);
	
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
		gl::vertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, startPosition));
		gl::vertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, sphericalPosition));
		gl::vertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, index));
		gl::vertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)offsetof(Particle, delay));
		
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

void Screen::loadUpdateProgram(string programName) {
	mParticleRenderProg = gl::getStockShader(gl::ShaderDef().color());
	mParticleUpdateProg = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset(programName))
											   .feedbackFormat(GL_INTERLEAVED_ATTRIBS)
											   .feedbackVaryings({"position", "color", "startPosition", "sphericalPosition", "index", "delay", "translation", "rotation"})
											   .attribLocation("iPosition", 0)
											   .attribLocation("iColor", 1)
											   .attribLocation("iStartPosition", 2)
											   .attribLocation("iSphericalPosition", 3)
											   .attribLocation("iIndex", 4)
											   .attribLocation("iDelay", 5)
											   .attribLocation("iTranslation", 6)
											   .attribLocation("iRotation", 10)
											   );
	mParticleHeadRenderProg = gl::getStockShader(gl::ShaderDef().color());
	mParticleHeadUpdateProg = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset(programName))
												   .feedbackFormat(GL_INTERLEAVED_ATTRIBS)
												   .feedbackVaryings({"position", "color", "startPosition", "sphericalPosition", "index", "delay", "translation", "rotation"})
												   .attribLocation("iPosition", 0)
												   .attribLocation("iColor", 1)
												   .attribLocation("iStartPosition", 2)
												   .attribLocation("iSphericalPosition", 3)
												   .attribLocation("iIndex", 4)
												   .attribLocation("iDelay", 5)
												   .attribLocation("iTranslation", 6)
												   .attribLocation("iRotation", 10)
												   );
	setupBuffers(mAttributes, mParticleBuffer, &mParticles);
	setupBuffers(mAttributesHead, mParticleHeadBuffer, &mParticleHeads);
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
	
	//vector<Particle>* positions = currentPositions();

	performProgramUpdate(mParticleUpdateProg, mParticleBuffer[mDestinationIndex], mAttributes[mSourceIndex], GL_LINES, (int)Num_Lines * 2);
	performProgramUpdate(mParticleHeadUpdateProg, mParticleHeadBuffer[mDestinationIndex], mAttributesHead[mSourceIndex], GL_TRIANGLES, (int)Num_Triangles * 3);
	
	// Swap source and destination for next loop
	std::swap(mSourceIndex, mDestinationIndex);
}

vector<Particle>* Screen::currentPositions() {
	
	vector<Particle> *tempData = new vector<Particle>();
	tempData->assign(Num_Lines * 2, Particle());
	
	if (mParticleBuffer[mDestinationIndex]) {
		mParticleBuffer[mDestinationIndex]->bind();
		size_t bufferSize = mParticleBuffer[mDestinationIndex]->getSize();
		mParticleBuffer[mDestinationIndex]->getBufferSubData(0, bufferSize, tempData->data());
		for (int i = 0; i < mParticles.size(); i++) {
			Particle currentParticle = mParticles.at(i);
			Particle currentTarget = tempData->at(i);
			currentTarget.pos = currentParticle.pos;
			currentTarget.startPosition = currentParticle.startPosition;
			currentTarget.sphericalPosition = currentParticle.sphericalPosition;
			currentTarget.color = currentParticle.color;
			currentTarget.index = currentParticle.index;
			currentTarget.delay = currentParticle.delay;
			currentTarget.translation = currentParticle.translation;
			currentTarget.rotation = currentParticle.rotation;
		}
	}
	
	return tempData;
}

void Screen::setScreenStartTime(float startTime) {
	mScreenStartTime = startTime;
}

void Screen::displayMessage(Dashboard *dashboard) {
}

void Screen::restart() {
	
}

void Screen::draw() {
	gl::clear(Color(0, 0, 0));
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	render(mParticleRenderProg, mAttributes[mSourceIndex], GL_LINES, (int)Num_Lines * 2);
	render(mParticleHeadRenderProg, mAttributesHead[mSourceIndex], GL_TRIANGLES, (int)Num_Triangles * 3);
	
}

