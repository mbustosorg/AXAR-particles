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

#ifndef SystemConfig_h
#define SystemConfig_h

#include <vector>
#include <unordered_map>

#include "cinder/color.h"
#include "cinder/app/Window.h"

//const int NUM_PARTICLES = 1001;
const int TRAIL_LENGTH = 20;
const int SPHERE_RADIUS = 2000;
const float SECTION_ARC_LENGTH = 0.02f;

const float HEAD_DEPTH = 10.0f;
const float HEAD_ARC_WIDTH = 0.005f;

const float DEFAULT_EYE_RADIUS = 4500.0f;
const float DEFAULT_EYE_HEIGHT = 2000.0f;
const float DEFAULT_EYE_DISTANCE = sqrt(pow(DEFAULT_EYE_RADIUS, 2.0f) + pow(DEFAULT_EYE_HEIGHT, 2.0f));
const float TARGET_DISTANCE_FACTOR = 2.0f;
const float ROTATION_SPEED = 0.002f;

extern float DEFAULT_TEXT_X;
extern float DEFAULT_TEXT_Y;
extern float WINDOW_WIDTH;
extern float WINDOW_HEIGHT;

const float MinRadius = 10.0f;
const int RadiusSteps = 50;
const float MINIMUM_WEIGHT = 0.0001f;

#ifdef AXA_SHORT
const float TIME_FACTOR = 1.0f;
#else
const float TIME_FACTOR = 1.0f;
#endif

const float UNIVERSE_FONT_SIZE = 100.0f;
const float ENTITY_FONT_SIZE = 50.0f;

const float MESSAGE_INTRO = 3.0f / TIME_FACTOR;
const float MESSAGE_TIME_PER_LINE = 1.0f / TIME_FACTOR;
const float MESSAGE_FADE_SECONDS = 8.0f / TIME_FACTOR;
const float CAMERA_APPROACH_FACTOR = 40.0f / TIME_FACTOR;
const float CAMERA_TARGET_APPROACH_FACTOR = 5.0f / TIME_FACTOR;

const float GEO_MSCIW_LENGTH = 250.0f / TIME_FACTOR;
const int GEO_MSCIW_ENTITY_COUNT = 10;
const std::vector<float> GEO_MSCIW_START = {10.0f / TIME_FACTOR, 30.0f / TIME_FACTOR, 50.0f / TIME_FACTOR, 70.0f / TIME_FACTOR, 90.0f / TIME_FACTOR, 110.0f / TIME_FACTOR, 130.0f / TIME_FACTOR, 150.0f / TIME_FACTOR, 170.0f / TIME_FACTOR, 190.0f / TIME_FACTOR};
const std::vector<float> GEO_MSCIW_END = {20.0f / TIME_FACTOR, 40.0f / TIME_FACTOR, 60.0f / TIME_FACTOR, 80.0f / TIME_FACTOR, 100.0f / TIME_FACTOR, 120.0f / TIME_FACTOR, 120.0f / TIME_FACTOR, 160.0f / TIME_FACTOR, 180.0f / TIME_FACTOR, 200.0f / TIME_FACTOR};

const float GEO_LENGTH = 80.0f / TIME_FACTOR;
const int GEO_ENTITY_COUNT = 3;
const std::vector<float> GEO_START = {15.0f / TIME_FACTOR, 35.0f / TIME_FACTOR, 55.0f / TIME_FACTOR};
const std::vector<float> GEO_END = {25.0f / TIME_FACTOR, 45.0f / TIME_FACTOR, 65.0f / TIME_FACTOR};

const float ORB_MSCIW_LENGTH = 250.0f / TIME_FACTOR;
const int ORB_MSCIW_ENTITY_COUNT = 2;
const std::vector<float> ORB_MSCIW_START = {40.0f / TIME_FACTOR, 90.0f / TIME_FACTOR};
const std::vector<float> ORB_MSCIW_END = {50.0f / TIME_FACTOR, 105.0f / TIME_FACTOR};

const float ORB_LENGTH = 80.0f / TIME_FACTOR;
const int ORB_ENTITY_COUNT = 2;
const std::vector<float> ORB_START = {15.0f / TIME_FACTOR, 45.0f / TIME_FACTOR};
const std::vector<float> ORB_END = {30.0f / TIME_FACTOR, 60.0f / TIME_FACTOR};

const float IND_MSCIW_LENGTH = 250.0f / TIME_FACTOR;
const int IND_MSCIW_ENTITY_COUNT = 2;
const std::vector<float> IND_MSCIW_START = {40.0f / TIME_FACTOR, 90.0f / TIME_FACTOR};
const std::vector<float> IND_MSCIW_END = {50.0f / TIME_FACTOR, 105.0f / TIME_FACTOR};

const float IND_LENGTH = 80.0f / TIME_FACTOR;
const int IND_ENTITY_COUNT = 2;
const std::vector<float> IND_START = {15.0f / TIME_FACTOR, 45.0f / TIME_FACTOR};
const std::vector<float> IND_END = {30.0f / TIME_FACTOR, 60.0f / TIME_FACTOR};

static cinder::Color colorFromHex(int r, int g, int b) {
	return cinder::Color(cinder::CM_RGB, r / 255.0f * 235.0f / 255.0f, g / 255.0f * 235.0f / 255.0f, b / 255.0f * 235.0f / 255.0f);
}

static cinder::Color Colors[] = {
	colorFromHex(0xA6, 0xCE, 0xE3),
	colorFromHex(0x1F, 0x78, 0xB4),
	colorFromHex(0xB2, 0xDF, 0x8A),
	colorFromHex(0x33, 0xA0, 0x2C),
	colorFromHex(0xFB, 0x9A, 0x99),
	colorFromHex(0xE3, 0x1A, 0x1C),
	colorFromHex(0xFB, 0xBF, 0x6F),
	colorFromHex(0xFF, 0x7F, 0x00),
	colorFromHex(0xCA, 0xB2, 0xD6),
	colorFromHex(0x6A, 0x3D, 0x9A),
	colorFromHex(0xA6, 0xCE, 0xE3),
	colorFromHex(0x1F, 0x78, 0xB4),
	colorFromHex(0xB2, 0xDF, 0x8A),
	colorFromHex(0x33, 0xA0, 0x2C),
	colorFromHex(0xFF, 0xFF, 0xFF)
};

static std::unordered_map<std::string, int> SectorIndices = {
	{"FINANCE", 1},
	{"CONSDIS", 2},
	{"CONSTAPL", 3},
	{"ENERGY", 4},
	{"BASICIND", 5},
	{"TECH", 6},
	{"TRANSPRT", 7},
	{"BUSINSRV", 8},
	{"INDUSTRY", 9},
	{"REALEST", 10},
	{"UTILS", 11},
	{"TELECOMM", 12},
	{"HEALTH", 13},
	{"", 14}
};

class underscore_numpunct : public std::numpunct<char>
{
protected:
	virtual char do_thousands_sep() const
	{
		return '_';
	}
	
	virtual std::string do_grouping() const
	{
		return "\03";
	}
};

#endif
