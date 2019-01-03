/*
 * Distributions.h
 *
 *  Created on: 2016/11/23
 *      Author: zhangshanming
 */

#ifndef GENERATOR_RANDOMGENERATOR_H_
#define GENERATOR_RANDOMGENERATOR_H_
#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

#include "../com/CommonFunctions.h"
#include "../com/CommonConstants.h"

using namespace std;

class RandomGenerator {
public:

	RandomGenerator();

	static int getUserVideoRequestsAmountWithPoissonDistribution();
	static int getUserVideoRequestDataRateWithPoissonDistribution();
	static int getUserVideoRequestDurationTimeWithExponentialDistribution();
	static vector<string> getUserVideoRequestSourceAndDestinationWithUniformDistribution(const int amount, const int minNodeId, int maxNodeId);
	static vector<int> getVirtualNetworkEmebeddingRequestsWithExponentialDistribution();

	static map<string, vector<string> > getVirtualNetworkWithPoissonAndUniformDistribution();
	static int getVirtualNetworkLinkBandwidthWithUniformDistribution();

	/*
	 * generate duration time for a virtual network by uniform distribution
	 *   duration time during one hour(3600s) and three hour(10800s)
	 *   return :  duration time units
	 *   input :
	 */
	static int getVirtualNetworkDurationTimeWithUniformDistribution();


	static int PoissonDistribution(const int mean);

	static vector<int> ExponentialDistribution();
	static int ExponentialDistribution( const int min, const int max);

	/*
	 * generator num random integers in a range [min,max]
	 */
	static vector<string> UniformDistribution(const int amount, const int min,
			const int max);
	static int UniformDistribution(const int min, const int max);

	static void showDistribution(vector<double> items);
	static void showDistribution(vector<int> items);
	static void printVirtualNetworksDetail(const vector<int> nodes);

	virtual ~RandomGenerator();
private:
	int pTopolgy[20]; //20 is number of random generated topologies that include node from 3~20.
};

#endif /* GENERATOR_RANDOMGENERATOR_H_ */
