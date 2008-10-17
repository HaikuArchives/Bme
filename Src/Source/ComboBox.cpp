/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef COMBO_BOX_H
#include "ComboBox.h"
#endif

#include <be/app/Clipboard.h>
#include <be/app/Looper.h>
#include <be/support/String.h>
#include <be/interface/Window.h>
#include <be/interface/Polygon.h>
#include <be/interface/InterfaceDefs.h>
#include <be/interface/ScrollView.h>

#include <stdio.h>
#include <string.h>
#include <iostream>

ComboBox::ComboBox(BRect frame, const char *name, const char* text, uint32 resizingMode, uint32 flags)	
			:	BView(frame, name, resizingMode, flags | B_FRAME_EVENTS| B_NAVIGABLE | B_NAVIGABLE_JUMP | B_INPUT_METHOD_AWARE),
				m_opened(false),
				m_selectionMessage(NULL),
				m_invocationMessage(NULL)
{		
	//combobox textview
	BRect textViewFrame = Bounds();
	//make space for border
	textViewFrame.top += 3.0f;
	textViewFrame.bottom -= 3.0f;
	textViewFrame.right -= 2.0f;
	
	BRect textFrame(0,0,4.0f * textViewFrame.Width(),textViewFrame.Height());
	textViewFrame.left += ComboBox::K_COMBO_DIVIDER;
	
	m_comboTextView = new BTextView(textViewFrame,"combotextview",textFrame,resizingMode,flags | B_FRAME_EVENTS | B_NAVIGABLE | B_NAVIGABLE_JUMP);//watch resizingMode!!!
	m_comboTextView->SetWordWrap(false);
	AddChild(m_comboTextView);
	//combo popupwindow
	BRect popupFrame(0,0,frame.Width(),100.0f);
	m_popupwindow = new ComboPopUpWindow(this,popupFrame); 
	m_popupwindow->SetSelectionMessage(new BMessage(ComboConstants::K_COMBO_NEW_SELECTION_MSG));
	m_popupwindow->SetInvocationMessage(new BMessage(ComboConstants::K_COMBO_INVOCATION_MSG));
	//add messagefilter to textview
	m_comboTextView->AddFilter(new ComboBoxMessageFilter(m_popupwindow));
	
	m_popupwindow->Show();
	m_popupwindow->Hide();	
			
}

ComboBox::~ComboBox()
{		
	delete m_selectionMessage;
	delete m_invocationMessage;
}

void ComboBox::AttachedToWindow()
{	
	m_comboTextView->SetViewColor(255,255,255);
	//move drop down view to combobox view position
	BPoint leftTop(Bounds().LeftBottom());
	m_popupwindow->MoveTo(ConvertToScreen(leftTop));
		
	BView::AttachedToWindow();	
}

void ComboBox::Draw(BRect updaterect)
{		
	//move and resize dropdown window here, so the dropdown follows the textview
	//bit dirty, but couldn't find a better solution, anyone?
	BPoint leftTop(Bounds().LeftBottom());
	m_popupwindow->MoveTo(ConvertToScreen(leftTop));
	float popupHeight = (m_popupwindow->Bounds()).Height();
	m_popupwindow->ResizeTo(Bounds().Width(),popupHeight);
	
	rgb_color uicolor = {255,255,255,255};
	SetHighColor(uicolor);
	FillRect(Bounds());
	//===============From Haiku TextControl code==========================
	rgb_color no_tint = ui_color(B_PANEL_BACKGROUND_COLOR),
	lighten1 = tint_color(no_tint, B_LIGHTEN_1_TINT),
	lighten2 = tint_color(no_tint, B_LIGHTEN_2_TINT),
	lightenmax = tint_color(no_tint, B_LIGHTEN_MAX_TINT),
	darken1 = tint_color(no_tint, B_DARKEN_1_TINT),
	darken2 = tint_color(no_tint, B_DARKEN_2_TINT),
	darken4 = tint_color(no_tint, B_DARKEN_4_TINT),
	darkenmax = tint_color(no_tint, B_DARKEN_MAX_TINT),
	nav = keyboard_navigation_color();

	bool enabled = true; //IsEnabled();
	bool active = false;

	BRect rect = Bounds();
	rect.InsetBy(1.0f, 1.0f);
	if (m_comboTextView->IsFocus() && Window()->IsActive()) 
	{
		SetHighColor(nav);
		StrokeRect(rect);
	} 
	else 
	{
		if (enabled)
			SetHighColor(darken4);
		else
			SetHighColor(darken2);

		StrokeLine(rect.LeftTop(), rect.LeftBottom());
		StrokeLine(rect.LeftTop(), rect.RightTop());

		SetHighColor(no_tint);
		StrokeLine(BPoint(rect.left + 1.0f, rect.bottom), rect.RightBottom());
		StrokeLine(BPoint(rect.right, rect.top + 1.0f));
	}

	rect.InsetBy(-1.0f, -1.0f);

	if (enabled)
		SetHighColor(darken1);
	else
		SetHighColor(no_tint);

	StrokeLine(rect.LeftBottom(), rect.LeftTop());
	StrokeLine(rect.RightTop());

	if (enabled)
		SetHighColor(lighten2);
	else
		SetHighColor(lighten1);

	StrokeLine(BPoint(rect.left + 1.0f, rect.bottom), rect.RightBottom());
	StrokeLine(BPoint(rect.right, rect.top + 1.0f), rect.RightBottom());
	//============================================================================
	//draw the dropdown arrow, looks like BOutlineListView dropdown arrow
	DrawLatch(m_comboTextView->Bounds(), !m_opened);	
}

