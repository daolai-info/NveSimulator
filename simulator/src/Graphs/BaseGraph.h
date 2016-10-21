/*
 * BaseGraph.h
 *
 *  Created on: Oct 16, 2016
 *      Author: abean
 */

#ifndef GRAPHS_BASEGRAPH_H_
#define GRAPHS_BASEGRAPH_H_

#include <iostream>
#include <vector>

#include "BaseNode.h"
#include "BaseLink.h"
using namespace std;

class BaseGraph {
public:
	BaseGraph();
	BaseGraph(const string * fileName);
	virtual ~BaseGraph();

	void addNodeIntoGraph(BaseNode * bNode);
	void deleteNodeFromGraph(BaseNode * bNode);

	void addLinkIntoGraph(BaseLink * bLink);
	void deleteLinkFromGraph(BaseLink * bLink);

	const vector<BaseLink*>& getLinks() const;
	void setLinks(const vector<BaseLink*>& links);

	const vector<BaseNode*>& getNodes() const;
	void setNodes(const vector<BaseNode*>& nodes);

	int getDirectedFlag() const;
	void setDirectedFlag(int directedFlag);

	int getLinksAmount() const;
	void setLinksAmount(int linksAmount) ;

	int getNodesAmount() const;
	void setNodesAmount(int nodesAmount);

	void clear();

protected:
	int directedFlag;   // directed graph: 1  undirected graph: 0
	int nodesAmount;
	int linksAmount;

	vector<BaseNode *> bNodes;
	vector<BaseLink *> bLinks;

private:
	void constructGraphFromGraphFile(const string fileName);

};

#endif /* GRAPHS_BASEGRAPH_H_ */
