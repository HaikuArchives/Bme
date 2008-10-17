/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef STATUSSES_H
#include "Statusses.h"
#endif

#include <be/storage/File.h>
#include <be/translation/TranslationUtils.h>
#include "StringList.h"
#include "constants.h"

BLocker statLock;
map<BString,Status*> statusses;

status_t LoadStatusses(BPath statFilePath)
{
	status_t error = B_OK;
	//open file
	BFile statFile(statFilePath.Path(), B_READ_ONLY);
	error = statFile.InitCheck();
	if (error != B_ERROR)
	{
		StringList *tokenList = new StringList();
		//read line
		off_t fileSize;
		if (statFile.GetSize(&fileSize) == B_OK)
		{			
			ssize_t bytesRead = 0;
			BString* token = new BString("");
			bool newToken = false;			
			do
			{
				char c[2];				
				bytesRead += statFile.Read(c,1);
				c[1] = '\0';				
				
				if (c[0] == '\t' || c[0] == '\n')
				{
					//clear token
					if (!newToken)
					{
						//add token to list
						tokenList->AddItem(token);						
						token = new BString("");				
						newToken = true;
					}
				}
				else 
				{
					token->Append(c);
					newToken = false;
				}
			}
			while (bytesRead < fileSize);
		}
		
		int32 startIndex = -1;
		int32 endIndex = -1;
		for (int32 i = 0; i < tokenList->CountItems(); i++)
		{
			if ((*tokenList)[i].ICompare("<status>") == 0)
				startIndex = i;
			else if ((*tokenList)[i].ICompare("</status>") == 0)
			{
				endIndex = i;
				break;
			}
		}
		
		if (startIndex != -1 && endIndex != -1)
		{
			//start past the <status> tag
			startIndex++;
			for (int32 i = startIndex; i < endIndex; i++)
			{
				int32 lastLineIndex = i + 4;

				if (lastLineIndex < endIndex)
				{
					//parse lines
					BString bitmapString = (*tokenList)[i];
					BString statusName = (*tokenList)[i+1];
					BString abbreviation = (*tokenList)[i+2];
					bool userChoice = atoi((*tokenList)[i+3].String());				
					BString colourString = (*tokenList)[i+4];
					i += 4;
					//construct bitmap path
					BPath bitmapPath;
					statFilePath.GetParent(&bitmapPath);
					bitmapPath.Append(bitmapString.String());
					BBitmap *icon = BTranslationUtils::GetBitmap(bitmapPath.Path());
					//construct status object
					Status *status = FindStatus(abbreviation);
					if (status != NULL)
					{
						status->AddIcon(icon);
					}
					else
					{
						//remove brackets from colourString
						colourString.RemoveAll("{");						
						colourString.RemoveAll("}");
						
						int32 colours[4];
						int32 startIndex = 0;						
						for (int i = 0; i < 4; i++)
						{
							int32 commaIndex = colourString.FindFirst(",",startIndex);							
							if (commaIndex != B_ERROR)
							{
								BString colourValue;
								colourString.CopyInto(colourValue,startIndex, commaIndex-startIndex);
								colours[i] = atoi(colourValue.String());
								startIndex = commaIndex + 1;
							}
							else if (i < 3)
							{								
								BString colourValue;								
								colourString.CopyInto(colourValue, startIndex, colourString.CountChars() - startIndex);
								colours[i] = atoi(colourValue.String());								
							}		
							else if (i == 3)
							{
								colours[i] = 255;
							}						
						}
						//construct status colour
						rgb_color statusColour;
						statusColour.red = colours[0];
						statusColour.green = colours[1];
						statusColour.blue = colours[2];
						statusColour.alpha = colours[3];
						//construct new status object
						Status *status = new Status(statusName,abbreviation, icon, userChoice, statusColour);
						statLock.Lock();
						statusses[abbreviation] = status;
						statLock.Unlock();	
							
					}		
				}
			}
		}
				
		StringList::deleteStrings(tokenList);
		delete tokenList;
	}	
	else
	{
		error = B_ERROR;
	}	
	return error;
}

void DeleteStatusses()
{
	statLock.Lock();
	for (SI p = statusses.begin(); p != statusses.end(); ++p)
	{
		delete (*p).second;
	}
	statLock.Unlock();
}

Status* FindStatus(BString abbreviation)
{
	statLock.Lock();
	Status *status = statusses[abbreviation];
	statLock.Unlock();
	
	return status;
}

void PrintStatusses()
{
	for (SI p = statusses.begin(); p != statusses.end(); ++p)
	{
		BString keyValue = (*p).first;
		cout << keyValue.String() << " " << (*p).second << endl;
	}
}
