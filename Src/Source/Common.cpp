/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef COMMON_H
#include "Common.h"
#endif

//CLEAN THIS UP LATER!
#include <be/storage/NodeInfo.h>
#include <iostream>
#include <stdio.h>
#include "PeopleDefs.h"

BString Common::decodeURL(BString encoded)
{
	BString decoded = encoded;
	int index = decoded.FindFirst ('%', 0);
  	while (index != B_ERROR)
  	{
   		char buf[3], c;
   		decoded.Remove(index,1);
   		decoded.MoveInto(buf,index,2);
   		buf[2] = '\0';
   		sscanf(buf, "%x", &c);
   		decoded.Insert(c,1,index);
   		index = decoded.FindFirst('%', index + 1);
	}
  	return decoded;
}

BString Common::encodeURL(BString toEncode)
{
  	const char* rptr;
  	char* wptr;
  	char* retval;

  	wptr = retval = new char[toEncode.Length()*3];
  	rptr = toEncode.String();
//áéíóú%20(@#$%25&/?¿¡!ñÑ<>)%20{^|¬°;,~\}%20[àèìòù]
//(áéíóú)@{àèìòù}#[ºª\|~¬æ®†¥ //∫ø //π∑∂ ƒ©™£ //≤≈ ¢√ßµ…ñÑ]
  	while(1)
  	{
    	if(*rptr=='\0')
    	{ 
    		*wptr='\0'; break; 
    	}
//    	if(!(isalpha(*rptr) || isdigit(*rptr))){
    	if((*rptr==' ') || (*rptr == '%'))
    	{
      		sprintf(wptr, "%%%2x", (uint8)(*rptr));
      		rptr++;
      		wptr+=3;
      		continue;
    	}
    	*wptr=*rptr;
    	wptr++;
    	rptr++;
  	}
  	BString encodedString(retval);
  	return encodedString;
}

//TODO: does not work with links like ftp:// or http:// etc.
status_t Common::SplitURL(const BString& url, BString *urlPart, int32 *portPart)
{
	int32 colonIndex = url.FindFirst(":");
	BString sbUrl;
	int32 sbPort;							
	if (colonIndex != B_ERROR)
	{
		//find port part
		BString sbPortString;
		url.CopyInto(sbPortString, colonIndex + 1,url.CountChars() - colonIndex);
		sbPort = atoi(sbPortString.String());
		*portPart = sbPort;
		//find the remaining url part
		url.CopyInto(sbUrl, 0, colonIndex);
		*urlPart = sbUrl;		
		
		return B_OK;
	}
	return B_ERROR;
}

