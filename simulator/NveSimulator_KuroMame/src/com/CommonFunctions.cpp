/*
 * CommonFunctions.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: ubean
 */

#include "CommonFunctions.h"

#include "../matplot/matplotlibcpp.h"
using namespace std;

namespace plt = matplotlibcpp;
CommonFunctions::CommonFunctions() {
	// TODO Auto-generated constructor stub

}

CommonFunctions::~CommonFunctions() {
	// TODO Auto-generated destructor stub
}

/*
 * linkId format : from_to
 */
string CommonFunctions::makeLinkId(const string from, const string to) {
	string str;
	str = from + '_' + to;
	return str;
}

string CommonFunctions::makePathId(const string from, const string to) {
	string str;
	str = from + '_' + to;
	return str;
}

string CommonFunctions::makeEndUserRequestId(const string vNwNo,
		const string timing, const string seqNo) {
	string str;
	str = vNwNo + '_' + timing + '_' + seqNo;
	return str;
}

vector<string> CommonFunctions::stringSplit(const string &str, char sep) {
	vector<string> v;
	stringstream ss(str);
	string buffer;
	while (getline(ss, buffer, sep)) {
		v.push_back(buffer);
	}
	return v;

}

string CommonFunctions::CoverIntegerToString(const int integer) {
	stringstream ss;
	ss << integer;
	return ss.str();
}

string CommonFunctions::CoverDoubleToString(const double data) {
	stringstream ss;
	ss << data;
	return ss.str();
}

void CommonFunctions::SleepThread(int hours, int minutes, int seconds) {
	using std::chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());

	struct tm * ptm = std::localtime(&tt);
	cout << "Current time: " << put_time(ptm, "%X") << '\n';

	cout << "Waiting for the " << hours << " hours " << minutes << " minutes "
			<< seconds << " seconds to begin...\n";
	ptm->tm_hour = ptm->tm_hour + hours;
	ptm->tm_min = ptm->tm_min + minutes;
	ptm->tm_sec = ptm->tm_sec + seconds;
	this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));

	cout << put_time(ptm, "%X") << " reached!\n";
}

void CommonFunctions::FAILURE_EXIT() {
	exit(1);
}

