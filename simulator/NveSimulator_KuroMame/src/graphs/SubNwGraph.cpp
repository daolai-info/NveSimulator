/*
 * SubNwGraph.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: abean
 */

#include "../graphs/SubNwGraph.h"

using namespace std;

SubNwGraph::SubNwGraph() {
	// TODO Auto-generated constructor stub

}

SubNwGraph::SubNwGraph(const string fileName) :
		BaseGraph(fileName) {
	// TODO Auto-generated constructor stub

}

SubNwGraph::~SubNwGraph() {
	cout << "     Release SubNwGraph's SubNwLinks resource start." << endl;
	for (map<string, SubNwLink*>::iterator slIter = subLinks.begin();
			slIter != subLinks.end(); ++slIter) {
		delete slIter->second;
	}
	cout << "     Release SubNwGraph's SubNwLinks resource end." << endl;

	cout << "     Release SubNwGraph's SubNwNodes resource start." << endl;
	for (map<string, SubNwNode*>::iterator snIter = subNodes.begin();
			snIter != subNodes.end(); ++snIter) {
		delete snIter->second;
	}
	cout << "    Release SubNwGraph's SubNwNodes resource end." << endl;
}

map<string, SubNwLink*>& SubNwGraph::getSubLinks() {
	return subLinks;
}

SubNwLink* SubNwGraph::getSubLink(const string linkId) {
	map<string, SubNwLink*>::iterator lIter;
	lIter = this->subLinks.find(linkId);
	if (lIter != this->subLinks.end()) {
		return lIter->second;
	} else {
		MessageSet::printErrorMessage(
				" The substrate link " + linkId
						+ " is no existence in substrate network.");
		return NULL;
	}
}

void SubNwGraph::setSubLinks(const map<string, SubNwLink*>& subLinks) {
	this->subLinks = subLinks;
}

map<string, SubNwNode*>& SubNwGraph::getSubNodes() {
	return subNodes;
}

SubNwNode* SubNwGraph::getSubNode(const string nodeId) {
	map<string, SubNwNode*>::iterator iter = this->subNodes.find(nodeId);

	if (iter != this->subNodes.end()) {
		return iter->second;
	} else {
		MessageSet::printErrorMessage(
				" The substrate node " + nodeId
						+ " is no existence in substrate network.");
		return NULL;
	}
}

void SubNwGraph::setSubNodes(const map<string, SubNwNode*>& subNodes) {
	this->subNodes = subNodes;
}

SubNwGraph* SubNwGraph::copySubstrateNetwork() {
	SubNwGraph * copySubNW = new SubNwGraph();

	map<string, SubNwLink*> copySubLinks;

	for (map<string, SubNwLink*>::iterator subLinkIter =
			this->getSubLinks().begin();
			subLinkIter != this->getSubLinks().end(); ++subLinkIter) {
		map<string, string> subLinkMappedVirLinks;

		SubNwLink* subLink = new SubNwLink();

		subLink->setSubLinkFrom(subLinkIter->second->getSubLinkFrom());
		subLink->setSubLinkTo(subLinkIter->second->getSubLinkTo());
		subLink->setSubLinkFromDomain(
				subLinkIter->second->getSubLinkFromDomain());
		subLink->setSubLinkToDomain(subLinkIter->second->getSubLinkToDomain());

		subLink->setSubLinkWeight(subLinkIter->second->getSubLinkWeight());
		subLink->setSubLinkDelay(subLinkIter->second->getSubLinkDelay());
		subLink->setSubLinkSwitchingEnergy(
				subLinkIter->second->getSubLinkSwitchingEnergy());

		subLink->setSubLinkBandwidth(
				subLinkIter->second->getSubLinkBandwidth());
		subLink->setSubLinkAvailableBandwidth(
				subLinkIter->second->getSubLinkAvailableBandwidth());
		subLink->setSubLinkEmbeddableBandwidth(
				subLinkIter->second->getSubLinkEmbeddableBandwidth());

		subLink->setSubLinkMappedVirLinks(
				subLinkIter->second->getSubLinkMappedVirLinks());

		map<string, SubNwLink*>::iterator iter = copySubLinks.begin();

		copySubLinks.insert(iter,
				pair<string, SubNwLink*>(subLinkIter->first, subLink));
	}

	map<string, SubNwNode*> copySubNodes;
	for (map<string, SubNwNode*>::iterator subNodeIter =
			this->getSubNodes().begin();
			subNodeIter != this->getSubNodes().end(); ++subNodeIter) {

		SubNwNode* subNode = new SubNwNode();
		subNode->setSubNodeId(subNodeIter->second->getSubNodeId());
		subNode->setSubNodeEmbeddedAmount(
				subNodeIter->second->getSubNodeEmbeddedAmount());
		subNode->setSubNodeCapability(
				subNodeIter->second->getSubNodeCapability());
		subNode->setSubNodeAvaiableCapability(
				subNodeIter->second->getSubNodeAvaiableCapability());
		subNode->setSubNodeSwitchingEnergy(
				subNodeIter->second->getSubNodeSwitchingEnergy());

		map<string, SubNwNode*>::iterator iter = copySubNodes.begin();

		copySubNodes.insert(iter,
				pair<string, SubNwNode*>(subNodeIter->first, subNode));
	}

	copySubNW->setSubLinks(copySubLinks);
	copySubNW->setSubNodes(copySubNodes);

	return copySubNW;

}

