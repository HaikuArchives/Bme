/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <app/Looper.h>
#include <app/Message.h>
#include <interface/Rect.h>
#include <interface/Window.h>
#include <interface/View.h>
#include <interface/TextControl.h>
#include <interface/CheckBox.h>
#include <interface/Button.h>
#include <interface/StringView.h>
#include <interface/MenuField.h>
#include <map>
#include "LayeredView.h"
#include "ComboBox.h"
#include "AnimatedBitmapView.h"
#include "ProtocolLooper.h"
#include "ContactListView.h"
#include "EditableStringView.h"
#include "StatusMenu.h"
#include "EditableListItem.h"
#include "Status.h"

class MainWindow : public BWindow
{
	public:
		MainWindow(BRect frame, ProtocolLooper *protocolLooper);
		virtual				~MainWindow();
		
		virtual void		MessageReceived(BMessage *message);		
		virtual bool		QuitRequested();
		
	private:
		BView				*constructLoginView(BRect frame);
		BView				*constructContactView(BRect frame);
		void				AddStatusItems(StatusMenu *statusMenu);
				
	private:
		ProtocolLooper		*m_protocolLooper;
		LayeredView			*m_layeredView;
		
		ComboBox			*m_username;
		BTextControl		*m_password;
		BCheckBox			*m_passwordSaveCheck;
		BButton				*m_signinButton;
		BBitmap				*m_statusBitmap;
		AnimatedBitmapView	*m_signinStatusView;
		BStringView			*m_statusBarView;		
		
		BMessage			m_passwordList;
			
		EditableStringView	*m_userNameView,
							*m_personalMessageView
							;
							
		Status				*m_currentStatus;
		StatusMenu 			*m_statusMenu;
		ContactListView		*m_contactListView;
				
		map<BString, EditableListItem*> groups;
		bool				m_signedIn;
		
		
	private:
		enum 
		{
			K_LOGIN_VIEW_LAYER =				0, 
			K_CONTACT_VIEW_LAYER =				1
		};
};

#endif
