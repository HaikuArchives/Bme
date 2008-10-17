/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef PING_HANDLER_H
#include "PingHandler.h"
#endif

#include <be/app/Looper.h>
#include <be/app/Messenger.h>
#include <be/app/MessageQueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "constants.h"
#include "ProtocolConstants.h"
#include "ServerLooper.h"
#include "MessageFields.h"
#include <iostream>

PingHandler::PingHandler()
				:	ServerHandler(),
					m_receivedPong(true)
{
	AddFilter(new PingFilter());
	
	BMessenger msgr(this);
	m_pingRunner = new BMessageRunner(msgr, new BMessage(PingHandler::K_PING_MSG),PingHandler::K_PING_TIME);	
}

PingHandler::~PingHandler()
{
	delete m_pingRunner;
}

void PingHandler::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			if (message->FindString(K_COMMAND, &command) == B_OK)
			{
				if (command == NotificationMessages::K_SERVER_PONG)
				{
					//server pong notification, still online
					m_receivedPong = true;
				}	 
				else if(command == NotificationMessages::K_SERVER_PING)
				{
					//server challenge, respond to it
					BString challenge;
					if (message->FindString(K_REMAINING_MSG, 1, &challenge) == B_OK)
					{										
						//find responseHash
						char response[33];
						response[32] = '\0';
cout << "challenge" << challenge.String() << endl;						
						DoMSNP11Challenge(challenge.String(),response);							
cout << "response" << response << endl;						
						//construct response				
						BMessage *pongMessage = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
						pongMessage->AddString(K_COMMAND,NotificationMessages::K_CLIENT_PONG);
						
						pongMessage->AddString(K_REMAINING_MSG,ProtocolConstants::K_MSN7_PRODUCT_ID); 					
						pongMessage->AddInt32(K_PAYLOAD_SIZE,strlen(response));
						pongMessage->AddString(K_PAYLOAD_DATA,response);
											
						//send ping(add to NSLoopers MessageQueue
cout << "send QRY" << endl;
						SendCommandMessageTrID(pongMessage);
					}
					else
					{
						//error
					}
				}
			}	
		}
		break;
		case PingHandler::K_PING_MSG:
		{
			if (m_receivedPong)
			{
				m_receivedPong = false;
				//construct ping message
				BMessage *pingMessage = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
				pingMessage->AddString(K_COMMAND,NotificationMessages::K_CLIENT_PING);
				//send ping(add to NSLoopers MessageQueue
				SendCommandMessage(pingMessage);
			}
			else
			{
				//user disconnected, notify user
				BMessage msg(InterfaceMessages::K_USER_SIGNED_OUT_MSG);
				SendCommandMessage(&msg);
			}			
		}
		break;
	}
}

//code from: http://msnpiki.msnfanatic.com/index.php/MSNP11:Challenges
void PingHandler::DoMSNP11Challenge(const char *szChallenge, char *szOutput) 
{
	//Step 1:The MD5 Hash
	MD5_CTX mdContext;
	MD5_Init(&mdContext);
	MD5_Update(&mdContext,(unsigned char *)szChallenge, strlen(szChallenge));
	MD5_Update(&mdContext,(unsigned char *)(ProtocolConstants::K_MSN7_PRODUCT_KEY).String(), strlen((ProtocolConstants::K_MSN7_PRODUCT_KEY).String()));
	
	unsigned char pMD5Hash[16];
	MD5_Final(pMD5Hash,&mdContext);
	
	int pMD5Parts[4];	
	memcpy(pMD5Parts,pMD5Hash,16);
	
	for (int i = 0; i < 4; i++) 
	{
		pMD5Parts[i] &= 0x7FFFFFFF;
	}
	//Step 2: A new String
	int nchlLen = strlen(szChallenge) + strlen((ProtocolConstants::K_MSN7_PRODUCT_ID).String());
	if (nchlLen % 8 != 0)
		nchlLen += 8 - (nchlLen % 8);
	
	char *chlString = new char[nchlLen];
	memset(chlString, '0', nchlLen);
	memcpy(chlString, szChallenge, strlen(szChallenge));
	memcpy(chlString + strlen(szChallenge), (ProtocolConstants::K_MSN7_PRODUCT_ID).String(), strlen((ProtocolConstants::K_MSN7_PRODUCT_ID).String()));
	int *pchlStringParts = (int*)chlString;
	
	//Step 3: The 64 bit key
	long long nHigh=0;
	long long nLow=0;

	for (int i = 0; i < (nchlLen/4) - 1; i += 2) 
	{
		long long temp = pchlStringParts[i];
		temp = (pMD5Parts[0] * (((0x0E79A9C1 * (long long)pchlStringParts[i]) % 0x7FFFFFFF) + nHigh) + pMD5Parts[1]) % 0x7FFFFFFF;
		
		nHigh = (pMD5Parts[2] * (((long long)pchlStringParts[i+1] + temp) % 0x7FFFFFFF) + pMD5Parts[3]) % 0x7FFFFFFF;
		
		nLow = nLow + nHigh + temp;
	}
	nHigh = (nHigh + pMD5Parts[1]) % 0x7FFFFFFF;
	nLow = (nLow + pMD5Parts[3]) % 0x7FFFFFFF;
	
	delete[] chlString;
	//Step 4: Using the key
	unsigned long *pNewHash = (unsigned long *)pMD5Hash;
	
	pNewHash[0] ^= nHigh;
	pNewHash[1] ^= nLow;
	pNewHash[2] ^= nHigh;
	pNewHash[3] ^= nLow;
	
	//convert resulting hash to hexadecimal?
	char szHexChars[]="0123456789abcdef";
	for (int i = 0; i < 16; i++) 
	{
		szOutput[i*2] = szHexChars[(pMD5Hash[i] >> 4) & 0xF];
		szOutput[(i*2)+1] = szHexChars[pMD5Hash[i] & 0xF];
	}
}

//=========================PingFilter========================================
PingFilter::PingFilter()
				:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE)
{
}

PingFilter::~PingFilter()
{
}

filter_result PingFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result = B_SKIP_MESSAGE;
	//only handle pings and challenges
	if (message->what == ProtocolConstants::K_COMMAND_MSG)
	{
		BString command;
		if (message->FindString(K_COMMAND,&command) == B_OK)
		{
			if ((command == NotificationMessages::K_SERVER_PONG) ||
				(command == NotificationMessages::K_SERVER_PING)
				)
			{
				result = B_DISPATCH_MESSAGE;
			}			
		}
	}
	else if (message->what == PingHandler::K_PING_MSG)
	{
		result = B_DISPATCH_MESSAGE;
	}
	
	return result;	
}
