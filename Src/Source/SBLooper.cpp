/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef SB_LOOPER_H
#include "SBLooper.h"
#endif

#include "constants.h"
#include "MessageFields.h"
#include "ChatHandler.h"
#include "ErrorHandler.h"
#include "ProtocolConstants.h"


SBLooper::SBLooper(BHandler *owner, BString passport, BString sbUrl, int32 port, BString authString)
			:	ServerLooper(owner),
				m_passport(passport),
				m_sbUrl(sbUrl),
				m_authString(authString),
				m_port(port)
{
	//send USR message to SB Server, user initiated the conversation
	BMessage *initMessage = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
	initMessage->AddString(K_COMMAND,SwitchboardMessages::K_SB_USR_COMMAND);
	initMessage->AddString(K_REMAINING_MSG, m_passport);
	initMessage->AddString(K_REMAINING_MSG, m_authString);
	
	init(initMessage);		
}

SBLooper::SBLooper(BHandler *owner, BString passport, BString sbUrl, int32 port, BString sessionId, BString authString)
			:	ServerLooper(owner),
				m_sbUrl(sbUrl),
				m_sessionId(sessionId),
				m_authString(authString),
				m_port(port)
{	
	//send ANS message to SB Server, a contact initiated the conversation
	BMessage *initMessage = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
	initMessage->AddString(K_COMMAND,SwitchboardMessages::K_OPEN_SB_SESSION);
	initMessage->AddString(K_REMAINING_MSG, m_passport);
	initMessage->AddString(K_REMAINING_MSG, m_authString);
	initMessage->AddString(K_REMAINING_MSG, m_sessionId);
	
	init(initMessage);	
}

SBLooper::~SBLooper()
{
}

void SBLooper::init()
{
}

void SBLooper::init(BMessage *message)
{	
	Connect(m_sbUrl.String(), m_port);
	AddHandler(new ChatHandler(message));
	AddHandler(new ErrorHandler());	
	init();	
}

void SBLooper::SetAuthString(BString authString)
{
	m_authString = authString;
}

void SBLooper::MessageReceived(BMessage* message)
{
	switch (message->what)
	{	
		default:
			ServerLooper::MessageReceived(message);
		break;
	}
}
