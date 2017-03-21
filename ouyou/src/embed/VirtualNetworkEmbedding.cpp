/*
 * VirtualNetworkEmbeding.cpp
 *
 *  Created on: 2016/11/21
 *      Author: zhangshanming
 */

#include "VirtualNetworkEmbedding.h"

using namespace std;

VirtualNetworkEmbedding::VirtualNetworkEmbedding() {

}

VirtualNetworkEmbedding::VirtualNetworkEmbedding(const string subNwGraphFile) {
	this->amountVNs = 0;
	this->subNwGraphFile = subNwGraphFile;

	this->kShortPaths = new KShortestPaths(this->subNwGraphFile);
	constructSubstrateNetwork(subNwGraphFile);
}

VirtualNetworkEmbedding::~VirtualNetworkEmbedding() {
	// TODO Auto-generated destructor stub
	delete this->kShortPaths;
	delete this->subNW;
}

void VirtualNetworkEmbedding::constructSubstrateNetwork(const string fileName) {

	MessageSet::printInfoMessage("Create Substrate Network Start");
	this->subNW = new SubNwGraph(fileName);

	for (map<string, BaseNode*>::const_iterator bIter =
			this->subNW->getNodes().begin();
			bIter != this->subNW->getNodes().end(); ++bIter) {

		SubNwNode* subNode = new SubNwNode();
		map<string, SubNwNode*>::const_iterator subIter =
				this->subNW->getSubNodes().begin();

		subNode->setSubNodeId(bIter->second->getNodeId());

		this->subNW->getSubNodes().insert(subIter,
				pair<string, SubNwNode*>(bIter->first, subNode));
	}

	for (map<string, BaseLink*>::const_iterator bIter =
			this->subNW->getLinks().begin();
			bIter != this->subNW->getLinks().end(); ++bIter) {
		SubNwLink* subLink = new SubNwLink();
		subLink->setSubLinkBandwidth(
				CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH);

		// default substrate link bandwidth - current used substrate link bandwidth
		subLink->setSubLinkAvailableBandwidth(
				CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH
						- bIter->second->getBandwidth());
		subLink->setSubLinkEmbeddableBandwidth(
				CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH
						- bIter->second->getBandwidth());

//		subLink->setSubLinkAvailableBandwidth(
//				CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH);
//		subLink->setSubLinkEmbeddableBandwidth(
//				CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH);

		cout << " Substrate link: " << bIter->first << " Bandwidth :"
				<< subLink->getSubLinkBandwidth() << " Used Bandwidth : "
				<< bIter->second->getBandwidth() << " Available Bandwidth : "
				<< subLink->getSubLinkAvailableBandwidth()
				<< " Embeddable Bandwidth : "
				<< subLink->getSubLinkEmbeddableBandwidth() << endl;

		subLink->setSubLinkDelay(bIter->second->getDelay());
		subLink->setSubLinkFrom(bIter->second->getFrom());
		subLink->setSubLinkTo(bIter->second->getTo());
		subLink->setSubLinkFromDomain(bIter->second->getFromDomain());
		subLink->setSubLinkToDomain(bIter->second->getToDomain());
		subLink->setSubLinkWeight(bIter->second->getWeight());

		double switchingEnergy =
				(this->subNW->getSubNodes().find(bIter->second->getFrom())->second->getSubNodeSwitchingEnergy()
						+ this->subNW->getSubNodes().find(
								bIter->second->getTo())->second->getSubNodeSwitchingEnergy())
						/ 2;

		subLink->setSubLinkSwitchingEnergy(switchingEnergy);

		map<string, SubNwLink*>::const_iterator subIter =
				this->subNW->getSubLinks().begin();

		this->subNW->getSubLinks().insert(subIter,
				pair<string, SubNwLink*>(bIter->first, subLink));

	}

	MessageSet::printInfoMessage("Create Substrate Network Successfully");
	MessageSet::printInfoMessage("Create Substrate Network END");
}

VirNwGraph* VirtualNetworkEmbedding::constructVirtualNetwork(const int timing) {
	MessageSet::printInfoMessage(
			" VirtualNetworkEmbedding::constructVirtualNetwork Start");

	/*
	 * string : virtual network topolgy file
	 * vector <string>: mapped physical nodes
	 */
	map<string, vector<string> > vnw =
			RandomGenerator::getVirtualNetworkWithPoissonAndUniformDistribution();

	map<string, vector<string> >::iterator vnIter = vnw.begin();
	VirNwGraph* virNW = new VirNwGraph(vnIter->first);

	//string fileName = "./topologies/VNW_3.brite";
	//VirNwGraph* virNW = new VirNwGraph(fileName);

	int virLinkBandwidth =
			RandomGenerator::getVirtualNetworkLinkBandwidthWithUniformDistribution();

	for (map<string, BaseLink*>::const_iterator bIter =
			virNW->getLinks().begin(); bIter != virNW->getLinks().end();
			++bIter) {

		VirNwLink* virLink = new VirNwLink();
		virLink->setVirLinkId(bIter->first);
		virLink->setVirLinkFrom(bIter->second->getFrom());
		virLink->setVirLinkTo(bIter->second->getTo());

		virLink->setVirFromDomain(bIter->second->getFromDomain());
		virLink->setVirToDomain(bIter->second->getToDomain());
		virLink->setVirLinkAvailableBandwidth(virLinkBandwidth); // initialization
		virLink->setVirLinkBandwidth(virLinkBandwidth);

		map<string, VirNwLink*>::const_iterator virIter =
				virNW->getVirLinks().begin();

		virNW->getVirLinks().insert(virIter,
				pair<string, VirNwLink*>(bIter->first, virLink));
		cout << " Virtual link: " << bIter->first << " "
				<< virLink->getVirLinkBandwidth() << "M" << endl;

	}

	for (map<string, BaseNode*>::const_iterator bIter =
			virNW->getNodes().begin(); bIter != virNW->getNodes().end();
			++bIter) {

		VirNwNode* virNode = new VirNwNode();
		virNode->setVirNodeId(bIter->second->getNodeId());

		map<string, VirNwNode*>::const_iterator virIter =
				virNW->getVirNodes().begin();

		virNW->getVirNodes().insert(virIter,
				pair<string, VirNwNode*>(bIter->first, virNode));
	}

	virNW->setMappedPhyNodes(vnIter->second);

	virNW->setVirNwStartTiming(timing);

	int duration =
			RandomGenerator::getVirtualNetworkDurationTimeWithUniformDistribution();
	int endtiming = virNW->getVirNwStartTiming() + duration;

	if (endtiming > CommonConstants::VNE_ELAPSED_TIME_UNITS) {
		// when the virtual network end timing extend the whole elapsed time
		virNW->setVirNwDurationTimeUnits(
				CommonConstants::VNE_ELAPSED_TIME_UNITS - timing);
		virNW->setVirNwEndTiming(CommonConstants::VNE_ELAPSED_TIME_UNITS - 1);

	} else {
		// in the case of no extension  the whole elapsed time
		virNW->setVirNwDurationTimeUnits(duration);
		virNW->setVirNwEndTiming(endtiming);
	}

	cout << virNW->getVirNwStartTiming() << " ";
	cout << virNW->getVirNwDurationTimeUnits() << " ";
	cout << virNW->getVirNwEndTiming() << endl;

	MessageSet::printInfoMessage("Create Virtual Network Successfully.");
	MessageSet::printInfoMessage(
			" VirtualNetworkEmbedding::constructVirtualNetwork END");

	return virNW;
}

int VirtualNetworkEmbedding::EmbeddingVirtualNetwork(VirNwGraph* vNwGraph,
		const int timing) {

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::EmbeddingVirtualNetwork Start.");

	string vnNo = CommonFunctions::CoverIntegerToString(
			this->getAmountVNs() + 1);

	bool checkFlag = true;

	// the same as subNW, and hold latest substrate network information, used before embedding check processing
	SubNwGraph * embedCheckSubNW = this->subNW->copySubstrateNetwork();

	// check the capability of substrate network nodes whether can embed virtual network nodes.
	map<string, SubNwNode*>::iterator subIter;
	for (map<string, string>::iterator vMapIter =
			vNwGraph->getMappedPhyNodes().begin();
			vMapIter != vNwGraph->getMappedPhyNodes().end(); ++vMapIter) {

		subIter = embedCheckSubNW->getSubNodes().find(vMapIter->second);

		if (subIter != embedCheckSubNW->getSubNodes().end()) {
			map<string, VirNwNode*>::iterator vIter =
					vNwGraph->getVirNodes().find(vMapIter->first);
			if (vIter != vNwGraph->getVirNodes().end()) {

				if (subIter->second->getSubNodeAvaiableCapability()
						< vIter->second->getVirNodeCapability()) {
					checkFlag = false;
					break;
				} else {
					subIter->second->setSubNodeAvaiableCapability(
							subIter->second->getSubNodeAvaiableCapability()
									- vIter->second->getVirNodeCapability());
				}
			} else {
				MessageSet::printErrorMessage(
						" VSirtual node: " + vMapIter->first
								+ " is not included inside virtual network.");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}

		} else {
			MessageSet::printErrorMessage(
					" Substrate node: " + vMapIter->second
							+ " is not included inside substrate network.");

			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : embeddingVirtualNetwork()");
			CommonFunctions::FAILURE_EXIT();
		}
	}

	// embedding failed
	if (false == checkFlag) {

		MessageSet::printWarningMessage(
				" Virtual network embedding failed  because the capability of substrate network node is not enough, try to regenerate and embed a virtual network.");

		delete embedCheckSubNW;
		delete vNwGraph;

		MessageSet::printInfoMessage(
				"VirtualNetworkEmbedding::EmbeddingVirtualNetwork End.");

		return 1;

	} else {
		MessageSet::printInfoMessage(
				" All virtual nodes may be mapped into substrate network, check successfully.");
		//CommonFunctions::SleepThread(0, 0, 3);
	}

	cout << " Virtual nodes check finished . " << endl;

	// for case of one virtual link to one substrate transport path
	map<string, BasePath*> transportPath;
	vector<string> CalaculatedVirLinks; // checked a_b, for  avoid b_a check

	// virtual links embedding: substrate transport path check
	for (map<string, VirNwLink*>::iterator vlIter =
			vNwGraph->getVirLinks().begin();
			vlIter != vNwGraph->getVirLinks().end(); ++vlIter) {

		// for repeat calacualte
		if (find(CalaculatedVirLinks.begin(), CalaculatedVirLinks.end(),
				vlIter->first) != CalaculatedVirLinks.end()) {
			continue;
		}

		cout << " Virtual link : " << vlIter->first << " "
				<< vlIter->second->getVirLinkBandwidth() << "(M)";
		// get the two virtual nodes of a virtual link
		vector<string> fromTo = CommonFunctions::stringSplit(vlIter->first,
				'_');
		string from = fromTo.at(0);
		string to = fromTo.at(1);

		// get mapped substrate nodes of two virtual nodes
		string subFrom = "*";
		string subTo = "*";

		//check the two terminal nodes of virtual link
		map<string, string>::iterator mIter;
		mIter = vNwGraph->getMappedPhyNodes().find(from);
		if (mIter != vNwGraph->getMappedPhyNodes().end()) {
			subFrom = mIter->second;
		} else {
			MessageSet::printErrorMessage(
					" Virtual node: " + from
							+ "'s mapped substrate node is no existence in physical network.");
			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : embeddingVirtualNetwork()");
			CommonFunctions::FAILURE_EXIT();
		}

		mIter = vNwGraph->getMappedPhyNodes().find(to);
		if (mIter != vNwGraph->getMappedPhyNodes().end()) {
			subTo = mIter->second;
		} else {
			MessageSet::printErrorMessage(
					" Virtual node: " + to
							+ "'s mapped substrate node is no existence in physical network.");
			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : embeddingVirtualNetwork()");
			CommonFunctions::FAILURE_EXIT();
		}

		if (subFrom == "*" || subTo == "*") {
			MessageSet::printErrorMessage(
					" Virtual link: " + vlIter->first
							+ " two terminals has no mapped substrate nodes in physical network.");

			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : embeddingVirtualNetwork()");
			CommonFunctions::FAILURE_EXIT();
		}

		// check substrate transport path whether exists in substrate network for each virtual link
		/*
		 * get the shortest path between mapped two substrate nodes,
		 * then check the available bandwidth of the shortest path
		 */

		// for a virtual link to get multiple substrate transport paths.
		// average hops during 4 and 10
		//cout << " subnode from :" << subFrom << " : subnode To :" << subTo
		//		<< endl;
		map<string, BasePath*> kPath =
				this->kShortPaths->getKShortestPathsWithHopRangeLimit(subFrom,
						subTo, 4, 10); //

		if (kPath.size() == 0) {

			kPath = this->kShortPaths->getKShortestPathsWithHopRangeLimit(
					subFrom, subTo, 1, 4); //
			if (kPath.size() == 0) {
				MessageSet::printInfoMessage(
						" Virtual link: " + vlIter->first
								+ " has no substrate transport path existed in physical network. Try to repeat embedding  and checking once again.");

				checkFlag = false;
				break;
			}
		}

		/*
		 * sorted order increasing by hops
		 */
		multimap<int, BasePath*> paths;
		for (map<string, BasePath*>::iterator kIter = kPath.begin();
				kIter != kPath.end(); ++kIter) {

			paths.insert(
					pair<int, BasePath*>(kIter->second->getPathHops(),
							kIter->second));

		}

		/*
		 * check bandwidth and confirm substrate transport path for a virtual link
		 * check from maximum hops for fairness bandwidth distribution
		 */
		vector<string> ngLinks;
		for (multimap<int, BasePath*>::reverse_iterator pIter = paths.rbegin();
				pIter != paths.rend(); ++pIter) {
			/*
			 * get path by link segments
			 * check bandwidth comparing to virtual link bandwidth
			 */
			bool vlinkBwCheckFlag = true;
			for (unsigned int i = 1; i < (pIter->second->getPathInfo()).size();
					++i) {
				/*
				 * get substrate path segment for bandwidth check
				 *       FromTo and ToFrom is a pair of substrate link
				 */
				string subLink_FromTo = CommonFunctions::makeLinkId(
						pIter->second->getPathInfo().at(i - 1),
						pIter->second->getPathInfo().at(i));

				string subLink_ToFrom = CommonFunctions::makeLinkId(
						pIter->second->getPathInfo().at(i),
						pIter->second->getPathInfo().at(i - 1));

//				cout << "virtual link  : " << vlIter->first << " "
//						<< vlIter->second->getVirLinkBandwidth() << endl;
//				cout << "subLink_FromTo: " << subLink_FromTo << " "
//						<< embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
//						<< endl;
//				cout << "subLink_ToFrom: " << subLink_ToFrom << " "
//						<< embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
//						<< endl;
				/*
				 * if the a->b substrate link's bandwidth is different from the b->a substrate link's.
				 * as exception, the program will exit.
				 */
				if (embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
						!= embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()) {

					MessageSet::printErrorMessage(
							" Substrate link: " + subLink_FromTo + "( "
									+ CommonFunctions::CoverDoubleToString(
											embedCheckSubNW->getSubLink(
													subLink_FromTo)->getSubLinkEmbeddableBandwidth())
									+ "M) and Substrate link: " + subLink_ToFrom
									+ " ("
									+ CommonFunctions::CoverDoubleToString(
											embedCheckSubNW->getSubLink(
													subLink_ToFrom)->getSubLinkEmbeddableBandwidth())
									+ "M) 's embeddableBandwidth is different. .");

					MessageSet::printFailureExitMessage(
							" Virtual Network Embedding  Exception: embeddingVirtualNetwork()");
					CommonFunctions::FAILURE_EXIT();
				}
				/*
				 *  if the substrate link has been check and NG,
				 *  then this time substrate transport path check will be over, and then to check next substrate transport path.
				 */
				if (ngLinks.end()
						!= find(ngLinks.begin(), ngLinks.end(),
								subLink_FromTo)) {
//					cout << " the substrate link " << subLink_FromTo
//							<< " has been checked, NG substrate link." << endl;
					vlinkBwCheckFlag = false;
					break;
				}

				if (ngLinks.end()
						!= find(ngLinks.begin(), ngLinks.end(),
								subLink_ToFrom)) {
					cout << " the substrate link " << subLink_ToFrom
							<< " has been checked, NG substrate link." << endl;
					vlinkBwCheckFlag = false;
					break;
				}

				/* check substrate transport path bandwidth whether is enough for the virtual link
				 *     if no,  the substrate links pair will be record and then to check next substrate path
				 */
				if (embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
						< vlIter->second->getVirLinkBandwidth()) {
					MessageSet::printInfoMessage(
							" Substrate link: " + subLink_FromTo
									+ "'s embeddable bandwidth("
									+ CommonFunctions::CoverDoubleToString(
											embedCheckSubNW->getSubLink(
													subLink_FromTo)->getSubLinkEmbeddableBandwidth())
									+ "M) cannot embed virtual link: "
									+ vlIter->first + "'s bandwidth ("
									+ CommonFunctions::CoverDoubleToString(
											vlIter->second->getVirLinkBandwidth())
									+ "M) continue to check next substrate path, repeatedly.");

					MessageSet::printInfoMessage(
							" Substrate link: " + subLink_ToFrom
									+ "'s embeddable bandwidth("
									+ CommonFunctions::CoverDoubleToString(
											embedCheckSubNW->getSubLink(
													subLink_ToFrom)->getSubLinkEmbeddableBandwidth())
									+ "M) cannot embed virtual link: "
									+ vlIter->first + "'s bandwidth ("
									+ CommonFunctions::CoverDoubleToString(
											vlIter->second->getVirLinkBandwidth())
									+ "M) continue to check next substrate path, repeatedly.");

					ngLinks.push_back(subLink_FromTo);
					ngLinks.push_back(subLink_ToFrom);
					vlinkBwCheckFlag = false;

					// update to the same as substrate network
					//embedCheckSubNW = this->subNW->copySubstrateNetwork();
					break;
				}
			} // substrate path segements check end

			// save the substrate transport path for one virtual link when check ok
			if (vlinkBwCheckFlag == true) {

				// update substrate network resource
				for (unsigned int i = 1;
						i < (pIter->second->getPathInfo()).size(); ++i) {

					string subLink_FromTo = CommonFunctions::makeLinkId(
							pIter->second->getPathInfo().at(i - 1),
							pIter->second->getPathInfo().at(i));

					string subLink_ToFrom = CommonFunctions::makeLinkId(
							pIter->second->getPathInfo().at(i),
							pIter->second->getPathInfo().at(i - 1));

					embedCheckSubNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
							embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
									- vlIter->second->getVirLinkBandwidth());

					embedCheckSubNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
							embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
									- vlIter->second->getVirLinkBandwidth());
				}

				//
				map<string, BasePath*>::iterator tIter = transportPath.begin();
				transportPath.insert(tIter,
						pair<string, BasePath*>(vlIter->first, pIter->second));

				cout << " Virtual link :" << vlIter->first
						<< "'s substrate path " << endl;
				CommonFunctions::printVector(pIter->second->getPathInfo());

				//substrate paths record
				BasePath* reDirection = new BasePath();

				reDirection->setPathSource(pIter->second->getPathDestination());
				reDirection->setPathDestination(pIter->second->getPathSource());
				reDirection->setPathId(
						CommonFunctions::makePathId(
								reDirection->getPathSource(),
								reDirection->getPathDestination()));

				reDirection->setPathDeley(pIter->second->getPathDeley());
				reDirection->setPathBandwidth(
						pIter->second->getPathBandwidth());

				reDirection->setPathAvailableBandwidth(
						pIter->second->getPathAvailableBandwidth());

				reDirection->setPathCost(pIter->second->getPathCost());
				reDirection->setPathDistance(pIter->second->getPathDistance());
				reDirection->setPathHops(pIter->second->getPathHops());

				vector<string> pInfo = pIter->second->getPathInfo();

				reverse(pInfo.begin(), pInfo.end());
				reDirection->setPathInfo(pInfo);
				reDirection->setPathSwitchingEnergy(
						pIter->second->getPathSwitchingEnergy());

				tIter = transportPath.begin();
				transportPath.insert(tIter,
						pair<string, BasePath*>(
								CommonFunctions::makeLinkId(to, from),
								reDirection));

				cout << " Virtual link :"
						<< CommonFunctions::makeLinkId(to, from)
						<< "'s substrate path " << endl;
				CommonFunctions::printVector(reDirection->getPathInfo());
				break;
			}
			// to next substrate path check
		} // Check whether there is substrate transport path exist, if exist, then record it and over the check

		/*
		 *  confirm virtual link whether ok?
		 *    if no,  the virtual network cannot be embedded
		 *    if yes, then next step for embedding process
		 */
		if (transportPath.find(vlIter->first) == transportPath.end()) {
			MessageSet::printInfoMessage(
					" Virtual link: " + vlIter->first
							+ " cannot be mapped by a substrate transport path in substrate network because that no enough bandwidth. Try to repeat embedding and checking.");

			checkFlag = false;
			break;
		} else {
			MessageSet::printInfoMessage(
					" Virtual link: " + vlIter->first
							+ "  can be mapped by a substrate transport path in substrate network, check successfully.");

			MessageSet::printInfoMessage(
					" Virtual link: " + CommonFunctions::makeLinkId(to, from)
							+ "  can be mapped by a substrate transport path in substrate network, check successfully.");
			//one virtual link check over
			CalaculatedVirLinks.push_back(vlIter->first);
			CalaculatedVirLinks.push_back(
					CommonFunctions::makeLinkId(to, from));
		}

	} /*
	 virtual links embedding: substrate transport path check End
	 all virtual links are checked
	 */

	// in case of all virtual links can be embedded into substrate network
	if (true == checkFlag
			&& transportPath.size() == vNwGraph->getVirLinks().size()) {
		MessageSet::printInfoMessage(
				" All virtual links and nodes will be mapped into substrate network, check successfully.");

		/*
		 *emebeding process including embedding nodes and links
		 *return 0 : successful
		 */

		//firstly embedding virtual nodes
		map<string, SubNwNode*>::iterator phyIter;

		for (map<string, string>::iterator vNodeMapIter =
				vNwGraph->getMappedPhyNodes().begin();
				vNodeMapIter != vNwGraph->getMappedPhyNodes().end();
				++vNodeMapIter) {

			phyIter = this->subNW->getSubNodes().find(vNodeMapIter->second);
			if (phyIter != this->subNW->getSubNodes().end()) {

				map<string, VirNwNode*>::iterator vnIter =
						vNwGraph->getVirNodes().find(vNodeMapIter->first);

				if (vnIter != vNwGraph->getVirNodes().end()) {

					if (phyIter->second->getSubNodeAvaiableCapability()
							>= vnIter->second->getVirNodeCapability()) {

						vnIter->second->setVirNodeMappedSubNodeId(
								vNodeMapIter->second);

						vnIter->second->setVirNwNo(vnNo);

						phyIter->second->setSubNodeAvaiableCapability(
								phyIter->second->getSubNodeAvaiableCapability()
										- vnIter->second->getVirNodeCapability());

						phyIter->second->setSubNodeEmbeddedAmount(
								phyIter->second->getSubNodeEmbeddedAmount()
										+ 1);

						MessageSet::printInfoMessage(
								" Virtual node :" + vNodeMapIter->first
										+ " has been embedding into substrate node: "
										+ phyIter->first + " successfully.");

					} else {
						MessageSet::printErrorMessage(
								" Virtual node: " + vNodeMapIter->first
										+ " can not be embedded into substrate node: "
										+ phyIter->first
										+ " because none enough capability.");
						MessageSet::printFailureExitMessage(
								" Virtual Network Embedding : embeddingVirtualNetwork()");
						CommonFunctions::FAILURE_EXIT();
					}
				} else {
					MessageSet::printErrorMessage(
							" Virtual node: " + vNodeMapIter->first
									+ " is not included inside virtual network.");
					MessageSet::printFailureExitMessage(
							" Virtual Network Embedding : embeddingVirtualNetwork()");
					CommonFunctions::FAILURE_EXIT();
				}

			} else {
				MessageSet::printErrorMessage(
						" Substrate node: " + vNodeMapIter->second
								+ " is not included inside substrate network.");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}
		}
		// embedding virtual nodes end

		// embedding vitual links start
		map<string, BasePath*>::iterator tranPathIter;
		for (map<string, VirNwLink*>::iterator vlIter =
				vNwGraph->getVirLinks().begin();
				vlIter != vNwGraph->getVirLinks().end(); ++vlIter) {

			tranPathIter = transportPath.find(vlIter->first);

			if (tranPathIter != transportPath.end()) {
				/*
				 * 1. configure the substrate transport path
				 * 2. configure the virtual link
				 */

				double switchingEnergyTransportPath = 0.0;
				double delayTransportPath = 0.0;
				double length = 0.0;

				for (unsigned int i = 1;
						i < tranPathIter->second->getPathInfo().size(); ++i) {

					string subLink_FromTo = CommonFunctions::makeLinkId(
							tranPathIter->second->getPathInfo().at(i - 1),
							tranPathIter->second->getPathInfo().at(i));

					// check physical transport path bandwidth whether is enough for the virtual link
					if (this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
							>= vlIter->second->getVirLinkBandwidth()) {

						double embedBandwidth =
								this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth();

						double updateBandwidth =
								vlIter->second->getVirLinkBandwidth();

						MessageSet::printInfoMessage(
								" Update substrate link: " + subLink_FromTo
										+ "'s embeddable bandwidth from "
										+ CommonFunctions::CoverDoubleToString(
												embedBandwidth) + " reduce  "
										+ CommonFunctions::CoverDoubleToString(
												updateBandwidth));

						this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
								embedBandwidth - updateBandwidth);

						this->subNW->getSubLink(subLink_FromTo)->setSubLinkMappedVirLink(
								vnNo, vlIter->first);

						switchingEnergyTransportPath =
								switchingEnergyTransportPath
										+ this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkSwitchingEnergy();

						delayTransportPath =
								delayTransportPath
										+ this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkDelay();

						length =
								length
										+ this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkWeight();

						MessageSet::printInfoMessage(
								" Updated substrate link: " + subLink_FromTo
										+ "'s embeddable bandwidth is "
										+ CommonFunctions::CoverDoubleToString(
												this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkEmbeddableBandwidth()));

					} else {
						MessageSet::printErrorMessage(
								" Virtual link: " + vlIter->first
										+ "'s mapped substrate link is not enough bandwidth for embedding. Check process bug.");

						MessageSet::printFailureExitMessage(
								" Virtual Network Embedding : embeddingVirtualNetwork()");
						CommonFunctions::FAILURE_EXIT();
					}

				}

				vlIter->second->setVirNwNo(vnNo);
				vlIter->second->setVirLinkSwitchingEnergy(
						switchingEnergyTransportPath);
				vlIter->second->setVirLinkDelay(delayTransportPath);
				vlIter->second->setVirLinkSubPathHops(
						tranPathIter->second->getPathInfo().size() - 1);
				vlIter->second->setVirLinkWeight(length);

				tranPathIter->second->setPathHops(
						tranPathIter->second->getPathInfo().size() - 1);
				tranPathIter->second->setPathDeley(delayTransportPath);
				tranPathIter->second->setPathSwitchingEnergy(
						switchingEnergyTransportPath);
				tranPathIter->second->setPathDistance(length);
				tranPathIter->second->setPathBandwidth(
						vlIter->second->getVirLinkBandwidth());
				tranPathIter->second->setPathAvailableBandwidth(
						vlIter->second->getVirLinkBandwidth());

				tranPathIter->second->setPathId(
						CommonFunctions::makePathId(
								tranPathIter->second->getPathId(),
								CommonFunctions::CoverIntegerToString(
										vlIter->second->getVirLinkSubPaths().size())));

				vlIter->second->addVirLinkSubPath(tranPathIter->second);

			} else {
				MessageSet::printErrorMessage(
						" Virtual link: " + vlIter->first
								+ " can not be embeded into substrate network, check process begun. ");

				MessageSet::printFailureExitMessage(
						" VSirtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}
		}
		// embedding virtual link end
		MessageSet::printInfoMessage(
				" Currently, there are " + vnNo
						+ " virtual networks have been embedding into substrate network.");

		// configure duration time information
//		vNwGraph->setVirNwStartTiming(timing);
//		vNwGraph->setVirNwDurationTimeUnits(
//				RandomGenerator::getVirtualNetworkDurationTimeWithUniformDistribution());
//		vNwGraph->setVirNwEndTiming(
//				vNwGraph->getVirNwStartTiming()
//						+ vNwGraph->getVirNwDurationTimeUnits());

		vNwGraph->setVirNwNo(
				CommonFunctions::CoverIntegerToString(
						this->getAmountVNs() + 1));
		this->vNws.insert(
				pair<int, VirNwGraph*>(this->getAmountVNs() + 1, vNwGraph));
		this->setAmountVNs(this->getAmountVNs() + 1);

		//CommonFunctions::SleepThread(0, 0, 3);
		VirNwGraph::printVirtualNetworkInfo(vNwGraph);
		MessageSet::printInfoMessage(
				"VirtualNetworkEmbedding::EmbeddingVirtualNetwork End.");

		return 0;

	} else { // the case of virtual network can not be embedded into the substrate network
		MessageSet::printInfoMessage(
				" All virtual links cannot be mapped into substrate substrate network at the same time. Try to repeat embedding and checking.");
	}

	MessageSet::printWarningMessage(
			"virtual network embedding failed  because the capability of substrate network node is not enough, try to regenerate and embed a virtual network.");

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::EmbeddingVirtualNetwork End.");
	return 1; // more than 10 time try to embedding, failure.

}

