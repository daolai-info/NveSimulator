/*
 * EndUserRequests.h
 *
 *  Created on: Dec 12, 2016
 *      Author: ubean
 */

#ifndef EMBED_ENDUSERREQUEST_H_
#define EMBED_ENDUSERREQUEST_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "../com/MessageSet.h"
#include "../com/CommonFunctions.h"

using namespace std;

/*
 *
 */
class EndUserRequest {
public:
	EndUserRequest();
	virtual ~EndUserRequest();
	double getReqDataRate() const;
	void setReqDataRate(double dataRate);
	int getReqDurationTimeUnits() const;
	void setReqDurationTimeUnits(int durationTimeUnits);
	const string& getReqDestinationVirNode() const;
	void setReqDestinationVirNode(const string& reqDestinationVirNode);
	int getReqEndTiming() const;
	void setReqEndTiming(int reqEndTiming);
	const string& getReqSourceVirNode() const;
	void setReqSourceVirNode(const string& reqSourceVirNode);
	int getReqStartTiming() const;
	void setReqStartTiming(int reqStartTiming);
	string getReqVirNwNo() const;
	void setReqVirNwNo(string virNwNo);
	//format: VNwId_timing_Sequece
	const string& getReqId() const;
	void setReqId(const string& reqId);

	vector<pair<string, double>> getReqSubPathsLinks();
	void setReqSubPathsLinks( vector<pair<string, double>> reqSubPathLinks);
	void addReqSubPathLink( string linkId,  double allocatedDatarate );

	vector<pair<string, double>> getReqVirPathLinks();
	void setReqVirPathLinks( vector<pair<string, double>> reqVirPathLinks);
	void addReqVirPathLink( string linkId,  double allocatedDatarate);

	static void printEndUserRequest(EndUserRequest* euRequest);
	vector<pair<string, double> >& getReqSubPathInfo();
	void setReqSubPathInfo(const vector<pair<string, double> >& reqSubPathInfo);
	void addReqSubPathInfo(string pathId, double allocatedDatarate);
	int getComFlag() const;
	void setComFlag(int comFlag);

	void copyEndUserRequests(EndUserRequest* euRequest);

private:
	int reqStartTiming;
	int reqDurationTimeUnits;
	int reqEndTiming;

	string reqVirNwNo;  // the request belong to virNwNo virtual network;
	string reqSourceVirNode;
	string reqDestinationVirNode;
	string reqId; // request ID format: VNwId_starttiming_Sequece;

	double reqDataRate; //

	/*
	 *  when the end user request is allocated into virtual network and reflected into substrate network,
	 *  the virtual network path and substrate network path information including link Id and allocated  datarate can record into the following two multimaps
	 */

	vector<pair<string, double>> reqVirPathLinks;  //   the virtual links and datarate of virtual data transport path on virtual network  string virtual link ID double: data rate
	vector<pair<string, double>> reqSubPathsLinks;  //   the substrate links of all data transport paths on substrate network string: substrate link id double: data rate

	vector<pair<string, double>> reqSubPathInfo; //  string: substrate transport path Id of virtua link, double allocated datarate

	int com_flag; // default 0, if the request belong to common virtual network, that is set to 1;
};


#endif /* EMBED_ENDUSERREQUEST_H_ */
