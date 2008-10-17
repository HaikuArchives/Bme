#ifndef CONTACT_LIST_VIEW_H
#define CONTACT_LIST_VIEW_H

#include "EditableListView.h"

class ContactDisplay;

class ContactListView : public EditableListView
{
	public:
		ContactListView(BRect frame, const char *name, list_view_type type, uint32 resizingMode, uint32 flags);
		virtual				~ContactListView();
		
		virtual void		MouseDown(BPoint where);
		
		bool 				IsContactItem(BListItem *item);				
		void				Display(ContactDisplay *display);				
		void				SortContacts();		
		
	private:
		static int			CompareContacts(const BListItem *first, const BListItem *second);					
};

/** This abstract class facilitates a common way of reordering the contacts or
	ListItems in the listview to display them according to a specific schema or
	ordering.
	for example:
	-online/offline view of the contact data
	-group based view of the contact data
 */
class ContactDisplay
{
	public:
		ContactDisplay(ContactListView *owner);
		virtual						~ContactDisplay();
		
		virtual void				SetOwner(ContactListView *owner);
		virtual ContactListView 	*Owner();
		
		virtual void				Order() = 0;
		
	protected:
		ContactListView				*m_owner;
};

#endif