void SubNwGraph::printSubstrateNetwork(SubNwGraph* subNw) {
	for (map<string, SubNwLink*>::iterator snlIter = subNw->subLinks.begin();
			snlIter != subNw->subLinks.end(); ++snlIter) {
		SubNwLink::printSubLinkInfo(snlIter->second);
	}

	for (map<string, SubNwNode*>::iterator snnIter = subNw->subNodes.begin();
			snnIter != subNw->subNodes.end(); ++snnIter) {
		SubNwNode::printSubNodeInfo(snnIter->second);
	}
}
void SubNwGraph::checkSubstrateLinksResourceStatus(SubNwGraph* subNw) {
	for (map<string, SubNwLink*>::iterator snlIter = subNw->subLinks.begin();
			snlIter != subNw->subLinks.end(); ++snlIter) {
		if (snlIter->second->getSubLinkAvailableBandwidth()
				!= snlIter->second->getSubLinkEmbeddableBandwidth()) {
			SubNwLink::printSubLinkInfo(snlIter->second);
		}
	}
}
void SubNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) {

	vector<string> checkedSubLinks;
	for (map<string, SubNwLink*>::iterator snlIter = subNw->subLinks.begin();
			snlIter != subNw->subLinks.end(); ++snlIter) {

		vector<string>::iterator bslIter = find(checkedSubLinks.begin(),
				checkedSubLinks.end(), snlIter->first);

		if (bslIter == checkedSubLinks.end()) {
			string subLinkId = CommonFunctions::makeLinkId(
					snlIter->second->getSubLinkTo(),
					snlIter->second->getSubLinkFrom());

			map<string, SubNwLink*>::iterator aslIter = subNw->subLinks.find(
					subLinkId);
			if (aslIter == subNw->subLinks.end()) {
				MessageSet::printErrorMessage(
						"The substrate link [" + aslIter->first
								+ "]'s pair substrate link[" + subLinkId
								+ "] is not exist. Please check this bug.");
				MessageSet::printFailureExitMessage(
						"SubNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw)");
				CommonFunctions::FAILURE_EXIT();
			}

			if (snlIter->second->getSubLinkBandwidth()
					!= aslIter->second->getSubLinkBandwidth()) {
				MessageSet::printErrorMessage(
						" Substrate link: " + snlIter->first
								+ "'s specified bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										snlIter->second->getSubLinkBandwidth())
								+ ") is not equal to substrate link : "
								+ aslIter->first + " 's bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										aslIter->second->getSubLinkBandwidth())
								+ ")");
				MessageSet::printFailureExitMessage(
						" SubNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			if (snlIter->second->getSubLinkBandwidth() < 0
					|| snlIter->second->getSubLinkBandwidth()
							> CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH) {
				MessageSet::printErrorMessage(
						" Substrate link: " + snlIter->first
								+ "'s specified bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										snlIter->second->getSubLinkBandwidth())
								+ ") is outside the defined resource range. ");
				MessageSet::printFailureExitMessage(
						" SubNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			if (snlIter->second->getSubLinkAvailableBandwidth()
					!= aslIter->second->getSubLinkAvailableBandwidth()) {
				MessageSet::printErrorMessage(
						" Substrate link: " + snlIter->first
								+ "'s available bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										snlIter->second->getSubLinkAvailableBandwidth())
								+ ") is not equal to substrate link : "
								+ aslIter->first + " 's available bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										aslIter->second->getSubLinkAvailableBandwidth())
								+ ")");
				MessageSet::printFailureExitMessage(
						" SubNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			if (snlIter->second->getSubLinkAvailableBandwidth() < 0
					|| snlIter->second->getSubLinkAvailableBandwidth()
							> CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH) {
				MessageSet::printErrorMessage(
						" Substrate link: " + snlIter->first
								+ "'s available bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										snlIter->second->getSubLinkAvailableBandwidth())
								+ ") is outside the defined resource range. ");
				MessageSet::printFailureExitMessage(
						" SubNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			if (snlIter->second->getSubLinkEmbeddableBandwidth()
					!= aslIter->second->getSubLinkEmbeddableBandwidth()) {
				MessageSet::printErrorMessage(
						" Substrate link: " + snlIter->first
								+ "'s embeddable bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										snlIter->second->getSubLinkEmbeddableBandwidth())
								+ ") is not equal to substrate link : "
								+ aslIter->first + " 's embeddable bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										aslIter->second->getSubLinkEmbeddableBandwidth())
								+ ")");
				MessageSet::printFailureExitMessage(
						" SubNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			if (snlIter->second->getSubLinkEmbeddableBandwidth() < 0
					|| snlIter->second->getSubLinkEmbeddableBandwidth()
							> CommonConstants::DEFAULT_SUBSTRATE_LINK_BANDWIDTH) {
				MessageSet::printErrorMessage(
						" Substrate link: " + snlIter->first
								+ "'s embeddable bandwidth ("
								+ CommonFunctions::CoverDoubleToString(
										snlIter->second->getSubLinkEmbeddableBandwidth())
								+ ") is outside the defined resource range. ");
				MessageSet::printFailureExitMessage(
						" SubNwGraph::checkSubstrateLinksResource(SubNwGraph* subNw) ");
				CommonFunctions::FAILURE_EXIT();
			}

			checkedSubLinks.push_back(snlIter->first);
		}
	}
}
