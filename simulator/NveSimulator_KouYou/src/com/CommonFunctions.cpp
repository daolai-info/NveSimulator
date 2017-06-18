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

void CommonFunctions::OutputRecordsIntoImageFile(
		multimap<int, string> congestionRecords_1,
		multimap<int, string> congestionRecords_2,
		multimap<int, string> congestionRecords_3,
		map<int, int> amountVirNwsRecords_1,
		map<int, int> amountVirNwsRecords_2,
		map<int, int> amountVirNwsRecords_3, map<int, int> currentNumberOfVns_1,
		map<int, int> currentNumberOfVns_2, map<int, int> currentNumberOfVns_3,
		map<int, pair<int, double>> currentRevenueVirLinkRecords_1,
		map<int, pair<int, double>> currentRevenueVirLinkRecords_2,
		map<int, pair<int, double>> currentRevenueVirLinkRecords_3,
		map<int, int> currentRevenueVirNodeRecords_1,
		map<int, int> currentRevenueVirNodeRecords_2,
		map<int, int> currentRevenueVirNodeRecords_3,
		map<int, double> dataSwitchingEnergy_1,
		map<int, double> dataSwitchingEnergy_2,
		map<int, double> dataSwitchingEnergy_3) {

	vector<double>::iterator cIter;
	vector<double> congestion1;
	vector<double> congestion2;
	vector<double> congestion3;
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
	for (int index = 0; index < CommonConstants::VNE_ELAPSED_TIME_UNITS;
			++index) {
		cIter = congestion3.begin();
		congestion3.insert(cIter, congestionRecords_3.count(index));

	}
	plt::named_plot("NVE#1", timing, congestion1);
	plt::named_plot("NVE#2", timing, congestion2);
	plt::named_plot("NVE#3", timing, congestion3);
	plt::title("Evaluation of End-User Request Congestion");

	plt::xlabel("Time Units");
	plt::ylabel("Congestion Times");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./CongestionTimis.jpg");

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

	vector<double> AmountNws_3;
	for (map<int, int>::iterator iter = amountVirNwsRecords_3.begin();
			iter != amountVirNwsRecords_3.end(); ++iter) {
		cIter = AmountNws_3.begin();
		AmountNws_3.insert(cIter, iter->second);
	}
	plt::named_plot("NVE#1", timing, AmountNws_1);
	plt::named_plot("NVE#2", timing, AmountNws_2);
	plt::named_plot("NVE#3", timing, AmountNws_3);
	//plt::ylim(0, 60);
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::title("Evaluation of Embedded Virtual Networks");
	plt::xlabel("Time Units");
	plt::ylabel("Embedded Virtual Networks");
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./AmountNws.jpg");

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

	vector<double> AvailableNws_3;
	for (map<int, int>::iterator iter = currentNumberOfVns_3.begin();
			iter != currentNumberOfVns_3.end(); ++iter) {

		cIter = AvailableNws_3.begin();
		AvailableNws_3.insert(cIter, iter->second);

	}

	plt::named_plot("NVE#1", timing, AvailableNws_1);
	plt::named_plot("NVE#2", timing, AvailableNws_2);
	plt::named_plot("NVE#3", timing, AvailableNws_3);

	plt::title("Evaluation of Available Virtual Networks");
	plt::xlabel("Time Units");
	plt::ylabel("Available Virtual Networks");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	//plt::ylim(0, 60);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./AvailableNws.jpg");

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
	vector<double> swichingEnergy_3;
	for (map<int, double>::iterator iter = dataSwitchingEnergy_3.begin();
			iter != dataSwitchingEnergy_3.end(); ++iter) {

		cIter = swichingEnergy_3.begin();
		swichingEnergy_3.insert(cIter, iter->second);

	}

	plt::named_plot("NVE#1", timing, swichingEnergy_1);
	plt::named_plot("NVE#2", timing, swichingEnergy_2);
	plt::named_plot("NVE#3", timing, swichingEnergy_3);

	plt::title("Evaluation of Data Transport Switching Energy Consumption");
	plt::xlabel("Time Units");
	plt::ylabel("Switching Energy Consumption");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./DataSwichingEnergy.jpg");

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
	vector<double> EmbeddedLinks_3;
	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords_3.begin();
			iter != currentRevenueVirLinkRecords_3.end(); ++iter) {

		cIter = EmbeddedLinks_3.begin();
		EmbeddedLinks_3.insert(cIter, iter->second.first);

	}

	plt::named_plot("NVE#1", timing, EmbeddedLinks_1);
	plt::named_plot("NVE#2", timing, EmbeddedLinks_2);
	plt::named_plot("NVE#3", timing, EmbeddedLinks_3);

	plt::title("Evaluation of Total Embedded Virtual Links");
	plt::xlabel("Time Units");
	plt::ylabel("Embedded Virtual Links");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./RevenueEmbeddedLinks.jpg");


	timing.clear();

	vector<double> EmbeddedNodes_1;
	for (map<int, int>::iterator iter =
			currentRevenueVirNodeRecords_1.begin();
			iter != currentRevenueVirNodeRecords_1.end(); ++iter) {
		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = EmbeddedNodes_1.begin();
		EmbeddedNodes_1.insert(cIter, iter->second);

	}

	vector<double> EmbeddedNodes_2;
	for (map<int, int>::iterator iter =
			currentRevenueVirNodeRecords_2.begin();
			iter != currentRevenueVirNodeRecords_2.end(); ++iter) {

		cIter = EmbeddedNodes_2.begin();
		EmbeddedNodes_2.insert(cIter, iter->second);

	}

	vector<double> EmbeddedNodes_3;
	for (map<int, int>::iterator iter =
			currentRevenueVirNodeRecords_3.begin();
			iter != currentRevenueVirNodeRecords_3.end(); ++iter) {

		cIter = EmbeddedNodes_3.begin();
		EmbeddedNodes_3.insert(cIter, iter->second);

	}

	plt::named_plot("NVE#1", timing, EmbeddedNodes_1);
	plt::named_plot("NVE#2", timing, EmbeddedNodes_2);
	plt::named_plot("NVE#3", timing, EmbeddedNodes_3);

	plt::title("Evaluation of Embedded Virtual Nodes");
	plt::xlabel("Time Units");
	plt::ylabel("Embedded Virtual Nodes");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./RevenueEmbeddedNodes.jpg");


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
	vector<double> EmbeddedBandwidth_3;
	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords_3.begin();
			iter != currentRevenueVirLinkRecords_3.end(); ++iter) {
		cIter = EmbeddedBandwidth_3.begin();
		EmbeddedBandwidth_3.insert(cIter, iter->second.second);

	}

	plt::named_plot("Embedded Virtual Bandwidth of NVE#1", timing, EmbeddedBandwidth_1);
	plt::named_plot("Embedded Virtual Bandwidth of NVE#2", timing, EmbeddedBandwidth_2);
	plt::named_plot("Embedded Virtual Bandwidth of NVE#3", timing, EmbeddedBandwidth_3);

	plt::title("Evaluation of Embedded Virtual Bandwidth");
	plt::xlabel("Time Units");
	plt::ylabel("Embedded Virtual Bandwidth");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./RevenueEmbeddedBandwidth.jpg");

}

