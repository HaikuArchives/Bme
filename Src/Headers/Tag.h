#ifndef TAG_H
#define TAG_H

#include <support/SupportDefs.h>
#include <support/String.h>

class BRect;
class TagQueue;

#define K_TEXT_TAG_TYPE			1
#define K_EMOTICON_TAG_TYPE		2
//TODO: add tag implementation as class/might add in separate file
/**	Tag structure to represent emoticons and other constructs within the text
 */
class Tag
{
	public:
		Tag(int32 type);
		Tag(int32 type, BString text);
		virtual				~Tag();
			
		int32				Type();
		virtual BRect		Bounds();							
		
		BString				Text();
		void				SetText(BString text);
		
		int32				StartIndex();
		int32				EndIndex();
		
		virtual bool		IsSplittable() = 0;
		virtual TagQueue*	Split() = 0;
		virtual void		DrawTag(BView *owner, BRect enclosingRect) = 0;
		virtual Tag*		Clone() = 0;
	
	private:
		int32			m_start,
						m_end,
						m_type;
		BString			m_text;						
};

#endif
