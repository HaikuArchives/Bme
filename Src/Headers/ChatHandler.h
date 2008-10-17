#ifndef CHAT_HANDLER_H
#define CHAT_HANDLER_H

#include <app/Handler.h>
#include <app/Message.h>
#include <app/MessageFilter.h>
#include <app/MessageRunner.h>
#include "ServerHandler.h"

class ChatHandler : public ServerHandler
{
	public:
		ChatHandler(BMessage *message);
		virtual				~ChatHandler();
		
		virtual void		MessageReceived(BMessage *message);	
	
};

class ChatFilter : public BMessageFilter
{
	public:
		ChatFilter();
		virtual ~ChatFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
};

#endif
