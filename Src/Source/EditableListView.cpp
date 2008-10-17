/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef EDITABLE_LIST_VIEW_H
#include "EditableListView.h"
#endif

#include <be/interface/Window.h>
#include <be/support/ClassInfo.h>
#include "EditableListItem.h"

//===============================EditableListView=============================
EditableListView::EditableListView(BRect frame, const char *name, list_view_type type, uint32 resizingMode, uint32 flags)
					:	BOutlineListView(frame, name, type, resizingMode, flags),
						m_editCount(0),
						m_editItem(-1)
{	
	BRect rect(0,0,150,16);
	BRect textrect(rect);
	textrect.OffsetBy(BPoint(1,1));
	m_editView = new TextEditView(rect,textrect,this);	
}

EditableListView::~EditableListView()
{
}

void EditableListView::AttachedToWindow()
{	
	SetViewColor(255,255,255);
	BOutlineListView::AttachedToWindow();
}

void EditableListView::MouseDown(BPoint where)
{
	uint32 buttons;
	
	// retrieve the button state from the MouseDown message
	if (Window()->CurrentMessage()->FindInt32("buttons", (int32 *)&buttons) == B_NO_ERROR) 
	{
		// find item at the mouse location
		int32 item = IndexOf(where);
		// make sure item is valid
		if ((item >= 0) && (item < CountItems())) 
		{
			if (buttons & B_PRIMARY_MOUSE_BUTTON) 
			{
				//send rename message to own view????
				if (m_editItem != item) 
				{ 
					m_editCount = 1;
					m_editItem = item;
					RemoveChild(m_editView);
				}
				else if (m_editCount == 1) 
				{
					m_editCount = 0;
					//check if the listItem selected is of the editable type
					BListItem *selectedItem = ItemAt(item);
					if (is_kind_of(selectedItem,EditableListItem))
					{
						//if so cast the listItem to and EditableListItem
						EditableListItem *editableItem = dynamic_cast<EditableListItem*>(selectedItem);
						//check if the EditableListItem may be edited
						if (editableItem->IsEditable())
						{
							BRect rect = ItemFrame(item);
							m_editView->MoveTo(rect.LeftTop() + BPoint(10.0f,0.0f));																		
							m_editView->ResizeTo(rect.Width(), rect.Height());
							m_editView->SetText(editableItem->Text());
							//add edit view to make editing of item possible
							AddChild(m_editView);
							m_editView->MakeFocus();
							m_editItem = item;						
						}
					}
				} 
			}
		}
	}
	BOutlineListView::MouseDown(where);
}

void EditableListView::MessageReceived(BMessage *message)
{	
	switch (message->what) 
	{	
		case ListConstants::K_EDIT_COMPLETE_MSG:
		{
			//change listitem name
			EditableListItem *selectedItem = dynamic_cast<EditableListItem*>(ItemAt(m_editItem));
			selectedItem->SetText(m_editView->Text());
			//inform owner window that change has taken place
			BMessenger msgr(Window());
			message->AddString("newtext",m_editView->Text());
			BMessage clone = *selectedItem->ChangedMessage();
			msgr.SendMessage(&clone);			
			//stop editing name
			RemoveChild(m_editView);		
		}
		break;
		default:
			BOutlineListView::MessageReceived(message);	
		break;
	}
	
}

//===============================TextEditView====================================
TextEditView::TextEditView(BRect textframe, BRect textrect, const BHandler* handler)
			:	BTextView(textframe, NULL, textrect, B_FOLLOW_NONE, B_WILL_DRAW),
				m_handler(handler)				
{	
	SetMaxBytes(30);//see if this can be set by the user
}

TextEditView::~TextEditView()
{
}

void TextEditView::KeyDown(const char *bytes, int32 numBytes)
{
	switch (*bytes)
	{
		case B_ENTER:
			BMessenger(m_handler).SendMessage(new BMessage(ListConstants::K_EDIT_COMPLETE_MSG));
		break;
		default:
			BTextView::KeyDown(bytes,numBytes);
		break;
	}
}

void TextEditView::Draw(BRect updateRect)
{
	BTextView::Draw(updateRect);	
	//StrokeRect(rect);
}


