/*
 * ShortestPathWithFloyd.h
 *  Floyd-Warshall algorithm
 *  Floyd-Warshall算法的时间复杂度为O(N3)，空间复杂度为O(N2)
 *  Created on: 2016/10/24
 *      Author: zhangshanming
 */

#ifndef ALGORITHMS_SHORTESTPATHWITHFLOYD_H_
#define ALGORITHMS_SHORTESTPATHWITHFLOYD_H_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include "../graphs/BaseGraph.h"
#include "../graphs/BaseLink.h"
#include "../graphs/BasePath.h"
using namespace std;

class ShortestPathWithFloyd {
public:
	ShortestPathWithFloyd();
	ShortestPathWithFloyd(const BaseGraph * bGraph);
	virtual ~ShortestPathWithFloyd();

	void calculateShortestPathsWithFloyd(const BaseGraph * bGraph);
	BasePath* getShortestPath(const string& source, const string& destination);

	void printShortestPath(const string& source, const string& destination, map<string, string> path);
private:
	void getPathInfo(const string& source, const string& destination, map<string, string> path, vector<string>* pIter);
	map < string, BasePath* > paths;
};


#endif /* ALGORITHMS_SHORTESTPATHWITHFLOYD_H_ */
