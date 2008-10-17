/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef USER_INFO_HANDLER_H
#define USER_INFO_HANDLER_H

#include <app/Handler.h>
#include <app/Message.h>
#include <app/MessageFilter.h>
#include <support/String.h>
#include "ServerHandler.h"

class UserInfoHandler : public ServerHandler
{
	public:
		UserInfoHandler();
		virtual				~UserInfoHandler();
		
		virtual void		MessageReceived(BMessage *message);
	
	private:
		BString				m_userStatus;
};

class UserInfoFilter : public BMessageFilter
{
	public:
		UserInfoFilter();
		virtual ~UserInfoFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
};

#endif
