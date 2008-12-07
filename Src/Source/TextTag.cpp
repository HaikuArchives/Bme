#ifndef TEXT_TAG_H
#include "TextTag.h"
#endif

#include <interface/Rect.h>
#include <interface/View.h>
#include <interface/Font.h>
#include <iostream>

TextTag::TextTag(BFont tagFont = be_plain_font)
			:	Tag(K_TEXT_TAG_TYPE),
				m_tagFont(tagFont),
				m_width(0.0f)
{
}

TextTag::TextTag(BString text, BFont tagFont)
			:	Tag(K_TEXT_TAG_TYPE, text),
				m_tagFont(tagFont),
				m_width(0.0f)				
{
}

TextTag::~TextTag()
{
}

void TextTag::SetWidth(float width)
{
	m_width = width;
}
		
BRect TextTag::Bounds()
{
	float x = 0.0f;//TODO: replace by current drawing (pen) coordinates
	float y = 0.0f;	
	//calculate the bounds of this tag
	float textWidth = m_width;//font.StringWidth(Text().String());
	                    
	font_height fHeight;
	m_tagFont.GetHeight(&fHeight);
	float textHeight = m_tagFont.Size();//fHeight.leading;
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

void TextTag::SetFont(BFont font)
{
	m_tagFont = font;
}

BFont TextTag::Font()
{
	return m_tagFont;
}

Tag* TextTag::Clone()
{
	TextTag* clone = new TextTag(Text());//TODO: add start index and end index
	clone->SetWidth(m_width);
	return clone;
}