#ifndef XML_PARSER_H
#include "XMLParser.h"
#endif

//TODO: move functionality of Common parser functions to this class!
//TODO: make this the single entry point for all xml related functions in Bme
XMLParser::XMLParser(BString xmlString)
{
	m_xmlDocument = ParseXMLString(xmlString);
}

XMLParser::~XMLParser()
{
	xmlFreeDoc(m_xmlDocument);
}

BString XMLParser::GetTagValue(const BString& tagName)
{
	BString tagValue;
	
	return tagValue;
}

BString XMLParser::GetAttributeValue(const BString& attributeName)
{
	BString attribValue;
	
	return attribValue;
}	

xmlDocPtr XMLParser::ParseXMLString(BString xmlString)
{	
	xmlString.Prepend("<?xml version=\"1.0\"?>");
	xmlDocPtr doc = xmlReadMemory(xmlString.String(), xmlString.Length(), "noname.xml", NULL, 0);
    if (doc == NULL) 
    {
        fprintf(stderr, "Failed to parse document\n");		
    }  
	
	return doc;
}