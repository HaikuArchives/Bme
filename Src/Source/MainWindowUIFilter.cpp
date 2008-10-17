/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MAIN_WINDOW_UI_FILTER_H
#include "MainWindowUIFilter.h"
#endif

#include "constants.h"

MainWindowUIFilter::MainWindowUIFilter()
				:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE)
{
}

MainWindowUIFilter::~MainWindowUIFilter()
{
}

filter_result MainWindowUIFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result;
	switch (message->what)
	{		
		case InterfaceMessages::K_USER_SIGNED_IN_MSG:
		case InterfaceMessages::K_USER_SIGNED_OUT_MSG:
		case InterfaceMessages::K_USER_DISCONNECTED_MSG:			
		case InterfaceMessages::K_USER_FILE_TRANSFER_UPDATE_MSG:
		case InterfaceMessages::K_USER_MAIL_NOTIFICATION_MSG:
		
		case InterfaceMessages::K_CONTACT_CHANGE_STATUS_MSG:
		case InterfaceMessages::K_CONTACT_CHANGE_NAME_MSG:
		case InterfaceMessages::K_CONTACT_CHANGE_DISPLAYPIC_MSG:
		case InterfaceMessages::K_CONTACT_START_CONV_MSG:
		case InterfaceMessages::K_CONTACT_SEND_TEXT_MSG:
		case InterfaceMessages::K_CONTACT_TYPING_MSG:
		case InterfaceMessages::K_CONTACT_START_FILE_TRANSFER_MSG:
		case InterfaceMessages::K_CONTACT_STOP_FILE_TRANSFER_MSG:
		case InterfaceMessages::K_CONTACT_ACCEPT_FILE_TRANSFER_MSG:
		case InterfaceMessages::K_CONTACT_REFUSE_FILE_TRANSFER_MSG:
		case InterfaceMessages::K_CONTACT_FILE_TRANSFER_UPDATE_MSG:
		case InterfaceMessages::K_CONTACT_SHARE_BACKGROUNDS_MSG:
		case InterfaceMessages::K_CONTACT_SHARE_EMOTICON_MSG:
		
		case InterfaceMessages::K_ADD_CONTACT_MSG:
		case InterfaceMessages::K_ADD_GROUP_MSG:
		case InterfaceMessages::K_USER_NAME_CHANGED_MSG:										
		{
			result = B_DISPATCH_MESSAGE;
		}
		break;
		default:
			result = B_SKIP_MESSAGE;
		break;
	}
	return result;
}
