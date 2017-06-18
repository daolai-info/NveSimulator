/*
 * BaseNode.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_BASENODE_H_
#define GRAPHS_BASENODE_H_
#include <string>
#include "../com/CommonConstants.h"
using namespace std;

class BaseNode {
public:
	BaseNode();
	BaseNode(string nodeID);
	virtual ~BaseNode();
	string& getNodeId();
	void setNodeId(const string& nodeId);

protected:
	string nodeID;
};

#endif /* GRAPHS_BASENODE_H_ */
