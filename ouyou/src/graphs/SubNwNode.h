/*
 * SubNwNode.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_SUBNWNODE_H_
#define GRAPHS_SUBNWNODE_H_
#include <string>
#include <iostream>

#include "../com/CommonConstants.h"

using namespace std;

class SubNwNode {
public:
	SubNwNode();
	virtual ~SubNwNode();

	int getSubNodeEmbeddedAmount() const;
	void setSubNodeEmbeddedAmount(int subEmbeddedAmount);
	double getSubNodeAvaiableCapability() const;
	void setSubNodeAvaiableCapability(double subNodeAvaiableCapability);
	double getSubNodeCapability() const;
	void setSubNodeCapability(double subNodeCapability);
	const string& getSubNodeId() const;
	void setSubNodeId(const string& subNodeId);
	double getSubNodeSwitchingEnergy() const;
	void setSubNodeSwitchingEnergy(double subNodeSwitchingEnergy);

	static void printSubNodeInfo(SubNwNode* subNode);

private:
	string subNodeId;
	int subNodeEmbeddedAmount; //  amount of embedding virtual nodes on the substrate node

	double subNodeCapability;   // defined capability (computing capability)
	double subNodeAvaiableCapability; //

	double subNodeSwitchingEnergy;
};

#endif /* GRAPHS_SUBNWNODE_H_ */
