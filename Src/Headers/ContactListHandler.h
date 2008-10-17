/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef CONTACT_LIST_HANDLER_H
#define CONTACT_LIST_HANDLER_H

#include <be/app/Handler.h>
#include <be/app/Message.h>
#include <be/app/MessageFilter.h>
#include <be/storage/Path.h>
#include <libxml/tree.h>
#include <map>
#include "ServerHandler.h"
#include "Contact.h"
#include "Group.h"

/**	Class that handles all contact list related messages, like 
	synchronising the list,	adding, removing principals and groups, etc.
	@author Tim de Jong
 */
class ContactListHandler : public ServerHandler
{
	public:
		ContactListHandler();
		virtual				~ContactListHandler();
		
		virtual void		MessageReceived(BMessage *message);
	
	private:
		void				LoadCachedList(BPath cachePath);
		void				AddContact(Contact *contact);
		void				RemoveContact(Contact *contact);
		void				AddGroup(BString groupName, BString groupID);
		void				SetPersonalMessage(Contact *contact, xmlDocPtr xmlDocument);
		
	private:
		//use unique identifiers!!!
		map<BString,Contact*>	m_contactGUID;
		map<BString,Contact*>	m_contactPassports;
		
		map<BString,Group*>		m_groups;
		Contact					*m_contactAddedLast;
		
		uint32					m_numContacts;
		uint32					m_contactsAdded;
		
		uint32					m_numGroups;
		uint32					m_groupsAdded;
};

class ContactListFilter : public BMessageFilter
{
	public:
		ContactListFilter();
		virtual ~ContactListFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
};

#endif
