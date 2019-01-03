/*
 * EndUserRequests.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: ubean
 */

#include "EndUserRequest.h"

using namespace std;

EndUserRequest::EndUserRequest() {
	// TODO Auto-generated constructor stub
	this->com_flag = 0; // is not commmon request
}

EndUserRequest::~EndUserRequest() {
	// TODO Auto-generated destructor stub
}

double EndUserRequest::getReqDataRate() const {
	return reqDataRate;
}

void EndUserRequest::setReqDataRate(double reqDataRate) {
	this->reqDataRate = reqDataRate;
}

int EndUserRequest::getReqDurationTimeUnits() const {
	return reqDurationTimeUnits;
}

void EndUserRequest::setReqDurationTimeUnits(int reqDurationTimeUnits) {
	this->reqDurationTimeUnits = reqDurationTimeUnits;
}

const string& EndUserRequest::getReqDestinationVirNode() const {
	return reqDestinationVirNode;
}

void EndUserRequest::setReqDestinationVirNode(
		const string& reqDestinationVirNode) {
	this->reqDestinationVirNode = reqDestinationVirNode;
}

int EndUserRequest::getReqEndTiming() const {
	return reqEndTiming;
}

void EndUserRequest::setReqEndTiming(int reqEndTiming) {
	this->reqEndTiming = reqEndTiming;
}

const string& EndUserRequest::getReqSourceVirNode() const {
	return reqSourceVirNode;
}

void EndUserRequest::setReqSourceVirNode(const string& reqSourceVirNode) {
	this->reqSourceVirNode = reqSourceVirNode;
}

int EndUserRequest::getReqStartTiming() const {
	return reqStartTiming;
}

void EndUserRequest::setReqStartTiming(int reqStartTiming) {
	this->reqStartTiming = reqStartTiming;
}

string EndUserRequest::getReqVirNwNo() const {
	return reqVirNwNo;
}

void EndUserRequest::setReqVirNwNo(string reqVirNwNo) {
	this->reqVirNwNo = reqVirNwNo;
}

const string& EndUserRequest::getReqId() const {
	return reqId;
}

void EndUserRequest::setReqId(const string& reqId) {
	this->reqId = reqId;
}

vector<pair<string, double>> EndUserRequest::getReqSubPathsLinks() {
	return this->reqSubPathsLinks;
}

void EndUserRequest::setReqSubPathsLinks(
		vector<pair<string, double>> reqSubPathsLinks) {
	this->reqSubPathsLinks = reqSubPathsLinks;
}

void EndUserRequest::addReqSubPathLink(string linkId,
		double allocatedDatarate) {
	this->reqSubPathsLinks.push_back(
			pair<string, double>(linkId, allocatedDatarate));
}

vector<pair<string, double>> EndUserRequest::getReqVirPathLinks() {
	return this->reqVirPathLinks;
}

void EndUserRequest::setReqVirPathLinks(
		vector<pair<string, double>> reqVirPathLinks) {
	this->reqVirPathLinks = reqVirPathLinks;
}

void EndUserRequest::addReqVirPathLink(string linkId,
		double allocatedDatarate) {
	this->reqVirPathLinks.push_back(
			pair<string, double>(linkId, allocatedDatarate));
}

void EndUserRequest::printEndUserRequest(EndUserRequest* euRequest) {
	cout << " ------ EndUserRequest Info Start------" << endl;

	cout << " RequestId: " << euRequest->reqId << endl;
	cout << " RequestVirNw: " << euRequest->reqVirNwNo << endl;
	cout << " SourceNode: " << euRequest->reqSourceVirNode << endl;
	cout << " DestinationNode: " << euRequest->reqDestinationVirNode << endl;
	cout << " RequestDataRate: " << euRequest->reqDataRate << endl;
	cout << " StartTiming: " << euRequest->reqStartTiming << endl;
	cout << " DurationTime: " << euRequest->reqDurationTimeUnits << endl;
	cout << " EndTiming: " << euRequest->reqEndTiming << endl;

	cout << " CommonFlag: " << euRequest->com_flag << endl;

	cout << " virtual links composed of virtual transport path: " << endl;
	for (vector<pair<string, double>>::iterator rvlIter =
			euRequest->getReqVirPathLinks().begin();
			rvlIter != euRequest->getReqVirPathLinks().end(); ++rvlIter) {
		cout << " virtual link: " << (*rvlIter).first
				<< " allocated bandwidth: " << (*rvlIter).second << endl;
	}

	cout << " substrate links composed of substrate transport paths: " << endl;
	for (vector<pair<string, double>>::iterator rslIter =
			euRequest->getReqSubPathsLinks().begin();
			rslIter != euRequest->getReqSubPathsLinks().end(); ++rslIter) {
		cout << " substrate link: " << (*rslIter).first
				<< " allocated bandwidth: " << (*rslIter).second << endl;
	}
	cout << " ------ EndUserRequest Info End------" << endl;
}
vector<pair<string, double> >& EndUserRequest::getReqSubPathInfo() {
	return reqSubPathInfo;
}

void EndUserRequest::setReqSubPathInfo(
		const vector<pair<string, double> >& reqSubPathInfo) {
	this->reqSubPathInfo = reqSubPathInfo;
}

void EndUserRequest::addReqSubPathInfo(string pathId,
		double allocatedDatarate) {
	this->reqSubPathInfo.push_back(
			pair<string, double>(pathId, allocatedDatarate));
}

int EndUserRequest::getComFlag() const {
	return com_flag;
}

void EndUserRequest::setComFlag(int comFlag) {
	com_flag = comFlag;
}

void EndUserRequest::copyEndUserRequests(EndUserRequest* euRequest) {

	this->setReqStartTiming(euRequest->getReqStartTiming());
	this->setReqDurationTimeUnits(euRequest->getReqDurationTimeUnits());
	this->setReqEndTiming(euRequest->getReqEndTiming());

	this->setReqVirNwNo(euRequest->getReqVirNwNo());
	this->setReqSourceVirNode(euRequest->getReqSourceVirNode());
	this->setReqDestinationVirNode(euRequest->getReqDestinationVirNode());
	this->setReqId(euRequest->getReqId());
	this->setReqDataRate(euRequest->getReqDataRate());

	this->setReqVirPathLinks(euRequest->getReqVirPathLinks());
	this->setReqSubPathsLinks(euRequest->getReqSubPathInfo());
	this->setComFlag(euRequest->getComFlag());

}
