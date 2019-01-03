/*
 * VirNwLink.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#include "../graphs/VirNwLink.h"

VirNwLink::VirNwLink() {
	// TODO Auto-generated constructor stub

}

VirNwLink::~VirNwLink() {
	// TODO Auto-generated destructor stub
	for (vector<BasePath*>::iterator bpIter =
			this->getVirLinkSubPaths().begin();
			bpIter != this->getVirLinkSubPaths().end(); ++bpIter) {
		delete *bpIter;
	}
}

string& VirNwLink::getVirFromDomain() {
	return virFromDomain;
}

void VirNwLink::setVirFromDomain(const string& virFromDomain) {
	this->virFromDomain = virFromDomain;
}

double VirNwLink::getVirLinkAvailableBandwidth() {
	return virLinkAvailableBandwidth;
}

void VirNwLink::setVirLinkAvailableBandwidth(double virLinkAvailableBandwidth) {
	this->virLinkAvailableBandwidth = virLinkAvailableBandwidth;
}

double VirNwLink::getVirLinkBandwidth() {
	return virLinkBandwidth;
}

void VirNwLink::setVirLinkBandwidth(double virLinkBandwidth) {
	this->virLinkBandwidth = virLinkBandwidth;
}

double VirNwLink::getVirLinkDelay() {
	return virLinkDelay;
}

void VirNwLink::setVirLinkDelay(double virLinkDelay) {
	this->virLinkDelay = virLinkDelay;
}

string VirNwLink::getVirLinkFrom() {
	return virLinkFrom;
}

void VirNwLink::setVirLinkFrom(const string virLinkFrom) {
	this->virLinkFrom = virLinkFrom;
}

string& VirNwLink::getVirLinkId() {
	return virLinkID;
}

void VirNwLink::setVirLinkId(const string& virLinkId) {
	virLinkID = virLinkId;
}

vector<BasePath*>& VirNwLink::getVirLinkSubPaths() {
	return virLinkSubPaths;
}

void VirNwLink::setVirLinkSubPaths(const vector<BasePath*>& virLinkSubPaths) {
	this->virLinkSubPaths = virLinkSubPaths;
}

void VirNwLink::addVirLinkSubPath(BasePath* virLinkSubPath) {
	this->virLinkSubPaths.push_back(virLinkSubPath);
}

string& VirNwLink::getVirLinkTo() {
	return virLinkTo;
}

void VirNwLink::setVirLinkTo(const string& virLinkTo) {
	this->virLinkTo = virLinkTo;
}

double VirNwLink::getVirLinkWeight() {
	return virLinkWeight;
}

void VirNwLink::setVirLinkWeight(double virLinkWeight) {
	this->virLinkWeight = virLinkWeight;
}

string& VirNwLink::getVirNwNo() {
	return virNwNo;
}

void VirNwLink::setVirNwNo(const string& virNwNo) {
	this->virNwNo = virNwNo;
}

string& VirNwLink::getVirToDomain() {
	return virToDomain;
}

void VirNwLink::setVirToDomain(const string& virToDomain) {
	this->virToDomain = virToDomain;
}

int VirNwLink::getVirLinkSubPathHops() {
	return virLinkSubPathHops;
}

void VirNwLink::setVirLinkSubPathHops(int virLinkSubPathHops) {
	this->virLinkSubPathHops = virLinkSubPathHops;
}

double VirNwLink::getVirLinkSwitchingEnergy() {
	return virLinkSwitchingEnergy;
}

void VirNwLink::setVirLinkSwitchingEnergy(double virLinkSwitchingEnergy) {
	this->virLinkSwitchingEnergy = virLinkSwitchingEnergy;
}

void VirNwLink::printVirNwLinkInfo(VirNwLink* virLink) {
	cout << "------Virtual Link Information Start------" << endl;

	cout << "VirLinkId:" << virLink->getVirLinkId() << endl;
	cout << "From:" << virLink->getVirLinkFrom() << endl;
	cout << "To:" << virLink->getVirLinkTo() << endl;
	cout << "FromDomain:" << virLink->getVirFromDomain() << endl;
	cout << "ToDomain:" << virLink->getVirToDomain() << endl;
	cout << "Delay:" << virLink->getVirLinkDelay() << endl;
	cout << "Bandwidth:" << virLink->getVirLinkBandwidth() << endl;
	cout << "AvailableBandwidth:" << virLink->getVirLinkAvailableBandwidth()
			<< endl;
	cout << "SubPathHops:" << virLink->getVirLinkSubPathHops() << endl;
	cout << "SubPathsSize:" << virLink->getVirLinkSubPaths().size() << endl;
	cout << "SwitchingEnergy:" << virLink->getVirLinkSwitchingEnergy() << endl;
	cout << "Weight:" << virLink->getVirLinkWeight() << endl;

	cout << "------Virtual Link Information End------" << endl;
}
