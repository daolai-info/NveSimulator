/*
 * SubNwLink.h
 *
 *  Created on: Oct 17, 2016
 *      Author: abean
 */

#ifndef GRAPHS_SUBNWLINK_H_
#define GRAPHS_SUBNWLINK_H_
#include "BaseLink.h"

class SubNwLink: public BaseLink {
public:
	SubNwLink();
	virtual ~SubNwLink();

	double getAvailableBw() const;
	void setAvailableBw(double availableBw);
	double getDelay() const;
	void setDelay(double delay);
	double getMaxBw() const;
	void setMaxBw(double maxBw);
	double getSwitchingEnergy() const;
	void setSwitchingEnergy(double switchingEnergy);

	void increaseLinkAvailableBandwidth( double variationBw);
	void decreaseLinkAvailableBandwidth( double variationBw);

protected:
	double delay;
	double switchingEnergy;
	double maxBw;     //
	double availableBw; // represents non-embedded bandwidth, not belong to any virtual link. It is not actual unused traffic bandwidth of substrate link.
};

#endif /* GRAPHS_SUBNWLINK_H_ */
