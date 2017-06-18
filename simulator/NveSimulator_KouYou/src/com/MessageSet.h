/*
 * MessageSet.h
 *
 *  Created on: Dec 1, 2016
 *      Author: ubean
 */

#ifndef COM_MESSAGESET_H_
#define COM_MESSAGESET_H_
#include <string>
#include <iostream>
using namespace std;

class MessageSet {
public:
	static const string INFO_MESSAGE_HEADER;
	static const string ERROR_MESSAGE_HEADER;
	static const string WARNING_MESSAGE_HEADER;
	static const string FAILURE_EXIT_MESSAGE_HEADER;

	static void printInfoMessage( const string message);
	static void printErrorMessage( const string message);
	static void printWarningMessage(const string message);
	static void printFailureExitMessage(const string message);
	MessageSet();
	virtual ~MessageSet();
};

#endif /* COM_MESSAGESET_H_ */
