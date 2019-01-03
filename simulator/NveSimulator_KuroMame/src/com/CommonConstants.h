/*
 * CommonConstants.h
 *
 *  Created on: Dec 3, 2016
 *      Author: ubean
 */

#ifndef COM_COMMONCONSTANTS_H_
#define COM_COMMONCONSTANTS_H_

#include <string>
#include <map>
#include <limits>


using namespace std;

class CommonConstants {
public:

	static const double MAX_PATH_DISTANCE;

	static const double DEFAULT_VIRTUAL_NODE_CAPABILITY;
	static const double DEFAULT_SUBSTRATE_NODE_CAPABILITY;

	static const string DEFAULT_NODE_ID;
	static const string DEFAULT_VIRTUAL_NETWORK_NO;

	static const double DEFAULT_SUBSTRATE_LINK_BANDWIDTH;
	static const double DEFAULT_SUBSTRATE_LINK_SWITCHING_ENERGY_EFFICIENCY;


	static const int DEFAULT_NUMBER_AMOUNT;
	static const int MIN_VNW_NODES_AMOUNT;
	static const int MAX_VNW_NODES_AMOUNT;
	static const int VNW_NODES_POISSON_MEAN;

	static const int MIN_VNW_LINK_BANDWIDTH;
	static const int MAX_VNW_LINK_BANDWIDTH;

	static const int PHY_NODES_AMOUNT;
	static const string VN_GRAPH_FILE_STR;


	static const int VNE_ELAPSED_TIME_UNITS;
	static const int MIN_VNW_DURATION_TIME_UNITS;
	static const int MAX_VNW_DURATION_TIME_UNITS;

	// virtual network embedding request time interval units: for example 100 second/request
	static const int VNW_REQUEST_TIME_INTERVAL_UNITS;

	static const int POISSON_DISTRIBUTION_REQUEST_DATA_RATE; // 320 kb/s
	static const int POISSON_DISTRIBUTION_REQUEST_AMOUNT_PER_SECOND; // 5 end user request for a virtual network in a time unit

	static const int MIN_END_USER_REQUEST_DURATION_TIME_UNITS;  //  one hour
	static const int MAX_END_USER_REQUEST_DURATION_TIME_UNITS;  // three hours


	static const double STATIC_VIRTUAL_LINK_RESOURCE_ALLOCATION_SPLITTING_RATIO;  // static allocation splitting ratio
	static const double DYNAMIC_VIRTUAL_LINK_RESOURCE_ALLOCATION_SPLITTING_RATIO;  // dynamic allocation splitting ratio

	static const string DIFFERENT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK;
	static const string DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK;

	static const string EQUAL_ENDUSER_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK;
	static const string NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK;

	// threshold value for reallocation
	static const double AVAILABLE_BANDWIDTH_THRESHOLD_VALUE_FOR_SPLIT_SUBSTRATE_PATH_OF_VIRTUAL_LINK;

	static const char* DEFAULT_RECORD_JSON;

	static int COMMON_VNW_ID;
	static const string COMMON_VNW_ID_FLAG;

	CommonConstants();
	virtual ~CommonConstants();


};

#endif /* COM_COMMONCONSTANTS_H_ */
