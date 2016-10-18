/*
 * SubNwLink.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#include "SubNwLink.h"

SubNwLink::SubNwLink() {
	// TODO Auto-generated constructor stub

}

SubNwLink::~SubNwLink() {
	// TODO Auto-generated destructor stub
}

double SubNwLink::getAvailableBw() const {
	return availableBw;
}

void SubNwLink::setAvailableBw(double availableBw) {
	this->availableBw = availableBw;
}

double SubNwLink::getDelay() const {
	return delay;
}

void SubNwLink::setDelay(double delay) {
	this->delay = delay;
}

double SubNwLink::getMaxBw() const {
	return maxBw;
}

void SubNwLink::setMaxBw(double maxBw) {
	this->maxBw = maxBw;
}

double SubNwLink::getSwitchingEnergy() const {
	return switchingEnergy;
}

void SubNwLink::setSwitchingEnergy(double switchingEnergy) {
	this->switchingEnergy = switchingEnergy;
}

void SubNwLink::increaseLinkAvailableBandwidth(double variationBw) {
	this->availableBw += variationBw;
}

void SubNwLink::decreaseLinkAvailableBandwidth(double variationBw) {
	this->availableBw -= variationBw;
}