void ComboBox::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case ComboConstants::K_CLOSE_COMBO_MSG:
		case ComboConstants::K_OPEN_COMBO_MSG:
		{
			//open combo drop down window
			showDropDown();
			//send new selection message to owner window
			BMessenger owner(Window());
			//add selected text to message
			BMessage msg(ComboConstants::K_COMBO_NEW_SELECTION_MSG);				
			owner.SendMessage(&msg);
		}
		break;
		case ComboConstants::K_COMBO_NEW_SELECTION_MSG:
		{
			//send message through to owner window
			BMessenger owner(Window());
			if (m_selectionMessage)
				owner.SendMessage(m_selectionMessage);
		}
		break;	
		case ComboConstants::K_COMBO_INVOCATION_MSG:
		{
			//send message through to owner window
			BMessenger owner(Window());
			if (m_invocationMessage)
				owner.SendMessage(m_invocationMessage);
		}
		break;
		case ComboConstants::K_COMBO_SWITCH_FOCUS_MSG:
		{
			MakeFocus(!IsFocus());			
		}
		break;			
		default:
			BView::MessageReceived(message);
		break;
	}
}

void ComboBox::MouseDown(BPoint point)
{	
	//write test function for this!!!
	if (point.x < ComboBox::K_COMBO_DIVIDER)
	{
		showDropDown();
	}	
}

void ComboBox::MakeFocus(bool state)
{		
	//m_comboTextView->MakeFocus(inFocus);
	//BView::MakeFocus(inFocus);			
	if (state != m_comboTextView->IsFocus()) 
	{
		m_comboTextView->MakeFocus(state);

		if (state)
			m_comboTextView->SelectAll();
	}
}

//from Haiku TextControl class
void ComboBox::WindowActivated(bool active)
{
	if (m_comboTextView->IsFocus()) 
	{
		// invalidate to remove/show focus indication
		BRect rect = m_comboTextView->Frame();
		rect.InsetBy(-1, -1);
		Invalidate(rect);

		// help out embedded text view which doesn't
		// get notified of this
		m_comboTextView->Invalidate();
	}
}

void ComboBox::SetText(const char* text)
{
	m_comboTextView->SetText(text);
}

const char* ComboBox::Text()
{
	return m_comboTextView->Text();
}

//BList methods
bool ComboBox::AddItem(BStringItem *item)
{
	bool added = false;
	if (m_popupwindow->Lock())
	{
		added = m_popupwindow->AddItem(item);
		m_popupwindow->Unlock();
	}	
	return added;
}

bool ComboBox::AddItem(BStringItem *item, int32 atIndex)
{
	bool added = false;
	if (m_popupwindow->Lock())
	{
		added = m_popupwindow->AddItem(item,atIndex);
		m_popupwindow->Unlock();
	}	
	return added;
}

