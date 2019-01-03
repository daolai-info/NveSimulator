/*
 * VirNwGraph.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: abean
 */

#include "../graphs/VirNwGraph.h"

using namespace std;

VirNwGraph::VirNwGraph() {
	// TODO Auto-generated constructor stub
	this->sameFlag = 0;
}

VirNwGraph::VirNwGraph(const string fileName) :
		BaseGraph(fileName) {
	// TODO Auto-generated constructor stub
	this->sameFlag = 0;
}

VirNwGraph::~VirNwGraph() {
	// TODO Auto-generated destructor stub
	cout << "   Release VirNwGraph's virtual links resource start. " << endl;
	for (map<string, VirNwLink*>::iterator vlIter = virLinks.begin();
			vlIter != virLinks.end(); ++vlIter) {

		for (vector<BasePath*>::iterator bpIter =
				vlIter->second->getVirLinkSubPaths().begin();
				bpIter != vlIter->second->getVirLinkSubPaths().end();
				++bpIter) {
			delete *bpIter;
		}

		delete vlIter->second;
	}
	cout << "   Release VirNwGraph's virtual links resource end. " << endl;

	cout << "   Release VirNwGraph's virtual nodes resource start. " << endl;
	for (map<string, VirNwNode*>::iterator vnIter = virNodes.begin();
			vnIter != virNodes.end(); ++vnIter) {
		delete vnIter->second;
	}
	cout << "   Release VirNwGraph's virtual nodes resource end. " << endl;
}

map<string, VirNwLink*>& VirNwGraph::getVirLinks() {
	return virLinks;
}

VirNwLink* VirNwGraph::getVirLink(const string virLinkId) {
	map<string, VirNwLink*>::iterator lIter;
	lIter = this->virLinks.find(virLinkId);
	if (lIter != this->virLinks.end()) {
		return lIter->second;
	} else {
		MessageSet::printErrorMessage(
				" The virtual link " + virLinkId
						+ " is no existence in virtual network.");
		return NULL;
	}
}

void VirNwGraph::setVirLinks(const map<string, VirNwLink*>& virLinks) {
	this->virLinks = virLinks;
}

map<string, VirNwNode*>& VirNwGraph::getVirNodes() {
	return virNodes;
}

void VirNwGraph::setVirNodes(const map<string, VirNwNode*>& virNodes) {
	this->virNodes = virNodes;
}

map<string, string>& VirNwGraph::getMappedPhyNodes() {
	return mappedPhyNodes;
}

void VirNwGraph::setMappedPhyNodes(const map<string, string>& mappedPhyNodes) {
	this->mappedPhyNodes = mappedPhyNodes;
}

void VirNwGraph::setMappedPhyNodes(const vector<string>& mappedPhyNodes) {
	map<string, VirNwNode*>::iterator vIter = this->getVirNodes().begin();
	vector<string>::const_iterator pIter = mappedPhyNodes.begin();

	while (vIter != this->getVirNodes().end() && pIter != mappedPhyNodes.end()) {
		// first string : virtual node id   second string: substrate node id
		map<string, string>::iterator mIter = this->getMappedPhyNodes().begin();
		this->getMappedPhyNodes().insert(mIter,
				pair<string, string>(vIter->first, *pIter));
		cout << " virtual node : " << vIter->first
				<< " mapped substrate node : " << *pIter << endl;
		++vIter;
		++pIter;
	}
}

const string& VirNwGraph::getVirNwNo() const {
	return virNwNO;
}

void VirNwGraph::setVirNwNo(const string& virNwNo) {
	virNwNO = virNwNo;
}

