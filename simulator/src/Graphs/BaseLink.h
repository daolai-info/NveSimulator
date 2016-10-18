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

	int getFrom() const;
	void setFrom(int from);

	int getTo() const;
	void setTo(int to);

	double getWeight() const;
	void setWeight(double weight);

protected:
	int from;
	int to;
	double weight;
};

#endif /* GRAPHS_BASELINK_H_ */
