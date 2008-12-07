#ifndef EMOTICON_TAG_H
#define EMOTICON_TAG_H

#include "Tag.h"
#include <interface/Rect.h>
#include <interface/View.h>
#include <interface/Bitmap.h>
#include <support/String.h>

class EmoticonTag : public Tag
{
	public:
		EmoticonTag();
		EmoticonTag(BString text, BBitmap* emoticon);
		virtual				~EmoticonTag();
		
		virtual BRect		Bounds();
		virtual bool		IsSplittable();
		virtual TagQueue*	Split();
		virtual void		DrawTag(BView *owner, BRect enclosingRect);
		
		virtual Tag*		Clone();
	
	private:
		BBitmap				*m_emoticon;
};

#endif
