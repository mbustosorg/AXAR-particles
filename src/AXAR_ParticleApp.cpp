
#include "AXAR_ParticleApp.hpp"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void AXAR_ParticleApp::setup() {
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
	screenManager.update();
	// Update mouse force.
	if(mMouseDown) {
		mMouseForce = 150.0f;
	}
	
}

void AXAR_ParticleApp::draw()
{
	
	screenManager.draw();
	gl::setMatricesWindowPersp(getWindowSize(), 60.0f, 1.0f, 100.0f);
	gl::setMatrices(mCamera.mCam);

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
