/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef PROTOCOL_HANDLER_H
#define PROTOCOL_HANDLER_H

#include <be/app/Message.h>
#include <be/app/Handler.h>
#include <be/app/MessageFilter.h>
#include <vector>
#include "NSLooper.h"

struct Login
{
	BString m_loginName;
	BString m_password;
};


class ProtocolHandler : public BHandler
{
	public:
		ProtocolHandler(NSLooper *nsHandler);
		virtual				~ProtocolHandler();
		
		virtual void		MessageReceived(BMessage *message);
	
	private:
		void				LoadPasswords();
	
	private:
		vector<Login>		m_passwords;
};

class ProtocolFilter : public BMessageFilter
{
	public:
		ProtocolFilter(NSLooper *nsLooper);
		virtual					~ProtocolFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
	
	private:
		NSLooper				*m_nsLooper;			
};

#endif
