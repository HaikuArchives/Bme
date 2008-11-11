#ifndef TEXT_WRAPPER_H
#include "TextWrapper.h"
#endif

#include <interface/Point.h>
#include "TaggedText.h"

TextWrapper::TextWrapper(BView *enclosingView, WrappingMode wrappingMode)
				:	m_enclosingView(enclosingView),
					m_wrappingMode(wrappingMode)
{
	m_lineBuffer = new LineBuffer();
}

TextWrapper::~TextWrapper()
{
	delete m_lineBuffer;
}

BRect TextWrapper::CalculateTextWrapping(BRect enclosingRect, TaggedText* textBuffer)
{
	//loop through elements	
	Line* currentLine = new Line(m_enclosingView);		
	while (textBuffer->HasNext())
	{
		//get bounding rectangle for each element
		//try to fit elements on line
		float totalWidth = 0.0f;				
		Tag* tag = textBuffer->Next();
		//calculate the line width if this tag would be added
		totalWidth = currentLine->Width() + tag->Bounds(m_enclosingView).Width();
		//see if the tag still fits on this line, if not add a new line			
		if (totalWidth > enclosingRect.Width())
		{				
			//add the current line to the line buffer
			m_lineBuffer->AddLine(currentLine);	
			//create a new line to add the next tags to
			currentLine = new Line(m_enclosingView);
		}
		//add the tag to the line
		currentLine->Add(tag);
		
		if (m_wrappingMode == K_HEIGHT_FIXED || m_wrappingMode == K_WIDTH_AND_HEIGHT_FIXED)
		{
			//calculate the height of the line buffer when the currentLine would be added
			float newLineBufferHeight = m_lineBuffer->Height() + currentLine->Height();
			//stop textwrapping when line buffer would exceed the height of the enclosingrect
			if (newLineBufferHeight > enclosingRect.Height())
			{
				//delete the current line, to prevent memory leak
				delete currentLine;
				//leave loop early, leaving still some items in the textbuffer
				break;
			}
		}					
	}	
	
	//if not all elements fit, this happens in case of:
	//a) K_HEIGHT_FIXED, in which case we should propagate the elements to other lines, 
	//   increasing their width, do this so long that the entire text fits
	//b) K_WIDTH_AND_HEIGHT_FIXED, no more text can be added, thus: do nothing anymore.
	if (textBuffer->CountItems() >= 1)
	{	
		//reloop with some kind of greedy function to: (not necessarily?)
		//a) propagate the elements to other lines
		//b) evenly (as evenly as possible) distribute the elements over the lines
		if (m_wrappingMode == K_HEIGHT_FIXED)
		{
			//loop through all remaining elements and add them to the linebuffer
			for (int i = 0; i < textBuffer->CountItems(); i++)
			{
				Tag *firstTag = textBuffer->Next();
				//slowly propagating non-fitting elements to line above		
				PropagateTags(m_lineBuffer,m_lineBuffer->CountLines(),firstTag);			
			}
		}		
	}	
	BRect lineBufferBounds(0.0f,0.0f,m_lineBuffer->Width(), m_lineBuffer->Height());
	return lineBufferBounds;
}

void TextWrapper::PropagateTags(LineBuffer *buffer, int32 lineIndex, Tag* addTag)
{	
	//get the current line to add new tags to
	Line* currentLine = buffer->LineAt(lineIndex);
	//add the tag to the end of the current line
	currentLine->Add(addTag);
	//jump to previous line
	int32 previousLineIndex = lineIndex - 1;
	//check if this isn't the first line already
	//else terminating condition, reached the first line in the buffer
	if (previousLineIndex >= 0)
	{
		//get the first tag of this line and add it to the previous line
		Tag* firstTag = currentLine->Next();					
		//call this function with the previous line index and the first tag of the current line
		PropagateTags(buffer, previousLineIndex, firstTag);
	}		
}

