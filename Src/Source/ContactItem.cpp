#ifndef CONTACT_ITEM_H
#include "ContactItem.h"
#endif

#include "Status.h"

ContactItem::ContactItem(Contact *contact, BMessage *changedMessage, bool editable)
				:	EditableListItem(contact->FriendlyName(), changedMessage, editable),
					m_contact(contact)
{
}

ContactItem::~ContactItem()
{
}

void ContactItem::DrawItem(BView *owner, BRect itemRect, bool drawEverything)
{
	BString name = m_contact->FriendlyName();
	Status *status = m_contact->GetStatus();
	//draw status icon
	BBitmap *statusBitmap = status->GetStatusIcon();
	
	owner->SetDrawingMode(B_OP_OVER);
	float bitmapWidth = (statusBitmap->Bounds()).Width();
	BRect fillRect = itemRect;
	itemRect.left += bitmapWidth;

	//if selected
	if(IsSelected())
		owner->SetHighColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));
	else
		owner->SetHighColor(255,255,255);	
	owner->FillRect(fillRect);
	
	owner->SetDrawingMode(B_OP_ALPHA);
	if (statusBitmap)
		owner->DrawBitmap(statusBitmap, itemRect.LeftTop() + BPoint(0.0f,1.0f));
	
	//draw name(with emoticons)
	float textHeight = 12.0f;			
	BFont normal;
	owner->SetFont(&normal);
	owner->SetHighColor(0,0,0);
	owner->SetDrawingMode(B_OP_ALPHA);
	owner->DrawString(name.String(), itemRect.LeftTop() + BPoint(bitmapWidth + 5.0f, 1.0f + textHeight));
	//draw personal message, if available
	if (m_contact->HasPersonalMessage())
	{
		owner->DrawString(" - ");
		BFont italic;
		italic.SetFace(B_ITALIC_FACE); 
		owner->SetFont(&italic);	
		BString personalMessage = m_contact->PersonalMessage();
		owner->DrawString(personalMessage.String());
	}
}

void ContactItem::Update(BView *owner, const BFont *font)
{
	BListItem::Update(owner, font);
	//make height equal to the bitmap's height
	Status *contactStatus = m_contact->GetStatus();
	BBitmap *statusBitmap = contactStatus->GetStatusIcon();
	if (statusBitmap)
	{
		float height = (statusBitmap->Bounds()).Height() + 2.0f;	
		SetHeight(height);	
	}
}

Contact* ContactItem::GetContact()
{
	return m_contact;
}
