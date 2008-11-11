#ifndef CHAT_MESSAGE_ITEM_H
#include "ChatMessageItem.h"
#endif

#include <interface/GraphicsDefs.h>
#include <interface/View.h>
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
}

ChatMessageItem::~ChatMessageItem()
{
	delete m_text;
}
		
void ChatMessageItem::DrawItem(BView* owner, BRect itemRect, bool drawEverything)
{
	ConvMessage *message = Message();	
	TextWrapper wrapper(owner,TextWrapper::K_WIDTH_FIXED);
	BRect wrapRect = itemRect;
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
		BPoint startPoint(0.0f, itemRect.top + 10.0f);
		
		owner->SetFont(be_bold_font);
		BString contactText = contact->FriendlyName().String(); 
		contactText.Append(" says:"); 
		owner->DrawString(contactText.String(),startPoint);
		
	//	SetHeight(startPoint.y + 10.0f);
		owner->SetFont(be_plain_font);
		wrapRect.top = startPoint.y + 10.0f;
	}	
	
	if (m_bounds.Width() != itemRect.Width())
	{
		m_bounds = wrapper.CalculateTextWrapping(wrapRect,m_text);
		SetHeight(m_bounds.Height());
		SetWidth(m_bounds.Width());
	}
	wrapper.DrawTextWithWrapping(wrapRect, m_text);	
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