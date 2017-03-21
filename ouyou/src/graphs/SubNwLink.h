/*
 * SubNwLink.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_SUBNWLINK_H_
#define GRAPHS_SUBNWLINK_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>

using namespace std;


class SubNwLink {
public:
	SubNwLink();
	SubNwLink( double swEnergy, double maxBandwidth);
	virtual ~SubNwLink();

	double getSubLinkAvailableBandwidth() const;
	void setSubLinkAvailableBandwidth(double subLinkAvailableBandwidth);
	double getSubLinkBandwidth() const;
	void setSubLinkBandwidth(double subLinkBandwidth);
	double getSubLinkDelay() const;
	void setSubLinkDelay(double subLinkDelay);
	double getSubLinkEmbeddableBandwidth() const;
	void setSubLinkEmbeddableBandwidth(double subLinkEmbeddableBandwidth);
	const string& getSubLinkFrom() const;
	void setSubLinkFrom(const string& subLinkFrom);
	const string& getSubLinkFromDomain() const;
	void setSubLinkFromDomain(const string& subLinkFromDomain);
	double getSubLinkSwitchingEnergy() const;
	void setSubLinkSwitchingEnergy(double subLinkSwitchingEnergy);
	const string& getSubLinkTo() const;
	void setSubLinkTo(const string& subLinkTo);
	const string& getSubLinkToDomain() const;
	void setSubLinkToDomain(const string& subLinkToDomain);
	double getSubLinkWeight() const;
	void setSubLinkWeight(double subLinkWeight);

	multimap<string, string>& getSubLinkMappedVirLinks();
	void setSubLinkMappedVirLink(string virNwNo, string virLinkId);
	void setSubLinkMappedVirLinks(multimap<string, string> subLinkMappedVirLinks);

	static void printSubLinkInfo(SubNwLink* subLink);

private:
	string subLinkFrom;
	string subLinkTo;

	string subLinkFromDomain;
	string subLinkToDomain;

	double subLinkWeight; // cost or distance
	double subLinkDelay;
	double subLinkSwitchingEnergy; // the switching energy of link

	double subLinkBandwidth;
	double subLinkAvailableBandwidth; //unused bandwidth
	double subLinkEmbeddableBandwidth; // available bandwidth: represents non-embedded bandwidth, not belong to any virtual link. It is not actual unused traffic bandwidth of substrate link.

	multimap<string,string> subLinkMappedVirLinks; // first string: virtual network no, second string: virtual link ID
};

#endif /* GRAPHS_SUBNWLINK_H_ */
