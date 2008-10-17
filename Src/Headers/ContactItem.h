#ifndef CONTACT_ITEM_H
#define CONTACT_ITEM_H

#include <be/app/Message.h>
#include <be/interface/Rect.h>
#include <be/interface/View.h>
#include "EditableListItem.h"
#include "Contact.h"

class ContactItem : public EditableListItem
{
	public:
		ContactItem(Contact *contact, BMessage *changedMessage, bool editable = false);
		virtual				~ContactItem();
		
		virtual void 		DrawItem(BView *owner, BRect itemRect, bool drawEverything = false);
		virtual void		Update(BView *owner, const BFont *font);
		Contact*			GetContact();
		
	private:
		Contact				*m_contact;		
};

#endif
