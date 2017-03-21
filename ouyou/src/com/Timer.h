/*
 * Timer.h
 *
 *  Created on: Dec 11, 2016
 *      Author: ubean
 */

#ifndef COM_TIMER_H_
#define COM_TIMER_H_

#include <iostream>
#include <chrono>

using namespace std;

class Timer {
public:
	Timer();
	virtual ~Timer();

	void startTiming();
	void endTiming();

	void elapsedTime();
	string getEndTiming();

private:
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;
};


#endif /* COM_TIMER_H_ */
