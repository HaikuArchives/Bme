/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef EMOTICON_H
#define EMOTICON_H

#include <interface/Bitmap.h>
#include <support/String.h>
#include <vector>

class Emoticon
{
	public:
		Emoticon();
		virtual				~Emoticon();
		
		void				AddIcon(BBitmap *icon);
		vector<BBitmap*>	Icons();
		
		void				SetName(BString name);
		BString				Name();
		
		void				AddText(BString text);
		vector<BString>		GetTextRepresentations();
	
	private:
		vector<BBitmap*>	m_icons;
		vector<BString>		m_textRepresentations;
								
		BString				m_name;
};

#endif
