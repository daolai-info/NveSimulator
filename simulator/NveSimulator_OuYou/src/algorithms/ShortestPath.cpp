/*
 * ShortestPath.cpp
 *
 *  Created on: 2016/10/24
 *      Author: zhangshanming
 */

#include "ShortestPath.h"

ShortestPath::ShortestPath() {
	// TODO Auto-generated constructor stub

}

ShortestPath::~ShortestPath() {
	// TODO Auto-generated destructor stub
}

BasePath* ShortestPath::getShortestPathWithDijkstra(const BaseGraph *bGraph,
		const string source, const string destination) {
	map<string, double> distance;
	map<string, string> path;
	map<string, bool> vistedFlags;

	vector<string> adjacencyNodes;

	// initialization distance map
	if (bGraph->getNodes().find(source) != bGraph->getNodes().end()) {
		adjacencyNodes = bGraph->getAdjacencyNodes().find(source)->second;
	} else {
		cout << " The input source node " << source
				<< "is not contained in the Graph. " << endl;
	}

	if (bGraph->getNodes().find(destination) == bGraph->getNodes().end()) {
		cout << " The input destination node " << destination
				<< "is not contained in the Graph. " << endl;
	}

	for (map<string, BaseNode*>::const_iterator iter =
			bGraph->getNodes().begin(); iter != bGraph->getNodes().end();
			++iter) {
		map<string, double>::iterator dIter = distance.begin();
		map<string, string>::iterator pIter = path.begin();

		if (iter->first == source) {
			distance.insert(dIter, pair<string, double>(iter->first, 0.0));
			path.insert(pIter, pair<string, string>(iter->first, source));

		} else if (find(adjacencyNodes.begin(), adjacencyNodes.end(),
				iter->first) != adjacencyNodes.end()) {
			string outLinkId = source + '_' + iter->first;
			string inLinkId = iter->first + '_' + source;

			if (bGraph->getLinks().find(outLinkId)
					!= bGraph->getLinks().end()) {
				distance.insert(dIter,
						pair<string, double>(iter->first,
								bGraph->getLinks().find(outLinkId)->second->getWeight()));
				path.insert(pIter, pair<string, string>(iter->first, source));
			} else if (bGraph->getLinks().find(inLinkId)
					!= bGraph->getLinks().end()) {
				distance.insert(dIter,
						pair<string, double>(iter->first,
								bGraph->getLinks().find(inLinkId)->second->getWeight()));
				path.insert(pIter, pair<string, string>(iter->first, source));
			} else {
				cout
						<< " The link between " + source + " and  "
								+ iter->first + " is not contained in graph !"
						<< endl;
			}
		} else {
			distance.insert(dIter,
					pair<string, double>(iter->first,
							CommonConstants::MAX_PATH_DISTANCE));
			path.insert(pIter, pair<string, string>(iter->first, "*"));
		}
		map<string, bool>::iterator vIter = vistedFlags.begin();
		vistedFlags.insert(vIter, pair<string, bool>(iter->first, false));
	}

	vistedFlags.find(source)->second = true;

	for (int index = 0; index < bGraph->getNodesAmount() - 1; ++index) {
		double minDistance = CommonConstants::MAX_PATH_DISTANCE;
		string nextNode;

		for (map<string, BaseNode*>::const_iterator nIter =
				bGraph->getNodes().begin(); nIter != bGraph->getNodes().end();
				++nIter) {

			map<string, bool>::iterator vIter = vistedFlags.find(nIter->first);
			map<string, double>::iterator dIter = distance.find(nIter->first);

			if (vIter != vistedFlags.end() || dIter != distance.end()) {
				if (vIter->second == false && dIter->second < minDistance) {
					minDistance = dIter->second;
					nextNode = vIter->first;
				}
			} else {
				cout << "The Node " << nIter->first
						<< " is not in vistedFlags map<string, bool> or distance map<string, double>"
						<< endl;
			}
		}

		map<string, bool>::iterator vIter = vistedFlags.find(nextNode);
		if (vIter != vistedFlags.end()) {
			vIter->second = true;
		} else {
			cout << "The next node" << nextNode << " is not existed in Graph"
					<< endl;
		}

		adjacencyNodes = bGraph->getAdjacencyNodes().find(nextNode)->second;

		for (map<string, BaseNode*>::const_iterator nIter =
				bGraph->getNodes().begin(); nIter != bGraph->getNodes().end();
				++nIter) {

			map<string, bool>::iterator vIter = vistedFlags.find(nIter->first);
			map<string, double>::iterator dIter = distance.find(nIter->first);

			if (vIter != vistedFlags.end() || dIter != distance.end()) {
				string linkId = nextNode + '_' + nIter->first;

				double linkDistance = 0.0;
				if (bGraph->getLinks().find(linkId)
						!= bGraph->getLinks().end()) {
					linkDistance =
							bGraph->getLinks().find(linkId)->second->getWeight();
				} else {
					continue;
				}

				if (vIter->second == false
						&& linkDistance + minDistance < dIter->second) {
					dIter->second = linkDistance + minDistance;
					path.find(nIter->first)->second = nextNode;
				}
			} else {
				cout << "The  Node " << nIter->first
						<< " is not in vistedFlags map<string, bool> or distance map<string, double>"
						<< endl;
			}
		}

	}

	vector<string> pathInfo;
	vector<string>::iterator pIter;
	string preNode = destination;
	while (preNode != source) {
		pIter = pathInfo.begin();
		pathInfo.insert(pIter, preNode);

		preNode = path.find(preNode)->second;
	}
	pIter = pathInfo.begin();
	pathInfo.insert(pIter, source);
	double pathDistance = distance.find(destination)->second;
	return new BasePath(BasePath::makePathId(source, destination), source,
			destination, pathDistance, pathInfo);
}

