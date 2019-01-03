/*
 * SubNwNode.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#include "../graphs/SubNwNode.h"

using namespace std;

SubNwNode::SubNwNode() {
	this->subNodeId = CommonConstants::DEFAULT_NODE_ID;
	this->subNodeEmbeddedAmount = 0; //  amount of embedding virtual nodes on the substrate node
	this->subNodeCapability = CommonConstants::DEFAULT_SUBSTRATE_NODE_CAPABILITY;   // defined capability (computing capability)
	this->subNodeAvaiableCapability = CommonConstants::DEFAULT_SUBSTRATE_NODE_CAPABILITY ; //
	this->subNodeSwitchingEnergy = CommonConstants::DEFAULT_SUBSTRATE_LINK_SWITCHING_ENERGY_EFFICIENCY;
}

SubNwNode::~SubNwNode() {
	// TODO Auto-generated destructor stub
}

int SubNwNode::getSubNodeEmbeddedAmount() const {
	return subNodeEmbeddedAmount;
}

void SubNwNode::setSubNodeEmbeddedAmount(int subNodeEmbeddedAmount) {
	this->subNodeEmbeddedAmount = subNodeEmbeddedAmount;
}

double SubNwNode::getSubNodeAvaiableCapability() const {
	return subNodeAvaiableCapability;
}

void SubNwNode::setSubNodeAvaiableCapability(double subNodeAvaiableCapability) {
	this->subNodeAvaiableCapability = subNodeAvaiableCapability;
}

double SubNwNode::getSubNodeCapability() const {
	return subNodeCapability;
}

void SubNwNode::setSubNodeCapability(double subNodeCapability) {
	this->subNodeCapability = subNodeCapability;
}

const string& SubNwNode::getSubNodeId() const {
	return subNodeId;
}

void SubNwNode::setSubNodeId(const string& subNodeId) {
	this->subNodeId = subNodeId;
}

double SubNwNode::getSubNodeSwitchingEnergy() const {
	return subNodeSwitchingEnergy;
}

void SubNwNode::setSubNodeSwitchingEnergy(double subNodeSwitchingEnergy) {
	this->subNodeSwitchingEnergy = subNodeSwitchingEnergy;
}


void SubNwNode::printSubNodeInfo(SubNwNode* subNode) {
	cout << "--------SubNode Information Start------" << endl;

	cout << "    Id: " << subNode->getSubNodeId() << endl;
	cout << "    Capability: " << subNode->getSubNodeCapability()<< endl;
	cout << "    AvaiableCapability: " << subNode->getSubNodeAvaiableCapability()<< endl;
	cout << "    EmbeddedAmount: " << subNode->getSubNodeEmbeddedAmount()<< endl;
	cout << "    SwitchingEnergy: " << subNode->getSubNodeSwitchingEnergy()<< endl;

	cout << "--------SubNode Information End------" << endl;

}