int VirtualNetworkEmbedding::InitiateEmbeddingVirtualNetworkEnvironment(
		const int timing) {
	MessageSet::printInfoMessage(
			"InitiateEmbeddingVirtualNetworkEnvironment Start.");
	string vnNo = CommonFunctions::CoverIntegerToString(
			this->getAmountVNs() + 1);

	bool checkFlag;
	int tryTime = 1;

	while (tryTime <= 10) {
		checkFlag = true;

		// the same as subNW, and hold latest substrate network information, used before embedding check processing
		SubNwGraph * embedCheckSubNW = this->subNW->copySubstrateNetwork();

		VirNwGraph * vNwGraph = constructVirtualNetwork(timing);

		// check the capability of substrate network nodes whether can embed virtual network nodes.
		map<string, SubNwNode*>::iterator subIter;
		for (map<string, string>::iterator vMapIter =
				vNwGraph->getMappedPhyNodes().begin();
				vMapIter != vNwGraph->getMappedPhyNodes().end(); ++vMapIter) {

			subIter = embedCheckSubNW->getSubNodes().find(vMapIter->second);

			if (subIter != embedCheckSubNW->getSubNodes().end()) {
				map<string, VirNwNode*>::iterator vIter =
						vNwGraph->getVirNodes().find(vMapIter->first);
				if (vIter != vNwGraph->getVirNodes().end()) {

					if (subIter->second->getSubNodeAvaiableCapability()
							< vIter->second->getVirNodeCapability()) {
						checkFlag = false;
						break;
					} else {
						subIter->second->setSubNodeAvaiableCapability(
								subIter->second->getSubNodeAvaiableCapability()
										- vIter->second->getVirNodeCapability());
					}
				} else {
					MessageSet::printErrorMessage(
							" VSirtual node: " + vMapIter->first
									+ " is not included inside virtual network.");

					MessageSet::printFailureExitMessage(
							" Virtual Network Embedding : embeddingVirtualNetwork()");
					CommonFunctions::FAILURE_EXIT();
				}

			} else {
				MessageSet::printErrorMessage(
						" Substrate node: " + vMapIter->second
								+ " is not included inside substrate network.");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}
		}

		// embedding failed
		if (false == checkFlag) {

			MessageSet::printWarningMessage(
					CommonFunctions::CoverIntegerToString(tryTime)
							+ " th time virtual network embedding failed  because the capability of substrate network node is not enough, try to regenerate and embed a virtual network.");
			++tryTime;
			delete embedCheckSubNW;
			delete vNwGraph;
			continue;
		} else {
			MessageSet::printInfoMessage(
					" All virtual nodes may be mapped into substrate network, check successfully.");
			//CommonFunctions::SleepThread(0, 0, 3);
		}

		cout << " Virtual nodes check finished . " << endl;

		// for case of one virtual link to one substrate transport path
		map<string, BasePath*> transportPath;
		vector<string> CalaculatedVirLinks; // checked a_b, for  avoid b_a check

		// virtual links embedding: substrate transport path check
		for (map<string, VirNwLink*>::iterator vlIter =
				vNwGraph->getVirLinks().begin();
				vlIter != vNwGraph->getVirLinks().end(); ++vlIter) {

			// for repeat calacualte
			if (find(CalaculatedVirLinks.begin(), CalaculatedVirLinks.end(),
					vlIter->first) != CalaculatedVirLinks.end()) {
				continue;
			}

			cout << " Virtual link : " << vlIter->first << " "
					<< vlIter->second->getVirLinkBandwidth() << "(M)";
			// get the two virtual nodes of a virtual link
			vector<string> fromTo = CommonFunctions::stringSplit(vlIter->first,
					'_');
			string from = fromTo.at(0);
			string to = fromTo.at(1);

			// get mapped substrate nodes of two virtual nodes
			string subFrom = "*";
			string subTo = "*";

			//check the two terminal nodes of virtual link
			map<string, string>::iterator mIter;
			mIter = vNwGraph->getMappedPhyNodes().find(from);
			if (mIter != vNwGraph->getMappedPhyNodes().end()) {
				subFrom = mIter->second;
			} else {
				MessageSet::printErrorMessage(
						" Virtual node: " + from
								+ "'s mapped substrate node is no existence in physical network.");
				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}

			mIter = vNwGraph->getMappedPhyNodes().find(to);
			if (mIter != vNwGraph->getMappedPhyNodes().end()) {
				subTo = mIter->second;
			} else {
				MessageSet::printErrorMessage(
						" Virtual node: " + to
								+ "'s mapped substrate node is no existence in physical network.");
				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}

			if (subFrom == "*" || subTo == "*") {
				MessageSet::printErrorMessage(
						" Virtual link: " + vlIter->first
								+ " two terminals has no mapped substrate nodes in physical network.");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}

			// check substrate transport path whether exists in substrate network for each virtual link
			/*
			 * get the shortest path between mapped two substrate nodes,
			 * then check the available bandwidth of the shortest path
			 */

			// for a virtual link to get multiple substrate transport paths.
			// average hops during 4 and 10
			//cout << " subnode from :" << subFrom << " : subnode To :" << subTo
			//		<< endl;
			map<string, BasePath*> kPath =
					this->kShortPaths->getKShortestPathsWithHopRangeLimit(
							subFrom, subTo, 4, 10); //

			if (kPath.size() == 0) {

				kPath = this->kShortPaths->getKShortestPathsWithHopRangeLimit(
						subFrom, subTo, 1, 4); //
				if (kPath.size() == 0) {
					MessageSet::printInfoMessage(
							" Virtual link: " + vlIter->first
									+ " has no substrate transport path existed in physical network. Try to repeat embedding  and checking once again.");

					checkFlag = false;
					break;
				}
			}

			/*
			 * sorted order increasing by hops
			 */
			multimap<int, BasePath*> paths;
			for (map<string, BasePath*>::iterator kIter = kPath.begin();
					kIter != kPath.end(); ++kIter) {
				paths.insert(
						pair<int, BasePath*>(kIter->second->getPathHops(),
								kIter->second));

			}

			/*
			 * check bandwidth and confirm substrate transport path for a virtual link
			 * check from maximum hops for fairness bandwidth distribution
			 */
			vector<string> ngLinks;
			for (multimap<int, BasePath*>::reverse_iterator pIter =
					paths.rbegin(); pIter != paths.rend(); ++pIter) {
				/*
				 * get path by link segments
				 * check bandwidth comparing to virtual link bandwidth
				 */
				bool vlinkBwCheckFlag = true;
				for (unsigned int i = 1;
						i < (pIter->second->getPathInfo()).size(); ++i) {
					/*
					 * get substrate path segment for bandwidth check
					 *       FromTo and ToFrom is a pair of substrate link
					 */
					string subLink_FromTo = CommonFunctions::makeLinkId(
							pIter->second->getPathInfo().at(i - 1),
							pIter->second->getPathInfo().at(i));

					string subLink_ToFrom = CommonFunctions::makeLinkId(
							pIter->second->getPathInfo().at(i),
							pIter->second->getPathInfo().at(i - 1));

//					cout << "virtual link  : " << vlIter->first << " "
//							<< vlIter->second->getVirLinkBandwidth() << endl;
//					cout << "subLink_FromTo: " << subLink_FromTo << " "
//							<< embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
//							<< endl;
//					cout << "subLink_ToFrom: " << subLink_ToFrom << " "
//							<< embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
//							<< endl;
					/*
					 * if the a->b substrate link's bandwidth is different from the b->a substrate link's.
					 * as exception, the program will exit.
					 */
					if (embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
							!= embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()) {

						MessageSet::printErrorMessage(
								" Substrate link: " + subLink_FromTo + "( "
										+ CommonFunctions::CoverDoubleToString(
												embedCheckSubNW->getSubLink(
														subLink_FromTo)->getSubLinkEmbeddableBandwidth())
										+ "M) and Substrate link: "
										+ subLink_ToFrom + " ("
										+ CommonFunctions::CoverDoubleToString(
												embedCheckSubNW->getSubLink(
														subLink_ToFrom)->getSubLinkEmbeddableBandwidth())
										+ "M) 's embeddableBandwidth is different. .");

						MessageSet::printFailureExitMessage(
								" Virtual Network Embedding  Exception: embeddingVirtualNetwork()");
						CommonFunctions::FAILURE_EXIT();
					}
					/*
					 *  if the substrate link has been check and NG,
					 *  then this time substrate transport path check will be over, and then to check next substrate transport path.
					 */
					if (ngLinks.end()
							!= find(ngLinks.begin(), ngLinks.end(),
									subLink_FromTo)) {
//						cout << " the substrate link " << subLink_FromTo
//								<< " has been checked, NG substrate link."
//								<< endl;
						vlinkBwCheckFlag = false;
						break;
					}

					if (ngLinks.end()
							!= find(ngLinks.begin(), ngLinks.end(),
									subLink_ToFrom)) {
						cout << " the substrate link " << subLink_ToFrom
								<< " has been checked, NG substrate link."
								<< endl;
						vlinkBwCheckFlag = false;
						break;
					}

					/* check substrate transport path bandwidth whether is enough for the virtual link
					 *     if no,  the substrate links pair will be record and then to check next substrate path
					 */
					if (embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
							< vlIter->second->getVirLinkBandwidth()) {
						MessageSet::printInfoMessage(
								" Substrate link: " + subLink_FromTo
										+ "'s embeddable bandwidth("
										+ CommonFunctions::CoverDoubleToString(
												embedCheckSubNW->getSubLink(
														subLink_FromTo)->getSubLinkEmbeddableBandwidth())
										+ "M) cannot embed virtual link: "
										+ vlIter->first + "'s bandwidth ("
										+ CommonFunctions::CoverDoubleToString(
												vlIter->second->getVirLinkBandwidth())
										+ "M) continue to check next substrate path, repeatedly.");

						MessageSet::printInfoMessage(
								" Substrate link: " + subLink_ToFrom
										+ "'s embeddable bandwidth("
										+ CommonFunctions::CoverDoubleToString(
												embedCheckSubNW->getSubLink(
														subLink_ToFrom)->getSubLinkEmbeddableBandwidth())
										+ "M) cannot embed virtual link: "
										+ vlIter->first + "'s bandwidth ("
										+ CommonFunctions::CoverDoubleToString(
												vlIter->second->getVirLinkBandwidth())
										+ "M) continue to check next substrate path, repeatedly.");

						ngLinks.push_back(subLink_FromTo);
						ngLinks.push_back(subLink_ToFrom);
						vlinkBwCheckFlag = false;

						// update to the same as substrate network
						//embedCheckSubNW = this->subNW->copySubstrateNetwork();
						break;
					}
				} // substrate path segements check end

				// save the substrate transport path for one virtual link when check ok
				if (vlinkBwCheckFlag == true) {

					// update substrate network resource
					for (unsigned int i = 1;
							i < (pIter->second->getPathInfo()).size(); ++i) {

						string subLink_FromTo = CommonFunctions::makeLinkId(
								pIter->second->getPathInfo().at(i - 1),
								pIter->second->getPathInfo().at(i));

						string subLink_ToFrom = CommonFunctions::makeLinkId(
								pIter->second->getPathInfo().at(i),
								pIter->second->getPathInfo().at(i - 1));

						embedCheckSubNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
								embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										- vlIter->second->getVirLinkBandwidth());

						embedCheckSubNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
								embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
										- vlIter->second->getVirLinkBandwidth());
					}

					//
					map<string, BasePath*>::iterator tIter =
							transportPath.begin();
					transportPath.insert(tIter,
							pair<string, BasePath*>(vlIter->first,
									pIter->second));

					cout << " Virtual link :" << vlIter->first
							<< "'s substrate path " << endl;
					CommonFunctions::printVector(pIter->second->getPathInfo());

					//substrate paths record
					BasePath* reDirection = new BasePath();

					reDirection->setPathSource(
							pIter->second->getPathDestination());
					reDirection->setPathDestination(
							pIter->second->getPathSource());
					reDirection->setPathId(
							CommonFunctions::makePathId(
									reDirection->getPathSource(),
									reDirection->getPathDestination()));

					reDirection->setPathDeley(pIter->second->getPathDeley());
					reDirection->setPathBandwidth(
							pIter->second->getPathBandwidth());
					reDirection->setPathAvailableBandwidth(
							pIter->second->getPathAvailableBandwidth());
					reDirection->setPathCost(pIter->second->getPathCost());
					reDirection->setPathDistance(
							pIter->second->getPathDistance());
					reDirection->setPathHops(pIter->second->getPathHops());

					vector<string> pInfo = pIter->second->getPathInfo();

					reverse(pInfo.begin(), pInfo.end());
					reDirection->setPathInfo(pInfo);
					reDirection->setPathSwitchingEnergy(
							pIter->second->getPathSwitchingEnergy());

					tIter = transportPath.begin();
					transportPath.insert(tIter,
							pair<string, BasePath*>(
									CommonFunctions::makeLinkId(to, from),
									reDirection));

					cout << " Virtual link :"
							<< CommonFunctions::makeLinkId(to, from)
							<< "'s substrate path " << endl;
					CommonFunctions::printVector(reDirection->getPathInfo());
					break;
				}
				// to next substrate path check
			} // Check whether there is substrate transport path exist, if exist, then record it and over the check

			/*
			 *  confirm virtual link whether ok?
			 *    if no,  the virtual network cannot be embedded
			 *    if yes, then next step for embedding process
			 */
			if (transportPath.find(vlIter->first) == transportPath.end()) {
				MessageSet::printInfoMessage(
						" Virtual link: " + vlIter->first
								+ " cannot be mapped by a substrate transport path in substrate network because that no enough bandwidth. Try to repeat embedding and checking.");

				checkFlag = false;
				break;
			} else {
				MessageSet::printInfoMessage(
						" Virtual link: " + vlIter->first
								+ "  can be mapped by a substrate transport path in substrate network, check successfully.");

				MessageSet::printInfoMessage(
						" Virtual link: "
								+ CommonFunctions::makeLinkId(to, from)
								+ "  can be mapped by a substrate transport path in substrate network, check successfully.");
				//one virtual link check over
				CalaculatedVirLinks.push_back(vlIter->first);
				CalaculatedVirLinks.push_back(
						CommonFunctions::makeLinkId(to, from));
			}

		} /*
		 virtual links embedding: substrate transport path check End
		 all virtual links are checked
		 */

		// in case of all virtual links can be embedded into substrate network
		if (true == checkFlag
				&& transportPath.size() == vNwGraph->getVirLinks().size()) {
			MessageSet::printInfoMessage(
					" All virtual links and nodes will be mapped into substrate network, check successfully.");

			/*
			 *emebeding process including embedding nodes and links
			 *return 0 : successful
			 */

			//firstly embedding virtual nodes
			map<string, SubNwNode*>::iterator phyIter;

			for (map<string, string>::iterator vNodeMapIter =
					vNwGraph->getMappedPhyNodes().begin();
					vNodeMapIter != vNwGraph->getMappedPhyNodes().end();
					++vNodeMapIter) {

				phyIter = this->subNW->getSubNodes().find(vNodeMapIter->second);
				if (phyIter != this->subNW->getSubNodes().end()) {

					map<string, VirNwNode*>::iterator vnIter =
							vNwGraph->getVirNodes().find(vNodeMapIter->first);

					if (vnIter != vNwGraph->getVirNodes().end()) {

						if (phyIter->second->getSubNodeAvaiableCapability()
								>= vnIter->second->getVirNodeCapability()) {

							vnIter->second->setVirNodeMappedSubNodeId(
									vNodeMapIter->second);

							vnIter->second->setVirNwNo(vnNo);

							phyIter->second->setSubNodeAvaiableCapability(
									phyIter->second->getSubNodeAvaiableCapability()
											- vnIter->second->getVirNodeCapability());

							phyIter->second->setSubNodeEmbeddedAmount(
									phyIter->second->getSubNodeEmbeddedAmount()
											+ 1);

							MessageSet::printInfoMessage(
									" Virtual node :" + vNodeMapIter->first
											+ " has been embedding into substrate node: "
											+ phyIter->first
											+ " successfully.");

						} else {
							MessageSet::printErrorMessage(
									" Virtual node: " + vNodeMapIter->first
											+ " can not be embedded into substrate node: "
											+ phyIter->first
											+ " because none enough capability.");
							MessageSet::printFailureExitMessage(
									" Virtual Network Embedding : embeddingVirtualNetwork()");
							CommonFunctions::FAILURE_EXIT();
						}
					} else {
						MessageSet::printErrorMessage(
								" Virtual node: " + vNodeMapIter->first
										+ " is not included inside virtual network.");
						MessageSet::printFailureExitMessage(
								" Virtual Network Embedding : embeddingVirtualNetwork()");
						CommonFunctions::FAILURE_EXIT();
					}

				} else {
					MessageSet::printErrorMessage(
							" Substrate node: " + vNodeMapIter->second
									+ " is not included inside substrate network.");

					MessageSet::printFailureExitMessage(
							" Virtual Network Embedding : embeddingVirtualNetwork()");
					CommonFunctions::FAILURE_EXIT();
				}
			}
			// embedding virtual nodes end

			// embedding vitual links start
			map<string, BasePath*>::iterator tranPathIter;
			for (map<string, VirNwLink*>::iterator vlIter =
					vNwGraph->getVirLinks().begin();
					vlIter != vNwGraph->getVirLinks().end(); ++vlIter) {

				tranPathIter = transportPath.find(vlIter->first);

				if (tranPathIter != transportPath.end()) {
					/*
					 * 1. configure the substrate transport path
					 * 2. configure the virtual link
					 */

					double switchingEnergyTransportPath = 0.0;
					double delayTransportPath = 0.0;
					double length = 0.0;

					for (unsigned int i = 1;
							i < tranPathIter->second->getPathInfo().size();
							++i) {

						string subLink_FromTo = CommonFunctions::makeLinkId(
								tranPathIter->second->getPathInfo().at(i - 1),
								tranPathIter->second->getPathInfo().at(i));

						// check physical transport path bandwidth whether is enough for the virtual link
						if (this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
								>= vlIter->second->getVirLinkBandwidth()) {

							double embedBandwidth =
									this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth();

							double updateBandwidth =
									vlIter->second->getVirLinkBandwidth();

							MessageSet::printInfoMessage(
									" Update substrate link: " + subLink_FromTo
											+ "'s embeddable bandwidth from "
											+ CommonFunctions::CoverDoubleToString(
													embedBandwidth)
											+ " reduce  "
											+ CommonFunctions::CoverDoubleToString(
													updateBandwidth));

							this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
									embedBandwidth - updateBandwidth);

							this->subNW->getSubLink(subLink_FromTo)->setSubLinkMappedVirLink(
									vnNo, vlIter->first);

							switchingEnergyTransportPath =
									switchingEnergyTransportPath
											+ this->subNW->getSubLink(
													subLink_FromTo)->getSubLinkSwitchingEnergy();

							delayTransportPath =
									delayTransportPath
											+ this->subNW->getSubLink(
													subLink_FromTo)->getSubLinkDelay();

							length =
									length
											+ this->subNW->getSubLink(
													subLink_FromTo)->getSubLinkWeight();

							MessageSet::printInfoMessage(
									" Updated substrate link: " + subLink_FromTo
											+ "'s embeddable bandwidth is "
											+ CommonFunctions::CoverDoubleToString(
													this->subNW->getSubLink(
															subLink_FromTo)->getSubLinkEmbeddableBandwidth()));

						} else {
							MessageSet::printErrorMessage(
									" Virtual link: " + vlIter->first
											+ "'s mapped substrate link is not enough bandwidth for embedding. Check process bug.");

							MessageSet::printFailureExitMessage(
									" Virtual Network Embedding : embeddingVirtualNetwork()");
							CommonFunctions::FAILURE_EXIT();
						}

					}

					vlIter->second->setVirNwNo(vnNo);
					vlIter->second->setVirLinkSwitchingEnergy(
							switchingEnergyTransportPath);
					vlIter->second->setVirLinkDelay(delayTransportPath);
					vlIter->second->setVirLinkSubPathHops(
							tranPathIter->second->getPathInfo().size() - 1);
					vlIter->second->setVirLinkWeight(length);

					tranPathIter->second->setPathHops(
							tranPathIter->second->getPathInfo().size() - 1);
					tranPathIter->second->setPathDeley(delayTransportPath);
					tranPathIter->second->setPathSwitchingEnergy(
							switchingEnergyTransportPath);
					tranPathIter->second->setPathDistance(length);
					tranPathIter->second->setPathBandwidth(
							vlIter->second->getVirLinkBandwidth());
					tranPathIter->second->setPathAvailableBandwidth(
							vlIter->second->getVirLinkBandwidth());

					tranPathIter->second->setPathId(
							CommonFunctions::makePathId(
									tranPathIter->second->getPathId(),
									CommonFunctions::CoverIntegerToString(
											vlIter->second->getVirLinkSubPaths().size())));

					vlIter->second->addVirLinkSubPath(tranPathIter->second);

				} else {
					MessageSet::printErrorMessage(
							" Virtual link: " + vlIter->first
									+ " can not be embeded into substrate network, check process begun. ");

					MessageSet::printFailureExitMessage(
							" VSirtual Network Embedding : embeddingVirtualNetwork()");
					CommonFunctions::FAILURE_EXIT();
				}
			}
			// embedding virtual link end
			MessageSet::printInfoMessage(
					" Currently, there are " + vnNo
							+ " virtual networks have been embedding into substrate network.");

			// configure duration time information
			vNwGraph->setVirNwStartTiming(timing);
			vNwGraph->setVirNwDurationTimeUnits(
					RandomGenerator::getVirtualNetworkDurationTimeWithUniformDistribution());
			vNwGraph->setVirNwEndTiming(
					vNwGraph->getVirNwStartTiming()
							+ vNwGraph->getVirNwDurationTimeUnits());

			vNwGraph->setVirNwNo(
					CommonFunctions::CoverIntegerToString(
							this->getAmountVNs() + 1));
			this->vNws.insert(
					pair<int, VirNwGraph*>(this->getAmountVNs() + 1, vNwGraph));
			this->setAmountVNs(this->getAmountVNs() + 1);

			//CommonFunctions::SleepThread(0, 0, 3);
			VirNwGraph::printVirtualNetworkInfo(vNwGraph);

			MessageSet::printInfoMessage(
					"InitiateEmbeddingVirtualNetworkEnvironment End.");

			return 0;
		} else { // the case of virtual network can not be embedded into the substrate network
			MessageSet::printInfoMessage(
					CommonFunctions::CoverIntegerToString(tryTime)
							+ "th embedding, all virtual links cannot be mapped into substrate substrate network at the same time. Try to repeat embedding and checking.");
		}

		MessageSet::printWarningMessage(
				CommonFunctions::CoverIntegerToString(tryTime)
						+ " th time virtual network embedding failed  because the capability of substrate network node is not enough, try to regenerate and embed a virtual network.");

		++tryTime;
		delete embedCheckSubNW;
	}

	MessageSet::printInfoMessage(
			"InitiateEmbeddingVirtualNetworkEnvironment End.");
	return 1; // more than 10 time try to embedding, failure.
}

/*
 *  embedding a virtual network based on virtual network request
 *   try to embed a virtual network in one time
 *    if success
 *       return 0
 *    else
 *       return 1
 */
int VirtualNetworkEmbedding::EmbeddingVirtualNetworkWithVirtualLinkSplittingRatio(
		VirNwGraph* vNwGraph, const int timing, const string pathsType) {

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::EmbeddingVirtualNetworkWithVirtualLinkSplittingRatio Start.");

	string vnNo = CommonFunctions::CoverIntegerToString(
			this->getAmountVNs() + 1);

	/*
	 *  checkFlag is used for confirm the virtual network's all virtual links  and nodes whether can be embedded.
	 */
	bool checkFlag = true;

	// the same as subNW, and hold latest substrate network information, used before embedding check processing
	SubNwGraph * embedCheckSubNW = this->subNW->copySubstrateNetwork();

	// check the capability of substrate network nodes whether can embed virtual network nodes.
	map<string, SubNwNode*>::iterator subIter;

	/*
	 * map<string, string>::iterator
	 *    first string:  virtual nodeId
	 *    second string: substrate nodeId
	 */
	for (map<string, string>::iterator vMapIter =
			vNwGraph->getMappedPhyNodes().begin();
			vMapIter != vNwGraph->getMappedPhyNodes().end(); ++vMapIter) {

		/*
		 * check mapped substrate  node whether exists inside substrate network
		 */
		subIter = embedCheckSubNW->getSubNodes().find(vMapIter->second);
		if (subIter != embedCheckSubNW->getSubNodes().end()) {

			/*
			 * check virtual  node whether exists inside vitual network.
			 */
			map<string, VirNwNode*>::iterator vIter =
					vNwGraph->getVirNodes().find(vMapIter->first);
			if (vIter != vNwGraph->getVirNodes().end()) {

				/*
				 * check the capability of subtrate node whether can embed the virtual node
				 */
				if (subIter->second->getSubNodeAvaiableCapability()
						< vIter->second->getVirNodeCapability()) {
					checkFlag = false;
					break;
				} else {
					subIter->second->setSubNodeAvaiableCapability(
							subIter->second->getSubNodeAvaiableCapability()
									- vIter->second->getVirNodeCapability());
				}

			} else {
				MessageSet::printErrorMessage(
						" virtual node: " + vMapIter->first
								+ " is not included inside virtual network. Check this bug. ");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetworkWithVirtualLinkSplittingRatio()");
				CommonFunctions::FAILURE_EXIT();
			}

		} else {
			MessageSet::printErrorMessage(
					" Substrate node: " + vMapIter->second
							+ " is not included inside substrate network. Check this bug.");

			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : embeddingVirtualNetworkWithVirtualLinkSplittingRatio()");
			CommonFunctions::FAILURE_EXIT();
		}
	} //EndFOR: check the capability of substrate network nodes whether can embed virtual network nodes end

	// there is virtual node cannot be embedded into substrate node
	if (false == checkFlag) {

		MessageSet::printWarningMessage(
				" Virtual network embedding with virtual link splitting ratio failed  because there is substrate network node's capability is not enough, try to regenerate and embed a virtual network, agaiin.");

		delete embedCheckSubNW;
		delete vNwGraph;

		MessageSet::printInfoMessage(
				"VirtualNetworkEmbedding::EmbeddingVirtualNetworkWithVirtualLinkSplittingRatio End.");

		return 1;

	} else {
		MessageSet::printInfoMessage(
				" All virtual nodes  may be mapped into substrate network, check successfully.");
		//CommonFunctions::SleepThread(0, 0, 3);
	}

	//The case of one virtual link mapped to multiple substrate links
	multimap<string, BasePath*> transportPath;

	// checked a_b, for  avoid b_a check
	vector<string> CalaculatedVirLinks;

	// a virtual network's virtual links embedding: substrate transport paths check start
	for (map<string, VirNwLink*>::iterator vlIter =
			vNwGraph->getVirLinks().begin();
			vlIter != vNwGraph->getVirLinks().end(); ++vlIter) {

		// confirme and avoid repeating virtual link calculate
		if (find(CalaculatedVirLinks.begin(), CalaculatedVirLinks.end(),
				vlIter->first) != CalaculatedVirLinks.end()) {
			continue;
		}

		cout << " Virtual link : " << vlIter->first << " ( "
				<< vlIter->second->getVirLinkBandwidth() << " M )" << endl;

		// get the two virtual nodes of a virtual link
		vector<string> fromTo = CommonFunctions::stringSplit(vlIter->first,
				'_');
		string from = fromTo.at(0);
		string to = fromTo.at(1);

		// get mapped substrate nodes of two virtual nodes
		string subFrom = "*";
		string subTo = "*";

		//check the two terminal nodes of virtual link
		map<string, string>::iterator mIter;
		mIter = vNwGraph->getMappedPhyNodes().find(from);
		if (mIter != vNwGraph->getMappedPhyNodes().end()) {
			subFrom = mIter->second;
		} else {
			MessageSet::printErrorMessage(
					" Virtual node: " + from
							+ "'s mapped substrate node is no existence in physical network.");
			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : embeddingVirtualNetwork()");
			CommonFunctions::FAILURE_EXIT();
		}

		mIter = vNwGraph->getMappedPhyNodes().find(to);
		if (mIter != vNwGraph->getMappedPhyNodes().end()) {
			subTo = mIter->second;
		} else {
			MessageSet::printErrorMessage(
					" Virtual node: " + to
							+ "'s mapped substrate node is no existence in physical network.");
			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
			CommonFunctions::FAILURE_EXIT();
		}

		if (subFrom == "*" || subTo == "*") {
			MessageSet::printErrorMessage(
					" Virtual link: " + vlIter->first
							+ " two terminals has no mapped substrate nodes in physical network.");
			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
			CommonFunctions::FAILURE_EXIT();
		}

		/*
		 * check bandwidth and confirm substrate transport path for a virtual link
		 * check from maximum hops for fairness bandwidth distribution
		 */

		// calculate shortest substrate transport paths for a virtual link for M:1
		vector<BasePath*> shortestPaths;
		if (pathsType
				== CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK) {
			shortestPaths =
					getTwoDisjointPathsWithComplexConstraintsOnSubstrateNetwork(
							embedCheckSubNW, subFrom, subTo,
							vlIter->second->getVirLinkBandwidth());
		} else {
			shortestPaths =
					getTwoDifferentPathsWithComplexConstraintsOnSubstrateNetwork(
							embedCheckSubNW, subFrom, subTo,
							vlIter->second->getVirLinkBandwidth());
		}

		if (shortestPaths.size() == 0) {
			MessageSet::printInfoMessage(
					" Virtual link: " + vlIter->first
							+ " has no substrate transport path existed in physical network. Try to repeat embedding  and checking once again.");
			checkFlag = false;
			break;
		}

		for (vector<BasePath*>::iterator pIter = shortestPaths.begin();
				pIter != shortestPaths.end(); ++pIter) {

			multimap<string, BasePath*>::iterator tIter = transportPath.begin();
			transportPath.insert(tIter,
					pair<string, BasePath*>(vlIter->first, (*pIter)));

			MessageSet::printInfoMessage(
					" Virtual link : " + vlIter->first + " 's "
							+ " One Substrate transport path: "
							+ (*pIter)->getPathId() + " is checked.");

			//substrate paths record
			BasePath* reDirection = new BasePath();

			reDirection->setPathSource((*pIter)->getPathDestination());
			reDirection->setPathDestination((*pIter)->getPathSource());
			reDirection->setPathId(
					CommonFunctions::makePathId(reDirection->getPathSource(),
							reDirection->getPathDestination()));

			reDirection->setPathDeley((*pIter)->getPathDeley());
			reDirection->setPathCost((*pIter)->getPathCost());
			reDirection->setPathDistance((*pIter)->getPathDistance());
			reDirection->setPathHops((*pIter)->getPathHops());

			vector<string> pInfo = (*pIter)->getPathInfo();

			reverse(pInfo.begin(), pInfo.end());
			reDirection->setPathInfo(pInfo);
			reDirection->setPathSwitchingEnergy(
					(*pIter)->getPathSwitchingEnergy());

			reDirection->setPathBandwidth((*pIter)->getPathBandwidth());
			reDirection->setPathAvailableBandwidth(
					(*pIter)->getPathAvailableBandwidth());

			tIter = transportPath.begin();
			transportPath.insert(tIter,
					pair<string, BasePath*>(
							CommonFunctions::makeLinkId(to, from),
							reDirection));

			MessageSet::printInfoMessage(
					" Virtual link : " + CommonFunctions::makeLinkId(to, from)
							+ "'s " + " One Substrate transport path: "
							+ reDirection->getPathId() + " is checked.");

			cout << " Virtual link :" << vlIter->first << "'s substrate path : "
					<< endl;
			CommonFunctions::printVector((*pIter)->getPathInfo());

			cout << " Virtual link :" << CommonFunctions::makeLinkId(to, from)
					<< "'s substrate path : " << endl;
			CommonFunctions::printVector(reDirection->getPathInfo());

		} // EndFor

		if (transportPath.count(vlIter->first) == shortestPaths.size()
				&& transportPath.count(CommonFunctions::makeLinkId(to, from))
						== shortestPaths.size()) {
			//one virtual link check over
			CalaculatedVirLinks.push_back(vlIter->first);
			CalaculatedVirLinks.push_back(
					CommonFunctions::makeLinkId(to, from));
		} else {
			MessageSet::printErrorMessage(
					" for virtual link: " + vlIter->first + "( "
							+ CommonFunctions::CoverDoubleToString(
									vlIter->second->getVirLinkBandwidth())
							+ "M, substrate path amount "
							+ CommonFunctions::CoverIntegerToString(
									shortestPaths.size()) + ") only allocate "
							+ CommonFunctions::CoverIntegerToString(
									transportPath.count(vlIter->first))
							+ " substrate paths ");

			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
			CommonFunctions::FAILURE_EXIT();
		}

	} //EndFOR a virtual network's virtual links embedding: substrate transport paths check end

	if (checkFlag == true) {

		MessageSet::printInfoMessage(
				" All virtual links and nodes will be mapped into substrate network, check successfully.");

		/*
		 *emebeding process including embedding nodes and links
		 *return 0 : successful
		 */

		//firstly embedding virtual nodes
		map<string, SubNwNode*>::iterator phyIter;

		for (map<string, string>::iterator vNodeMapIter =
				vNwGraph->getMappedPhyNodes().begin();
				vNodeMapIter != vNwGraph->getMappedPhyNodes().end();
				++vNodeMapIter) {

			phyIter = this->subNW->getSubNodes().find(vNodeMapIter->second);
			if (phyIter != this->subNW->getSubNodes().end()) {

				map<string, VirNwNode*>::iterator vnIter =
						vNwGraph->getVirNodes().find(vNodeMapIter->first);

				if (vnIter != vNwGraph->getVirNodes().end()) {

					if (phyIter->second->getSubNodeAvaiableCapability()
							>= vnIter->second->getVirNodeCapability()) {

						vnIter->second->setVirNodeMappedSubNodeId(
								vNodeMapIter->second);

						vnIter->second->setVirNwNo(vnNo);

						phyIter->second->setSubNodeAvaiableCapability(
								phyIter->second->getSubNodeAvaiableCapability()
										- vnIter->second->getVirNodeCapability());

						phyIter->second->setSubNodeEmbeddedAmount(
								phyIter->second->getSubNodeEmbeddedAmount()
										+ 1);

						MessageSet::printInfoMessage(
								" Virtual node :" + vNodeMapIter->first
										+ " has been embedding into substrate node: "
										+ phyIter->first + " successfully.");

					} else {
						MessageSet::printErrorMessage(
								" Virtual node: " + vNodeMapIter->first
										+ " can not be embedded into substrate node: "
										+ phyIter->first
										+ " because none enough capability.");
						MessageSet::printFailureExitMessage(
								" Virtual Network Embedding : embeddingVirtualNetwork()");
						CommonFunctions::FAILURE_EXIT();
					}
				} else {
					MessageSet::printErrorMessage(
							" Virtual node: " + vNodeMapIter->first
									+ " is not included inside virtual network.");
					MessageSet::printFailureExitMessage(
							" Virtual Network Embedding : embeddingVirtualNetwork()");
					CommonFunctions::FAILURE_EXIT();
				}

			} else {
				MessageSet::printErrorMessage(
						" Substrate node: " + vNodeMapIter->second
								+ " is not included inside substrate network.");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}
		}
		// embedding virtual nodes end

		// re-embedding virtual links
		for (map<string, VirNwLink*>::iterator vlIter =
				vNwGraph->getVirLinks().begin();
				vlIter != vNwGraph->getVirLinks().end(); ++vlIter) {

			pair<multimap<string, BasePath*>::iterator,
					multimap<string, BasePath*>::iterator> ret;

			ret = transportPath.equal_range(vlIter->first);

			// embedding virtual link
			for (multimap<string, BasePath*>::iterator tranPathIter = ret.first;
					tranPathIter != ret.second; ++tranPathIter) {

				if (tranPathIter->second != NULL) {

					/*
					 * 1. configure the substrate transport path
					 * 2. configure the virtual link
					 */

					double switchingEnergyTransportPath = 0.0;
					double delayTransportPath = 0.0;
					double length = 0.0;

					for (unsigned int i = 1;
							i < tranPathIter->second->getPathInfo().size();
							++i) {

						string subLink_FromTo = CommonFunctions::makeLinkId(
								tranPathIter->second->getPathInfo().at(i - 1),
								tranPathIter->second->getPathInfo().at(i));

						cout << " ******:**** " << endl;
						cout << " virtual link: " << vlIter->first
								<< " Bandwidth: "
								<< vlIter->second->getVirLinkBandwidth()
								<< endl;
						// check physical transport path bandwidth whether is enough
						cout << " subLink : " << subLink_FromTo
								<< " 's embeddable bandwidth "
								<< this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
								<< endl;

						cout << " subPath : "
								<< tranPathIter->second->getPathId()
								<< " 's bandwidth : "
								<< tranPathIter->second->getPathBandwidth()
								<< endl;

						if (this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
								>= tranPathIter->second->getPathBandwidth()) {

							double embedBandwidth =
									this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth();

							double updateBandwidth =
									tranPathIter->second->getPathBandwidth();

							MessageSet::printInfoMessage(
									" Update substrate link: " + subLink_FromTo
											+ "'s embeddable bandwidth from "
											+ CommonFunctions::CoverDoubleToString(
													embedBandwidth)
											+ " reduce  "
											+ CommonFunctions::CoverDoubleToString(
													updateBandwidth));

							this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
									embedBandwidth - updateBandwidth);

							this->subNW->getSubLink(subLink_FromTo)->setSubLinkMappedVirLink(
									vNwGraph->getVirNwNo(), vlIter->first);

							switchingEnergyTransportPath =
									switchingEnergyTransportPath
											+ this->subNW->getSubLink(
													subLink_FromTo)->getSubLinkSwitchingEnergy();

							delayTransportPath =
									delayTransportPath
											+ this->subNW->getSubLink(
													subLink_FromTo)->getSubLinkDelay();

							length =
									length
											+ this->subNW->getSubLink(
													subLink_FromTo)->getSubLinkWeight();

							MessageSet::printInfoMessage(
									" Updated substrate link: " + subLink_FromTo
											+ "'s embeddable bandwidth is "
											+ CommonFunctions::CoverDoubleToString(
													this->subNW->getSubLink(
															subLink_FromTo)->getSubLinkEmbeddableBandwidth()));

						} else {
							MessageSet::printErrorMessage(
									" Virtual link: " + vlIter->first
											+ "'s mapped substrate transport path is not enough bandwidth for embedding. Check process bug.");

							MessageSet::printFailureExitMessage(
									" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio() ");
							CommonFunctions::FAILURE_EXIT();
						}

					} //EndFor

					vlIter->second->setVirNwNo(vnNo);

					tranPathIter->second->setPathHops(
							tranPathIter->second->getPathInfo().size() - 1);
					tranPathIter->second->setPathDeley(delayTransportPath);
					tranPathIter->second->setPathSwitchingEnergy(
							switchingEnergyTransportPath);
					tranPathIter->second->setPathDistance(length);

					tranPathIter->second->setPathId(
							CommonFunctions::makePathId(
									tranPathIter->second->getPathId(),
									CommonFunctions::CoverIntegerToString(
											vlIter->second->getVirLinkSubPaths().size())));

					vlIter->second->addVirLinkSubPath(tranPathIter->second);

				} else {
					MessageSet::printErrorMessage(
							" Virtual link: " + vlIter->first
									+ " has no substrate transport path in this step, it is bug. ");

					MessageSet::printFailureExitMessage(
							" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio(). ");
					CommonFunctions::FAILURE_EXIT();
				} //EndIF

			} // EndFor embedding a virtual link

		} // EndFor all virtual links

		// embedding virtual link end
		MessageSet::printInfoMessage(
				" Currently, there are " + vnNo
						+ " virtual networks have been embedding into substrate network.");

		// configure duration time information
		vNwGraph->setVirNwStartTiming(timing);
		vNwGraph->setVirNwDurationTimeUnits(
				RandomGenerator::getVirtualNetworkDurationTimeWithUniformDistribution());
		vNwGraph->setVirNwEndTiming(
				vNwGraph->getVirNwStartTiming()
						+ vNwGraph->getVirNwDurationTimeUnits());

		vNwGraph->setVirNwNo(
				CommonFunctions::CoverIntegerToString(
						this->getAmountVNs() + 1));

		this->vNws.insert(
				pair<int, VirNwGraph*>(this->getAmountVNs() + 1, vNwGraph));

		this->setAmountVNs(this->getAmountVNs() + 1);

		//CommonFunctions::SleepThread(0, 0, 3);
		VirNwGraph::printVirtualNetworkInfo(vNwGraph);

		MessageSet::printInfoMessage(
				"VirtualNetworkEmbedding::EmbeddingVirtualNetworkWithVirtualLinkSplittingRatio End.");

		return 0;

	} else { // the case of virtual network can not be embedded into the substrate network
		MessageSet::printInfoMessage(
				" All virtual links cannot be mapped into substrate substrate network at the same time. Try to repeat embedding and checking.");
	}

	MessageSet::printWarningMessage(
			" Virtual network embedding failed  because the capability of substrate network node is not enough, try to regenerate and embed a virtual network.");

	delete embedCheckSubNW;
	delete vNwGraph;

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::EmbeddingVirtualNetworkWithVirtualLinkSplittingRatio End.");

	return 1; // more than 10 time try to embedding, failure.

}

