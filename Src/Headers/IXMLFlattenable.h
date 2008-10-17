/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef XML_FLATTENABLE_H
#define XML_FLATTENABLE_H

#include <be/support/String.h>
#include <libxml/tree.h>

/**	Interface for classes which contents could be written to and read from
	XML code. The classes adding the XML flatten functionality should inherit
	from this class and implement its two methods. This interface should be
	seen as a XML counterpart to BFlattenable.
	@author Tim de Jong
 */
class IXMLFlattenable
{
	/**	Method which converts the classes contents into XML and gives the
		XML code back into a BString.
		@return a BString containing the XML code representing the class
		content.
	 */
	virtual BString				XMLFlatten() = 0;
	
	/** Method that converts the XML code(presumably written by the XMLFlatten
		method) into an class object containing the values specified by the XML.
		Thus, this method reads back the XML code and converts into a valid class
		object.
		@param xmlDocument, a DOM_Document containing the xml code that specifies the
		values/content for the object. 
	 */
	virtual void				XMLUnflatten(xmlDocPtr xmlDocument) = 0;
};

#endif
