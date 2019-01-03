/*
 * SubNwGraph.h
 *
 *  Created on: Oct 16, 2016
 *      Author: abean
 */

#ifndef GRAPHS_SUBNWGRAPH_H_
#define GRAPHS_SUBNWGRAPH_H_
#include "../graphs/BaseGraph.h"
#include "../graphs/BaseLink.h"
#include "../graphs/SubNwLink.h"
#include "../graphs/SubNwNode.h"
#include "../com/MessageSet.h"
#include <string>

class SubNwGraph: public BaseGraph {
public:

	SubNwGraph();
	SubNwGraph(const string filename);
	virtual ~SubNwGraph();

	SubNwLink* getSubLink(const string linkId);
	map<string, SubNwLink*>& getSubLinks();
	void setSubLinks(const map<string, SubNwLink*>& subLinks);
	map<string, SubNwNode*>& getSubNodes();
	SubNwNode* getSubNode(const string nodeId);
	void setSubNodes(const map<string, SubNwNode*>& subNodes);

	SubNwGraph* copySubstrateNetwork();
	static void printSubstrateNetwork(SubNwGraph* subNw);

	static void checkSubstrateLinksResource(SubNwGraph* subNw);

	// used after last processing.
	static void checkSubstrateLinksResourceStatus(SubNwGraph* subNw);


private:

	map<string, SubNwLink*> subLinks;
	map<string, SubNwNode*> subNodes;
};

#endif /* GRAPHS_SUBNWGRAPH_H_ */
