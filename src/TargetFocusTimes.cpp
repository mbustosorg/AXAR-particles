/*
 
 Copyright (C) 2018 Mauricio Bustos (m@bustos.org)
 
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

#include "TargetFocusTimes.hpp"
#include "spdlog/spdlog.h"

TargetFocusTimes::TargetFocusTimes(vector<int>* focusIndexes, const vector<float> startTimes, const vector<float> endTimes) {
	mFocusIndexes = focusIndexes;
	mStartTimes = startTimes;
	mEndTimes = endTimes;
}

TargetFocusTimes::~TargetFocusTimes() {
	
}

float TargetFocusTimes::startTime() {
	return mStartTimes.at(mIndex);
}

float TargetFocusTimes::endTime() {
	return mEndTimes.at(mIndex);
}

void TargetFocusTimes::increment() {
	mIndex++;
}

void TargetFocusTimes::restart(int upperEntryIndex, vector<int>* focusIndexes) {
	mIndex = 0;
	delete mFocusIndexes;
	mFocusIndexes = focusIndexes; 
}

bool TargetFocusTimes::newFocusTrigger(float time) {
	return time > mStartTimes.at(mIndex) && time < mEndTimes.at(mIndex);
}

bool TargetFocusTimes::active() {
	return mIndex < mStartTimes.size();
}

bool TargetFocusTimes::expired(float time) {
	return time > mEndTimes.at(mIndex);
}

int TargetFocusTimes::focusIndex() {
	if (mIndex >= mFocusIndexes->size()) {
		spdlog::get("particleApp")->info("{}", mFocusIndexes->size());
		(void) 0;
	}
	return mFocusIndexes->at(mIndex);
}

