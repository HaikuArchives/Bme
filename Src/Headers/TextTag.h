#ifndef TEXT_TAG_H
#define TEXT_TAG_H

#include "Tag.h"
#include <support/String.h>
#include <interface/Font.h>

class BRect;
class BView;

class TextTag : public Tag
{
	public:
		TextTag(BFont tagFont = be_plain_font);
		TextTag(BString text, BFont tagFont = be_plain_font);
		virtual					~TextTag();
				
		void					SetWidth(float width);
		virtual BRect			Bounds();
		virtual bool			IsSplittable();
		virtual TagQueue*		Split();
		virtual void			DrawTag(BView *owner, BRect enclosingRect);
		
		void					SetFont(BFont font);
		BFont					Font();
		
		virtual Tag*			Clone();
		
	private:
		BFont					m_tagFont;
		float					m_width;
};

#endif

