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
			map<int, double> dataSwitchingEnergy);

	static void OutputRecordsIntoImageFile(
			multimap<int, string> congestionRecords_1,
			multimap<int, string> congestionRecords_2,
			multimap<int, string> congestionRecords_3,
			map<int, int> amountVirNwsRecords_1,
			map<int, int> amountVirNwsRecords_2,
			map<int, int> amountVirNwsRecords_3,
			map<int, int> currentNumberOfVns_1,
			map<int, int> currentNumberOfVns_2,
			map<int, int> currentNumberOfVns_3,
			map<int, pair<int, double>> currentRevenueVirLinkRecords_1,
			map<int, pair<int, double>> currentRevenueVirLinkRecords_2,
			map<int, pair<int, double>> currentRevenueVirLinkRecords_3,
			map<int, int> currentRevenueVirNodeRecords_1,
			map<int, int> currentRevenueVirNodeRecords_2,
			map<int, int> currentRevenueVirNodeRecords_3,
			map<int, double> dataSwitchingEnergy_1,
			map<int, double> dataSwitchingEnergy_2,
			map<int, double> dataSwitchingEnergy_3);

};

#endif /* COM_COMMONFUNCTIONS_H_ */
