/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef COMMON_H
#define COMMON_H

#include <app/Message.h>
#include <support/String.h>
#include <interface/Bitmap.h>
#include <storage/File.h>
#include <translation/TranslatorRoster.h>
#include <libxml/tree.h>
#include <vector.h>
#include "Contact.h"

namespace Common
{
	BString				decodeURL(BString encoded);
	BString				encodeURL(BString toEncode);
	status_t			SplitURL(const BString& url, BString *urlPart, int32 *portPart);
	long				StringHash(BString string);
	
	BBitmap*			ConvertBitmap(const unsigned char *rgbaBitmap, uint32 bitmapWidth, uint32 bitmapHeight);	
	
	//xml wrapper functions
	xmlDocPtr			ParseXMLString(BString xmlString);
	
	uint32				FindTranslatorConstant(BTranslatorRoster *roster, const char *mime);
	
	Contact*			FindContact(BString contactPassport);
	status_t 			WritePeopleData(BString filename, Contact *contact);
	status_t 			WritePeopleData(BString filename, Contact *contact, BString groupName);
	
	vector<BString>		Readlines(const BString& string, BString endlineSeparator);
	BString				ReadKeyValue(const BString& string, BString key, BString beginSeparator, BString endSeparator);
	BString				ReplaceParam(const BString& string, int32 replaceIndex, BString replacement);
	
	rgb_color			ColorFromString(BString colorString);
};

#endif
