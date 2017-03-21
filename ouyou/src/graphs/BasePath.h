/*
 * BasePath.h
 *
 *  Created on: 2016/10/23
 *      Author: zhangshanming
 */

#ifndef GRAPHS_BASEPATH_H_
#define GRAPHS_BASEPATH_H_
#include <string>
#include <vector>
#include <iostream>
#include <limits>

#include "../com/CommonFunctions.h"

using namespace std;

class BasePath {
public:
	//static const double MAX_PATH_DISTANCE;

	static string makePathId(const string& source, const string& destination);
	BasePath();
	BasePath(const string& pathId, const string& source,
			const string& destination, const double pathDistance,
			const vector<string>& pathInfo);
	BasePath(const string& pathId, const string& source,
			const string& destination, const double pathDistance,
			const int pathHops, const vector<string>& pathInfo);

	BasePath(const string& pathId, const string& source,
			const string& destination, const double pathDistance,
			const double pathCost, const int pathHops,
			const vector<string>& pathInfo);

	virtual ~BasePath();
	const string& getPathDestination() const;
	void setPathDestination(const string& destination);
	const string& getPathId() const;
	void setPathId(const string& pathId);
	const vector<string>& getPathInfo() const;
	void setPathInfo(const vector<string>& pathInfo);
	const string& getPathSource() const;
	void setPathSource(const string& source);
	double getPathDistance() const;
	void setPathDistance(double pathDistance);
	int getPathHops();
	void setPathHops(int pathHops);
	double getPathDeley() const;
	void setPathDeley(double deley);
	double getPathSwitchingEnergy() const;
	void setPathSwitchingEnergy(double switchingEnergy);
	double getPathCost() const;
	void setPathCost(double cost);
	double getPathBandwidth() const;
	void setPathBandwidth(double pathBandwidth);

	static void printBasePath(BasePath* bPath);

	double getPathAvailableBandwidth();
	void setPathAvailableBandwidth(double avalibaleBandwidth);

private:
	string pathId; // format: source_destination_no
	string pathSource;
	string pathDestination;

	// add 1 start
	double pathSwitchingEnergy;
	double pathDeley;
	// add 1 end

	double pathDistance; // weight or length
	double pathBandwidth; // default 0.0. allocate bandwidth at embedding
	double pathAvailableBandwidth;
	int pathHops;
	vector<string> pathInfo; //  save node list from first to last nodes, not links

	double pathCost;  // the cost used for calculating variable
};

#endif /* GRAPHS_BASEPATH_H_ */
