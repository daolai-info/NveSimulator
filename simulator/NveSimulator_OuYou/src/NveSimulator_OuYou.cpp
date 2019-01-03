//============================================================================
// Name        : NveSimulator_KouYou.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iterator>
#include <map>
#include <vector>

#include <fstream>

#include "algorithms/KShortestPaths.h"
#include "algorithms/ShortestPath.h"
#include "algorithms/ShortestPathWithFloyd.h"
#include "embed/VirtualNetworkEmbedding.h"
#include "generator/RandomGenerator.h"
#include "graphs/BaseGraph.h"
#include "graphs/SubNwGraph.h"
#include "graphs/BasePath.h"
#include "com/Timer.h"
#include "com/CommonConstants.h"

using namespace std;

template<class T1, class T2>
void printMapVector(const map<T1, vector<T2> > vtr) {
	cout << " printed map< string, vector>'s size: " << vtr.size() << endl;
	for (typename map<T1, vector<T2> >::const_iterator it = vtr.begin();
			it != vtr.end(); ++it) {
		cout << it->first << " : ";
		printVector(it->second);
		cout << endl;
	}
	cout << endl;
}

int main() {

	Timer * timer = new Timer();
	timer->startTiming();

	string fileName = "./topologies/SubNW_50_5.brite";

	cout << " Create a network virtualization environment (NVE_Ouyou) " << endl;

	VirtualNetworkEmbedding *vne_ouyou = new VirtualNetworkEmbedding(fileName);

	int timing = 0;

	while (true) {

		if (0
				== vne_ouyou->InitiateEmbeddingVirtualNetworkEnvironment(
						timing)) {

			cout
					<< " A virtual network is embedded into NVE_Ouyou successfully. "
					<< endl;

		} else {
			cout << " The initiation of NVE_Ouyou is finished that embedded  "
					<< vne_ouyou->getNws().size() << " virtual networks."
					<< endl;
			SubNwGraph::checkSubstrateLinksResource(vne_ouyou->getSubNw());
			break;
		}

	}

	vne_ouyou->clearVirtualLinksResourceMappingAndAllocations();
	vne_ouyou->reallocationVirtualLinksResourceBySplittingRatio(
			CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK);

	SubNwGraph::checkSubstrateLinksResource(vne_ouyou->getSubNw());
	cout
			<< "  *** Each virtual link is mapping to two static substrate paths that average virtual link's bandwidth resource."
			<< endl;

	map<string, EndUserRequest*> euRequests_ouyou;

	vector<int> vNwRequests = CommonFunctions::generateVirtualNetworkRequests();

	for (int timing = 0; timing < CommonConstants::VNE_ELAPSED_TIME_UNITS;
			++timing) {

		cout << " Current time :" << timing << endl;

		vector<int>::iterator iter = find(vNwRequests.begin(),
				vNwRequests.end(), timing);

		if (timing != 0 && iter != vNwRequests.end()) {

			VirNwGraph* virNwDouble = vne_ouyou->constructVirtualNetwork(
					timing);

			if (0
					== vne_ouyou->EmbeddingVirtualNetworkWithVirtualLinkSplittingRatio(
							virNwDouble, timing,
							CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK)) {
				cout << " Current time :" << timing << endl;
				cout << " A virtual network is embedded successfully." << endl;

			} else {
				VirNwGraph::ReleaseVirtualNetworkResource(virNwDouble);
			}

		}
		cout << " Currently, there are virtual networks ("
				<< vne_ouyou->getNws().size() << ") embedded into NVE_Ouyou."
				<< endl;

		// generate new end user request
		int requestAmount =
				RandomGenerator::getUserVideoRequestsAmountWithPoissonDistribution();

		vne_ouyou->updateNveDataSwitchingEnergy(timing);
		euRequests_ouyou =
				vne_ouyou->generateEndUserVideoRequestsForVirtualNetwork(timing,
						requestAmount);

		vne_ouyou->performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
				timing, euRequests_ouyou,
				CommonConstants::EQUAL_ENDUSER_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK);
		vne_ouyou->releaseEndUserVideoRequestsForVirtualNetwork(timing);
		euRequests_ouyou.clear();

		SubNwGraph::checkSubstrateLinksResource(vne_ouyou->getSubNw());

		vne_ouyou->updateAmountOfVnsRecords(timing);
		vne_ouyou->updateCurrentNumberOfVns(timing);
		vne_ouyou->updateCurrentRevenueVirLinkAndNodeRecords(timing);

	}

	string timestr = timer->getEndTiming();

	cout
			<< "  ****The total available virtual networks in Network Virtualization Environment (NVE_Ouyou) :"
			<< vne_ouyou->getAmountVNs() << endl;
	SubNwGraph::printSubstrateNetwork(vne_ouyou->getSubNw());
	SubNwGraph::checkSubstrateLinksResource(vne_ouyou->getSubNw());

	CommonFunctions::OutputRecordsIntoJsonFile("NVE_Ouyou_" + timestr,
			"NVE_Ouyou",
			"NVE_Ouyou with static substrate transport paths: Each virtual link mapped into two static substrate data transport paths. ",
			vne_ouyou->getCongestionRecords(), vne_ouyou->getAmountVnsRecords(),
			vne_ouyou->getCurrentNumberOfVns(),
			vne_ouyou->getCurrentRevenueVirLinkRecords(),
			vne_ouyou->getCurrentRevenueVirNodeRecords(),
			vne_ouyou->getDataSwitchingEnergy());

	timer->endTiming();
	timer->elapsedTime();
	delete timer;
	cout << " --- OK ---" << endl;

	return 0;
}
