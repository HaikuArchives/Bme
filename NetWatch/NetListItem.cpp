/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#include "NetListItem.h"

#define BEGINS_CHAR(byte) ((byte & 0xc0) != 0x80)

#include <View.h>
#include <cstdio>
#include <cstdlib>
#include <Region.h>

NetListItem::NetListItem(event_type type, const char* text, const BFont* font, uint32 level = 0, bool expanded = true)
				:	BListItem(level, expanded)
{
	font_height height;
	font->GetHeight(&height);
	fFontAscent = height.ascent;
	fLineHeight = static_cast<int>(height.ascent + height.descent + height.leading);

	fText = static_cast<char*>(malloc(strlen(text) + 1));
	strcpy(fText, text);
		
	fLineInfo.count = 1;
	fLineInfo.lines = (line_info*)malloc(sizeof(line_info));
	
	fLineInfo.lines[0].numChars = 0;
	fLineInfo.lines[0].numWrapPoints = 0;
	fLineInfo.lines[0].byteNum = (int32*)malloc(sizeof(int32));
	fLineInfo.lines[0].byteNum[0] = 0;
		
	for(int32 byteNum = 0; fText[byteNum] != 0; byteNum++)
	{ 
		if(BEGINS_CHAR(fText[byteNum]))
		{
			fLineInfo.lines[fLineInfo.count - 1].numChars++;
			if(WrapChar(fText[byteNum]))
				fLineInfo.lines[fLineInfo.count - 1].numWrapPoints++;
			if(fText[byteNum] == '\n')
			{
				// Don't count '\n' as a character
				fLineInfo.lines[fLineInfo.count - 1].numChars--;
				
				fLineInfo.count++;
				fLineInfo.lines = (line_info*)realloc(fLineInfo.lines, fLineInfo.count * sizeof(line_info));
				fLineInfo.lines[fLineInfo.count - 1].numChars = 0;
				fLineInfo.lines[fLineInfo.count - 1].numWrapPoints = 0;
				fLineInfo.lines[fLineInfo.count - 1].byteNum = (int32*)malloc(sizeof(int32));
				fLineInfo.lines[fLineInfo.count - 1].byteNum[0] = byteNum + 1;
			}
		}
	}
	
	// First pass finished, now know how many characters in each line
	for(int lineNum = 0; lineNum < fLineInfo.count; lineNum++)
	{
		fLineInfo.lines[lineNum].cumulativeWidth = (float*)malloc(fLineInfo.lines[lineNum].numChars * sizeof(float));
		fLineInfo.lines[lineNum].byteNum = (int32*)realloc(fLineInfo.lines[lineNum].byteNum, fLineInfo.lines[lineNum].numChars * sizeof(int32));
		fLineInfo.lines[lineNum].wrapPoints = (int32*)malloc(fLineInfo.lines[lineNum].numWrapPoints * sizeof(int32));
	
		// Get escapements
		float* tmpEscapements = new float[fLineInfo.lines[lineNum].numChars];
		font->GetEscapements(&fText[fLineInfo.lines[lineNum].byteNum[0]], fLineInfo.lines[lineNum].numChars, tmpEscapements);
		
		// Loop over that line to fill in details
		int charNum = -1;
		int wrapNum = -1;
		float cumulWidth = 0;
		for(int byteNum = fLineInfo.lines[lineNum].byteNum[0]; fText[byteNum] != '\n' && fText[byteNum] != 0; byteNum++)
		{
			if(BEGINS_CHAR(fText[byteNum]))
			{
				charNum++;
				fLineInfo.lines[lineNum].byteNum[charNum] = byteNum;
				cumulWidth += tmpEscapements[charNum];
				fLineInfo.lines[lineNum].cumulativeWidth[charNum] = cumulWidth;
				if(WrapChar(fText[byteNum]))
				{
					wrapNum++;
					fLineInfo.lines[lineNum].wrapPoints[wrapNum] = charNum;
				}
			}
		}
	}	
	
	fType = type;
}

void NetListItem::DrawItem(BView *owner, BRect itemRect, bool drawEverything = false)
{
	rgb_color bgColor = {255, 255, 255, 255};
	switch(fType)
	{
		case INFO_EVENT:
			bgColor.blue = 220;
		break;
		case DATA_RECEIVED_EVENT:
			bgColor.red = 128;
			bgColor.green = 128;
		break;
		case DATA_SENT_EVENT:
			bgColor.green = 128;
			bgColor.blue = 128;
		break;
		default:
			bgColor = ui_color(B_PANEL_BACKGROUND_COLOR);
		break;
	}

	owner->SetHighColor(bgColor);
	owner->FillRect(itemRect);
			
	rgb_color tintedColor = tint_color(bgColor, B_DARKEN_2_TINT);
	owner->SetHighColor(tintedColor);
	owner->StrokeLine(itemRect.LeftBottom(), itemRect.RightBottom());
	owner->StrokeLine(itemRect.RightTop(), itemRect.RightBottom());
	
	tintedColor = tint_color(bgColor, B_LIGHTEN_2_TINT);
	owner->SetHighColor(tintedColor);
	owner->StrokeLine(itemRect.LeftTop(), itemRect.RightTop());
	owner->StrokeLine(itemRect.LeftTop(), itemRect.LeftBottom());
	
	owner->SetLowColor(bgColor);
	owner->SetHighColor(0, 0, 0, 255);
	
	BPoint stringPos = itemRect.LeftTop();
	stringPos.x += 4;
	stringPos.y += fFontAscent + 2;
	
	for(int lineNum = 0; fStartChar[lineNum] > -1; lineNum++)
	{ 
		owner->DrawString(&fText[fStartChar[lineNum]], fLineLen[lineNum], stringPos);
		stringPos.y += fLineHeight;
	}
}

