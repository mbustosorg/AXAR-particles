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

#include "AXAR_ParticleApp.hpp"
#include "FinancialData.hpp"
#include "SystemConfig.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

namespace spd = spdlog;
using namespace ci;
using namespace std;

float DEFAULT_TEXT_X;
float DEFAULT_TEXT_Y;
float WINDOW_HEIGHT;
float WINDOW_WIDTH;

void AXAR_ParticleApp::setup() {

	WINDOW_WIDTH = getWindowWidth();
	WINDOW_HEIGHT = getWindowHeight();
	DEFAULT_TEXT_X = WINDOW_WIDTH / 2;
	DEFAULT_TEXT_Y = WINDOW_HEIGHT / 2;

	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
	sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logfile.txt", 23, 59));
	auto combined_logger = std::make_shared<spdlog::logger>("particleApp", begin(sinks), end(sinks));
	//register it if you need to access it globally
	spdlog::register_logger(combined_logger);

	spd::set_pattern("[%T%z][%L] %v");

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
	mScreenManager = new ScreenManager();
	mScreenManager->setCamera(&mCamera);
	mScreenManager->setDashboard(mDashboard);
}

void AXAR_ParticleApp::update()
{
	mScreenManager->update();
	// Update mouse force.
	if(mMouseDown) {
		mMouseForce = 150.0f;
	}
    hideCursor();
	
}

void AXAR_ParticleApp::draw()
{
	
	mScreenManager->draw();
	gl::setMatricesWindowPersp(getWindowSize(), 60.0f, 1.0f, 100.0f);
	gl::setMatrices(mCamera.mCam);

	/*
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
	*/
	
	//mDashboard->displayMessage("Framerate: " + to_string(int(getAverageFps())), 1500.0f, 1100.0f, 50, Color(1.0f, 1.0f, 1.0f));
	//mDashboard->displayMessage("Upper right", 1500.0f, -1100.0f);
	//mDashboard->displayMessage("Lower right", 1500.0f, 900.0f);
	//mDashboard->displayMessage("Lower left", -2000.0f, 900.0f);
	
	mCamera.update();
}

CINDER_APP(AXAR_ParticleApp, RendererGl, [] (App::Settings *settings) {
    settings->setFullScreen();
	settings->setMultiTouchEnabled(false);
})
