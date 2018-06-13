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

#include "TargetFocusTimes.hpp"

TargetFocusTimes::TargetFocusTimes(vector<int>* focusIndexes, vector<float>* startTimes, vector<float>* endTimes) {
	mFocusIndexes = focusIndexes;
	mStartTimes = startTimes;
	mEndTimes = endTimes;
}

TargetFocusTimes::~TargetFocusTimes() {
	
}

float TargetFocusTimes::startTime() {
	return mStartTimes->at(mIndex);
}

float TargetFocusTimes::endTime() {
	return mEndTimes->at(mIndex);
}

void TargetFocusTimes::increment() {
	mIndex++;
}

void TargetFocusTimes::restart(int upperEntryIndex) {
	mIndex = 0;
	delete mFocusIndexes;
	mFocusIndexes = new vector<int>{static_cast<int>(rand() % upperEntryIndex), static_cast<int>(rand() % upperEntryIndex)};
}

bool TargetFocusTimes::newFocusTrigger(unsigned long time) {
	return time > mStartTimes->at(mIndex);
}

bool TargetFocusTimes::active() {
	return mIndex < mStartTimes->size();
}

bool TargetFocusTimes::expired(unsigned long time) {
	return time > mEndTimes->at(mIndex);
}

int TargetFocusTimes::focusIndex() {
	return mFocusIndexes->at(mIndex);
}