bool ComboBox::AddList(BList *newItems)
{
	bool added = false;
	if (m_popupwindow->Lock())
	{
		added = m_popupwindow->AddList(newItems);
		m_popupwindow->Unlock();
	}	
	return added;
}

bool ComboBox::AddList(BList *newItems, int32 atIndex)
{
	bool added = false;
	if (m_popupwindow->Lock())
	{
		added = m_popupwindow->AddList(newItems, atIndex);
		m_popupwindow->Unlock();
	}	
	return added;
}

bool ComboBox::RemoveItem(BStringItem *item)
{
	bool removed = false;
	if (m_popupwindow->Lock())
	{
		removed = m_popupwindow->RemoveItem(item);
		m_popupwindow->Unlock();
	}	
	return removed;
}

BStringItem* ComboBox::RemoveItem(int32 index)
{
	BStringItem *removedItem = NULL;
	if (m_popupwindow->Lock())
	{
		removedItem = m_popupwindow->RemoveItem(index);
		m_popupwindow->Unlock();
	}	
	return removedItem;
}

bool ComboBox::RemoveItems(int32 index, int32 count)
{
	bool removed = false;
	if (m_popupwindow->Lock())
	{
		removed = m_popupwindow->RemoveItems(index, count);
		m_popupwindow->Unlock();
	}	
	return removed;
}

void ComboBox::SetSelectionMessage(BMessage *message)
{
	m_selectionMessage = message;
}

void ComboBox::SetInvocationMessage(BMessage *message)
{
	m_invocationMessage = message;
}

BMessage* ComboBox::SelectionMessage() const
{	
	return m_selectionMessage;
}

uint32 ComboBox::SelectionCommand() const
{
	uint32 command = 0;
	if (m_selectionMessage)
	{
		command = m_selectionMessage->what;
	}	
	return command;
}

BMessage* ComboBox::InvocationMessage() const
{	
	return m_invocationMessage;
}

uint32 ComboBox::InvocationCommand() const
{
	uint32 command = 0;
	if (m_invocationMessage)
	{
		command = m_invocationMessage->what;
	}	
	return command;
}

BStringItem *ComboBox::ItemAt(int32 index) const
{
	BStringItem *item = NULL;
	if (m_popupwindow->Lock())
	{
		item = m_popupwindow->ItemAt(index);
		m_popupwindow->Unlock();
	}	
	return item;
}

int32 ComboBox::IndexOf(BPoint point) const
{
	int32 index = -1;
	if (m_popupwindow->Lock())
	{
		index = m_popupwindow->IndexOf(point);
		m_popupwindow->Unlock();
	}	
	return index;
}

int32 ComboBox::IndexOf(BStringItem *item) const
{
	int32 index = -1;
	if (m_popupwindow->Lock())
	{
		index = m_popupwindow->IndexOf(item);
		m_popupwindow->Unlock();
	}	
	return index;
}

BStringItem* ComboBox::FirstItem() const
{
	BStringItem *firstItem = NULL;
	if (m_popupwindow->Lock())
	{
		firstItem = m_popupwindow->FirstItem();
		m_popupwindow->Unlock();
	}	
	return firstItem;
}

BStringItem* ComboBox::LastItem() const
{
	BStringItem *lastItem = NULL;
	if (m_popupwindow->Lock())
	{
		lastItem = m_popupwindow->LastItem();
		m_popupwindow->Unlock();
	}	
	return lastItem;
}

bool ComboBox::HasItem(BStringItem *item) const
{
	bool hasItem = false;
	if (m_popupwindow->Lock())
	{
		hasItem = m_popupwindow->HasItem(item);
		m_popupwindow->Unlock();
	}	
	return hasItem;
}

int32 ComboBox::CountItems() const
{
	int32 noItems = -1;
	if (m_popupwindow->Lock())
	{
		noItems = m_popupwindow->CountItems();
		m_popupwindow->Unlock();
	}	
	return noItems;
}

void ComboBox::MakeEmpty()
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->MakeEmpty();
		m_popupwindow->Unlock();
	}		
}

bool ComboBox::IsEmpty() const
{
	bool empty = true;
	if (m_popupwindow->Lock())
	{
		empty = m_popupwindow->IsEmpty();
		m_popupwindow->Unlock();
	}	
	return empty;
}

