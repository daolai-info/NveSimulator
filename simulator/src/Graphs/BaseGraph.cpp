/*
 * BaseGraph.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: abean
 */

#include "BaseGraph.h"

BaseGraph::BaseGraph() {
	// TODO Auto-generated constructor stub

}

BaseGraph::~BaseGraph() {
	// TODO Auto-generated destructor stub
}

const vector<BaseLink*>& BaseGraph::getLinks() const {
	return bLinks;
}

void BaseGraph::setLinks(const vector<BaseLink*>& links) {
	bLinks = links;
}

const vector<BaseNode*>& BaseGraph::getNodes() const {
	return bNodes;
}

void BaseGraph::setNodes(const vector<BaseNode*>& nodes) {
	bNodes = nodes;
}

int BaseGraph::getDirectedFlag() const {
	return directedFlag;
}

void BaseGraph::setDirectedFlag(int directedFlag) {
	this->directedFlag = directedFlag;
}

int BaseGraph::getLinksAmount() const {
	return linksAmount;
}

void BaseGraph::setLinksAmount(int linksAmount) {
	this->linksAmount = linksAmount;
}

int BaseGraph::getNodesAmount() const {
	return nodesAmount;
}

void BaseGraph::setNodesAmount(int nodesAmount) {
	this->nodesAmount = nodesAmount;
}
