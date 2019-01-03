//============================================================================
// Name        : simulator.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "com/CommonConstants.h"
#include "com/CommonFunctions.h"
#include "com/Timer.h"
#include "embed/EndUserRequest.h"
#include "embed/VirtualNetworkEmbedding.h"
#include "generator/RandomGenerator.h"
#include "graphs/SubNwGraph.h"
#include "graphs/VirNwGraph.h"

using namespace std;

int main() {

	Timer * timer = new Timer();
	timer->startTiming();

	string fileName = "./topologies/SubNW_50_5.brite";

	cout << " Create a network virtualization environment (NVE_KouYou) "
			<< endl;

	VirtualNetworkEmbedding *vne_kouyou = new VirtualNetworkEmbedding(fileName);

	int timing = 0;

	while (true) {

		if (0
				== vne_kouyou->InitiateEmbeddingVirtualNetworkEnvironment(
						timing)) {

			cout
					<< " A virtual network is embedded into NVE_KouYou successfully. "
					<< endl;

		} else {
			cout << " The initiation of NVE_Ouyou is finished that embedded  "
					<< vne_kouyou->getNws().size() << " virtual networks."
					<< endl;
			SubNwGraph::checkSubstrateLinksResource(vne_kouyou->getSubNw());
			break;
		}
	}

	map<string, EndUserRequest*> euRequests_default;

	vector<int> vNwRequests = CommonFunctions::generateVirtualNetworkRequests();

	for (int timing = 0; timing < CommonConstants::VNE_ELAPSED_TIME_UNITS;
			++timing) {

		cout << " Current time :" << timing << endl;

		vector<int>::iterator iter = find(vNwRequests.begin(),
				vNwRequests.end(), timing);
		if (timing != 0 && iter != vNwRequests.end()) {

			VirNwGraph* virNwSingle = vne_kouyou->constructVirtualNetwork(
					timing);

			if (0 == vne_kouyou->EmbeddingVirtualNetwork(virNwSingle, timing)) {
				cout << " Current time :" << timing << endl;
				cout << " A virtual network is embedded successfully." << endl;

			} else {

				VirNwGraph::ReleaseVirtualNetworkResource(virNwSingle);
			}

		}
		cout << " Currently, there are virtual networks ("
				<< vne_kouyou->getNws().size() << ") embedded into NVE_Ouyou."
				<< endl;
		// generate new end user request
		int requestAmount =
				RandomGenerator::getUserVideoRequestsAmountWithPoissonDistribution();

		// place before perform end-user request function
		vne_kouyou->updateNveDataSwitchingEnergy(timing);
		euRequests_default =
				vne_kouyou->generateEndUserVideoRequestsForVirtualNetwork(
						timing, requestAmount);

		vne_kouyou->performVirtualNetworkEnvironment(timing,
				euRequests_default);
		vne_kouyou->releaseEndUserVideoRequestsForVirtualNetwork(timing);
		euRequests_default.clear();

		SubNwGraph::checkSubstrateLinksResource(vne_kouyou->getSubNw());

		vne_kouyou->updateAmountOfVnsRecords(timing);
		vne_kouyou->updateCurrentNumberOfVns(timing);
		vne_kouyou->updateCurrentRevenueVirLinkAndNodeRecords(timing);

	}

	string timestr = timer->getEndTiming();

	cout
			<< "  ****The total available virtual networks in Network Virtualization Environment (NVE_KouYou) :"
			<< vne_kouyou->getAmountVNs() << endl;

	SubNwGraph::checkSubstrateLinksResource(vne_kouyou->getSubNw());
	SubNwGraph::checkSubstrateLinksResourceStatus(vne_kouyou->getSubNw());

	CommonFunctions::OutputRecordsIntoJsonFile("NVE_KouYou_" + timestr,
			"NVE_KouYou",
			"NVE_KouYou: Each virtual link mapped into a static substrate data transport path on substrate network. ",
			vne_kouyou->getCongestionRecords(),
			vne_kouyou->getAmountVnsRecords(),
			vne_kouyou->getCurrentNumberOfVns(),
			vne_kouyou->getCurrentRevenueVirLinkRecords(),
			vne_kouyou->getCurrentRevenueVirNodeRecords(),
			vne_kouyou->getDataSwitchingEnergy());

	timer->endTiming();
	timer->elapsedTime();
	delete timer;
	cout << " --- Finished, OK  ---" << endl;
	return 0;
}
