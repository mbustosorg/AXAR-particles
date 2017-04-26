
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#include "RCamera.hpp"
#include "Dashboard.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

struct Particle
{
	vec4	pos;
	vec4	ppos;
	vec4	home;
	ColorA  color;
	float	damping;
	float	index;
	mat4	translation;
	mat4	rotation;
};

const int NUM_PARTICLES = 201;
const int TRAIL_LENGTH = 150;
const int SPHERE_RADIUS = 2500;
const int NUM_LINES = NUM_PARTICLES * TRAIL_LENGTH;
const int NUM_TRIANGLES = NUM_PARTICLES * 4 * 2;
const float SECTION_ARC_LENGTH = 0.01f;

const float HEAD_DEPTH = 20.0f;
const float HEAD_ARC_WIDTH = 0.01f;

const float ROTATION_SPEED = 0.01f;

class AXAR_ParticleApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
	
	void setupBuffers(gl::VaoRef* vaos, gl::VboRef* vbos, vector<Particle> particles);
	void performProgramUpdate(gl::GlslProgRef mUpdateProg, gl::VboRef mBuffer, gl::VaoRef mAttributes, int drawType, int count);
	void render(gl::GlslProgRef mRenderProg, gl::VaoRef mAttributes, int drawType, int count);
	void updateHeadParticle(int index, int total, Particle* current,
							float theta, float theta_offset,
							float phi, float phi_offset,
							float radius, float radius_offset,
							float rotZ, float rotSpeed);

	private:
	gl::GlslProgRef mParticleRenderProg;
	gl::GlslProgRef mParticleUpdateProg;
	gl::GlslProgRef mParticleHeadRenderProg;
	gl::GlslProgRef mParticleHeadUpdateProg;
	
	// Descriptions of particle data layout.
	gl::VaoRef		mAttributes[2];
	// Buffers holding raw particle data on GPU.
	gl::VboRef		mParticleBuffer[2];
	// Descriptions of particle data layout.
	gl::VaoRef		mHeadAttributes[2];
	// Buffers holding raw particle data on GPU.
	gl::VboRef		mHeadParticleBuffer[2];
	
	// Current source and destination buffers for transform feedback.
	// Source and destination are swapped each frame after update.
	std::uint32_t	mSourceIndex		= 0;
	std::uint32_t	mDestinationIndex	= 1;
	
	// Mouse state suitable for passing as uniforms to update program
	bool			mMouseDown = false;
	float			mMouseForce = 0.0f;
	vec3			mMousePos = vec3(0, 0, 0);
	
	RCamera			mCamera;
	Dashboard		*mDashboard = new Dashboard(&mCamera);
};

void AXAR_ParticleApp::updateHeadParticle(int index, int total, Particle* current,
										  float theta, float thetaOffset,
										  float phi, float phiOffset,
										  float radius, float radiusOffset,
										  float rotZ, float rotSpeed) {
	
	vec4 center = vec4();
	
	float pX = (radius + radiusOffset) * cos(theta + thetaOffset) * sin(phi + phiOffset);  // Red
	float pY = (radius + radiusOffset) * sin(theta + thetaOffset) * sin(phi + phiOffset);  // Green
	float pZ = (radius + radiusOffset) * cos(phi + phiOffset);               // Blue
	
	float rX = radius * cos(theta) * sin(phi);  // Red
	float rY = radius * sin(theta) * sin(phi);  // Green
	float rZ = radius * cos(phi);               // Blue
	
	current->pos = (center + vec4(pX, pY, pZ, 1.0)) * rotate(-rotZ, vec3(rX, rY, 0.0f));
	current->rotation = rotate(rotSpeed, vec3(vec4(rY, -rX, 0.0f, 1.0) * rotate(-rotZ, vec3(rX, rY, 0.0f))));
	current->home = current->pos;
	current->ppos = current->home;
	current->damping = rotSpeed;
	current->color = Color(CM_HSV,
						lmap<float>(index, 0.0f, total, 0.5f, 0.0f),
						1.0f, 1.0f);
	current->index = index;
	current->translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
}

