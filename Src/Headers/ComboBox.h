/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include <be/app/MessageFilter.h>
#include <be/interface/Rect.h>
#include <be/interface/Point.h>
#include <be/interface/Window.h>
#include <be/interface/View.h>
#include <be/interface/Bitmap.h>
#include <be/interface/TextView.h>
#include <be/interface/GraphicsDefs.h>
#include <be/interface/ListView.h>
#include <be/interface/TextControl.h>

class ComboTextView;
class ComboPopUpWindow;

namespace ComboConstants
{
	const uint32 
			K_OPEN_COMBO_MSG =			'OPco',
			K_CLOSE_COMBO_MSG =			'CLco',
			K_COMBO_TYPED_MSG =			'COty',
			K_COMBO_NEW_SELECTION_MSG =	'COne',
			K_COMBO_SELECT_NEXT_MSG =	'COsn',
			K_COMBO_SELECT_PREV_MSG	=	'COpr',
			K_COMBO_SELECT_MOUSE_MSG =	'COsm',
			K_COMBO_SWITCH_FOCUS_MSG =	'COsf',
			K_COMBO_INVOCATION_MSG =	'COim'
			;
};

/**	Combobox class based on Themis UrlView class.
	
	Themis' UrlView class
	@author Raymond "Z3R0 One" Rodgers
	
	Bme Combobox class
	@author Tim de Jong
	@comment adapted to Bme coding style
	TODO: needs lots of work, when finished adapting to style:
		1) change looks to plain BeOS looks
		2) implement search mechanism, that looks for best suitable
		   string in combobox when typing something in textview of it.
		3) See if we can get rid of ComboListView!
		4) See what methods can be added to ComboBox, everything should
		   be accessible from this class, rest of the classes are helper
		   classes, Combobox should encapsulate everything.
 */
class ComboBox : public BView
{
	public:
		ComboBox(BRect frame, const char *name,const char* text, uint32 resizingMode, uint32 flags);
		virtual				~ComboBox();
										
		virtual void		AttachedToWindow();											 	 
		virtual void		Draw(BRect updaterect);
		virtual void		MessageReceived(BMessage *message);
		virtual void		MouseDown(BPoint point);
		virtual void		MakeFocus(bool inFocus = true);
		virtual void		WindowActivated(bool active);
				
		virtual void		SetText(const char* text);
		virtual const char*	Text();		
		//BList methods
		virtual	bool		AddItem(BStringItem *item);
		virtual bool		AddItem(BStringItem *item, int32 atIndex);
		virtual bool		AddList(BList *newItems);
		virtual bool		AddList(BList *newItems, int32 atIndex);
		virtual bool		RemoveItem(BStringItem *item);
		virtual BStringItem	*RemoveItem(int32 index);
		virtual bool		RemoveItems(int32 index, int32 count);

		virtual	void		SetSelectionMessage(BMessage *message);
		virtual	void		SetInvocationMessage(BMessage *message);

		BMessage			*SelectionMessage() const;
		uint32				SelectionCommand() const;
		BMessage			*InvocationMessage() const;
		uint32				InvocationCommand() const;

		BStringItem			*ItemAt(int32 index) const;
		int32				IndexOf(BPoint point) const;
		int32				IndexOf(BStringItem *item) const;
		BStringItem			*FirstItem() const;
		BStringItem			*LastItem() const;
		bool				HasItem(BStringItem *item) const;
		int32				CountItems() const;
		virtual	void		MakeEmpty();
		bool				IsEmpty() const;
		void				DoForEach(bool (*func)(BStringItem *));
		void				DoForEach(bool (*func)(BStringItem *, void *), void *);
		const BListItem		**Items() const;
		void				InvalidateItem(int32 index);
		void				ScrollToSelection();

		void				Select(int32 index, bool extend = false);
		void				Select(int32 from, int32 to, bool extend = false);
		bool				IsItemSelected(int32 index) const;
		int32				CurrentSelection(int32 index = 0) const;
		
