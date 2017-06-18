/*
 * KShortestPaths.cpp
 *
 *  Created on: 2016/10/24
 *      Author: zhangshanming
 */

#include "KShortestPaths.h"

KShortestPaths::KShortestPaths(const string graphFile) {
	this->algs = new JNI_CALL_JAVA_ALGORITHMs();
	this->algs->JNI_CALL_JAVA_Constructor_KShortestPath_Instance(graphFile);
}

KShortestPaths::~KShortestPaths() {
	delete this->algs;
}

map<string, BasePath*> KShortestPaths::getKShortestpaths(const string source,
		const string destination) {
	return this->algs->JNI_CALL_JAVA_Get_KShortestPaths(source, destination);
}

/*
 * used for calculate shortest path according to one constraint
 * Then based on the results can be to filter the optimal path with complex constraints
 * map<string, double> graphLinksInfo
 *    string: link Id
 *    double: link cost or link switching or link delay and so on
 *
 * outPut:
 * map<string, BasePath*>
 *   string:  path Id
 *   BasePath*:  path pinter.   pathCost and pathHops and pathInfo has value, other properties(swtiching energy and delay) are not set value.
 *
 */
map<string, BasePath*> KShortestPaths::getKShortestpaths(const string source,
		const string destination, map<string, double> graphLinksInfo) {
	return this->algs->JNI_CALL_JAVA_Get_KShortestPaths_With_GraphLinksInfo(
			source, destination, graphLinksInfo);
}

map<string, BasePath*> KShortestPaths::getKShortestpathsWithHopLimit(
		const string source, const string destination,
		map<string, double> graphLinksInfo, const int hopMinLimit,
		const int hopMaxLimit) {
	return this->algs->JNI_CALL_JAVA_Get_KShortestPaths_BaseOn_GraphLinksInfo_With_HopLimit(
			source, destination, graphLinksInfo, hopMinLimit, hopMaxLimit);
}

map<string, BasePath*> KShortestPaths::getKShortestPathsWithKLimit(
		const string source, const string destination, const int k) {
	return this->algs->JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit(source,
			destination, k);
}

map<string, BasePath*> KShortestPaths::getKShortestPathsWithHopMinimumLimit(
		const string source, const string destination, const int minimumHop) {
	return this->algs->JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit(
			source, destination, minimumHop);
}

map<string, BasePath*> KShortestPaths::getKShortestPathsWithHopMaximumLimit(
		const string source, const string destination, const int maximumHop) {
	return this->algs->JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit(
			source, destination, maximumHop);
}

map<string, BasePath*> KShortestPaths::getKShortestPathsWithHopRangeLimit(
		const string source, const string destination, const int minimumHop,
		const int maximumHop) {
	return this->algs->JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit(
			source, destination, minimumHop, maximumHop);
}
