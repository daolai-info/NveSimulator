/*
 * Paths.h
 *
 *  Created on: 2016/10/24
 *      Author: zhangshanming
 */

#ifndef GRAPHS_PATHS_H_
#define GRAPHS_PATHS_H_

#include <map>
#include <vector>
#include "../graphs/BasePath.h"

namespace std {

class Paths: public BasePath {
public:
	Paths();
	virtual ~Paths();

private:
	map< string, vector <string> > paths;
};

} /* namespace std */

#endif /* GRAPHS_PATHS_H_ */
