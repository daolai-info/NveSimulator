/*
 * VirNwLink.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_VIRNWLINK_H_
#define GRAPHS_VIRNWLINK_H_

#include <vector>
#include <iostream>
#include "../graphs/BaseLink.h"
#include "../graphs/BasePath.h"

using namespace std;

class VirNwLink {
public:
	VirNwLink();
	virtual ~VirNwLink();


	string& getVirFromDomain();
	void setVirFromDomain(const string& virFromDomain);

	double getVirLinkAvailableBandwidth();
	void setVirLinkAvailableBandwidth(double virLinkAvailableBandwidth);

	double getVirLinkBandwidth();
	void setVirLinkBandwidth(double virLinkBandwidth);

	double getVirLinkDelay();
	void setVirLinkDelay(double virLinkDelay);

	string getVirLinkFrom();
	void setVirLinkFrom(const string virLinkfrom);

	string& getVirLinkId();
	void setVirLinkId(const string& virLinkId);

	vector<BasePath*>& getVirLinkSubPaths();
	void setVirLinkSubPaths(const vector<BasePath*>& virLinkSubPaths);
	void addVirLinkSubPath(BasePath* virLinkSubPath);

	string& getVirLinkTo();
	void setVirLinkTo(const string& virLinkto);

	double getVirLinkWeight();
	void setVirLinkWeight(double virLinkWeight);

	string& getVirNwNo();
	void setVirNwNo(const string& virNwNo);

	string& getVirToDomain();
	void setVirToDomain(const string& virToDomain);
	int getVirLinkSubPathHops();
	void setVirLinkSubPathHops(int virLinkSubPathHops);
	double getVirLinkSwitchingEnergy();
	void setVirLinkSwitchingEnergy(double virLinkSwitchingEnergy);

	static void printVirNwLinkInfo(VirNwLink* virLink);

private:
	string virLinkID;
	string virNwNo;


	string virLinkFrom;
	string virLinkTo;
	string virFromDomain;
	string virToDomain;

	int virLinkSubPathHops;
	double virLinkSwitchingEnergy;
	double virLinkWeight;
	double virLinkDelay;
	double virLinkBandwidth;
	double virLinkAvailableBandwidth; //unused bandwidth


	vector<BasePath*> virLinkSubPaths;
};

#endif /* GRAPHS_VIRNWLINK_H_ */
