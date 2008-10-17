/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef GROUP_H
#define GROUP_H

#include "IXMLFlattenable.h"

#include <be/support/String.h>

class Group : public IXMLFlattenable
{
	public:
		Group();
		Group(BString name, BString groupID);
		virtual				~Group();
		
		//inherited from IXMLFlattenable
		BString				XMLFlatten();
		void				XMLUnflatten(xmlDocPtr xmlDocument);
		
		void				SetGroupName(BString name);
		BString				GroupName();
		
		void				SetGroupID(BString id);
		BString				GroupID();
		
	private:
		BString				m_groupName,
							m_groupID;
};

#endif
