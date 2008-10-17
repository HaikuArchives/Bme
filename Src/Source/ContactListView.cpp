#ifndef CONTACT_LIST_VIEW_H
#include "ContactListView.h"
#endif

#include <be/support/ClassInfo.h>
#include "Common.h"
#include "ContactItem.h" 

ContactListView::ContactListView(BRect frame, const char *name, list_view_type type, uint32 resizingMode, uint32 flags)
					:	EditableListView(frame, name, type, resizingMode, flags)						
{
}

ContactListView::~ContactListView()
{	
}

void ContactListView::MouseDown(BPoint where)
{	
	//============code from Hartmut Reh's Haiku Newsletter==================
    BPoint cursor;
    uint32 buttons;
	
    SetMouseEventMask(B_POINTER_EVENTS, 0 );// view can receive mouse-events
    GetMouse(&cursor, &buttons);

    if (buttons & B_PRIMARY_MOUSE_BUTTON)
    {        
        //find selected contact items
        
        //compose drag & drop bitmap from contactItem
        
        //get boundaries of image
        //BRect bitmapRect  = m_contactPicture->Bounds();                         
        //create bitmap for drag-and-drop 
        //BBitmap *bitmap = new BBitmap(bitmapRect, B_RGB32,true);       		                                      
        //copy bitmap
        //memcpy(bitmap->Bits(), m_contactPicture->Bits(), m_contactPicture->BitsLength());  
        //position relative to bitmap
        //BPoint origBitmap = BPoint(bitmapRect.Width(), bitmapRect.Height());     
        BMessage dragMessage(B_SIMPLE_DATA);
        //AddDragFormats(&dragMessage);
        //bitmapMessage.AddInt32("be:actions",B_COPY_TARGET);
        //bitmapMessage.AddString("be:clip_name", "test");//m_contact->getFriendlyName());
        //m_contactPicture->Archive(&bitmapMessage);
       // DragMessage(&bitmapMessage, bitmap, B_OP_BLEND, where, this); 	
    }
    EditableListView::MouseDown(where);
}

bool ContactListView::IsContactItem(BListItem *item)
{
	return is_kind_of(item, ContactItem);
}

void ContactListView::Display(ContactDisplay *display)
{
	//reorder items according to a specific view
	display->Order();	
}

void ContactListView::SortContacts()
{
	//sort all contacts under the superitems(=group or online/offline items)
	for (int i = 0; i < FullListCountItems(); i++)
	{
		BListItem *underItem = FullListItemAt(i);
		//find all items at the top level in the hierarchy
		if (underItem->OutlineLevel() == 0)
		{
			//sort all items under the top level alphabetically and on status
			SortItemsUnder(underItem, true, ContactListView::CompareContacts);
		}
	}
}

//sorter based on 1) status(online/offline) AND 2) Alphabetic order
//compare like this: double compare = onlineStatus * (alphaValue ^ onlineStatus)
//where: alphaValue is the alphanumeric value of the string
//and: onlineStatus = 1 for status = online
//and: onlineStatus = -1 for status = offline
//sort: higher values first, lower values last
int	ContactListView::CompareContacts(const BListItem *f, const BListItem *s)
{
	//remove the const of the parameters
	BListItem *first = const_cast<BListItem*>(f);
	BListItem *second = const_cast<BListItem*>(s);
	//find out if the listitems are of ContactItem type or one of it subclasses
	if (is_kind_of(first, ContactItem))
	{
		ContactItem *firstContact = dynamic_cast<ContactItem*>(first);
		if (is_kind_of(second, ContactItem))
		{	
			ContactItem *secondContact = dynamic_cast<ContactItem*>(second);
			
			int firstOnlineStatus = firstContact->GetContact()->IsOnline() ? -1 : 1;
			int secondOnlineStatus = secondContact->GetContact()->IsOnline() ? -1 : 1;

			BString firstContactName = firstContact->GetContact()->FriendlyName();
			BString secondContactName = secondContact->GetContact()->FriendlyName();	

			double firstAlphaValue = (double)firstContactName.Compare("");
			double secondAlphaValue = (double)secondContactName.Compare("");
			//the next calculations take into account the online status 
			//and the alpha numeric value of the string when sorting			
			double firstCharValue = firstOnlineStatus * pow(firstAlphaValue, firstOnlineStatus);
			double secondCharValue = secondOnlineStatus * pow(secondAlphaValue, secondOnlineStatus);
			
			if (firstCharValue > secondCharValue)
				return 1;
			else if (firstCharValue == secondCharValue)
				return 0;
			else
				return -1;		

		}
		else
		{
			//if the second listitem is not a contactitem, this item should be ordered first
			return -1;
		}
	}
	//if the first listitem is not a contactitem, this item should be ordered first
	return 1;
}

//======================ContactDisplay abstract class=========================
ContactDisplay::ContactDisplay(ContactListView *owner)
					:	m_owner(owner)
{
}

ContactDisplay::~ContactDisplay()
{
}

void ContactDisplay::SetOwner(ContactListView *owner)
{	
	m_owner = owner;
}

ContactListView* ContactDisplay::Owner()
{
	return m_owner;
}
