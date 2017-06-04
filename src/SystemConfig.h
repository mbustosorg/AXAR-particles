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

//const int NUM_PARTICLES = 1001;
const int TRAIL_LENGTH = 15;
const int SPHERE_RADIUS = 2500;
const float SECTION_ARC_LENGTH = 0.02f;

const float HEAD_DEPTH = 10.0f;
const float HEAD_ARC_WIDTH = 0.005f;

const float ROTATION_SPEED = 0.002f;

static Color colorFromHex(int r, int g, int b) {
	return Color(CM_RGB, r / 255.0f, g / 255.0f, b / 255.0f);
}

static Color Colors[] = {
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
};

static std::unordered_map<std::string, int> SectorIndices = {
	{"Consumer Discretionary", 0},
	{"Consumer Staples", 1},
	{"Energy", 2},
	{"Financials", 3},
	{"Health Care", 4},
	{"Industrials", 5},
	{"Information Technology", 6},
	{"Materials", 7},
	{"Telecommunication Services", 8},
	{"Utilities", 9}
};

#endif
