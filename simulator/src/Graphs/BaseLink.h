/*
 * BaseLink.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_BASELINK_H_
#define GRAPHS_BASELINK_H_

class BaseLink {
public:
	BaseLink();
	virtual ~BaseLink();


	void setLinkFrom( int lFrom );
	int getLinkFrom();

	void setLinkFrom( int lTo);
	int getLinkTo();

	void setLinkWeight( double lWeight);
	double getLinkWeight();

protected:
	int from;
	int to;
	double weight;
};

#endif /* GRAPHS_BASELINK_H_ */
