/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef SERVER_LOOPER_H
#define SERVER_LOOPER_H

#include <app/Message.h>
#include <app/Looper.h>
#include <app/Handler.h>
#include <net/NetEndpoint.h>
#include "NetworkLooper.h"

class ServerLooper : public BLooper
{
	public:
		ServerLooper(BHandler *owner);
		virtual				~ServerLooper();		
		
		virtual void		init() = 0;
		virtual	void		MessageReceived(BMessage* message);
		
		uint32				getCurrentTrID();
		virtual status_t	Connect(const char *address, int port);
		
		void				incTrID();	
		
	protected:			
		BHandler			*m_owner;
		uint32				m_currentTrID;
		
	private:
		BNetEndpoint		*m_connection;
		NetworkLooper		*m_networkLooper;		
		
		bool				m_isConnected;		
};

#endif
