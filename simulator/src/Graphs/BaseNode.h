/*
 * BaseNode.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_BASENODE_H_
#define GRAPHS_BASENODE_H_

class BaseNode {
public:
	BaseNode();
	virtual ~BaseNode();

	int getNodeID();
	void setNodeID();
	double getNodeCapability();
	void setNodeCapability(double nCapability);

protected:
	int nodeID;
	int degree;
	int inDegree;
	int outDegree;
	double capability;
};

#endif /* GRAPHS_BASENODE_H_ */
