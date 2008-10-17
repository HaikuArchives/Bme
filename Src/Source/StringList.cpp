/*****************************************************************
 * Copyright (c) 2005 Héctor Daniel Guajardo, Tim de Jong		 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef STRING_LIST_H
#include "StringList.h"
#endif

/**
 * Constructor, creates the BList.
 */
StringList::StringList() 
				:	BList()
{
}

/**
 * Destructor
 */
StringList::~StringList()
{
}

/**
 * Redefined to return BString * .
 * @param index
 * @return <code> NULL </code> if the index is out of range.
 *         Otherwise returs the BString at index.
 * @see BString::ItemAt
 */
BString* StringList::ItemAt(int32 index)
{
	return (BString*) BList::ItemAt(index);
}

/**
 * Redefined to return BString * .
 * @param index
 * @return <code> NULL </code> if the index is out of range.
 *         Otherwise returs the BString at index.
 * @see BString::RemoveItem
 */
BString& StringList::operator[](int index)
{
	return (BString&) *ItemAt(index);
}

/**
 * To access the BString at the index given.
 * Be sure that the BString at index exists before calling this.
 * In case it does not it would return a reference to NULL.
 * @param index
 * @return <code> NULL </code> if the index is out of range.
 *         Otherwise returs the BString at index.
 * @see BString::ItemAt
 */
BString* StringList::RemoveItem(int32 index)
{
	return (BString*) BList::RemoveItem(index);
}

/**
 * Deletes the BStrings held by list. Not the list itself.
 * @param list
 */	
void StringList::deleteStrings(StringList *list)
{
	BString *anItem;
	while((anItem = list->RemoveItem((long)0)))
		delete anItem;
}

