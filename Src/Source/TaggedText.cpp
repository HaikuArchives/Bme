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

TaggedText::TaggedText(BString &txt, BList &tagList)
{
	m_text = txt;
	m_tags = tagList;
}

Tag* TaggedText::Pop()
{
	//get the next tag in the Queue
	Tag* tag = TagQueue::Pop();
	//find the text belonging to this Tag
	BString tagText = GetTextForTag(tag);
	tag->SetText(tagText);
}

BString TaggedText::GetText()
{
	return m_text;
}

BList& TaggedText::GetTags()
{
	return m_tags;
}

BString	TaggedText::GetTextForTag(Tag* tag)
{
	BString textForTag;
	//find the beginning and end indices for the text this tag says something about
	int32 startIndex = tag->StartIndex();
	int32 endIndex = tag->EndIndex();
	//copy & return only the text belonging to the corresponding tag
	m_text.CopyInto(textForTag, startIndex, endIndex - startIndex);	
	return textForTag;
}
