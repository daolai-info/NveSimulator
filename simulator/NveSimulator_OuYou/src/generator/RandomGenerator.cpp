/*
 * Distributions.cpp
 *
 *  Created on: 2016/11/23
 *      Author: zhangshanming
 */

#include "RandomGenerator.h"
//#include "../matplot/matplotlibcpp.h"
//namespace plt = matplotlibcpp;

RandomGenerator::RandomGenerator() {
	//getVirtualNetworkWithUniformDistribution();
	//PoissonDistribution();

}

RandomGenerator::~RandomGenerator() {
	// TODO Auto-generated destructor stub
}

/*
 *  get duration time for each virtual network, now used the uniformDistribution  not exponential distribution
 */
int RandomGenerator::getVirtualNetworkDurationTimeWithUniformDistribution() {
	return UniformDistribution(CommonConstants::MIN_VNW_DURATION_TIME_UNITS,
			CommonConstants::MAX_VNW_DURATION_TIME_UNITS);
}
/*
 *  get end-user request amount for each virtual network in a timing unit;
 */
int RandomGenerator::getUserVideoRequestsAmountWithPoissonDistribution() {
	return PoissonDistribution(
			CommonConstants::POISSON_DISTRIBUTION_REQUEST_AMOUNT_PER_SECOND);
}

/*
 *  get each video end-user request data rate
 */
int RandomGenerator::getUserVideoRequestDataRateWithPoissonDistribution() {
	return PoissonDistribution(
			CommonConstants::POISSON_DISTRIBUTION_REQUEST_DATA_RATE);
}

/*
 * get each end-user video request duration time
 */
int RandomGenerator::getUserVideoRequestDurationTimeWithExponentialDistribution() {
	return ExponentialDistribution(
			CommonConstants::MIN_END_USER_REQUEST_DURATION_TIME_UNITS,
			CommonConstants::MAX_END_USER_REQUEST_DURATION_TIME_UNITS);
}

vector<string> RandomGenerator::getUserVideoRequestSourceAndDestinationWithUniformDistribution(
		const int amount, const int minNodeId, int maxNodeId) {
	return UniformDistribution(amount, minNodeId, maxNodeId);
}

/*
 * return each virtual network embedding request timing list
 */
vector<int> RandomGenerator::getVirtualNetworkEmebeddingRequestsWithExponentialDistribution() {
	return ExponentialDistribution();
}

/*
 * get virtual network link bandwidth for embedding virtual network
 */
int RandomGenerator::getVirtualNetworkLinkBandwidthWithUniformDistribution() {
	unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(SEED);
	uniform_int_distribution<int> distribution(
			CommonConstants::MIN_VNW_LINK_BANDWIDTH,
			CommonConstants::MAX_VNW_LINK_BANDWIDTH);
	return distribution(generator);
}
/*
 * return map<string, vector<string> >
 *    string :  virtual network topology file
 *    vector<string>  substrate network nodes mapped into virtual network nodes
 */
map<string, vector<string> > RandomGenerator::getVirtualNetworkWithPoissonAndUniformDistribution() {
	vector<string> mappedPhyNodes; // mapped physical nodes

	//int number = UniformDistribution(CommonConstants::MIN_VNW_NODES_AMOUNT,
	//		CommonConstants::MAX_VNW_NODES_AMOUNT);
	int number = 0;
	do {
		number = PoissonDistribution(CommonConstants::VNW_NODES_POISSON_MEAN);
	} while (number < CommonConstants::MIN_VNW_NODES_AMOUNT
			|| number > CommonConstants::MAX_VNW_NODES_AMOUNT);

	//int number = 3;
	mappedPhyNodes = UniformDistribution(number, 0,
			CommonConstants::PHY_NODES_AMOUNT - 1);

	string strNum = CommonFunctions::CoverIntegerToString(number);
	string str = CommonConstants::VN_GRAPH_FILE_STR;
	size_t found = str.rfind("*");
	string fileName = "";
	if (found != std::string::npos) {
		fileName = str.replace(found, 1, strNum);
	}

	map<string, vector<string> > vnwNodes;
	vnwNodes.insert(pair<string, vector<string> >(fileName, mappedPhyNodes));
	return vnwNodes;
}

// enable to generate end user request's data rate . kb/s
// distribution parameter, mean = 320  represent 320kb/s based on youtube datasheet

// enable to generate end user request amount for one virtual network  in one time units
// distribution parameter, mean = 5
int RandomGenerator::PoissonDistribution(const int mean) {
	unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(SEED);
	std::poisson_distribution<int> distribution(mean);
	return distribution(generator);

//	vector<int> numbers;
//	for (int index = 0; index < vNwAmount;
//			++index) {
//		int num = distribution(generator);
//		numbers.push_back(num);
//		cout << " index: " << index << " " << num << endl;
//	}
//	showDistribution(numbers);
}

/*
 * generator virtual network embedding request arrive timing
 *   the last request' duration time is at least one hour 3600s
 */
vector<int> RandomGenerator::ExponentialDistribution() {
	unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(SEED);
	exponential_distribution<double> distribution(10.0);
	vector<int> numbers;
	for (int index = 0;
			index < (CommonConstants::VNE_ELAPSED_TIME_UNITS);  //3600
			index = index + CommonConstants::VNW_REQUEST_TIME_INTERVAL_UNITS) {
		int num = (int) (distribution(generator)
				* CommonConstants::VNW_REQUEST_TIME_INTERVAL_UNITS);
		numbers.push_back(index + num);
		//cout << " index: " << index << " " << index + num << endl;
	}
	//showDistribution(numbers);
	return numbers;
}

// enable to generater end user request duration time from 200s to 1000s , the most is 200 ~ 400 seconds
int RandomGenerator::ExponentialDistribution(const int min, const int max) {
	unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(SEED);
	exponential_distribution<double> distribution(5.0);

	return min + (int) (distribution(generator) * 1000);
}

/*
 * generate random numbers they are whether repeat according to amount and max - min
 */
vector<string> RandomGenerator::UniformDistribution(const int amount,
		const int min, const int max) {
	unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(SEED);
	uniform_int_distribution<int> distribution(min, max);   // y
	vector<string> items;
	while ((int) items.size() < amount) {
		string number = CommonFunctions::CoverIntegerToString(
				distribution(generator));
		if (amount <= (max - min)) {
			if (find(items.begin(), items.end(), number) == items.end()) {
				items.push_back(number);
			}
		} else {
			items.push_back(number);
		}
	}
	//showDistribution(items);
	return items;
}

// enable to generate mapping substrate node for each virtual node
// enable to generate end user request duration time.

int RandomGenerator::UniformDistribution(const int min, const int max) {
	unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(SEED);
	uniform_int_distribution<int> distribution(min, max);   // y
	return distribution(generator);
}

void RandomGenerator::showDistribution(vector<int> items) {
	//plt::plot(items);
	//plt::show();
}

void RandomGenerator::showDistribution(vector<double> items) {
	//plt::plot(items);
	//plt::show();
}
void RandomGenerator::printVirtualNetworksDetail(const vector<int> nodes) {
	vector<int>::const_iterator nIter = nodes.begin();

	for (; nIter != nodes.end(); ++nIter) {
		cout << "  " << *nIter;
	}
	cout << endl;
}
