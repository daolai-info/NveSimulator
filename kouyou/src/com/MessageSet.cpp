/*
 * MessageSet.cpp
 *
 *  Created on: Dec 1, 2016
 *      Author: ubean
 */

#include "MessageSet.h"

const string MessageSet::INFO_MESSAGE_HEADER = "INFO : ";
const string MessageSet::ERROR_MESSAGE_HEADER = "ERROR	 : ";
const string MessageSet::WARNING_MESSAGE_HEADER = "WARNING	 : ";
const string MessageSet::FAILURE_EXIT_MESSAGE_HEADER = " -FAILURE EXIT : ";

MessageSet::MessageSet() {
	// TODO Auto-generated constructor stub

}

void MessageSet::printInfoMessage(const string message) {
	cout << endl;
	cout << MessageSet::INFO_MESSAGE_HEADER << message << endl;
}

void MessageSet::printErrorMessage(const string message) {
	cout << endl;
	cerr << MessageSet::ERROR_MESSAGE_HEADER << message << endl;
}

void MessageSet::printWarningMessage(const string message) {
	cout << endl;
	cout << MessageSet::WARNING_MESSAGE_HEADER << message << endl;
}

void MessageSet::printFailureExitMessage(const string message) {
	cout << endl;
	cout << MessageSet::FAILURE_EXIT_MESSAGE_HEADER << message << endl;
}

MessageSet::~MessageSet() {
	// TODO Auto-generated destructor stub
}

