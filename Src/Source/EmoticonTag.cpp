#ifndef EMOTICON_TAG_H
#include "EmoticonTag.h"
#endif

EmoticonTag::EmoticonTag()
				:	Tag(K_EMOTICON_TAG_TYPE),
					m_emoticon(NULL)
{
}

EmoticonTag::EmoticonTag(BString text, BBitmap* emoticon)
				:	Tag(K_EMOTICON_TAG_TYPE, text),
					m_emoticon(emoticon)
{
}

EmoticonTag::~EmoticonTag()
{
}

BRect EmoticonTag::Bounds(BView *owner)
{
	BRect rect(0.0f,0.0f,0.0f,0.0f);
	if (m_emoticon)
	{
		rect = m_emoticon->Bounds();
		//rect.InsetBy(-2.0f,-2.0f);
	}
	return rect;
}

bool EmoticonTag::IsSplittable()
{
	//TODO: look at BString to determine if splitting is possible
	return true;
}

TagQueue* EmoticonTag::Split()
{
	//TODO: splitting algorithm for this text tag
}

void EmoticonTag::DrawTag(BView *owner, BRect enclosingRect)
{
	//save the original drawing mode
	drawing_mode oldMode = owner->DrawingMode();
	//draw the emoticon bitmap
	owner->SetDrawingMode(B_OP_ALPHA);
	BRect originalRect = enclosingRect;	
	owner->DrawBitmap(m_emoticon, enclosingRect);
	//restore the original drawing mode
	owner->SetDrawingMode(oldMode);
}

Tag* EmoticonTag::Clone()
{
	EmoticonTag* clone = new EmoticonTag(Text(), m_emoticon);//TODO: add start index and end index
	return clone;
}