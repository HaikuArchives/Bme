#ifndef EMOTICON_H
#include "Emoticon.h"
#endif

Emoticon::Emoticon()
{
}

Emoticon::~Emoticon()
{
	//delete the icon bitmaps
	typedef vector<BBitmap*>::const_iterator CI;
	for (CI p = m_icons.begin(); p != m_icons.end();++p)
	{
		delete *p;
	}
}
		
void Emoticon::AddIcon(BBitmap *icon)
{
	m_icons.push_back(icon);
}

vector<BBitmap*> Emoticon::Icons()
{
	return m_icons;
}
		
void Emoticon::SetName(BString name)
{
	m_name = name;
}

BString Emoticon::Name()
{
	return m_name;
}
		
void Emoticon::AddText(BString text)
{
	m_textRepresentations.push_back(text);
}

vector<BString> Emoticon::GetTextRepresentations()
{
	return m_textRepresentations;
}
