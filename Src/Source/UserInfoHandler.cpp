/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef USER_INFO_HANDLER_H
#include "UserInfoHandler.h"
#endif

#include <libxml/parser.h>
#include "constants.h"
#include "ProtocolConstants.h"
#include "Common.h"
#include "PrefsMethods.h"
#include "Exception.h"
#include "Status.h"
#include "MessageFields.h"

UserInfoHandler::UserInfoHandler()
					:	ServerHandler(),
						m_userStatus(Statusses::K_HIDDEN)
{
	AddFilter(new UserInfoFilter());
}

UserInfoHandler::~UserInfoHandler()
{
}

void UserInfoHandler::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);
			if (command == NotificationMessages::K_CHANGE_DISPLAY_NAME) 
			{	
cout << "prp" << endl;				
				//find out what kind of prp message this is!
				BString prpType;
				if (message->FindString(K_REMAINING_MSG, 0, &prpType) == B_OK)
				{
					BString parameter;
cout << "0 " << prpType.String() << endl;					
					if (message->FindString(K_REMAINING_MSG, 1, &parameter) == B_OK)
					{
cout << "1" << endl;						
						//get the user friendly display name from message
						parameter = Common::decodeURL(parameter);
						if (prpType == SettingTypes::K_MY_FRIENDLY_NAME)
						{						
cout << "change name" << endl;							
							//change the display name in GUI
							BMessage *changeNameMsg = new BMessage(InterfaceMessages::K_USER_NAME_CHANGED_MSG);
							changeNameMsg->AddString(K_NEW_NAME_STRING, parameter);
							SendCommandMessage(changeNameMsg);
						}
						else if (prpType == SettingTypes::K_HOME_PHONE_NUMBER)
						{
							//prefsLock.Lock();
							//preferences.ReplaceString( , parameter);
							//prefsLock.Unlock();
						}
						else if (prpType == SettingTypes::K_WORK_PHONE_NUMBER)
						{
							//prefsLock.Lock();
							//preferences.ReplaceString( , parameter);
							//prefsLock.Unlock();
						}
						else if (prpType == SettingTypes::K_MOBILE_PHONE_NUMBER)
						{
							//prefsLock.Lock();
							//preferences.ReplaceString( , parameter);
							//prefsLock.Unlock();
						}						
					}
				}
			}
			else if (command == NotificationMessages::K_MOBILE_CREDITS_MSG)
			{
				//not sure what to do with this command???
			}						
			else if (command == NotificationMessages::K_CHANGE_PERSONAL_MSG)
			{
				BString okMessage;
				if (message->FindString(K_REMAINING_MSG, 0, &okMessage) == B_OK)
				{
					if (okMessage != "0")
					{
						//error setting personal message
					}
				}
				else
				{
				}
			}
		}
		break;
		case InterfaceMessages::K_INITIAL_PRESENCE_MSG:
		{	
			BMessage *initialPresenceMsg = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
			initialPresenceMsg->AddString(K_COMMAND, NotificationMessages::K_CHANGE_STATUS);							
			initialPresenceMsg->AddString(K_REMAINING_MSG, m_userStatus);
			//find client ID 
			BString clientID;
			int64 id;
cout << "K_INITIAL_PRESENCE_MSG" << endl;			
	/**		try
			{*/
				Preferences *prefs = PrefsMethods::GetPreferences(AppConstants::K_MAIN_APP_PREFS);				
cout << "before id prefs= " << prefs << endl;				
				id = prefs->FindInt64(K_CLIENT_ID);
cout << "after id" << endl;				
	/*		}
			catch (Exception& e)
			{
				cout << e.ErrorMessage().String() << endl;
			}*/	
					
			id = ClientIdentification::K_MSNC4;
		cout << "id= " << id << endl;
			
			clientID << id;
			initialPresenceMsg->AddString(K_REMAINING_MSG,clientID);
			//add msn object!
			
			SendCommandMessageTrID(initialPresenceMsg);
		}
		break;
		case InterfaceMessages::K_USER_CHANGE_NAME_MSG:
		{			
			BString newName;
			if (message->FindString(K_NEW_NAME_STRING,&newName) == B_OK)
			{
				BMessage *changeNameMsg = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
			
				changeNameMsg->AddString(K_COMMAND,NotificationMessages::K_CHANGE_DISPLAY_NAME);		
				changeNameMsg->AddString(K_REMAINING_MSG,SettingTypes::K_MY_FRIENDLY_NAME);			 
				
				newName = Common::encodeURL(newName);
				changeNameMsg->AddString(K_REMAINING_MSG,newName);
			
				SendCommandMessageTrID(changeNameMsg);
			}
		}
		break;
		case InterfaceMessages::K_USER_CHANGE_STATUS_MSG:
		{			
			if (message->FindString(K_NEW_STATUS,&m_userStatus) == B_OK)
			{
				BMessage *changeStatusMsg = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
				
				changeStatusMsg->AddString(K_COMMAND,NotificationMessages::K_CHANGE_STATUS);		
				//add status to message			
				changeStatusMsg->AddString(K_REMAINING_MSG, m_userStatus);			 
				
				BString clientID;
				//try
				//{
cout << "K_USER_CHANGE_STATUS_MSG" << endl;					
					Preferences *prefs = PrefsMethods::GetPreferences(AppConstants::K_MAIN_APP_PREFS);
					//find client ID 
					int64 id = prefs->FindInt64(K_CLIENT_ID);
					//add client ID to message					
					clientID << id;
					changeStatusMsg->AddString(K_REMAINING_MSG,clientID);
				/*}
				catch (status_t e)
				{
					cout << e << endl;
				}*/
				//add msn object to message
				
				//send message
				SendCommandMessageTrID(changeStatusMsg);
			}
		}
		break;
		case InterfaceMessages::K_USER_SET_PERSONAL_MSG:
		{
			BString personalMessage;
			//acquire personalMessage
			if (message->FindString(K_PERSONAL_MESSAGE,  &personalMessage) == B_OK)
			{
				//create protocol message to actually change the personal message
				BMessage *personalMsg = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
				personalMsg->AddString(K_COMMAND,NotificationMessages::K_CHANGE_PERSONAL_MSG);			
				//create xml part for message
				xmlDocPtr personalMsgDoc = xmlNewDoc(BAD_CAST "1.0");
				xmlNodePtr rootNode = xmlNewNode(NULL, BAD_CAST PersonalMessage::K_DOCUMENT_TAG.String());
	    		xmlDocSetRootElement(personalMsgDoc, rootNode);
	    		xmlNewChild(rootNode, NULL, BAD_CAST PersonalMessage::K_PERSONAL_MESSAGE_TAG.String(), BAD_CAST personalMessage.String());
	    		xmlNewChild(rootNode, NULL, BAD_CAST PersonalMessage::K_CURRENT_MEDIA_TAG.String(), NULL);
	    		
	    		xmlChar *xmlbuff;
	    		int buffersize;
	    		xmlDocDumpFormatMemory(personalMsgDoc, &xmlbuff, &buffersize, 1);
	    		BString xmlMessage((char *) xmlbuff);
	    		xmlFree(xmlbuff);
	    		xmlFreeDoc(personalMsgDoc);
	    		//add xml part as payload to the message			
				personalMsg->AddInt32(K_PAYLOAD_SIZE,xmlMessage.Length());
				personalMsg->AddString(K_PAYLOAD_DATA,xmlMessage.String());			
				//prefsLock.Lock();
				//preferences.ReplaceString( , personalMessage);
				//prefsLock.Unlock();
				SendCommandMessageTrID(personalMsg);			
			}
		}
		break;		
		default:
			BHandler::MessageReceived(message);
		break;
	}	
}

//==========================UserInfoFilter================================
UserInfoFilter::UserInfoFilter()
						:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE)
{
}

UserInfoFilter::~UserInfoFilter()
{
}

filter_result UserInfoFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result;
	switch (message->what)
	{		
		case InterfaceMessages::K_USER_CHECK_MAIL_MSG:
		case InterfaceMessages::K_USER_SET_PERSONAL_MSG:
		case InterfaceMessages::K_USER_CHANGE_NAME_MSG:
		case InterfaceMessages::K_USER_CHANGE_STATUS_MSG:
		case InterfaceMessages::K_INITIAL_PRESENCE_MSG:		
		{
			result = B_DISPATCH_MESSAGE;
		}
		break;
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);
			if	(	(command == NotificationMessages::K_CHANGE_DISPLAY_NAME) ||
					(command == NotificationMessages::K_MOBILE_CREDITS_MSG) ||
					(command == NotificationMessages::K_CHANGE_PERSONAL_MSG)
				)
			{				
				result = B_DISPATCH_MESSAGE;
			}
		}
		break;
		default:
			result = B_SKIP_MESSAGE;
		break;
	}
	return result;
}
