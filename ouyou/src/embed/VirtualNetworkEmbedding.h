/*
 * VirtualNetworkEmbedding.h
 *
 *  Created on: 2016/11/21
 *      Author: zhangshanming
 */

#ifndef EMBED_VIRTUALNETWORKEMBEDDING_H_
#define EMBED_VIRTUALNETWORKEMBEDDING_H_

#include <string>
#include <map>

#include <algorithm>

#include "../graphs/BaseGraph.h"
#include "../graphs/BasePath.h"
#include "../graphs/SubNwGraph.h"
#include "../graphs/VirNwGraph.h"
#include "../generator/RandomGenerator.h"
#include "../algorithms/KShortestPaths.h"

#include "../com/MessageSet.h"
#include "../com/CommonFunctions.h"
#include "../com/CommonConstants.h"
#include "../embed/EndUserRequest.h"

using namespace std;

class VirtualNetworkEmbedding {

public:

	VirtualNetworkEmbedding();
	VirtualNetworkEmbedding(const string subNwGraphFile);
	virtual ~VirtualNetworkEmbedding();

	void constructSubstrateNetwork(const string graphFile);

	VirNwGraph* constructVirtualNetwork(const int timing = 0);

	int getAmountVNs();
	void setAmountVNs(int amountVNs);
	SubNwGraph*& getSubNw();
	void setSubNw(SubNwGraph*& subNw);
	map<int, VirNwGraph*>& getNws();
	void setNws(const map<int, VirNwGraph*>& nws);
	KShortestPaths*& getShortPaths();
	void setShortPaths(KShortestPaths*& shortPaths);

	/*
	 * copy the mapping of virtual networks and  substrate network including resource allocation
	 */
	VirtualNetworkEmbedding* copyVirtualNetworkEmbeddingEnvironment();

	/*
	 * embedding virtual network with virtual link split ratio
	 */
	int EmbeddingVirtualNetworkWithVirtualLinkSplittingRatio(
			VirNwGraph* vNwGraph, const int timing = 0,
			const string pathsType =
					CommonConstants::DIFFERENT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK);

