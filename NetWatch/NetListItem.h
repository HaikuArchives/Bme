/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#ifndef _NET_LIST_ITEM_H
#define _NET_LIST_ITEM_H

#include <ListItem.h>

enum event_type {
	INFO_EVENT,
	DATA_RECEIVED_EVENT,
	DATA_SENT_EVENT
};

struct line_info {
	int32		numChars;			// number of characters in the line
	float		avCharWidth;		// average width of a character in the line
	float*		cumulativeWidth;	// width of line up to this point
	int32*		byteNum;			// byte offset of this char in char array
	int32		numWrapPoints;		// number of possible wrap points
	int32*		wrapPoints;			// numWrapPoints entries - preferred wrap locations		
};

struct line_info_array {
	int32		count;				// number of lines in the array
	line_info*	lines;				// array of count lines 
};

class NetListItem : public BListItem
{
public:
	NetListItem(event_type type, const char* text, const BFont* font, uint32 level = 0, bool expanded = true);
	virtual void DrawItem(BView *owner, BRect itemRect, bool drawEverything = false);
	virtual void Update(BView *owner, const BFont *font);
	void CalcWordWrap(const float wrapWidth);
protected:
	bool WrapChar(char testChar);
	event_type fType;
	char* fText;
		
	line_info_array	fLineInfo; 
	
	int32 fLineLen[100];
	int32 fStartChar[100];
	
	int fLineHeight;
	double fFontAscent; 
};

#endif
