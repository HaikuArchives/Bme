/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef CONTACT_LIST_HANDLER_H
#include "ContactListHandler.h"
#endif

#include <stdlib.h>
#include "constants.h"
#include "ProtocolConstants.h"
#include "Common.h"
#include "MessageFields.h"
#include "Status.h"
#include "Statusses.h"
#include "Preferences.h"

ContactListHandler::ContactListHandler()
						:	ServerHandler(),
							m_contactsAdded(0),
							m_groupsAdded(0)
{
	AddFilter(new ContactListFilter());
	//load cached list if present
}

ContactListHandler::~ContactListHandler()
{
	//clean up contact list
	typedef map<BString,Contact*>::const_iterator CI;
	for (CI p = m_contactGUID.begin(); p != m_contactGUID.end();++p)
	{
		delete (*p).second;
	}
	//clean up group list
	typedef map<BString,Group*>::const_iterator GI;
	for (GI p = m_groups.begin(); p != m_groups.end();++p)
	{
		delete (*p).second;	
	}
}

void ContactListHandler::MessageReceived(BMessage *message)
{
	switch(message->what)
	{	
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);
			//CHANGED IN MSNP11!!!!
			if (command == NotificationMessages::K_SYNCHRONISE_LIST)
			{
				BString firstTimeStamp;
				if (message->FindString(K_REMAINING_MSG,0,&firstTimeStamp) == B_OK)
				{
					BString secondTimeStamp;
					if (message->FindString(K_REMAINING_MSG,1,&secondTimeStamp) == B_OK)
					{
						if (firstTimeStamp != secondTimeStamp)
						{
							//we should update our contactlist	
							BString numContacts;
							if (message->FindString(K_REMAINING_MSG,2,&numContacts) == B_OK)
							{
								m_numContacts = atoi(numContacts.String());
								BString numGroups;
								if (message->FindString(K_REMAINING_MSG,3,&numGroups) == B_OK)
								{
									m_numGroups = atoi(numGroups.String());
									//update container sizes!
								}
								else
								{
									Error("SYN: Couldn't find the number of groups in the contact list", true);
								}
							}
							else
							{
								Error("SYN: Couldn't find the number of contacts in the contact list", true);
							}							
						}
						else
						{
							//if we don't have to synchronise the list and are using the cached contact list instead
							//be sure to set the m_contactsAdded and m_groupsAdded to the number of contacts and groups
							//available. In that way the initial status message will always be sent, and always after
							//the syn message
							m_contactsAdded = m_numContacts;
							m_groupsAdded = m_numGroups;
						}
					}
					else
					{
						//error
						Error("SYN: Couldn't find second time stamp", true);
					}
				}
				else
				{
					//error
					Error("SYN: Couldn't find first time stamp", true);
				}
			}
			else if (command == NotificationMessages::K_CONTACT_LIST_MSG)
			{		
				BString temp;
				int32 i = 0;
				int32 listBitIndex = -1;
				Contact *newContact = new Contact();
				while (message->FindString(K_REMAINING_MSG,i,&temp) == B_OK)
				{
					if (temp.FindFirst("N=") != B_ERROR)
					{						
						temp.RemoveFirst("N=");		
						newContact->SetPassport(temp);
					}
					else if (temp.FindFirst("F=") != B_ERROR)
					{
						BString friendlyName = temp;
						friendlyName.RemoveFirst("F=");
						friendlyName = Common::decodeURL(friendlyName);
						newContact->SetFriendlyName(friendlyName);
					}
					else if (temp.FindFirst("C=") != B_ERROR)
					{
						temp.RemoveFirst("C=");
						newContact->SetGUID(temp);
					}
					else if (i == 4)
					{
						//only in this order!!! 
						//parse groupID's
						BString groupID;
						int32 startIndex = 0;
						int32 commaIndex = B_ERROR;
						do
						{
							commaIndex = temp.FindFirst(",",startIndex);
							if (commaIndex != B_ERROR)
							{
								//get one of the groupID's
								temp.CopyInto(groupID,startIndex, commaIndex-startIndex);		
								//add group to grouplist of contact
								Group *group = m_groups[groupID];
								newContact->AddGroup(group);
cout << "GroupID 1= " << group << ", " << groupID.String() << endl;								
								groupID = "";
								//start searching at position past the previous found comma
								startIndex = commaIndex + 1;
							}
							else
							{
								//get one of the groupID's
								//temp.CopyInto(groupID,0, temp.CountChars());		
								groupID = temp;
								//add group to grouplist of contact
								Group *group = m_groups[groupID];
								newContact->AddGroup(group);
cout << "GroupID 2= " << group << ", " << groupID.String() << endl;
								groupID = "";								
								break;
							}							
						}
						while (commaIndex <= temp.CountChars());					
					}
					else
					{ 
						int32 partOfLists = atoi(temp.String());
						newContact->SetPartOfLists(partOfLists);
						listBitIndex = i;
					}
					i++;					
				}
								
				//check if part of forward list
				m_contactAddedLast = newContact;
				AddContact(m_contactAddedLast);
			}
			else if (command == NotificationMessages::K_GROUP_LIST_MSG)
			{
				BString groupName;
				if (message->FindString(K_REMAINING_MSG,0,&groupName) == B_OK)
				{
					BString groupID;					
					if (message->FindString(K_REMAINING_MSG,1,&groupID) == B_OK)
					{						
						AddGroup(groupName,groupID);
					}
				}
			}			
			else if (
						(command == NotificationMessages::K_PRINCIPAL_INITIAL_PRESENCE) ||
						(command == NotificationMessages::K_PRINCIPAL_STATUS)
					)
			{				
				BString temp;
				if (message->FindString(K_REMAINING_MSG,0,&temp) == B_OK)
				{
					//find status
					Status *status = statusses[temp];
					if (status)
					{
						//find contactID if available
						BString contactPassport;
						if (message->FindString(K_REMAINING_MSG,1,&contactPassport) == B_OK)
						{
							Contact *contact = m_contactPassports[contactPassport];								
							if (contact)
							{
								contact->SetStatus(status);
								SendCommandMessage(new BMessage(InterfaceMessages::K_CONTACT_CHANGE_STATUS_MSG));
							}							
						}
					}
				}
			}
			else if (command == NotificationMessages::K_PRINCIPAL_STATUS)
			{
			}
			else if (command == NotificationMessages::K_ADD_PRINCIPAL)
			{
			}
			else if (command == NotificationMessages::K_REMOVE_PRINCIPAL)
			{
			}
			else if (command == NotificationMessages::K_RENAME_PRINCIPAL)
			{
				//see if still works!!!
			}
			else if (command == NotificationMessages::K_CONTACT_PERSONAL_MSG)
			{
				//buddy sent a personal message
				BString principalPassport;
				//find out which buddy
				if (message->FindString(K_REMAINING_MSG,0,&principalPassport) == B_OK)
				{					
					//get the message
					BString payload;
					if (message->FindString(K_PAYLOAD_DATA, &payload) == B_OK)
					{
						//message is formatted in xml, parse the xml
						xmlDocPtr xmlDocument = Common::ParseXMLString(payload);
						//find contact to set the personal message for
						Contact *contact = m_contactPassports[principalPassport];
						//find out if personal message or music playing message
						SetPersonalMessage(contact, xmlDocument);						
					}
				}
			}						
		}
		break;
		case ProtocolConstants::K_SYNCHRONISE_MSG:
		{
			//send initial SYN message
			BMessage *synMessage = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
			synMessage->AddString(K_COMMAND,NotificationMessages::K_SYNCHRONISE_LIST);				
			synMessage->AddString(K_REMAINING_MSG,"0");
			synMessage->AddString(K_REMAINING_MSG,"0");
				
			m_contactsAdded = 0;
			m_groupsAdded = 0;			
			SendCommandMessageTrID(synMessage);
		}
		break;
		case InterfaceMessages::K_USER_ADD_PRINCIPAL:
		{
		}
		break;
		case InterfaceMessages::K_USER_REMOVE_PRINCIPAL:
		{
		}
		break;
		case InterfaceMessages::K_USER_RENAME_PRINCIPAL:
		{
		}
		break;
		case InterfaceMessages::K_USER_ADD_GROUP_MSG:
		{
		}
		break;
		case InterfaceMessages::K_USER_REMOVE_GROUP_MSG:
		{
		}
		break;
		case InterfaceMessages::K_USER_RENAME_GROUP_MSG:
		{
		}
		break;		
		default:
			BHandler::MessageReceived(message);
		break;
	}
}