//djb2 hash algorithm
long Common::StringHash(BString string)
{	
	unsigned long hash = 5381;
    for (int i = 0; i < string.CountChars(); i++)
   	{
    	int c = string[i];
    	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
    return hash;
}

BBitmap* Common::ConvertBitmap(const unsigned char *rgbaBitmap, uint32 bitmapWidth, uint32 bitmapHeight)
{
	BBitmap* bitmap = new BBitmap(BRect(0.0, 0.0, bitmapWidth - 1.0, bitmapHeight - 1.0), 0, B_RGBA32);
	
	if (bitmap && bitmap->IsValid()) 
	{
    	memcpy(bitmap->Bits(), rgbaBitmap, bitmap->BitsLength());
	}
	return bitmap;	
}

xmlDocPtr Common::ParseXMLString(BString xmlString)
{	
	xmlString.Prepend("<?xml version=\"1.0\"?>");
	xmlDocPtr doc = xmlReadMemory(xmlString.String(), xmlString.Length(), "noname.xml", NULL, 0);
    if (doc == NULL) 
    {
        fprintf(stderr, "Failed to parse document\n");		
    }  
	
	return doc;
}
	
uint32 Common::FindTranslatorConstant(BTranslatorRoster *roster, const char *mime)
{
	translator_id *translators;
	int32 num_translators;
  
	roster->GetAllTranslators(&translators, &num_translators);

	for (int32 i=0;i<num_translators;i++) 
	{
		const translation_format *fmts;
     	int32 num_fmts;

     	roster->GetOutputFormats(translators[i], &fmts, &num_fmts);

     	for (int32 j = 0;j < num_fmts; j++) 
     	{
        	if (!strcasecmp(fmts[j].MIME, mime))
           	{
              	return fmts[j].type;
            }
     	}
  	}
  	return 0;
}

//TODO: implement!!!
Contact* Common::FindContact(BString contactPassport)
{
	return NULL;
}

status_t Common::WritePeopleData(BString filename, Contact *contact)
{	
	return WritePeopleData(filename, contact, "Ungrouped");	
}

status_t Common::WritePeopleData(BString filename, Contact *contact, BString groupName)
{	
	BFile file(filename.String(), B_CREATE_FILE | B_WRITE_ONLY);
	//set file mime type to people file mime
	BNodeInfo fileInfo(&file);
	fileInfo.SetType(K_PERSON_FILE_TYPE);
	//check if file is opened ok! 
	status_t err = file.InitCheck();
	if (err == B_OK)
	{
		BString contactName;
		file.WriteAttr(K_PERSON_NAME, B_STRING_TYPE, 0, contactName.String(), contactName.Length() + 1);
		
		BString nickName = contact->FriendlyName();
		file.WriteAttr(K_PERSON_NICKNAME, B_STRING_TYPE, 0, nickName.String(), nickName.Length() + 1); 
		
		BString email = contact->Passport();
		file.WriteAttr(K_PERSON_EMAIL, B_STRING_TYPE, 0, email.String(), email.Length() + 1); 
		file.WriteAttr(K_PERSON_MSN, B_STRING_TYPE, 0, email.String(), email.Length() + 1);
		
		/*file.WriteAttr(K_PERSON_HOME_PHONE, B_STRING_TYPE, 0, fHomePhone.String(), fHomePhone.Length()+1); 
		file.WriteAttr(K_PERSON_WORK_PHONE, B_STRING_TYPE, 0, fWorkPhone.String(), fWorkPhone.Length()+1); 
		file.WriteAttr(K_PERSON_CELL_PHONE, B_STRING_TYPE, 0, fCellPhone.String(), fCellPhone.Length()+1); */
		
		if(groupName.Length() > 0 && groupName.ICompare("Ungrouped") != 0)
		{
			file.WriteAttr(K_PERSON_GROUP, B_STRING_TYPE, 0, groupName.String(), groupName.Length() + 1);
		}
		else
		{
			file.WriteAttr(K_PERSON_GROUP, B_STRING_TYPE, 0, "", 1);
		}
	}
	return err;
}

vector<BString> Common::Readlines(const BString& string, BString endlineSeparator)
{
	//a vector to contain all lines in the string
	vector<BString> lines;
	if (endlineSeparator.CountChars() > 0)
	{
		//convert the string into a vector for each line
		int32 oldIndex = 0;
		int32 endlineIndex = string.FindFirst(endlineSeparator);
		while (endlineIndex != B_ERROR)
		{		
			//read one line from the string
			BString oneLine;
			int32 numChars = endlineIndex-oldIndex;
			//line could be empty!
			if (numChars > 0)
			{
				string.CopyInto(oneLine, oldIndex, numChars);
			}
			else
			{
				//line is empty
				oneLine = "";
			}			
			//copy the line into the vector and move on to the next line
			lines.push_back(oneLine);
			//save the current endlineIndex
			oldIndex = endlineIndex + endlineSeparator.CountChars();		
			//search next occurrence of an endline		
			endlineIndex = string.FindFirst(endlineSeparator, oldIndex);		
		}
	
		//copy remaining string as extra line
		if (oldIndex != B_ERROR && oldIndex < string.CountChars())
		{
			//read one line from the string
			BString oneLine;
			string.CopyInto(oneLine, oldIndex, string.CountChars()-oldIndex);	
			//copy the line into the vector and move on to the next line
			lines.push_back(oneLine);
		}
	}		
	return lines;
}

BString Common::ReadKeyValue(const BString& string, BString key, BString beginSeparator, BString endSeparator)
{
	//string to hold the value
	BString value;
	//find the key
	int32 keyIndex = string.FindFirst(key);
	if (keyIndex != B_ERROR)
	{
		//find out if there's a separator after the keyIndex
		int32 separatorIndex = (beginSeparator == "") ?	B_ERROR : 
														string.FindFirst(beginSeparator, keyIndex);
		//calculate the start of the value in the string											
		int32 valueIndex = (separatorIndex != B_ERROR) ? 	separatorIndex + beginSeparator.CountChars(): 
															keyIndex + key.CountChars();
 						
		//find the first endline after the key-value pair
		int32 endlineIndex = string.FindFirst(endSeparator, valueIndex);
		//if the endline has not been found truncate until the end of the string
		if (endlineIndex == B_ERROR)
		{
			endlineIndex = string.CountChars();
		}
		//copy the value part to be returned 
		string.CopyInto(value, valueIndex, endlineIndex - valueIndex);
	}	
	return value;
}

BString Common::ReplaceParam(const BString& string, int32 replaceIndex, BString replacement)
{
	BString replacedString = string;
	int32 regexBeginIndex, regexEndIndex = 0;
	//loop through the string until the correct regex is found.
	do
	{
		//find the regular expression in the string (if present)	
		regexBeginIndex = replacedString.FindFirst("{",regexEndIndex);
		regexEndIndex = replacedString.FindFirst("}",regexBeginIndex);
		int32 distance = regexEndIndex - regexBeginIndex;
		if (regexBeginIndex != B_ERROR && regexEndIndex != B_ERROR && distance > 1)
		{		
			//get the regexParam index
			BString regexParam;
			replacedString.CopyInto(regexParam,regexBeginIndex + 1, regexEndIndex - (regexBeginIndex + 1));
			int32 readIndex = atoi(regexParam.String());
			//see if this one is the one that we want to replace.
			if (replaceIndex == readIndex)
			{
				replacedString.Remove(regexBeginIndex, regexEndIndex - regexBeginIndex + 1);
				replacedString.Insert(replacement, regexBeginIndex); 
				break;
			}		
		}	
	}
	while (regexBeginIndex != B_ERROR || regexEndIndex != B_ERROR); 
	
	return replacedString;
}

rgb_color Common::ColorFromString(BString colorString)
{
	rgb_color statusColor;
	//remove brackets from colourString
	colorString.RemoveAll("{");						
	colorString.RemoveAll("}");
	
	int32 colors[4];
	int32 startIndex = 0;						
	for (int i = 0; i < 4; i++)
	{
		int32 commaIndex = colorString.FindFirst(",",startIndex);							
		if (commaIndex != B_ERROR)
		{
			BString colorValue;
			colorString.CopyInto(colorValue,startIndex, commaIndex-startIndex);
			colors[i] = atoi(colorValue.String());
			startIndex = commaIndex + 1;
		}
		else if (i < 3)
		{								
			BString colorValue;								
			colorString.CopyInto(colorValue, startIndex, colorString.CountChars() - startIndex);
			colors[i] = atoi(colorValue.String());								
		}		
		else if (i == 3)
		{
			colors[i] = 255;
		}						
	}
	//construct status colour	
	statusColor.red = colors[0];
	statusColor.green = colors[1];
	statusColor.blue = colors[2];
	statusColor.alpha = colors[3];
	return statusColor;
}