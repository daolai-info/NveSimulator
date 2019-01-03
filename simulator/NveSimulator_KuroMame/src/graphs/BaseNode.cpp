/*
 * BaseNode.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#include "../graphs/BaseNode.h"

BaseNode::BaseNode() {
	// TODO Auto-generated constructor stub
	this->nodeID = CommonConstants::DEFAULT_NODE_ID; // unknown node ID
}

BaseNode::BaseNode(string nodeID) {
	// TODO Auto-generated constructor stub
	this->nodeID = nodeID;
}

BaseNode::~BaseNode() {
	// TODO Auto-generated destructor stub
}

string& BaseNode::getNodeId() {
	return nodeID;
}

void BaseNode::setNodeId(const string& nodeId) {
	nodeID = nodeId;
}
