
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

const int NUM_PARTICLES = 1001;
const int TRAIL_LENGTH = 100;
const int SPHERE_RADIUS = 1500;
const int NUM_LINES = NUM_PARTICLES * TRAIL_LENGTH * 2;
const float SECTION_ARC_LENGTH = 0.005;
const float ROTATION_SPEED = 0.02f;

class AXAR_ParticleApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
private:
	gl::GlslProgRef mRenderProg;
	gl::GlslProgRef mUpdateProg;
	
	// Descriptions of particle data layout.
	gl::VaoRef		mAttributes[2];
	// Buffers holding raw particle data on GPU.
	gl::VboRef		mParticleBuffer[2];
	
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

void AXAR_ParticleApp::setup()
{
	vector<Particle> particles;
	particles.assign(NUM_PARTICLES * TRAIL_LENGTH * 2, Particle());
	vec4 center = vec4();
	
	for(int i = 0; i < particles.size(); i = i + TRAIL_LENGTH * 2)
	{
		int particle_id = i / (TRAIL_LENGTH * 2);
		
		float rotSpeed = Rand::randFloat(ROTATION_SPEED - 0.0002f, ROTATION_SPEED + 0.0002f);
		float rotZ = Rand::randFloat(-1.0f, 1.0f);
		
		for (int j = 0; j < TRAIL_LENGTH; j++) {
			
			auto &p = particles.at(i + j * 2);
			
			float theta = (float)particle_id / (float)NUM_PARTICLES * 2.0 * M_PI;
			float phi = M_PI / 2.0 - j * SECTION_ARC_LENGTH;
			float x = SPHERE_RADIUS * cos(theta) * sin(phi);  // Red
			float y = SPHERE_RADIUS * sin(theta) * sin(phi);  // Green
			float z = SPHERE_RADIUS * cos(phi);  // Blue
			
			p.pos = (center + vec4(x, y, z, 1.0)) * rotate(-rotZ, vec3(x, y, 0));
			p.home = p.pos;
			p.ppos = p.home; // random initial velocity
			p.damping = rotSpeed;
			p.color = Color(CM_HSV, lmap<float>(i, 0.0f, particles.size(), 0.0f, 0.66f), 1.0f, 1.0f);
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
			pNext.color = Color(CM_HSV, lmap<float>(i, 0.0f, particles.size(), 0.0f, 0.66f), 1.0f, 1.0f);
			pNext.index = i;
			pNext.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
			pNext.rotation = rotate(rotSpeed, vec3(vec4(y, -x, 0.0f, 1.0) * rotate(-rotZ, vec3(x, y, 0.0f))));
		}
	}
	
	// Create particle buffers on GPU and copy data into the first buffer.
	// Mark as static since we only write from the CPU once.
	mParticleBuffer[mSourceIndex] = gl::Vbo::create(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_STATIC_DRAW);
	mParticleBuffer[mDestinationIndex] = gl::Vbo::create(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), nullptr, GL_STATIC_DRAW);
	
	// Load our update program.
	// Match up our attribute locations with the description we gave.
	
	mRenderProg = gl::getStockShader(gl::ShaderDef().color());
	mUpdateProg = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("particleUpdate.vs"))
									   
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
	
	for(int i = 0; i < 2; ++i)
	{	// Describe the particle layout for OpenGL.
		mAttributes[i] = gl::Vao::create();
		gl::ScopedVao vao(mAttributes[i]);
		
		// Define attributes as offsets into the bound particle buffer
		gl::ScopedBuffer buffer(mParticleBuffer[i]);
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

void AXAR_ParticleApp::update()
{
	for (int i = 0; i < NUM_PARTICLES * 2; i++) {
		
	}
	// Update particles on the GPU
	gl::ScopedGlslProg prog(mUpdateProg);
	gl::ScopedState rasterizer(GL_RASTERIZER_DISCARD, true);	// turn off fragment stage
	
	// Bind the source data (Attributes refer to specific buffers).
	gl::ScopedVao source(mAttributes[mSourceIndex]);
	// Bind destination as buffer base.
	gl::bindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mParticleBuffer[mDestinationIndex]);
	gl::beginTransformFeedback(GL_LINES);
	
	// Draw source into destination, performing our vertex transformations.
	gl::drawArrays(GL_LINES, 0, NUM_LINES);
	
	gl::endTransformFeedback();
	
	// Swap source and destination for next loop
	std::swap(mSourceIndex, mDestinationIndex);
	
	// Update mouse force.
	if(mMouseDown) {
		mMouseForce = 150.0f;
	}
	
}

void AXAR_ParticleApp::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatricesWindowPersp(getWindowSize(), 60.0f, 1.0f, 100.0f);

	gl::setMatrices(mCamera.mCam);
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	gl::ScopedGlslProg render(mRenderProg);
	gl::ScopedVao vao(mAttributes[mSourceIndex]);
	gl::context()->setDefaultShaderVars();
	gl::drawArrays(GL_LINES, 0, NUM_LINES);
	
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
