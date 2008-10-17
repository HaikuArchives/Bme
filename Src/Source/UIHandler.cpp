/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef UI_HANDLER_H
#include "UIHandler.h"
#endif

#include <be/app/Messenger.h>

UIHandler::UIHandler(BLooper *owner)
			:	BHandler(),
				m_owner(owner)
{
}

UIHandler::~UIHandler()
{
}

void UIHandler::MessageReceived(BMessage *message)
{
	BMessenger msgr(m_owner);
	msgr.SendMessage(message);
}