void ComboBox::DoForEach(bool (*func)(BStringItem *))
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->DoForEach(func);
		m_popupwindow->Unlock();
	}	
}

void ComboBox::DoForEach(bool (*func)(BStringItem *, void *), void * pointer)
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->DoForEach(func, pointer);
		m_popupwindow->Unlock();
	}		
}

const BListItem** ComboBox::Items() const
{
	const BListItem **items;
	if (m_popupwindow->Lock())
	{
		items = m_popupwindow->Items();
		m_popupwindow->Unlock();
	}	
	return items;
}

void ComboBox::InvalidateItem(int32 index)
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->InvalidateItem(index);
		m_popupwindow->Unlock();
	}	
}

void ComboBox::ScrollToSelection()
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->ScrollToSelection();
		m_popupwindow->Unlock();
	}		
}

void ComboBox::Select(int32 index, bool extend)
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->Select(index, extend);
		m_popupwindow->Unlock();
	}	
}

void ComboBox::Select(int32 from, int32 to, bool extend)
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->Select(from, to, extend);
		m_popupwindow->Unlock();
	}		
}

bool ComboBox::IsItemSelected(int32 index) const
{
	bool selected = false;
	if (m_popupwindow->Lock())
	{
		selected = m_popupwindow->IsItemSelected(index);
		m_popupwindow->Unlock();
	}	
	return selected;
}

int32 ComboBox::CurrentSelection(int32 index) const
{
	int32 currentSelection = -1;
	if (m_popupwindow->Lock())
	{
		currentSelection = m_popupwindow->CurrentSelection(index);
		m_popupwindow->Unlock();
	}	
	return currentSelection;
}

void ComboBox::DeselectAll()
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->DeselectAll();
		m_popupwindow->Unlock();
	}	
}

void ComboBox::DeselectExcept(int32 except_from, int32 except_to)
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->DeselectExcept(except_from,except_to);
		m_popupwindow->Unlock();
	}	
}

void ComboBox::Deselect(int32 index)
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->Deselect(index);
		m_popupwindow->Unlock();
	}	
}

void ComboBox::SortItems(int (*cmp)(const void *, const void *))
{
	if (m_popupwindow->Lock())
	{
		m_popupwindow->SortItems(cmp);
		m_popupwindow->Unlock();
	}	
}

bool ComboBox::SwapItems(int32 a, int32 b)
{
	bool swapped = false;
	if (m_popupwindow->Lock())
	{
		swapped = m_popupwindow->SwapItems(a, b);
		m_popupwindow->Unlock();
	}	
	return swapped;
}

bool ComboBox::MoveItem(int32 from, int32 to)
{
	bool moved = false;
	if (m_popupwindow->Lock())
	{
		moved = m_popupwindow->MoveItem(from, to);
		m_popupwindow->Unlock();
	}	
	return moved;
}

bool ComboBox::ReplaceItem(int32 index, BStringItem *item)
{
	bool replaced = false;
	if (m_popupwindow->Lock())
	{
		replaced = m_popupwindow->ReplaceItem(index, item);
		m_popupwindow->Unlock();
	}	
	return replaced;
}

BRect ComboBox::ItemFrame(int32 index)
{
	BRect frame;
	if (m_popupwindow->Lock())
	{
		frame = m_popupwindow->ItemFrame(index);
		m_popupwindow->Unlock();
	}	
	return frame;
}

void ComboBox::showDropDown()
{
	if(!m_opened)
	{
		//show combo drop down list
		m_popupwindow->LockLooper();
		m_popupwindow->Show();
		m_popupwindow->UnlockLooper();
			
		m_opened = true;
		Invalidate();
	}
	else
	{
		//show combo drop down list
		m_popupwindow->LockLooper();
		m_popupwindow->Hide();
		m_popupwindow->UnlockLooper();
		
		m_opened = false;
		Invalidate();
	}
}

