/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef GROUP_H
#include "Group.h"
#endif

Group::Group()
{
}

Group::Group(BString name, BString groupID)
		:	m_groupName(name),
			m_groupID(groupID)
{
}

Group::~Group()
{
}

BString	Group::XMLFlatten()
{
	BString xmlString = "<group id = \"";  
	xmlString.Append(GroupID());
	xmlString.Append("\">");
	xmlString.Append(GroupName());
	xmlString.Append("</group>");
	return xmlString;
}

void Group::XMLUnflatten(xmlDocPtr xmlDocument)
{
}

void Group::SetGroupName(BString name)
{
	m_groupName = name;
}

BString Group::GroupName()
{
	return m_groupName;
}

void Group::SetGroupID(BString id)
{
	m_groupID = id;
}

BString Group::GroupID()
{
	return m_groupID;
}
