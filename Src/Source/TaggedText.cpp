/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef TAGGED_TEXT_H
#include "TaggedText.h"
#endif

TaggedText::TaggedText()
{
}

TaggedText::TaggedText(BList &tagList)
{
	for (int i = 0; i < tagList.CountItems();i++)
	{
		Tag* tag = static_cast<Tag*>(tagList.ItemAt(i));
		Add(tag);
	}
}

BString TaggedText::GetText()
{
	BString text;	
	for (int i = 0; i < CountItems();i++)
	{
		Tag* tag = TagAt(i);
		text.Append(tag->Text());
	}
	return text;
}