//========================from Haiku BOutlineListView code================================================
void ComboBox::DrawLatch(BRect itemRect, bool collapsed)
{
	float itemHeight = itemRect.Height();
	float maxLatchHeight = 8.0f;
	float latchY = (itemHeight - maxLatchHeight) / 2.0f;

	if (collapsed)
	{		
		SetHighColor(192, 192, 192);
			
		FillTriangle(itemRect.LeftTop() + BPoint(4.0f, latchY + 2.0f),
			itemRect.LeftTop() + BPoint(4.0f, latchY + 10.0f),
			itemRect.LeftTop() + BPoint(8.0f, latchY + 6.0f));

		SetHighColor(0, 0, 0);

		StrokeTriangle(itemRect.LeftTop() + BPoint(4.0f, latchY + 2.0f),
			itemRect.LeftTop() + BPoint(4.0f, latchY + 10.0f),
			itemRect.LeftTop() + BPoint(8.0f, latchY + 6.0f));
	}
	else
	{
		SetHighColor(192, 192, 192);
			
		FillTriangle(itemRect.LeftTop() + BPoint(2.0f, latchY + 4.0f),
			itemRect.LeftTop() + BPoint(10.0f, latchY + 4.0f),
			itemRect.LeftTop() + BPoint(6.0f, latchY + 8.0f));

		SetHighColor(0, 0, 0);

		StrokeTriangle(itemRect.LeftTop() + BPoint(2.0f, latchY + 4.0f),
			itemRect.LeftTop() + BPoint(10.0f, latchY + 4.0f),
			itemRect.LeftTop() + BPoint(6.0f, latchY + 8.0f));
	}
}
//========================================================================================================

ComboPopUpViewMessageFilter::ComboPopUpViewMessageFilter()
								:	BMessageFilter(B_ANY_DELIVERY, B_ANY_SOURCE),
									m_lastbutton(0)
{
}

ComboPopUpViewMessageFilter::~ComboPopUpViewMessageFilter()
{
}

filter_result ComboPopUpViewMessageFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result(B_DISPATCH_MESSAGE);
	
	switch(message->what)
	{
		case B_MOUSE_DOWN:
		{
			int32 buttons, clicks;
			buttons = message->FindInt32("buttons");
			clicks = message->FindInt32("clicks");
			
			BMessage *selectmsg = new BMessage(ComboConstants::K_COMBO_SELECT_MOUSE_MSG);
			
			if(buttons == m_lastbutton && clicks > 1)
			{
				selectmsg->AddBool("doubleclick", true);
			}
			m_lastbutton = buttons;
						
			BMessenger msgr((*target));
			msgr.SendMessage(selectmsg);
			
			delete selectmsg;
			
			result = B_DISPATCH_MESSAGE;
					
		}
		break;				
	}
	return result;
}

ComboPopUpWindow::ComboPopUpWindow(ComboBox* parent, BRect frame)
					:	BWindow(frame,"ComboPopUpWindow",B_NO_BORDER_WINDOW_LOOK,
							B_FLOATING_APP_WINDOW_FEEL,	B_NOT_MOVABLE | B_NOT_CLOSABLE | B_NOT_ZOOMABLE |
							B_NOT_MINIMIZABLE | B_WILL_ACCEPT_FIRST_CLICK |
							B_AVOID_FOCUS | B_ASYNCHRONOUS_CONTROLS,
							B_CURRENT_WORKSPACE),
							m_parentview(parent),
							m_lastitem(0)										
							
{	
	BRect listFrame = Bounds();
	listFrame.InsetBy(1.0f,1.0f);
	listFrame.right -= B_V_SCROLL_BAR_WIDTH + 1.0f;
	
	m_combopopupview = new BListView(listFrame,"combopopupview");
	m_combopopupview->AddFilter(new ComboPopUpViewMessageFilter());
	
	BScrollView *listScroll = new BScrollView("listScroll",m_combopopupview, B_FOLLOW_LEFT | B_FOLLOW_TOP,0,false,true);	
	AddChild(listScroll);
}

