/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef EDITABLE_STRING_VIEW
#define EDITABLE_STRING_VIEW

#include <be/app/MessageFilter.h>
#include <be/interface/Rect.h>
#include <be/interface/Control.h>
#include <be/interface/ScrollView.h>
#include <be/interface/StringView.h>
#include <be/interface/TextView.h>

const uint32 K_CHANGE_VIEW_MSG = 'CHvw';

class EditableStringView : public BControl 
{
	public:
		EditableStringView(BRect frame, const char *name, const char *text, BMessage *message, const char* label = "", uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, uint32 flags = B_WILL_DRAW);
		virtual				~EditableStringView();
		
		virtual void		MessageReceived(BMessage *message);		
		virtual void		SetFont(const BFont *font, uint32 properties = B_FONT_ALL);
		void				SetText(const char *string);
		const char*			Text() const;
		
	private:
		BTextView			*m_editView;
		BScrollView			*m_editScroll;
		BStringView			*m_labelView;		
		bool				m_editing;
};

class MouseDownFilter : public BMessageFilter
{
	public:
		MouseDownFilter(BView *owner);
		virtual					~MouseDownFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
	
	private:
		BView					*m_owner;
};

class KeyFilter : public BMessageFilter
{
	public:
		KeyFilter(BView *owner);
		virtual					~KeyFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
	
	private:
		BView					*m_owner;
};

#endif
