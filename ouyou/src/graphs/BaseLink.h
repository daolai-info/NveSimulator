/*
 * BaseLink.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_BASELINK_H_
#define GRAPHS_BASELINK_H_

#include <string>

#include <limits>
using namespace std;

class BaseLink {
public:
	static const double MAX_LINK_DISTANCE;

	BaseLink();
	BaseLink(string from, string to, double weight, double delay, double bandwidth, string fromDomain, string toDomain);
	BaseLink(string from, string to, double weight, double delay, double bandwidth, double availableBandwidth, string fromDomain, string toDomain );
	virtual ~BaseLink();

	double getBandwidth() const;
	void setBandwidth(double bandwidth);
	double getDelay() const;
	void setDelay(double delay);
	const string& getFrom() const;
	void setFrom(const string& from);
	const string& getFromDomain() const;
	void setFromDomain(const string& fromDomain);
	const string& getTo() const;
	void setTo(const string& to);
	const string& getToDomain() const;
	void setToDomain(const string& toDomain);
	double getWeight() const;
	void setWeight(double weight);
	double getAvailableBandwidth() const;
	void setAvailableBandwidth(double availableBandwidth);

private:
	string from;
	string to;

	double weight;
	double delay;
	double bandwidth;
	double availableBandwidth; //unused bandwidth

	string fromDomain;
	string toDomain;

};

#endif /* GRAPHS_BASELINK_H_ */