void TextWrapper::DrawTextWithWrapping(BRect enclosingRect, TaggedText* text)
{
	//split the text up into wrappable text parts
	TaggedText* splitText = SplitText(text);
	//calculate where the text should be split up, and where the split up text should be drawn
	//CalculateTextWrapping(enclosingRect,splitText); //carried out twice
	//loop through the line buffer and draw the tagged text
	float topY = enclosingRect.top;
	for (int lineIndex = 0; lineIndex < m_lineBuffer->CountLines(); lineIndex++)
	{
		//get the next line from the line buffer
		Line *currentLine = m_lineBuffer->LineAt(lineIndex);
		float lineHeight = currentLine->Height();
		float leftX = enclosingRect.left;
		float rightX = enclosingRect.right;
		float bottomY = topY + lineHeight;
		BRect lineBounds(leftX, topY, rightX, bottomY); 
		//loop through the tags in the current line
		float tagLeftX = leftX;
		for (int tagIndex = 0; tagIndex	< currentLine->CountItems(); tagIndex++)
		{
			//draw each tag in the enclosing view
			Tag* tag = currentLine->TagAt(tagIndex);
						
			BRect tagBounds = lineBounds;
			tagBounds.left = tagLeftX;
			float tagRightX = tagLeftX + tag->Bounds(m_enclosingView).Width();
			float endX = tagRightX < rightX ? tagRightX : rightX;
			tagBounds.right = endX;			
			tag->DrawTag(m_enclosingView, tagBounds); 
			
			tagLeftX = endX;
		}
		topY += lineHeight;
	}	
}

TaggedText*	TextWrapper::SplitText(TaggedText* text)
{
	TaggedText *stringParts = text;
	/*const char setOfCharsToEscape[] = {' ','\n','\t'};
	string.CharacterEscape(setOfCharsToEscape, ' ');
	
	int32 startOffset = 0;
	
	vector<BString> stringParts;
	do
	{
		int32 endOffset = string.FindFirst(' ', startOffset);
		
		BString textPart;
		string.CopyInto(textPart, endOffset, endOffset - startOffset);
		stringParts.push_back(textPart);
		
		startOffset = endOffset + 1;
	}
	while (startOffset < string.CountChars());*/
	
	return stringParts;
}

//===============Implementation of Line class===================================================
Line::Line(BView *enclosingView)
		:	TagQueue(),
			m_maxHeight(0),
			m_enclosingView(enclosingView)			
{
}

Line::~Line()
{
}
		
float Line::Height()
{
	return m_maxHeight;
}

float Line::Width()
{		
	float width;
	//calculate the line width
	for (int32 i = 0; i < CountItems(); i++)
	{
		Tag* tag = TagAt(i);
		width += tag->Bounds(m_enclosingView).Width();
	}
	return width;
}

Tag* Line::Next()
{
	Tag* nextTag = TagQueue::Next();	
	return nextTag;
}

void Line::Add(Tag* tag)
{
	TagQueue::Add(tag);
	//see if the tag is higher than any other in this line
	float tagHeight = tag->Bounds(m_enclosingView).Height();
	if (tagHeight > m_maxHeight)
	{
		m_maxHeight = tagHeight;
	}
}

//implementation part of the LineBuffer class
LineBuffer::LineBuffer()
{				
	m_lineList = new BList();
}

LineBuffer::~LineBuffer()
{
	//delete the contents of the linebuffer
	for (int32 i = 0; i < CountLines(); i++)
	{
		//delete the first line
		Line* line = LineAt(i);
		delete line;
	}
	//delete the list itself
	delete m_lineList;
}
		
float LineBuffer::Height()
{
	//calculate the height of this line buffer
	float bufferHeight = 0;
	for (int32 i = 0; i < CountLines(); i++)
	{
		Line *line = LineAt(i);
		bufferHeight += line->Height();		
	}
	return bufferHeight;
}

float LineBuffer::Width()
{
	//find the maximum width of this line buffer
	float maxWidth = 0;
	for (int32 i = 0; i < CountLines(); i++)
	{
		Line *line = LineAt(i);
		float lineWidth = line->Width();
		if (lineWidth > maxWidth)
		{
			maxWidth = lineWidth;
		}
	}
	return maxWidth;
}
		
bool LineBuffer::IsEmpty()
{
	return m_lineList->IsEmpty();
}

int32 LineBuffer::CountLines()
{
	return m_lineList->CountItems();
}

void LineBuffer::AddLine(Line *line)
{
	m_lineList->AddItem(line);
}

Line* LineBuffer::LineAt(int32 index)
{
	Line* line = static_cast<Line*>(m_lineList->ItemAt(index));
	return line;
}
