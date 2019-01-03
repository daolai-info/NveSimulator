/*
 * VirNwNode.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#include "../graphs/VirNwNode.h"

VirNwNode::VirNwNode() {
	this->virNodeCapability = CommonConstants::DEFAULT_VIRTUAL_NODE_CAPABILITY; // represent computing abalitay such as CPU;
	this->virNodeMappedSubNodeId = CommonConstants::DEFAULT_NODE_ID;
	this->virNwNO = CommonConstants::DEFAULT_VIRTUAL_NETWORK_NO; // belong to which virtual network
	this->virNodeId = CommonConstants::DEFAULT_NODE_ID;
}

VirNwNode::~VirNwNode() {
	// TODO Auto-generated destructor stub
}

double VirNwNode::getVirNodeCapability() const {
	return virNodeCapability;
}

void VirNwNode::setVirNodeCapability(double virNodeCapability) {
	this->virNodeCapability = virNodeCapability;
}

const string& VirNwNode::getVirNodeId() const {
	return virNodeId;
}

void VirNwNode::setVirNodeId(const string& virNodeId) {
	this->virNodeId = virNodeId;
}

const string& VirNwNode::getVirNodeMappedSubNodeId() const {
	return virNodeMappedSubNodeId;
}

void VirNwNode::setVirNodeMappedSubNodeId(
		const string& virNodeMappedSubNodeId) {
	this->virNodeMappedSubNodeId = virNodeMappedSubNodeId;
}

const string& VirNwNode::getVirNwNo() const {
	return virNwNO;
}

void VirNwNode::setVirNwNo(const string& virNwNo) {
	virNwNO = virNwNo;
}

void VirNwNode::printVirNodeInfo(VirNwNode* virNode) {
	cout << "--------VirNode Information ------" << endl;

	cout << "Id: " << virNode->getVirNodeId() << endl;
	cout << "NwNo: " << virNode->getVirNwNo() << endl;
	cout << "Capability: " << virNode->getVirNodeCapability() << endl;
	cout << "MappedSubNodeId: " << virNode->getVirNodeMappedSubNodeId() << endl;

	cout << "--------VirNode Information ------" << endl;

}
