/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef NETWORK_LOOPER_H
#define NETWORK_LOOPER_H

#include <be/app/Looper.h>
#include <be/app/Message.h>
#include <be/net/NetEndpoint.h>
#include <be/net/NetBuffer.h>
#include <be/app/MessageRunner.h>
#include <be/app/Messenger.h>
#include <be/support/String.h>

const uint32 K_CONNECTION_MSG = 'COms';

class NetworkLooper : public BLooper
{
	public:
		NetworkLooper(BLooper *owner, BNetEndpoint *connection);
		virtual ~NetworkLooper();
		
		virtual void MessageReceived(BMessage *message);
			
	private:
		void				parseCommandMessage(BString& cacheString);
	
	private:
		BLooper				*m_owner;
		BNetEndpoint		*m_connection;
		
		BNetBuffer			m_inBuffer;
		BString				m_outBuffer;
		
		BMessageRunner		*m_connectionRunner;
		BString				m_commandCache;	
		bool				m_sendCommand;
					
};

#endif