VirNwGraph* VirNwGraph::copyVirtualNetwork() {
	VirNwGraph* copyVirNw = new VirNwGraph();

	copyVirNw->setVirNwNo(this->getVirNwNo());
	copyVirNw->setSameFlag(this->getSameFlag());

	copyVirNw->setVirNwDurationTimeUnits(this->getVirNwDurationTimeUnits());
	copyVirNw->setVirNwStartTiming(this->getVirNwStartTiming());
	copyVirNw->setVirNwEndTiming(this->getVirNwEndTiming());

	for (map<string, VirNwLink*>::const_iterator bIter =
			this->getVirLinks().begin(); bIter != this->getVirLinks().end();
			++bIter) {

		VirNwLink* virLink = new VirNwLink();

		virLink->setVirLinkId(bIter->second->getVirLinkId());
		virLink->setVirNwNo(bIter->second->getVirNwNo());

		virLink->setVirLinkFrom(bIter->second->getVirLinkFrom());
		virLink->setVirLinkTo(bIter->second->getVirLinkTo());
		virLink->setVirFromDomain(bIter->second->getVirFromDomain());
		virLink->setVirToDomain(bIter->second->getVirToDomain());

		virLink->setVirLinkSubPathHops(bIter->second->getVirLinkSubPathHops());

		virLink->setVirLinkSwitchingEnergy(
				bIter->second->getVirLinkSwitchingEnergy());

		virLink->setVirLinkWeight(bIter->second->getVirLinkWeight());
		virLink->setVirLinkDelay(bIter->second->getVirLinkDelay());
		virLink->setVirLinkBandwidth(bIter->second->getVirLinkBandwidth());
		virLink->setVirLinkAvailableBandwidth(
				bIter->second->getVirLinkAvailableBandwidth());

		vector<BasePath*> copyVirLinkSubPaths;

		for (vector<BasePath*>::const_iterator sIter =
				bIter->second->getVirLinkSubPaths().begin();
				sIter != bIter->second->getVirLinkSubPaths().end(); ++sIter) {

			BasePath* copyBasePath = new BasePath();

			copyBasePath->setPathId((*sIter)->getPathId());
			copyBasePath->setPathSource((*sIter)->getPathSource());
			copyBasePath->setPathDestination((*sIter)->getPathDestination());
			copyBasePath->setPathBandwidth((*sIter)->getPathBandwidth());
			copyBasePath->setPathAvailableBandwidth(
					(*sIter)->getPathAvailableBandwidth());

			copyBasePath->setPathDistance((*sIter)->getPathDistance());
			copyBasePath->setPathHops((*sIter)->getPathHops());
			copyBasePath->setPathDeley((*sIter)->getPathDeley());
			copyBasePath->setPathSwitchingEnergy(
					(*sIter)->getPathSwitchingEnergy());

			vector<string> copyPathInfo;
			for (vector<string>::const_iterator pIter =
					(*sIter)->getPathInfo().begin();
					pIter != (*sIter)->getPathInfo().end(); ++pIter) {
				copyPathInfo.push_back(*pIter);
			}

			//cout << copyBasePath->getDeley() << endl;
			//cout << copyBasePath->getSwitchingEnergy() << endl;

			copyBasePath->setPathInfo(copyPathInfo);

			copyVirLinkSubPaths.push_back(copyBasePath);

		}

		virLink->setVirLinkSubPaths(copyVirLinkSubPaths);
		map<string, VirNwLink*>::const_iterator virIter =
				copyVirNw->getVirLinks().begin();

		copyVirNw->getVirLinks().insert(virIter,
				pair<string, VirNwLink*>(bIter->first, virLink));

	}

	for (map<string, VirNwNode*>::const_iterator vnIter =
			this->getVirNodes().begin(); vnIter != this->getVirNodes().end();
			++vnIter) {

		VirNwNode* virNode = new VirNwNode();

		virNode->setVirNodeId(vnIter->second->getVirNodeId());
		virNode->setVirNodeCapability(vnIter->second->getVirNodeCapability());
		virNode->setVirNodeMappedSubNodeId(
				vnIter->second->getVirNodeMappedSubNodeId());
		virNode->setVirNwNo(vnIter->second->getVirNwNo());

		map<string, VirNwNode*>::const_iterator virIter =
				copyVirNw->getVirNodes().begin();

		copyVirNw->getVirNodes().insert(virIter,
				pair<string, VirNwNode*>(vnIter->first, virNode));
	}

	copyVirNw->setMappedPhyNodes(this->getMappedPhyNodes());

	return copyVirNw;
}

int VirNwGraph::getVirNwDurationTimeUnits() const {
	return virNwDurationTimeUnits;
}

void VirNwGraph::setVirNwDurationTimeUnits(int virNwDurationTimeUnits) {
	this->virNwDurationTimeUnits = virNwDurationTimeUnits;
}

int VirNwGraph::getVirNwEndTiming() const {
	return virNwEndTiming;
}

void VirNwGraph::setVirNwEndTiming(int virNwEndTiming) {
	this->virNwEndTiming = virNwEndTiming;
}

