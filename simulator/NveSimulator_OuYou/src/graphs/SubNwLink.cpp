/*
 * SubNwLink.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#include "../graphs/SubNwLink.h"
using namespace std;

SubNwLink::SubNwLink() {
}

SubNwLink::SubNwLink(double swEnergy, double maxBandwidth) {

}
SubNwLink::~SubNwLink() {
	// TODO Auto-generated destructor stub
}

double SubNwLink::getSubLinkAvailableBandwidth() const {
	return subLinkAvailableBandwidth;
}

void SubNwLink::setSubLinkAvailableBandwidth(double subLinkAvailableBandwidth) {
	this->subLinkAvailableBandwidth = subLinkAvailableBandwidth;
}

double SubNwLink::getSubLinkBandwidth() const {
	return subLinkBandwidth;
}

void SubNwLink::setSubLinkBandwidth(double subLinkBandwidth) {
	this->subLinkBandwidth = subLinkBandwidth;
}

double SubNwLink::getSubLinkDelay() const {
	return subLinkDelay;
}

void SubNwLink::setSubLinkDelay(double subLinkDelay) {
	this->subLinkDelay = subLinkDelay;
}

double SubNwLink::getSubLinkEmbeddableBandwidth() const {
	return subLinkEmbeddableBandwidth;
}

void SubNwLink::setSubLinkEmbeddableBandwidth(
		double subLinkEmbeddableBandwidth) {
	this->subLinkEmbeddableBandwidth = subLinkEmbeddableBandwidth;
}

const string& SubNwLink::getSubLinkFrom() const {
	return subLinkFrom;
}

void SubNwLink::setSubLinkFrom(const string& subLinkFrom) {
	this->subLinkFrom = subLinkFrom;
}

const string& SubNwLink::getSubLinkFromDomain() const {
	return subLinkFromDomain;
}

void SubNwLink::setSubLinkFromDomain(const string& subLinkFromDomain) {
	this->subLinkFromDomain = subLinkFromDomain;
}

double SubNwLink::getSubLinkSwitchingEnergy() const {
	return subLinkSwitchingEnergy;
}

void SubNwLink::setSubLinkSwitchingEnergy(double subLinkSwitchingEnergy) {
	this->subLinkSwitchingEnergy = subLinkSwitchingEnergy;
}

const string& SubNwLink::getSubLinkTo() const {
	return subLinkTo;
}

void SubNwLink::setSubLinkTo(const string& subLinkTo) {
	this->subLinkTo = subLinkTo;
}

const string& SubNwLink::getSubLinkToDomain() const {
	return subLinkToDomain;
}

void SubNwLink::setSubLinkToDomain(const string& subLinkToDomain) {
	this->subLinkToDomain = subLinkToDomain;
}

double SubNwLink::getSubLinkWeight() const {
	return subLinkWeight;
}

void SubNwLink::setSubLinkWeight(double subLinkWeight) {
	this->subLinkWeight = subLinkWeight;
}

multimap<string, string>& SubNwLink::getSubLinkMappedVirLinks() {
	return subLinkMappedVirLinks;
}

void SubNwLink::setSubLinkMappedVirLinks(multimap<string, string> subLinkMappedVirLinks) {
	this->subLinkMappedVirLinks = subLinkMappedVirLinks;
}

void SubNwLink::setSubLinkMappedVirLink(string virNwNo, string virLinkId) {
	this->subLinkMappedVirLinks.insert(
			pair<string, string>(virNwNo, virLinkId));
}

void SubNwLink::printSubLinkInfo(SubNwLink* subLink) {
	//cout << "-------- Sub link Info Start--------" << endl;

	cout << setw(10) << "From: " << subLink->getSubLinkFrom() << " ";
	cout << setw(10) << " To:" << subLink->getSubLinkTo() << " ";
	cout << setw(10) << " FromDomain:" << subLink->getSubLinkFromDomain() << " ";
	cout << setw(10) << " ToDomain:" << subLink->getSubLinkToDomain() << " ";
	cout << setw(10) << " Bandwidth:" << subLink->getSubLinkBandwidth() << " ";
	cout << " Delay:" << subLink->getSubLinkDelay() << " ";
	cout << " AvailableBandwidth:" << subLink->getSubLinkAvailableBandwidth() << " ";
	cout << " EmbeddableBandwidth:" << subLink->getSubLinkEmbeddableBandwidth() << " ";
	cout << " SwitchingEnergy:" << subLink->getSubLinkSwitchingEnergy() << " ";
	cout << " Weight:" << subLink->getSubLinkWeight() << " ";
	cout << " MappedVirLinksSize:" << subLink->getSubLinkMappedVirLinks().size() << endl;

	//cout << "-------- Sub link Info End--------" << endl;
}
