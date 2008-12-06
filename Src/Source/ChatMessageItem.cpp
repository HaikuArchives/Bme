#ifndef CHAT_MESSAGE_ITEM_H
#include "ChatMessageItem.h"
#endif

#include <interface/GraphicsDefs.h>
#include <interface/View.h>
#include <interface/Window.h>
#include <translation/TranslationUtils.h>
#include "ConvMessage.h"
#include "Contact.h"
#include "TextTag.h"
#include "EmoticonTag.h"

ChatMessageItem::ChatMessageItem(ConvMessage* message, bool followUp)
						:	MessageItem(message),
							m_followUp(followUp),	
							m_lineBuffer(NULL)
{
	BBitmap *emoticon1Bitmap = BTranslationUtils::GetBitmap("/boot/home/Documents/Programming/Miksprojects/Internet/bme/trunk/Graphics/Icons/Standard/Emoticons/001.png");
	BBitmap *emoticon2Bitmap = BTranslationUtils::GetBitmap("/boot/home/Documents/Programming/Miksprojects/Internet/bme/trunk/Graphics/Icons/Standard/Emoticons/002.png");
	//draw the message text
	m_text = new TaggedText();
	for (int i = 0; i < 100; i++)
	{
		m_text->Add(new TextTag("Hello "));
		m_text->Add(new TextTag("how "));
		m_text->Add(new TextTag("are "));
		m_text->Add(new TextTag("you "));
		m_text->Add(new TextTag("Tally?"));
		BBitmap *emoticonBitmap = (i % 2) == 0 ? emoticon1Bitmap : emoticon2Bitmap;
		m_text->Add(new EmoticonTag(":D",emoticonBitmap));
	}	
	
/*	for (int i = 0; i < 100; i++)
	{
		m_text->Add(new TextTag("Hello how are you?"));
	}*/
}

ChatMessageItem::~ChatMessageItem()
{
	delete m_text;
	delete m_lineBuffer;
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
		if (!m_lineBuffer || (m_lineBuffer->Width() != itemRect.Width()))
		{
cout << "calculating anew, follow up=" << IsFollowUp() << endl;			
			m_lineBuffer = wrapper.CalculateTextWrapping(wrapRect,m_text);
			SetHeight(contactNameHeight + m_lineBuffer->Height());
			SetWidth(m_lineBuffer->Width());
		}
		bigtime_t startDrawTime = real_time_clock_usecs();
		wrapper.DrawLineBuffer(wrapRect, m_lineBuffer);	
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
	/*TextWrapper wrapper(owner,TextWrapper::K_WIDTH_FIXED);
	BRect wrapRect = 
	wrapper.DrawLineBuffer(wrapRect, m_lineBuffer);*/
}

bool ChatMessageItem::IsFollowUp()
{
	return m_followUp;
}

void ChatMessageItem::SetFollowUp(bool followUp)
{
	m_followUp = followUp;
}