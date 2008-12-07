#ifndef TEXT_WRAPPER_H
#include "TextWrapper.h"
#endif

#include <kernel/OS.h>
#include <interface/Point.h>
#include <support/ClassInfo.h>
#include "TaggedText.h"
#include "TextTag.h"

TextWrapper::TextWrapper(BView *enclosingView, WrappingMode wrappingMode)
				:	m_enclosingView(enclosingView),
					m_wrappingMode(wrappingMode)
{	
}

TextWrapper::~TextWrapper()
{	
}

LineBuffer* TextWrapper::CalculateTextWrapping(BRect enclosingRect, TaggedText* textBuffer)
{
	//create a line buffer to hold the lines of text
	LineBuffer* lineBuffer = new LineBuffer();
	//calculate all the stringwidths in the textbuffer at once
    CalculateStringWidths(textBuffer);
	//loop through elements	
	Line* currentLine = new Line(m_enclosingView, K_LINE_SPACER);			                 
	while (textBuffer->HasNext())
	{
		//get bounding rectangle for each element
		//try to fit elements on line
		float totalWidth = 0.0f;				
		Tag* tag = textBuffer->Next();
		//calculate the line width if this tag would be added		                 
		float tagWidth = tag->Bounds().Width();
		if (tagWidth > enclosingRect.Width() && tag->IsSplittable())
		{
			//tag is too wide to fit on a line, split up
			
		}
		else
		{
			//
			totalWidth = currentLine->Width() + tagWidth;
			//see if the tag still fits on this line, if not add a new line	when there's still tags left		
			if (totalWidth > enclosingRect.Width() && textBuffer->HasNext())
			{				
				//add the current line to the line buffer
				lineBuffer->AddLine(currentLine);	
				//create a new line to add the next tags to
				currentLine = new Line(m_enclosingView, K_LINE_SPACER);
			}
			//add the tag to the line
			currentLine->Add(tag->Clone());
			//if no tags left add the line
			if (!textBuffer->HasNext())
			{
				//add the current line to the line buffer
				lineBuffer->AddLine(currentLine);	
			}
		
			if (m_wrappingMode == K_HEIGHT_FIXED || m_wrappingMode == K_WIDTH_AND_HEIGHT_FIXED)
			{
				//calculate the height of the line buffer when the currentLine would be added
				float newLineBufferHeight = lineBuffer->Height() + currentLine->Height();
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
				PropagateTags(lineBuffer,lineBuffer->CountLines(),firstTag->Clone());			
			}
		}		
	}	
	//put the list index pointer to the beginning of the tag queue again
	textBuffer->Rewind();
	return lineBuffer;
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

void TextWrapper::DrawLineBuffer(BRect enclosingRect, LineBuffer* lineBuffer)
{
	//calculate where the text should be split up, and where the split up text should be drawn
	//CalculateTextWrapping(enclosingRect,splitText); //carried out twice
	//loop through the line buffer and draw the tagged text
	float topY = enclosingRect.top;
	for (int lineIndex = 0; lineIndex < lineBuffer->CountLines(); lineIndex++)
	{
		//get the next line from the line buffer
		Line *currentLine = lineBuffer->LineAt(lineIndex);
		//calculate line height and extra space
		float lineHeight = currentLine->Height();
		float leftX = enclosingRect.left;
		float rightX = enclosingRect.right;
		float bottomY = topY + lineHeight;
		BRect lineBounds(leftX, topY, rightX, bottomY); 
		//make sure we have some extra space above and below the line
		float spacing = currentLine->Spacing() / 2;
		lineBounds.InsetBy(0.0f, spacing);
		//loop through the tags in the current line
		float tagLeftX = leftX;
		for (int tagIndex = 0; tagIndex	< currentLine->CountItems(); tagIndex++)
		{
			//draw each tag in the enclosing view
			Tag* tag = currentLine->TagAt(tagIndex);
						
			BRect tagBounds = lineBounds;
			tagBounds.left = tagLeftX;
			float tagRightX = tagLeftX + tag->Bounds().Width();
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

void TextWrapper::CalculateStringWidths(TaggedText* textBuffer)
{
	//prepare the string arrays
	int32 numTags = textBuffer->CountItems();
	char* stringArray[numTags];
	int32 lengthArray[numTags];	
	for (int32 i = 0; i < numTags; i++)
	{
		Tag* tag = textBuffer->TagAt(i);
		BString text = tag->Text();
		//copy text into a new string buffer
		int32 stringLength = text.CountChars();
		char* tagString = new char[stringLength+1]; 
		text.CopyInto(tagString,0,stringLength);
		tagString[stringLength] = '\0';
		
		stringArray[i] = tagString;
		lengthArray[i] = stringLength;	
    }
	//calculate string widths
	float widthArray[numTags];
	m_enclosingView->GetStringWidths(stringArray, lengthArray, numTags, widthArray);	
    //set string widths                 
    for (int32 i = 0; i < numTags; i++)
	{
		Tag* tag = textBuffer->TagAt(i);
		if (is_instance_of(tag, TextTag))
		{
			TextTag *textTag = dynamic_cast<TextTag*>(tag);
//			float width = m_enclosingView->StringWidth(tag->Text().String());
	//		textTag->SetWidth(width);
			textTag->SetWidth(widthArray[i]);
		}		
    }    
}

//===============Implementation of Line class===================================================
Line::Line(BView *enclosingView, float spacing)
		:	TagQueue(),
			m_maxHeight(0.0f),
			m_lineWidth(0.0f),
			m_spacing(spacing),
			m_enclosingView(enclosingView)			
{
}

Line::~Line()
{
}
		
float Line::Height()
{
	return m_maxHeight + m_spacing;
}

float Line::Width()
{	
	return m_lineWidth;
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
	float tagHeight = tag->Bounds().Height();
	if (tagHeight > m_maxHeight)
	{
		m_maxHeight = tagHeight;
	}
	m_lineWidth += tag->Bounds().Width();
}

void Line::SetSpacing(float spacing)
{
	m_spacing = spacing;
}

float Line::Spacing()
{
	return m_spacing;
}

//implementation part of the LineBuffer class
LineBuffer::LineBuffer()
				:	m_bufferHeight(0.0f),
					m_bufferWidth(0.0f)
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
	return m_bufferHeight;
}

float LineBuffer::Width()
{
	return m_bufferWidth;
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
	//add the height to the total height of the buffer
	m_bufferHeight += line->Height();
	//see if this line is longer than the longest line
	float lineWidth = line->Width();
	if (lineWidth > m_bufferWidth)
	{
		m_bufferWidth = lineWidth;
	}
}

Line* LineBuffer::LineAt(int32 index)
{
	Line* line = static_cast<Line*>(m_lineList->ItemAt(index));
	return line;
}
