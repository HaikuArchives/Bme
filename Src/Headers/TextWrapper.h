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

#define K_LINE_SPACER	4.0f

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
			
		LineBuffer*			CalculateTextWrapping(BRect enclosingRect, TaggedText* text);
		void 				DrawLineBuffer(BRect enclosingRect, LineBuffer* lineBuffer);
				
	private:
		void 				PropagateTags(LineBuffer *buffer, int32 lineIndex, Tag* addTag);
		TaggedText*			SplitText(TaggedText* text);
		void				CalculateStringWidths(TaggedText* textBuffer);
			
	private:
		BView*				m_enclosingView;
		WrappingMode		m_wrappingMode;			
};

//=Queue
class Line : public TagQueue
{
	public:
		Line(BView *enclosingView, float spacing = 0.0f);
		virtual			~Line();
		
		float			Height();
		float			Width();
						
		virtual Tag*	Next();
		virtual void	Add(Tag* tag);				
	
		void			SetSpacing(float spacing);
		float			Spacing();
		
	private:
		float			m_maxHeight,
						m_lineWidth,
						m_spacing;		
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
		float				m_bufferHeight,
							m_bufferWidth;	
};
	
#endif
