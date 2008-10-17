#ifndef CONTACT_DETAILS_ITEM
#include "ContactDetailsItem.h"
#endif

#include "Common.h"
#include "InAppIcons.h"

ContactDetailsItem::ContactDetailsItem(Contact *contact)
						:	BListItem(),
							m_contact(contact),
							m_isTyping(false)
{
	m_isTypingIcon = Common::ConvertBitmap(K_IS_TYPING_BITS, K_IS_TYPING_WIDTH, K_IS_TYPING_HEIGHT);
	//set colour according to status
	Status *contactStatus = m_contact->GetStatus();
	SetBackgroundColour(contactStatus->StatusColour());
}

ContactDetailsItem::~ContactDetailsItem()
{
	delete m_isTypingIcon;
}

void ContactDetailsItem::DrawItem(BView *owner, BRect itemRect, bool drawEverything = false)
{
	//calculate border colours
	rgb_color lighterBorderColour = tint_color(m_backgroundColour, B_LIGHTEN_2_TINT);
	rgb_color darkerBorderColour = tint_color(m_backgroundColour, B_DARKEN_2_TINT);
	//draw item background
	owner->SetDrawingMode(B_OP_OVER);
	owner->SetHighColor(m_backgroundColour);	
	owner->FillRect(itemRect);
	//draw item background border
	owner->SetHighColor(lighterBorderColour);
	owner->StrokeLine(itemRect.LeftTop(), itemRect.LeftBottom());
	owner->StrokeLine(itemRect.LeftTop(),itemRect.RightTop());
		
	owner->SetHighColor(darkerBorderColour);	
	owner->StrokeLine(itemRect.RightTop(),itemRect.RightBottom());
	owner->StrokeLine(itemRect.LeftBottom(), itemRect.RightBottom());
	
	float drawAtX = 2.0f;
	if (m_isTyping)
	{		
		owner->SetDrawingMode(B_OP_ALPHA);
 		//draw typing icon
		owner->DrawBitmap(m_isTypingIcon, itemRect.LeftTop() + BPoint(drawAtX,3.0f));		
	}
	drawAtX += (m_isTypingIcon->Bounds()).Width() + 5.0f;
	//draw contact status icon
	Status *contactStatus = m_contact->GetStatus();
	BBitmap *statusBitmap = contactStatus->GetStatusIcon();
	owner->SetDrawingMode(B_OP_ALPHA);
	owner->DrawBitmap(statusBitmap, itemRect.LeftTop() + BPoint(drawAtX, 1.0f));
	drawAtX += (statusBitmap->Bounds()).Width() + 10.0f;
	
	float contactPictureWidth = 18.0f;
	//draw contact name(with emoticons)
	owner->SetDrawingMode(B_OP_OVER);
	owner->SetHighColor(0,0,0);
	BString contactName = m_contact->FriendlyName();
	//don't draw all text if item isn't wide enough
	BString drawText = "";
	float textWidth = owner->StringWidth(contactName.String()) + contactPictureWidth + 10.0f;
	float textEnd = drawAtX + textWidth;
	float itemWidth = itemRect.Width();
	if (textEnd >= itemWidth)
	{
		//shorten text
		int32 endIndex = 0;
		float ellipsisWidth = owner->StringWidth(B_UTF8_ELLIPSIS);
		for (int32 i = 0; i < contactName.CountChars(); i++)
		{
			drawText += contactName[i];
			textEnd = drawAtX + owner->StringWidth(drawText.String()) + ellipsisWidth;
			if (textEnd >= itemWidth)
			{
				endIndex = i;
				break;
			}
		}		
		drawText.Remove(endIndex,1);
		drawText += B_UTF8_ELLIPSIS;
	}
	else
	{
		drawText = contactName;
	}
	//draw the text
	float fontHeight = 12.0f;
	owner->MovePenTo(itemRect.LeftTop() + BPoint(drawAtX,3.0f + fontHeight));
	owner->DrawString(drawText.String());	
	//draw contact picture if available, resize to small size
}

void ContactDetailsItem::Update(BView *owner, const BFont *font)
{	
	BListItem::Update(owner, font);
	//make height equal to the bitmap's height
	Status *contactStatus = m_contact->GetStatus();
	BBitmap *statusBitmap = contactStatus->GetStatusIcon();
	float height = (statusBitmap->Bounds()).Height() + 2.0f;	
	SetHeight(height);	
}

//object is not the owner of the contact objects!!!
void ContactDetailsItem::SetContact(Contact *contact)
{
	m_contact = contact;
}

Contact* ContactDetailsItem::GetContact()
{
	return m_contact;
}

void ContactDetailsItem::SetTyping(bool isTyping)
{
	m_isTyping = isTyping;
}

bool ContactDetailsItem::IsTyping()
{
	return m_isTyping;
}

void ContactDetailsItem::SetBackgroundColour(rgb_color colour)
{
	m_backgroundColour = colour;
}

rgb_color ContactDetailsItem::BackgroundColour()
{
	return m_backgroundColour;
}
