/*
 * BasePath.cpp
 *
 *  Created on: 2016/10/23
 *      Author: zhangshanming
 */

#include "../graphs/BasePath.h"

using namespace std;

//const double BasePath::MAX_PATH_DISTANCE = numeric_limits<double>::max();

BasePath::BasePath() {
	// TODO Auto-generated constructor stub

}

BasePath::BasePath(const string& pathId, const string& source,
		const string& destination, const double pathDistance,
		const vector<string>& pathInfo) {
	this->pathId = pathId;
	this->pathSource = source;
	this->pathDestination = destination;
	this->pathDistance = pathDistance;
	this->pathCost = 0.0;
	this->pathInfo = pathInfo;
	this->pathHops = pathInfo.size() - 1;
}

BasePath::BasePath(const string& pathId, const string& source,
		const string& destination, const double pathDistance,
		const int pathHops, const vector<string>& pathInfo) {
	this->pathId = pathId;
	this->pathSource = source;
	this->pathDestination = destination;
	this->pathDistance = pathDistance;
	this->pathCost = 0.0; // default distance cost
	this->pathInfo = pathInfo;
	this->pathHops = pathHops;
}

BasePath::BasePath(const string& pathId, const string& source,
		const string& destination, const double pathDistance,
		const double pathCost, const int pathHops,
		const vector<string>& pathInfo) {
	this->pathId = pathId;
	this->pathSource = source;
	this->pathDestination = destination;
	this->pathDistance = pathDistance;
	this->pathCost = pathCost; // default distance cost
	this->pathInfo = pathInfo;
	this->pathHops = pathHops;
}

BasePath::~BasePath() {

}

string BasePath::makePathId(const string& source, const string& destination) {
	return source + "_" + destination;
}

const string& BasePath::getPathDestination() const {
	return pathDestination;
}

void BasePath::setPathDestination(const string& pathDestination) {
	this->pathDestination = pathDestination;
}

const string& BasePath::getPathId() const {
	return pathId;
}

void BasePath::setPathId(const string& pathId) {
	this->pathId = pathId;
}

const vector<string>& BasePath::getPathInfo() const {
	return pathInfo;
}

void BasePath::setPathInfo(const vector<string>& pathInfo) {
	this->pathInfo = pathInfo;
}

const string& BasePath::getPathSource() const {
	return pathSource;
}

void BasePath::setPathSource(const string& pathSource) {
	this->pathSource = pathSource;
}

double BasePath::getPathDistance() const {
	return pathDistance;
}

void BasePath::setPathDistance(double pathDistance) {
	this->pathDistance = pathDistance;
}

int BasePath::getPathHops() {
	return pathHops;
}

void BasePath::setPathHops(int pathHops) {
	this->pathHops = pathHops;
}

double BasePath::getPathDeley() const {
	return pathDeley;
}

void BasePath::setPathDeley(double deley) {
	this->pathDeley = pathDeley;
}

double BasePath::getPathSwitchingEnergy() const {
	return pathSwitchingEnergy;
}

void BasePath::setPathSwitchingEnergy(double pathSwitchingEnergy) {
	this->pathSwitchingEnergy = pathSwitchingEnergy;
}

double BasePath::getPathCost() const {
	return pathCost;
}

void BasePath::setPathCost(double cost) {
	this->pathCost = cost;
}

double BasePath::getPathBandwidth() const {
	return pathBandwidth;
}

void BasePath::setPathBandwidth(double pathBandwidth) {
	this->pathBandwidth = pathBandwidth;
}

void BasePath::printBasePath(BasePath* bPath) {

	cout << "--- BasePath Info Start ---" << endl;
	cout << "--PathId         :" << bPath->getPathId() << endl;
	cout << "--Source         :" << bPath->getPathSource() << endl;
	cout << "--Destination    :" << bPath->getPathDestination() << endl;
	cout << "--PathBandwidth  :" << bPath->getPathBandwidth() << endl;
	cout << "--PathAvailableBandwidth  :" << bPath->getPathAvailableBandwidth() << endl;
	cout << "--PathDistance   :" << bPath->getPathDistance() << endl;
	cout << "--Deley          :" << bPath->getPathDeley() << endl;
	cout << "--PathCost       :" << bPath->getPathCost() << endl;
	cout << "--getPathHops    :" << bPath->getPathHops() << endl;
	cout << "--SwitchingEnergy:" << bPath->getPathSwitchingEnergy() << endl;
	cout << "--PathInfo       :" << endl;
	CommonFunctions::printVector(bPath->getPathInfo());

	cout << "--- BasePath Info End ---" << endl;

}

double BasePath::getPathAvailableBandwidth() {
	return pathAvailableBandwidth;
}

void BasePath::setPathAvailableBandwidth(double pathAvailableBandwidth) {
	this->pathAvailableBandwidth = pathAvailableBandwidth;
}
