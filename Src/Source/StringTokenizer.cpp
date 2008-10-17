/*****************************************************************
 * Copyright (c) 2003-2005,	Héctor Daniel Guajardo, Tim de Jong	 *
 *																 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#ifndef STRING_TOKENIZER_H
#include "StringTokenizer.h"
#endif

#include <stdio.h>
#include "StringList.h"

StringTokenizer::StringTokenizer(BString tokenString,BString delimiter)
					:	m_tokens(new StringList()),
						m_tokenIndex(0)

{
	//preprocessing, replace all enters and tabs by spaces
	tokenString.ReplaceAll("\n"," \n ",0);
	tokenString.ReplaceAll("\t"," \t ",0);
	tokenString.ReplaceAll(delimiter.String()," ");
	
	BString token = "";
//	int noChars = tokenString.CountChars();
	int noChars = tokenString.Length();
	for (int i = 0; i < noChars; i++)
	{

		char character = tokenString.ByteAt(i);
		if (character == ' ')
		{
			if (token != "")
			{
				m_tokens->AddItem(new BString(token));
				token = "";
			}
		}
		else
		{
			token += character;
		}
	}
	
	if (token != "")
	{
		m_tokens->AddItem(new BString(token));
	}
}

StringTokenizer::StringTokenizer(const char * tokenString, const char * delimiter)
					:	m_tokens(new StringList()),
						m_tokenIndex(0)

{
	BString string(tokenString);
//  Next command doesn't work with strings containing áéíóú 
//  It replaces those with " " when delimiter = " \r\n".
//	string.ReplaceSet(delimiter," "); // Replace all delimiter chars with ' '
//	while(true){
//		int32 index = string.FindFirst(' ');
//		if (index == B_ERROR){
//			tokens->AddItem(new BString(string));
//			break;
//		}else if (index == 0){
//			string.Remove(index,1);
//		}else{
//			BString *token = new BString();	
//			string.MoveInto(*token,0,i);
//			tokens->AddItem(token);	
//		}
	for (int32 i = 0; i < string.Length(); i++)
	{
		char c = string[i];
		if ((c == ' ') || (c == '\r') || (c == '\n'))
		{
			if (i == 0)
			{
				string.Remove(i,1);
			}
			else
			{
				BString *token = new BString();	
				string.MoveInto(*token,0,i);
				m_tokens->AddItem(token);	
			}
			i = -1;		//start over			
		}
	}
}

StringTokenizer::~StringTokenizer()
{
	delete m_tokens;
}

StringList* StringTokenizer::getTokens()
{
	return new StringList(*m_tokens);  // user should delete the list
}

//check if this works?
bool StringTokenizer::hasMoreTokens()
{
	int32 numItems = m_tokens->CountItems();
	return (m_tokenIndex < numItems && numItems != 0);
}

BString* StringTokenizer::nextToken()
{
	BString *token = m_tokens->ItemAt(m_tokenIndex);
	m_tokenIndex++;	
	return token;
}
