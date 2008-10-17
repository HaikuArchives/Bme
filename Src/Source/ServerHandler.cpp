/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef SERVER_HANDLER_H
#include "ServerHandler.h"
#endif

#include <app/Looper.h>
#include <app/Messenger.h>
#include "ServerLooper.h"
#include "ProtocolConstants.h"
#include "MessageFields.h"
#include <iostream>

/**	Constructor
 */
ServerHandler::ServerHandler()
					:	BHandler()
{
}

/**	Destructor
 */
ServerHandler::~ServerHandler()
{
}

/**	Sends a command message without trID to the handlers owner,
	the handler's owner must be subtype of ServerLooper.
	@param the command message to send to the msn server.
 */
void ServerHandler::SendCommandMessage(BMessage *message)
{
	BMessenger msgr(Looper());
	msgr.SendMessage(message);	
}

/**	Sends a command message with the current trID to the handlers owner,
	the handler's owner must be subtype of ServerLooper. The current trID
	is added automatically to the message, after that SendCommandMessage is
	called with the updated message.
	@param the command message to send to the msn server. 
 */ 
void ServerHandler::SendCommandMessageTrID(BMessage *message)
{
	//add current trID to command message
	ServerLooper *owner = dynamic_cast<ServerLooper*>(Looper());
	//check if cast has succeeded
	if (owner != NULL)
	{
		if (owner->Lock())
		{
				//find current trID
				int32 trID = owner->getCurrentTrID();
				message->AddInt32(K_TR_ID, trID);
				owner->incTrID();
				owner->Unlock();
				//send command message with trID
				SendCommandMessage(message);				
		}
	}
}

/**	Sends a command message with the current trID to the handlers owner,
	the handler's owner must be subtype of ServerLooper. The current trID
	is added automatically to the message, after that SendCommandMessage is
	called with the updated message.
	@param the command message to send to the msn server. 
	@param a pointer to a variable that will hold the trId this message was send with
 */ 
void ServerHandler::SendCommandMessageTrID(BMessage *message, int32 *currentTrId)
{
	//add current trID to command message
	ServerLooper *owner = dynamic_cast<ServerLooper*>(Looper());
	//check if cast has succeeded
	if (owner != NULL)
	{
		if (owner->Lock())
		{
				//find current trID
				int32 trID = owner->getCurrentTrID();
				*currentTrId = trID;
				message->AddInt32(K_TR_ID, trID);
				owner->incTrID();
				owner->Unlock();
				//send command message with trID
				SendCommandMessage(message);				
		}
	}
}

/** Called if an error occurs within the ServerHandler or its subclasses.
	The error message that describes the error will be send to the owner looper.
	If the error is critical, as indicated by the bool critical, all connections to
	servers should be closed and Bme should sign out.
	@param error, a string indicating the cause or nature of the error.
	@param critical, a bool value indicating whether this error is critical,
	i.e. should cause a sign out of the program.
 */
void ServerHandler::Error(BString error, bool critical)
{
	BMessage *errorMsg = new BMessage(ProtocolConstants::K_ERROR_MESSAGE);
	errorMsg->AddString(K_ERROR_STRING,error);
	errorMsg->AddBool(K_CRITICAL_BOOL,critical);
	
	SendCommandMessage(errorMsg);	
}
