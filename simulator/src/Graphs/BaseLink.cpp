/*
 * BaseLink.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#include "BaseLink.h"

BaseLink::BaseLink() {
	// TODO Auto-generated constructor stub

}

BaseLink::~BaseLink() {
	// TODO Auto-generated destructor stub
}

int BaseLink::getFrom() const {
	return from;
}

void BaseLink::setFrom(int from) {
	this->from = from;
}

int BaseLink::getTo() const {
	return to;
}

void BaseLink::setTo(int to) {
	this->to = to;
}

double BaseLink::getWeight() const {
	return weight;
}

void BaseLink::setWeight(double weight) {
	this->weight = weight;
}