ComboPopUpWindow::ComboPopUpWindow(ComboBox* parent, BRect frame, BList *items)
					:	BWindow(frame,"ComboPopUpWindow",B_NO_BORDER_WINDOW_LOOK,
							B_FLOATING_APP_WINDOW_FEEL,	B_NOT_MOVABLE | B_NOT_CLOSABLE | B_NOT_ZOOMABLE |
							B_NOT_MINIMIZABLE | B_WILL_ACCEPT_FIRST_CLICK |
							B_AVOID_FOCUS | B_ASYNCHRONOUS_CONTROLS,
							B_CURRENT_WORKSPACE),							
							m_parentview(parent),
							m_lastitem(0)							
{
	//replace by calling other constructor?
	BRect listFrame = Bounds();
	listFrame.InsetBy(1.0f,1.0f);
	listFrame.right -= B_V_SCROLL_BAR_WIDTH + 1.0f;
	
	m_combopopupview = new BListView(listFrame,"combopopupview");
	m_combopopupview->AddFilter(new ComboPopUpViewMessageFilter());
	
	BScrollView *listScroll = new BScrollView("listScroll",m_combopopupview, B_FOLLOW_LEFT | B_FOLLOW_TOP,0,false,true);	
	AddChild(listScroll);
	
	m_combopopupview->AddList(items);
}

ComboPopUpWindow::~ComboPopUpWindow()
{
	//BStringItem* item = NULL;
	//delete list????
	/*for(int32 i = 0; i < m_combo_list->CountItems(); i++)
	{
		item = (BStringItem*)m_combo_list->RemoveItem((int32)0);
		delete item;
	}	
	delete m_combo_list;	*/
}

void ComboPopUpWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case ComboConstants::K_COMBO_TYPED_MSG:
		{
			//find an item in the list that has the same prefix as the typed text			
		}
		break;
		case ComboConstants::K_COMBO_SELECT_MOUSE_MSG:
		{			
			int32 selectedIndex = m_combopopupview->CurrentSelection();
			if (selectedIndex != B_ERROR)
			{
				BStringItem* item = (BStringItem*)m_combopopupview->ItemAt(selectedIndex);						
				if (m_parentview->LockLooper())
				{
					m_parentview->SetText(item->Text());
					m_parentview->UnlockLooper();
				}
				// doubleclick opens url
				uint32 clickitem = m_combopopupview->CurrentSelection();
				if(message->HasBool("doubleclick"))
				{
					if(message->FindBool("doubleclick") == true)
					{
						if(clickitem == m_lastitem)
						{
							BMessenger msgr(m_parentview);
							msgr.SendMessage(new BMessage(ComboConstants::K_OPEN_COMBO_MSG));
						}
					}
				}
				m_lastitem = clickitem;						
			}
		}
		break;
		case ComboConstants::K_COMBO_SELECT_NEXT_MSG:
		{
			Lock();			
			int32 selectedIndex = m_combopopupview->CurrentSelection();
			if (selectedIndex != B_ERROR)
			{
				m_combopopupview->Select(selectedIndex + 1);
				m_combopopupview->ScrollToSelection();
							
				BStringItem* item = (BStringItem*)m_combopopupview->ItemAt(selectedIndex);
				if(!item)
					break;
				
				m_parentview->SetText(item->Text());
			}						
			Unlock();							
		}
		break;
		case ComboConstants::K_COMBO_SELECT_PREV_MSG:
		{
			Lock();
			int32 selectedIndex = m_combopopupview->CurrentSelection();
			if (selectedIndex != B_ERROR)
			{
				m_combopopupview->Select(selectedIndex - 1);
				m_combopopupview->ScrollToSelection();
						
				BStringItem* item = (BStringItem*)m_combopopupview->ItemAt(selectedIndex);
				if(!item)
					break;
			
				m_parentview->SetText(item->Text());
			}						
			Unlock();			
		}
		break;
		case ComboConstants::K_COMBO_NEW_SELECTION_MSG:
		case ComboConstants::K_COMBO_INVOCATION_MSG:
		{									
			BMessenger msgr(m_parentview);
			msgr.SendMessage(message);
		}
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}