/*
 *  Initiate virtual network environment by embedding a virtual network based on virtual network request
 *   try to embed a virtual network in ten times
 *    if success
 *       return 0
 *    else
 *       return 1
 *
 *  pathsType:
 *      CommonConstants::DIFFERENT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK
 *       or
 *		CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK
 */
int VirtualNetworkEmbedding::InitiateEmbeddingVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
		const int timing, const string pathsType) {

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::InitiateEmbeddingVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio Start.");

	string vnNo = CommonFunctions::CoverIntegerToString(
			this->getAmountVNs() + 1);

	/*
	 *  checkFlag is used for confirm the virtual network's all virtual links  and nodes whether can be embedded.
	 */
	bool checkFlag;

	/*
	 * try to build a virtual network in 10 time.
	 */
	int tryTime = 1;
	while (tryTime <= 10) {

		checkFlag = true;

		// the same as subNW, and hold latest substrate network information, used before embedding check processing
		SubNwGraph * embedCheckSubNW = this->subNW->copySubstrateNetwork();

		VirNwGraph * vNwGraph = constructVirtualNetwork(timing);

		// check the capability of substrate network nodes whether can embed virtual network nodes.
		map<string, SubNwNode*>::iterator subIter;

		/*
		 * map<string, string>::iterator
		 *    first string:  virtual nodeId
		 *    second string: substrate nodeId
		 */
		for (map<string, string>::iterator vMapIter =
				vNwGraph->getMappedPhyNodes().begin();
				vMapIter != vNwGraph->getMappedPhyNodes().end(); ++vMapIter) {

			/*
			 * check mapped substrate  node whether exists inside substrate network
			 */
			subIter = embedCheckSubNW->getSubNodes().find(vMapIter->second);
			if (subIter != embedCheckSubNW->getSubNodes().end()) {

				/*
				 * check virtual  node whether exists inside vitual network.
				 */
				map<string, VirNwNode*>::iterator vIter =
						vNwGraph->getVirNodes().find(vMapIter->first);
				if (vIter != vNwGraph->getVirNodes().end()) {

					/*
					 * check the capability of subtrate node whether can embed the virtual node
					 */
					if (subIter->second->getSubNodeAvaiableCapability()
							< vIter->second->getVirNodeCapability()) {
						checkFlag = false;
						break;
					} else {
						subIter->second->setSubNodeAvaiableCapability(
								subIter->second->getSubNodeAvaiableCapability()
										- vIter->second->getVirNodeCapability());
					}

				} else {
					MessageSet::printErrorMessage(
							" virtual node: " + vMapIter->first
									+ " is not included inside virtual network. Check this bug. ");

					MessageSet::printFailureExitMessage(
							" Virtual Network Embedding : embeddingVirtualNetworkWithVirtualLinkSplittingRatio()");
					CommonFunctions::FAILURE_EXIT();
				}

			} else {
				MessageSet::printErrorMessage(
						" Substrate node: " + vMapIter->second
								+ " is not included inside substrate network. Check this bug.");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetworkWithVirtualLinkSplittingRatio()");
				CommonFunctions::FAILURE_EXIT();
			}
		} //EndFOR: check the capability of substrate network nodes whether can embed virtual network nodes end

		// there is virtual node cannot be embedded into substrate node
		if (false == checkFlag) {

			MessageSet::printWarningMessage(
					CommonFunctions::CoverIntegerToString(tryTime)
							+ " th time virtual network embedding with virtual link splitting ratio failed  because there is substrate network node's capability is not enough, try to regenerate and embed a virtual network, agaiin.");
			++tryTime;

			delete embedCheckSubNW;
			delete vNwGraph;

			continue;

		} else {
			MessageSet::printInfoMessage(
					" All virtual nodes  may be mapped into substrate network, check successfully.");
			//CommonFunctions::SleepThread(0, 0, 3);
		}

		//The case of one virtual link mapped to multiple substrate links
		multimap<string, BasePath*> transportPath;

		// checked a_b, for  avoid b_a check
		vector<string> CalaculatedVirLinks;

		// a virtual network's virtual links embedding: substrate transport paths check start
		for (map<string, VirNwLink*>::iterator vlIter =
				vNwGraph->getVirLinks().begin();
				vlIter != vNwGraph->getVirLinks().end(); ++vlIter) {

			// confirme and avoid repeating virtual link calculate
			if (find(CalaculatedVirLinks.begin(), CalaculatedVirLinks.end(),
					vlIter->first) != CalaculatedVirLinks.end()) {
				continue;
			}

			cout << " Virtual link : " << vlIter->first << " ( "
					<< vlIter->second->getVirLinkBandwidth() << " M )" << endl;

			// get the two virtual nodes of a virtual link
			vector<string> fromTo = CommonFunctions::stringSplit(vlIter->first,
					'_');
			string from = fromTo.at(0);
			string to = fromTo.at(1);

			// get mapped substrate nodes of two virtual nodes
			string subFrom = "*";
			string subTo = "*";

			//check the two terminal nodes of virtual link
			map<string, string>::iterator mIter;
			mIter = vNwGraph->getMappedPhyNodes().find(from);
			if (mIter != vNwGraph->getMappedPhyNodes().end()) {
				subFrom = mIter->second;
			} else {
				MessageSet::printErrorMessage(
						" Virtual node: " + from
								+ "'s mapped substrate node is no existence in physical network.");
				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}

			mIter = vNwGraph->getMappedPhyNodes().find(to);
			if (mIter != vNwGraph->getMappedPhyNodes().end()) {
				subTo = mIter->second;
			} else {
				MessageSet::printErrorMessage(
						" Virtual node: " + to
								+ "'s mapped substrate node is no existence in physical network.");
				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
				CommonFunctions::FAILURE_EXIT();
			}

			if (subFrom == "*" || subTo == "*") {
				MessageSet::printErrorMessage(
						" Virtual link: " + vlIter->first
								+ " two terminals has no mapped substrate nodes in physical network.");
				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
				CommonFunctions::FAILURE_EXIT();
			}

			/*
			 * check bandwidth and confirm substrate transport path for a virtual link
			 * check from maximum hops for fairness bandwidth distribution
			 */

			// calculate shortest substrate transport paths for a virtual link for M:1
			vector<BasePath*> shortestPaths;
			if (pathsType
					== CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK) {
				shortestPaths =
						getTwoDisjointPathsWithComplexConstraintsOnSubstrateNetwork(
								embedCheckSubNW, subFrom, subTo,
								vlIter->second->getVirLinkBandwidth());
			} else {
				shortestPaths =
						getTwoDifferentPathsWithComplexConstraintsOnSubstrateNetwork(
								embedCheckSubNW, subFrom, subTo,
								vlIter->second->getVirLinkBandwidth());
			}

			if (shortestPaths.size() == 0) {
				MessageSet::printInfoMessage(
						" Virtual link: " + vlIter->first
								+ " has no substrate transport path existed in physical network. Try to repeat embedding  and checking once again.");
				checkFlag = false;
				break;
			}

			for (vector<BasePath*>::iterator pIter = shortestPaths.begin();
					pIter != shortestPaths.end(); ++pIter) {

				multimap<string, BasePath*>::iterator tIter =
						transportPath.begin();
				transportPath.insert(tIter,
						pair<string, BasePath*>(vlIter->first, (*pIter)));

				MessageSet::printInfoMessage(
						" Virtual link : " + vlIter->first + " 's "
								+ " One Substrate transport path: "
								+ (*pIter)->getPathId() + " is checked.");

				//substrate paths record
				BasePath* reDirection = new BasePath();

				reDirection->setPathSource((*pIter)->getPathDestination());
				reDirection->setPathDestination((*pIter)->getPathSource());
				reDirection->setPathId(
						CommonFunctions::makePathId(
								reDirection->getPathSource(),
								reDirection->getPathDestination()));

				reDirection->setPathDeley((*pIter)->getPathDeley());

				reDirection->setPathCost((*pIter)->getPathCost());
				reDirection->setPathDistance((*pIter)->getPathDistance());
				reDirection->setPathHops((*pIter)->getPathHops());

				vector<string> pInfo = (*pIter)->getPathInfo();

				reverse(pInfo.begin(), pInfo.end());
				reDirection->setPathInfo(pInfo);
				reDirection->setPathSwitchingEnergy(
						(*pIter)->getPathSwitchingEnergy());

				reDirection->setPathBandwidth((*pIter)->getPathBandwidth());
				reDirection->setPathAvailableBandwidth(
						(*pIter)->getPathAvailableBandwidth());

				tIter = transportPath.begin();
				transportPath.insert(tIter,
						pair<string, BasePath*>(
								CommonFunctions::makeLinkId(to, from),
								reDirection));

				MessageSet::printInfoMessage(
						" Virtual link : "
								+ CommonFunctions::makeLinkId(to, from) + "'s "
								+ " One Substrate transport path: "
								+ reDirection->getPathId() + " is checked.");

				cout << " Virtual link :" << vlIter->first
						<< "'s substrate path : " << endl;
				CommonFunctions::printVector((*pIter)->getPathInfo());

				cout << " Virtual link :"
						<< CommonFunctions::makeLinkId(to, from)
						<< "'s substrate path : " << endl;
				CommonFunctions::printVector(reDirection->getPathInfo());

			} // EndFor

			if (transportPath.count(vlIter->first) == shortestPaths.size()
					&& transportPath.count(
							CommonFunctions::makeLinkId(to, from))
							== shortestPaths.size()) {
				//one virtual link check over
				CalaculatedVirLinks.push_back(vlIter->first);
				CalaculatedVirLinks.push_back(
						CommonFunctions::makeLinkId(to, from));
			} else {
				MessageSet::printErrorMessage(
						" for virtual link: " + vlIter->first + "( "
								+ CommonFunctions::CoverDoubleToString(
										vlIter->second->getVirLinkBandwidth())
								+ "M, substrate path amount "
								+ CommonFunctions::CoverIntegerToString(
										shortestPaths.size())
								+ ") only allocate "
								+ CommonFunctions::CoverIntegerToString(
										transportPath.count(vlIter->first))
								+ " substrate paths ");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
				CommonFunctions::FAILURE_EXIT();
			}

		} //EndFOR a virtual network's virtual links embedding: substrate transport paths check end

		if (checkFlag == true) {

			MessageSet::printInfoMessage(
					" All virtual links and nodes will be mapped into substrate network, check successfully.");

			/*
			 *emebeding process including embedding nodes and links
			 *return 0 : successful
			 */

			//firstly embedding virtual nodes
			map<string, SubNwNode*>::iterator phyIter;

			for (map<string, string>::iterator vNodeMapIter =
					vNwGraph->getMappedPhyNodes().begin();
					vNodeMapIter != vNwGraph->getMappedPhyNodes().end();
					++vNodeMapIter) {

				phyIter = this->subNW->getSubNodes().find(vNodeMapIter->second);
				if (phyIter != this->subNW->getSubNodes().end()) {

					map<string, VirNwNode*>::iterator vnIter =
							vNwGraph->getVirNodes().find(vNodeMapIter->first);

					if (vnIter != vNwGraph->getVirNodes().end()) {

						if (phyIter->second->getSubNodeAvaiableCapability()
								>= vnIter->second->getVirNodeCapability()) {

							vnIter->second->setVirNodeMappedSubNodeId(
									vNodeMapIter->second);

							vnIter->second->setVirNwNo(vnNo);

							phyIter->second->setSubNodeAvaiableCapability(
									phyIter->second->getSubNodeAvaiableCapability()
											- vnIter->second->getVirNodeCapability());

							phyIter->second->setSubNodeEmbeddedAmount(
									phyIter->second->getSubNodeEmbeddedAmount()
											+ 1);

							MessageSet::printInfoMessage(
									" Virtual node :" + vNodeMapIter->first
											+ " has been embedding into substrate node: "
											+ phyIter->first
											+ " successfully.");

						} else {
							MessageSet::printErrorMessage(
									" Virtual node: " + vNodeMapIter->first
											+ " can not be embedded into substrate node: "
											+ phyIter->first
											+ " because none enough capability.");
							MessageSet::printFailureExitMessage(
									" Virtual Network Embedding : embeddingVirtualNetwork()");
							CommonFunctions::FAILURE_EXIT();
						}
					} else {
						MessageSet::printErrorMessage(
								" Virtual node: " + vNodeMapIter->first
										+ " is not included inside virtual network.");
						MessageSet::printFailureExitMessage(
								" Virtual Network Embedding : embeddingVirtualNetwork()");
						CommonFunctions::FAILURE_EXIT();
					}

				} else {
					MessageSet::printErrorMessage(
							" Substrate node: " + vNodeMapIter->second
									+ " is not included inside substrate network.");

					MessageSet::printFailureExitMessage(
							" Virtual Network Embedding : embeddingVirtualNetwork()");
					CommonFunctions::FAILURE_EXIT();
				}
			}
			// embedding virtual nodes end

			// re-embedding virtual links
			for (map<string, VirNwLink*>::iterator vlIter =
					vNwGraph->getVirLinks().begin();
					vlIter != vNwGraph->getVirLinks().end(); ++vlIter) {

				pair<multimap<string, BasePath*>::iterator,
						multimap<string, BasePath*>::iterator> ret;

				ret = transportPath.equal_range(vlIter->first);

				// embedding virtual link
				for (multimap<string, BasePath*>::iterator tranPathIter =
						ret.first; tranPathIter != ret.second; ++tranPathIter) {

					if (tranPathIter->second != NULL) {

						/*
						 * 1. configure the substrate transport path
						 * 2. configure the virtual link
						 */

						double switchingEnergyTransportPath = 0.0;
						double delayTransportPath = 0.0;
						double length = 0.0;

						for (unsigned int i = 1;
								i < tranPathIter->second->getPathInfo().size();
								++i) {

							string subLink_FromTo = CommonFunctions::makeLinkId(
									tranPathIter->second->getPathInfo().at(
											i - 1),
									tranPathIter->second->getPathInfo().at(i));

							cout << " ******:**** " << endl;
							cout << " virtual link: " << vlIter->first
									<< " Bandwidth: "
									<< vlIter->second->getVirLinkBandwidth()
									<< endl;
							// check physical transport path bandwidth whether is enough
							cout << " subLink : " << subLink_FromTo
									<< " 's embeddable bandwidth "
									<< this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
									<< endl;

							cout << " subPath : "
									<< tranPathIter->second->getPathId()
									<< " 's bandwidth : "
									<< tranPathIter->second->getPathBandwidth()
									<< endl;

							if (this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
									>= tranPathIter->second->getPathBandwidth()) {

								double embedBandwidth =
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth();

								double updateBandwidth =
										tranPathIter->second->getPathBandwidth();

								MessageSet::printInfoMessage(
										" Update substrate link: "
												+ subLink_FromTo
												+ "'s embeddable bandwidth from "
												+ CommonFunctions::CoverDoubleToString(
														embedBandwidth)
												+ " reduce  "
												+ CommonFunctions::CoverDoubleToString(
														updateBandwidth));

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
										embedBandwidth - updateBandwidth);

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkMappedVirLink(
										vNwGraph->getVirNwNo(), vlIter->first);

								switchingEnergyTransportPath =
										switchingEnergyTransportPath
												+ this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkSwitchingEnergy();

								delayTransportPath =
										delayTransportPath
												+ this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkDelay();

								length =
										length
												+ this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkWeight();

								MessageSet::printInfoMessage(
										" Updated substrate link: "
												+ subLink_FromTo
												+ "'s embeddable bandwidth is "
												+ CommonFunctions::CoverDoubleToString(
														this->subNW->getSubLink(
																subLink_FromTo)->getSubLinkEmbeddableBandwidth()));

							} else {
								MessageSet::printErrorMessage(
										" Virtual link: " + vlIter->first
												+ "'s mapped substrate transport path is not enough bandwidth for embedding. Check process bug.");

								MessageSet::printFailureExitMessage(
										" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio() ");
								CommonFunctions::FAILURE_EXIT();
							}

						} //EndFor

						vlIter->second->setVirNwNo(vnNo);

						tranPathIter->second->setPathHops(
								tranPathIter->second->getPathInfo().size() - 1);
						tranPathIter->second->setPathDeley(delayTransportPath);
						tranPathIter->second->setPathSwitchingEnergy(
								switchingEnergyTransportPath);
						tranPathIter->second->setPathDistance(length);

						cout << " amount of substrate paths: "
								<< vlIter->second->getVirLinkSubPaths().size()
								<< endl;

						tranPathIter->second->setPathId(
								CommonFunctions::makePathId(
										tranPathIter->second->getPathId(),
										CommonFunctions::CoverIntegerToString(
												vlIter->second->getVirLinkSubPaths().size())));

						vlIter->second->addVirLinkSubPath(tranPathIter->second);
					} else {
						MessageSet::printErrorMessage(
								" Virtual link: " + vlIter->first
										+ " has no substrate transport path in this step, it is bug. ");

						MessageSet::printFailureExitMessage(
								" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio(). ");
						CommonFunctions::FAILURE_EXIT();
					} //EndIF

				} // EndFor embedding a virtual link

			} // EndFor all virtual links

			// embedding virtual link end
			MessageSet::printInfoMessage(
					" Currently, there are " + vnNo
							+ " virtual networks have been embedding into substrate network.");

			// configure duration time information
			vNwGraph->setVirNwStartTiming(timing);
			vNwGraph->setVirNwDurationTimeUnits(
					RandomGenerator::getVirtualNetworkDurationTimeWithUniformDistribution());
			vNwGraph->setVirNwEndTiming(
					vNwGraph->getVirNwStartTiming()
							+ vNwGraph->getVirNwDurationTimeUnits());

			vNwGraph->setVirNwNo(
					CommonFunctions::CoverIntegerToString(
							this->getAmountVNs() + 1));

			this->vNws.insert(
					pair<int, VirNwGraph*>(this->getAmountVNs() + 1, vNwGraph));

			this->setAmountVNs(this->getAmountVNs() + 1);

			//CommonFunctions::SleepThread(0, 0, 3);
			VirNwGraph::printVirtualNetworkInfo(vNwGraph);

			return 0;

		} else { // the case of virtual network can not be embedded into the substrate network
			MessageSet::printInfoMessage(
					CommonFunctions::CoverIntegerToString(tryTime)
							+ "th embedding, all virtual links cannot be mapped into substrate substrate network at the same time. Try to repeat embedding and checking.");
		}

		MessageSet::printWarningMessage(
				CommonFunctions::CoverIntegerToString(tryTime)
						+ " th time virtual network embedding failed  because the capability of substrate network node is not enough, try to regenerate and embed a virtual network.");

		++tryTime;
		delete embedCheckSubNW;
		delete vNwGraph;
	}

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::InitiateEmbeddingVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio End.");

	return 1; // more than 10 time try to embedding, failure.
}

int VirtualNetworkEmbedding::getAmountVNs() {
	return amountVNs;
}

void VirtualNetworkEmbedding::setAmountVNs(int amountVNs) {
	this->amountVNs = amountVNs;
}

SubNwGraph*& VirtualNetworkEmbedding::getSubNw() {
	return subNW;
}

void VirtualNetworkEmbedding::setSubNw(SubNwGraph*& subNw) {
	this->subNW = subNw;
}

map<int, VirNwGraph*>& VirtualNetworkEmbedding::getNws() {
	return vNws;
}

void VirtualNetworkEmbedding::setNws(const map<int, VirNwGraph*>& nws) {
	vNws = nws;
}

KShortestPaths*& VirtualNetworkEmbedding::getShortPaths() {
	return kShortPaths;
}

void VirtualNetworkEmbedding::setShortPaths(KShortestPaths*& shortPaths) {
	kShortPaths = shortPaths;
}

VirtualNetworkEmbedding* VirtualNetworkEmbedding::copyVirtualNetworkEmbeddingEnvironment() {
	MessageSet::printInfoMessage(
			" Copy Virtual Network Embedding Environment Start. ");
	VirtualNetworkEmbedding* copyVNEmedding = new VirtualNetworkEmbedding();

	copyVNEmedding->setAmountVNs(this->getAmountVNs());
	copyVNEmedding->setShortPaths(this->getShortPaths());

	SubNwGraph * copySubNW = this->subNW->copySubstrateNetwork();
	copyVNEmedding->setSubNw(copySubNW);

	map<int, VirNwGraph*> copyVirNWs;
	for (map<int, VirNwGraph*>::iterator iter = this->getNws().begin();
			iter != this->getNws().end(); ++iter) {
		VirNwGraph* copyVirNw = iter->second->copyVirtualNetwork();
		map<int, VirNwGraph*>::iterator vIter = copyVirNWs.begin();
		copyVirNWs.insert(vIter,
				pair<int, VirNwGraph*>(iter->first, copyVirNw));
	}
	copyVNEmedding->setNws(copyVirNWs);

	MessageSet::printInfoMessage(
			" Copy  A Virtual Network Embedding Environment End. ");
	return copyVNEmedding;

}

/*
 * clear the mapping of virtual links  on a virtual network embedding environment
 * for reallocation of virtual link resource
 */

void VirtualNetworkEmbedding::clearVirtualLinksResourceMappingAndAllocations() {
	MessageSet::printInfoMessage(
			" Clear Virtual links Resource allocations and mapping on Virtual Network Embedding Environment Start. ");

	for (map<int, VirNwGraph*>::iterator vnwIter = this->getNws().begin();
			vnwIter != this->getNws().end(); ++vnwIter) {

		for (map<string, VirNwLink*>::iterator vlIter =
				vnwIter->second->getVirLinks().begin();
				vlIter != vnwIter->second->getVirLinks().end(); ++vlIter) {

			vlIter->second->setVirLinkSubPathHops(0);
			vlIter->second->setVirLinkSwitchingEnergy(0.0);
			vlIter->second->setVirLinkWeight(0.0);
			vlIter->second->setVirLinkDelay(0.0);
			for (vector<BasePath*>::iterator bpIter =
					vlIter->second->getVirLinkSubPaths().begin();
					bpIter != vlIter->second->getVirLinkSubPaths().end();
					++bpIter) {
				delete *bpIter;
			}
			vlIter->second->getVirLinkSubPaths().clear();

			//VirNwLink::printVirNwLinkInfo(vlIter->second);

			for (map<string, SubNwLink*>::iterator slIter =
					this->getSubNw()->getSubLinks().begin();
					slIter != this->getSubNw()->getSubLinks().end(); ++slIter) {
				slIter->second->getSubLinkMappedVirLinks().clear();
				//reset embeddable bandwidth;
				slIter->second->setSubLinkEmbeddableBandwidth(
						slIter->second->getSubLinkAvailableBandwidth());
				//SubNwLink::printSubLinkInfo(slIter->second);
			}

		}
	}

	MessageSet::printInfoMessage(
			" Clear Virtual Links Resource Allocations and Mapping on Virtual Network Embedding Environment End. ");
}

/* pathsType :two substrate paths type
 *   :  "Different":
 *   :  "Disjoint" :
 */
