/*
 * ShortestPathWithDijkstra.h
 *
 *  Created on: 2016/10/24
 *      Author: zhangshanming
 */

#ifndef ALGORITHMS_SHORTESTPATHWITHDIJKSTRA_H_
#define ALGORITHMS_SHORTESTPATHWITHDIJKSTRA_H_
#include <iostream>
#include <limits>
#include <string>
#include <stack>
#include <algorithm>

#include "../graphs/BaseGraph.h"
#include "../graphs/BasePath.h"
#include "../com/CommonConstants.h"

using namespace std;

class ShortestPathWithDijkstra {

public:

	ShortestPathWithDijkstra();
	virtual ~ShortestPathWithDijkstra();

	BasePath* getShortestPath(const BaseGraph *bGraph, const string source, const string destination);
};


#endif /* ALGORITHMS_SHORTESTPATHWITHDIJKSTRA_H_ */
