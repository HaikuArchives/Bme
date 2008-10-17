/*****************************************************************
 * Copyright (c) 2005 Héctor Daniel Guajardo, Tim de Jong		 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <be/support/List.h>
#include <be/support/String.h>

/**
 * Class to hold BStrings
 * @author Daniel Guajardo
 */
class StringList : public BList
{
	public:
		StringList();
		virtual				~StringList();
		
		BString*			ItemAt(int32 index);		
		BString*			RemoveItem(int32 index);		
		BString&			operator[](int index);		
		static void			deleteStrings(StringList *list);
};

#endif
