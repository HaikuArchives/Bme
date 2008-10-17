#ifndef CHAT_WINDOW_HANDLER_H
#define CHAT_WINDOW_HANDLER_H

#include <app/Handler.h>
#include <app/Message.h>
#include <app/MessageFilter.h>
#include <interface/Rect.h>
#include <support/String.h>
#include "Contact.h"

class ChatWindowHandler : public BHandler
{
	public:
		ChatWindowHandler();
		virtual				~ChatWindowHandler();
		
		virtual void		MessageReceived(BMessage *message);
	
	private:		
		BRect				CalculateChatWindowFrame();
	
	private:
		BString				m_passport;
};

class ChatWindowFilter : public BMessageFilter
{
	public:
		ChatWindowFilter();
		virtual					~ChatWindowFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
			
};

#endif
