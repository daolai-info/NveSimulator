/*
 * BaseGraph.h
 *
 *  Created on: Oct 16, 2016
 *      Author: abean
 */

#ifndef GRAPHS_BASEGRAPH_H_
#define GRAPHS_BASEGRAPH_H_

#include <vector>
#include <map>
#include <set>
#include <iostream>

#include <limits>

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "../graphs/BaseLink.h"
#include "../graphs/BaseNode.h"
#include "../graphs/BasePath.h"

using namespace std;

class BaseGraph {
public:

	BaseGraph();
	BaseGraph(const string fileName);
	virtual ~BaseGraph();

	void clear();

	string makeLinkId(const string from, const string to) const;

	const map<string, vector<string> >& getAdjacencyNodes() const;
	void setAdjacencyNodes(const map<string, vector<string> >& adjacencyNodes);
	int getDirectedFlag() const;
	void setDirectedFlag(int directedFlag);
	const vector<BaseLink*>& getLinkPtrs() const;
	void setLinkPtrs(const vector<BaseLink*>& linkPtrs);
	const map<string, BaseLink*>& getLinks() const;
	const BaseLink* getLink(const string linkId) const;

	void setLinks(const map<string, BaseLink*>& links);
	int getLinksAmount() const;
	void setLinksAmount(int linksAmount);
	const vector<BaseNode*>& getNodePtrs() const;
	void setNodePtrs(const vector<BaseNode*>& nodePtrs);
	const map<string, BaseNode*>& getNodes() const;
	void setNodes(const map<string, BaseNode*>& nodes);
	int getNodesAmount() const;
	void setNodesAmount(int nodesAmount);

	template<class T>
	void printVector(const vector<T> vtr);
	template<class T1, class T2>
	void printMap(const map<T1, T2> vtr);
	template<class T1, class T2>
	void printMapVector(const map<T1, vector<T2> > vtr);
	template<class T1>
	void printLinkDetails(const map<T1, BaseLink*> vtr);

protected:
	int directedFlag;   // directed graph: 1  undirected graph: 0
	int nodesAmount;
	int linksAmount;

	vector<BaseNode *> NodePtrs;
	vector<BaseLink *> LinkPtrs;
	map<string, BaseNode*> Nodes;
	map<string, BaseLink*> Links;
	map<string, vector<string> > adjacencyNodes;

private:
	void constructGraphFromGraphFile(const string fileName);

	void addAdjacencyNodes(const string from, const string to);
	void addNodeToGraph(const string node);
	void addLinkToGraph(const string linkId, const string from, const string to,
			const double cost, const double delay, const double bandwidth,
			const string fromDomain, const string toDomain);

};

#endif /* GRAPHS_BASEGRAPH_H_ */
