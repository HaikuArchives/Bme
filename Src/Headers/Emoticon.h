/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef EMOTICON_H
#define EMOTICON_H

#include <support/String.h>
#include <support/List.h>

//make sure these emoticons will be deleted at the end...
//maybe object with destructor
struct Emoticon
{
	BList				m_stringRepresentations,
						m_bitmaps;
						
	BString				m_iconPath,
						m_description;
};

#endif