void VirtualNetworkEmbedding::reallocationVirtualLinksResourceBySplittingRatio(
		const string pathsType) {

	MessageSet::printInfoMessage(
			"reallocationVirtualLinksResourceBySplittingRatio Start.");

// all virtual networks' virtual link resource reallocation Start
	for (map<int, VirNwGraph*>::iterator vnwIter = this->getNws().begin();
			vnwIter != this->getNws().end(); ++vnwIter) {

		cout << " Reallocaton Virtual network: "
				<< vnwIter->second->getVirNwNo() << endl;
		/*
		 * get the latest substrate network resource information
		 */
		SubNwGraph * embedCheckSubNW = this->subNW->copySubstrateNetwork();

		//The case of one virtual link mapped to multiple substrate links
		multimap<string, BasePath*> transportPath;

		/*
		 *  checkFlag is used for confirm the virtual network's all virtual links whether can be embedded.
		 */
		bool checkFlag = true;

		// checked a_b, for  avoid b_a check
		vector<string> CalaculatedVirLinks;

		// a virtual network's virtual links embedding: substrate transport paths check start
		for (map<string, VirNwLink*>::iterator vlIter =
				vnwIter->second->getVirLinks().begin();
				vlIter != vnwIter->second->getVirLinks().end(); ++vlIter) {

			// confirme and avoid repeating virtual link calculate
			if (find(CalaculatedVirLinks.begin(), CalaculatedVirLinks.end(),
					vlIter->first) != CalaculatedVirLinks.end()) {
				continue;
			}

			cout << " Virtual link : " << vlIter->first << " ( "
					<< vlIter->second->getVirLinkBandwidth() << " M)" << endl;

			// get the two virtual nodes of a virtual link
			vector<string> fromTo = CommonFunctions::stringSplit(vlIter->first,
					'_');
			string from = fromTo.at(0);
			string to = fromTo.at(1);

			// get mapped substrate nodes of two virtual nodes
			string subFrom = "*";
			string subTo = "*";

			//check the two terminal nodes of virtual link
			map<string, string>::iterator mIter;
			mIter = vnwIter->second->getMappedPhyNodes().find(from);
			if (mIter != vnwIter->second->getMappedPhyNodes().end()) {
				subFrom = mIter->second;
			} else {
				MessageSet::printErrorMessage(
						" Virtual node: " + from
								+ "'s mapped substrate node is no existence in physical network.");
				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : embeddingVirtualNetwork()");
				CommonFunctions::FAILURE_EXIT();
			}

			mIter = vnwIter->second->getMappedPhyNodes().find(to);
			if (mIter != vnwIter->second->getMappedPhyNodes().end()) {
				subTo = mIter->second;
			} else {
				MessageSet::printErrorMessage(
						" Virtual node: " + to
								+ "'s mapped substrate node is no existence in physical network.");
				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
				CommonFunctions::FAILURE_EXIT();
			}

			if (subFrom == "*" || subTo == "*") {
				MessageSet::printErrorMessage(
						" Virtual link: " + vlIter->first
								+ " two terminals has no mapped substrate nodes in physical network.");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
				CommonFunctions::FAILURE_EXIT();
			}

			/*
			 * check bandwidth and confirm substrate transport path for a virtual link
			 * check from maximum hops for fairness bandwidth distribution
			 */

			// calculate shortest substrate transport paths for a virtual link for M:1
			vector<BasePath*> shortestPaths;
			if (pathsType
					== CommonConstants::DISJOINT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK) {
				shortestPaths =
						getTwoDisjointPathsWithComplexConstraintsOnSubstrateNetwork(
								embedCheckSubNW, subFrom, subTo,
								vlIter->second->getVirLinkBandwidth());
			} else {
				shortestPaths =
						getTwoDifferentPathsWithComplexConstraintsOnSubstrateNetwork(
								embedCheckSubNW, subFrom, subTo,
								vlIter->second->getVirLinkBandwidth());
			}

			if (shortestPaths.size() == 0) {
				MessageSet::printInfoMessage(
						" Virtual link: " + vlIter->first
								+ " has no substrate transport path existed in physical network. Try to repeat embedding  and checking once again.");
				checkFlag = false;
				break;
			}

			for (vector<BasePath*>::iterator pIter = shortestPaths.begin();
					pIter != shortestPaths.end(); ++pIter) {

				multimap<string, BasePath*>::iterator tIter =
						transportPath.begin();
				transportPath.insert(tIter,
						pair<string, BasePath*>(vlIter->first, (*pIter)));

				MessageSet::printInfoMessage(
						" Virtual link : " + vlIter->first + " 's "
								+ " One Substrate transport path: "
								+ (*pIter)->getPathId() + " is checked.");

				//substrate paths record
				BasePath* reDirection = new BasePath();

				reDirection->setPathSource((*pIter)->getPathDestination());
				reDirection->setPathDestination((*pIter)->getPathSource());
				reDirection->setPathId(
						CommonFunctions::makePathId(
								reDirection->getPathSource(),
								reDirection->getPathDestination()));

				reDirection->setPathDeley((*pIter)->getPathDeley());
				reDirection->setPathCost((*pIter)->getPathCost());
				reDirection->setPathDistance((*pIter)->getPathDistance());
				reDirection->setPathHops((*pIter)->getPathHops());

				vector<string> pInfo = (*pIter)->getPathInfo();

				reverse(pInfo.begin(), pInfo.end());
				reDirection->setPathInfo(pInfo);
				reDirection->setPathSwitchingEnergy(
						(*pIter)->getPathSwitchingEnergy());

				reDirection->setPathBandwidth((*pIter)->getPathBandwidth());
				reDirection->setPathAvailableBandwidth(
						(*pIter)->getPathAvailableBandwidth());

				tIter = transportPath.begin();
				transportPath.insert(tIter,
						pair<string, BasePath*>(
								CommonFunctions::makeLinkId(to, from),
								reDirection));

				MessageSet::printInfoMessage(
						" Virtual link : "
								+ CommonFunctions::makeLinkId(to, from) + "'s "
								+ " One Substrate transport path: "
								+ reDirection->getPathId() + " is checked.");

				cout << " Virtual link :" << vlIter->first
						<< "'s substrate path : " << endl;
				CommonFunctions::printVector((*pIter)->getPathInfo());

				cout << " Virtual link :"
						<< CommonFunctions::makeLinkId(to, from)
						<< "'s substrate path : " << endl;
				CommonFunctions::printVector(reDirection->getPathInfo());

			} // EndFor

			if (transportPath.count(vlIter->first) == shortestPaths.size()
					&& transportPath.count(
							CommonFunctions::makeLinkId(to, from))
							== shortestPaths.size()) {
				//one virtual link check over
				CalaculatedVirLinks.push_back(vlIter->first);
				CalaculatedVirLinks.push_back(
						CommonFunctions::makeLinkId(to, from));
			} else {
				MessageSet::printErrorMessage(
						" for virtual link: " + vlIter->first + "( "
								+ CommonFunctions::CoverDoubleToString(
										vlIter->second->getVirLinkBandwidth())
								+ "M, substrate path amount "
								+ CommonFunctions::CoverIntegerToString(
										shortestPaths.size())
								+ ") only allocate "
								+ CommonFunctions::CoverIntegerToString(
										transportPath.count(vlIter->first))
								+ " substrate paths ");

				MessageSet::printFailureExitMessage(
						" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
				CommonFunctions::FAILURE_EXIT();
			}

		} // a virtual network's all virtual links embedding: substrate transport paths check end

		if (checkFlag == true) {

			// re-embedding virtual links

			for (map<string, VirNwLink*>::iterator vlIter =
					vnwIter->second->getVirLinks().begin();
					vlIter != vnwIter->second->getVirLinks().end(); ++vlIter) {

				pair<multimap<string, BasePath*>::iterator,
						multimap<string, BasePath*>::iterator> ret;

				ret = transportPath.equal_range(vlIter->first);

				// embedding virtual link
				for (multimap<string, BasePath*>::iterator tranPathIter =
						ret.first; tranPathIter != ret.second; ++tranPathIter) {

					if (tranPathIter->second != NULL) {

						/*
						 * 1. configure the substrate transport path
						 * 2. configure the virtual link
						 */

						double switchingEnergyTransportPath = 0.0;
						double delayTransportPath = 0.0;
						double length = 0.0;

						for (unsigned int i = 1;
								i < tranPathIter->second->getPathInfo().size();
								++i) {

							string subLink_FromTo = CommonFunctions::makeLinkId(
									tranPathIter->second->getPathInfo().at(
											i - 1),
									tranPathIter->second->getPathInfo().at(i));

							cout << " ******:**** " << endl;
							cout << " virtual link: " << vlIter->first << endl;
							// check physical transport path bandwidth whether is enough
							cout << " subLink : " << subLink_FromTo
									<< " 's embeddable bandwidth "
									<< this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
									<< endl;

							cout << " subPath : "
									<< tranPathIter->second->getPathId()
									<< " 's bandwidth : "
									<< tranPathIter->second->getPathBandwidth()
									<< endl;

							if (this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
									>= tranPathIter->second->getPathBandwidth()) {

								double embedBandwidth =
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth();

								double updateBandwidth =
										tranPathIter->second->getPathBandwidth();

								MessageSet::printInfoMessage(
										" Update substrate link: "
												+ subLink_FromTo
												+ "'s embeddable bandwidth from "
												+ CommonFunctions::CoverDoubleToString(
														embedBandwidth)
												+ " reduce  "
												+ CommonFunctions::CoverDoubleToString(
														updateBandwidth));

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
										embedBandwidth - updateBandwidth);

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkMappedVirLink(
										vnwIter->second->getVirNwNo(),
										vlIter->first);

								switchingEnergyTransportPath =
										switchingEnergyTransportPath
												+ this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkSwitchingEnergy();

								delayTransportPath =
										delayTransportPath
												+ this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkDelay();

								length =
										length
												+ this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkWeight();

								MessageSet::printInfoMessage(
										" Updated substrate link: "
												+ subLink_FromTo
												+ "'s embeddable bandwidth is "
												+ CommonFunctions::CoverDoubleToString(
														this->subNW->getSubLink(
																subLink_FromTo)->getSubLinkEmbeddableBandwidth()));

							} else {
								MessageSet::printErrorMessage(
										" Virtual link: " + vlIter->first
												+ "'s mapped substrate transport path is not enough bandwidth for embedding. Check process bug.");

								MessageSet::printFailureExitMessage(
										" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio() ");
								CommonFunctions::FAILURE_EXIT();
							}

						} //EndFor

						tranPathIter->second->setPathHops(
								tranPathIter->second->getPathInfo().size() - 1);
						tranPathIter->second->setPathDeley(delayTransportPath);
						tranPathIter->second->setPathSwitchingEnergy(
								switchingEnergyTransportPath);
						tranPathIter->second->setPathDistance(length);

						tranPathIter->second->setPathId(
								CommonFunctions::makePathId(
										tranPathIter->second->getPathId(),
										CommonFunctions::CoverIntegerToString(
												vlIter->second->getVirLinkSubPaths().size())));

						vlIter->second->addVirLinkSubPath(tranPathIter->second);

					} else {
						MessageSet::printErrorMessage(
								" Virtual link: " + vlIter->first
										+ " has no substrate transport path in this step, it is bug. ");

						MessageSet::printFailureExitMessage(
								" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio(). ");
						CommonFunctions::FAILURE_EXIT();
					} //EndIF

				} // EndFor embedding a virtual link

			} // EndFor all virtual links

		} else {
			MessageSet::printErrorMessage(
					" virtual links' resource cannot be reallocated. currently, in this case, program exits. ");

			MessageSet::printFailureExitMessage(
					" Virtual Network Embedding : reallocationVirtualLinksResourceBySplittingRatio()");
			// cuurently, the process exit;
			CommonFunctions::FAILURE_EXIT();
		} // EndIf

		delete embedCheckSubNW;
		VirNwGraph::printVirtualNetworkInfo(vnwIter->second);
	}
// all virtual networks' virtual link resource reallocation End.

	MessageSet::printInfoMessage(
			"Virtual Network Embedding::reallocationVirtualLinksResourceBySplittingRatio End.");
}

const string& VirtualNetworkEmbedding::getSubNwGraphFile() const {
	return subNwGraphFile;
}

void VirtualNetworkEmbedding::setSubNwGraphFile(const string& subNwGraphFile) {
	this->subNwGraphFile = subNwGraphFile;
}

/*
 * generate end user request for live virtual network at current time
 *  input parameter:  current timing
 *  output         :  void, the generated end user requests recorded into endUserRequests
 */
map<string, EndUserRequest*> VirtualNetworkEmbedding::generateEndUserVideoRequestsForVirtualNetwork(
		const int timing, const int requestAmount) {

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::generateEndUserVideoRequestsForVirtualNetwork Start.");
	/*
	 * get end user requests for current timing, firstly clear existing records
	 */
	map<string, EndUserRequest*> endUserRequests;

	for (map<int, VirNwGraph*>::iterator vmIter = this->vNws.begin();
			vmIter != this->vNws.end(); ++vmIter) {
		/*
		 * virtual network duration time check whether living now.
		 */
		int virNwEndTiming = vmIter->second->getVirNwEndTiming();

		if (virNwEndTiming <= timing) {
			continue;
		}

		//int amount = 1;

		for (int index = 0; index < requestAmount; ++index) {
			EndUserRequest *euRequest = new EndUserRequest();
			euRequest->setReqStartTiming(timing);

			int duration =
					RandomGenerator::getUserVideoRequestDurationTimeWithExponentialDistribution();
			int endTiming = euRequest->getReqStartTiming() + duration;

			// the duration time whether extend the end timing of virtual network
			if (endTiming >= virNwEndTiming) {
				euRequest->setReqDurationTimeUnits(virNwEndTiming - timing);
				euRequest->setReqEndTiming(virNwEndTiming);

			} else {
				euRequest->setReqDurationTimeUnits(duration);
				euRequest->setReqEndTiming(endTiming);
			}

			euRequest->setReqVirNwNo(vmIter->second->getVirNwNo());

			euRequest->setReqId(
					CommonFunctions::makeEndUserRequestId(
							euRequest->getReqVirNwNo(),
							CommonFunctions::CoverIntegerToString(timing),
							CommonFunctions::CoverIntegerToString(index)));
			// datarate unit is  Mbps, generated data rate unit is kbps
			euRequest->setReqDataRate(
					RandomGenerator::getUserVideoRequestDataRateWithPoissonDistribution()
							/ 1024.0);

			//cout << " amount : " << vmIter->second->getNodesAmount() << endl;

			vector<string> nodePair =
					RandomGenerator::getUserVideoRequestSourceAndDestinationWithUniformDistribution(
							2, 0, vmIter->second->getVirNodes().size() - 1);

			//cout << "source : " << nodePair[0] << endl;
			//cout << "destination : " << nodePair[1] << endl;

			euRequest->setReqSourceVirNode(nodePair[0]);
			euRequest->setReqDestinationVirNode(nodePair[1]);

			// add record list
			multimap<int, EndUserRequest*>::iterator rIter =
					this->endUserRequestRecords.begin();
			this->endUserRequestRecords.insert(rIter,
					pair<int, EndUserRequest*>(euRequest->getReqEndTiming(),
							euRequest));

			// add current request list
			map<string, EndUserRequest*>::iterator cIter =
					endUserRequests.begin();
			endUserRequests.insert(cIter,
					pair<string, EndUserRequest*>(euRequest->getReqId(),
							euRequest));
		}
	}

	cout << " endUserRequests amount : " << endUserRequests.size() << endl;
	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::generateEndUserVideoRequestsForVirtualNetwork End.");
	return endUserRequests;
}

void VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
		const int timing, map<string, EndUserRequest*> euRequests,
		const string splitFlag) {

	MessageSet::printInfoMessage(
			" VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio Start ");

	// release end user request resource from virtual network and substrate network
	if (this->endUserRequestRecords.size() > 0) {
		cout << " At timing: " << timing
				<< ", these end-user requests finish and release virtual and substrate networks resource: "
				<< endl;

		pair<multimap<int, EndUserRequest*>::iterator,
				multimap<int, EndUserRequest*>::iterator> endReqIter;

		endReqIter = this->endUserRequestRecords.equal_range(timing);

		for (multimap<int, EndUserRequest*>::iterator reqIter = endReqIter.first;
				reqIter != endReqIter.second; ++reqIter) {

			cout << "request Id: " << reqIter->second->getReqId()
					<< " requested VirNw: " << reqIter->second->getReqVirNwNo()
					<< " data rate:" << reqIter->second->getReqDataRate()
					<< " start timing: " << reqIter->second->getReqStartTiming()
					<< " duration time: "
					<< reqIter->second->getReqDurationTimeUnits()
					<< " end timing: " << reqIter->second->getReqEndTiming()
					<< " virtual path links amount : "
					<< reqIter->second->getReqVirPathLinks().size()
					<< " substrate path links amount : "
					<< reqIter->second->getReqSubPathsLinks().size() << endl;

			for (map<int, VirNwGraph*>::iterator vNwIter = this->vNws.begin();
					vNwIter != this->vNws.end(); ++vNwIter) {

				if (vNwIter->second->getVirNwNo()
						== reqIter->second->getReqVirNwNo()) {

					cout << " Virtual network ("
							<< vNwIter->second->getVirNwNo() << ")'s request:"
							<< reqIter->second->getReqId() << " VM: "
							<< reqIter->second->getReqVirNwNo()
							<< " virtual links : "
							<< reqIter->second->getReqVirPathLinks().size()
							<< endl;

					//release substrate links resource
					for (unsigned int index = 0;
							index
									< reqIter->second->getReqSubPathsLinks().size();
							++index) {

						string subLinkId =
								reqIter->second->getReqSubPathsLinks()[index].first;
						double subDatarate =
								reqIter->second->getReqSubPathsLinks()[index].second;

						cout << " substrate network 's substrate link ("
								<< subLinkId << "("
								<< this->subNW->getSubLink(subLinkId)->getSubLinkBandwidth()
								<< " Mb)" << ")'s available bandwidth ("
								<< this->subNW->getSubLink(subLinkId)->getSubLinkAvailableBandwidth()
								<< " Mb) will add bandwidth ("
								<< reqIter->second->getReqSubPathsLinks()[index].second
								<< "Mb) recorded allocated bandwidth ("
								<< subDatarate << " Mb)"
								<< " embeddeable bandwidth ( "
								<< this->subNW->getSubLink(subLinkId)->getSubLinkEmbeddableBandwidth()
								<< " (M)" << endl;

						this->subNW->getSubLink(subLinkId)->setSubLinkAvailableBandwidth(
								this->subNW->getSubLink(subLinkId)->getSubLinkAvailableBandwidth()
										+ subDatarate);

						cout << " substrate network 's substrate link ("
								<< subLinkId << "("
								<< this->subNW->getSubLink(subLinkId)->getSubLinkBandwidth()
								<< " Mb)" << ")'s available bandwidth ("
								<< this->subNW->getSubLink(subLinkId)->getSubLinkAvailableBandwidth()
								<< " Mb)" << " embeddeable bandwidth ( "
								<< this->subNW->getSubLink(subLinkId)->getSubLinkEmbeddableBandwidth()
								<< "(M)" << endl;

					}

					//release virtual links resource
					for (unsigned int index = 0;
							index < reqIter->second->getReqVirPathLinks().size();
							++index) {

						string virLinkId =
								reqIter->second->getReqVirPathLinks()[index].first;

						double virDatarate =
								reqIter->second->getReqVirPathLinks()[index].second;

						cout << " virtual network (" << vNwIter->first
								<< ")'s virtual link (" << virLinkId << "( "
								<< vNwIter->second->getVirLink(virLinkId)->getVirLinkBandwidth()
								<< " Mb)" << " )'s available bandwidth ( "
								<< vNwIter->second->getVirLink(virLinkId)->getVirLinkAvailableBandwidth()
								<< " Mb) will add bandwidth ( "
								<< reqIter->second->getReqDataRate()
								<< " Mb) recorded allocated bandwidth ( "
								<< virDatarate << " Mb)" << endl;

						vNwIter->second->getVirLink(virLinkId)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink(virLinkId)->getVirLinkAvailableBandwidth()
										+ virDatarate);

						cout << " virtual network (" << vNwIter->first
								<< ")'s virtual link (" << virLinkId << "("
								<< vNwIter->second->getVirLink(virLinkId)->getVirLinkBandwidth()
								<< " Mb)'s available bandwidth ("
								<< vNwIter->second->getVirLink(virLinkId)->getVirLinkAvailableBandwidth()
								<< " Mb)." << endl;

						// release substrate transport path resource of virtual link when releasing a end user request
						for (vector<pair<string, double>>::iterator rIter =
								reqIter->second->getReqSubPathInfo().begin();
								rIter
										!= reqIter->second->getReqSubPathInfo().end();
								++rIter) {

							for (vector<BasePath*>::iterator bpIter =
									vNwIter->second->getVirLink(virLinkId)->getVirLinkSubPaths().begin();
									bpIter
											!= vNwIter->second->getVirLink(
													virLinkId)->getVirLinkSubPaths().end();
									++bpIter) {
								cout << " virtual network (" << vNwIter->first
										<< ")'s virtual link (" << virLinkId
										<< "( "
										<< vNwIter->second->getVirLink(
												virLinkId)->getVirLinkBandwidth()
										<< " Mb)"
										<< " )'s available bandwidth ( "
										<< vNwIter->second->getVirLink(
												virLinkId)->getVirLinkAvailableBandwidth()
										<< " Mb)'s substrate transport path "
										<< (*rIter).first
										<< " will add bandwidth ( "
										<< reqIter->second->getReqDataRate()
										<< " Mb) recorded allocated bandwidth ( "
										<< (*rIter).second << " Mb)" << endl;

								if ((*bpIter)->getPathId() == (*rIter).first) {
									(*bpIter)->setPathAvailableBandwidth(
											(*bpIter)->getPathAvailableBandwidth()
													+ (*rIter).second);
								}
							}
						}

					}

				}
			}

		}
	}

	cout << " function step 0 ************ start " << endl;
	//SubNwGraph::printSubstrateNetwork(this->subNW);
	SubNwGraph::checkSubstrateLinksResource(this->subNW);

	cout << " function step 0 ************ end " << endl;

	//release virtual network resource based on the ending timing of virtual network
	// delete virtual network pointer;
	MessageSet::printInfoMessage(
			" Current, there are "
					+ CommonFunctions::CoverIntegerToString(this->vNws.size())
					+ " Virtual Networks embedded on Substrate network. Before release processing.");

	for (map<int, VirNwGraph*>::iterator vnwIter = this->vNws.begin();
			vnwIter != this->vNws.end();) {
		if (vnwIter->second->getVirNwEndTiming() == timing) {

			MessageSet::printInfoMessage(
					" Virtual network (" + vnwIter->second->getVirNwNo()
							+ ") will be release.");

			for (map<string, VirNwLink*>::iterator vlIter =
					vnwIter->second->getVirLinks().begin();
					vlIter != vnwIter->second->getVirLinks().end(); ++vlIter) {

				VirNwLink::printVirNwLinkInfo(vlIter->second);

				cout << " before substrate path amount : "
						<< vlIter->second->getVirLinkSubPaths().size() << endl;
				// one virtual link may be mapped into some substrate paths
				int index = 0;
				for (vector<BasePath*>::iterator bpIter =
						vlIter->second->getVirLinkSubPaths().begin();
						bpIter != vlIter->second->getVirLinkSubPaths().end();
						++bpIter) {

					cout << ++index << endl;
					cout << " *****" << endl;
					double bandwidth = (*bpIter)->getPathBandwidth();
					cout << " path ID:  " << (*bpIter)->getPathId() << endl;

					//release each substrate link resource
					for (unsigned int i = 1;
							i < (*bpIter)->getPathInfo().size(); ++i) {
						cout << " index: " << i << " substrate path: "
								<< (*bpIter)->getPathId() << endl;
						string subLink_FromTo = CommonFunctions::makeLinkId(
								(*bpIter)->getPathInfo().at(i - 1),
								(*bpIter)->getPathInfo().at(i));

						SubNwLink::printSubLinkInfo(
								this->subNW->getSubLink(subLink_FromTo));

						this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
								this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										+ bandwidth);

						SubNwLink::printSubLinkInfo(
								this->subNW->getSubLink(subLink_FromTo));

					}

					// delete virtual link mapped substrate link pointer
					MessageSet::printInfoMessage(
							" Virtual link(" + vlIter->second->getVirLinkId()
									+ ")'s substrate path ("
									+ (*bpIter)->getPathId() + ", "
									+ CommonFunctions::CoverDoubleToString(
											(*bpIter)->getPathBandwidth())
									+ " M) will be released. "
									+ CommonFunctions::CoverDoubleToString(
											(*bpIter)->getPathAvailableBandwidth())
									+ "(M)");
					delete (*bpIter);
					cout << "----" << endl;
				}

				vlIter->second->getVirLinkSubPaths().clear();
				cout << " After substrate path amount : "
						<< vlIter->second->getVirLinkSubPaths().size() << endl;
				// delete virtual node pointer
				MessageSet::printInfoMessage(
						" Virtual link (" + vlIter->second->getVirLinkId()
								+ ") will be released. ");
				delete vlIter->second;
			}
			vnwIter->second->getVirLinks().clear();

			//release substrate node resource and virNode memory and delete pointer
			for (map<string, VirNwNode*>::iterator vnIter =
					vnwIter->second->getVirNodes().begin();
					vnIter != vnwIter->second->getVirNodes().end(); ++vnIter) {

				VirNwNode::printVirNodeInfo(vnIter->second);

				string subNodeId = vnIter->second->getVirNodeMappedSubNodeId();
				double capapbility = vnIter->second->getVirNodeCapability();

				if (this->subNW->getSubNode(subNodeId) == NULL) {
					MessageSet::printErrorMessage(
							" Substrate Node: " + subNodeId
									+ " is no existence in substrate network.");
					CommonFunctions::FAILURE_EXIT();
				}

				SubNwNode::printSubNodeInfo(this->subNW->getSubNode(subNodeId));

				this->subNW->getSubNode(subNodeId)->setSubNodeAvaiableCapability(
						this->subNW->getSubNode(subNodeId)->getSubNodeAvaiableCapability()
								+ capapbility);

				this->subNW->getSubNode(subNodeId)->setSubNodeEmbeddedAmount(
						this->subNW->getSubNode(subNodeId)->getSubNodeEmbeddedAmount()
								- 1);

				SubNwNode::printSubNodeInfo(this->subNW->getSubNode(subNodeId));

				// delete virtual node pointer
				MessageSet::printInfoMessage(
						" Virtual Node (" + vnIter->second->getVirNodeId()
								+ ") will be released. ");
				delete vnIter->second;
			}
			vnwIter->second->getVirNodes().clear();

			MessageSet::printInfoMessage(
					" Virtual Network (" + vnwIter->second->getVirNwNo()
							+ ") has been released. ");
			VirNwGraph::printVirtualNetworkInfo(vnwIter->second);
			delete vnwIter->second;
			this->vNws.erase(vnwIter);

			cout << " the virtual networks amount : " << this->vNws.size()
					<< endl;
			vnwIter = this->vNws.begin();
			continue;
		}
		++vnwIter;
	}

	MessageSet::printInfoMessage(
			" Current, there are "
					+ CommonFunctions::CoverIntegerToString(this->vNws.size())
					+ " Virtual Networks embedded on Substrate network. After release processing.");

	cout << " function step 1 ************ start " << endl;
	//SubNwGraph::printSubstrateNetwork(this->subNW);
	SubNwGraph::checkSubstrateLinksResource(this->subNW);

	cout << " function step 1 ************ end " << endl;

	// for proposal reallocation virtual link substrate resource
	if (splitFlag
			== CommonConstants::NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK) {
		// loop each virtual network's virtual link
		for (map<int, VirNwGraph*>::iterator vnwIter = this->vNws.begin();
				vnwIter != this->vNws.end(); ++vnwIter) {
			/*
			 * check the virtual link's available bandwidth status
			 */
			vector<string> CalaculatedVirLinks;
			for (map<string, VirNwLink*>::iterator vlIter =
					vnwIter->second->getVirLinks().begin();
					vlIter != vnwIter->second->getVirLinks().end(); ++vlIter) {

				// for repeat calacualte
				if (find(CalaculatedVirLinks.begin(), CalaculatedVirLinks.end(),
						vlIter->first) != CalaculatedVirLinks.end()) {
					continue;
				}

				/*
				 * before configure and update virtual link[a,b]'s substrate transport paths,
				 * we first get the pair virtual link [b,a], and update and configure its substrate transport paths, at the same time.
				 */
				string virLinkToFrom_Id = CommonFunctions::makeLinkId(
						vlIter->second->getVirLinkTo(),
						vlIter->second->getVirLinkFrom());

				/*
				 * Currently, the amount of substrate transport path of virtual link is one or two,
				 * except cases are wrong that is bug.
				 */
				if (vlIter->second->getVirLinkSubPaths().size() == 0
						|| vlIter->second->getVirLinkSubPaths().size() > 2) {

					MessageSet::printErrorMessage(
							" Virtual link: " + vlIter->second->getVirLinkId()
									+ " has not mapped into substrate transport path, or mapped substrate transport paths are more than Two. Please check this bug. ");
					CommonFunctions::FAILURE_EXIT();
				}

				/*
				 *  if only one substrate path for the virtual link, then to reallocate a new substrate path
				 *   and update existing substrate path information
				 */
				if (1 == vlIter->second->getVirLinkSubPaths().size()
						&& vlIter->second->getVirLinkAvailableBandwidth()
								> 0.0) {

					/*   if  available more than a constant value, reallocation
					 *   else  do nothing
					 */
					double virlinkAvailableBw =
							vlIter->second->getVirLinkAvailableBandwidth();

					if (virlinkAvailableBw > 0.0
							&& virlinkAvailableBw
									< vlIter->second->getVirLinkBandwidth()
											* (1
													- CommonConstants::AVAILABLE_BANDWIDTH_THRESHOLD_VALUE_FOR_SPLIT_SUBSTRATE_PATH_OF_VIRTUAL_LINK)) {

						cout << " Virtual link: "
								<< vlIter->second->getVirLinkId()
								<< " has mapped one substrate transport path. ready to reallocate another substrate path with "
								<< virlinkAvailableBw << " (M) for it." << endl;

						vector<BasePath*>::iterator bIter =
								vlIter->second->getVirLinkSubPaths().begin();

						if ((*bIter)->getPathAvailableBandwidth()
								!= virlinkAvailableBw) {
							MessageSet::printErrorMessage(
									" Virtual link: "
											+ vlIter->second->getVirLinkId()
											+ " 's available bandwidth is not equal to its substrate path available bandwidth. Please check this bug. ");
							CommonFunctions::FAILURE_EXIT();
						}

						cout << " Existing substrate path information: "
								<< endl;
						BasePath::printBasePath((*bIter));
						cout << " ****** " << endl;

						double bpSwithingEnergy =
								(*bIter)->getPathSwitchingEnergy();

						/*
						 *  calucuate a new substrate path for the virtual link
						 */
						BasePath* anotherBPath =
								getOneDisjointPathWithComplexConstraintsOnSubstrateNetwork(
										this->subNW, (*bIter),
										virlinkAvailableBw, bpSwithingEnergy);

						if (anotherBPath != NULL) {

							vector<BasePath*>::iterator toFromBIter =
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths().begin();

							if ((*toFromBIter)->getPathAvailableBandwidth()
									!= virlinkAvailableBw) {
								MessageSet::printErrorMessage(
										" Virtual link[a,b]'s pair virtual link[b,a]: "
												+ vnwIter->second->getVirLink(
														virLinkToFrom_Id)->getVirLinkId()
												+ " 's available bandwidth is not equal to its substrate path available bandwidth. Please check this bug. ");
								CommonFunctions::FAILURE_EXIT();
							}

							BasePath* anotherToFromBPath = new BasePath();

							vector<string> items = CommonFunctions::stringSplit(
									(*toFromBIter)->getPathId(), '_');
							cout << " Split data path "
									<< (*toFromBIter)->getPathId() << endl;
							CommonFunctions::printVector(items);
							if (items[2] == "1") {
								anotherToFromBPath->setPathId(
										CommonFunctions::makePathId(
												CommonFunctions::makePathId(
														anotherBPath->getPathDestination(),
														anotherBPath->getPathSource()),
												"0"));
							} else {
								anotherToFromBPath->setPathId(
										CommonFunctions::makePathId(
												CommonFunctions::makePathId(
														anotherBPath->getPathDestination(),
														anotherBPath->getPathSource()),
												"1"));
							}

							items.clear();

							items = CommonFunctions::stringSplit(
									(*bIter)->getPathId(), '_');

							cout << " Split data path " << (*bIter)->getPathId()
									<< endl;

							CommonFunctions::printVector(items);
							if (items[2] == "1") {
								anotherBPath->setPathId(
										CommonFunctions::makePathId(
												anotherBPath->getPathId(),
												"0"));
							} else {
								anotherBPath->setPathId(
										CommonFunctions::makePathId(
												anotherBPath->getPathId(),
												"1"));
							}

							anotherToFromBPath->setPathAvailableBandwidth(
									anotherBPath->getPathAvailableBandwidth());
							anotherToFromBPath->setPathBandwidth(
									anotherBPath->getPathBandwidth());
							anotherToFromBPath->setPathCost(
									anotherBPath->getPathCost());
							anotherToFromBPath->setPathDeley(
									anotherBPath->getPathDeley());
							anotherToFromBPath->setPathDestination(
									anotherBPath->getPathSource());
							anotherToFromBPath->setPathDistance(
									anotherBPath->getPathDistance());
							anotherToFromBPath->setPathHops(
									anotherBPath->getPathHops());

							vector<string> pathInfo =
									anotherBPath->getPathInfo();
							reverse(pathInfo.begin(), pathInfo.end());
							anotherToFromBPath->setPathInfo(pathInfo);

							anotherToFromBPath->setPathSource(
									anotherBPath->getPathDestination());
							anotherToFromBPath->setPathSwitchingEnergy(
									anotherBPath->getPathSwitchingEnergy());

							cout << " virtual link:" << virLinkToFrom_Id
									<< "'s new substrate transport path ID:"
									<< anotherToFromBPath->getPathId() << endl;

							cout << " virtual link:"
									<< vlIter->second->getVirLinkId()
									<< "'s new substrate transport path ID:"
									<< anotherBPath->getPathId() << endl;

							cout << " New allocate substrate path information: "
									<< endl;
							BasePath::printBasePath(anotherBPath);
							cout << " ****** " << endl;

							// configue another path
							// update substrate network resource
							for (unsigned int i = 1;
									i < anotherBPath->getPathInfo().size();
									++i) {
								// for virtual ink [a,b]' substrate transport path
								string subLink_FromTo =
										CommonFunctions::makeLinkId(
												anotherBPath->getPathInfo().at(
														i - 1),
												anotherBPath->getPathInfo().at(
														i));

								// for virtual link [b,a]' substrate transport path
								string subLink_ToFrom =
										CommonFunctions::makeLinkId(
												anotherBPath->getPathInfo().at(
														i),
												anotherBPath->getPathInfo().at(
														i - 1));

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
												- virlinkAvailableBw);

								this->subNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
												- virlinkAvailableBw);
							}

							// update virtual link [a,b]'s existing substrate transport path
							(*bIter)->setPathBandwidth(
									(*bIter)->getPathBandwidth()
											- virlinkAvailableBw);
							(*bIter)->setPathAvailableBandwidth(0.0);

							// update virtual link [b,a]'s existing substrate transport path
							(*toFromBIter)->setPathBandwidth(
									(*toFromBIter)->getPathBandwidth()
											- virlinkAvailableBw);
							(*toFromBIter)->setPathAvailableBandwidth(0.0);

							cout
									<< " updated Existing substrate path information: "
									<< endl;
							BasePath::printBasePath((*bIter));
							cout << " ****** " << endl;

							for (unsigned int i = 1;
									i < (*bIter)->getPathInfo().size(); ++i) {

								string subLink_FromTo =
										CommonFunctions::makeLinkId(
												(*bIter)->getPathInfo().at(
														i - 1),
												(*bIter)->getPathInfo().at(i));

								string subLink_ToFrom =
										CommonFunctions::makeLinkId(
												(*bIter)->getPathInfo().at(i),
												(*bIter)->getPathInfo().at(
														i - 1));

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), avaliable Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M) will add embeddable bandwidth: "
										<< virlinkAvailableBw << " M " << endl;

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
												+ virlinkAvailableBw);

								this->subNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
												+ virlinkAvailableBw);
							}

							vlIter->second->addVirLinkSubPath(anotherBPath);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->addVirLinkSubPath(
									anotherToFromBPath);

						} else {
							MessageSet::printInfoMessage(
									" Currently, no enough substrate resource for reallocation a new substrate transport path for virtual link");
						}

					}

				} else if (2 == vlIter->second->getVirLinkSubPaths().size()
						&& vlIter->second->getVirLinkAvailableBandwidth()
								> 0.0) {
					/*
					 * if two substrate path for the virtual link, then to consider magerate available bandwidth to saving substrate path
					 *  and update existing substrate path information
					 */

					cout << " Virtual link: " << vlIter->second->getVirLinkId()
							<< " " << vlIter->second->getVirLinkBandwidth()
							<< " M"
							<< " has mapped two substrate transport path. ready to adjust two substrate path's available resource "
							<< vlIter->second->getVirLinkAvailableBandwidth()
							<< " M" << endl;

					cout << "--Path--- subPath0---start--" << endl;
					BasePath::printBasePath(
							vlIter->second->getVirLinkSubPaths()[0]);
					cout << "--Path--- subPath0---end--" << endl;

					cout << "--Path--- subPath1---start--" << endl;
					BasePath::printBasePath(
							vlIter->second->getVirLinkSubPaths()[1]);
					cout << "--Path--- subPath1---end--" << endl;

					int pathNo = 0;
					BasePath* savingEnergyBPath =
							vlIter->second->getVirLinkSubPaths()[0];

					double bpAvailabeBw =
							vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth();

					cout << "step 1" << endl;
					if (vlIter->second->getVirLinkSubPaths()[0]->getPathSwitchingEnergy()
							> vlIter->second->getVirLinkSubPaths()[1]->getPathSwitchingEnergy()) {
						savingEnergyBPath =
								vlIter->second->getVirLinkSubPaths()[1];
						bpAvailabeBw =
								vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth();
						pathNo = 1;
					}

					if (bpAvailabeBw == 0.0) {
						MessageSet::printInfoMessage(
								" another substrate transport path's substrate links have no available bandwidth to reallocate for this substrate path. ");
						CalaculatedVirLinks.push_back(
								vlIter->second->getVirLinkId());
						CalaculatedVirLinks.push_back(virLinkToFrom_Id);

						continue;
					}
					cout << "step 2" << endl;

					bool magratableFlag = true;

					// check substrate links's embedding available resource for megrate resource
					for (unsigned int i = 1;
							i < savingEnergyBPath->getPathInfo().size(); ++i) {

						string subLink_FromTo = CommonFunctions::makeLinkId(
								savingEnergyBPath->getPathInfo().at(i - 1),
								savingEnergyBPath->getPathInfo().at(i));

						if (this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
								< bpAvailabeBw) {
							magratableFlag = false;
						}

					}

					cout << "step 3" << endl;
					if (magratableFlag == true) {

						//
						if (vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkAvailableBandwidth()
								!= vlIter->second->getVirLinkAvailableBandwidth()) {
							CommonFunctions::printFailureMessageAndExit(
									"virtual link:"
											+ vlIter->second->getVirLinkId()
											+ "'s available bandwidth is not equal to its pair virtual link:"
											+ virLinkToFrom_Id + "'s.",
									"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");

						}

						//
						if (vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkBandwidth()
								!= vlIter->second->getVirLinkBandwidth()) {
							CommonFunctions::printFailureMessageAndExit(
									"virtual link:"
											+ vlIter->second->getVirLinkId()
											+ "'s bandwidth is not equal to its pair virtual link:"
											+ virLinkToFrom_Id + "'s.",
									"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");
						}

						if (2
								!= vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths().size()) {
							CommonFunctions::printFailureMessageAndExit(
									"virtual link:"
											+ vlIter->second->getVirLinkId()
											+ "'s substrate transport path' amount is not equal to its pair virtual link:"
											+ virLinkToFrom_Id + "'s.",
									"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");
						}

						if (vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
								+ vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
								!= vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
										+ vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()) {
							CommonFunctions::printFailureMessageAndExit(
									"virtual link:"
											+ vlIter->second->getVirLinkId()
											+ "'s substrate transport paths' available bandwidth is not equal to its pair virtual link:"
											+ virLinkToFrom_Id
											+ "'s substrate transport paths's",
									"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");

						}

						if (vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[0]->getPathBandwidth()
								+ vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[1]->getPathBandwidth()
								!= vlIter->second->getVirLinkSubPaths()[0]->getPathBandwidth()
										+ vlIter->second->getVirLinkSubPaths()[1]->getPathBandwidth()) {
							CommonFunctions::printFailureMessageAndExit(
									"virtual link:"
											+ vlIter->second->getVirLinkId()
											+ "'s substrate transport paths' bandwidth is not equal to its pair virtual link:"
											+ virLinkToFrom_Id
											+ "'s substrate transport paths's",
									"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");

						}

						vector<string> pathNodes =
								vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[0]->getPathInfo();

						reverse(pathNodes.begin(), pathNodes.end());
						int pairPathFlag_0 = -1;
						int pairPathFlag_1 = -1;
						// pathNo == 0: allocate the substrate path 1's available bandwidth to substrate path 0
						if (pathNo == 0) {
							// confirm and match the pair virtual link's substrate path 1 or 2 to match
							if (pathNodes
									== vlIter->second->getVirLinkSubPaths()[0]->getPathInfo()) {
								pairPathFlag_0 = 0;
								pairPathFlag_1 = 1;
							} else {
								pathNodes =
										vnwIter->second->getVirLink(
												virLinkToFrom_Id)->getVirLinkSubPaths()[1]->getPathInfo();

								reverse(pathNodes.begin(), pathNodes.end());

								if (pathNodes
										== vlIter->second->getVirLinkSubPaths()[0]->getPathInfo()) {
									pairPathFlag_0 = 1;
									pairPathFlag_1 = 0;
								} else {

									cout
											<< "******PathNo#0--- updated subPath0---start--"
											<< endl;
									BasePath::printBasePath(
											vlIter->second->getVirLinkSubPaths()[0]);
									cout
											<< "******PathNo#0--- updated subPath0---end--"
											<< endl;

									cout
											<< "******PathNo#0--- updated subPath1---start--"
											<< endl;
									BasePath::printBasePath(
											vlIter->second->getVirLinkSubPaths()[1]);
									cout
											<< "******PathNo#0--- updated subPath1---end--"
											<< endl;

									cout
											<< "******Pair virtual link PathNo#0--- updated subPath0---start--"
											<< endl;
									BasePath::printBasePath(
											vnwIter->second->getVirLink(
													virLinkToFrom_Id)->getVirLinkSubPaths()[0]);
									cout
											<< "******Pair virtual link PathNo#0--- updated subPath0---end--"
											<< endl;

									cout
											<< "******Pair virtual link PathNo#0--- updated subPath1---start--"
											<< endl;
									BasePath::printBasePath(
											vnwIter->second->getVirLink(
													virLinkToFrom_Id)->getVirLinkSubPaths()[1]);
									cout
											<< "******Pair virtual link PathNo#0--- updated subPath1---end--"
											<< endl;

									CommonFunctions::printFailureMessageAndExit(
											"virtual link:"
													+ vlIter->second->getVirLinkId()
													+ "'s substrate transport paths can not map to its pair virtual link:"
													+ virLinkToFrom_Id
													+ "'s. substrate transport paths.",
											"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");

								}
							}

							if (pairPathFlag_0 == -1 || pairPathFlag_1 == -1) {
								CommonFunctions::printFailureMessageAndExit(
										" This is a bug for comparing virtual links[a-b][b-a]'s substrate transport paths.",
										"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");

							}

							// update existing two substrate transport path information
							vlIter->second->getVirLinkSubPaths()[0]->setPathAvailableBandwidth(
									vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
											+ bpAvailabeBw);

							vlIter->second->getVirLinkSubPaths()[0]->setPathBandwidth(
									vlIter->second->getVirLinkSubPaths()[0]->getPathBandwidth()
											+ bpAvailabeBw);

							vlIter->second->getVirLinkSubPaths()[1]->setPathAvailableBandwidth(
									vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
											- bpAvailabeBw);
							vlIter->second->getVirLinkSubPaths()[1]->setPathBandwidth(
									vlIter->second->getVirLinkSubPaths()[1]->getPathBandwidth()
											- bpAvailabeBw);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_0]->setPathAvailableBandwidth(
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_0]->getPathAvailableBandwidth()
											+ bpAvailabeBw);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_0]->setPathBandwidth(
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_0]->getPathBandwidth()
											+ bpAvailabeBw);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_1]->setPathAvailableBandwidth(
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_1]->getPathAvailableBandwidth()
											- bpAvailabeBw);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_1]->setPathBandwidth(
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_1]->getPathBandwidth()
											- bpAvailabeBw);

							cout << "step 4_0_0" << endl;
							if (vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
									> vlIter->second->getVirLinkSubPaths()[0]->getPathBandwidth()) {
								MessageSet::printErrorMessage(
										" The available bandwidth of substrate transport path of virtual link is more than its specified bandwidth. Please check this bug.");
								CommonFunctions::FAILURE_EXIT();
							}

							if (vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
									< 0.0) {
								MessageSet::printErrorMessage(
										" The available bandwidth of substrate transport path of virtual link is less than zero. Please check this bug.");
								CommonFunctions::FAILURE_EXIT();
							}

							if (vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
									+ vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
									!= vlIter->second->getVirLinkAvailableBandwidth()) {

								MessageSet::printErrorMessage(
										" The available bandwidth of two substrate transport paths of virtual link is not equal to the available bandwidth of virtual link. Please check this bug.");
								cout << " Path#0 available bandwidth:"
										<< vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
										<< endl;
								cout << " Path#1 available bandwidth: "
										<< vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
										<< endl;
								cout << " Virtual link's available bandwidth: "
										<< vlIter->second->getVirLinkAvailableBandwidth()
										<< endl;

								CommonFunctions::FAILURE_EXIT();

							}

							if (vlIter->second->getVirLinkSubPaths()[0]->getPathBandwidth()
									+ vlIter->second->getVirLinkSubPaths()[1]->getPathBandwidth()
									!= vlIter->second->getVirLinkBandwidth()) {

								MessageSet::printErrorMessage(
										" The sum of specified bandwidth of two substrate transport paths of virtual link is not equal to the specified bandwidth of virtual link. Please check this bug.");
								cout << " Path#0 specified bandwidth:"
										<< vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
										<< endl;
								cout << " Path#1 specified bandwidth: "
										<< vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
										<< endl;
								cout << " Virtual link's specified bandwidth: "
										<< vlIter->second->getVirLinkAvailableBandwidth()
										<< endl;

								CommonFunctions::FAILURE_EXIT();

							}

							cout << "--PathNo#0--- updated subPath0---start--"
									<< endl;
							BasePath::printBasePath(
									vlIter->second->getVirLinkSubPaths()[0]);
							cout << "--PathNo#0--- updated subPath0---end--"
									<< endl;

							cout << "--PathNo#0--- updated subPath1---start--"
									<< endl;
							BasePath::printBasePath(
									vlIter->second->getVirLinkSubPaths()[1]);
							cout << "--PathNo#0--- updated subPath1---end--"
									<< endl;

							cout << "step 4_0_1" << endl;
							// update substrate link resource information
							for (unsigned int i = 1;
									i
											< vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().size();
									++i) {

								string subLink_FromTo =
										CommonFunctions::makeLinkId(
												vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().at(
														i - 1),
												vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().at(
														i));

								string subLink_ToFrom =
										CommonFunctions::makeLinkId(
												vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().at(
														i),
												vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().at(
														i - 1));

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), avaliable Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M) will reduce embeddable bandwidth: "
										<< bpAvailabeBw << " M " << endl;

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
												- bpAvailabeBw);

								this->subNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
												- bpAvailabeBw);

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), available Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M)" << endl;
							}

							cout << "step 4_0_2" << endl;
							for (unsigned int i = 1;
									i
											< vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().size();
									++i) {

								string subLink_FromTo =
										CommonFunctions::makeLinkId(
												vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().at(
														i - 1),
												vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().at(
														i));

								string subLink_ToFrom =
										CommonFunctions::makeLinkId(
												vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().at(
														i),
												vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().at(
														i - 1));

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), avaliable Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M) will add embeddable bandwidth: "
										<< bpAvailabeBw << endl;

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
												+ bpAvailabeBw);

								this->subNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
												+ bpAvailabeBw);

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), available Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M)" << endl;
							}

							cout << "step 4_0_3" << endl;
							// release one substrate transport path
							if (vlIter->second->getVirLinkSubPaths()[0]->getPathBandwidth()
									== vlIter->second->getVirLinkBandwidth()
									&& vlIter->second->getVirLinkSubPaths()[1]->getPathBandwidth()
											== 0.0) {
								cout << " Virtual link:"
										<< vlIter->second->getVirLinkId()
										<< " 's one substrate transport path "
										<< vlIter->second->getVirLinkSubPaths()[1]->getPathId()
										<< "will be release because its bandwidth is zero."
										<< endl;
								delete *(vlIter->second->getVirLinkSubPaths().begin()
										+ 1);

								vlIter->second->getVirLinkSubPaths().erase(
										vlIter->second->getVirLinkSubPaths().begin()
												+ 1);

								delete *(vnwIter->second->getVirLink(
										virLinkToFrom_Id)->getVirLinkSubPaths().begin()
										+ pairPathFlag_1);
								vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths().erase(
										vnwIter->second->getVirLink(
												virLinkToFrom_Id)->getVirLinkSubPaths().begin()
												+ pairPathFlag_1);
							}

						} else {
							// pathNo == 1: allocate the substrate path 0's available bandwidth to substrate path 1

							if (pathNodes
									== vlIter->second->getVirLinkSubPaths()[1]->getPathInfo()) {
								pairPathFlag_0 = 0;
								pairPathFlag_1 = 1;
							} else {
								pathNodes =
										vnwIter->second->getVirLink(
												virLinkToFrom_Id)->getVirLinkSubPaths()[1]->getPathInfo();
								reverse(pathNodes.begin(), pathNodes.end());

								if (pathNodes
										== vlIter->second->getVirLinkSubPaths()[1]->getPathInfo()) {
									pairPathFlag_0 = 1;
									pairPathFlag_1 = 0;
								} else {

									cout
											<< "******PathNo#0--- updated subPath0---start--"
											<< endl;
									BasePath::printBasePath(
											vlIter->second->getVirLinkSubPaths()[0]);
									cout
											<< "******PathNo#0--- updated subPath0---end--"
											<< endl;

									cout
											<< "******PathNo#0--- updated subPath1---start--"
											<< endl;
									BasePath::printBasePath(
											vlIter->second->getVirLinkSubPaths()[1]);
									cout
											<< "******PathNo#0--- updated subPath1---end--"
											<< endl;

									cout
											<< "******Pair virtual link PathNo#0--- updated subPath0---start--"
											<< endl;
									BasePath::printBasePath(
											vnwIter->second->getVirLink(
													virLinkToFrom_Id)->getVirLinkSubPaths()[0]);
									cout
											<< "******Pair virtual link PathNo#0--- updated subPath0---end--"
											<< endl;

									cout
											<< "******Pair virtual link PathNo#0--- updated subPath1---start--"
											<< endl;
									BasePath::printBasePath(
											vnwIter->second->getVirLink(
													virLinkToFrom_Id)->getVirLinkSubPaths()[1]);
									cout
											<< "******Pair virtual link PathNo#0--- updated subPath1---end--"
											<< endl;

									CommonFunctions::printFailureMessageAndExit(
											"virtual link:"
													+ vlIter->second->getVirLinkId()
													+ "'s substrate transport paths can not map to its pair virtual link:"
													+ virLinkToFrom_Id
													+ "'s. substrate transport paths.",
											"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");

								}
							}

							if (pairPathFlag_0 == -1 || pairPathFlag_1 == -1) {
								CommonFunctions::printFailureMessageAndExit(
										" This is a bug for comparing virtual links[a-b][b-a]'s substrate transport paths.",
										"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio");

							}

							// update existing two substrate path information
							vlIter->second->getVirLinkSubPaths()[1]->setPathAvailableBandwidth(
									vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
											+ bpAvailabeBw);
							vlIter->second->getVirLinkSubPaths()[1]->setPathBandwidth(
									vlIter->second->getVirLinkSubPaths()[1]->getPathBandwidth()
											+ bpAvailabeBw);

							vlIter->second->getVirLinkSubPaths()[0]->setPathAvailableBandwidth(
									vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
											- bpAvailabeBw);
							vlIter->second->getVirLinkSubPaths()[0]->setPathBandwidth(
									vlIter->second->getVirLinkSubPaths()[0]->getPathBandwidth()
											- bpAvailabeBw);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_0]->setPathAvailableBandwidth(
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_0]->getPathAvailableBandwidth()
											+ bpAvailabeBw);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_0]->setPathBandwidth(
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_0]->getPathBandwidth()
											+ bpAvailabeBw);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_1]->setPathAvailableBandwidth(
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_1]->getPathAvailableBandwidth()
											- bpAvailabeBw);

							vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_1]->setPathBandwidth(
									vnwIter->second->getVirLink(
											virLinkToFrom_Id)->getVirLinkSubPaths()[pairPathFlag_1]->getPathBandwidth()
											- bpAvailabeBw);

							cout << "step 4_1_0" << endl;
							if (vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
									> vlIter->second->getVirLinkSubPaths()[1]->getPathBandwidth()) {
								MessageSet::printErrorMessage(
										" The available bandwidth of substrate transport path of virtual link is more than its specified bandwidth. Please check this bug.");
								CommonFunctions::FAILURE_EXIT();
							}

							if (vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
									< 0.0) {
								MessageSet::printErrorMessage(
										" The available bandwidth of substrate transport path of virtual link is less than zero. Please check this bug.");
								CommonFunctions::FAILURE_EXIT();
							}

							if (vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
									+ vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
									!= vlIter->second->getVirLinkAvailableBandwidth()) {
								cout << " Path#0 available bandwidth:"
										<< vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
										<< endl;
								cout << " Path#1 available bandwidth: "
										<< vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
										<< endl;
								cout << " Virtual link's available bandwidth: "
										<< vlIter->second->getVirLinkAvailableBandwidth()
										<< endl;

								MessageSet::printErrorMessage(
										" The available bandwidth of two substrate transport paths of virtual link is not equal to the available bandwidth of virtual link. Please check this bug.");
								CommonFunctions::FAILURE_EXIT();

							}

							if (vlIter->second->getVirLinkSubPaths()[1]->getPathBandwidth()
									+ vlIter->second->getVirLinkSubPaths()[0]->getPathBandwidth()
									!= vlIter->second->getVirLinkBandwidth()) {

								MessageSet::printErrorMessage(
										" The sum of specified bandwidth of two substrate transport paths of virtual link is not equal to the specified bandwidth of virtual link. Please check this bug.");
								cout << " Path#0 specified bandwidth:"
										<< vlIter->second->getVirLinkSubPaths()[1]->getPathAvailableBandwidth()
										<< endl;
								cout << " Path#1 specified bandwidth: "
										<< vlIter->second->getVirLinkSubPaths()[0]->getPathAvailableBandwidth()
										<< endl;
								cout << " Virtual link's specified bandwidth: "
										<< vlIter->second->getVirLinkAvailableBandwidth()
										<< endl;

								CommonFunctions::FAILURE_EXIT();

							}
							cout << "--PathNo#1--- updated subPath0---start--"
									<< endl;
							BasePath::printBasePath(
									vlIter->second->getVirLinkSubPaths()[0]);
							cout << "--PathNo#1--- updated subPath0---end--"
									<< endl;

							cout << "--PathNo#1--- updated subPath1---start--"
									<< endl;
							BasePath::printBasePath(
									vlIter->second->getVirLinkSubPaths()[1]);
							cout << "--PathNo#1--- updated subPath1---end--"
									<< endl;

							cout << "step 4_1_1" << endl;
							// update substrate link resource information
							for (unsigned int i = 1;
									i
											< vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().size();
									++i) {

								string subLink_FromTo =
										CommonFunctions::makeLinkId(
												vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().at(
														i - 1),
												vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().at(
														i));

								string subLink_ToFrom =
										CommonFunctions::makeLinkId(
												vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().at(
														i),
												vlIter->second->getVirLinkSubPaths()[1]->getPathInfo().at(
														i - 1));

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), avaliable Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M) will reduce embeddable bandwidth: "
										<< bpAvailabeBw << endl;

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
												- bpAvailabeBw);

								this->subNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
												- bpAvailabeBw);

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), available Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M)" << endl;
							}

							cout << "step 4_1_2" << endl;
							for (unsigned int i = 1;
									i
											< vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().size();
									++i) {

								string subLink_FromTo =
										CommonFunctions::makeLinkId(
												vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().at(
														i - 1),
												vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().at(
														i));
								string subLink_ToFrom =
										CommonFunctions::makeLinkId(
												vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().at(
														i),
												vlIter->second->getVirLinkSubPaths()[0]->getPathInfo().at(
														i - 1));

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), avaliable Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M) will add embeddable bandwidth: "
										<< bpAvailabeBw << endl;

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
												+ bpAvailabeBw);

								this->subNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
										this->subNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
												+ bpAvailabeBw);

								cout << " substrate link: " << subLink_FromTo
										<< " ( embeddable Bandwidth: "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										<< " M), available Bandwidth "
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " M)" << endl;
							}

							cout
									<< "step 4_1_3  virtual link substrate transport paths amount : "
									<< vlIter->second->getVirLinkSubPaths().size()
									<< endl;

							// release one substrate transport path
							if (vlIter->second->getVirLinkSubPaths()[1]->getPathBandwidth()
									== vlIter->second->getVirLinkBandwidth()
									&& vlIter->second->getVirLinkSubPaths()[0]->getPathBandwidth()
											== 0.0) {
								cout << "step 4_1_4" << endl;
								cout << " Virtual link:"
										<< vlIter->second->getVirLinkId()
										<< " 's one substrate transport path "
										<< vlIter->second->getVirLinkSubPaths()[1]->getPathId()
										<< "will be release because its bandwidth is zero."
										<< endl;

								delete *(vlIter->second->getVirLinkSubPaths().begin());
								vlIter->second->getVirLinkSubPaths().erase(
										vlIter->second->getVirLinkSubPaths().begin());

								delete *(vnwIter->second->getVirLink(
										virLinkToFrom_Id)->getVirLinkSubPaths().begin()
										+ pairPathFlag_1);

								vnwIter->second->getVirLink(virLinkToFrom_Id)->getVirLinkSubPaths().erase(
										vnwIter->second->getVirLink(
												virLinkToFrom_Id)->getVirLinkSubPaths().begin()
												+ pairPathFlag_1);
								cout
										<< "virtual link substrate transport paths amount : "
										<< vlIter->second->getVirLinkSubPaths().size()
										<< endl;
							}
						}
					} else {
						MessageSet::printInfoMessage(
								" Currently, there is no available substrate embeddable resource for allcate avaliable bandwidth from one substrate transport path to another. ");
					}
				}
				CalaculatedVirLinks.push_back(vlIter->second->getVirLinkId());
				CalaculatedVirLinks.push_back(virLinkToFrom_Id);
			}

			cout << " function step 2 ************ start " << endl;
			//SubNwGraph::printSubstrateNetwork(this->subNW);
			SubNwGraph::checkSubstrateLinksResource(this->subNW);

			cout << " function step 2 ************ end " << endl;
		}

	}

	cout << " function step 3 ************ start " << endl;
	//SubNwGraph::printSubstrateNetwork(this->subNW);
	SubNwGraph::checkSubstrateLinksResource(this->subNW);

	cout << " function step 3 ************ end " << endl;

	for (map<string, EndUserRequest*>::iterator euIter = euRequests.begin();
			euIter != euRequests.end(); ++euIter) {

		for (map<int, VirNwGraph*>::iterator vNwIter = this->vNws.begin();
				vNwIter != this->vNws.end(); ++vNwIter) {

			/*
			 * adjust end user request belong to which virtual network
			 */
			if (euIter->second->getReqVirNwNo()
					== vNwIter->second->getVirNwNo()) {

				EndUserRequest::printEndUserRequest(euIter->second);
				// calculate data transport path on virtual network layer
				string vSourceNode = euIter->second->getReqSourceVirNode();
				string vDestinationNode =
						euIter->second->getReqDestinationVirNode();

				double dataRate = euIter->second->getReqDataRate();

				/*
				 * the link weight is a  parameter used calaculate
				 */
				map<string, double> graphLinksInfo;
				for (map<string, VirNwLink*>::iterator vlIter =
						vNwIter->second->getVirLinks().begin();
						vlIter != vNwIter->second->getVirLinks().end();
						++vlIter) {

					graphLinksInfo.insert(
							pair<string, double>(vlIter->first,
									vlIter->second->getVirLinkWeight()));
				}

				// get data transport path on virtual network that can accept this request dataRate.
				BasePath* virPath =
						getOptimalShortestPathWithComplexConstraintsOnVirtualNetwork(
								vNwIter->second, vSourceNode, vDestinationNode,
								graphLinksInfo, 0.0, dataRate, 0.0, 0.0, 0);

				/*
				 *  if existing, then to update substrate transport path resource and virtual link resource information.
				 *
				 *  else, record this end-user request as a congestion.
				 */
				if (virPath != NULL) {
					cout << " virtual path from " << vSourceNode << " to "
							<< vDestinationNode << " length: "
							<< virPath->getPathInfo().size() << endl;

					BasePath::printBasePath(virPath);

					for (unsigned int i = 1; i < virPath->getPathInfo().size();
							++i) {

						// update virtual links bandwidth
						string virLink_FromTo = CommonFunctions::makeLinkId(
								virPath->getPathInfo().at(i - 1),
								virPath->getPathInfo().at(i));

						string virLink_ToFrom = CommonFunctions::makeLinkId(
								virPath->getPathInfo().at(i),
								virPath->getPathInfo().at(i - 1));

						// virtual link's available bandwidth check.
						if (vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
								< dataRate) {
							MessageSet::printErrorMessage(
									" Virtual data transport path's available bandwidth cannot accept this end-user request's datarate, Please check this bug. ");
							CommonFunctions::FAILURE_EXIT();

						}
						// virtual link's available bandwidth check.
						if (vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
								< dataRate) {
							MessageSet::printErrorMessage(
									" Virtual data transport path's available bandwidth cannot accept this end-user request's datarate, Please check this bug. ");
							CommonFunctions::FAILURE_EXIT();

						}
						// virtual links' available bandwidth check.
						if (vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
								!= vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()) {
							MessageSet::printErrorMessage(
									" Virtual links [a-b, b-a] available bandwidth is not the same as. Please check this bug.");
							CommonFunctions::FAILURE_EXIT();

						}

						/*
						 *  update substrate links of data transport path of virtual links
						 *  in case of the only one substrate path
						 */
						cout << " Virtual link's substrate path amount : "
								<< vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkSubPaths().size()
								<< endl;
						/*
						 * Currently, the amount of substrate transport path of virtual link is one or two,
						 * except cases are wrong that is bug.
						 */
						if (vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkSubPaths().size()
								< 1
								or vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkSubPaths().size()
										> 2) {
							MessageSet::printErrorMessage(
									" Virtual link: " + virLink_FromTo
											+ " has not mapped into substrate transport path, or mapped substrate transport paths are more than Two. Please check this bug. ");
							CommonFunctions::FAILURE_EXIT();
						}

						// virtual links's substrate transport paths check.
						if (vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkSubPaths().size()
								!= vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkSubPaths().size()) {

							MessageSet::printErrorMessage(
									" Virtual link pair[ a-b, b-a]'s substrate transport paths amount is different. Please check this bug. ");
							MessageSet::printFailureExitMessage(
									"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio.");
							CommonFunctions::FAILURE_EXIT();
						}

						// first handle the from-to virtual link. then handle the to-from virtual link.

						// in case of one substrate transport path of virtual links.
						if (vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkSubPaths().size()
								== 1) {

							BasePath* subPath = vNwIter->second->getVirLink(
									virLink_FromTo)->getVirLinkSubPaths().at(0);

							cout << " virtual link " << virLink_FromTo
									<< "'s substrate path:" << endl;

							BasePath::printBasePath(subPath);

							if (subPath->getPathBandwidth()
									!= vNwIter->second->getVirLink(
											virLink_FromTo)->getVirLinkBandwidth()) {
								MessageSet::printErrorMessage(
										" Virtual link's specified bandwidth is not the same as its substrate transport path's available bandwidth. Please check this bug. ");
								MessageSet::printFailureExitMessage("");
								CommonFunctions::FAILURE_EXIT();

							}

							if (subPath->getPathAvailableBandwidth()
									!= vNwIter->second->getVirLink(
											virLink_FromTo)->getVirLinkAvailableBandwidth()) {
								MessageSet::printErrorMessage(
										" Virtual link's available bandwidth is not the same as its substrate transport path's available bandwidth. Please check this bug. ");
								CommonFunctions::FAILURE_EXIT();

							}

							for (unsigned int i = 1;
									i < subPath->getPathInfo().size(); ++i) {
								// update substrate links bandwidth
								string subLink_FromTo =
										CommonFunctions::makeLinkId(
												subPath->getPathInfo().at(
														i - 1),
												subPath->getPathInfo().at(i));

								cout << " Virtual network (No="
										<< vNwIter->second->getVirNwNo()
										<< ")'s virtual link : "
												+ virLink_FromTo
												+ " 's substrate path link: "
										<< subLink_FromTo
										<< " 's available bandwidth ("
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " Mb) will be reduced bandwidth ("
										<< dataRate
										<< "Mb) for end user request." << endl;

								this->subNW->getSubLink(subLink_FromTo)->setSubLinkAvailableBandwidth(
										this->subNW->getSubLink(subLink_FromTo)->getSubLinkAvailableBandwidth()
												- dataRate);

								if (this->subNW->getSubLink(subLink_FromTo)->getSubLinkAvailableBandwidth()
										< 0) {
									MessageSet::printErrorMessage(
											" The substrate link resource is less than 0. Please check this bug.");
									CommonFunctions::FAILURE_EXIT();
								}

								cout << " Virtual network (No="
										<< vNwIter->second->getVirNwNo()
										<< ")'s virtual link : "
												+ virLink_FromTo
												+ " 's substrate path link: "
										<< subLink_FromTo
										<< " 's available bandwidth ("
										<< this->subNW->getSubLink(
												subLink_FromTo)->getSubLinkAvailableBandwidth()
										<< " Mb) ." << endl;

								euIter->second->addReqSubPathLink(
										subLink_FromTo, dataRate);
							}

							subPath->setPathAvailableBandwidth(
									subPath->getPathAvailableBandwidth()
											- dataRate);

							if (subPath->getPathAvailableBandwidth() < 0.0) {
								MessageSet::printErrorMessage(
										" The substrate transport path's available resource is less than 0. Please check this bug.");
								CommonFunctions::FAILURE_EXIT();

							}

							euIter->second->addReqSubPathInfo(
									subPath->getPathId(), dataRate);

						} else if (vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkSubPaths().size()
								== 2) {

							if (CommonConstants::EQUAL_ENDUSER_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK
									== splitFlag) {

								MessageSet::printInfoMessage(
										"EQUAL_ENDUSER_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK Start.");

								double subDataRate = dataRate / 2;

								for (int index = 0; index < 2; ++index) {

									BasePath* subPath =
											vNwIter->second->getVirLink(
													virLink_FromTo)->getVirLinkSubPaths().at(
													index);

									cout << " virtual link " << virLink_FromTo
											<< "'s substrate path:" << endl;

									BasePath::printBasePath(subPath);

									for (unsigned int i = 1;
											i < subPath->getPathInfo().size();
											++i) {
										// update substrate links bandwidth
										string subLink_FromTo =
												CommonFunctions::makeLinkId(
														subPath->getPathInfo().at(
																i - 1),
														subPath->getPathInfo().at(
																i));

										cout << " Virtual network (No="
												<< vNwIter->second->getVirNwNo()
												<< ")'s virtual link : "
														+ virLink_FromTo
														+ " 's substrate path link: "
												<< subLink_FromTo
												<< " 's available bandwidth ( "
												<< this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkAvailableBandwidth()
												<< " Mb) will be reduced bandwidth ( "
												<< subDataRate
												<< " Mb) for end user request."
												<< endl;

										this->subNW->getSubLink(subLink_FromTo)->setSubLinkAvailableBandwidth(
												this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkAvailableBandwidth()
														- subDataRate);

										cout << " Virtual network (No= "
												<< vNwIter->second->getVirNwNo()
												<< " )'s virtual link : "
														+ virLink_FromTo
														+ " 's substrate path link: "
												<< subLink_FromTo
												<< " 's available bandwidth ( "
												<< this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkAvailableBandwidth()
												<< " Mb) ." << endl;

										euIter->second->addReqSubPathLink(
												subLink_FromTo, subDataRate);
									}

									subPath->setPathAvailableBandwidth(
											subPath->getPathAvailableBandwidth()
													- subDataRate);

									euIter->second->addReqSubPathInfo(
											subPath->getPathId(), subDataRate);

								}

								MessageSet::printInfoMessage(
										"EQUAL_ENDUSER_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK End.");

							} else if (CommonConstants::NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK
									== splitFlag) {

								MessageSet::printInfoMessage(
										"NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK Start.");

								int pathFlag = 2;
								BasePath* subPath_0 =
										vNwIter->second->getVirLink(
												virLink_FromTo)->getVirLinkSubPaths().at(
												0);
								BasePath* subPath_1 =
										vNwIter->second->getVirLink(
												virLink_FromTo)->getVirLinkSubPaths().at(
												1);

								//BasePath* subPath = new BasePath();
								BasePath* subPath = NULL;
								/*
								 * first check path's available bandwidth,
								 * if all more than data rate, then to choose less switching energy path
								 * else choose available bandwidth more than datarate one. no considing switching eneryg.
								 *
								 * if all less than data rate, splitting datarate to two paths,
								 *     first consider less switching path. than another one
								 *
								 */

								/*
								 * two path's available bandwidth are more than datarate
								 */
								if ((subPath_0->getPathAvailableBandwidth()
										>= dataRate)
										&& (subPath_1->getPathAvailableBandwidth()
												>= dataRate)) {

									if (subPath_0->getPathSwitchingEnergy()
											< subPath_1->getPathSwitchingEnergy()) {
										subPath =
												vNwIter->second->getVirLink(
														virLink_FromTo)->getVirLinkSubPaths().at(
														0);
									} else {
										subPath =
												vNwIter->second->getVirLink(
														virLink_FromTo)->getVirLinkSubPaths().at(
														1);
									}
									pathFlag = 1;
								}

								// only one path's available bandwidth is more than datarate
								if (subPath_0->getPathAvailableBandwidth()
										< dataRate
										&& subPath_1->getPathAvailableBandwidth()
												>= dataRate) {
									subPath =
											vNwIter->second->getVirLink(
													virLink_FromTo)->getVirLinkSubPaths().at(
													1);
									pathFlag = 1;
								}

								// only one path's available bandwidth is more than datarate
								if (subPath_0->getPathAvailableBandwidth()
										>= dataRate
										&& subPath_1->getPathAvailableBandwidth()
												< dataRate) {
									subPath =
											vNwIter->second->getVirLink(
													virLink_FromTo)->getVirLinkSubPaths().at(
													0);
									pathFlag = 1;
								}

								if (1 == pathFlag) {
									cout << " one path" << endl;
									cout << " virtual link " << virLink_FromTo
											<< "'s substrate path:" << endl;

									BasePath::printBasePath(subPath);

									for (unsigned int i = 1;
											i < subPath->getPathInfo().size();
											++i) {
										// update substrate links bandwidth
										string subLink_FromTo =
												CommonFunctions::makeLinkId(
														subPath->getPathInfo().at(
																i - 1),
														subPath->getPathInfo().at(
																i));

										cout << " Virtual network (No="
												<< vNwIter->second->getVirNwNo()
												<< ")'s virtual link : "
														+ virLink_FromTo
														+ " 's substrate path link: "
												<< subLink_FromTo
												<< " 's available bandwidth ("
												<< this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkAvailableBandwidth()
												<< " Mb) will be reduced bandwidth ("
												<< dataRate
												<< "Mb) for end user request."
												<< endl;

										this->subNW->getSubLink(subLink_FromTo)->setSubLinkAvailableBandwidth(
												this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkAvailableBandwidth()
														- dataRate);

										cout << " Virtual network (No="
												<< vNwIter->second->getVirNwNo()
												<< ")'s virtual link : "
														+ virLink_FromTo
														+ " 's substrate path link: "
												<< subLink_FromTo
												<< " 's available bandwidth ("
												<< this->subNW->getSubLink(
														subLink_FromTo)->getSubLinkAvailableBandwidth()
												<< " Mb) ." << endl;

										euIter->second->addReqSubPathLink(
												subLink_FromTo, dataRate);
									}

									subPath->setPathAvailableBandwidth(
											subPath->getPathAvailableBandwidth()
													- dataRate);

									euIter->second->addReqSubPathInfo(
											subPath->getPathId(), dataRate);

								} else { // pathFlag = 2;

									cout << " two path" << endl;
									double subDatarate_0;
									double subDatarate_1;

									// two paths' available bandwidth are less than datarate
									if (subPath_0->getPathAvailableBandwidth()
											< dataRate
											&& subPath_1->getPathAvailableBandwidth()
													< dataRate) {

										if (subPath_0->getPathSwitchingEnergy()
												< subPath_1->getPathSwitchingEnergy()) {

											subDatarate_0 =
													subPath_0->getPathAvailableBandwidth();
											subDatarate_1 = dataRate
													- subDatarate_0;

										} else {

											subDatarate_1 =
													subPath_1->getPathAvailableBandwidth();
											subDatarate_0 = dataRate
													- subDatarate_1;

										}

										cout << "subDatarate_0:"
												<< subDatarate_0
												<< " path0 available Bw:"
												<< subPath_0->getPathAvailableBandwidth()
												<< " subDatarate_1:"
												<< subDatarate_1
												<< " path1 available Bw:"
												<< subPath_1->getPathAvailableBandwidth()
												<< endl;

										cout << " virtual link "
												<< virLink_FromTo
												<< "'s first substrate path:"
												<< endl;

										BasePath::printBasePath(subPath_0);

										cout << " virtual link "
												<< virLink_FromTo
												<< "'s second substrate path:"
												<< endl;

										BasePath::printBasePath(subPath_1);

										for (int index = 0; index < 2;
												++index) {

											subPath =
													vNwIter->second->getVirLink(
															virLink_FromTo)->getVirLinkSubPaths().at(
															index);

											double subDatarate;
											if (index == 0) {
												subDatarate = subDatarate_0;
											} else {
												subDatarate = subDatarate_1;
											}

											for (unsigned int i = 1;
													i
															< subPath->getPathInfo().size();
													++i) {
												// update substrate links bandwidth
												string subLink_FromTo =
														CommonFunctions::makeLinkId(
																subPath->getPathInfo().at(
																		i - 1),
																subPath->getPathInfo().at(
																		i));

												cout << " Virtual network (No="
														<< vNwIter->second->getVirNwNo()
														<< ")'s virtual link : "
																+ virLink_FromTo
																+ " 's substrate path link: "
														<< subLink_FromTo
														<< " 's available bandwidth ("
														<< this->subNW->getSubLink(
																subLink_FromTo)->getSubLinkAvailableBandwidth()
														<< " Mb) will be reduced bandwidth ("
														<< subDatarate
														<< "Mb) for end user request."
														<< endl;

												this->subNW->getSubLink(
														subLink_FromTo)->setSubLinkAvailableBandwidth(
														this->subNW->getSubLink(
																subLink_FromTo)->getSubLinkAvailableBandwidth()
																- subDatarate);

												cout << " Virtual network (No="
														<< vNwIter->second->getVirNwNo()
														<< ")'s virtual link : "
																+ virLink_FromTo
																+ " 's substrate path link: "
														<< subLink_FromTo
														<< " 's available bandwidth ("
														<< this->subNW->getSubLink(
																subLink_FromTo)->getSubLinkAvailableBandwidth()
														<< " Mb) ." << endl;

												euIter->second->addReqSubPathLink(
														subLink_FromTo,
														subDatarate);
											}

											subPath->setPathAvailableBandwidth(
													subPath->getPathAvailableBandwidth()
															- subDatarate);

											euIter->second->addReqSubPathInfo(
													subPath->getPathId(),
													subDatarate);
										}
										pathFlag = 2;
									} else {
										MessageSet::printErrorMessage(
												" Datarate and the avaliable bandwidth of substrate transport paths is not right. Please check this bug.");
										MessageSet::printFailureExitMessage(
												" NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK Error.");
										CommonFunctions::FAILURE_EXIT();

									}
								}

								MessageSet::printInfoMessage(
										"NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK End.");

							} else {
								MessageSet::printErrorMessage(
										" Splitting Flag: splitFlag parameter is inputed wrong. ");
								MessageSet::printFailureExitMessage(
										" This is configure bug, Please check the bug. ");
								CommonFunctions::FAILURE_EXIT();
							}
						} // two path end

						cout << " Virtual network ( No="
								<< vNwIter->second->getVirNwNo()
								<< " )'s virtual link : " + virLink_FromTo
										+ " 's available bandwidth ( "
								<< vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
								<< " Mb ) defined bandwidth: "
								<< vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkBandwidth()
								<< " Mb ) will be reduced bandwidth ( "
								<< dataRate << " Mb ) for end user request."
								<< endl;

						vNwIter->second->getVirLink(virLink_FromTo)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
										- dataRate);

						cout << " Virtual network ( No="
								<< vNwIter->second->getVirNwNo()
								<< " )'s virtual link : " + virLink_FromTo
										+ " 's available bandwidth ( "
								<< vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
								<< " Mb ) defined bandwidth: "
								<< vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkBandwidth()
								<< " Mb ) ." << endl;

						euIter->second->addReqVirPathLink(virLink_FromTo,
								dataRate);

						/*
						 *  update substrate links of data transport path of virtual links
						 *  in case of the only one substrate path
						 */
						cout << " Virtual link's substrate path amount : "
								<< vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkSubPaths().size()
								<< endl;
						/*
						 * Currently, the amount of substrate transport path of virtual link is one or two,
						 * except cases are wrong that is bug.
						 */
						if (vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkSubPaths().size()
								== 0
								or vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkSubPaths().size()
										> 2) {
							MessageSet::printErrorMessage(
									" Virtual link: " + virLink_FromTo
											+ " has not mapped into substrate transport path, or mapped substrate transport paths are more than Two. Please check this bug. ");
							CommonFunctions::FAILURE_EXIT();
						}

						if (vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkSubPaths().size()
								== 1) {
							BasePath* subPath = vNwIter->second->getVirLink(
									virLink_ToFrom)->getVirLinkSubPaths().at(0);

							cout << " virtual link " << virLink_FromTo
									<< "'s substrate path:" << endl;

							CommonFunctions::printVector(
									subPath->getPathInfo());

							for (unsigned int i = 1;
									i < subPath->getPathInfo().size(); ++i) {
								// update substrate links bandwidth
								string subLink_ToFrom =
										CommonFunctions::makeLinkId(
												subPath->getPathInfo().at(
														i - 1),
												subPath->getPathInfo().at(i));

								cout << " Virtual network (No="
										<< vNwIter->second->getVirNwNo()
										<< ")'s virtual link : "
												+ virLink_ToFrom
												+ " 's substrate path link: "
										<< subLink_ToFrom
										<< " 's available bandwidth ("
										<< this->subNW->getSubLink(
												subLink_ToFrom)->getSubLinkAvailableBandwidth()
										<< " Mb) will be reduced bandwidth ("
										<< dataRate
										<< "Mb) for end user request." << endl;

								this->subNW->getSubLink(subLink_ToFrom)->setSubLinkAvailableBandwidth(
										this->subNW->getSubLink(subLink_ToFrom)->getSubLinkAvailableBandwidth()
												- dataRate);

								cout << " Virtual network (No="
										<< vNwIter->second->getVirNwNo()
										<< ")'s virtual link : "
												+ virLink_ToFrom
												+ " 's substrate path link: "
										<< subLink_ToFrom
										<< " 's available bandwidth ("
										<< this->subNW->getSubLink(
												subLink_ToFrom)->getSubLinkAvailableBandwidth()
										<< " Mb) ." << endl;

								euIter->second->addReqSubPathLink(
										subLink_ToFrom, dataRate);
							}

							subPath->setPathAvailableBandwidth(
									subPath->getPathAvailableBandwidth()
											- dataRate);

							euIter->second->addReqSubPathInfo(
									subPath->getPathId(), dataRate);

						} else if (vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkSubPaths().size()
								== 2) {

							if (CommonConstants::EQUAL_ENDUSER_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK
									== splitFlag) {

								double subDataRate = dataRate / 2;

								for (int index = 0; index < 2; ++index) {

									BasePath* subPath =
											vNwIter->second->getVirLink(
													virLink_ToFrom)->getVirLinkSubPaths().at(
													index);

									cout << " virtual link " << virLink_ToFrom
											<< "'s substrate path:" << endl;

									CommonFunctions::printVector(
											subPath->getPathInfo());

									for (unsigned int i = 1;
											i < subPath->getPathInfo().size();
											++i) {
										// update substrate links bandwidth
										string subLink_ToFrom =
												CommonFunctions::makeLinkId(
														subPath->getPathInfo().at(
																i - 1),
														subPath->getPathInfo().at(
																i));

										cout << " Virtual network (No="
												<< vNwIter->second->getVirNwNo()
												<< ")'s virtual link : "
														+ virLink_ToFrom
														+ " 's substrate path link: "
												<< subLink_ToFrom
												<< " 's available bandwidth ( "
												<< this->subNW->getSubLink(
														subLink_ToFrom)->getSubLinkAvailableBandwidth()
												<< " Mb) will be reduced bandwidth ( "
												<< subDataRate
												<< " Mb) for end user request."
												<< endl;

										this->subNW->getSubLink(subLink_ToFrom)->setSubLinkAvailableBandwidth(
												this->subNW->getSubLink(
														subLink_ToFrom)->getSubLinkAvailableBandwidth()
														- subDataRate);

										cout << " Virtual network (No= "
												<< vNwIter->second->getVirNwNo()
												<< " )'s virtual link : "
														+ virLink_ToFrom
														+ " 's substrate path link: "
												<< subLink_ToFrom
												<< " 's available bandwidth ( "
												<< this->subNW->getSubLink(
														subLink_ToFrom)->getSubLinkAvailableBandwidth()
												<< " Mb) ." << endl;

										euIter->second->addReqSubPathLink(
												subLink_ToFrom, subDataRate);
									}

									subPath->setPathAvailableBandwidth(
											subPath->getPathAvailableBandwidth()
													- subDataRate);

									euIter->second->addReqSubPathInfo(
											subPath->getPathId(), subDataRate);

								}

							} else if (CommonConstants::NO_EQUA_ENDUSERL_DATARATE_SPLIT_FLAG_FOR_SUBSTRATE_PATH_OF_VIRTUAL_LINK
									== splitFlag) {

								int pathFlag = 2;
								BasePath* subPath_0 =
										vNwIter->second->getVirLink(
												virLink_ToFrom)->getVirLinkSubPaths().at(
												0);
								BasePath* subPath_1 =
										vNwIter->second->getVirLink(
												virLink_ToFrom)->getVirLinkSubPaths().at(
												1);

								//BasePath* subPath = new BasePath();
								BasePath* subPath = NULL;
								/*
								 * first check path's available bandwidth,
								 * if all more than data rate, then to choose less switching energy path
								 * else choose available bandwidth more than datarate one. no considing switching eneryg.
								 *
								 * if all less than data rate, splitting datarate to two paths,
								 *     first consider less switching path. than another one
								 *
								 */

								/*
								 * two path's available bandwidth are more than datarate
								 */
								if ((subPath_0->getPathAvailableBandwidth()
										>= dataRate)
										&& (subPath_1->getPathAvailableBandwidth()
												>= dataRate)) {

									if (subPath_0->getPathSwitchingEnergy()
											< subPath_1->getPathSwitchingEnergy()) {
										subPath =
												vNwIter->second->getVirLink(
														virLink_ToFrom)->getVirLinkSubPaths().at(
														0);
									} else {
										subPath =
												vNwIter->second->getVirLink(
														virLink_ToFrom)->getVirLinkSubPaths().at(
														1);
									}
									pathFlag = 1;
								}

								// only one path's available bandwidth is more than datarate
								if (subPath_0->getPathAvailableBandwidth()
										< dataRate
										&& subPath_1->getPathAvailableBandwidth()
												>= dataRate) {
									subPath =
											vNwIter->second->getVirLink(
													virLink_ToFrom)->getVirLinkSubPaths().at(
													1);
									pathFlag = 1;
								}

								// only one path's available bandwidth is more than datarate
								if (subPath_0->getPathAvailableBandwidth()
										>= dataRate
										&& subPath_1->getPathAvailableBandwidth()
												< dataRate) {
									subPath =
											vNwIter->second->getVirLink(
													virLink_ToFrom)->getVirLinkSubPaths().at(
													0);
									pathFlag = 1;
								}

								if (1 == pathFlag) {
									cout << " virtual link " << virLink_ToFrom
											<< "'s substrate path:" << endl;

									CommonFunctions::printVector(
											subPath->getPathInfo());

									for (unsigned int i = 1;
											i < subPath->getPathInfo().size();
											++i) {
										// update substrate links bandwidth
										string subLink_ToFrom =
												CommonFunctions::makeLinkId(
														subPath->getPathInfo().at(
																i - 1),
														subPath->getPathInfo().at(
																i));

										cout << " Virtual network (No="
												<< vNwIter->second->getVirNwNo()
												<< ")'s virtual link : "
														+ virLink_ToFrom
														+ " 's substrate path link: "
												<< subLink_ToFrom
												<< " 's available bandwidth ("
												<< this->subNW->getSubLink(
														subLink_ToFrom)->getSubLinkAvailableBandwidth()
												<< " Mb) will be reduced bandwidth ("
												<< dataRate
												<< "Mb) for end user request."
												<< endl;

										this->subNW->getSubLink(subLink_ToFrom)->setSubLinkAvailableBandwidth(
												this->subNW->getSubLink(
														subLink_ToFrom)->getSubLinkAvailableBandwidth()
														- dataRate);

										cout << " Virtual network (No="
												<< vNwIter->second->getVirNwNo()
												<< ")'s virtual link : "
														+ virLink_ToFrom
														+ " 's substrate path link: "
												<< subLink_ToFrom
												<< " 's available bandwidth ("
												<< this->subNW->getSubLink(
														subLink_ToFrom)->getSubLinkAvailableBandwidth()
												<< " Mb) ." << endl;

										euIter->second->addReqSubPathLink(
												subLink_ToFrom, dataRate);
									}

									subPath->setPathAvailableBandwidth(
											subPath->getPathAvailableBandwidth()
													- dataRate);

									euIter->second->addReqSubPathInfo(
											subPath->getPathId(), dataRate);

								} else { // pathFlag = 2;

									double subDatarate_0;
									double subDatarate_1;

									// two paths' available bandwidth are less than datarate
									if (subPath_0->getPathAvailableBandwidth()
											< dataRate
											&& subPath_1->getPathAvailableBandwidth()
													< dataRate) {

										if (subPath_0->getPathSwitchingEnergy()
												< subPath_1->getPathSwitchingEnergy()) {

											subDatarate_0 =
													subPath_0->getPathAvailableBandwidth();
											subDatarate_1 = dataRate
													- subDatarate_0;

										} else {

											subDatarate_1 =
													subPath_1->getPathAvailableBandwidth();
											subDatarate_0 = dataRate
													- subDatarate_1;

										}

										cout << "subDatarate_0:"
												<< subDatarate_0
												<< " path0 available Bw:"
												<< subPath_0->getPathAvailableBandwidth()
												<< " subDatarate_1:"
												<< subDatarate_1
												<< " path1 available Bw:"
												<< subPath_1->getPathAvailableBandwidth()
												<< endl;

										cout << " virtual link "
												<< virLink_ToFrom
												<< "'s first substrate path:"
												<< endl;

										CommonFunctions::printVector(
												subPath_0->getPathInfo());

										cout << " virtual link "
												<< virLink_ToFrom
												<< "'s second substrate path:"
												<< endl;

										CommonFunctions::printVector(
												subPath_1->getPathInfo());

										for (int index = 0; index < 2;
												++index) {

											subPath =
													vNwIter->second->getVirLink(
															virLink_ToFrom)->getVirLinkSubPaths().at(
															index);

											double subDatarate;
											if (index == 0) {
												subDatarate = subDatarate_0;
											} else {
												subDatarate = subDatarate_1;
											}

											for (unsigned int i = 1;
													i
															< subPath->getPathInfo().size();
													++i) {
												// update substrate links bandwidth
												string subLink_ToFrom =
														CommonFunctions::makeLinkId(
																subPath->getPathInfo().at(
																		i - 1),
																subPath->getPathInfo().at(
																		i));

												cout << " Virtual network (No="
														<< vNwIter->second->getVirNwNo()
														<< ")'s virtual link : "
																+ virLink_ToFrom
																+ " 's substrate path link: "
														<< subLink_ToFrom
														<< " 's available bandwidth ("
														<< this->subNW->getSubLink(
																subLink_ToFrom)->getSubLinkAvailableBandwidth()
														<< " Mb) will be reduced bandwidth ("
														<< subDatarate
														<< "Mb) for end user request."
														<< endl;

												this->subNW->getSubLink(
														subLink_ToFrom)->setSubLinkAvailableBandwidth(
														this->subNW->getSubLink(
																subLink_ToFrom)->getSubLinkAvailableBandwidth()
																- subDatarate);

												cout << " Virtual network (No="
														<< vNwIter->second->getVirNwNo()
														<< ")'s virtual link : "
																+ virLink_ToFrom
																+ " 's substrate path link: "
														<< subLink_ToFrom
														<< " 's available bandwidth ("
														<< this->subNW->getSubLink(
																subLink_ToFrom)->getSubLinkAvailableBandwidth()
														<< " Mb) ." << endl;

												euIter->second->addReqSubPathLink(
														subLink_ToFrom,
														subDatarate);
											}
											subPath->setPathAvailableBandwidth(
													subPath->getPathAvailableBandwidth()
															- subDatarate);

											euIter->second->addReqSubPathInfo(
													subPath->getPathId(),
													subDatarate);
										}
										pathFlag = 2;
									}
								}

							} else {
								MessageSet::printErrorMessage(
										" Splitting Flag: splitFlag parameter is inputed wrong. ");
								MessageSet::printFailureExitMessage(
										" This is configure bug, Please check the bug. ");
								CommonFunctions::FAILURE_EXIT();
							}
						} // two path end

						cout << " Virtual network (No="
								<< vNwIter->second->getVirNwNo()
								<< ")'s virtual link : " + virLink_ToFrom
										+ " 's bandwidth ("
								<< vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
								<< " Mb) will be reduced bandwidth ("
								<< dataRate << " Mb) for end user request."
								<< endl;

						vNwIter->second->getVirLink(virLink_ToFrom)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
										- dataRate);

						cout << " Virtual network ( No="
								<< vNwIter->second->getVirNwNo()
								<< " )'s virtual link : " + virLink_ToFrom
										+ " 's bandwidth ( "
								<< vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
								<< " Mb )." << endl;

						euIter->second->addReqVirPathLink(virLink_ToFrom,
								dataRate);
					}

				} else {
					// congestion happened record
					//map< int , int >
					//  int: timing
					//  int: congestion times
					cout << " End User Request Congestion: "
							<< euIter->second->getReqId() << endl;
					setCongestionRecord(timing, euIter->second->getReqId());
				}
				VirNwGraph::checkVirtualLinksResource(vNwIter->second);
			}
		}

	}

	cout << " function step 4 ************ start " << endl;
	//SubNwGraph::printSubstrateNetwork(this->subNW);
	SubNwGraph::checkSubstrateLinksResource(this->subNW);

	cout << " function step 4 ************ end " << endl;

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio End ");
}
/*
 *  input:
 *     timing :  current timing
 *     euRequests: the new end user requests [ requestId, pointer]
 */
