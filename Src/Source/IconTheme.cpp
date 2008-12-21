#ifndef ICON_THEME_H
#include "IconTheme.h"
#endif 

IconTheme::IconTheme(BPath themePath)
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
            		if (textNode->type == XML_TEXT_NODE)
            		{
            			printf("node type: Element, name: %s\n", textNode->content);
            		}
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
	return statusses;
}

vector<Emoticon*> IconTheme::ParseEmoticons(xmlNode *emoticonsNode)
{
	vector<Emoticon*> emoticons;
	//parse all emoticon xml into objects
	return emoticons;
}