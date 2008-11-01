#ifndef TEXT_WRAPPER_H
#define TEXT_WRAPPER_H

#include <interface/View.h>
#include <support/String.h>
#include <support/List.h>
#include <vector.h>
#include "TagQueue.h"

class Tag;
class TaggedText;
class LineBuffer;

class TextWrapper
{
	public:
		enum WrappingMode
		{
			K_WIDTH_FIXED = 0,
			K_HEIGHT_FIXED,
			K_WIDTH_AND_HEIGHT_FIXED
		};
	
	public:
		TextWrapper(BView *enclosingView, WrappingMode wrappingMode = K_WIDTH_AND_HEIGHT_FIXED);
		virtual				~TextWrapper();
			
		BRect 				CalculateTextWrapping(BRect enclosingRect, TaggedText* text);
		void 				DrawTextWithWrapping(BRect enclosingRect, TaggedText* text);
				
	private:
		void 				PropagateTags(LineBuffer *buffer, int32 lineIndex, Tag* addTag);
		TaggedText*			SplitText(TaggedText* text);
			
	private:
		BView*				m_enclosingView;
		WrappingMode		m_wrappingMode;	
		LineBuffer*			m_lineBuffer;	
};

//=Queue
class Line : public TagQueue
{
	public:
		Line(BView *enclosingView);
		virtual			~Line();
		
		float			Height();
		float			Width();
						
		virtual Tag*	Next();
		virtual void	Add(Tag* tag);				
	
	private:
		float			m_maxHeight;
		BView 			*m_enclosingView;
};

class LineBuffer
{
	public:
		LineBuffer();
		virtual				~LineBuffer();
		
		float				Height();
		float				Width();
		
		bool				IsEmpty();
		int32				CountLines();
		void				AddLine(Line *line);
		Line*				LineAt(int32 index);
	
	private:
		BList*				m_lineList;			
};
	
#endif
