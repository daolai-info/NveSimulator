/*
 * KShortestPathsWithYEN.h
 *
 *  Created on: 2016/10/24
 *      Author: zhangshanming
 */

#ifndef ALGORITHMS_KSHORTESTPATHS_H_
#define ALGORITHMS_KSHORTESTPATHS_H_

#include <string>
#include <map>
#include <iostream>

#include "../jnicall/JNI_CALL_JAVA_ALGORITHMs.h"
#include "../graphs/BasePath.h"

using namespace std;

class KShortestPaths{
public:
	KShortestPaths(const string graphFile);
	virtual ~KShortestPaths();

	map<string, BasePath*> getKShortestpaths(const string source,
			const string destination);

	/*
	 * used for calculate shortest path according to one constraint
	 * Then based on the results can be to filter the optimal path with complex constraints
	 * map<string, double> graphLinksInfo
	 *    string: link Id
	 *    double: link cost or link switching or link delay and so on
	 */
	map<string, BasePath*> getKShortestpaths(const string source,
				const string destination, map<string, double> graphLinksInfo);

	map<string, BasePath*> getKShortestpathsWithHopLimit(
			const string source, const string destination,
			map<string, double> graphLinksInfo, const int hopMinLimit,
			const int hopMaxLimit);

	map<string, BasePath*> getKShortestPathsWithKLimit(const string source,
			const string destination, const int k);

	map<string, BasePath*> getKShortestPathsWithHopMinimumLimit(
			const string source, const string destination,
			const int minimumHop);

	map<string, BasePath*> getKShortestPathsWithHopMaximumLimit(
			const string source, const string destination,
			const int maximumHop);

	map<string, BasePath*> getKShortestPathsWithHopRangeLimit(
			const string source, const string destination, const int minimumHop,
			const int maximumHop);

private:
	JNI_CALL_JAVA_ALGORITHMs * algs;
};

#endif /* ALGORITHMS_KSHORTESTPATHS_H_ */
