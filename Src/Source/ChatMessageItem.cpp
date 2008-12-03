#ifndef CHAT_MESSAGE_ITEM_H
#include "ChatMessageItem.h"
#endif

#include <interface/GraphicsDefs.h>
#include <interface/View.h>
#include <interface/Window.h>
#include "ConvMessage.h"
#include "Contact.h"
#include "TextWrapper.h"
#include "TextTag.h"

ChatMessageItem::ChatMessageItem(ConvMessage* message, bool followUp)
						:	MessageItem(message),
							m_followUp(followUp)
{
	//draw the message text
	m_text = new TaggedText();
	for (int i = 0; i < 100; i++)
	{
		m_text->Add(new TextTag("Hello "));
		m_text->Add(new TextTag("how "));
		m_text->Add(new TextTag("are "));
		m_text->Add(new TextTag("you? "));
	}	
	
/*	for (int i = 0; i < 100; i++)
	{
		m_text->Add(new TextTag("Hello how are you?"));
	}*/
}

ChatMessageItem::~ChatMessageItem()
{
	delete m_text;
}
		
void ChatMessageItem::DrawItem(BView* owner, BRect itemRect, bool drawEverything)
{	
	bigtime_t startTime = real_time_clock_usecs();
	if(owner->Window()->LockLooper())
	{
		ConvMessage *message = Message();	
		bigtime_t startWrapTime = real_time_clock_usecs();
		TextWrapper wrapper(owner,TextWrapper::K_WIDTH_FIXED);
		bigtime_t endWrapTime = real_time_clock_usecs();
		cout << "wrap time" << endWrapTime - startWrapTime << endl;
		BRect wrapRect = itemRect;
		float contactNameHeight = 0.0f;
		if (IsFollowUp())
		{
			//if this MessageItem follows one of the same contact, only draw a separator with timestamp
			owner->StrokeLine(itemRect.LeftTop(),itemRect.RightTop(),B_MIXED_COLORS);
			//draw timestamp from message
			/*bigtime_t timestamp = message->Timestamp();
			BPoint startPoint( )
			owner->DrawString( , startPoint);*/
		}
		else
		{
			//if this MessageItem is the first in the list, draw the contactName
			Contact* contact = message->Sender();
					
			BRect boundingBox = be_plain_font->BoundingBox();
			
			owner->SetFont(be_bold_font);
			contactNameHeight = be_bold_font->Size();
			BPoint startPoint(0.0f, itemRect.top + contactNameHeight);
						
			BString contactText = contact->FriendlyName().String(); 
			contactText.Append(" says:"); 
			owner->DrawString(contactText.String(),startPoint);			
			
		//	SetHeight(startPoint.y + 10.0f);
			owner->SetFont(be_plain_font);
			wrapRect.top = startPoint.y;
		}	
		wrapRect.InsetBy(0.0f, 2.0f);
		if (m_bounds.Width() != itemRect.Width())
		{
			m_bounds = wrapper.CalculateTextWrapping(wrapRect,m_text);
			SetHeight(contactNameHeight + m_bounds.Height());
			SetWidth(m_bounds.Width());
		}
		bigtime_t startDrawTime = real_time_clock_usecs();
		wrapper.DrawTextWithWrapping(wrapRect, m_text);	
		bigtime_t endDrawTime = real_time_clock_usecs();
		cout << "draw time" << endDrawTime - startDrawTime << endl;
		
		owner->Window()->UnlockLooper();
	}
	bigtime_t endTime = real_time_clock_usecs();
	cout << "ChatMessageItem::DrawItem time" << endTime - startTime << endl;
}

BRect ChatMessageItem::ItemBounds()
{
	return m_bounds;
}

void ChatMessageItem::Update(BView* owner, const BFont* font)
{
}

bool ChatMessageItem::IsFollowUp()
{
	return m_followUp;
}

void ChatMessageItem::SetFollowUp(bool followUp)
{
	m_followUp = followUp;
}