void CommonFunctions::OutputRecordsIntoJsonFile(string fileName, string type,
		string description, multimap<int, string> congestionRecords,
		map<int, int> amountVirNwsRecords, map<int, int> currentNumberOfVns,
		map<int, pair<int, double>> currentRevenueVirLinkRecords,
		map<int, int> currentRevenueVirNodeRecords,
		map<int, double> dataSwitchingEnergy) {

	StringBuffer strJson;
	Writer<StringBuffer> writer(strJson);

	writer.StartObject();               // Between StartObject()/EndObject(),

	writer.Key("Type");                // output a key,
	writer.String(type.c_str());             // follow by a value.

	writer.Key("Description");
	writer.String(description.c_str());

	writer.Key("CongestionRecords");
	writer.StartArray();

	vector<double> timing;
	vector<double>::iterator cIter;
	vector<double> congestion;

	for (int index = 0; index < CommonConstants::VNE_ELAPSED_TIME_UNITS;
			++index) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(index);

		writer.Key("Total");
		writer.Int(congestionRecords.count(index));

		writer.EndObject();

		cIter = timing.begin();
		timing.insert(cIter, index);

		cIter = congestion.begin();
		congestion.insert(cIter, congestionRecords.count(index));

	}
	writer.EndArray();

	plt::named_plot(type, timing, congestion);
	plt::title("Evaluation of End-User Request Congestion");
	plt::xlabel("Time Units");
	plt::ylabel("Congestion Times");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./CongestionTimis.jpg");

	writer.Key("AmountNws");
	writer.StartArray();

	timing.clear();

	vector<double> AmountNws;
	for (map<int, int>::iterator iter = amountVirNwsRecords.begin();
			iter != amountVirNwsRecords.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

		cIter = timing.begin();
		timing.insert(cIter, (iter->first));

		cIter = AmountNws.begin();
		AmountNws.insert(cIter, iter->second);

	}
	writer.EndArray();

	plt::named_plot( type, timing, AmountNws);
	plt::title("Evaluation of Embedded Virtual Networks");
	plt::xlabel("Time Units");
	plt::ylabel("Embedded Virtual Networks");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	//plt::ylim(0, 60);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./AmountNws.jpg");

	timing.clear();

	writer.Key("AvailableNws");
	writer.StartArray();

	vector<double> AvailableNws;

	for (map<int, int>::iterator iter = currentNumberOfVns.begin();
			iter != currentNumberOfVns.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = AvailableNws.begin();
		AvailableNws.insert(cIter, iter->second);
	}
	writer.EndArray();

	plt::named_plot(type, timing, AvailableNws);
	plt::title("Evaluation of Available Virtual Networks");
	plt::xlabel("Time Units");
	plt::ylabel("Available Virtual Networks");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	//plt::ylim(0, 60);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./AvailableNws.jpg");

	timing.clear();

	writer.Key("SwichingEnergy");
	writer.StartArray();

	vector<double> swichingEnergy;

	for (map<int, double>::iterator iter = dataSwitchingEnergy.begin();
			iter != dataSwitchingEnergy.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = swichingEnergy.begin();
		swichingEnergy.insert(cIter, iter->second);

	}
	writer.EndArray();

	plt::named_plot(type, timing, swichingEnergy);
	plt::title("Evaluation of Data Switching Energy Consumption ");
	plt::xlabel("Time Units");
	plt::ylabel("Switching Energy Consumption");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./DataSwichingEnergy.jpg");

	timing.clear();

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

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = EmbeddedLinks.begin();
		EmbeddedLinks.insert(cIter, iter->second.first);

	}
	writer.EndArray();

	plt::named_plot( type, timing, EmbeddedLinks);
	plt::title("Evaluation of Embedded Virtual Links");
	plt::xlabel("Time Units");
	plt::ylabel("Embedded Virtual Links");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./RevenueEmbeddedLinks.jpg");

	timing.clear();

	writer.Key("EmbeddedNodes");
	writer.StartArray();

	vector<double> EmbeddedNodes;

	for (map<int, int>::iterator iter =
			currentRevenueVirNodeRecords.begin();
			iter != currentRevenueVirNodeRecords.end(); ++iter) {

		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second);

		writer.EndObject();

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = EmbeddedNodes.begin();
		EmbeddedNodes.insert(cIter, iter->second);

	}
	writer.EndArray();

	plt::named_plot( type, timing, EmbeddedNodes);
	plt::title("Evaluation of Embedded Virtual Nodes");
	plt::xlabel("Time Units");
	plt::ylabel("Embedded Virtual Nodes");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./RevenueEmbeddedLinks.jpg");

	timing.clear();

	writer.Key("EmbeddedBandwidth");
	writer.StartArray();

	vector<double> EmbeddedBandwidth;

	for (map<int, pair<int, double>>::iterator iter =
			currentRevenueVirLinkRecords.begin();
			iter != currentRevenueVirLinkRecords.end(); ++iter) {
		writer.StartObject();

		writer.Key("Timing");
		writer.Int(iter->first);

		writer.Key("Total");
		writer.Int(iter->second.second);

		writer.EndObject();

		cIter = timing.begin();
		timing.insert(cIter, iter->first);

		cIter = EmbeddedBandwidth.begin();
		EmbeddedBandwidth.insert(cIter, iter->second.second);
	}
	writer.EndArray();

	writer.EndObject();

	plt::named_plot(type, timing, EmbeddedBandwidth);
	plt::title("Evaluation of Total Embedded Virtual Bandwidth");
	plt::xlabel("Time Units");
	plt::ylabel("Embedded Virtual Bandwidth");
	plt::xlim(0, CommonConstants::VNE_ELAPSED_TIME_UNITS);
	plt::grid(true);
	plt::legend();
	plt::show();
	//plt::save("./RevenueEmbeddedBandwidth.jpg");


	Document doc;
	doc.Parse(strJson.GetString());
	FILE* fp = fopen((fileName + "_recordsOutput.json").c_str(), "w");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> fileWriter(os);
	doc.Accept(fileWriter);
	fclose(fp);
}
