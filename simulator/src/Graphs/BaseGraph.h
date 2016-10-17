/*
 * BaseGraph.h
 *
 *  Created on: Oct 16, 2016
 *      Author: abean
 */

#ifndef GRAPHS_BASEGRAPH_H_
#define GRAPHS_BASEGRAPH_H_

class BaseGraph {
public:
	BaseGraph();
	virtual ~BaseGraph();
<<<<<<< HEAD

	void setNodesAmount(int nAmount);
	int getNodesAmount();

	void setLinksAmount(int lAmount);
	int getLinksAmount();

	void addNodeIntoGraph(BaseNode * bNode);
	void deleteNodeFromGraph(BaseNode * bNode);

	void addLinkIntoGraph(BaseLink * bLink);
	void deleteLinkFromGraph(BaseLink * bLink);

protected:
	int directedFlag;   // directed graph: 1  undirected graph: 0
	int nodesAmount;
	int linksAmount;

	vector<BaseNode *> bNodes;
	vector<BaseLink *> bLinks;
=======
>>>>>>> 536deaa461219e1131939774ede991ed08bf6d63
};

#endif /* GRAPHS_BASEGRAPH_H_ */
