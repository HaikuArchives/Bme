#ifndef TAG_H
#include "Tag.h"
#endif

#include <interface/Rect.h>
#include "TagQueue.h"

Tag::Tag(int32 type)
		:	m_type(type),
			m_text("")
{
}

Tag::Tag(int32 type, BString text)
		:	m_type(type),
			m_text(text)
{
}

Tag::~Tag()
{
}

int32 Tag::Type()
{
	return m_type;
}

BRect Tag::Bounds()
{
	BRect bounds;
	return bounds;
}						

BString	Tag::Text()
{
	return m_text;
}

void Tag::SetText(BString text)
{
	m_text = text;
}
		
int32 Tag::StartIndex()
{
	return m_start;
}

int32 Tag::EndIndex()
{
	return m_end;
}
