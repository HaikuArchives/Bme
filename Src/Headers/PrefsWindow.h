/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef PREFS_WINDOW_H
#define PREFS_WINDOW_H

#include <interface/Window.h>

class BString;
class BMessage;
class BScrollView;
class BView;
class BBox;
class BTextControl;
class BCheckBox;
class LayeredView;
class BListView;
class BBitmap;
class BOutlineListView;
class BButton;
class BTextView;
class BMenuField;
class UserPictView;

class PrefsWindow : public BWindow
{
	public:
		PrefsWindow(BRect frame,const char * title,window_look look,window_feel feel,uint32 flags,uint32 workspace = B_CURRENT_WORKSPACE);
		virtual				~PrefsWindow();
		
		virtual void		MessageReceived(BMessage *message);
		virtual void		SetActiveTab(BString activeTab);
		
	private:
		BView*				CreateAppPrefsView(BRect frame);
		BView*				CreateUserPrefsView(BRect frame);
		BView*				CreateContactListPrefsView(BRect frame);
		BView*				CreateBackgroundPrefsView(BRect frame);
		BView*				CreateEmoticonPrefsView(BRect frame);		
		
	private:
		BScrollView			*m_prefsscroll;
		LayeredView			*m_prefsLayerView;
		BListView			*m_prefsListView;
		
		//app prefs view
		
		//user prefs view
		UserPictView		*m_userpicview;
							
		BBox				*m_userprefsbox,
							*m_phonenumberbox
							;
							
		BTextControl		*m_usernamecontrol,
							*m_personalmessagetext
							;
				
		BTextControl		*m_homephonecontrol,
							*m_mobilephonecontrol,
							*m_workphonecontrol
							;
		
		BCheckBox			*m_musicplayingcheck,
							*m_displaypicturecheck
							;		
		
		//contact prefs view		
		BTextView			*m_contactlistcolumnview;
		BMenuField			*m_orderingfield;
	
		BCheckBox			*m_blpcheck,
							*m_addcheck,
							*m_contactNamesCheck
							;	
		
		BButton				*m_allowbutton,
							*m_deletebutton,
							*m_blockbutton
							;
				
		BBox				*m_contactPrefsBox;

		
		//background prefs view
		BOutlineListView	*m_backgroundListView;
		BButton				*m_downloadButton;	
		
		//emoticon prefs view
		BOutlineListView	*m_emoticonListView;
		BButton				*m_downloadEmoticonsButton;
		
};

#endif