void VirtualNetworkEmbedding::performVirtualNetworkEnvironment(const int timing,
		map<string, EndUserRequest*> euRequests) {

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::performVirtualNetworkEnvironment Start.");
// release end user request resource from virtual network and substrate network

	if (this->endUserRequestRecords.size() > 0) {
		cout << " At timing: " << timing
				<< ", these end-user requests finish and release virtual and substrate networks resource: "
				<< endl;

		pair<multimap<int, EndUserRequest*>::iterator,
				multimap<int, EndUserRequest*>::iterator> endReqIter;

		endReqIter = this->endUserRequestRecords.equal_range(timing);

		for (multimap<int, EndUserRequest*>::iterator reqIter = endReqIter.first;
				reqIter != endReqIter.second; ++reqIter) {

			cout << "request Id: " << reqIter->second->getReqId()
					<< " requested VirNw: " << reqIter->second->getReqVirNwNo()
					<< " data rate:" << reqIter->second->getReqDataRate()
					<< " start timing: " << reqIter->second->getReqStartTiming()
					<< " duration time: "
					<< reqIter->second->getReqDurationTimeUnits()
					<< " end timing: " << reqIter->second->getReqEndTiming()
					<< " virtual path links amount : "
					<< reqIter->second->getReqVirPathLinks().size()
					<< " substrate path links amount : "
					<< reqIter->second->getReqSubPathsLinks().size() << endl;

			for (map<int, VirNwGraph*>::iterator vNwIter = this->vNws.begin();
					vNwIter != this->vNws.end(); ++vNwIter) {

				if (vNwIter->second->getVirNwNo()
						== reqIter->second->getReqVirNwNo()) {

					cout << " Virtual network ("
							<< vNwIter->second->getVirNwNo() << ")'s request:"
							<< reqIter->second->getReqId() << " VM: "
							<< reqIter->second->getReqVirNwNo()
							<< " virtual links : "
							<< reqIter->second->getReqVirPathLinks().size()
							<< endl;

					for (unsigned int index = 0;
							index < reqIter->second->getReqVirPathLinks().size();
							++index) {

						string virLinkId =
								reqIter->second->getReqVirPathLinks()[index].first;
						double virDatarate =
								reqIter->second->getReqVirPathLinks()[index].second;

						cout << " virtual network (" << vNwIter->first
								<< ")'s virtual link (" << virLinkId << "( "
								<< vNwIter->second->getVirLink(virLinkId)->getVirLinkBandwidth()
								<< " Mb)" << " )'s available bandwidth ( "
								<< vNwIter->second->getVirLink(virLinkId)->getVirLinkAvailableBandwidth()
								<< " Mb) will add bandwidth ( "
								<< reqIter->second->getReqDataRate()
								<< " Mb) recorded allocated bandwidth ( "
								<< virDatarate << " Mb)" << endl;

						vNwIter->second->getVirLink(virLinkId)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink(virLinkId)->getVirLinkAvailableBandwidth()
										+ virDatarate);

						cout << " virtual network (" << vNwIter->first
								<< ")'s virtual link (" << virLinkId << "("
								<< vNwIter->second->getVirLink(virLinkId)->getVirLinkBandwidth()
								<< " Mb)'s available bandwidth ("
								<< vNwIter->second->getVirLink(virLinkId)->getVirLinkAvailableBandwidth()
								<< " Mb)." << endl;

						// release substrate transport path resource of virtual link when releasing a end user request
						for (vector<pair<string, double>>::iterator rIter =
								reqIter->second->getReqSubPathInfo().begin();
								rIter
										!= reqIter->second->getReqSubPathInfo().end();
								++rIter) {

							for (vector<BasePath*>::iterator bpIter =
									vNwIter->second->getVirLink(virLinkId)->getVirLinkSubPaths().begin();
									bpIter
											!= vNwIter->second->getVirLink(
													virLinkId)->getVirLinkSubPaths().end();
									++bpIter) {
								//								cout << " virtual network (" << vNwIter->first
								//										<< ")'s virtual link (" << virLinkId
								//										<< "( "
								//										<< vNwIter->second->getVirLink(
								//												virLinkId)->getVirLinkBandwidth()
								//										<< " Mb)"
								//										<< " )'s available bandwidth ( "
								//										<< vNwIter->second->getVirLink(
								//												virLinkId)->getVirLinkAvailableBandwidth()
								//										<< " Mb)'s substrate transport path "
								//										<< (*rIter).first
								//										<< " will add bandwidth ( "
								//										<< reqIter->second->getReqDataRate()
								//										<< " Mb) recorded allocated bandwidth ( "
								//										<< virDatarate << " Mb)" << endl;

								if ((*bpIter)->getPathId() == (*rIter).first) {
									(*bpIter)->setPathAvailableBandwidth(
											(*bpIter)->getPathAvailableBandwidth()
													+ (*rIter).second);
								}
							}
						}

					}

					for (unsigned int index = 0;
							index
									< reqIter->second->getReqSubPathsLinks().size();
							++index) {

						string subLinkId =
								reqIter->second->getReqSubPathsLinks()[index].first;
						double subDatarate =
								reqIter->second->getReqSubPathsLinks()[index].second;

						cout << " substrate network 's substrate link ("
								<< subLinkId << "("
								<< this->subNW->getSubLink(subLinkId)->getSubLinkBandwidth()
								<< " Mb)" << ")'s available bandwidth ("
								<< this->subNW->getSubLink(subLinkId)->getSubLinkAvailableBandwidth()
								<< " Mb) will add bandwidth ("
								<< reqIter->second->getReqDataRate()
								<< "Mb) recorded allocated bandwidth ("
								<< subDatarate << " Mb)" << endl;

						this->subNW->getSubLink(subLinkId)->setSubLinkAvailableBandwidth(
								this->subNW->getSubLink(subLinkId)->getSubLinkAvailableBandwidth()
										+ subDatarate);

						cout << " substrate network 's substrate link ("
								<< subLinkId << "("
								<< this->subNW->getSubLink(subLinkId)->getSubLinkBandwidth()
								<< " Mb)" << ")'s available bandwidth ("
								<< this->subNW->getSubLink(subLinkId)->getSubLinkAvailableBandwidth()
								<< " Mb)" << endl;

					}

				}
			}

		}
	}

