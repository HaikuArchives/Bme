/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef AUTHENTICATION_HANDLER_H
#define AUTHENTICATION_HANDLER_H

#include <app/Handler.h>
#include <app/Message.h>
#include <app/MessageFilter.h>
#include <support/String.h>
#include <support/SupportDefs.h>
#include <map>
#include "ServerHandler.h"
#include "HTTPFormatter.h"
#include "Contact.h"

class AuthenticationHandler : public ServerHandler
{
	public:
		AuthenticationHandler();
		virtual				~AuthenticationHandler();
		
		virtual void		MessageReceived(BMessage *message);
		
	private:
		bool				startLogin(BString loginName, BString password, const char* address, int port);
		BString				TweenerAuthenticate(BString challenge);
		status_t			SSLSend(const char *host, HTTPFormatter *send, HTTPFormatter **recv);		
		void				HandleSwitchboard(BMessage *switchboardMessage);
		
	private:
		BString				m_loginName,
							m_password,
							m_initialStatus;
							
		bool				m_dispatch;
		map<int32, Contact*>		m_sbUserRequests;
};

class AuthenticationFilter : public BMessageFilter
{
	public:
		AuthenticationFilter();
		virtual ~AuthenticationFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
};

#endif
