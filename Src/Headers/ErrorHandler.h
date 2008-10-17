/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <be/app/Message.h>
#include <be/app/Handler.h>
#include <be/app/MessageFilter.h>
#include "ServerHandler.h"

class ErrorHandler : public ServerHandler
{
	public:
		ErrorHandler();
		virtual				~ErrorHandler();
		
		virtual void		MessageReceived(BMessage *message);
};

class ErrorFilter : public BMessageFilter
{
	public:
		ErrorFilter();
		virtual ~ErrorFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
};

#endif