//release virtual network resource based on the ending timing of virtual network
// delete virtual network pointer;
	MessageSet::printInfoMessage(
			" Current, there are "
					+ CommonFunctions::CoverIntegerToString(this->vNws.size())
					+ " Virtual Networks embedded on Substrate network.");

	for (map<int, VirNwGraph*>::iterator vnwIter = this->vNws.begin();
			vnwIter != this->vNws.end();) {
		if (vnwIter->second->getVirNwEndTiming() == timing) {

			MessageSet::printInfoMessage(
					" Virtual network (" + vnwIter->second->getVirNwNo()
							+ ") will be release.");

			for (map<string, VirNwLink*>::iterator vlIter =
					vnwIter->second->getVirLinks().begin();
					vlIter != vnwIter->second->getVirLinks().end(); ++vlIter) {

				VirNwLink::printVirNwLinkInfo(vlIter->second);

				cout << " befor substrate path amount : "
						<< vlIter->second->getVirLinkSubPaths().size() << endl;
				// one virtual link may be mapped into some substrate paths
				int index = 0;
				for (vector<BasePath*>::iterator bpIter =
						vlIter->second->getVirLinkSubPaths().begin();
						bpIter != vlIter->second->getVirLinkSubPaths().end();
						++bpIter) {

					cout << ++index << endl;
					cout << " *****" << endl;
					double bandwidth = (*bpIter)->getPathBandwidth();
					cout << " path ID:  " << (*bpIter)->getPathId() << endl;

					//release each substrate link resource
					for (unsigned int i = 1;
							i < (*bpIter)->getPathInfo().size(); ++i) {
						cout << " index: " << i << " substrate path: "
								<< (*bpIter)->getPathId() << endl;
						string subLink_FromTo = CommonFunctions::makeLinkId(
								(*bpIter)->getPathInfo().at(i - 1),
								(*bpIter)->getPathInfo().at(i));

						SubNwLink::printSubLinkInfo(
								this->subNW->getSubLink(subLink_FromTo));

						this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
								this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										+ bandwidth);

						SubNwLink::printSubLinkInfo(
								this->subNW->getSubLink(subLink_FromTo));

					}

					// delete virtual link mapped substate link pointer
					MessageSet::printInfoMessage(
							" Virtual link(" + vlIter->second->getVirLinkId()
									+ ")'s substrate path ("
									+ (*bpIter)->getPathId()
									+ ") will be released. ");
					delete (*bpIter);
					cout << "----" << endl;
				}

				vlIter->second->getVirLinkSubPaths().clear();
				cout << " After substrate path amount : "
						<< vlIter->second->getVirLinkSubPaths().size() << endl;
				// delete virtual node pointer
				MessageSet::printInfoMessage(
						" Virtual link (" + vlIter->second->getVirLinkId()
								+ ") will be released. ");
				delete vlIter->second;
			}
			vnwIter->second->getVirLinks().clear();

			//release substrate node resource and virNode memory and delete pointer
			for (map<string, VirNwNode*>::iterator vnIter =
					vnwIter->second->getVirNodes().begin();
					vnIter != vnwIter->second->getVirNodes().end(); ++vnIter) {

				VirNwNode::printVirNodeInfo(vnIter->second);

				string subNodeId = vnIter->second->getVirNodeMappedSubNodeId();
				double capapbility = vnIter->second->getVirNodeCapability();

				if (this->subNW->getSubNode(subNodeId) == NULL) {
					MessageSet::printErrorMessage(
							" Substrate Node: " + subNodeId
									+ " is no existence in substrate network.");
					CommonFunctions::FAILURE_EXIT();
				}

				SubNwNode::printSubNodeInfo(this->subNW->getSubNode(subNodeId));

				this->subNW->getSubNode(subNodeId)->setSubNodeAvaiableCapability(
						this->subNW->getSubNode(subNodeId)->getSubNodeAvaiableCapability()
								+ capapbility);

				this->subNW->getSubNode(subNodeId)->setSubNodeEmbeddedAmount(
						this->subNW->getSubNode(subNodeId)->getSubNodeEmbeddedAmount()
								- 1);

				SubNwNode::printSubNodeInfo(this->subNW->getSubNode(subNodeId));

				// delete virtual node pointer
				MessageSet::printInfoMessage(
						" Virtual Node (" + vnIter->second->getVirNodeId()
								+ ") will be released. ");
				delete vnIter->second;
			}
			vnwIter->second->getVirNodes().clear();

			MessageSet::printInfoMessage(
					" Virtual Network (" + vnwIter->second->getVirNwNo()
							+ ") has been released. ");
			VirNwGraph::printVirtualNetworkInfo(vnwIter->second);
			delete vnwIter->second;
			this->vNws.erase(vnwIter);

			cout << " the virtual networks amount : " << this->vNws.size()
					<< endl;
			vnwIter = this->vNws.begin();
			continue;
		}
		++vnwIter;
	}

	MessageSet::printInfoMessage(
			" Current, there are "
					+ CommonFunctions::CoverIntegerToString(this->vNws.size())
					+ " Virtual Networks embedded on Substrate network.");
// process each end user request, allocate their datarate from virtual tranpsort path to substrate transport path.

	for (map<string, EndUserRequest*>::iterator euIter = euRequests.begin();
			euIter != euRequests.end(); ++euIter) {

		for (map<int, VirNwGraph*>::iterator vNwIter = this->vNws.begin();
				vNwIter != this->vNws.end(); ++vNwIter) {

			if (euIter->second->getReqVirNwNo()
					== vNwIter->second->getVirNwNo()) {

				cout << "      " << endl;
				cout << " Processing RequestID:" << euIter->second->getReqId()
						<< endl;
				// calculate data transport path on virtual network layer
				string vSourceNode = euIter->second->getReqSourceVirNode();
				string vDestinationNode =
						euIter->second->getReqDestinationVirNode();

				double dataRate = euIter->second->getReqDataRate();

				/*
				 * the link weight is a  parameter used calacuate
				 */
				map<string, double> graphLinksInfo;
				for (map<string, VirNwLink*>::iterator vlIter =
						vNwIter->second->getVirLinks().begin();
						vlIter != vNwIter->second->getVirLinks().end();
						++vlIter) {

					graphLinksInfo.insert(
							pair<string, double>(vlIter->first,
									vlIter->second->getVirLinkWeight()));
				}

				BasePath* virPath =
						getOptimalShortestPathWithComplexConstraintsOnVirtualNetwork(
								vNwIter->second, vSourceNode, vDestinationNode,
								graphLinksInfo, 0.0, dataRate, 0.0, 0.0, 0);

				if (virPath != NULL) {
					cout << " Virtual path from " << vSourceNode << " to "
							<< vDestinationNode << " length: "
							<< virPath->getPathInfo().size() << endl;

					CommonFunctions::printVector(virPath->getPathInfo());

					for (unsigned int i = 1; i < virPath->getPathInfo().size();
							++i) {
						// update virtual links bandwidth
						string virLink_FromTo = CommonFunctions::makeLinkId(
								virPath->getPathInfo().at(i - 1),
								virPath->getPathInfo().at(i));

//
						cout << " Virtual network (No="
								<< vNwIter->second->getVirNwNo()
								<< ")'s virtual link : " + virLink_FromTo
										+ " 's bandwidth ("
								<< vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
								<< " Mb) will be reduced bandwidth ("
								<< dataRate << "(Mb) for end user request."
								<< endl;

						vNwIter->second->getVirLink(virLink_FromTo)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
										- dataRate);

						cout << "   " << virLink_FromTo << endl;
						euIter->second->addReqVirPathLink(virLink_FromTo,
								dataRate);

						BasePath* subPath_FromTo = vNwIter->second->getVirLink(
								virLink_FromTo)->getVirLinkSubPaths().at(0);

						cout << " virtual link " << virLink_FromTo
								<< "'s substrate path:" << endl;

						CommonFunctions::printVector(
								subPath_FromTo->getPathInfo());

						for (unsigned int i = 1;
								i < subPath_FromTo->getPathInfo().size(); ++i) {
							// update substrate links bandwidth

							string subLink_FromTo = CommonFunctions::makeLinkId(
									subPath_FromTo->getPathInfo().at(i - 1),
									subPath_FromTo->getPathInfo().at(i));

							//cout << "  " << subLink_FromTo << endl;
							this->subNW->getSubLink(subLink_FromTo)->setSubLinkAvailableBandwidth(
									this->subNW->getSubLink(subLink_FromTo)->getSubLinkAvailableBandwidth()
											- dataRate);

							//cout << "   " << subLink_FromTo << endl;
							euIter->second->addReqSubPathLink(subLink_FromTo,
									dataRate);

						}

						cout << " exchange : " << endl;
						string virLink_ToFrom = CommonFunctions::makeLinkId(
								virPath->getPathInfo().at(i),
								virPath->getPathInfo().at(i - 1));

						cout << " Virtual network (No="
								<< vNwIter->second->getVirNwNo()
								<< ")'s virtual link : " + virLink_ToFrom
										+ " 's bandwidth ("
								<< vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
								<< " Mb) will be reduced bandwidth ("
								<< dataRate << "(Mb) for end user request."
								<< endl;

						vNwIter->second->getVirLink(virLink_ToFrom)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
										- dataRate);

						//cout << "   " << virLink_ToFrom << endl;
						euIter->second->addReqVirPathLink(virLink_ToFrom,
								dataRate);

						/*
						 *  update substrate links of data transport path of virtual links
						 *  in case of the only one substrate path
						 */

						BasePath* subPath_ToFrom = vNwIter->second->getVirLink(
								virLink_ToFrom)->getVirLinkSubPaths().at(0);

						cout << " virtual link " << virLink_ToFrom
								<< "'s substrate path:" << endl;

						CommonFunctions::printVector(
								subPath_ToFrom->getPathInfo());

						for (unsigned int i = 1;
								i < subPath_ToFrom->getPathInfo().size(); ++i) {
							// update substrate links bandwidth
							string subLink_ToFrom = CommonFunctions::makeLinkId(
									subPath_ToFrom->getPathInfo().at(i - 1),
									subPath_ToFrom->getPathInfo().at(i));

							//cout << "  " << subLink_ToFrom << endl;

							this->subNW->getSubLink(subLink_ToFrom)->setSubLinkAvailableBandwidth(
									this->subNW->getSubLink(subLink_ToFrom)->getSubLinkAvailableBandwidth()
											- dataRate);
							//cout << "   " << subLink_ToFrom << endl;
							euIter->second->addReqSubPathLink(subLink_ToFrom,
									dataRate);
						}
					}

//					cout << " request ID:" << euIter->second->getReqId()
//							<< endl;
//					cout << "  Virtual links: " << endl;
//
//					for (unsigned int index = 0;
//							index < euIter->second->getReqVirPathLinks().size();
//							++index) {
//						cout
//								<< euIter->second->getReqVirPathLinks()[index].first
//								<< endl;
//					}
//					cout << " link amount : "
//							<< euIter->second->getReqVirPathLinks().size()
//							<< endl;
//
//					cout << endl;
//
//					cout << "  Substrate links: " << endl;
//
//					for (unsigned int index = 0;
//							index < euIter->second->getReqSubPathsLinks().size();
//							++index) {
//						cout
//								<< euIter->second->getReqSubPathsLinks()[index].first
//								<< endl;
//					}
//					cout << " link amount : "
//							<< euIter->second->getReqSubPathsLinks().size()
//							<< endl;
//
//					cout << endl;
				} else {
					// congestion happened record
					//map< int , int >
					//  int: timing
					//  int: congestion times
					cout << " End User Request Congestion: "
							<< euIter->second->getReqId() << endl;
					setCongestionRecord(timing, euIter->second->getReqId());
				}
				VirNwGraph::checkVirtualLinksResource(vNwIter->second);
			}
		}

	}

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::performVirtualNetworkEnvironment End.");
}

/*
 *  remove ending user request records and release memories.
 *    place after performVirtualNetworkEnvironment* and updateNveDataSwitchingEnergy* function.
 */
void VirtualNetworkEmbedding::releaseEndUserVideoRequestsForVirtualNetwork(
		const int timing) {

	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::releaseEndUserVideoRequestsForVirtualNetwork("
					+ CommonFunctions::CoverIntegerToString(timing)
					+ ") start.");
	int amount = 0;
	for (multimap<int, EndUserRequest*>::iterator reqIter =
			this->endUserRequestRecords.begin();
			reqIter != this->endUserRequestRecords.end(); ++reqIter) {

		if (reqIter->second->getReqEndTiming() < timing) {
			amount++;
			delete reqIter->second;
			this->endUserRequestRecords.erase(reqIter);
		}
	}

	CommonFunctions::printInfoMessage(
			" At timing: " + CommonFunctions::CoverIntegerToString(timing)
					+ ", there are "
					+ CommonFunctions::CoverIntegerToString(amount)
					+ " end-user request records removed and their memories resource are released. ");
	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::releaseEndUserVideoRequestsForVirtualNetwork("
					+ CommonFunctions::CoverIntegerToString(timing) + ") end.");
}
/*
 * get a disjoint path based on original substrate transport path and specified bandwidth, length constraints
 *  if  vertex disjoint-path does not exist
 *   then to get edge disjoint-path
 *
 *   adoptedPathFlag :  F: the shortest path, R: the lengthest path: M: middle path
 */
BasePath* VirtualNetworkEmbedding::getOneDisjointPathWithComplexConstraintsOnSubstrateNetwork(
		SubNwGraph * embedCheckSubNW, const BasePath* bPath, double bandwidth,
		double bpSwithingEnergy) {

	map<string, BasePath*> kPaths =
			getVertexDisjointPathWithComplexConstraintsOnSubstrateNetwork(
					embedCheckSubNW, bPath, bandwidth);

	if (kPaths.size() == 0) {
		cout
				<< " getVertexDisjointPathWithComplexConstraintsOnSubstrateNetwork's result is null "
				<< endl;
		kPaths = getEdgeDisjointPathWithComplexConstraintsOnSubstrateNetwork(
				embedCheckSubNW, bPath, bandwidth);
		if (kPaths.size() == 0) {
			cout
					<< " getEdgeDisjointPathWithComplexConstraintsOnSubstrateNetwork's result is null "
					<< endl;
			return NULL;
		}
	}
	cout
			<< " getOneDisjointPathWithComplexConstraintsOnSubstrateNetwork Step 1:"
			<< kPaths.size() << endl;
	/*
	 * sorted order increasing by switching energy
	 */
	multimap<double, BasePath*> paths;
	for (map<string, BasePath*>::iterator kIter = kPaths.begin();
			kIter != kPaths.end(); ++kIter) {

		double swichingEnergy = 0.0;
		for (unsigned int i = 1; i < kIter->second->getPathInfo().size(); ++i) {

			string subLink_FromTo = CommonFunctions::makeLinkId(
					kIter->second->getPathInfo().at(i - 1),
					kIter->second->getPathInfo().at(i));

			swichingEnergy =
					swichingEnergy
							+ embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkSwitchingEnergy();

		}

		if (swichingEnergy >= bpSwithingEnergy) {
			continue;
		}

		cout << " existing path switching energy: " << bpSwithingEnergy
				<< " new allocated path switching energy: " << swichingEnergy
				<< endl;

		kIter->second->setPathBandwidth(bandwidth);
		kIter->second->setPathAvailableBandwidth(bandwidth);
		kIter->second->setPathSwitchingEnergy(swichingEnergy);
		kIter->second->setPathDistance(kIter->second->getPathCost());

		paths.insert(
				pair<double, BasePath*>(kIter->second->getPathSwitchingEnergy(),
						kIter->second));
	}

	cout
			<< " getOneDisjointPathWithComplexConstraintsOnSubstrateNetwork Step 2:"
			<< paths.size() << endl;
	if (paths.size() != 0) {
		int index = 0;
		int middleIndex = paths.size() / 2;

		for (multimap<double, BasePath*>::iterator iter = paths.begin();
				iter != paths.end(); ++iter) {

			if (index == middleIndex) {
				cout
						<< " the new substrate transport path 's swiching energy is :"
						<< iter->second->getPathSwitchingEnergy() << endl;
				return iter->second;
			}
			index++;
		}
	} else {
		return NULL;
	}
}

map<string, BasePath*> VirtualNetworkEmbedding::getVertexDisjointPathWithComplexConstraintsOnSubstrateNetwork(
		SubNwGraph * embedCheckSubNW, const BasePath* bPath, double bandwidth) {
	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::getVertexDisjointPathWithComplexConstraintsOnSubstrateNetwork Start");

	vector<string> nodes = bPath->getPathInfo();

// remove the source node
	nodes.erase(nodes.begin());

// remove the destination node
	nodes.pop_back();

	/*
	 * the link weight is a  parameter used calcuate
	 */
	map<string, double> graphLinksInfo;
	for (map<string, SubNwLink*>::iterator slIter =
			embedCheckSubNW->getSubLinks().begin();
			slIter != embedCheckSubNW->getSubLinks().end(); ++slIter) {
		vector<string> FromTo = CommonFunctions::stringSplit(slIter->first,
				'_');

// for node disjoint-path
		if (find(nodes.begin(), nodes.end(), FromTo[0]) != nodes.end()) {
			continue;
		}

		if (find(nodes.begin(), nodes.end(), FromTo[1]) != nodes.end()) {
			continue;
		}

		if (slIter->second->getSubLinkEmbeddableBandwidth() >= bandwidth) {
			graphLinksInfo.insert(
					pair<string, double>(slIter->first,
							slIter->second->getSubLinkWeight()));
		}
	}

	map<string, BasePath*> kPaths = this->kShortPaths->getKShortestpaths(
			bPath->getPathSource(), bPath->getPathDestination(),
			graphLinksInfo);

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::getVertexDisjointPathWithComplexConstraintsOnSubstrateNetwork End");
	return kPaths;
}

map<string, BasePath*> VirtualNetworkEmbedding::getEdgeDisjointPathWithComplexConstraintsOnSubstrateNetwork(
		SubNwGraph * embedCheckSubNW, const BasePath* bPath, double bandwidth) {

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::getEdgeDisjointPathWithComplexConstraintsOnSubstrateNetwork Start");
	vector<string> nodes = bPath->getPathInfo();

// remove the source node
	nodes.erase(nodes.begin());

// remove the destination node
	nodes.pop_back();

	vector<string> subLinks;

	for (unsigned int i = 1; i < nodes.size(); ++i) {

		string subLink_FromTo = CommonFunctions::makeLinkId(nodes.at(i - 1),
				nodes.at(i));

		string subLink_ToFrom = CommonFunctions::makeLinkId(nodes.at(i),
				nodes.at(i - 1));

		subLinks.push_back(subLink_FromTo);
		subLinks.push_back(subLink_ToFrom);
	}

	/*
	 * the link weight is a  parameter used calcuate
	 */
	map<string, double> graphLinksInfo;
	for (map<string, SubNwLink*>::iterator slIter =
			embedCheckSubNW->getSubLinks().begin();
			slIter != embedCheckSubNW->getSubLinks().end(); ++slIter) {

// for edge disjoint-path
		if (find(subLinks.begin(), subLinks.end(), slIter->first)
				!= subLinks.end()) {
			continue;
		}

		if (slIter->second->getSubLinkEmbeddableBandwidth() >= bandwidth) {
			graphLinksInfo.insert(
					pair<string, double>(slIter->first,
							slIter->second->getSubLinkWeight()));
		}
	}

	map<string, BasePath*> kPaths = this->kShortPaths->getKShortestpaths(
			bPath->getPathSource(), bPath->getPathDestination(),
			graphLinksInfo);

	MessageSet::printInfoMessage(
			"VirtualNetworkEmbedding::getEdgeDisjointPathWithComplexConstraintsOnSubstrateNetwork End");
	return kPaths;
}

vector<BasePath*> VirtualNetworkEmbedding::getTwoDisjointPathsWithComplexConstraintsOnSubstrateNetwork(
		SubNwGraph * embedCheckSubNW, const string source,
		const string destination, double virLinkBandwidth) {

	MessageSet::printInfoMessage(
			" VirtualNetworkEmbedding::getTwoDisjointPathsWithComplexConstraintsOnSubstrateNetwork Start. ");

	vector<BasePath*> kBasePath;

// one virtual link mapped to multiple substrate transport path, splitting virtual link bandwidth for check
	vector<double> vlSubPathBandwidth;
	vlSubPathBandwidth.push_back(
			virLinkBandwidth
					* CommonConstants::STATIC_VIRTUAL_LINK_RESOURCE_ALLOCATION_SPLITTING_RATIO);
	vlSubPathBandwidth.push_back(
			virLinkBandwidth
					- (virLinkBandwidth
							* CommonConstants::STATIC_VIRTUAL_LINK_RESOURCE_ALLOCATION_SPLITTING_RATIO));
// process bigger bandwidth path firstly.
	if (vlSubPathBandwidth[0] < vlSubPathBandwidth[1]) {
		double bw = vlSubPathBandwidth[1];
		vlSubPathBandwidth[1] = vlSubPathBandwidth[0];
		vlSubPathBandwidth[0] = bw;
	}

//default two substrate paths
	int pathAmount = 2;

	/*
	 * the link weight and bandwidth is a  parameter used calculate
	 */
	map<string, double> graphLinksInfo;
	for (map<string, SubNwLink*>::iterator slIter =
			embedCheckSubNW->getSubLinks().begin();
			slIter != embedCheckSubNW->getSubLinks().end(); ++slIter) {

		if (slIter->second->getSubLinkEmbeddableBandwidth()
				>= vlSubPathBandwidth[0]) {
			graphLinksInfo.insert(
					pair<string, double>(slIter->first,
							slIter->second->getSubLinkWeight()));
		}
	}

//	map<string, BasePath*> kPaths_First = this->kShortPaths->getKShortestpaths(
//			source, destination, graphLinksInfo);
	map<string, BasePath*> kPaths_First =
			this->kShortPaths->getKShortestpathsWithHopLimit(source,
					destination, graphLinksInfo, 0, 10);

//	// hops maximum 10~1
//	map<string, BasePath*> paths_Temp;
//	for (map<string, BasePath*>::iterator kfIter = kPaths_First.begin();
//			kfIter != kPaths_First.end(); ++kfIter) {
//		if (kfIter->second->getPathInfo().size() <= 10) {
//			paths_Temp.insert(
//					pair<string, BasePath*>(kfIter->first, kfIter->second));
//		}
//	}
//
//	if (paths_Temp.size() > 0) {
//		kPaths_First.clear();
//		kPaths_First = paths_Temp;
//	}
//	// hops maximum 10~1

	cout << " step 1 " << endl;
	map<string, BasePath*> kPaths;
	if (kPaths_First.size() == 0 or kPaths_First.size() == 1) {
		// if it is impossiable for two substrate paths, then change to get one substrate path.
		cout
				<< "It is impossiable for two substrate paths, then change to get one substrate path."
				<< endl;
		graphLinksInfo.clear();
		for (map<string, SubNwLink*>::iterator slIter =
				embedCheckSubNW->getSubLinks().begin();
				slIter != embedCheckSubNW->getSubLinks().end(); ++slIter) {

			if (slIter->second->getSubLinkEmbeddableBandwidth()
					>= virLinkBandwidth) {
				graphLinksInfo.insert(
						pair<string, double>(slIter->first,
								slIter->second->getSubLinkWeight()));
			}
		}

		kPaths = this->kShortPaths->getKShortestpathsWithHopLimit(source,
				destination, graphLinksInfo, 0, 10);

		if (kPaths.size() == 0) {
			return kBasePath;
		}

		// hops maximum 10~1
		map<string, BasePath*> paths_Temp;
		for (map<string, BasePath*>::iterator kfIter = kPaths.begin();
				kfIter != kPaths.end(); ++kfIter) {
			if (kfIter->second->getPathInfo().size() <= 10) {
				paths_Temp.insert(
						pair<string, BasePath*>(kfIter->first, kfIter->second));
			}
		}

		if (paths_Temp.size() > 0) {
			kPaths.clear();
			kPaths = paths_Temp;
		} else {
			return kBasePath;
		}
		// hops maximum 10~1

		pathAmount = 1;
	}

	cout << " step 2 " << endl;

// one substrate transport path
	if (pathAmount == 1) {

		cout << " step 3 " << endl;
		/*
		 * sorted order increasing by hops
		 */
		multimap<int, BasePath*> paths;
		for (map<string, BasePath*>::iterator kIter = kPaths.begin();
				kIter != kPaths.end(); ++kIter) {

			kIter->second->setPathBandwidth(virLinkBandwidth);
			kIter->second->setPathAvailableBandwidth(virLinkBandwidth);
			paths.insert(
					pair<int, BasePath*>(kIter->second->getPathHops(),
							kIter->second));
		}

		// the maximum hops path as the substrate transport path of virtual link.
		multimap<int, BasePath*>::reverse_iterator pIter = paths.rbegin();

		for (unsigned int i = 1; i < pIter->second->getPathInfo().size(); ++i) {
			string subLink_FromTo = CommonFunctions::makeLinkId(
					pIter->second->getPathInfo().at(i - 1),
					pIter->second->getPathInfo().at(i));

			string subLink_ToFrom = CommonFunctions::makeLinkId(
					pIter->second->getPathInfo().at(i),
					pIter->second->getPathInfo().at(i - 1));

			embedCheckSubNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
					embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
							- pIter->second->getPathBandwidth());

			embedCheckSubNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
					embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
							- pIter->second->getPathBandwidth());
		}

		kBasePath.push_back(pIter->second);

	} else { //  two substrate tranpsort pathes
		cout << " step 5 " << endl;

		multimap<int, BasePath*> paths_First;

		// get disjoint substrate paths.
		for (map<string, BasePath*>::iterator kfIter = kPaths_First.begin();
				kfIter != kPaths_First.end(); ++kfIter) {
			kfIter->second->setPathBandwidth(vlSubPathBandwidth[0]);
			kfIter->second->setPathAvailableBandwidth(vlSubPathBandwidth[0]);

			paths_First.insert(
					pair<int, BasePath*>(kfIter->second->getPathHops(),
							kfIter->second));
		}

		cout << " step 6 " << endl;

		for (multimap<int, BasePath*>::reverse_iterator pfIter =
				paths_First.rbegin(); pfIter != paths_First.rend(); ++pfIter) {

			vector<string> nodes_Frist = pfIter->second->getPathInfo();

			// remove the source node
			nodes_Frist.erase(nodes_Frist.begin());

			// remove the destination node
			nodes_Frist.pop_back();

			cout << " step 7 " << endl;
			graphLinksInfo.clear();
			for (map<string, SubNwLink*>::iterator slIter =
					embedCheckSubNW->getSubLinks().begin();
					slIter != embedCheckSubNW->getSubLinks().end(); ++slIter) {

				if (slIter->second->getSubLinkEmbeddableBandwidth()
						>= vlSubPathBandwidth[1]
						&& find(nodes_Frist.begin(), nodes_Frist.end(),
								slIter->second->getSubLinkFrom())
								== nodes_Frist.end()
						&& find(nodes_Frist.begin(), nodes_Frist.end(),
								slIter->second->getSubLinkTo())
								== nodes_Frist.end()) {
					graphLinksInfo.insert(
							pair<string, double>(slIter->first,
									slIter->second->getSubLinkWeight()));
				}
			}

			cout << " step 8 " << endl;
			map<string, BasePath*> kPaths_Second =
					this->kShortPaths->getKShortestpathsWithHopLimit(source, destination,
							graphLinksInfo,0, 10);

			kPaths.clear();

			if (kPaths_Second.size() == 0) {
				// if it is impossiable for two substrate paths, then change to get one substrate path.
				cout
						<< "It is impossiable for two substrate paths, then change to get one substrate path."
						<< endl;
				graphLinksInfo.clear();
				for (map<string, SubNwLink*>::iterator slIter =
						embedCheckSubNW->getSubLinks().begin();
						slIter != embedCheckSubNW->getSubLinks().end();
						++slIter) {

					if (slIter->second->getSubLinkEmbeddableBandwidth()
							>= virLinkBandwidth) {
						graphLinksInfo.insert(
								pair<string, double>(slIter->first,
										slIter->second->getSubLinkWeight()));
					}
				}

				kPaths = this->kShortPaths->getKShortestpathsWithHopLimit(source,
						destination, graphLinksInfo,0, 10);

				if (kPaths.size() == 0) {
					return kBasePath;
				}

				pathAmount = 1;
			}

			cout << " step 9 " << endl;
			if (pathAmount == 1) {

				cout << " step 10 " << endl;
				/*
				 * sorted order increasing by hops
				 */
				multimap<int, BasePath*> paths;
				for (map<string, BasePath*>::iterator kIter = kPaths.begin();
						kIter != kPaths.end(); ++kIter) {
					kIter->second->setPathBandwidth(virLinkBandwidth);
					kIter->second->setPathAvailableBandwidth(virLinkBandwidth);

					paths.insert(
							pair<int, BasePath*>(kIter->second->getPathHops(),
									kIter->second));
				}

				multimap<int, BasePath*>::reverse_iterator pIter =
						paths.rbegin();

				for (unsigned int i = 1;
						i < pIter->second->getPathInfo().size(); ++i) {
					string subLink_FromTo = CommonFunctions::makeLinkId(
							pIter->second->getPathInfo().at(i - 1),
							pIter->second->getPathInfo().at(i));

					string subLink_ToFrom = CommonFunctions::makeLinkId(
							pIter->second->getPathInfo().at(i),
							pIter->second->getPathInfo().at(i - 1));

					embedCheckSubNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
							embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
									- pIter->second->getPathBandwidth());

					embedCheckSubNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
							embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
									- pIter->second->getPathBandwidth());
				}

				kBasePath.push_back(pIter->second);
				return kBasePath;
			}

			cout << " step 11 " << endl;
			multimap<int, BasePath*> paths_Second;
			for (map<string, BasePath*>::iterator ksIter =
					kPaths_Second.begin(); ksIter != kPaths_Second.end();
					++ksIter) {

				ksIter->second->setPathBandwidth(vlSubPathBandwidth[1]);
				ksIter->second->setPathAvailableBandwidth(
						vlSubPathBandwidth[1]);

				paths_Second.insert(
						pair<int, BasePath*>(ksIter->second->getPathHops(),
								ksIter->second));
			}

			cout << " step 12 " << endl;
			for (unsigned int i = 1; i < pfIter->second->getPathInfo().size();
					++i) {
				string subLink_FromTo = CommonFunctions::makeLinkId(
						pfIter->second->getPathInfo().at(i - 1),
						pfIter->second->getPathInfo().at(i));

				string subLink_ToFrom = CommonFunctions::makeLinkId(
						pfIter->second->getPathInfo().at(i),
						pfIter->second->getPathInfo().at(i - 1));

				embedCheckSubNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
						embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
								- pfIter->second->getPathBandwidth());

				embedCheckSubNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
						embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
								- pfIter->second->getPathBandwidth());
			}
			kBasePath.push_back(pfIter->second);

			cout << " step 13 " << endl;
			multimap<int, BasePath*>::reverse_iterator psIter =
					paths_Second.rbegin();

			for (unsigned int i = 1; i < psIter->second->getPathInfo().size();
					++i) {
				string subLink_FromTo = CommonFunctions::makeLinkId(
						psIter->second->getPathInfo().at(i - 1),
						psIter->second->getPathInfo().at(i));

				string subLink_ToFrom = CommonFunctions::makeLinkId(
						psIter->second->getPathInfo().at(i),
						psIter->second->getPathInfo().at(i - 1));

				embedCheckSubNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
						embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
								- psIter->second->getPathBandwidth());

				embedCheckSubNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
						embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
								- psIter->second->getPathBandwidth());
			}
			kBasePath.push_back(psIter->second);

			cout << " step 14 " << endl;
			break;
		}
	}

	MessageSet::printInfoMessage(
			" VirtualNetworkEmbedding::getTwoDisjointPathsWithComplexConstraintsOnSubstrateNetwork End. ");

	cout << " kBasePath amount : " << kBasePath.size() << endl;

	return kBasePath;
}

/*
 * get not completely substrate transport paths between source and destination nodes for one virtual link
 *    firstBandwidth, and secondBandwidth are the substrate transport pathe's limited bandwidth.
 *
 * return:
 *    currently  not only two pathes, maybe one or null, or more than two.
 *
 */
