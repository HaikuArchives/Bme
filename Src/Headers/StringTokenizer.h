/*****************************************************************
 * Copyright (c) 2003-2005,	Héctor Daniel Guajardo, Tim de Jong, *
 *																 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
 
#ifndef STRING_TOKENIZER_H
#define STRING_TOKENIZER_H

#include <be/support/String.h>
#include <be/support/List.h>

class StringList;

class StringTokenizer
{
	public:
		StringTokenizer(BString tokenString, BString delimiter = " ");
		StringTokenizer(const char *tokenString, const char *delimiter);
		virtual				~StringTokenizer();
		
		StringList			*getTokens();
		bool				hasMoreTokens();
		BString				*nextToken();
		
	private:
		StringList			*m_tokens;
		int32				m_tokenIndex;			
};

#endif