void ComboPopUpWindow::ResizeToPrefered()
{
	BPoint parent_bottom = m_parentview->Bounds().RightBottom();
	BPoint parent_bottom_cts = m_parentview->ConvertToScreen(parent_bottom);
	
	float itemheight = m_combopopupview->ItemAt(0)->Height();
	int32 count = m_combopopupview->CountItems();
	
	float newheight = (itemheight * count) + (1.9 * count) + 1;
		
	BPoint new_popup_bottom;
	new_popup_bottom.y = newheight;
	BPoint new_popup_bottom_cts = ConvertToScreen(new_popup_bottom);
	
	float difference = parent_bottom_cts.y - new_popup_bottom_cts.y - 20;
	
	float textview_width = m_parentview->Bounds().right;
	
	if(difference < 0)
	{
		// the urlpopup would reach over the parentwindows bottom
		
		new_popup_bottom.y -= -difference;
		ResizeTo(textview_width, new_popup_bottom.y);		
	}
	else
	{
		// the urlpopup fits into the parentwindow		
		ResizeTo(textview_width, new_popup_bottom.y);		
	}
}

//BList methods
bool ComboPopUpWindow::AddItem(BStringItem *item)
{
	return m_combopopupview->AddItem(item);
}

bool ComboPopUpWindow::AddItem(BStringItem *item, int32 atIndex)
{
	return m_combopopupview->AddItem(item,atIndex);
}

bool ComboPopUpWindow::AddList(BList *newItems)
{
	return m_combopopupview->AddList(newItems);
}

bool ComboPopUpWindow::AddList(BList *newItems, int32 atIndex)
{
	return m_combopopupview->AddList(newItems, atIndex);
}

bool ComboPopUpWindow::RemoveItem(BStringItem *item)
{
	return m_combopopupview->RemoveItem(item);
}

BStringItem* ComboPopUpWindow::RemoveItem(int32 index)
{
	return dynamic_cast<BStringItem*>(m_combopopupview->RemoveItem(index));
}

bool ComboPopUpWindow::RemoveItems(int32 index, int32 count)
{
	return m_combopopupview->RemoveItems(index, count);
}

void ComboPopUpWindow::SetSelectionMessage(BMessage *message)
{
	m_combopopupview->SetSelectionMessage(message);
}

void ComboPopUpWindow::SetInvocationMessage(BMessage *message)
{
	m_combopopupview->SetInvocationMessage(message);
}

BMessage* ComboPopUpWindow::SelectionMessage() const
{
	return m_combopopupview->SelectionMessage();
}

uint32 ComboPopUpWindow::SelectionCommand() const
{
	return m_combopopupview->SelectionCommand();
}

BMessage* ComboPopUpWindow::InvocationMessage() const
{
	return m_combopopupview->InvocationMessage();
}

uint32 ComboPopUpWindow::InvocationCommand() const
{
	return m_combopopupview->InvocationCommand();
}

BStringItem *ComboPopUpWindow::ItemAt(int32 index) const
{
	return dynamic_cast<BStringItem*>(m_combopopupview->ItemAt(index));
}

int32 ComboPopUpWindow::IndexOf(BPoint point) const
{
	return m_combopopupview->IndexOf(point);
}

int32 ComboPopUpWindow::IndexOf(BStringItem *item) const
{
	return m_combopopupview->IndexOf(item);
}

BStringItem* ComboPopUpWindow::FirstItem() const
{
	return dynamic_cast<BStringItem*>(m_combopopupview->FirstItem());
}

BStringItem* ComboPopUpWindow::LastItem() const
{
	return dynamic_cast<BStringItem*>(m_combopopupview->LastItem());
}

bool ComboPopUpWindow::HasItem(BStringItem *item) const
{
	return m_combopopupview->HasItem(item);
}

int32 ComboPopUpWindow::CountItems() const
{
	return m_combopopupview->CountItems();
}

void ComboPopUpWindow::MakeEmpty()
{
	m_combopopupview->MakeEmpty();
}

bool ComboPopUpWindow::IsEmpty() const
{
	return m_combopopupview->IsEmpty();
}

void ComboPopUpWindow::DoForEach(bool (*func)(BStringItem *))
{
	for (int32 i = 0; i < CountItems(); i++)
	{
		BStringItem *item = ItemAt(i);
		func(item);
	}
}

void ComboPopUpWindow::DoForEach(bool (*func)(BStringItem *, void *arg2), void *arg2)
{
	for (int32 i = 0; i < CountItems(); i++)
	{
		BStringItem *item = ItemAt(i);
		func(item, arg2);
	}
}

const BListItem** ComboPopUpWindow::Items() const
{		
	return m_combopopupview->Items();
}

