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

#ifndef TargetFocusTimes_hpp
#define TargetFocusTimes_hpp

#include <stdio.h>
#include <vector>

using namespace std;

class TargetFocusTimes {
	
public:
	TargetFocusTimes(vector<int>* focusIndexes, const vector<float> startTimes, const vector<float> endTimes);
	~TargetFocusTimes();
	
	float startTime();
	float endTime();
	void increment();
	void restart(int upperEntryIndex);
	bool expired(float time);
	bool active();
	bool newFocusTrigger(float time);
	int focusIndex();
	
protected:
	int mIndex = 0;
	vector<int>* mFocusIndexes;
	vector<float> mStartTimes;
	vector<float> mEndTimes;
};


#endif
