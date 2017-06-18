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

	VirtualNetworkEmbedding *vne = new VirtualNetworkEmbedding(fileName);

	int timing = 0;

	while (true) {

		if (0 == vne->InitiateEmbeddingVirtualNetworkEnvironment(timing)) {

			cout << " A Virtual Network Embedded Successfully :"
					<< vne->getNws().size() << endl;

		} else {
			cout
					<< " Initiate Virtual Network Environment Embedding Finished:  "
					<< vne->getNws().size() << endl;
			SubNwGraph::checkSubstrateLinksResource(vne->getSubNw());
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

			VirNwGraph* virNwSingle = vne->constructVirtualNetwork(timing);

			if (0 == vne->EmbeddingVirtualNetwork(virNwSingle, timing)) {
				cout << " Current time :" << timing << endl;
				cout
						<< " One Virtual Network Embedded With One Substrate transport Path successfully :"
						<< vne->getNws().size() << endl;

			} else {
				cout << " Current time :" << timing << endl;
				cout
						<< " Embedding Finished ( With One Substrate transport Path):  "
						<< vne->getNws().size() << endl;

				VirNwGraph::ReleaseVirtualNetworkResource(virNwSingle);
			}

		}

		// generate new end user request
		int requestAmount =
				RandomGenerator::getUserVideoRequestsAmountWithPoissonDistribution();

		// place before perform end-user request function
		vne->updateNveDataSwitchingEnergy(timing);
		euRequests_default = vne->generateEndUserVideoRequestsForVirtualNetwork(
				timing, requestAmount);
		cout << " vne->performVirtualNetworkEnvironment " << endl;
		vne->performVirtualNetworkEnvironment(timing, euRequests_default);
		vne->releaseEndUserVideoRequestsForVirtualNetwork(timing);
		euRequests_default.clear();

		SubNwGraph::checkSubstrateLinksResource(vne->getSubNw());

		vne->updateAmountOfVnsRecords(timing);
		vne->updateCurrentNumberOfVns(timing);
		vne->updateCurrentRevenueVirLinkAndNodeRecords(timing);

	}

	string timestr = timer->getEndTiming();

	cout << "  ****Embedding Virtual Network Environment's virtual networks :"
			<< vne->getNws().size() << endl;

	SubNwGraph::checkSubstrateLinksResource(vne->getSubNw());
	SubNwGraph::checkSubstrateLinksResourceStatus(vne->getSubNw());

	CommonFunctions::OutputRecordsIntoJsonFile("NVE_KouYou" + timestr,
			"NVE_KouYou",
			"NVE_KouYou: Each virtual link mapped into a static substrate data transport path on substrate network. ",
			vne->getCongestionRecords(), vne->getAmountVnsRecords(),
			vne->getCurrentNumberOfVns(),
			vne->getCurrentRevenueVirLinkRecords(),
			vne->getCurrentRevenueVirNodeRecords(),
			vne->getDataSwitchingEnergy());

	timer->endTiming();
	timer->elapsedTime();
	delete timer;
	cout << " --- Finished, OK  ---" << endl;
	return 0;
}
