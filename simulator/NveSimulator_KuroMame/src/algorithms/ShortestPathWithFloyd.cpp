/*
 * ShortestPathWithFloyd.cpp
 *
 *  Created on: 2016/10/24
 *      Author: zhangshanming
 */

#include "ShortestPathWithFloyd.h"

ShortestPathWithFloyd::ShortestPathWithFloyd() {
	// TODO Auto-generated constructor stub

}

ShortestPathWithFloyd::ShortestPathWithFloyd(const BaseGraph * bGraph) {
	calculateShortestPathsWithFloyd(bGraph);
}

ShortestPathWithFloyd::~ShortestPathWithFloyd() {
	// TODO Auto-generated destructor stub
}

void ShortestPathWithFloyd::calculateShortestPathsWithFloyd(
		const BaseGraph * bGraph) {
	map<string, double> distance;
	map<string, string> path;

	for (map<string, BaseNode*>::const_iterator outIter =
			bGraph->getNodes().begin(); outIter != bGraph->getNodes().end();
			++outIter) {
		for (map<string, BaseNode*>::const_iterator inIter =
				bGraph->getNodes().begin(); inIter != bGraph->getNodes().end();
				++inIter) {
			map<string, double>::iterator dIter = distance.begin();
			map<string, string>::iterator pIter = path.begin();

			string pathId = BasePath::makePathId(outIter->first, inIter->first);

			double pathDistance = CommonConstants::MAX_PATH_DISTANCE;

			const BaseLink* bLink = bGraph->getLink(pathId);

			if (bLink != NULL) {
				pathDistance = bLink->getWeight();
			}

			if (outIter->first == inIter->first) {
				pathDistance = 0.0;
			}
			distance.insert(dIter, pair<string, double>(pathId, pathDistance));

			path.insert(pIter, pair<string, string>(pathId, inIter->first));
		}
	}

	map<string, BaseNode*>::const_iterator sIter = bGraph->getNodes().begin();
	map<string, BaseNode*>::const_iterator eIter = bGraph->getNodes().end();

	for (map<string, BaseNode*>::const_iterator middleIter = sIter;
			middleIter != eIter; ++middleIter) {
		for (map<string, BaseNode*>::const_iterator fromIter = sIter;
				fromIter != eIter; ++fromIter) {
			for (map<string, BaseNode*>::const_iterator toIter = sIter;
					toIter != eIter; ++toIter) {

				string fromPath = BasePath::makePathId(fromIter->first,
						middleIter->first);
				string toPath = BasePath::makePathId(middleIter->first,
						toIter->first);
				string ftPath = BasePath::makePathId(fromIter->first,
						toIter->first);

				map<string, double>::iterator fIter = distance.find(fromPath);
				map<string, double>::iterator tIter = distance.find(toPath);
				map<string, double>::iterator ftIter = distance.find(ftPath);

				if (ftIter->second > fIter->second + tIter->second) {
					ftIter->second = fIter->second + tIter->second;
					map<string, string>::iterator pftIter = path.find(ftPath);
					pftIter->second = middleIter->first;
				}
			}
		}
	}

	for (map<string, BaseNode*>::const_iterator fromIter =
			bGraph->getNodes().begin(); fromIter != bGraph->getNodes().end();
			++fromIter) {

		for (map<string, BaseNode*>::const_iterator toIter =
				bGraph->getNodes().begin(); toIter != bGraph->getNodes().end();
				++toIter) {
			// the format of pathId is the same as linkId 's
			string pathId = BasePath::makePathId(fromIter->first,
					toIter->first);
			string fromeNode = fromIter->first;
			vector<string> pathInfo;
			pathInfo.push_back(fromeNode);

			getPathInfo(fromIter->first, toIter->first, path, &pathInfo);
//			for (vector<string>::iterator iter = pathInfo.begin();
//					iter != pathInfo.end(); ++iter) {
//				cout << *iter << " ";
//			}

			map<string, string>::iterator pIter = path.find(pathId);

			double distanceAll = 0.0;
			string linkId = "";
			while (pIter->second != toIter->first) {
				linkId = BasePath::makePathId(fromeNode, pIter->second);

				distanceAll = distanceAll + distance.find(linkId)->second;
				fromeNode = pIter->second;
				pIter = path.find(
						BasePath::makePathId(pIter->second, toIter->first));
			}
			linkId = BasePath::makePathId(fromeNode, pIter->second);
			distanceAll = distanceAll + distance.find(linkId)->second;

			BasePath * aPath = new BasePath(pathId, fromIter->first,
					toIter->first, distanceAll, pathInfo);
			map<string, BasePath*>::iterator pathsIter = this->paths.begin();
			this->paths.insert(pathsIter,
					pair<string, BasePath*>(pathId, aPath));
		}
	}
}

void ShortestPathWithFloyd::getPathInfo(const string& source,
		const string& destination, map<string, string> path,
		vector<string> * pIter) {
	vector<string> pathInfo;
	if (source == destination) {
		pIter->push_back(destination);
	} else if (path.find(BasePath::makePathId(source, destination))->second
			== destination) {
		pIter->push_back(destination);
	} else {
		getPathInfo(source,
				path.find(BasePath::makePathId(source, destination))->second,
				path, pIter);
		getPathInfo(
				path.find(BasePath::makePathId(source, destination))->second,
				destination, path, pIter);
	}
}

void ShortestPathWithFloyd::printShortestPath(const string& source,
		const string& destination, map<string, string> path) {
	string pathId = BasePath::makePathId(source, destination);
	if (path.find(pathId)->second == "") {
		cout << source << " " << destination << endl;
	} else {
		printShortestPath(source, path.find(pathId)->second, path);
		printShortestPath(path.find(pathId)->second, destination, path);
	}
}
BasePath* ShortestPathWithFloyd::getShortestPath(const string& source,
		const string& destination) {
	map<string, BasePath*>::iterator pIter = this->paths.find(
			BasePath::makePathId(source, destination));
	if (pIter != this->paths.end()) {
		return pIter->second;
	} else {
		return NULL;
	}
}

