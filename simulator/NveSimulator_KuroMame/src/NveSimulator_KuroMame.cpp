//============================================================================
// Name        : NveSimulator_KuroMame.cpp
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
	string timestr;

	string fileName = "./topologies/SubNW_50_5.brite";

	cout << " Create a network virtualization environment (NVE_KuroMame) "
			<< endl;

	VirtualNetworkEmbedding *vne_kuromame = new VirtualNetworkEmbedding(
			fileName);

	int timing = 0;

	while (true) {

		if (0
				== vne_kuromame->InitiateEmbeddingVirtualNetworkEnvironment(
						timing)) {

			cout
					<< " A virtual network is embedded into NVE_Kuromame successfully. "
					<< endl;

		} else {
			cout
					<< " The initiation of NVE_Kuromame is finished that embedded  "
					<< vne_kuromame->getNws().size() << " virtual networks."
					<< endl;
			SubNwGraph::checkSubstrateLinksResource(vne_kuromame->getSubNw());
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

			VirNwGraph* virNwSingle = vne_kuromame->constructVirtualNetwork(
					timing);

			if (0
					== vne_kuromame->EmbeddingVirtualNetwork(virNwSingle,
							timing)) {

				cout << " Current time :" << timing << endl;
				cout << " A virtual network is embedded successfully." << endl;

			} else {
				VirNwGraph::ReleaseVirtualNetworkResource(virNwSingle);
			}

		}
		cout << " Currently, there are virtual networks ("
				<< vne_kuromame->getNws().size() << ") embedded into NVE_Ouyou."
				<< endl;
// generate new end user request
		int requestAmount =
				RandomGenerator::getUserVideoRequestsAmountWithPoissonDistribution();

// place before perform end-user request function
		vne_kuromame->updateNveDataSwitchingEnergy(timing);
		euRequests_default =
				vne_kuromame->generateEndUserVideoRequestsForVirtualNetwork(
						timing, requestAmount);
		vne_kuromame->performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
				timing, euRequests_default,
				CommonConstants::NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK);

		vne_kuromame->releaseEndUserVideoRequestsForVirtualNetwork(timing);

		SubNwGraph::checkSubstrateLinksResource(vne_kuromame->getSubNw());

		vne_kuromame->updateAmountOfVnsRecords(timing);
		vne_kuromame->updateCurrentNumberOfVns(timing);
		vne_kuromame->updateCurrentRevenueVirLinkAndNodeRecords(timing);
		vne_kuromame->updateCurrentSubLinkEmbeddingResourceInfo(timing,
				CommonConstants::MIN_VNW_LINK_BANDWIDTH,
				CommonConstants::MAX_VNW_LINK_BANDWIDTH);

		euRequests_default.clear();

	}

	timestr = timer->getEndTiming();

	cout
			<< "  ****The total available virtual networks in Network Virtualization Environment (NVE_KuroMame) :"
			<< vne_kuromame->getAmountVNs() << endl;

	SubNwGraph::checkSubstrateLinksResource(vne_kuromame->getSubNw());
	SubNwGraph::checkSubstrateLinksResourceStatus(vne_kuromame->getSubNw());

	CommonFunctions::OutputRecordsIntoJsonFile("NVE_KuroMame_" + timestr,
			"NVE_KuroMame",
			"NVE_KuroMame with Dynamic Substrate Transport Paths: Each virtual link mapped into a substrate data transport path and may dynamically reallocate a new substrate data transport path. ",
			vne_kuromame->getCongestionRecords(),
			vne_kuromame->getAmountVnsRecords(),
			vne_kuromame->getCurrentNumberOfVns(),
			vne_kuromame->getCurrentRevenueVirLinkRecords(),
			vne_kuromame->getCurrentRevenueVirNodeRecords(),
			vne_kuromame->getDataSwitchingEnergy(),
			vne_kuromame->getTransportedDataVolume(),
			vne_kuromame->getComDataSwitchingEnergy(),
			vne_kuromame->getComTransportedDataVolume(),
			vne_kuromame->getCurrentAvailableEndUserRequests(),

			vne_kuromame->getSameVirNwDataSwitchingEnergy(),
			vne_kuromame->getSameVirNwTransportedDataVolume(),
			vne_kuromame->getCurrentCommonEndUserReqs(),
			vne_kuromame->getCurrentEndingEndUserReqs(),
			vne_kuromame->getCurrentEndingVirNws(),
			vne_kuromame->getCurrentSubLinkResourceInfo());

	timer->endTiming();
	timer->elapsedTime();
	delete timer;
	cout << " --- OK ---" << endl;
	return 0;
}
