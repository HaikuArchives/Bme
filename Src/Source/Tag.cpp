#ifndef TAG_H
#include "Tag.h"
#endif

#include <interface/Rect.h>
#include <support/String.h>

Tag::Tag()
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
	
void Tag::SetText(BString text)
{
}

int32 Tag::StartIndex()
{
	return m_start;
}

int32 Tag::EndIndex()
{
	return m_end;
}
