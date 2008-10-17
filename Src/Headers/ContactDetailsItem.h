#ifndef CONTACT_DETAILS_ITEM
#define CONTACT_DETAILS_ITEM

#include <be/interface/View.h>
#include <be/interface/ListItem.h>
#include <be/support/String.h>
#include "Contact.h"

class ContactDetailsItem : public BListItem
{
	public:
		ContactDetailsItem(Contact *contact);
		virtual				~ContactDetailsItem();
		
		virtual void 		DrawItem(BView *owner, BRect itemRect, bool drawEverything = false);
		virtual void		Update(BView *owner, const BFont *font);
		
		void				SetContact(Contact *contact);
		Contact*			GetContact();
		
		void				SetTyping(bool isTyping);
		bool				IsTyping();
		
		void				SetBackgroundColour(rgb_color colour);
		rgb_color			BackgroundColour();
	
	private:
		Contact*			m_contact;
		bool				m_isTyping;
		BBitmap				*m_isTypingIcon;
		rgb_color			m_backgroundColour;
};

#endif


