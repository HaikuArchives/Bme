#ifndef ICON_THEME_H
#include "IconTheme.h"
#endif 

#include <translation/TranslationUtils.h>
#include <cstdlib>
#include "Common.h"

IconTheme::IconTheme(BPath themePath)
				:	m_themePath(themePath)
{
	ParseThemeFile(themePath);
}

IconTheme::~IconTheme()
{
	//delete statusses
	for (SI s = m_statusses.begin(); s != m_statusses.end(); ++s)
	{
		delete (*s).second;
	}
	//delete emoticons
	for (EI e = m_emoticons.begin(); e != m_emoticons.end(); ++e)
	{
		delete *e;
	}
}

void IconTheme::SetName(BString name)
{
	m_name = name;
}

BString IconTheme::Name()
{
	return m_name;
}

void IconTheme::SetStatusses(map<BString,Status*> statusses)
{
	m_statusses = statusses;
}

map<BString,Status*> IconTheme::Statusses()
{
	return m_statusses;
}
		
void IconTheme::SetEmoticons(vector<Emoticon*> emoticons)
{
	m_emoticons = emoticons;
}

vector<Emoticon*> IconTheme::Emoticons()
{
	return m_emoticons;
}

void IconTheme::ParseThemeFile(BPath themePath)
{
	//read the theme setting xml file
	xmlDocPtr doc = xmlReadFile(themePath.Path(), NULL, 0);
    if (doc == NULL) 
    {
        fprintf(stderr, "Failed to parse document\n");		
    }  
    //find the root element of the xml document
    xmlNode *rootElement = xmlDocGetRootElement(doc);
	
	xmlChar* K_ICON_PREFS_TAG = xmlCharStrdup("iconprefs");
	xmlChar* K_NAME_TAG = xmlCharStrdup("name");
	xmlChar* K_STATUSSES_TAG = xmlCharStrdup("statusses");
	xmlChar* K_EMOTICONS_TAG = xmlCharStrdup("emoticons");
	
	if (xmlStrEqual(rootElement->name, K_ICON_PREFS_TAG))
	{	
		xmlNode *cur_node = NULL;
    	for (cur_node = rootElement->children; cur_node; cur_node = cur_node->next) 
    	{
       		if (cur_node->type == XML_ELEMENT_NODE) 
       	 	{
            	if (xmlStrEqual(cur_node->name, K_NAME_TAG))
            	{
            		xmlNode* textNode = cur_node->children;
            		SetName(GetNodeText(textNode));            		
            	}
            	else if (xmlStrEqual(cur_node->name, K_STATUSSES_TAG))
            	{
            		m_statusses = ParseStatusses(cur_node->children);
            	}
            	else if (xmlStrEqual(cur_node->name, K_EMOTICONS_TAG))
            	{
            		m_emoticons = ParseEmoticons(cur_node->children);
            	}    
	        }       		
    	}
    }
    //free the xml document
    xmlFreeDoc(doc);    
}

