/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <app/Message.h>
#include <app/Messenger.h>
#include <interface/Rect.h>
#include <interface/Window.h>
#include <interface/TextView.h>
#include <interface/ListView.h>
#include <interface/ScrollView.h>
#include <support/String.h>
#include "ConvLogView.h"
#include "Contact.h"
#include "GradientView.h"
#include "ContactPictureView.h"
#include "Toolbar.h"

class ChatWindow : public BWindow
{
	public:
		ChatWindow(BRect frame);
		virtual				~ChatWindow();
		
		void				SetOwner(BMessenger owner);
				
		virtual void		MessageReceived(BMessage *message);
		virtual bool		QuitRequested();
		
		void				AddContact(Contact *contact);
		void				RemoveContact(BString contactPassport);
		
	private:
		ConvLogView			*m_convLogView;
		BTextView			*m_convTextView;
		BListView			*m_contactDetailsView;

		BScrollView 		*m_convScroll,
							*m_convTextScroll,
							*m_contactScroll;
							
		WToolbar			*m_toolbarView;
		ContactPictureView	*m_contactPictureView;
		
		BMessenger			m_owner;
		
		bool				m_firstMessage;
};

#endif
