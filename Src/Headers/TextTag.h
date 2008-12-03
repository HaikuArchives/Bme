#ifndef TEXT_TAG_H
#define TEXT_TAG_H

#include "Tag.h"
#include <support/String.h>

class BRect;
class BView;

class TextTag : public Tag
{
	public:
		TextTag();
		TextTag(BString text);
		virtual					~TextTag();
				
		void					SetWidth(float width);
		virtual BRect			Bounds(BView *owner);
		virtual bool			IsSplittable();
		virtual TagQueue*		Split();
		virtual void			DrawTag(BView *owner, BRect enclosingRect);
		
		virtual Tag*			Clone();
		
	private:
		float					m_width;
};

#endif