map<BString,Status*> IconTheme::ParseStatusses(xmlNode *statussesNode)
{
	map<BString,Status*> statusses;
	//parse all status xml into objects
	xmlChar* K_STATUS_TAG = xmlCharStrdup("status");
	xmlChar* K_ICON_TAG = xmlCharStrdup("icon");
	xmlChar* K_MESSAGE_TAG = xmlCharStrdup("message");
	xmlChar* K_PROTOCOL_TAG = xmlCharStrdup("protocol");
	xmlChar* K_VISIBLE_TAG = xmlCharStrdup("visible");
	xmlChar* K_COLOR_TAG = xmlCharStrdup("color");	
	
	for (xmlNode *cur_node = statussesNode; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE && xmlStrEqual(cur_node->name, K_STATUS_TAG)) 
      	{   
      		Status *status = new Status();    	 		
      		for (xmlNode *statusNode = cur_node->children; statusNode; statusNode = statusNode->next)
			{      	 		
       	 		if (xmlStrEqual(statusNode->name, K_ICON_TAG))
            	{
            		xmlNode* textNode = statusNode->children;
            		BString nodeText = GetNodeText(textNode);
            		status->AddIcon(GetIconBitmap(nodeText));
            	}
            	else if (xmlStrEqual(statusNode->name, K_MESSAGE_TAG))
            	{
            		xmlNode* textNode = statusNode->children;
            		BString nodeText = GetNodeText(textNode);
            		status->SetStatusName(nodeText);
            	}
            	else if (xmlStrEqual(statusNode->name, K_PROTOCOL_TAG))
            	{
            		xmlNode* textNode = statusNode->children;
            		BString nodeText = GetNodeText(textNode);
            		status->SetAbbreviation(nodeText);
            	}            	
            	else if (xmlStrEqual(statusNode->name, K_VISIBLE_TAG))
            	{
            		xmlNode* textNode = statusNode->children;
            		BString nodeText = GetNodeText(textNode);
            		status->SetUserChoice(atoi(nodeText.String()));
            	}
            	else if (xmlStrEqual(statusNode->name, K_COLOR_TAG))
            	{
            		xmlNode* textNode = statusNode->children;
            		BString nodeText = GetNodeText(textNode);
            		status->SetStatusColour(Common::ColorFromString(nodeText));
            	}            	
            }
            statusses[status->GetAbbreviation()] = status;
      	}
	}
	return statusses;
}

vector<Emoticon*> IconTheme::ParseEmoticons(xmlNode *emoticonsNode)
{
	vector<Emoticon*> emoticons;
	//parse all emoticon xml into objects
	xmlChar* K_EMOTICON_TAG = xmlCharStrdup("emoticon");
	xmlChar* K_ICON_TAG = xmlCharStrdup("icon");
	xmlChar* K_NAME_TAG = xmlCharStrdup("name");
	xmlChar* K_TEXT_TAG = xmlCharStrdup("etext");
	for (xmlNode *cur_node = emoticonsNode; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE && xmlStrEqual(cur_node->name, K_EMOTICON_TAG)) 
   	 	{
   	 		Emoticon *emoticon = new Emoticon();
   	 		//loop through children of <emoticon> tags
   	 		for (xmlNode *emoticonNode = cur_node->children; emoticonNode; emoticonNode = emoticonNode->next)
			{					   	 		
	   	 		if (xmlStrEqual(emoticonNode->name, K_ICON_TAG))
	        	{        		
	        		xmlNode* textNode = emoticonNode->children;
	        		BString nodeText = GetNodeText(textNode);
	        		emoticon->AddIcon(GetIconBitmap(nodeText));
	        	}
	        	else if (xmlStrEqual(emoticonNode->name, K_NAME_TAG))
	        	{
	        		xmlNode* textNode = emoticonNode->children;
	        		BString nodeText = GetNodeText(textNode);
	        		emoticon->SetName(nodeText);
	        	}
	        	else if (xmlStrEqual(emoticonNode->name, K_TEXT_TAG))
	        	{
	        		xmlNode* textNode = emoticonNode->children;      		
	        		BString nodeText = GetNodeText(textNode);
	        		emoticon->AddText(nodeText);
	        	}   
	        }
	        m_emoticons.push_back(emoticon);         	
   	 	}
	}	
	return emoticons;
}

BString IconTheme::GetNodeText(xmlNode *node)
{
	BString nodeText;
	if (node->type == XML_TEXT_NODE)
	{
		xmlChar* content = node->content;
		char c;
		int i = 0;
		while ((c = content[i]) != '\0')
		{
			nodeText.Append(c,1);
			i++;
		}	
	}	
	return nodeText;
}

BBitmap* IconTheme::GetIconBitmap(BString path)
{
	//get icon file relative to theme path
	BPath bitmapPath;
	m_themePath.GetParent(&bitmapPath);
	bitmapPath.Append(path.String());		
	BBitmap *icon = BTranslationUtils::GetBitmap(bitmapPath.Path());
	return icon;
}
	