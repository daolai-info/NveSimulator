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

		if (0
				== vne->InitiateEmbeddingVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
						timing,
						CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK)) {
			cout
					<< " An Virtual Network With Virtual Link Splitting Ratio Embedded into NVE :"
					<< vne->getNws().size() << endl;

		} else {
			cout
					<< " The initiation of Embedding Virtual Network With Virtual Link Splitting Ratio Finished:  "
					<< vne->getNws().size() << endl;

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

			VirNwGraph* virNwDouble = vne->constructVirtualNetwork(timing);

			if (0
					== vne->EmbeddingVirtualNetworkWithVirtualLinkSplittingRatio(
							virNwDouble, timing,
							CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK)) {
				cout << " Current time :" << timing << endl;
				cout
						<< " An Virtual Network Embedded With Two Substrate transport Paths :"
						<< vne->getNws().size() << endl;

			} else {
				cout
						<< " Embedding Finished ( With Two Substrate transport Paths):  "
						<< vne->getNws().size() << endl;

				VirNwGraph::ReleaseVirtualNetworkResource(virNwDouble);
			}

		}

		// generate new end user request
		int requestAmount =
				RandomGenerator::getUserVideoRequestsAmountWithPoissonDistribution();

		//
		vne->updateNveDataSwitchingEnergy(timing);
		euRequests_default =
				vne->generateEndUserVideoRequestsForVirtualNetwork(
						timing, requestAmount);

		vne->performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
				timing, euRequests_default,
				CommonConstants::EQUAL_ENDUSER_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK);
		vne->releaseEndUserVideoRequestsForVirtualNetwork(timing);
		euRequests_default.clear();

		SubNwGraph::checkSubstrateLinksResource(vne->getSubNw());

		vne->updateAmountOfVnsRecords(timing);
		vne->updateCurrentNumberOfVns(timing);
		vne->updateCurrentRevenueVirLinkAndNodeRecords(timing);

	}

	string timestr = timer->getEndTiming();

	SubNwGraph::checkSubstrateLinksResource(vne->getSubNw());
	SubNwGraph::checkSubstrateLinksResourceStatus(vne->getSubNw());

	CommonFunctions::OutputRecordsIntoJsonFile("NVE_OuYou" + timestr,
			"NVE_OuYou",
			"NVE_OuYou: Each virtual link mapped into two static substrate data transport path on substrate network. ",
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