void CommonFunctions::printVector(vector<string> vtr) {
	cout << " printed vector's size: " << vtr.size() << endl;
	for (typename vector<string>::const_iterator it = vtr.begin();
			it != vtr.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
}

void CommonFunctions::printMap(map<string, string> vtr) {
	cout << " printed map's size: " << vtr.size() << endl;
	for (typename map<string, string>::iterator it = vtr.begin();
			it != vtr.end(); ++it) {
		cout << it->first << " : " << it->second << endl;
	}
	cout << endl;
}

void CommonFunctions::printMapInteger(map<int, int> vtr) {
	cout << " printed map's size: " << vtr.size() << endl;
	for (typename map<int, int>::iterator it = vtr.begin(); it != vtr.end();
			++it) {
		cout << it->first << " : " << it->second << endl;
	}
	cout << endl;
}

void CommonFunctions::printMultimapMapIntegerString(multimap<int, string> vtr) {
	cout << " printed multimap's size: " << vtr.size() << endl;
	for (typename multimap<int, string>::iterator it = vtr.begin();
			it != vtr.end(); ++it) {
		cout << it->first << " : " << it->second << endl;
	}
	cout << endl;
}

vector<int> CommonFunctions::generateVirtualNetworkRequests() {
	return RandomGenerator::getVirtualNetworkEmebeddingRequestsWithExponentialDistribution();
}

void CommonFunctions::printFailureMessageAndExit(const string errorMessage,
		const string functionName) {
	MessageSet::printErrorMessage(errorMessage);
	MessageSet::printFailureExitMessage(functionName);
	CommonFunctions::FAILURE_EXIT();
}

void CommonFunctions::printWarningMessage(const string warningMessage) {
	MessageSet::printWarningMessage(warningMessage);
}

void CommonFunctions::printInfoMessage(const string infoMessage) {
	MessageSet::printInfoMessage(infoMessage);
}

void CommonFunctions::OutputRecordsIntoImageFile(const string type_1,
		const string type_2, multimap<int, string> congestionRecords_1,
		multimap<int, string> congestionRecords_2,
		map<int, int> amountVirNwsRecords_1,
		map<int, int> amountVirNwsRecords_2, map<int, int> currentNumberOfVns_1,
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

		map<int, double> sameVirNwDataSwitchingEnergy_1,
		map<int, double> sameVirNwDataSwitchingEnergy_2,

		map<int, double> sameVirNwTransportedDataVolume_1,
		map<int, double> sameVirNwTransportedDataVolume_2,

		map<int, unsigned int> currentCommonEndUserReqs_1,
		map<int, unsigned int> currentCommonEndUserReqs_2,

		map<int, int> currentEndingEndUserReqs_1,
		map<int, int> currentEndingEndUserReqs_2,

		map<int, int> currentEndingVirNws_1,
		map<int, int> currentEndingVirNws_2,

		map<int, pair<int, int>> currentSubLinkResourceInfo_1,
		map<int, pair<int, int>> currentSubLinkResourceInfo_2

		) {

	vector<double>::iterator cIter;
	vector<double> congestion1;
	vector<double> congestion2;
	vector<double> timing;

	for (int index = 0; index < CommonConstants::VNE_ELAPSED_TIME_UNITS;
			++index) {
		cIter = timing.begin();
		timing.insert(cIter, index);
		cIter = congestion1.begin();
		congestion1.insert(cIter, congestionRecords_1.count(index));
	}

	for (int index = 0; index < CommonConstants::VNE_ELAPSED_TIME_UNITS;
			++index) {
		cIter = congestion2.begin();
		congestion2.insert(cIter, congestionRecords_2.count(index));
	}

	plt::named_plot(type_1, timing, congestion1);
	plt::named_plot(type_2, timing, congestion2);
	plt::title("Evaluation of End-User Request Congestion");

	plt::xlabel("Time Unit(s)");
	plt::ylabel("Congestion Time(s)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> AmountNws_1;
	for (map<int, int>::iterator iter = amountVirNwsRecords_1.begin();
			iter != amountVirNwsRecords_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, (iter->first));

		cIter = AmountNws_1.begin();
		AmountNws_1.insert(cIter, iter->second);
	}

	vector<double> AmountNws_2;
	for (map<int, int>::iterator iter = amountVirNwsRecords_2.begin();
			iter != amountVirNwsRecords_2.end(); ++iter) {
		cIter = AmountNws_2.begin();
		AmountNws_2.insert(cIter, iter->second);
	}

	plt::named_plot(type_1, timing, AmountNws_1);
	plt::named_plot(type_2, timing, AmountNws_2);
	//plt::ylim(0, 60);
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::title("Evaluation of Embedded Virtual Network(s)");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Embedded Virtual Network(s)");
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> AvailableNws_1;
	for (map<int, int>::iterator iter = currentNumberOfVns_1.begin();
			iter != currentNumberOfVns_1.end(); ++iter) {

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = AvailableNws_1.begin();
		AvailableNws_1.insert(cIter, iter->second);

	}

	vector<double> AvailableNws_2;
	for (map<int, int>::iterator iter = currentNumberOfVns_2.begin();
			iter != currentNumberOfVns_2.end(); ++iter) {

		cIter = AvailableNws_2.begin();
		AvailableNws_2.insert(cIter, iter->second);

	}

	plt::named_plot(type_1, timing, AvailableNws_1);
	plt::named_plot(type_2, timing, AvailableNws_2);

	plt::title("Evaluation of Available Virtual Network(s)");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Available Virtual Network(s)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	//plt::ylim(0, 60);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> swichingEnergy_1;
	for (map<int, double>::iterator iter = dataSwitchingEnergy_1.begin();
			iter != dataSwitchingEnergy_1.end(); ++iter) {

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = swichingEnergy_1.begin();
		swichingEnergy_1.insert(cIter, iter->second);

	}

	vector<double> swichingEnergy_2;
	for (map<int, double>::iterator iter = dataSwitchingEnergy_2.begin();
			iter != dataSwitchingEnergy_2.end(); ++iter) {

		cIter = swichingEnergy_2.begin();
		swichingEnergy_2.insert(cIter, iter->second);

	}

	plt::named_plot(type_1, timing, swichingEnergy_1);
	plt::named_plot(type_2, timing, swichingEnergy_2);

	plt::title("Evaluation of Data Transport Switching Energy Consumption");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Data Switching Energy(Joule)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> sameSwichingEnergy_1;
	for (map<int, double>::iterator iter =
			sameVirNwDataSwitchingEnergy_1.begin();
			iter != sameVirNwDataSwitchingEnergy_1.end(); ++iter) {

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = sameSwichingEnergy_1.begin();
		sameSwichingEnergy_1.insert(cIter, iter->second);

	}

	vector<double> sameSwichingEnergy_2;
	for (map<int, double>::iterator iter =
			sameVirNwDataSwitchingEnergy_2.begin();
			iter != sameVirNwDataSwitchingEnergy_2.end(); ++iter) {

		cIter = sameSwichingEnergy_2.begin();
		sameSwichingEnergy_2.insert(cIter, iter->second);

	}

	plt::named_plot(type_1, timing, sameSwichingEnergy_1);
	plt::named_plot(type_2, timing, sameSwichingEnergy_2);

	plt::title(
			"Evaluation of Data Transport Switching Energy Consumption of Same Virtual Networks");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Data Switching Energy(Joule)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> comSwichingEnergy_1;
	for (map<int, double>::iterator iter = comDataSwitchingEnergy_1.begin();
			iter != comDataSwitchingEnergy_1.end(); ++iter) {

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = comSwichingEnergy_1.begin();
		comSwichingEnergy_1.insert(cIter, iter->second);

	}

	vector<double> comSwichingEnergy_2;
	for (map<int, double>::iterator iter = comDataSwitchingEnergy_2.begin();
			iter != comDataSwitchingEnergy_2.end(); ++iter) {

		cIter = comSwichingEnergy_2.begin();
		comSwichingEnergy_2.insert(cIter, iter->second);

	}

	plt::named_plot(type_1, timing, comSwichingEnergy_1);
	plt::named_plot(type_2, timing, comSwichingEnergy_2);

	plt::title(
			"Evaluation of Common Data Transport Switching Energy Consumption");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Data Switching Energy(Joule)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> EmbeddedLinks_1;
	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords_1.begin();
			iter != currentRevenueVirLinkRecords_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = EmbeddedLinks_1.begin();
		EmbeddedLinks_1.insert(cIter, iter->second.first);

	}

	vector<double> EmbeddedLinks_2;
	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords_2.begin();
			iter != currentRevenueVirLinkRecords_2.end(); ++iter) {

		cIter = EmbeddedLinks_2.begin();
		EmbeddedLinks_2.insert(cIter, iter->second.first);

	}

	plt::named_plot(type_1, timing, EmbeddedLinks_1);
	plt::named_plot(type_2, timing, EmbeddedLinks_2);

	plt::title("Evaluation of Total Embedded Virtual Link(s)");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Embedded Virtual Link(s)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> EmbeddedNodes_1;
	for (map<int, int>::iterator iter = currentRevenueVirNodeRecords_1.begin();
			iter != currentRevenueVirNodeRecords_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = EmbeddedNodes_1.begin();
		EmbeddedNodes_1.insert(cIter, iter->second);

	}

	vector<double> EmbeddedNodes_2;
	for (map<int, int>::iterator iter = currentRevenueVirNodeRecords_2.begin();
			iter != currentRevenueVirNodeRecords_2.end(); ++iter) {

		cIter = EmbeddedNodes_2.begin();
		EmbeddedNodes_2.insert(cIter, iter->second);

	}

	plt::named_plot(type_1, timing, EmbeddedNodes_1);
	plt::named_plot(type_2, timing, EmbeddedNodes_2);

	plt::title("Evaluation of Embedded Virtual Node(s)");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Embedded Virtual Node(s)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> EmbeddedBandwidth_1;
	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords_1.begin();
			iter != currentRevenueVirLinkRecords_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = EmbeddedBandwidth_1.begin();
		EmbeddedBandwidth_1.insert(cIter, iter->second.second);

	}
	vector<double> EmbeddedBandwidth_2;
	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords_2.begin();
			iter != currentRevenueVirLinkRecords_2.end(); ++iter) {
		cIter = EmbeddedBandwidth_2.begin();
		EmbeddedBandwidth_2.insert(cIter, iter->second.second);
	}

	plt::named_plot(type_1, timing, EmbeddedBandwidth_1);
	plt::named_plot(type_2, timing, EmbeddedBandwidth_2);

	plt::title("Evaluation of Embedded Virtual Bandwidth");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Embedded Virtual Bandwidth");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> dataVolume_1;
	for (map<int, double>::iterator iter = transportedDataVolume_1.begin();
			iter != transportedDataVolume_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = dataVolume_1.begin();
		dataVolume_1.insert(cIter, iter->second);

	}
	vector<double> dataVolume_2;
	for (map<int, double>::iterator iter = transportedDataVolume_2.begin();
			iter != transportedDataVolume_2.end(); ++iter) {
		cIter = dataVolume_2.begin();
		dataVolume_2.insert(cIter, iter->second);
	}

	plt::named_plot(type_1, timing, dataVolume_1);
	plt::named_plot(type_2, timing, dataVolume_2);

	plt::title("Evaluation of Switching Data Volume");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Data Volume(Megabits)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> sameDataVolume_1;
	for (map<int, double>::iterator iter =
			sameVirNwTransportedDataVolume_1.begin();
			iter != sameVirNwTransportedDataVolume_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = sameDataVolume_1.begin();
		sameDataVolume_1.insert(cIter, iter->second);

	}
	vector<double> sameDataVolume_2;
	for (map<int, double>::iterator iter =
			sameVirNwTransportedDataVolume_2.begin();
			iter != sameVirNwTransportedDataVolume_2.end(); ++iter) {
		cIter = sameDataVolume_2.begin();
		sameDataVolume_2.insert(cIter, iter->second);
	}

	plt::named_plot(type_1, timing, sameDataVolume_1);
	plt::named_plot(type_2, timing, sameDataVolume_2);

	plt::title("Evaluation of Switching Data Volume of Same Virtual Networks");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Data Volume(Megabits)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> comDataVolume_1;
	for (map<int, double>::iterator iter = comTransportedDataVolume_1.begin();
			iter != comTransportedDataVolume_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = comDataVolume_1.begin();
		comDataVolume_1.insert(cIter, iter->second);

	}
	vector<double> comDataVolume_2;
	for (map<int, double>::iterator iter = comTransportedDataVolume_2.begin();
			iter != comTransportedDataVolume_2.end(); ++iter) {
		cIter = comDataVolume_2.begin();
		comDataVolume_2.insert(cIter, iter->second);
	}

	plt::named_plot(type_1, timing, comDataVolume_1);
	plt::named_plot(type_2, timing, comDataVolume_2);

	plt::title("Evaluation of Common Switching Data Volume");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Data Volume(Megabits)");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> request_1;
	for (map<int, int>::iterator iter = currentEndUserRequests_1.begin();
			iter != currentEndUserRequests_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = request_1.begin();
		request_1.insert(cIter, iter->second);

	}
	vector<double> request_2;
	for (map<int, int>::iterator iter = currentEndUserRequests_2.begin();
			iter != currentEndUserRequests_2.end(); ++iter) {
		cIter = request_2.begin();
		request_2.insert(cIter, iter->second);
	}

	plt::named_plot(type_1, timing, request_1);
	plt::named_plot(type_2, timing, request_2);

	plt::title("Evaluation of End-user Requests");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Request(s)");

	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> comRequest_1;
	for (map<int, unsigned int>::iterator iter =
			currentCommonEndUserReqs_1.begin();
			iter != currentCommonEndUserReqs_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = comRequest_1.begin();
		comRequest_1.insert(cIter, iter->second);

	}
	vector<double> comRequest_2;
	for (map<int, unsigned int>::iterator iter =
			currentCommonEndUserReqs_2.begin();
			iter != currentCommonEndUserReqs_2.end(); ++iter) {
		cIter = comRequest_2.begin();
		comRequest_2.insert(cIter, iter->second);
	}

	plt::named_plot(type_1, timing, comRequest_1);
	plt::named_plot(type_2, timing, comRequest_2);

	plt::title("Evaluation of Common End-user Requests");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Request(s)");

	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> endingRequest_1;
	for (map<int, int>::iterator iter = currentEndingEndUserReqs_1.begin();
			iter != currentEndingEndUserReqs_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = endingRequest_1.begin();
		endingRequest_1.insert(cIter, iter->second);

	}

	vector<double> endingRequest_2;
	for (map<int, int>::iterator iter = currentEndingEndUserReqs_2.begin();
			iter != currentEndingEndUserReqs_2.end(); ++iter) {
		cIter = endingRequest_2.begin();
		endingRequest_2.insert(cIter, iter->second);
	}

	plt::named_plot(type_1, timing, endingRequest_1);
	plt::named_plot(type_2, timing, endingRequest_2);

	plt::title("Evaluation of Current Ending End-user Requests");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Request(s)");

	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> endingVirNws_1;
	for (map<int, int>::iterator iter = currentEndingVirNws_1.begin();
			iter != currentEndingVirNws_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = endingVirNws_1.begin();
		endingVirNws_1.insert(cIter, iter->second);

	}
	vector<double> endingVirNws_2;
	for (map<int, int>::iterator iter = currentEndingVirNws_2.begin();
			iter != currentEndingVirNws_2.end(); ++iter) {
		cIter = endingVirNws_2.begin();
		endingVirNws_2.insert(cIter, iter->second);
	}

	plt::named_plot(type_1, timing, endingVirNws_1);
	plt::named_plot(type_2, timing, endingVirNws_2);

	plt::title("Evaluation of Current Ending Virtual Networks");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Virtual Network(s)");

	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

	timing.clear();

	vector<double> minSublinks_1;
	vector<double> maxSublinks_1;
	for (map<int, pair<int, int>>::iterator iter =
			currentSubLinkResourceInfo_1.begin();
			iter != currentSubLinkResourceInfo_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = minSublinks_1.begin();
		minSublinks_1.insert(cIter, iter->second.first);

		cIter = maxSublinks_1.begin();
		maxSublinks_1.insert(cIter, iter->second.second);

	}

	vector<double> minSublinks_2;
	vector<double> maxSublinks_2;
	for (map<int, pair<int, int>>::iterator iter =
			currentSubLinkResourceInfo_2.begin();
			iter != currentSubLinkResourceInfo_2.end(); ++iter) {

		cIter = minSublinks_2.begin();
		minSublinks_2.insert(cIter, iter->second.first);

		cIter = maxSublinks_2.begin();
		maxSublinks_2.insert(cIter, iter->second.second);

	}

	cout << " step 3" << endl;
	cout << "timing size:" << timing.size() << endl;

	cout << " 1 size: " << minSublinks_1.size() << endl;

	cout << " 2.size: " << minSublinks_2.size() << endl;

	cout << " 1 size: " << maxSublinks_1.size() << endl;

	cout << " 2.size: " << maxSublinks_2.size() << endl;

	plt::named_plot(type_1 + "-less than 30 M", timing, minSublinks_1);
	plt::named_plot(type_2 + "-less than 30 M", timing, minSublinks_2);
	plt::named_plot(type_1 + "-less than 100 M", timing, maxSublinks_1);
	plt::named_plot(type_2 + "-less than 100 M", timing, maxSublinks_2);

	plt::title("Evaluation of Current Resource Substrate Links");
	plt::xlabel("Time Unit(s)");
	plt::ylabel("Amount of Substrate Link(s)");

	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();

}

