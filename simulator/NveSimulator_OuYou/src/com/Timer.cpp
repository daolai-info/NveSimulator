/*
 * Timer.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: ubean
 */

#include "Timer.h"

using namespace std;

Timer::Timer() {
	cout << " Timer has been created. " << endl;
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

void Timer::startTiming() {
	this->start = std::chrono::system_clock::now();

	std::time_t start_time = std::chrono::system_clock::to_time_t(this->start);
	cout << " Start Time: " << std::ctime(&start_time) << endl;
}

void Timer::endTiming() {
	this->end = std::chrono::system_clock::now();
}

string Timer::getEndTiming() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d%H:%I:%S", timeinfo);
	return string(buffer);
}
void Timer::elapsedTime() {
	std::chrono::duration<double> elapsed_seconds = this->end - this->start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(this->end);
	;
	cout << " Finish Time: " << std::ctime(&end_time) << endl;
	cout << " Elapsed Time:" << std::chrono::duration_cast
			< std::chrono::minutes
			> (this->end - this->start).count() << " (Minutes) "
					<< elapsed_seconds.count() << "(seconds) "
					<< std::chrono::duration_cast < std::chrono::milliseconds
			> (this->end - this->start).count() << " (milliseconds)" << endl;

}
