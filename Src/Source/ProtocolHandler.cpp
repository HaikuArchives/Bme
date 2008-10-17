/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef PROTOCOL_HANDLER_H
#include "ProtocolHandler.h"
#endif

#include <be/storage/Path.h>
#include <be/storage/File.h>
#include <be/storage/Entry.h>
#include <be/storage/Directory.h>
#include <be/storage/FindDirectory.h>
#include "constants.h"
#include "MessageFields.h"
#include "AddNewContactWindow.h"

ProtocolHandler::ProtocolHandler(NSLooper *nsHandler)
					:	BHandler()
{
	AddFilter(new ProtocolFilter(nsHandler));
	//load passwords from disk
	LoadPasswords();
}

ProtocolHandler::~ProtocolHandler()
{
}

void ProtocolHandler::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case InterfaceMessages::K_GET_LOGIN_NAMES:
		{
			BMessage reply(InterfaceMessages::K_LOGIN_NAMES);
			
			typedef vector<Login>::const_iterator CI;
			for(CI p = m_passwords.begin(); p != m_passwords.end();++p)
			{
				reply.AddString("loginname",(*p).m_loginName);
				reply.AddString("password",(*p).m_password);
			}
			
			message->SendReply(&reply);
		}
		break;
		case InterfaceMessages::K_CONTACT_REQUEST_ADD_MSG:
		{
			BString contactPassport;
			message->FindString(K_CONTACT_PASSPORT,&contactPassport);
			
			AddNewContactWindow *newContactWindow = new AddNewContactWindow(BRect(300,300,710,500), contactPassport.String());
			newContactWindow->Show();
		}
		break;
		default:
			BHandler::MessageReceived(message);
		break;
	}
}

void ProtocolHandler::LoadPasswords()
{
	//find settings dir
	BPath passwordPath;
	find_directory(B_USER_SETTINGS_DIRECTORY, &passwordPath);
	//append Bme path and Cache path
	passwordPath.Append(K_BME_SETTINGS_PATH);
	passwordPath.Append(K_CACHE_PATH);
	//loop through Bme Cache path and find all login names with saved passwords
	BDirectory cacheDir(passwordPath.Path());
		
	int32 entryNum = cacheDir.CountEntries();
	//compose list of loginNames with passwords
	for (int32 i = 0; i < entryNum;i++)
	{
		BEntry entry;
		cacheDir.GetNextEntry(&entry);
		//only open if it is a file!!!
		if (entry.IsFile())
		{
			BFile settingsFile(&entry, B_READ_ONLY);
			//data is stored as BMessages
			BMessage message;
			message.Unflatten(&settingsFile);
			//see if password is stored in message
			BString password;
			BMessage userMessage;
			if (message.FindMessage("user",&userMessage) == B_OK)
			{				
				if (userMessage.FindString("User::password",&password) == B_OK)
				{					
					BString loginName;
					if (userMessage.FindString("Contact::passport" ,&loginName) == B_OK)
					{						
						Login login = {loginName,password};
						m_passwords.push_back(login);
					}
				}
			}
		}
	}
}

//=============================ProtocolFilter=================================
ProtocolFilter::ProtocolFilter(NSLooper *nsLooper)
					:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE),
						m_nsLooper(nsLooper)
{
}

ProtocolFilter::~ProtocolFilter()
{
}

filter_result ProtocolFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result;
	switch (message->what)
	{
		//send these messages through to the NSLooper
		case InterfaceMessages::K_USER_SIGN_IN_MSG:
		case InterfaceMessages::K_USER_SIGN_OUT_MSG:
		case InterfaceMessages::K_USER_CHANGE_STATUS_MSG:
		case InterfaceMessages::K_USER_CHANGE_NAME_MSG:
		case InterfaceMessages::K_USER_SET_PERSONAL_MSG:
		case InterfaceMessages::K_USER_ADD_GROUP_MSG:
		case InterfaceMessages::K_USER_REMOVE_GROUP_MSG:
		case InterfaceMessages::K_USER_RENAME_GROUP_MSG:
		case InterfaceMessages::K_USER_START_CONV_MSG:		
		{	
			BMessenger msgr(m_nsLooper);
			msgr.SendMessage(message);	
			result = B_SKIP_MESSAGE;
		}
		break;
		//send through to default handler
		case InterfaceMessages::K_CONTACT_REQUEST_ADD_MSG:
		case InterfaceMessages::K_GET_LOGIN_NAMES:
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
