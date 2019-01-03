/*
 * CommonConstants.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: ubean
 */

#include "CommonConstants.h"

using namespace std;

const double CommonConstants::MAX_PATH_DISTANCE = numeric_limits<double>::max();

const double CommonConstants::DEFAULT_VIRTUAL_NODE_CAPABILITY = 1.0; // one computing unit(CPU unit): 1.0
const double CommonConstants::DEFAULT_SUBSTRATE_NODE_CAPABILITY = 1000.0; // represent computing capability or embedding capability, how many virtual nodes can be embedded into the substrate node

const string CommonConstants::DEFAULT_NODE_ID = "*"; //
const string CommonConstants::DEFAULT_VIRTUAL_NETWORK_NO = "0"; //

const double CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH = 1024; // kb, 1024M

/*
 * 1njoule = pow(10,-9) joule/bit
 * 1 bit = pow(10, -6) megabit
 *  10 nJoule/bit = pow(10, -8) joule/ pow(10, -6) megabit
 *                = pow(10, -8)/pow(10,-6) joule/megabit
 *                = pow(10, -2) joule/megabit
 *                = 0.01 joule/megabit
 */

const double CommonConstants::DEFAULT_SUBSTRATE_LINK_SWITCHING_ENERGY_EFFICIENCY =
		0.01; // ethernet switch 0.01 joule/megabit/second

const int CommonConstants::DEFAULT_NUMBER_AMOUNT = 1000;

const int CommonConstants::MIN_VNW_NODES_AMOUNT = 3;
const int CommonConstants::MAX_VNW_NODES_AMOUNT = 20; //
const int CommonConstants::VNW_NODES_POISSON_MEAN = 6; // usually  virtual network request with nodes 6

const int CommonConstants::MIN_VNW_LINK_BANDWIDTH = 30;  //30M
const int CommonConstants::MAX_VNW_LINK_BANDWIDTH = 100; //100M

const int CommonConstants::PHY_NODES_AMOUNT = 50;
const string CommonConstants::VN_GRAPH_FILE_STR = "./topologies/VNW_*.brite";

const int CommonConstants::VNE_ELAPSED_TIME_UNITS = 5000;  // 24 hours
const int CommonConstants::MIN_VNW_DURATION_TIME_UNITS = 15;  //  one hour 3600
const int CommonConstants::MAX_VNW_DURATION_TIME_UNITS = 20; // three hours: 10800

const int CommonConstants::VNW_REQUEST_TIME_INTERVAL_UNITS = 10; // time interval : 100 seconds

const int CommonConstants::POISSON_DISTRIBUTION_REQUEST_DATA_RATE = 320; // 320 kb/s translate to Mb/s at applying
const int CommonConstants::POISSON_DISTRIBUTION_REQUEST_AMOUNT_PER_SECOND = 15; // 5 end user request for a virtual network in a time unit

//end-user request duration time from 200s to 1000s , the most is 200 ~ 400 seconds
const int CommonConstants::MIN_END_USER_REQUEST_DURATION_TIME_UNITS = 10; //  one hour
const int CommonConstants::MAX_END_USER_REQUEST_DURATION_TIME_UNITS = 15; // three hours

const double CommonConstants::STATIC_VIRTUAL_LINK_RESOURCE_ALLOCATION_SPLITTING_RATIO =
		0.5;  // static allocation splitting ratio

const double CommonConstants::DYNAMIC_VIRTUAL_LINK_RESOURCE_ALLOCATION_SPLITTING_RATIO =
		0.3;  // dynamic allocation splitting ratio

const string CommonConstants::DIFFERENT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK =
		"DIFFERENT";
const string CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK =
		"DISTJOINT";

const string CommonConstants::EQUAL_ENDUSER_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK =
		"1:1";
const string CommonConstants::NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK =
		"1:0";

/*
 *  more than 30%, then reallocate the substrate path resource
 */
const double CommonConstants::AVAILABLE_BANDWIDTH_THRESHOLD_VALUE_FOR_SPLIT_SUBSTRATE_PATH_OF_VIRTUAL_LINK =
		0.3;

const char* CommonConstants::DEFAULT_RECORD_JSON =
		"{\"name\":\"null\", \"description\" : \"description\", \"CongestionRecords\": [], \"AmountNws\" : [],\"AvailableNws\" : [], \"DataSwichingEnergy\": [], \"ComDataSwichingEnergy\": [], \"RevenueEmbeddedLinks\": [], \"RevenueEmbeddedBandwidth\": [], \"TransportedDataVolume\": [], \"ComTransportedDataVolume\": [], \"CurrentEndUserRequests\": []}";


int CommonConstants::COMMON_VNW_ID = 0;
const string CommonConstants::COMMON_VNW_ID_FLAG = "COM#";

CommonConstants::CommonConstants() {
	// TODO Auto-generated constructor stub

}

CommonConstants::~CommonConstants() {
	// TODO Auto-generated destructor stub
}

