#ifndef CHAT_MESSAGE_ITEM_H
#include "ChatMessageItem.h"
#endif

#include <interface/GraphicsDefs.h>
#include <interface/View.h>
#include "ConvMessage.h"
#include "Contact.h"

ChatMessageItem::ChatMessageItem(ConvMessage* message, bool followUp)
						:	MessageItem(message),
							m_followUp(followUp)
{
}

ChatMessageItem::~ChatMessageItem()
{
}
		
void ChatMessageItem::DrawItem(BView* owner, BRect itemRect, bool drawEverything)
{
	ConvMessage *message = Message();	
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
		BPoint startPoint(0.0f, itemRect.top + 10.0f);//boundingBox.Height());
		
		owner->SetFont(be_bold_font);
		BString contactText = contact->FriendlyName().String(); 
		contactText.Append(" says:"); 
		owner->DrawString(contactText.String(),startPoint);
		
		SetHeight(startPoint.y + 10.0f);
	}	
	//draw the message text
	
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