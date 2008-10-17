/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef SERVER_LOOPER_H
#include "ServerLooper.h"
#endif

#include "ProtocolConstants.h"
#include "ProtocolErrors.h"
#include "WatchNetEndpoint.h"
#include "MessageFields.h"
#include <iostream>

ServerLooper::ServerLooper(BHandler *owner)
				:	BLooper(),
					m_owner(owner),
					m_currentTrID(1),
					//m_connection(new BNetEndpoint()),
					m_connection(new WatchNetEndpoint("ServerLooper")),					
					m_networkLooper(NULL),					
					m_isConnected(false)
					
{			
}

ServerLooper::~ServerLooper()
{
	//delete all handlers!
	int32 noHandlers = CountHandlers();
	if (noHandlers > 1)
	{
		for (int32 i = 1; i < noHandlers; i++)
		{
			BHandler *handler = HandlerAt(i);
			delete handler;
		}
	}
	//closes connection if there still is one
	m_connection->Close();
	m_networkLooper->Lock();
	m_networkLooper->Quit();
	delete m_connection;
}

void ServerLooper::MessageReceived(BMessage* message)
{	
	int32 noHandlers = CountHandlers();
	if (message->what == ProtocolConstants::K_ADD_COMMAND_MESSAGE)
	{
		//BMessenger msgr(m_networkLooper);
		//msgr.SendMessage(message);
		//add command message to m_outBuffer
		BString commandMsg;
		
		BString command;
		message->FindString(K_COMMAND,&command);
		commandMsg += command;
				
		int32 trID;
		if (message->FindInt32(K_TR_ID,&trID) == B_OK)
		{
			commandMsg << " " << trID;
		}
		
		BString remainingMsg;
		int32 remainingIndex = 0;
		while (message->FindString(K_REMAINING_MSG,remainingIndex,&remainingMsg) == B_OK)
		{
			commandMsg << " " << remainingMsg;
			remainingIndex++;
		}
					
		//add payload message part if present
		int32 payloadSize;
		if (message->FindInt32(K_PAYLOAD_SIZE,&payloadSize) == B_OK)
		{
			BString payloadData;
			if (message->FindString(K_PAYLOAD_DATA,&payloadData) == B_OK)
			{
				commandMsg << " " << payloadSize << "\r\n";
				commandMsg << payloadData;
			}
		}
		else
		{
			//at \r\n to mark the end of the regular command part
			commandMsg << "\r\n";		
		}
				
		//send and receive data via network buffers			
		if (commandMsg.Length() > 0)
		{
			int32 sentBytes = m_connection->Send(commandMsg.String(),commandMsg.Length());
			commandMsg.Remove(0,sentBytes);
			if (sentBytes == -1)
				cout << "error sending bytes" << endl;			
		}	
	}
	else if (message->what == ProtocolConstants::K_ERROR_MESSAGE)
	{
		//an error occured!
	}
	else if (noHandlers > 1)
	{	
		//make sure the message won't be send to the looper main handler again!
		for (int32 i = 1; i < noHandlers; i++)
		{
			BHandler *handler = HandlerAt(i);
			BMessenger msgr(handler);
			msgr.SendMessage(message);
		}
	}	
}

uint32 ServerLooper::getCurrentTrID()
{
	return m_currentTrID;
}

status_t ServerLooper::Connect(const char *address, int port)
{
	//close old connection if there is one
	if (m_isConnected)
	{
		m_connection->Close();
	}
	
	//connect to specified server
	status_t error = B_OK;	
	if ((error = m_connection->InitCheck()) == B_OK)
	{		
		 error = m_connection->Connect(address,port);
		 if (error == B_OK)
		 {	
			m_connection->SetNonBlocking(true);
		 	m_isConnected = true;			
		 	//stop old networklooper
		 	if (m_networkLooper)
		 	{
		 		m_networkLooper->Lock();
				m_networkLooper->Quit();
		 	}	
		 	//construct one for this connection
		 	m_networkLooper = new NetworkLooper(this,m_connection);
			m_networkLooper->Run();
		}
	}		
	return error;
}

/**	This function increments the transaction id used in the
	msn command messages. It also checks if this transaction ID
	is within its legal boundaries, i.e. trID > 0 && trID <= 2^32-1
 */
void ServerLooper::incTrID()
{
	uint32 newTr = m_currentTrID + 1;
	if (newTr > 0 && newTr <= ProtocolConstants::K_MAX_TR_ID)
		m_currentTrID++;
}
