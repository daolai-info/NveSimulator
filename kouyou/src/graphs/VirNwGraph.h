/*
 * VirNwGraph.h
 *
 *  Created on: Oct 16, 2016
 *      Author: abean
 */

#ifndef GRAPHS_VIRNWGRAPH_H_
#define GRAPHS_VIRNWGRAPH_H_
#include "../graphs/BaseGraph.h"
#include "../graphs/VirNwLink.h"
#include "../graphs/VirNwNode.h"
#include "../com/MessageSet.h"

#include <string>

using namespace std;

class VirNwGraph: public BaseGraph {
public:
	VirNwGraph();
	VirNwGraph(const string fileName);
	virtual ~VirNwGraph();
	map<string, VirNwLink*>& getVirLinks();
	VirNwLink* getVirLink(const string virLinkId);

	void setVirLinks(const map<string, VirNwLink*>& virLinks);
	map<string, VirNwNode*>& getVirNodes();
	void setVirNodes(const map<string, VirNwNode*>& virNodes);

	map<string, string>& getMappedPhyNodes();
	void setMappedPhyNodes(const map<string, string>& mappedPhyNodes);

	void setMappedPhyNodes(const vector<string>& mappedPhyNodes);

	const string& getVirNwNo() const;
	void setVirNwNo(const string& virNwNo);

	VirNwGraph* copyVirtualNetwork();

	int getVirNwDurationTimeUnits() const;
	void setVirNwDurationTimeUnits(int virNwDurationTimeUnits);
	int getVirNwEndTiming() const;
	void setVirNwEndTiming(int virNwEndTiming);
	int getVirNwStartTiming() const;
	void setVirNwStartTiming(int virNwStartTiming);

	static void printVirtualNetworkInfo(VirNwGraph* virNw);
	static void checkVirtualLinksResource(VirNwGraph* virNw);

	/*
	 * release a virtual network release while embedding failure.
	 */
	static void ReleaseVirtualNetworkResource(VirNwGraph* virNw);

private:
	map<string, VirNwLink*> virLinks; // string : linkId  VirNwLink* : virtual link pointer
	map<string, VirNwNode*> virNodes; // string : nodeId  virNwNode* : virtual node pointer
	map<string, string> mappedPhyNodes; // string : virtual network nodeId string : substrate network nodeId

	string virNwNO; // string : virtual network Id

	int virNwDurationTimeUnits;
	int virNwStartTiming;
	int virNwEndTiming;  //  equals to sum( durationTimeUnits + strartTiming );
};

#endif /* GRAPHS_VIRNWGRAPH_H_ */
