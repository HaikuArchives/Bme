#ifndef ICON_THEME_H
#define ICON_THEME_H

#include <interface/Bitmap.h>
#include <be/storage/Path.h>
#include <be/support/String.h>
#include <libxml/tree.h>
#include <map>
#include <vector>
#include "Status.h"
#include "Emoticon.h"

typedef map<BString,Status*>::const_iterator SI;
typedef vector<Emoticon*>::const_iterator EI;

class IconTheme
{
	public:
		IconTheme(BPath themePath);
		virtual					~IconTheme();		
	
		void					SetName(BString name);
		BString					Name();
		
		void					SetStatusses(map<BString,Status*> statusses);
		map<BString,Status*>	Statusses();
		
		void					SetEmoticons(vector<Emoticon*> emoticons);
		vector<Emoticon*>		Emoticons();
	
	private:
		void					ParseThemeFile(BPath themePath);
		map<BString,Status*>	ParseStatusses(xmlNode *statussesNode);		
		vector<Emoticon*>		ParseEmoticons(xmlNode *emoticonsNode);	
		BString					GetNodeText(xmlNode *node);
		BBitmap*				GetIconBitmap(BString path);
	
	private:
		BPath					m_themePath;
		BString					m_name;
		map<BString,Status*>	m_statusses;
		vector<Emoticon*>		m_emoticons;
};

#endif