vector<BasePath*> VirtualNetworkEmbedding::getTwoDifferentPathsWithComplexConstraintsOnSubstrateNetwork(
		SubNwGraph * embedCheckSubNW, const string source,
		const string destination, double virLinkBandwidth) {

	vector<BasePath*> kBasePath;

// one virtual link mapped to multiple substrate transport path, splitting virtual link bandwidth for check
	vector<double> vlSubPathBandwidth;
	vlSubPathBandwidth.push_back(
			virLinkBandwidth
					* CommonConstants::STATIC_VIRTUAL_LINK_RESOURCE_ALLOCATION_SPLITTING_RATIO);
	vlSubPathBandwidth.push_back(
			virLinkBandwidth
					- (virLinkBandwidth
							* CommonConstants::STATIC_VIRTUAL_LINK_RESOURCE_ALLOCATION_SPLITTING_RATIO));
// process bigger bandwidth path firstly.
	if (vlSubPathBandwidth[0] < vlSubPathBandwidth[1]) {
		double bw = vlSubPathBandwidth[1];
		vlSubPathBandwidth[1] = vlSubPathBandwidth[0];
		vlSubPathBandwidth[0] = bw;
	}

//default two substrate paths
	int pathAmount = 2;
	vector<string> firstPathNodes;
	for (unsigned int index = 0; index < vlSubPathBandwidth.size(); ++index) {

		/*
		 * the link weight and bandwidth is a  parameter used calculate
		 */
		map<string, double> graphLinksInfo;
		for (map<string, SubNwLink*>::iterator slIter =
				embedCheckSubNW->getSubLinks().begin();
				slIter != embedCheckSubNW->getSubLinks().end(); ++slIter) {

			if (slIter->second->getSubLinkEmbeddableBandwidth()
					>= vlSubPathBandwidth[index]) {
				graphLinksInfo.insert(
						pair<string, double>(slIter->first,
								slIter->second->getSubLinkWeight()));
			}
		}

		map<string, BasePath*> kPaths = this->kShortPaths->getKShortestpathsWithHopLimit(
				source, destination, graphLinksInfo, 0, 10);

		if (kPaths.size() == 0) {
			// if it is impossiable for two substrate paths, then change to get one substrate path.
			cout
					<< "It is impossiable for two substrate paths, then change to get one substrate path."
					<< endl;
			graphLinksInfo.clear();
			for (map<string, SubNwLink*>::iterator slIter =
					embedCheckSubNW->getSubLinks().begin();
					slIter != embedCheckSubNW->getSubLinks().end(); ++slIter) {

				if (slIter->second->getSubLinkEmbeddableBandwidth()
						>= virLinkBandwidth) {
					graphLinksInfo.insert(
							pair<string, double>(slIter->first,
									slIter->second->getSubLinkWeight()));
				}
			}

			kPaths = this->kShortPaths->getKShortestpathsWithHopLimit(source, destination,
					graphLinksInfo, 0, 10);

			if (kPaths.size() == 0) {
				break;
			}

			pathAmount = 1;
		}

		/*
		 * sorted order increasing by hops
		 */
		multimap<int, BasePath*> paths;
		for (map<string, BasePath*>::iterator kIter = kPaths.begin();
				kIter != kPaths.end(); ++kIter) {
			// for different path
			if (kBasePath.size() > 0 && kPaths.size() > 1
					&& kBasePath.at(0)->getPathInfo()
							== kIter->second->getPathInfo()) {
				continue;
			}

			if (pathAmount == 2) {
				kIter->second->setPathBandwidth(vlSubPathBandwidth[index]);
				kIter->second->setPathAvailableBandwidth(
						vlSubPathBandwidth[index]);
			}

			if (pathAmount == 1) {
				kIter->second->setPathBandwidth(virLinkBandwidth);
				kIter->second->setPathAvailableBandwidth(virLinkBandwidth);
			}

			paths.insert(
					pair<int, BasePath*>(kIter->second->getPathHops(),
							kIter->second));
		}

		multimap<int, BasePath*>::reverse_iterator pIter = paths.rbegin();

		for (unsigned int i = 1; i < pIter->second->getPathInfo().size(); ++i) {
			string subLink_FromTo = CommonFunctions::makeLinkId(
					pIter->second->getPathInfo().at(i - 1),
					pIter->second->getPathInfo().at(i));

			string subLink_ToFrom = CommonFunctions::makeLinkId(
					pIter->second->getPathInfo().at(i),
					pIter->second->getPathInfo().at(i - 1));

			embedCheckSubNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
					embedCheckSubNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
							- pIter->second->getPathBandwidth());

			embedCheckSubNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
					embedCheckSubNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
							- pIter->second->getPathBandwidth());
		}

		kBasePath.push_back(pIter->second);
		if (pathAmount == 1) {
			break;
		}

	} // EndFor two substrate transport path

	cout << " kBasePath amount : " << kBasePath.size() << endl;
	return kBasePath;

}

/*
 * limitations:
 * cost: value < cost
 * hops: value < hops
 * bandwidth: value > bandwidth
 * switchingEnergy: value < switchingEnergy
 * delay: value < delay
 *
 */
BasePath* VirtualNetworkEmbedding::getOptimalShortestPathWithComplexConstraintsOnVirtualNetwork(
		VirNwGraph* vNw, const string source, const string destination,
		map<string, double> graphLinksInfo, const double cost,
		const double bandwidth, const double switchingEnergy,
		const double delay, const int hops) {

	cout << " From virtual node:" << source << " to " << destination << endl;

	map<string, BasePath*> kPaths = this->kShortPaths->getKShortestpaths(source,
			destination, graphLinksInfo);

	cout << " From virtual node:" << source << " to " << destination
			<< " kpath size : " << kPaths.size() << endl;

	bool checkFlag = false;
	for (map<string, BasePath*>::iterator pIter = kPaths.begin();
			pIter != kPaths.end(); ++pIter) {

// the path hops extends constraint hops
		if (hops != 0) {
			if (pIter->second->getPathHops() > hops) {
				continue;
			}
		}

		checkFlag = true;
		double pathDelay = 0.0;
		double pathSwitchingEnergy = 0.0;
		double pathCost = 0.0;

// if there are some constraints
		if (cost != 0.0 || bandwidth != 0.0 || switchingEnergy != 0.0
				|| delay != 0.0) {

			// check each virtual link with constraints
			for (unsigned int i = 1; i < pIter->second->getPathInfo().size();
					++i) {

				string virLink_FromTo = CommonFunctions::makeLinkId(
						pIter->second->getPathInfo().at(i - 1),
						pIter->second->getPathInfo().at(i));

				// check virtual link's bandwidth whether available
				cout << " Virtual link: " << virLink_FromTo
						<< " limited bandwidth: " << bandwidth
						<< "(Mb) virtual link available bandwidth: "
						<< vNw->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
						<< "(Mb)" << endl;

				if (bandwidth != 0.0
						&& vNw->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
								< bandwidth) {
					checkFlag = false;
					break;
				}

				pathDelay = pathDelay
						+ vNw->getVirLink(virLink_FromTo)->getVirLinkDelay();
				// current only consider one-to-one mapping relationship between virtual link and substrate paths
				pathSwitchingEnergy =
						pathSwitchingEnergy
								+ vNw->getVirLink(virLink_FromTo)->getVirLinkSubPaths().at(
										0)->getPathSwitchingEnergy();

				pathCost = pathCost + pIter->second->getPathCost();

				if (switchingEnergy != 0.0
						&& pathSwitchingEnergy > switchingEnergy) {
					checkFlag = false;
					break;
				}

				if (delay != 0.0 && pathDelay > delay) {

					checkFlag = false;
					break;

				}

				if (cost != 0.0 && pathCost > cost) {
					checkFlag = false;
					break;

				}
			}

		}

		if (checkFlag == true) {
			return pIter->second;
		} else {
			MessageSet::printWarningMessage(
					" From virtual node : " + source + " to virtual node :"
							+ destination
							+ ", the optimal path doesn't exist on virtual network ( No="
							+ vNw->getVirNwNo() + "). ");
			return NULL;
		}
	}
}

const multimap<int, string>& VirtualNetworkEmbedding::getCongestionRecords() const {
	return congestionRecords;
}

void VirtualNetworkEmbedding::setCongestionRecords(
		const multimap<int, string>& congestionRecords) {
	this->congestionRecords = congestionRecords;
}

void VirtualNetworkEmbedding::setCongestionRecord(const int timing,
		const string requestId) {
// add record list
	multimap<int, string>::iterator rIter = this->congestionRecords.begin();
	this->congestionRecords.insert(rIter, pair<int, string>(timing, requestId));
}

const multimap<int, EndUserRequest*>& VirtualNetworkEmbedding::getEndUserRequestRecords() const {
	return endUserRequestRecords;
}

void VirtualNetworkEmbedding::setEndUserRequestRecords(
		const multimap<int, EndUserRequest*>& endUserRequestRecords) {
	this->endUserRequestRecords = endUserRequestRecords;
}

/*
 *
 */

/*
 * proposal for dynamic virtual link resource reallocation based on specified virtual link splitting ratio
 *  input:
 *     timing :  current timing
 *     euRequests: the new end user requests [ requestId, pointer]
 */
void VirtualNetworkEmbedding::performVirtualNetworkEnvironmentWithVirLinkDynamicReallocation(
		const int timing, map<string, EndUserRequest*> euRequests) {

// release end user request resource from virtual network and substrate network

	if (this->endUserRequestRecords.size() > 0) {
		pair<multimap<int, EndUserRequest*>::iterator,
				multimap<int, EndUserRequest*>::iterator> endReqIter;
		endReqIter = this->endUserRequestRecords.equal_range(timing);

		cout << " At timing: " << timing
				<< ", these end-user requests finish and release virtual and substrate networks resource: "
				<< endl;

		for (multimap<int, EndUserRequest*>::iterator iter = endReqIter.first;
				iter != endReqIter.second; ++iter) {
			cout << "request Id: " << iter->second->getReqId() << " data rate:"
					<< iter->second->getReqDataRate() << " start timing: "
					<< iter->second->getReqStartTiming() << " duration time: "
					<< iter->second->getReqDurationTimeUnits()
					<< " end timing: " << iter->second->getReqEndTiming()
					<< endl;

			for (map<int, VirNwGraph*>::iterator vNwIter = this->vNws.begin();
					vNwIter != this->vNws.end(); ++vNwIter) {
				cout << " Virtual network (" << vNwIter->second->getVirNwNo()
						<< ")'s request:" << iter->second->getReqId() << " VM: "
						<< iter->second->getReqVirNwNo() << endl;
				if (vNwIter->second->getVirNwNo()
						== iter->second->getReqVirNwNo()) {
					cout << " Virtual network ("
							<< vNwIter->second->getVirNwNo() << ")'s request:"
							<< iter->second->getReqId() << " VM: "
							<< iter->second->getReqVirNwNo()
							<< " virtual links : "
							<< iter->second->getReqVirPathLinks().size()
							<< endl;

					for (vector<pair<string, double>>::iterator vlIter =
							iter->second->getReqVirPathLinks().begin();
							vlIter != iter->second->getReqVirPathLinks().end();
							++vlIter) {
						cout << " virtual network (" << vNwIter->first
								<< ")'s virtual link (" << (*vlIter).first
								<< "("
								<< vNwIter->second->getVirLink((*vlIter).first)->getVirLinkBandwidth()
								<< " Mb)" << ")'s available bandwidth ("
								<< vNwIter->second->getVirLink((*vlIter).first)->getVirLinkAvailableBandwidth()
								<< " Mb) will add bandwidth ("
								<< (*vlIter).second << "Mb)" << endl;

						vNwIter->second->getVirLink((*vlIter).first)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink((*vlIter).first)->getVirLinkAvailableBandwidth()
										+ (*vlIter).second);

						cout << " virtual network (" << vNwIter->first
								<< ")'s virtual link (" << (*vlIter).first
								<< "("
								<< vNwIter->second->getVirLink((*vlIter).first)->getVirLinkBandwidth()
								<< " Mb)'s available bandwidth ("
								<< vNwIter->second->getVirLink((*vlIter).first)->getVirLinkAvailableBandwidth()
								<< " Mb)." << endl;

					}

					for (vector<pair<string, double>>::iterator slIter =
							iter->second->getReqSubPathsLinks().begin();
							slIter != iter->second->getReqSubPathsLinks().end();
							++slIter) {

						cout << " substrate network 's substrate link ("
								<< (*slIter).first << "("
								<< this->subNW->getSubLink((*slIter).first)->getSubLinkBandwidth()
								<< " Mb)" << ")'s available bandwidth ("
								<< this->subNW->getSubLink((*slIter).first)->getSubLinkAvailableBandwidth()
								<< " Mb) will add bandwidth ("
								<< (*slIter).second << "Mb)" << endl;

						this->subNW->getSubLink((*slIter).first)->setSubLinkAvailableBandwidth(
								this->subNW->getSubLink((*slIter).first)->getSubLinkAvailableBandwidth()
										+ (*slIter).second);

						cout << " substrate network 's substrate link ("
								<< slIter->first << "("
								<< this->subNW->getSubLink((*slIter).first)->getSubLinkBandwidth()
								<< " Mb)" << ")'s available bandwidth ("
								<< this->subNW->getSubLink((*slIter).first)->getSubLinkAvailableBandwidth()
								<< " Mb)" << endl;

					}

				}
			}

		}
		cout << " end user request records amount before: "
				<< this->endUserRequestRecords.size() << endl;
		this->endUserRequestRecords.erase(
				this->endUserRequestRecords.find(timing));
		cout << " end user request records amount after: "
				<< this->endUserRequestRecords.size() << endl;
	}

//release virtual network resource based on the ending timing of virtual network
// delete virtual network pointer;
	MessageSet::printInfoMessage(
			" Current, there are "
					+ CommonFunctions::CoverIntegerToString(this->vNws.size())
					+ " Virtual Networks embedded on Substrate network.");

	for (map<int, VirNwGraph*>::iterator vnwIter = this->vNws.begin();
			vnwIter != this->vNws.end();) {
		if (vnwIter->second->getVirNwEndTiming() == timing) {

			MessageSet::printInfoMessage(
					" Virtual network (" + vnwIter->second->getVirNwNo()
							+ ") will be release.");

			for (map<string, VirNwLink*>::iterator vlIter =
					vnwIter->second->getVirLinks().begin();
					vlIter != vnwIter->second->getVirLinks().end(); ++vlIter) {

				VirNwLink::printVirNwLinkInfo(vlIter->second);

				cout << " substrate path amount : "
						<< vlIter->second->getVirLinkSubPaths().size() << endl;
				// one virtual link may be mapped into some substrate paths
				int index = 0;
				for (vector<BasePath*>::iterator bpIter =
						vlIter->second->getVirLinkSubPaths().begin();
						bpIter != vlIter->second->getVirLinkSubPaths().end();
						++bpIter) {

					cout << ++index << endl;
					cout << " *****" << endl;
					double bandwidth = (*bpIter)->getPathBandwidth();
					cout << " path ID:  " << (*bpIter)->getPathId() << endl;

					//release each substrate link resource
					for (unsigned int i = 1;
							i < (*bpIter)->getPathInfo().size(); ++i) {
						cout << " index: " << i << " substrate path: "
								<< (*bpIter)->getPathId() << endl;
						string subLink_FromTo = CommonFunctions::makeLinkId(
								(*bpIter)->getPathInfo().at(i - 1),
								(*bpIter)->getPathInfo().at(i));

						string subLink_ToFrom = CommonFunctions::makeLinkId(
								(*bpIter)->getPathInfo().at(i),
								(*bpIter)->getPathInfo().at(i - 1));

						SubNwLink::printSubLinkInfo(
								this->subNW->getSubLink(subLink_FromTo));
						this->subNW->getSubLink(subLink_FromTo)->setSubLinkAvailableBandwidth(
								this->subNW->getSubLink(subLink_FromTo)->getSubLinkAvailableBandwidth()
										+ bandwidth);
						this->subNW->getSubLink(subLink_FromTo)->setSubLinkEmbeddableBandwidth(
								this->subNW->getSubLink(subLink_FromTo)->getSubLinkEmbeddableBandwidth()
										+ bandwidth);

						SubNwLink::printSubLinkInfo(
								this->subNW->getSubLink(subLink_FromTo));
						//this->subNW->getSubLink(subLink_ToFrom)->setSubLinkMappedVirLinks();

						SubNwLink::printSubLinkInfo(
								this->subNW->getSubLink(subLink_ToFrom));
						this->subNW->getSubLink(subLink_ToFrom)->setSubLinkAvailableBandwidth(
								this->subNW->getSubLink(subLink_ToFrom)->getSubLinkAvailableBandwidth()
										+ bandwidth);

						this->subNW->getSubLink(subLink_ToFrom)->setSubLinkEmbeddableBandwidth(
								this->subNW->getSubLink(subLink_ToFrom)->getSubLinkEmbeddableBandwidth()
										+ bandwidth);

						SubNwLink::printSubLinkInfo(
								this->subNW->getSubLink(subLink_ToFrom));
						//this->subNW->getSubLink(subLink_ToFrom)->setSubLinkMappedVirLinks();
					}

					// delete virtual link mapped substate link pointer
					MessageSet::printInfoMessage(
							" Virtual link(" + vlIter->second->getVirLinkId()
									+ ")'s substrate path ("
									+ (*bpIter)->getPathId()
									+ ") will be released. ");
					delete (*bpIter);
					cout << "----" << endl;
				}
				vlIter->second->getVirLinkSubPaths().clear();
				cout << " substrate path amount : "
						<< vlIter->second->getVirLinkSubPaths().size() << endl;
				// delete virtual node pointer
				MessageSet::printInfoMessage(
						" Virtual link (" + vlIter->second->getVirLinkId()
								+ ") will be released. ");
				delete vlIter->second;
			}
			vnwIter->second->getVirLinks().clear();

			//release substrate node resource and virNode memory and delete pointer
			for (map<string, VirNwNode*>::iterator vnIter =
					vnwIter->second->getVirNodes().begin();
					vnIter != vnwIter->second->getVirNodes().end(); ++vnIter) {

				VirNwNode::printVirNodeInfo(vnIter->second);

				string subNodeId = vnIter->second->getVirNodeMappedSubNodeId();
				double capapbility = vnIter->second->getVirNodeCapability();

				if (this->subNW->getSubNode(subNodeId) == NULL) {
					MessageSet::printErrorMessage(
							" Substrate Node: " + subNodeId
									+ " is no existence in substrate network.");
					CommonFunctions::FAILURE_EXIT();
				}

				SubNwNode::printSubNodeInfo(this->subNW->getSubNode(subNodeId));

				this->subNW->getSubNode(subNodeId)->setSubNodeAvaiableCapability(
						this->subNW->getSubNode(subNodeId)->getSubNodeAvaiableCapability()
								+ capapbility);

				this->subNW->getSubNode(subNodeId)->setSubNodeEmbeddedAmount(
						this->subNW->getSubNode(subNodeId)->getSubNodeEmbeddedAmount()
								- 1);

				SubNwNode::printSubNodeInfo(this->subNW->getSubNode(subNodeId));

				// delete virtual node pointer
				MessageSet::printInfoMessage(
						" Virtual Node (" + vnIter->second->getVirNodeId()
								+ ") will be released. ");
				delete vnIter->second;
			}
			vnwIter->second->getVirNodes().clear();

			MessageSet::printInfoMessage(
					" Virtual Network (" + vnwIter->second->getVirNwNo()
							+ ") will be released. ");
			VirNwGraph::printVirtualNetworkInfo(vnwIter->second);
			delete vnwIter->second;
			this->vNws.erase(vnwIter);

			vnwIter = this->vNws.begin();
			continue;
		}
		++vnwIter;
	}

	MessageSet::printInfoMessage(
			" Current, there are "
					+ CommonFunctions::CoverIntegerToString(this->vNws.size())
					+ " Virtual Networks embedded on Substrate network.");
//
	for (map<string, EndUserRequest*>::iterator euIter = euRequests.begin();
			euIter != euRequests.end(); ++euIter) {

		for (map<int, VirNwGraph*>::iterator vNwIter = this->vNws.begin();
				vNwIter != this->vNws.end(); ++vNwIter) {

			if (euIter->second->getReqVirNwNo()
					== vNwIter->second->getVirNwNo()) {

				// calculate data transport path on virtual network layer
				string vSourceNode = euIter->second->getReqSourceVirNode();
				string vDestinationNode =
						euIter->second->getReqDestinationVirNode();

				double dataRate = euIter->second->getReqDataRate();

				/*
				 * the link weight is a  parameter used calcuate
				 */
				map<string, double> graphLinksInfo;
				for (map<string, VirNwLink*>::iterator vlIter =
						vNwIter->second->getVirLinks().begin();
						vlIter != vNwIter->second->getVirLinks().end();
						++vlIter) {

					graphLinksInfo.insert(
							pair<string, double>(vlIter->first,
									vlIter->second->getVirLinkWeight()));
				}

				BasePath* virPath =
						getOptimalShortestPathWithComplexConstraintsOnVirtualNetwork(
								vNwIter->second, vSourceNode, vDestinationNode,
								graphLinksInfo, 0.0, dataRate, 0.0, 0.0, 0);

				if (virPath != NULL) {
					cout << " virtual path from " << vSourceNode << " to "
							<< vDestinationNode << " length: "
							<< virPath->getPathInfo().size() << endl;

					CommonFunctions::printVector(virPath->getPathInfo());

					for (unsigned int i = 1; i < virPath->getPathInfo().size();
							++i) {
						// update virtual links bandwidth
						string virLink_FromTo = CommonFunctions::makeLinkId(
								virPath->getPathInfo().at(i - 1),
								virPath->getPathInfo().at(i));

						string virLink_ToFrom = CommonFunctions::makeLinkId(
								virPath->getPathInfo().at(i),
								virPath->getPathInfo().at(i - 1));

						cout << " Virtual network (No="
								<< vNwIter->second->getVirNwNo()
								<< ")'s virtual link : " + virLink_FromTo
										+ " 's bandwidth ("
								<< vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
								<< " Mb) will be reduced bandwidth ("
								<< dataRate << "(Mb) for end user request."
								<< endl;

						vNwIter->second->getVirLink(virLink_FromTo)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
										- dataRate);
						cout << " Virtual network (No="
								<< vNwIter->second->getVirNwNo()
								<< ")'s virtual link : " + virLink_FromTo
										+ " 's bandwidth ("
								<< vNwIter->second->getVirLink(virLink_FromTo)->getVirLinkAvailableBandwidth()
								<< " Mb) ." << endl;

						cout << " Virtual network (No="
								<< vNwIter->second->getVirNwNo()
								<< ")'s virtual link : " + virLink_ToFrom
										+ " 's bandwidth ("
								<< vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
								<< " Mb) will be reduced bandwidth ("
								<< dataRate << " Mb) for end user request."
								<< endl;

						vNwIter->second->getVirLink(virLink_ToFrom)->setVirLinkAvailableBandwidth(
								vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
										- dataRate);

						cout << " Virtual network (No="
								<< vNwIter->second->getVirNwNo()
								<< ")'s virtual link : " + virLink_ToFrom
										+ " 's bandwidth ("
								<< vNwIter->second->getVirLink(virLink_ToFrom)->getVirLinkAvailableBandwidth()
								<< " Mb)." << endl;

						euIter->second->addReqVirPathLink(virLink_FromTo,
								dataRate);

						euIter->second->addReqVirPathLink(virLink_ToFrom,
								dataRate);

						//euIter->second->get
						/*
						 *  update substrate links of data transport path of virtual links
						 *  in case of the only one substrate path
						 */

						BasePath* subPath = vNwIter->second->getVirLink(
								virLink_FromTo)->getVirLinkSubPaths().at(0);

						cout << " virtual link " << virLink_FromTo
								<< "'s substrate path:" << endl;

						CommonFunctions::printVector(subPath->getPathInfo());

						for (unsigned int i = 1;
								i < subPath->getPathInfo().size(); ++i) {
							// update substrate links bandwidth
							string subLink_FromTo = CommonFunctions::makeLinkId(
									subPath->getPathInfo().at(i - 1),
									subPath->getPathInfo().at(i));

							string subLink_ToFrom = CommonFunctions::makeLinkId(
									subPath->getPathInfo().at(i),
									subPath->getPathInfo().at(i - 1));

							cout << " Virtual network (No="
									<< vNwIter->second->getVirNwNo()
									<< ")'s virtual link : " + virLink_ToFrom
											+ " 's substrate path link: "
									<< subLink_FromTo
									<< " 's available bandwidth ("
									<< this->subNW->getSubLink(subLink_FromTo)->getSubLinkAvailableBandwidth()
									<< " Mb) will be reduced bandwidth ("
									<< dataRate << "Mb) for end user request."
									<< endl;
							this->subNW->getSubLink(subLink_FromTo)->setSubLinkAvailableBandwidth(
									this->subNW->getSubLink(subLink_FromTo)->getSubLinkAvailableBandwidth()
											- dataRate);

							cout << " Virtual network (No="
									<< vNwIter->second->getVirNwNo()
									<< ")'s virtual link : " + virLink_ToFrom
											+ " 's substrate path link: "
									<< subLink_FromTo
									<< " 's available bandwidth ("
									<< this->subNW->getSubLink(subLink_FromTo)->getSubLinkAvailableBandwidth()
									<< " Mb) ." << endl;

							cout << " Virtual network (No="
									<< vNwIter->second->getVirNwNo()
									<< ")'s virtual link : " + virLink_ToFrom
											+ " 's substrate path link: "
									<< subLink_ToFrom
									<< " 's available bandwidth ("
									<< this->subNW->getSubLink(subLink_ToFrom)->getSubLinkAvailableBandwidth()
									<< " Mb) will be reduced bandwidth ("
									<< dataRate << " Mb) for end user request."
									<< endl;

							this->subNW->getSubLink(subLink_ToFrom)->setSubLinkAvailableBandwidth(
									this->subNW->getSubLink(subLink_ToFrom)->getSubLinkAvailableBandwidth()
											- dataRate);

							cout << " Virtual network (No="
									<< vNwIter->second->getVirNwNo()
									<< ")'s virtual link : " + virLink_ToFrom
											+ " 's substrate path link: "
									<< subLink_ToFrom
									<< " 's available bandwidth ("
									<< this->subNW->getSubLink(subLink_ToFrom)->getSubLinkAvailableBandwidth()
									<< " Mb) ." << endl;

							euIter->second->addReqSubPathLink(subLink_FromTo,
									dataRate);
							euIter->second->addReqSubPathLink(subLink_ToFrom,
									dataRate);
						}

					}

				} else {
					// congestion happened record
					//map< int , int >
					//  int: timing
					//  int: congestion times
					cout << " End User Request Congestion: "
							<< euIter->second->getReqId() << endl;
					setCongestionRecord(timing, euIter->second->getReqId());
				}

			}
		}

	}
}

void VirtualNetworkEmbedding::updateNveDataSwitchingEnergy(const int timing) {
	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::updateNveDataSwitchingEnergy("
					+ CommonFunctions::CoverIntegerToString(timing)
					+ ") start.");

	double switchingEnergy = 0.0;

	for (multimap<int, EndUserRequest*>::iterator reqIter =
			this->endUserRequestRecords.begin();
			reqIter != this->endUserRequestRecords.end(); ++reqIter) {

		if (timing <= reqIter->second->getReqEndTiming()
				&& timing >= reqIter->second->getReqStartTiming()) {

			for (unsigned int index = 0;
					index < reqIter->second->getReqSubPathsLinks().size();
					++index) {
//				cout << " dataswiching :"
//						<< reqIter->second->getReqSubPathsLinks()[index].first
//						<< " "
//						<< reqIter->second->getReqSubPathsLinks()[index].second
//						<< endl;
//				switchingEnergy =
//						switchingEnergy
//								+ this->getSubNw()->getSubLink(
//										reqIter->second->getReqSubPathsLinks()[index].first)->getSubLinkSwitchingEnergy()
//										* ((reqIter->second->getReqSubPathsLinks()[index].second)
//												* 1024.0 * 1024.0 * 8);
				// mbp/s
				switchingEnergy =
						switchingEnergy
								+ this->getSubNw()->getSubLink(
										reqIter->second->getReqSubPathsLinks()[index].first)->getSubLinkSwitchingEnergy()
										* (reqIter->second->getReqSubPathsLinks()[index].second);
			}
		}
	}

	// switching energy unit: njoule/bit
	switchingEnergy = switchingEnergy / 2;
	this->dataSwitchingEnergy.insert(make_pair(timing, switchingEnergy));
	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::updateNveDataSwitchingEnergy("
					+ CommonFunctions::CoverIntegerToString(timing)
					+ ") start.");

}

map<int, double>& VirtualNetworkEmbedding::getDataSwitchingEnergy() {
	return dataSwitchingEnergy;
}

void VirtualNetworkEmbedding::setDataSwitchingEnergy(
		const map<int, double>& dataSwitchingEnergy) {
	this->dataSwitchingEnergy = dataSwitchingEnergy;
}

map<int, int>& VirtualNetworkEmbedding::getAmountVnsRecords() {
	return amountVnsRecords;
}

void VirtualNetworkEmbedding::setAmountVnsRecords(
		const map<int, int>& amountVnsRecords) {
	this->amountVnsRecords = amountVnsRecords;
}

void VirtualNetworkEmbedding::updateAmountOfVnsRecords(const int timing) {
	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::updateAmountOfVnsRecords("
					+ CommonFunctions::CoverIntegerToString(timing)
					+ ") start.");
	map<int, int>::iterator iter = this->amountVnsRecords.begin();
	this->amountVnsRecords.insert(iter,
			make_pair(timing, this->getAmountVNs()));

	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::updateAmountOfVnsRecords("
					+ CommonFunctions::CoverIntegerToString(timing) + ") end.");
}

map<int, int>& VirtualNetworkEmbedding::getCurrentNumberOfVns() {
	return currentNumberOfVns;
}

void VirtualNetworkEmbedding::setCurrentNumberOfVns(
		const map<int, int>& currentNumberOfVns) {
	this->currentNumberOfVns = currentNumberOfVns;
}

void VirtualNetworkEmbedding::updateCurrentNumberOfVns(const int timing) {

	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::updateCurrentNumberOfVns("
					+ CommonFunctions::CoverIntegerToString(timing)
					+ ") start.");

	map<int, int>::iterator iter = this->currentNumberOfVns.begin();
	this->currentNumberOfVns.insert(iter,
			make_pair(timing, this->getNws().size()));

	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::updateCurrentNumberOfVns("
					+ CommonFunctions::CoverIntegerToString(timing) + ") end.");
}

map<int, pair<int, double> >& VirtualNetworkEmbedding::getCurrentRevenueVirLinkRecords() {
	return currentRevenueVirLinkRecords;
}

void VirtualNetworkEmbedding::setCurrentRevenueVirLinkRecords(
		const map<int, pair<int, double> >& currentRevenueVirLinkRecords) {
	this->currentRevenueVirLinkRecords = currentRevenueVirLinkRecords;
}

void VirtualNetworkEmbedding::updateCurrentRevenueVirLinkAndNodeRecords(
		const int timing) {
	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::updateCurrentRevenueVirLinkAndNodeRecords("
					+ CommonFunctions::CoverIntegerToString(timing)
					+ ") start.");

	int amountVirLinks = 0;
	int amountVirNodes = 0;

	double totalEmbeddedBandwidth = 0.0;

	for (map<int, VirNwGraph*>::iterator vnwIter = this->vNws.begin();
			vnwIter != this->vNws.end(); ++vnwIter) {

		for (map<string, VirNwLink*>::iterator vlIter =
				vnwIter->second->getVirLinks().begin();
				vlIter != vnwIter->second->getVirLinks().end(); ++vlIter) {
			amountVirLinks = amountVirLinks + 1;
			totalEmbeddedBandwidth = totalEmbeddedBandwidth
					+ vlIter->second->getVirLinkBandwidth();
		}

		amountVirNodes = amountVirNodes + vnwIter->second->getVirNodes().size();
	}

	// divide value for pair virtual links[a-b][b-a]
	this->currentRevenueVirLinkRecords.insert(
			make_pair(timing,
					make_pair(amountVirLinks / 2, totalEmbeddedBandwidth / 2)));
	CommonFunctions::printInfoMessage(
			" VirtualNetworkEmbedding::updateCurrentRevenueVirLinkAndNodeRecords("
					+ CommonFunctions::CoverIntegerToString(timing) + ") end.");

	map<int, int>::iterator nIter = this->currentRevenueVirNodeRecords.begin();
	this->currentRevenueVirNodeRecords.insert(nIter,
			pair<int, int>(timing, amountVirNodes));

}

const map<int, int>& VirtualNetworkEmbedding::getCurrentRevenueVirNodeRecords() const {
	return currentRevenueVirNodeRecords;
}

void VirtualNetworkEmbedding::setCurrentRevenueVirNodeRecords(
		const map<int, int>& currentRevenueVirNodeRecords) {
	this->currentRevenueVirNodeRecords = currentRevenueVirNodeRecords;
}
