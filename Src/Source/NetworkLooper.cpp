/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef NETWORK_LOOPER_H
#include "NetworkLooper.h"
#endif

#include "ProtocolConstants.h"
#include "StringList.h"
#include "StringTokenizer.h"
#include "MessageFields.h"
#include <stdlib.h>
#include <iostream>

NetworkLooper::NetworkLooper(BLooper *owner, BNetEndpoint *connection)
					:	BLooper(),
						m_owner(owner),
						m_connection(connection),
						m_sendCommand(false)
						
{
	//spawn a connectionrunner so that every 100 micro seconds commands are
	//sent and received
	BMessenger targetMsgr(this);
	bigtime_t interval = 500000;
	m_connectionRunner = new BMessageRunner(targetMsgr,new BMessage(K_CONNECTION_MSG), interval);	
}

NetworkLooper::~NetworkLooper()
{	
	delete m_connectionRunner;
}

void NetworkLooper::MessageReceived(BMessage *message)
{
	switch (message->what)
	{		
		case K_CONNECTION_MSG:
		{			
			//got the 1536 from Daniel's code, not sure how he got it 
			//is this the msn maximum commandlength?			
			size_t bufSize = 1536;//max packetlength I think
			char temp[bufSize + 1];
			int32 recBytes = m_connection->Receive(temp,bufSize);
			temp[recBytes + 1] = '\0';
			
			/*
			if (recBytes == -1)
				cout << "error receiving bytes" << endl;
			*/	
					
			if (recBytes > 0)
			{				
				m_commandCache.Append(temp,recBytes);
				m_commandCache.Append('\0');
			}
			//find out if new commands in cache
			if (m_commandCache.Length() > 0)//or Length()?
				parseCommandMessage(m_commandCache);				
				
		}
		break;
		default:
			BLooper::MessageReceived(message);
		break;
	}
}

/**	Parses the command code received from the msn server and wraps it
	into a BMessage. Depending on the command received the message can
	have the following structure:
	-Normal command, fields:
		BString commandName
		int32	trID
		BString remainingMsg (with leading/trailing spaces removed)
	-Payload command, fields:
		BString commandName
		BString remainingMsg
		int32 payloadSize
		BString payloadData
	-Asynchronous command, fields:
		BString commandName
		(int32	trID, possible(then trID=0) but not necessary)
		BString remainingMsg
 */
void NetworkLooper::parseCommandMessage(BString& cacheString)
{	
//cout << "cS 1 = " << cacheString.String() << endl;
	BMessage commandMsg(ProtocolConstants::K_COMMAND_MSG);
	//examine cache for newline
	int32 newlineIndex = cacheString.FindFirst("\r\n");	
	if (newlineIndex != B_ERROR)
	{		
		//get commandline from cache
		BString commandLine;
		cacheString.CopyInto(commandLine,0,newlineIndex);
//cout << "cS 2 = " << cacheString.String() << endl;
		//tokenise string
		StringTokenizer tokenizer(commandLine);
		StringList *commandTokens = tokenizer.getTokens();
		//analyse what sort of command it is
		BString command = *commandTokens->ItemAt(0);
		commandMsg.AddString(K_COMMAND,command);
//cout << "command=" << command.String() << endl;
			
		//check if message contains more than command only
		if (commandTokens->CountItems() > 1)
		{		
			if ((command == NotificationMessages::K_NOTICE_MSG) ||
				(command == NotificationMessages::K_NS_PAYLOAD_MSG) ||
				(command == NotificationMessages::K_CONTACT_PERSONAL_MSG)
				)
			{
				//CHECK IF WORKS CORRECTLY!!!
				//incoming payload commands
//cout << "payload command" << endl;
				
				//find payload size						
				BString payloadSizeString = *commandTokens->ItemAt(commandTokens->CountItems()-1);								
				int32 payloadSize = atoi(payloadSizeString.String());
				commandMsg.AddInt32(K_PAYLOAD_SIZE,payloadSize);
//cout << "payload size= " << payloadSize << endl;				
				//modify remaining message for payload commands, so that it doesn't include payload size
				//command at pos 0, rest command starts at 1, payloadsize at last pos commandstring
				for (int32 i = 1; i < commandTokens->CountItems()-1; i++)
				{
					//add remaining command tokens to message
					BString token = *commandTokens->ItemAt(i);
					commandMsg.AddString(K_REMAINING_MSG,token);	
				}
				
				//int32 remainingCacheSize = cacheString.CountChars() - (newlineIndex + 2);
				int32 remainingCacheSize = cacheString.Length() - (newlineIndex + 2);
//cout << "cS= " << cacheString.CountChars() << " rC= " << remainingCacheSize << endl;				
				if (payloadSize > remainingCacheSize)
				{		
//cout << "here" << endl;					
					//handle command later
					return;
				}
				else if (payloadSize > 0)
				{					
					//entire payload is in cache							
					BString payload;
					cacheString.CopyInto(payload,newlineIndex+2,payloadSize);
					commandMsg.AddString(K_PAYLOAD_DATA, payload);
//cout << "payload= " << payload.String() << endl;					
					//send message to ServerLooper
					BMessenger msgr(m_owner);
					msgr.SendMessage(&commandMsg);
					//entire payload message should be removed at end of this method
					newlineIndex += payloadSize; 
				}							
			}
			else if ((command == NotificationMessages::K_PRINCIPAL_CLOSES_PRESENCE) ||
					 (command == NotificationMessages::K_PRINCIPAL_STATUS) ||
					 (command == NotificationMessages::K_SERVER_PING) ||
					 (command == NotificationMessages::K_MOBILE_CREDITS_MSG) ||
					 (command == NotificationMessages::K_CHANGE_DISPLAY_NAME) ||
					 (command == NotificationMessages::K_CONTACT_LIST_MSG) ||
					 (command == NotificationMessages::K_GROUP_LIST_MSG) ||
					 (command == NotificationMessages::K_INVITED_FOR_SWITCHBOARD)
					 )//see if this can be solved differently
					 //check if there's a PRP version with trID also!!!
					 
			{					
				//asynchronous command without trID
//cout << "asynchronous command" << endl;	
				//command at pos 0, rest command starts at 1
				for (int32 i = 1; i < commandTokens->CountItems(); i++)
				{
					//add remaining command tokens to message
					BString token = *commandTokens->ItemAt(i);
					commandMsg.AddString(K_REMAINING_MSG,token);
				}
				//send message to ServerLooper
				BMessenger msgr(m_owner);
				msgr.SendMessage(&commandMsg);						
			}
			else
			{
				//normal command or asynchronous command with trID					
				BString trIDString = *commandTokens->ItemAt(1);										
				int32 trID = atoi(trIDString.String());
				commandMsg.AddInt32(K_TR_ID,trID);
				
				//command at pos 0, trId at 1, rest command starts at 2
				for (int32 i = 2; i < commandTokens->CountItems(); i++)
				{
					//add remaining command tokens to message
					BString token = *commandTokens->ItemAt(i);
					commandMsg.AddString(K_REMAINING_MSG,token);	
				}
				//send message to ServerLooper
				BMessenger msgr(m_owner);
				msgr.SendMessage(&commandMsg);					
			}
		}
		//clean up token-list	
		StringList::deleteStrings(commandTokens);
	}
	else
	{
		//no new command found
		return;
	}	
	//remove handled command from cache
//cout << cacheString.String() << endl;	
	cacheString.Remove(0,newlineIndex + 2);// /r/n
	//loop back, find out if more commands are in cache
	if (cacheString.CountChars() > 0)
		parseCommandMessage(cacheString);	
		
}