int VirNwGraph::getVirNwStartTiming() const {
	return virNwStartTiming;
}

void VirNwGraph::setVirNwStartTiming(int virNwStartTiming) {
	this->virNwStartTiming = virNwStartTiming;
}

void VirNwGraph::printVirtualNetworkInfo(VirNwGraph* virNw) {
	cout << " ---- Virtual Network Info  Start ---" << endl;
	cout << " VirNwNo : " << virNw->getVirNwNo() << endl;
	cout << " StartTiming : " << virNw->getVirNwStartTiming() << endl;
	cout << " DurationTime: " << virNw->getVirNwDurationTimeUnits() << endl;
	cout << " EndTiming: " << virNw->getVirNwEndTiming() << endl;
	cout << " Nodes: " << virNw->getVirNodes().size() << endl;

	for (map<string, VirNwNode*>::iterator nIter = virNw->getVirNodes().begin();
			nIter != virNw->getVirNodes().end(); ++nIter) {
		cout << " Virtual Node: " << nIter->second->getVirNodeId()
				<< " Capability: " << nIter->second->getVirNodeCapability()
				<< " MappedSubNode: "
				<< nIter->second->getVirNodeMappedSubNodeId() << " VirNwNo: "
				<< nIter->second->getVirNwNo() << endl;
	}

	cout << " Links: " << virNw->getVirLinks().size() << endl;
	for (map<string, VirNwLink*>::iterator vlIter =
			virNw->getVirLinks().begin(); vlIter != virNw->getVirLinks().end();
			++vlIter) {
		cout << "VirLinkId: " << vlIter->second->getVirLinkId()
				<< " Bandwidth: " << vlIter->second->getVirLinkBandwidth()
				<< " Available Bandwidth: "
				<< vlIter->second->getVirLinkAvailableBandwidth() << endl;
		cout << " Substrate Paths : " << endl;
		for (vector<BasePath*>::iterator bIter =
				vlIter->second->getVirLinkSubPaths().begin();
				bIter != vlIter->second->getVirLinkSubPaths().end(); ++bIter) {
			BasePath::printBasePath(*bIter);
		}
	}

	cout << " ---- Virtual Network Info  End ---" << endl;
}