void ContactListHandler::LoadCachedList(BPath cachePath)
{
}

void ContactListHandler::AddContact(Contact *contact)
{
	m_contactGUID[contact->GUID()] = contact;//check if valid guid!!!
	m_contactPassports[contact->Passport()] = contact;//check if valid passport string!!!
	//set initial status to offline
	contact->SetStatus(statusses[Statusses::K_OFFLINE]);
	//only add to contact list(UI) if on forward list
	if (contact->IsOnForwardList())
	{
		BMessage *contactMsg = new BMessage(InterfaceMessages::K_ADD_CONTACT_MSG);
		contactMsg->AddPointer(K_CONTACT, contact);
		SendCommandMessage(contactMsg);		
	}
	//we added another contact
	m_contactsAdded++;
	
	//change this afterwards!!! to work also when we're using a cached list
	if (m_contactsAdded == m_numContacts)
	{
		//send initial status message
		BMessage *setInitialStatusMsg = new BMessage(InterfaceMessages::K_INITIAL_PRESENCE_MSG);
		SendCommandMessage(setInitialStatusMsg);	
	}		
}

void ContactListHandler::RemoveContact(Contact *contact)
{
}

void ContactListHandler::AddGroup(BString groupName, BString groupID)
{
	groupName = Common::decodeURL(groupName);
	Group *newGroup = new Group(groupName,groupID);
	m_groups[groupID] = newGroup;
	
	BMessage *groupMsg = new BMessage(InterfaceMessages::K_ADD_GROUP_MSG);
	groupMsg->AddString(K_GROUP_NAME, groupName);
	groupMsg->AddString(K_GROUP_ID, groupID);

	SendCommandMessage(groupMsg);
}

