/*
 * BaseLink.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#include "../graphs/BaseLink.h"


const double BaseLink::MAX_LINK_DISTANCE = numeric_limits<double>::max();

BaseLink::BaseLink() {
	this->from = "*";
	this->to = "*";
	this->weight = 0.0;
	this->delay = 0.0;
	this->bandwidth = 0.0;
	this->availableBandwidth = 0.0;
	this->fromDomain = "*";
	this->toDomain = "*";
}

BaseLink::BaseLink(string from, string to, double weight, double delay, double bandwidth, string fromDomain, string toDomain) {
	// TODO Auto-generated constructor stub
	this->from = from;
	this->to = to;
	this->weight = weight;
	this->delay = delay;
	this->bandwidth = bandwidth;
	this->availableBandwidth = 0.0;
	this->fromDomain = fromDomain;
	this->toDomain = toDomain;
}

BaseLink::BaseLink(string from, string to, double weight, double delay, double bandwidth, double availableBandwidth, string fromDomain, string toDomain) {
	// TODO Auto-generated constructor stub
	this->from = from;
	this->to = to;
	this->weight = weight;
	this->delay = delay;
	this->bandwidth = bandwidth;
	this->availableBandwidth = availableBandwidth;
	this->fromDomain = fromDomain;
	this->toDomain = toDomain;
}

BaseLink::~BaseLink() {
	// TODO Auto-generated destructor stub
}

double BaseLink::getBandwidth() const {
	return bandwidth;
}

void BaseLink::setBandwidth(double bandwidth) {
	this->bandwidth = bandwidth;
}

double BaseLink::getDelay() const {
	return delay;
}

void BaseLink::setDelay(double delay) {
	this->delay = delay;
}

const string& BaseLink::getFrom() const {
	return from;
}

void BaseLink::setFrom(const string& from) {
	this->from = from;
}

const string& BaseLink::getFromDomain() const {
	return fromDomain;
}

void BaseLink::setFromDomain(const string& fromDomain) {
	this->fromDomain = fromDomain;
}

const string& BaseLink::getTo() const {
	return to;
}

void BaseLink::setTo(const string& to) {
	this->to = to;
}

const string& BaseLink::getToDomain() const {
	return toDomain;
}

void BaseLink::setToDomain(const string& toDomain) {
	this->toDomain = toDomain;
}

double BaseLink::getWeight() const {
	return weight;
}

void BaseLink::setWeight(double weight) {
	this->weight = weight;
}

double BaseLink::getAvailableBandwidth() const {
	return availableBandwidth;
}

void BaseLink::setAvailableBandwidth(double availableBandwidth) {
	this->availableBandwidth = availableBandwidth;
}