void NetListItem::Update(BView *owner, const BFont *font)
{
	BListItem::Update(owner, font);
	float wrapWidth = (Width() - 6)/font->Size();
	CalcWordWrap(wrapWidth);
}

void NetListItem::CalcWordWrap(const float wrapWidth)
{
	int currentLine = 0;

	fLineLen[currentLine] = 0;
	fStartChar[currentLine] = 0;

	for(int lineNum = 0; lineNum < fLineInfo.count; lineNum++)
	{
		// Set start character
		fStartChar[currentLine] = fLineInfo.lines[lineNum].byteNum[0];
		int lineChar = 0;
		int nextWrap = 0;
		float cumWrapWidth = wrapWidth;
		if(fLineInfo.lines[lineNum].numChars > 0)
		{
			while(cumWrapWidth < fLineInfo.lines[lineNum].cumulativeWidth[fLineInfo.lines[lineNum].numChars - 1])
			{
				bool midWordWrap = false;
				if(nextWrap < fLineInfo.lines[lineNum].numWrapPoints)
				{
					if(cumWrapWidth < fLineInfo.lines[lineNum].cumulativeWidth[fLineInfo.lines[lineNum].wrapPoints[nextWrap]])
						midWordWrap = true;
				} else {
					midWordWrap = true;
				}
				
				if(midWordWrap)
				{
					int wrapChar = lineChar + 1;
					while(cumWrapWidth > fLineInfo.lines[lineNum].cumulativeWidth[wrapChar])
						wrapChar++;
					if(wrapChar > lineChar + 1)
						wrapChar--;
					// Wrap after wrapChar
					fLineLen[currentLine] = fLineInfo.lines[lineNum].byteNum[wrapChar + 1] - fStartChar[currentLine];
					lineChar = wrapChar + 1;
					cumWrapWidth = fLineInfo.lines[lineNum].cumulativeWidth[lineChar - 1] + wrapWidth;
					currentLine++;
					fLineLen[currentLine] = 0;
					fStartChar[currentLine] = fLineInfo.lines[lineNum].byteNum[lineChar];					
				} else {
					if(nextWrap + 1 < fLineInfo.lines[lineNum].numWrapPoints)
					{
						while(cumWrapWidth > fLineInfo.lines[lineNum].cumulativeWidth[fLineInfo.lines[lineNum].wrapPoints[nextWrap + 1]])
						{
							nextWrap++;
							if(nextWrap + 1 >= fLineInfo.lines[lineNum].numWrapPoints)
								break;
						}
					}
					// Wrap after nextWrap
					fLineLen[currentLine] = fLineInfo.lines[lineNum].byteNum[fLineInfo.lines[lineNum].wrapPoints[nextWrap] + 1] - fStartChar[currentLine];
					lineChar = fLineInfo.lines[lineNum].wrapPoints[nextWrap] + 1;
					cumWrapWidth = fLineInfo.lines[lineNum].cumulativeWidth[lineChar - 1] + wrapWidth;
					nextWrap++;
					currentLine++;
					fLineLen[currentLine] = 0;
					fStartChar[currentLine] = fLineInfo.lines[lineNum].byteNum[lineChar];
				}
			}
			// Final wrapped line of this source line
			fLineLen[currentLine] = fLineInfo.lines[lineNum].byteNum[fLineInfo.lines[lineNum].numChars - 1] - fStartChar[currentLine] + 1;
			while(! BEGINS_CHAR(fText[fStartChar[currentLine] + fLineLen[currentLine]]))
				fLineLen[currentLine]++;
		
			currentLine++;
		} else {
			// No characters in this line
			fLineLen[currentLine] = 0;
			currentLine++;
		}
	}

	fLineLen[currentLine] = 0;
	fStartChar[currentLine] = -1;
	
	SetHeight(fLineHeight * currentLine + 4);
}

bool NetListItem::WrapChar(char testChar)
{
	switch (testChar) 
	{
		case B_SPACE:
		case B_TAB:
		case '&':
		case '*':
		case '+':
		case '-':
		case '/':
		case '<':
		case '=':
		case '>':
		case '\\':
		case '^':
		case '|':		
			return true;
		
		default:
			return false;		
	}
}