void ComboPopUpWindow::InvalidateItem(int32 index)
{
	m_combopopupview->InvalidateItem(index);
}

void ComboPopUpWindow::ScrollToSelection()
{
	m_combopopupview->ScrollToSelection();
}

void ComboPopUpWindow::Select(int32 index, bool extend)
{
	m_combopopupview->Select(index, extend);
}

void ComboPopUpWindow::Select(int32 from, int32 to, bool extend)
{
	m_combopopupview->Select(from, to, extend);
}

bool ComboPopUpWindow::IsItemSelected(int32 index) const
{
	return m_combopopupview->IsItemSelected(index);
}

int32 ComboPopUpWindow::CurrentSelection(int32 index) const
{
	return m_combopopupview->CurrentSelection(index);
}

void ComboPopUpWindow::DeselectAll()
{
	m_combopopupview->DeselectAll();
}

void ComboPopUpWindow::DeselectExcept(int32 except_from, int32 except_to)
{
	m_combopopupview->DeselectExcept(except_from,except_to);
}

void ComboPopUpWindow::Deselect(int32 index)
{
	m_combopopupview->Deselect(index);
}

void ComboPopUpWindow::SortItems(int (*cmp)(const void *, const void *))
{
	m_combopopupview->SortItems(cmp);
}

bool ComboPopUpWindow::SwapItems(int32 a, int32 b)
{
	return m_combopopupview->SwapItems(a, b);
}

bool ComboPopUpWindow::MoveItem(int32 from, int32 to)
{
	return m_combopopupview->MoveItem(from, to);
}

bool ComboPopUpWindow::ReplaceItem(int32 index, BStringItem *item)
{
	return m_combopopupview->ReplaceItem(index, item);
}

BRect ComboPopUpWindow::ItemFrame(int32 index)
{
	return m_combopopupview->ItemFrame(index);
}

ComboBoxMessageFilter::ComboBoxMessageFilter(BWindow *popupwindow)
						:	BMessageFilter(B_ANY_DELIVERY, B_ANY_SOURCE),
							m_combopopupwindow(popupwindow)
{	
}

filter_result ComboBoxMessageFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result(B_DISPATCH_MESSAGE);
	
	switch(message->what)
	{
		case B_KEY_DOWN:
		{									
			const char* key = message->FindString("bytes");			
			BMessenger messagetarget(*target);
			
			switch(key[0])
			{
				case B_DOWN_ARROW: 
				{
					BMessenger msgr(m_combopopupwindow);
					msgr.SendMessage(ComboConstants::K_COMBO_SELECT_NEXT_MSG);
					
					messagetarget.SendMessage(ComboConstants::K_COMBO_NEW_SELECTION_MSG);
					result = B_SKIP_MESSAGE;							
				}
				break;
				case B_UP_ARROW:
				{					
					BMessenger msgr(m_combopopupwindow);
					msgr.SendMessage(ComboConstants::K_COMBO_SELECT_PREV_MSG);
					
					messagetarget.SendMessage(ComboConstants::K_COMBO_NEW_SELECTION_MSG);										
					result = B_SKIP_MESSAGE;					
				}
				break;
				case B_ESCAPE:
				{
					messagetarget.SendMessage(ComboConstants::K_CLOSE_COMBO_MSG);
					result = B_SKIP_MESSAGE;					
				}
				break;
				// the next 2 cases need to be in here...
				// dont ask me why, but when i am really intensively using
				// _all_ arrow keys to navigate themis sometimes hangs ( no reall crash )
				// if i put those 2 in, i can't reprocude the 'hanging'
				// this is really weird
				case B_LEFT_ARROW:
					break;
				case B_RIGHT_ARROW:
					break;
				case B_ENTER:
				{					
					result = B_SKIP_MESSAGE;										
				}
				break;
				case B_TAB:
				{					
					messagetarget.SendMessage(ComboConstants::K_COMBO_SWITCH_FOCUS_MSG);
					result = B_SKIP_MESSAGE;
				}
				break;
				default:				
					messagetarget.SendMessage(ComboConstants::K_COMBO_TYPED_MSG);			
				break;
			}
		}		
	}
	return result;
}
