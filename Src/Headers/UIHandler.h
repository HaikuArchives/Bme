/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include <be/app/Message.h>
#include <be/app/Handler.h>
#include <be/app/Looper.h>

class UIHandler : public BHandler
{
	public:
		UIHandler(BLooper *owner);
		virtual				~UIHandler();
	
		virtual void		MessageReceived(BMessage *message);
		
	private:
		BLooper				*m_owner;
};

#endif
