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

	int EmbeddingVirtualNetwork(VirNwGraph* vNwGraph, const int timing = 0);
	int InitiateEmbeddingVirtualNetworkEnvironment(const int timing = 0);

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
	 * clear the mapping of virtual links  on a virtual network embedding environment
	 * for reallocation of virtual link resource
	 */
	void clearVirtualLinksResourceMappingAndAllocations();

	/*
	 * reallocation virtual network resource based on virtual link splitting ratio
	 */
	void reallocationVirtualLinksResourceBySplittingRatio(
			const string pathsType =
					CommonConstants::DIFFERENT_SUBSTRATE_PATH_TYPE_OF_VIRTUAL_LINK);

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

	void addEndUserVideoRequestIntoRecords(
			map<string, EndUserRequest*> euRequest);
	/*
	 *  remove ending user request records and release memories.
	 *    place after performVirtualNetworkEnvironment* and updateNveDataSwitchingEnergy* function.
	 */
	void releaseEndUserVideoRequestsForVirtualNetwork(const int timing);

	void performVirtualNetworkEnvironment(const int timing,
			map<string, EndUserRequest*> euRequests);

	/*
	 * splitFlag:  how to divided end user request datarate to each substrate transport path
	 *        splitFlag = 1:1     =>  equal division for the data rate : 50%: 50%
	 *        splitFlag = 1:0     =>  no-equal division.  100%: 0%
	 */
	void performVirtualNetworkEnvironmentWithVirtualLinkSplittingRatio(
			const int timing, map<string, EndUserRequest*> euRequests,
			const string splitFlag);
	/*
	 * proposal for dynamic virtual link resource reallocation based on specified virtual link splitting ratio
	 */
	void performVirtualNetworkEnvironmentWithVirLinkDynamicReallocation(
			const int timing, map<string, EndUserRequest*> euRequests);

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

	const map<int, double>& getTransportedDataVolume() const;
	void setTransportedDataVolume(
			const map<int, double>& transportedDataVolume);

	const map<int, int>& getCurrentAvailableEndUserRequests() const;
	void setCurrentAvailableEndUserRequests(
			const map<int, int>& currentEndUserRequests);
	void updateCurrentAvailableEndUserRequests(const int timing,
			const int amountAvailableEndUserRequests);
	/*
	 *  for comparing
	 */
	map<string,string>& getComVirNwNos();
	void setComVirNwNos(const map<string,string>& comVirNwNos);
	void addComVirNwNos(const string virNwNo_1, const string virNwNo_2);

	/*
	 * get com end-user request
	 */
	map<string, EndUserRequest*> getComEndUserRequest(
			map<string, EndUserRequest*> euRequests);

	/*
	 * remove com end-user request
	 */
	map<string, EndUserRequest*> removeEndUserRequestofComVirNw(
			map<string, EndUserRequest*> euRequests);
	const map<int, double>& getComDataSwitchingEnergy() const;
	void setComDataSwitchingEnergy(
			const map<int, double>& comDataSwitchingEnergy);
	const map<int, double>& getComTransportedDataVolume() const;
	void setComTransportedDataVolume(
			const map<int, double>& comTransportedDataVolume);

	const map<int, int>& getCurrentEndingVirNws() const;
	void setCurrentEndingVirNws(const map<int, int>& currentEndingVirNws);
	void updateCurrentEndingVirNwsInfo(const int timing,
			const int amountEndingVirNws);

	const map<int, int>& getCurrentEndingEndUserReqs() const;
	void setCurrentEndingEndUserReqs(
			const map<int, int>& currentEndingEndUserReqs);
	void updateCurrentEndingEndUserReqs(const int timing,
			const int amountEndingEuReqs);

	const map<int, unsigned int>& getCurrentCommonEndUserReqs() const;
	void setCurrentCommonEndUserReqs(
			const map<int, unsigned int>& currentCommonEndUserReqs);
	void updateCurrentCommonEndUserReqs(const int timing,
			const unsigned int currentCommonEndUserReqs);

	const map<int, pair<int, int> >& getCurrentSubLinkResourceInfo() const;
	void setCurrentSubLinkResourceInfo(
			const map<int, pair<int, int> >& currentSubLinkResourceInfo);
	void updateCurrentSubLinkEmbeddingResourceInfo(
			const int timing, const double minBw, const double maxBw) ;

	const map<int, double>& getSameVirNwDataSwitchingEnergy() const;
	void setSameVirNwDataSwitchingEnergy(
			const map<int, double>& sameVirDataSwitchingEnergy);
	void updateSameVirNwDataSwitchingEnergy(const int timing, const double sameVirDataSwitchingEnergy);

	const map<int, double>& getSameVirNwTransportedDataVolume() const;
	void setSameVirNwTransportedDataVolume(
			const map<int, double>& sameVirTransportedDataVolume);
	void updateSameVirNwTransportedDataVolume(const int timing, const double sameVirTransportedDataVolume);


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

	// int: timing,  double: current total transport data(megabit);
	map<int, double> transportedDataVolume;

	// int: timing,  int: current amount of end-user requests;
	map<int, int> currentAvailableEndUserRequests;

	//int: timing, pair<int, double>: int: amount of virtual links, double: current total bandwidth of virtual links
	map<int, pair<int, double>> currentRevenueVirLinkRecords;

	//int: timing, int virNodes: int: amount of virtual links, int: current total bandwidth of virtual nodes
	map<int, int> currentRevenueVirNodeRecords;


	// string nve_1 virNwId, string nve_x virNwId
	map<string, string> comVirNwNos;

	// int: timing,  double: switching energy;
	map<int, double> comDataSwitchingEnergy;

	// int: timing,  double: current total transport data(megabit);
	map<int, double> comTransportedDataVolume;

	// int: timing,  int: current amount of ending virtual networks;
	map<int, int> currentEndingVirNws;

	// int: timing,  int: current amount of ending end-user requests;
	map<int, int> currentEndingEndUserReqs;



	// int: timing,  int: current common amount of ending end-user requests;
	map<int, unsigned int> currentCommonEndUserReqs;

	/*
	 * int: timing,
	 * int: current amount of substrate links(available Bw < 30 M)
	 * int: current amount of substrate links(available Bw < 100 M)
	 */
	map<int, pair<int, int>> currentSubLinkResourceInfo;

	/*
	 *  the same virtual networks swiching energy or data volume
	 *
	 */
	// int: timing,  double: switching energy;
	map<int, double> sameVirNwDataSwitchingEnergy;

	// int: timing,  double: current total transport data(megabit);
	map<int, double> sameVirNwTransportedDataVolume;
};

#endif /* EMBED_VIRTUALNETWORKEMBEDDING_H_ */
