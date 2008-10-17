/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef PROTOCOL_ERRORS_H
#define PROTOCOL_ERRORS_H

#include <be/support/String.h>

struct error
{
	uint32				m_errorNumber;
	BString				m_errorMessage;
};

const uint32 K_NUMBER_ERRORS = 64;

const error K_MSN_ERROR_MSGS[] = 
{
	{200, "Invalid syntax"},
	{201, "Invalid parameter"},
	{205, "Invalid principal"},
	{206, "Domain name missing"},
	{207, "Already logged in"},
	{208, "Invalid principal name"},
	{209, "Nickname change illegal"},
	{210, "Principal list full"},
	{215, "Principal already on list"},
	{216, "Principal not on list"},
	{217, "Principal not online"},
	{218, "Already in mode"},
	{219, "Principal is in opposite list"},
	{223, "Too many groups"},
	{224, "Invalid group"},
	{225, "Principal not in group"},
	{229, "Group name too long"},
	{230, "Cannot remove group zero"},
	{231, "Invalid group"},
	{280, "Switchboard failed"},
	{281, "Transfer to switchboard failed"},
	
	{300, "Required field missing"},
	{302, "Not logged in"},
	
	{402, "Error accessing contact list"},
	{403, "Error accessing contact list"},
	
	{500, "Internal server error"},
	{501, "Database server error"},
	{502, "Command disabled"},
	{510, "File operation failed"},
	{511, "Banned"},
	{520, "Memory allocation failed"},
	{540, "Challenge response failed"},
	
	{600, "Server is busy"},
	{601, "Server is unavailable"},
	{602, "Peer nameserver is down"},
	{603, "Database connection failed"},
	{604, "Server is going down"},
	{605, "Server is unavailable"},
	
	{700, "Could not create connection"},
	{710, "Bad CVR parameters sent"},
	{711, "Write is blocking"},
	{712, "Session is overloaded"},
	{713, "Calling too rapidly"},
	{714, "Too many sessions"},
	{715, "Not expected"},
	{717, "Bad friend file"},
	{731, "Not expected"},
	
	{800, "Changing too rapidly"},
	
	{910, "Server too busy"},
	{911, "Server is busy"},
	{912, "Server too busy"},
	{913, "Not allowed when hiding"},
	{914, "Server unavailable"},
	{915, "Server unavailable"},
	{916, "Server unavailable"},
	{917, "Authentication failed"},
	{918, "Server too busy"},
	{919, "Server too busy"},
	{920, "Not accepting new principals"},
	{921, "Server too busy"},
	{922, "Server too busy"},
	{923, "Kids passport without parental consent"},
	{924, "Passport account not yet verified"},
	{928, "Bad ticket"}
};

#endif
