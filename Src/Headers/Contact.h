/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef CONTACT_H
#define CONTACT_H

#include "IXMLFlattenable.h"

#include <be/support/String.h>
#include <vector.h>
#include "Status.h"
#include "Group.h"

class Contact : public IXMLFlattenable
{
	public:
		Contact();
		Contact(BString passport, BString friendlyName, BString guid, int32 partOfLists, Status *status);
		Contact(BString passport, BString friendlyName, BString guid, int32 partOfLists, Status *status, vector<Group*> &groups);
		virtual				~Contact();
		
		//inherited from IXMLFlattenable
		BString				XMLFlatten();
		void				XMLUnflatten(xmlDocPtr xmlDocument);
		
		void				SetPassport(BString passport);
		BString				Passport();
		
		void				SetFriendlyName(BString friendlyName);
		BString				FriendlyName();
		
		void				SetGUID(BString guid);
		BString				GUID();
		
		void				AddGroup(Group *group);
		vector<Group*>		Groups();
		
		void				SetPartOfLists(int32 partOfLists);
		int32				PartOfLists();
		
		void				SetStatus(Status *status);
		Status*				GetStatus();
		
		bool				IsOnForwardList();
		bool				OnReverseList();
		bool				IsOnAllowList();
		bool				IsBlocked();				
		bool				IsOnline();
		
		bool				HasPersonalMessage();
		void				SetPersonalMessage(BString personalMessage);
		BString				PersonalMessage();
		
	private:
		BString				m_passport,
							m_friendlyName,
							m_guid,
							m_personalMessage
							;		
							
		bool				m_hasPersonalMessage;
		int32				m_partOfLists;
		
		Status				*m_status;
		vector<Group*>		m_groups;
};

#endif