void CommonFunctions::OutputRecordsIntoJsonFile(string fileName, string type,
		string description, multimap<int, string> congestionRecords,
		map<int, int> amountVirNwsRecords, map<int, int> currentNumberOfVns,
		map<int, pair<int, double>> currentRevenueVirLinkRecords,
		map<int, int> currentRevenueVirNodeRecords,
		map<int, double> dataSwitchingEnergy,
		map<int, double> transportedDataVolume,

		map<int, double> comDataSwitchingEnergy,
		map<int, double> comTransportedDataVolume,

		map<int, int> currentEndUserRequests,

		map<int, double> sameVirNwDataSwitchingEnergy,
		map<int, double> sameVirNwTransportedDataVolume,

		map<int, unsigned int> currentCommonEndUserReqs,
		map<int, int> currentEndingEndUserReqs,
		map<int, int> currentEndingVirNws,

		map<int, pair<int, int>> currentSubLinkResourceInfo

		) {

	StringBuffer strJson;
	Writer<StringBuffer> writer(strJson);

	writer.StartObject();               // Between StartObject()/EndObject(),

	writer.Key("Type");                // output a key,
	writer.String(type.c_str());             // follow by a value.

	writer.Key("Description");
	writer.String(description.c_str());

	writer.Key("CongestionRecords");
	writer.StartArray();

	for (int index = 0; index < CommonConstants::VNE_ELAPSED_TIME_UNITS;
			++index) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(index);

		writer.Key("Total");
		writer.Int(congestionRecords.count(index));

		writer.EndObject();

	}
	writer.EndArray();

	/*
	 * amount of substrate links whose available bandwidth is less than min
	 */
	writer.Key("CurrentSubLinkResourceMinInfo");
	writer.StartArray();

	for (map<int, pair<int, int>>::iterator iter =
			currentSubLinkResourceInfo.begin();
			iter != currentSubLinkResourceInfo.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second.first);

		writer.EndObject();
	}
	writer.EndArray();

	/*
	 * amount of substrate links whose available bandwidth is less than max
	 */
	writer.Key("CurrentSubLinkResourceMaxInfo");
	writer.StartArray();

	for (map<int, pair<int, int>>::iterator iter =
			currentSubLinkResourceInfo.begin();
			iter != currentSubLinkResourceInfo.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second.second);

		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("SameVirNwDataSwitchingEnergy");
	writer.StartArray();

	for (map<int, double>::iterator iter = sameVirNwDataSwitchingEnergy.begin();
			iter != sameVirNwDataSwitchingEnergy.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("SameVirNwTransportedDataVolume");
	writer.StartArray();

	for (map<int, double>::iterator iter =
			sameVirNwTransportedDataVolume.begin();
			iter != sameVirNwTransportedDataVolume.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("CurrentCommonEndUserReqs");
	writer.StartArray();

	for (map<int, unsigned int>::iterator iter =
			currentCommonEndUserReqs.begin();
			iter != currentCommonEndUserReqs.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("CurrentEndingEndUserReqs");
	writer.StartArray();

	for (map<int, int>::iterator iter = currentEndingEndUserReqs.begin();
			iter != currentEndingEndUserReqs.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("CurrentEndingVirNws");
	writer.StartArray();

	for (map<int, int>::iterator iter = currentEndingVirNws.begin();
			iter != currentEndingVirNws.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("AmountVirNws");
	writer.StartArray();

	vector<double> AmountNws;
	for (map<int, int>::iterator iter = amountVirNwsRecords.begin();
			iter != amountVirNwsRecords.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("AvailableVirNws");
	writer.StartArray();

	for (map<int, int>::iterator iter = currentNumberOfVns.begin();
			iter != currentNumberOfVns.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("DataSwichingEnergy");
	writer.StartArray();

	for (map<int, double>::iterator iter = dataSwitchingEnergy.begin();
			iter != dataSwitchingEnergy.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("ComDataSwichingEnergy");
	writer.StartArray();

	for (map<int, double>::iterator iter = comDataSwitchingEnergy.begin();
			iter != comDataSwitchingEnergy.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("EmbeddedLinks");
	writer.StartArray();

	vector<double> EmbeddedLinks;

	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords.begin();
			iter != currentRevenueVirLinkRecords.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second.first);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("EmbeddedNodes");
	writer.StartArray();

	for (map<int, int>::iterator iter = currentRevenueVirNodeRecords.begin();
			iter != currentRevenueVirNodeRecords.end(); ++iter) {

		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.Key("EmbeddedBandwidth");
	writer.StartArray();

	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords.begin();
			iter != currentRevenueVirLinkRecords.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second.second);

		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("TransportedDataVolume");
	writer.StartArray();

	for (map<int, double>::iterator iter = transportedDataVolume.begin();
			iter != transportedDataVolume.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("ComTransportedDataVolume");
	writer.StartArray();

	for (map<int, double>::iterator iter = comTransportedDataVolume.begin();
			iter != comTransportedDataVolume.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("CurrentEndUserRequests");
	writer.StartArray();

	for (map<int, int>::iterator iter = currentEndUserRequests.begin();
			iter != currentEndUserRequests.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

	}
	writer.EndArray();

	writer.EndObject();

	Document doc;
	doc.Parse(strJson.GetString());
	FILE* fp = fopen((fileName + "_recordsOutput.json").c_str(), "w");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> fileWriter(os);
	doc.Accept(fileWriter);
	fclose(fp);
}

string CommonFunctions::getNewCommonVirtuaNetworkID(){
	int virNo = ++CommonConstants::COMMON_VNW_ID;
	cout << " common no: " << CommonConstants::COMMON_VNW_ID << endl;
	cout << " no: " << virNo << endl;
	return CommonConstants::COMMON_VNW_ID_FLAG + CommonFunctions::CoverIntegerToString(virNo);;

}