	int InitiateEmbeddingVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
			const int timing = 0,
			const string pathsType =
					CommonConstants::DIFFERENT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK);

	const string& getSubNwGraphFile() const;
	void setSubNwGraphFile(const string& subNwGraphFile);

	/*
	 * generate end user requests at the timing and record them into endUserRequests
	 *    timing:
	 *    requestAmount: amount for each virtual network
	 */
	map<string, EndUserRequest*> generateEndUserVideoRequestsForVirtualNetwork(
			const int timing, const int requestAmount);
	/*
	 *  remove ending user request records and release memories.
	 *    place after performVirtualNetworkEnvironment* and updateNveDataSwitchingEnergy* function.
	 */
	void releaseEndUserVideoRequestsForVirtualNetwork(const int timing);


	/*
	 * splitFlag:  how to divided end user request datarate to each substrate transport path
	 *        splitFlag = 1:1     =>  equal division for the data rate : 50%: 50%
	 *        splitFlag = 1:0     =>  no-equal division.  100%: 0%
	 */
	void performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
			const int timing, map<string, EndUserRequest*> euRequests,
			const string splitFlag);

	/*
	 * get a disjoint path based on original substrate transport path and bandwidth, length constraints
	 */
	BasePath* getOneDisjointPathWithComplexConstraintsOnSubstrateNetwork(
			SubNwGraph * embedCheckSubNW, const BasePath* bPath,
			double bandwidth = 0.0, double bpSwithingEnergy = 0.0);

	map<string, BasePath*> getVertexDisjointPathWithComplexConstraintsOnSubstrateNetwork(
			SubNwGraph * embedCheckSubNW, const BasePath* bPath,
			double bandwidth = 0.0);

	map<string, BasePath*> getEdgeDisjointPathWithComplexConstraintsOnSubstrateNetwork(
			SubNwGraph * embedCheckSubNW, const BasePath* bPath,
			double bandwidth = 0.0);

	vector<BasePath*> getTwoDisjointPathsWithComplexConstraintsOnSubstrateNetwork(
			SubNwGraph * embedCheckSubNW, const string source,
			const string destination, double virLinkBandwidth);
	/*
	 * get two not completely substrate transport paths between source and destination nodes for one virtual link
	 *    firstBandwidth, and secondBandwidth are the substrate transport pathe's limited bandwidth.
	 *
	 */
	vector<BasePath*> getTwoDifferentPathsWithComplexConstraintsOnSubstrateNetwork(
			SubNwGraph * embedCheckSubNW, const string source,
			const string destination, double virLinkBandwidth = 0.0);

	/*
	 * used for calculate optimal shortest path according to some constraint
	 *   constraints including:
	 *     cost(length)
	 *     bandwidth
	 *     switchingEnergy
	 *     delay
	 *   if not take into account in calcuating, set these parameter values to default value 0.0
	 *
	 * map<string, double> graphLinksInfo
	 *    string: link Id
	 *    double: one comstraint, such as link bandwith, switchingEnergy or delay  and so on
	 */
	BasePath* getOptimalShortestPathWithComplexConstraintsOnVirtualNetwork(
			VirNwGraph* vNw, const string source, const string destination,
			map<string, double> graphLinksInfo, const double cost = 0.0,
			const double bandwidth = 0.0, const double switchingEnergy = 0.0,
			const double delay = 0.0, const int hops = 0);

	const multimap<int, string>& getCongestionRecords() const;
	void setCongestionRecords(const multimap<int, string>& congestionRecords);
	/*
	 * add one time congestion for current timing
	 */
	void setCongestionRecord(const int timing, const string requestId);
	const multimap<string, EndUserRequest*>& getCurrentEndUserRequests() const;
	void setCurrentEndUserRequests(
			const multimap<string, EndUserRequest*>& currentEndUserRequests);
	const multimap<int, EndUserRequest*>& getEndUserRequestRecords() const;
	void setEndUserRequestRecords(
			const multimap<int, EndUserRequest*>& endUserRequestRecords);

	void updateNveDataSwitchingEnergy(const int timing);
	map<int, double>& getDataSwitchingEnergy();
	void setDataSwitchingEnergy(const map<int, double>& dataSwitchingEnergy);

	void updateAmountOfVnsRecords(const int timing);
	map<int, int>& getAmountVnsRecords();
	void setAmountVnsRecords(const map<int, int>& amountVnsRecords);

	void updateCurrentNumberOfVns(const int timing);
	map<int, int>& getCurrentNumberOfVns();
	void setCurrentNumberOfVns(const map<int, int>& currentNumberOfVns);

	void updateCurrentRevenueVirLinkAndNodeRecords(const int timing);
	map<int, pair<int, double> >& getCurrentRevenueVirLinkRecords();

	void setCurrentRevenueVirLinkRecords(
			const map<int, pair<int, double> >& currentRevenueVirLinkRecords);
	const map<int, int>& getCurrentRevenueVirNodeRecords() const;
	void setCurrentRevenueVirNodeRecords(
			const map<int, int>& currentRevenueVirNodeRecords);

private:
	int amountVNs;

	map<int, VirNwGraph*> vNws;
	SubNwGraph * subNW;
	string subNwGraphFile;
	KShortestPaths *kShortPaths;

	multimap<int, string> congestionRecords;

	/*
	 * end user request record list
	 *    int:    the end timing of end user request
	 *    EndUserRequest*:  the end user request pointer
	 */
	multimap<int, EndUserRequest*> endUserRequestRecords;

	// int: timing,  int: total embedded virtual network amount;
	map<int, int> amountVnsRecords;

	// int: timing,  int: current available virtual networks;
	map<int, int> currentNumberOfVns;

	// int: timing,  double: switching energy;
	map<int, double> dataSwitchingEnergy;

	//int: timing, pair<int, double>: int: amount of virtual links, double: current total bandwidth of virtual links
	map<int, pair<int, double>> currentRevenueVirLinkRecords;

	//int: timing, int virNodes: int: amount of virtual links, int: current total bandwidth of virtual nodes
	map<int, int> currentRevenueVirNodeRecords;

};

#endif /* EMBED_VIRTUALNETWORKEMBEDDING_H_ */