void AXAR_ParticleApp::setup()
{
	vector<Particle> particles;
	particles.assign(NUM_LINES * 2, Particle());
	
	vector<Particle> particleHeads;
	particleHeads.assign(NUM_TRIANGLES * 3, Particle());
	
	vec4 center = vec4();
	
	for (int i = 0; i < particles.size(); i = i + TRAIL_LENGTH * 2)
	{
		int particleId = i / (TRAIL_LENGTH * 2);
		
		float rotSpeed = -Rand::randFloat(ROTATION_SPEED - 0.0002f, ROTATION_SPEED + 0.0002f);
		float rotZ = Rand::randFloat(-1.0f, 1.0f);
		
		float theta = (float)particleId / (float)NUM_PARTICLES * 2.0 * M_PI;
		float ringPhi = M_PI / 2.0;
		float sectionLength = 10.0f;
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24    )), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 1)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 2)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 3)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 4)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 5)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 6)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 7)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 8)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 9)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 10)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 11)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 12)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 13)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 14)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 15)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 16)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 17)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 18)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 19)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 20)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 21)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 22)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 23)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		for (int j = 0; j < TRAIL_LENGTH; j++) {
			
			float phi = M_PI / 2.0 - j * SECTION_ARC_LENGTH;
			float x = SPHERE_RADIUS * cos(theta) * sin(phi);  // Red
			float y = SPHERE_RADIUS * sin(theta) * sin(phi);  // Green
			float z = SPHERE_RADIUS * cos(phi);  // Blue

			auto &p = particles.at(i + j * 2);
			
			p.pos = (center + vec4(x, y, z, 1.0)) * rotate(-rotZ, vec3(x, y, 0));
			p.home = p.pos;
			p.ppos = p.home; // random initial velocity
			p.damping = rotSpeed;
			p.color = Color(CM_HSV,
							lmap<float>(i, 0.0f, particles.size(), 0.0f, 1.0f),
							1.0f, 1.0);
							//lmap<float>(j, 0.0f, TRAIL_LENGTH, 1.0f, 0.0f));
			p.index = i;
			p.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
			p.rotation = rotate(rotSpeed, vec3(vec4(y, -x, 0.0f, 1.0) * rotate(-rotZ, vec3(x, y, 0.0f))));

			auto &pNext = particles.at(i + j * 2 + 1);
			
			phi = M_PI / 2.0 - (j + 1) * SECTION_ARC_LENGTH;
			x = SPHERE_RADIUS * cos(theta) * sin(phi);  // Red
			y = SPHERE_RADIUS * sin(theta) * sin(phi);  // Green
			z = SPHERE_RADIUS * cos(phi);  // Blue

			pNext.pos = (center + vec4(x, y, z, 1.0));
			pNext.pos = pNext.pos * rotate(-rotZ, vec3(x, y, 0));
			pNext.home = p.pos;
			pNext.ppos = p.home; // random initial velocity
			pNext.damping = rotSpeed;
			pNext.color = Color(CM_HSV,
							lmap<float>(i, 0.0f, particles.size(), 0.0f, 1.0f),
							1.0f, 1.0f);
							//lmap<float>(j, 0.0f, TRAIL_LENGTH, 1.0f, 0.0f));
			pNext.index = i;
			pNext.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
			pNext.rotation = rotate(rotSpeed, vec3(vec4(y, -x, 0.0f, 1.0) * rotate(-rotZ, vec3(x, y, 0.0f))));
		}
	}
	
	// Load our update program.
	// Match up our attribute locations with the description we gave.
	
	mParticleRenderProg = gl::getStockShader(gl::ShaderDef().color());
	mParticleUpdateProg = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("particleUpdate.vs"))
									   .feedbackFormat(GL_INTERLEAVED_ATTRIBS)
									   .feedbackVaryings({ "position", "pposition", "home", "color", "damping", "index", "translation", "rotation"})
									   .attribLocation("iPosition", 0)
									   .attribLocation("iColor", 1)
									   .attribLocation("iPPosition", 2)
									   .attribLocation("iHome", 3)
									   .attribLocation("iDamping", 4)
									   .attribLocation("iIndex", 5)
									   .attribLocation("iTranslation", 6)
									   .attribLocation("iRotation", 10)
									   );
	setupBuffers(mAttributes, mParticleBuffer, particles);
	mParticleHeadRenderProg = gl::getStockShader(gl::ShaderDef().color());
	mParticleHeadUpdateProg = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("particleUpdate.vs"))
											   .feedbackFormat(GL_INTERLEAVED_ATTRIBS)
											   .feedbackVaryings({ "position", "pposition", "home", "color", "damping", "index", "translation", "rotation"})
											   .attribLocation("iPosition", 0)
											   .attribLocation("iColor", 1)
											   .attribLocation("iPPosition", 2)
											   .attribLocation("iHome", 3)
											   .attribLocation("iDamping", 4)
											   .attribLocation("iIndex", 5)
											   .attribLocation("iTranslation", 6)
											   .attribLocation("iRotation", 10)
											   );
	setupBuffers(mHeadAttributes, mHeadParticleBuffer, particleHeads);
	
	// Listen to mouse events so we can send data as uniforms.
	getWindow()->getSignalMouseDown().connect([this](MouseEvent event)
											  {
												  mMouseDown = true;
												  mMouseForce = 500.0f;
												  mMousePos = vec3(event.getX(), event.getY(), 0.0f);
												  mCamera.trigger();
											  });
	getWindow()->getSignalMouseDrag().connect([this](MouseEvent event)
											  {
												  mMousePos = vec3(event.getX(), event.getY(), 0.0f) - vec3(getWindowCenter(), 0.0f);
											  });
	getWindow()->getSignalMouseUp().connect([this](MouseEvent event)
											{
												mMouseForce = 0.0f;
												mMouseDown = false;
											});
	mCamera.update();

}

