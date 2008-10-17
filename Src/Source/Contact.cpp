/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef CONTACT_H
#include "Contact.h"
#endif

#include "ProtocolConstants.h"

Contact::Contact()
			:	m_hasPersonalMessage(false)
{
}

Contact::Contact(BString passport, BString friendlyName, BString guid, int32 partOfLists, Status *status)
			:	m_passport(passport),
				m_friendlyName(friendlyName),
				m_guid(guid),
				m_hasPersonalMessage(false),
				m_partOfLists(partOfLists),
				m_status(status)
				
{
}

Contact::Contact(BString passport, BString friendlyName, BString guid, int32 partOfLists, Status *status, vector<Group*> &groups)
			:	m_passport(passport),
				m_friendlyName(friendlyName),
				m_guid(guid),
				m_hasPersonalMessage(false),	
				m_partOfLists(partOfLists),
				m_status(status)
							
{
	m_groups = groups;
}

Contact::~Contact()
{
}

BString	Contact::XMLFlatten()
{
	BString xmlString = "";
	return xmlString;
}

void Contact::XMLUnflatten(xmlDocPtr xmlDocument)
{
}

void Contact::SetPassport(BString passport)
{
	m_passport = passport;
}

BString Contact::Passport()
{
	return m_passport;
}

void Contact::SetFriendlyName(BString friendlyName)
{
	m_friendlyName = friendlyName;
}

BString Contact::FriendlyName()
{
	return m_friendlyName;
}

void Contact::SetGUID(BString guid)
{
	m_guid = guid;
}

BString Contact::GUID()
{
	return m_guid;
}

void Contact::AddGroup(Group *group)
{
	if (group)
	{
		m_groups.push_back(group);
	}
}

vector<Group*> Contact::Groups()
{
	return m_groups;
}

void Contact::SetPartOfLists(int32 partOfLists)
{
	m_partOfLists = partOfLists;
}

int32 Contact::PartOfLists()
{
	return m_partOfLists;
}

void Contact::SetStatus(Status *status)
{
	m_status = status;
}

Status* Contact::GetStatus()
{
	return m_status;
}

bool Contact::IsOnForwardList()
{
	//find out if this contact is part of the forward list
	return ((m_partOfLists & ContactListTypes::K_FORWARD_LIST) == 1);
}

bool Contact::OnReverseList()
{
	//find out if this contact has you on their forward list
	return ((m_partOfLists & ContactListTypes::K_REVERSE_LIST) == 1);
}

bool Contact::IsOnAllowList()
{
	//find out if this contact has you on their allow list
	return ((m_partOfLists & ContactListTypes::K_ALLOW_LIST) == 1);
}

bool Contact::IsBlocked()
{
	//find out if this contact is part of the blocked list
	return ((m_partOfLists & ContactListTypes::K_BLOCK_LIST) == 1);
}

bool Contact::IsOnline()
{
	return (m_status->GetAbbreviation() != Statusses::K_OFFLINE);
}

bool Contact::HasPersonalMessage()
{	
	return m_hasPersonalMessage;
}

void Contact::SetPersonalMessage(BString personalMessage)
{
	m_hasPersonalMessage = true;
	m_personalMessage = personalMessage;
}

BString Contact::PersonalMessage()
{
	return m_personalMessage;
}
