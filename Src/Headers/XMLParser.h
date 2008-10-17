#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <support/String.h>
#include <libxml/tree.h>

//wrapper class for c xml functions
class XMLParser
{
	public:
		XMLParser(BString xmlString);
		virtual				~XMLParser();
	
		BString				GetTagValue(const BString& tagName);
		BString				GetAttributeValue(const BString& attributeName);
		
	private:
		xmlDocPtr			ParseXMLString(BString xmlString);
			
	private:
		xmlDocPtr			m_xmlDocument;
};

#endif
