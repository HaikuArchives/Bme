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

BString TaggedText::getText()
{
	return m_text;
}

BList& TaggedText::getTags()
{
	return m_tags;
}