void AXAR_ParticleApp::setupBuffers(gl::VaoRef* vaos, gl::VboRef* vbos, vector<Particle> particleData) {
	
	// Create particle buffers on GPU and copy data into the first buffer.
	// Mark as static since we only write from the CPU once.
	vbos[mSourceIndex] = gl::Vbo::create(GL_ARRAY_BUFFER, particleData.size() * sizeof(Particle), particleData.data(), GL_STATIC_DRAW);
	vbos[mDestinationIndex] = gl::Vbo::create(GL_ARRAY_BUFFER, particleData.size() * sizeof(Particle), nullptr, GL_STATIC_DRAW);
	
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

void AXAR_ParticleApp::performProgramUpdate(gl::GlslProgRef mUpdateProg, gl::VboRef mBuffer, gl::VaoRef mAttributes, int drawType, int count) {
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

void AXAR_ParticleApp::update()
{
	for (int i = 0; i < NUM_PARTICLES * 2; i++) {
		
	}
	performProgramUpdate(mParticleUpdateProg, mParticleBuffer[mDestinationIndex], mAttributes[mSourceIndex], GL_LINES, NUM_LINES * 2);
	performProgramUpdate(mParticleHeadUpdateProg, mHeadParticleBuffer[mDestinationIndex], mHeadAttributes[mSourceIndex], GL_TRIANGLES, NUM_TRIANGLES * 3);

	// Swap source and destination for next loop
	std::swap(mSourceIndex, mDestinationIndex);
	
	// Update mouse force.
	if(mMouseDown) {
		mMouseForce = 150.0f;
	}
	
}

void AXAR_ParticleApp::render(gl::GlslProgRef mRenderProg, gl::VaoRef mAttributes, int drawType, int count) {
	gl::ScopedGlslProg render(mRenderProg);
	gl::ScopedVao vao(mAttributes);
	gl::context()->setDefaultShaderVars();
	gl::drawArrays(drawType, 0, count);
}

void AXAR_ParticleApp::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatricesWindowPersp(getWindowSize(), 60.0f, 1.0f, 100.0f);

	gl::setMatrices(mCamera.mCam);
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	render(mParticleRenderProg, mAttributes[mSourceIndex], GL_LINES, NUM_LINES * 2);
	render(mParticleHeadRenderProg, mHeadAttributes[mSourceIndex], GL_TRIANGLES, NUM_TRIANGLES * 3);
	
	gl::color(Color(255, 0, 0));
	gl::drawLine(vec3(0.0f, 0.0f, 0.0f), vec3(300.0f, 0.0f, 0.0f));
	gl::drawLine(vec3(100.0f, -20.0f, 0.0f), vec3(100.0f, 20.0f, 0.0f));
	gl::drawLine(vec3(200.0f, -20.0f, 0.0f), vec3(200.0f, 20.0f, 0.0f));
	gl::color(Color(0, 255, 0));
	gl::drawLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 300.0f, 0.0f));
	gl::drawLine(vec3(0.0f, 100.0f, -20.0f), vec3(0.0f, 100.0f, 20.0f));
	gl::drawLine(vec3(0.0f, 200.0f, -20.0f), vec3(0.0f, 200.0f, 20.0f));
	gl::color(Color(0, 0, 255));
	gl::drawLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 300.0f));
	gl::drawLine(vec3(-20.0f, 0.0f, 100.0f), vec3(20.0f, 0.0f, 100.0f));
	gl::drawLine(vec3(-20.0f, 0.0f, 200.0f), vec3(20.0f, 0.0f, 200.0f));
	
	mDashboard->displayMessage("Framerate: " + to_string(getAverageFps()), -2000.0f, -1100.0f);
	mDashboard->displayMessage("Upper right", 1500.0f, -1100.0f);
	mDashboard->displayMessage("Lower right", 1500.0f, 900.0f);
	mDashboard->displayMessage("Lower left", -2000.0f, 900.0f);
	mCamera.update();
}

CINDER_APP(AXAR_ParticleApp, RendererGl, [] (App::Settings *settings) {
	settings->setWindowSize(8000, 4000);
	settings->setMultiTouchEnabled(false);
})
