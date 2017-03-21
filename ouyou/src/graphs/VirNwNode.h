/*
 * VirNwNode.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_VIRNWNODE_H_
#define GRAPHS_VIRNWNODE_H_

#include "../com/CommonConstants.h"
#include <iostream>

using namespace std;

class VirNwNode{
public:
	VirNwNode();
	virtual ~VirNwNode();

	double getVirNodeCapability() const;
	void setVirNodeCapability(double virNodeCapability);
	const string& getVirNodeId() const;
	void setVirNodeId(const string& virNodeId);
	const string& getVirNodeMappedSubNodeId() const;
	void setVirNodeMappedSubNodeId(const string& virNodeMappedSubNodeId);
	const string& getVirNwNo() const;
	void setVirNwNo(const string& virNwNo);

	static void printVirNodeInfo(VirNwNode* virNode);

private:

	double virNodeCapability;  // represent computing ability such as CPU;
	string virNodeMappedSubNodeId;
	string virNwNO; // belong to which virtual network
	string virNodeId;
};

#endif /* GRAPHS_VIRNWNODE_H_ */