		void				DeselectAll();
		void				DeselectExcept(int32 except_from, int32 except_to);
		void				Deselect(int32 index);
		void				SortItems(int (*cmp)(const void *, const void *));
		bool				SwapItems(int32 a, int32 b);
		bool				MoveItem(int32 from, int32 to);
		bool				ReplaceItem(int32 index, BStringItem * item);
		BRect				ItemFrame(int32 index);

	public:
		enum
		{
			K_COMBO_DIVIDER = 12,
			K_COMBO_ARROW_WIDTH_A = 6,
			K_COMBO_ARROW_HEIGHT_A = 5,
			K_COMBO_ARROW_WIDTH_B = 5,
			K_COMBO_ARROW_HEIGHT_B = 6
		};					
		
	private:
		void				showDropDown();
		void				DrawLatch(BRect itemRect, bool collapsed);
		
	private:	
		ComboPopUpWindow	*m_popupwindow;
		BTextView			*m_comboTextView;		
		bool				m_opened;	
		
		BMessage			*m_selectionMessage,
							*m_invocationMessage;
		
};

class ComboPopUpViewMessageFilter : public BMessageFilter
{
	public:
		ComboPopUpViewMessageFilter();
		virtual					~ComboPopUpViewMessageFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
		
	public:	
		int32					m_lastbutton;
};

class ComboPopUpWindow : public BWindow
{
	public:
		ComboPopUpWindow(ComboBox* parent, BRect frame);
		ComboPopUpWindow(ComboBox* parent, BRect frame, BList *items);
		virtual				~ComboPopUpWindow();
									
		virtual void		MessageReceived(BMessage *message);		
		void				ResizeToPrefered();
		
		//BList methods
		virtual	bool		AddItem(BStringItem *item);
		virtual bool		AddItem(BStringItem *item, int32 atIndex);
		virtual bool		AddList(BList *newItems);
		virtual bool		AddList(BList *newItems, int32 atIndex);
		virtual bool		RemoveItem(BStringItem *item);
		virtual BStringItem	*RemoveItem(int32 index);
		virtual bool		RemoveItems(int32 index, int32 count);

		virtual	void		SetSelectionMessage(BMessage *message);
		virtual	void		SetInvocationMessage(BMessage *message);

		BMessage			*SelectionMessage() const;
		uint32				SelectionCommand() const;
		BMessage			*InvocationMessage() const;
		uint32				InvocationCommand() const;

		BStringItem			*ItemAt(int32 index) const;
		int32				IndexOf(BPoint point) const;
		int32				IndexOf(BStringItem *item) const;
		BStringItem			*FirstItem() const;
		BStringItem			*LastItem() const;
		bool				HasItem(BStringItem *item) const;
		int32				CountItems() const;
		virtual	void		MakeEmpty();
		bool				IsEmpty() const;
		void				DoForEach(bool (*func)(BStringItem *));
		void				DoForEach(bool (*func)(BStringItem *, void *arg2), void *arg2);
		const  BListItem**	Items() const;
		void				InvalidateItem(int32 index);
		void				ScrollToSelection();

		void				Select(int32 index, bool extend = false);
		void				Select(int32 from, int32 to, bool extend = false);
		bool				IsItemSelected(int32 index) const;
		int32				CurrentSelection(int32 index = 0) const;
		
		void				DeselectAll();
		void				DeselectExcept(int32 except_from, int32 except_to);
		void				Deselect(int32 index);
		void				SortItems(int (*cmp)(const void *, const void *));
		bool				SwapItems(int32 a, int32 b);
		bool				MoveItem(int32 from, int32 to);
		bool				ReplaceItem(int32 index, BStringItem * item);
		BRect				ItemFrame(int32 index);
		
	public:	
		BListView			*m_combopopupview;		
		
	private:
		ComboBox			*m_parentview;
		
		uint32				m_lastitem;		
							
};



class ComboBoxMessageFilter : public BMessageFilter
{
	public:
		ComboBoxMessageFilter(BWindow *popupwindow);
		virtual filter_result Filter(BMessage *message, BHandler **target);

	public:	
		BWindow				*m_combopopupwindow;
	
};

#endif
