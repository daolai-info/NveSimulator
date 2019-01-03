/*
 * ShortestPathWithDijkstra.h
 *
 *  Created on: 2016/10/24
 *      Author: zhangshanming
 */

#ifndef ALGORITHMS_SHORTESTPATH_H_
#define ALGORITHMS_SHORTESTPATH_H_
#include <iostream>
#include <limits>
#include <string>
#include <stack>
#include <algorithm>

#include "../graphs/BaseGraph.h"
#include "../graphs/BasePath.h"
#include "../com/CommonConstants.h"

using namespace std;

class ShortestPath {

public:

	ShortestPath();
	virtual ~ShortestPath();

	static BasePath* getShortestPathWithDijkstra(const BaseGraph *bGraph,
			const string source, const string destination);

};

#endif /* ALGORITHMS_SHORTESTPATH_H_ */