void ContactListHandler::SetPersonalMessage(Contact *contact, xmlDocPtr xmlDocument)
{
	xmlNode *firstChild = xmlDocGetRootElement(xmlDocument);	
	BString compareTag((char*)firstChild->name);
	if (compareTag == PersonalMessage::K_DOCUMENT_TAG)
	{		
		//loop through children list
		xmlNode *children = firstChild->children;
		for (xmlNode *childNode = children; childNode; childNode = childNode->next) 
		{			
			BString childName ((char*)childNode->name);
			if (childName == PersonalMessage::K_PERSONAL_MESSAGE_TAG)
			{				
				xmlNode *childrenNode = childNode->children;
				if (childrenNode)
				{
					BString personalMessage((char*)childrenNode->content);
					if (personalMessage != "")
					{
						contact->SetPersonalMessage(personalMessage);			
					}
				}
			}
			else if (childName == PersonalMessage::K_CURRENT_MEDIA_TAG)
			{
				xmlNode *childrenNode = childNode->children;
				if (childrenNode)
				{
					BString currentMediaMessage((char*)childrenNode->content);
					if (currentMediaMessage != "")
					{
						//parse current media tag
						vector<BString> mediaMsgs = Common::Readlines(currentMediaMessage, "\\0");
						if (mediaMsgs.size() >= 3)
						{ 
							BString application = mediaMsgs[0];
							BString mediaType = mediaMsgs[1];
							bool enabled = atoi(mediaMsgs[2].String());
							//parse media message if and only if it's enabled
							if (enabled && (mediaMsgs.size() >= 5) && (mediaType == "Music" || mediaType == "Games"))
							{
								BString format = mediaMsgs[3];
								BString personalMessage = format;
								//parse the rest of the messages into the format message
								for (uint32 i = 4; i < mediaMsgs.size(); i++)
								{
									personalMessage = Common::ReplaceParam(personalMessage, (i - 4), mediaMsgs[i]);									
								}
								contact->SetPersonalMessage(personalMessage);							
							}
							else if (enabled && mediaType == "Office")
							{
							}										
						}
					}
				}
			}
		}
	}
	xmlFreeDoc(xmlDocument);
}

//==========================ContactListFilter================================
ContactListFilter::ContactListFilter()
						:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE)
{
}

ContactListFilter::~ContactListFilter()
{
}

filter_result ContactListFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result;
	switch (message->what)
	{		
		case InterfaceMessages::K_USER_ADD_PRINCIPAL:
		case InterfaceMessages::K_USER_REMOVE_PRINCIPAL:
		case InterfaceMessages::K_USER_RENAME_PRINCIPAL:			
		case InterfaceMessages::K_USER_ADD_GROUP_MSG:
		case InterfaceMessages::K_USER_REMOVE_GROUP_MSG:
		case InterfaceMessages::K_USER_RENAME_GROUP_MSG:
		case ProtocolConstants::K_SYNCHRONISE_MSG:
		{
			result = B_DISPATCH_MESSAGE;
		}
		break;
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);
			if	((command == NotificationMessages::K_CHANGE_STATUS) ||
				 (command == NotificationMessages::K_SYNCHRONISE_LIST) ||
				 (command == NotificationMessages::K_PRINCIPAL_INITIAL_PRESENCE) ||
				 (command == NotificationMessages::K_CONTACT_PERSONAL_MSG) 
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