void VirNwGraph::checkVirtualLinksResource(VirNwGraph* virNw) {

	vector<string> checkedSubLinks;
	for (map<string, VirNwLink*>::iterator vnlIter =
			virNw->getVirLinks().begin(); vnlIter != virNw->getVirLinks().end();
			++vnlIter) {

		vector<string>::iterator bvlIter = find(checkedSubLinks.begin(),
				checkedSubLinks.end(), vnlIter->first);

		if (bvlIter == checkedSubLinks.end()) {

			string subLinkId = CommonFunctions::makeLinkId(
					vnlIter->second->getVirLinkTo(),
					vnlIter->second->getVirLinkFrom());

			map<string, VirNwLink*>::iterator avlIter =
					virNw->getVirLinks().find(subLinkId);
			if (avlIter == virNw->getVirLinks().end()) {
				MessageSet::printErrorMessage(
						" The virtual link [" + avlIter->first
								+ "]'s pair virtual link[" + subLinkId
								+ "] is not exist. Please check this bug.");
				MessageSet::printFailureExitMessage(
						" VirNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw)");
				CommonFunctions::FAILURE_EXIT();
			}

			if (vnlIter->second->getVirLinkBandwidth()
					!= avlIter->second->getVirLinkBandwidth()) {
				MessageSet::printErrorMessage(
						" Virtual link: " + vnlIter->first
								+ "'s specified bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										vnlIter->second->getVirLinkBandwidth())
								+ ") is not equal to virtual link : "
								+ avlIter->first + " 's bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										avlIter->second->getVirLinkBandwidth())
								+ ")");
				MessageSet::printFailureExitMessage(
						" VirNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			if (vnlIter->second->getVirLinkBandwidth() < 0
					|| vnlIter->second->getVirLinkBandwidth()
							> CommonConstants::MAX_VNW_LINK_BANDWIDTH) {
				MessageSet::printErrorMessage(
						" Virtual link: " + vnlIter->first
								+ "'s specified bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										vnlIter->second->getVirLinkBandwidth())
								+ ") is outside the specified resource range "
								+ CommonFunctions::CoverDoubleToString(
										CommonConstants::MAX_VNW_LINK_BANDWIDTH)
								+ ")");
				MessageSet::printFailureExitMessage(
						" VirNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			if (vnlIter->second->getVirLinkAvailableBandwidth()
					!= avlIter->second->getVirLinkAvailableBandwidth()) {
				MessageSet::printErrorMessage(
						" Substrate link: " + vnlIter->first
								+ "'s available bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										vnlIter->second->getVirLinkAvailableBandwidth())
								+ ") is not equal to substrate link : "
								+ avlIter->first + " 's available bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										avlIter->second->getVirLinkAvailableBandwidth())
								+ ")");
				MessageSet::printFailureExitMessage(
						" VirNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			if (vnlIter->second->getVirLinkAvailableBandwidth() < 0
					|| vnlIter->second->getVirLinkAvailableBandwidth()
							> CommonConstants::MAX_VNW_LINK_BANDWIDTH) {
				MessageSet::printErrorMessage(
						" Virtual link: " + vnlIter->first
								+ "'s available bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										vnlIter->second->getVirLinkAvailableBandwidth())
								+ ") is outside the specified resource range "
								+ CommonFunctions::CoverDoubleToString(
										CommonConstants::MAX_VNW_LINK_BANDWIDTH)
								+ ")");
				MessageSet::printFailureExitMessage(
						" VirNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}
			checkedSubLinks.push_back(vnlIter->first);
		}
	}
}

void VirNwGraph::ReleaseVirtualNetworkResource(VirNwGraph* virNw) {
	MessageSet::printInfoMessage(
			" VirNwGraph::ReleaseVirtualNetworkResource Start");

	for (map<string, BaseLink*>::const_iterator bIter =
			virNw->getLinks().begin(); bIter != virNw->getLinks().end();
			++bIter) {
		delete bIter->second;
	}

	for (map<string, BaseNode*>::const_iterator bIter =
			virNw->getNodes().begin(); bIter != virNw->getNodes().end();
			++bIter) {
		delete bIter->second;
	}

	MessageSet::printInfoMessage("Release Virtual Network Successfully.");
	MessageSet::printInfoMessage(
			" VirNwGraph::ReleaseVirtualNetworkResource END");
}
/*
 *  output: 0: different 1: same
 */
int VirNwGraph::compareTwoVirtualNetworks(VirNwGraph* virNw_1,
		VirNwGraph* virNw_2) {

	if (virNw_1->getSameFlag() != virNw_2->getSameFlag()) {
		//CommonFunctions::printWarningMessage("");
		return 0;
	}
	cout << " step 1 " << endl;
//
//	if (virNw_1->getVirNwNo() != virNw_2->getVirNwNo()) {
//		return 0;
//	}

	if (virNw_1->getVirNwStartTiming() != virNw_2->getVirNwStartTiming()) {
		return 0;
	}

	cout << " step 2 " << endl;
	cout << virNw_1->getVirNwDurationTimeUnits() << endl;
	cout << virNw_2->getVirNwDurationTimeUnits() << endl;

	if (virNw_1->getVirNwDurationTimeUnits()
			!= virNw_2->getVirNwDurationTimeUnits()) {
		return 0;
	}

	cout << " step 3 " << endl;

	cout << virNw_1->getVirNwEndTiming() << endl;
	cout << virNw_2->getVirNwEndTiming() << endl;
	if (virNw_1->getVirNwEndTiming() != virNw_2->getVirNwEndTiming()) {
		return 0;
	}

	cout << " step 4 " << endl;

	if (virNw_1->getMappedPhyNodes() != virNw_2->getMappedPhyNodes()) {
		return 0;
	}


	cout << " step 6 " << endl;
	for (map<string, VirNwNode*>::iterator vnIter_1 =
			virNw_1->getVirNodes().begin();
			vnIter_1 != virNw_1->getVirNodes().end(); ++vnIter_1) {
		int flag = 0;
		for (map<string, VirNwNode*>::iterator vnIter_2 =
				virNw_2->getVirNodes().begin();
				vnIter_2 != virNw_1->getVirNodes().end(); ++vnIter_2) {
			if (vnIter_1->second->getVirNodeId()
					== vnIter_2->second->getVirNodeId()) {

				if (vnIter_1->second->getVirNodeCapability()
						!= vnIter_2->second->getVirNodeCapability()) {
					return 0;
				}

				if (vnIter_1->second->getVirNodeMappedSubNodeId()
						!= vnIter_2->second->getVirNodeMappedSubNodeId()) {
					return 0;
				}
				flag = 1;
				break;
			}
		}

		if (flag == 0) {
			return 0;
		}
	}

	cout << " step 7 " << endl;
	for (map<string, VirNwLink*>::iterator vlIter_1 =
			virNw_1->getVirLinks().begin();
			vlIter_1 != virNw_1->getVirLinks().end(); ++vlIter_1) {

		int flag = 0;

		for (map<string, VirNwLink*>::iterator vlIter_2 =
				virNw_2->getVirLinks().begin();
				vlIter_2 != virNw_1->getVirLinks().end(); ++vlIter_2) {

			if (vlIter_1->second->getVirLinkId()
					== vlIter_2->second->getVirLinkId()) {

				if (vlIter_1->second->getVirLinkAvailableBandwidth()
						!= vlIter_2->second->getVirLinkAvailableBandwidth()) {
					return 0;
				}

				cout << " step 8 " << endl;
				if (vlIter_1->second->getVirLinkBandwidth()
						!= vlIter_2->second->getVirLinkBandwidth()) {
					return 0;
				}

				cout << " step 9 " << endl;
//				if (vlIter_1->second->getVirLinkDelay()
//						!= vlIter_2->second->getVirLinkDelay()) {
//					return 0;
//				}

				if (vlIter_1->second->getVirLinkFrom()
						!= vlIter_2->second->getVirLinkFrom()) {
					return 0;
				}

				cout << " step 10 " << endl;
				if (vlIter_1->second->getVirLinkSubPathHops()
						!= vlIter_2->second->getVirLinkSubPathHops()) {
					return 0;
				}

				cout << " step 11 " << endl;
				if (vlIter_1->second->getVirLinkSubPaths().size()
						!= vlIter_2->second->getVirLinkSubPaths().size()) {
					return 0;
				}

				cout << " step 12 " << endl;
				if (vlIter_1->second->getVirLinkSubPaths().size() == 1
						&& vlIter_2->second->getVirLinkSubPaths().size() == 1) {

					if (vlIter_1->second->getVirLinkSubPaths()[0]->getPathInfo()
							!= vlIter_2->second->getVirLinkSubPaths()[0]->getPathInfo()) {
						return 0;
					}
				}

				cout << " step 13 " << endl;
				if (vlIter_1->second->getVirLinkSubPaths().size() == 2
						&& vlIter_2->second->getVirLinkSubPaths().size() == 2) {

					int flag = 0;
					for (vector<BasePath*>::iterator iter_1 =
							vlIter_1->second->getVirLinkSubPaths().begin();
							iter_1 != vlIter_1->second->getVirLinkSubPaths().end();
							++iter_1) {


						for (vector<BasePath*>::iterator iter_2 =
								vlIter_2->second->getVirLinkSubPaths().begin();
								iter_2
										!= vlIter_2->second->getVirLinkSubPaths().end();
								++iter_2) {
							if ((*iter_1)->getPathInfo() == (*iter_2)->getPathInfo()) {
								flag++;
							}
						}

					}

					if (flag != 2) {
						return 0;
					}
				}

				cout << " step 14 " << endl;
				if (vlIter_1->second->getVirLinkSwitchingEnergy()
						!= vlIter_2->second->getVirLinkSwitchingEnergy()) {
					return 0;
				}

				cout << " step 15 " << endl;
				if (vlIter_1->second->getVirLinkTo()
						!= vlIter_2->second->getVirLinkTo()) {
					return 0;
				}

				cout << " step 16 " << endl;
				if (vlIter_1->second->getVirLinkWeight()
						!= vlIter_2->second->getVirLinkWeight()) {
					return 0;
				}

				cout << " step 17 " << endl;
				flag = 1;
				break;
			}
		}

		cout << " step 18 " << endl;
		/*
		 *  is not exist return 0;
		 */
		if (flag == 0) {
			return 0;
		}

		cout << " step 19 " << endl;
	}

	cout << " step 20 " << endl;
	return 1;
}

int VirNwGraph::getSameFlag() const {
	return sameFlag;
}

void VirNwGraph::setSameFlag(int sameFlag) {
	this->sameFlag = sameFlag;
}
