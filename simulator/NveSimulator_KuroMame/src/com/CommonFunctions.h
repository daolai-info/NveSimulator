/*
 * CommonFunctions.h
 *
 *  Created on: Dec 3, 2016
 *      Author: ubean
 */

#ifndef COM_COMMONFUNCTIONS_H_
#define COM_COMMONFUNCTIONS_H_

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>        // std::put_time
#include <thread>         // std::this_thread::sleep_until
#include <chrono>         // std::chrono::system_clock
#include <ctime>          // std::time_t, std::tm, std::localtime, std::mktime
#include <map>

#include "../generator/RandomGenerator.h"
#include "../com/MessageSet.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/document.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/filewritestream.h"
#include "../com/CommonConstants.h"

using namespace std;
using namespace rapidjson;

class CommonFunctions {
public:
	CommonFunctions();
	virtual ~CommonFunctions();

	static string makeLinkId(const string from, const string to);
	static string makePathId(const string from, const string to);
	static string makeEndUserRequestId(const string vNwNo, const string timing,
			const string seqNo);

	static vector<string> stringSplit(const string &str, char sep);

	static string CoverIntegerToString(const int integer);
	static string CoverDoubleToString(const double data);

	static void SleepThread(int hours, int minutes, int seconds);

	static void FAILURE_EXIT();

	static void printVector(vector<string> vtr);
	static void printMap(map<string, string> vtr);
	static void printMapInteger(map<int, int> vtr);
	static void printMultimapMapIntegerString(multimap<int, string> vtr);

	static vector<int> generateVirtualNetworkRequests();

	static void printFailureMessageAndExit(const string errorMessage,
			const string functionName);
	static void printWarningMessage(const string warningMessage);
	static void printInfoMessage(const string InfoMessage);

	static void OutputRecordsIntoJsonFile(string fileName, string type,
			string description, multimap<int, string> congestionRecords,
			map<int, int> amountVirNwsRecords, map<int, int> currentNumberOfVns,
			map<int, pair<int, double>> currentRevenueVirLinkRecords,
			map<int, int> currentRevenueVirNodeRecords,
			map<int, double> dataSwitchingEnergy,
			map<int, double> transportedDataVolume,

			map<int, double> comDataSwitchingEnergy,
			map<int, double> comTransportedDataVolume,

			map<int, int> currentEndUserRequests,

			map<int, double> sameVirDataSwitchingEnergy,
			map<int, double> sameVirTransportedDataVolume,
			map<int, unsigned int> currentCommonEndUserReqs,
			map<int, int> currentEndingEndUserReqs,
			map<int, int> currentEndingVirNws,
			map<int, pair<int, int>> currentSubLinkResourceInfo);

	static void OutputRecordsIntoImageFile(const string type_1,
			const string type_2, multimap<int, string> congestionRecords_1,
			multimap<int, string> congestionRecords_2,
			map<int, int> amountVirNwsRecords_1,
			map<int, int> amountVirNwsRecords_2,
			map<int, int> currentNumberOfVns_1,
			map<int, int> currentNumberOfVns_2,
			map<int, pair<int, double>> currentRevenueVirLinkRecords_1,
			map<int, pair<int, double>> currentRevenueVirLinkRecords_2,
			map<int, int> currentRevenueVirNodeRecords_1,
			map<int, int> currentRevenueVirNodeRecords_2,
			map<int, double> dataSwitchingEnergy_1,
			map<int, double> dataSwitchingEnergy_2,
			map<int, double> transportedDataVolume_1,
			map<int, double> transportedDataVolume_2,

			map<int, double> comDataSwitchingEnergy_1,
			map<int, double> comDataSwitchingEnergy_2,
			map<int, double> comTransportedDataVolume_1,
			map<int, double> comTransportedDataVolume_2,

			map<int, int> currentEndUserRequests_1,
			map<int, int> currentEndUserRequests_2,

			map<int, double> sameVirDataSwitchingEnergy_1,
			map<int, double> sameVirDataSwitchingEnergy_2,

			map<int, double> sameVirTransportedDataVolume_1,
			map<int, double> sameVirTransportedDataVolume_2,

			map<int, unsigned int> currentCommonEndUserReqs_1,
			map<int, unsigned int> currentCommonEndUserReqs_2,

			map<int, int> currentEndingEndUserReqs_1,
			map<int, int> currentEndingEndUserReqs_2,

			map<int, int> currentEndingVirNws_1,
			map<int, int> currentEndingVirNws_2,

			map<int, pair<int, int>> currentSubLinkResourceInfo_1,
			map<int, pair<int, int>> currentSubLinkResourceInfo_2);

	static string getNewCommonVirtuaNetworkID();

};

#endif /* COM_COMMONFUNCTIONS_H_ */
