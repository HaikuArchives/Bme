/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef EDITABLE_STRING_VIEW
#include "EditableStringView.h"
#endif

#include <be/app/Messenger.h>
#include <be/interface/Window.h>
#include <iostream>

EditableStringView::EditableStringView(BRect frame, const char *name, const char *text, BMessage *message, const char* label, uint32 resizingMode, uint32 flags)
						:	BControl(frame, name, label, message, resizingMode, flags),
							m_editing(false)
{	
	//construct label view
	m_labelView = new BStringView(Bounds(), name, text, resizingMode, flags);
	m_labelView->AddFilter(new MouseDownFilter(this));
	AddChild(m_labelView);
		
	//construct edit view
	BRect textRect = Bounds();
	m_editView = new BTextView(Bounds(),"editView", textRect, B_FOLLOW_ALL_SIDES, B_WILL_DRAW);	
	m_editView->AddFilter(new KeyFilter(this));
	m_editScroll = new BScrollView("editScroll",m_editView);	
	AddChild(m_editScroll);	
		
	m_editScroll->Hide();
}

EditableStringView::~EditableStringView()
{
}

void EditableStringView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case K_CHANGE_VIEW_MSG:
		{
			m_editing = !m_editing;
			if (m_editing)
			{
				//start the editing process
				m_editScroll->Show();
				m_editView->MakeFocus(true);
				m_editView->SelectAll();
				m_labelView->Hide();
			}
			else
			{	
				//end editing
				m_editScroll->Hide();
				m_labelView->Show();
				m_labelView->SetText(m_editView->Text());
				//notify user that the label has been changed
				Invoke();				
			}
		}
		break;
		default:
			BView::MessageReceived(message);
		break;
	}			
}

void EditableStringView::SetFont(const BFont *font, uint32 properties)
{
	m_labelView->SetFont(font, properties);
	m_editView->SetFont(font, properties);
	BView::SetFont(font, properties);
}

void EditableStringView::SetText(const char *string)
{
	m_labelView->SetText(string);
	m_editView->SetText(string);
}

const char*	EditableStringView::Text() const
{
	return m_editView->Text();
}

//======================================MouseDownFilter====================================================
MouseDownFilter::MouseDownFilter(BView *owner)
					:	BMessageFilter(B_ANY_DELIVERY,B_ANY_SOURCE),
						m_owner(owner)
{
}

MouseDownFilter::~MouseDownFilter()
{
}

filter_result MouseDownFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result = B_DISPATCH_MESSAGE;
	switch (message->what)
	{
		case B_MOUSE_DOWN:
		{	
			//send message to owner
			BMessenger owner(m_owner);
			owner.SendMessage(new BMessage(K_CHANGE_VIEW_MSG));
			result = B_SKIP_MESSAGE;			
		}
		break;
	}
	return result;
}

//=====================================KeyFilter===========================================================
KeyFilter::KeyFilter(BView *owner)
					:	BMessageFilter(B_ANY_DELIVERY,B_ANY_SOURCE),
						m_owner(owner)
{
}

KeyFilter::~KeyFilter()
{
}

filter_result KeyFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result = B_DISPATCH_MESSAGE;
	switch (message->what)
	{
		case B_KEY_DOWN:
		{	
			int8 byte;
			if (message->FindInt8("byte", &byte) == B_OK)
			{
				if (byte == B_ENTER)
				{
					//send message to owner
					BMessenger owner(m_owner);
					owner.SendMessage(new BMessage(K_CHANGE_VIEW_MSG));
					result = B_SKIP_MESSAGE;			
				}
			}				
		}
		break;
	}
	return result;
}
