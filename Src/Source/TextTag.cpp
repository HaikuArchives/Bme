#ifndef TEXT_TAG_H
#include "TextTag.h"
#endif

#include <interface/Rect.h>
#include <interface/View.h>
#include <interface/Font.h>
#include <iostream>

TextTag::TextTag()
			:	Tag(K_TEXT_TAG_TYPE)
{
}

TextTag::TextTag(BString text)
			:	Tag(K_TEXT_TAG_TYPE, text)
{
}

TextTag::~TextTag()
{
}
		
BRect TextTag::Bounds(BView *owner)
{
	float x = 0.0f;//TODO: replace by current drawing (pen) coordinates
	float y = 0.0f;
	//get the current font for this view
	BFont font;
	owner->GetFont(&font);
	//calculate the bounds of this tag
	float textWidth = font.StringWidth(Text().String());
	
	font_height fHeight;
	font.GetHeight(&fHeight);
	float textHeight = font.Size();//fHeight.leading;
	BRect bounds(x, y, x + textWidth, y + textHeight);
	return bounds;
}

bool TextTag::IsSplittable()
{
	//TODO: look at BString to determine if splitting is possible
	return true;
}

TagQueue* TextTag::Split()
{
	//TODO: splitting algorithm for this text tag
}

void TextTag::DrawTag(BView *owner, BRect enclosingRect)
{
	owner->DrawString(Text().String(), enclosingRect.LeftBottom());
}
