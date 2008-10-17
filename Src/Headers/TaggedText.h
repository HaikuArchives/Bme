/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
 
#ifndef TAGGED_TEXT_H
#define TAGGED_TEXT_H

#include <be/support/String.h>
#include <be/support/List.h>
#include "TagQueue.h"
#include "Emoticon.h"
#include "Tag.h"

/**	This class represents text and the emoticons in this text
	@author Tim de Jong	
 */
class TaggedText : public TagQueue
{
	public:
		TaggedText();
		TaggedText(BString& txt, BList& tagList);
		
		bool				IsEmpty();		
		void				Add(Tag* tag);
		Tag*				Pop();
		
		BString				getText();
		BList&				getTags();
	
	public:
		enum {	K_EMOTICON =				0,
				K_USER_EMOTICON =			1,
				K_WEB_ADDRESS =				2};
		
	private:
		//private variables
		BString				m_text;
		BList				m_tags;		
};

#endif

