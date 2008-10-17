/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef PREFS_WINDOW_H
#include "PrefsWindow.h"
#endif

#include <app/Message.h>
#include <interface/View.h>
#include <interface/Box.h>
#include <interface/ListView.h>
#include <interface/OutlineListView.h>
#include <interface/ScrollView.h>
#include <interface/TextControl.h>
#include <interface/TextView.h>
#include <interface/CheckBox.h>
#include <interface/Button.h>
#include <interface/PopUpMenu.h>
#include <interface/MenuItem.h>
#include <interface/MenuField.h>
#include <translation/TranslationUtils.h>
#include <support/String.h>
#include "UserPictView.h"
#include "LayeredView.h"
#include "constants.h"
#include "Common.h"
#include "PrefsBitmaps.h"
#include "BitmapListItem.h"

PrefsWindow::PrefsWindow(BRect frame,const char *title,window_look look,window_feel feel,uint32 flags,uint32 workspace)
				:	BWindow(frame,title,look,feel,flags,workspace)
{	
	BView *backgroundView = new BView(Bounds(),"backgroundView", B_FOLLOW_ALL, B_WILL_DRAW);
	backgroundView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backgroundView);
	//Create listview to choose the different layers
	m_prefsListView = new BListView(BRect(10.0f,13.0f,160.0f,304.0f),"prefsList");
	m_prefsListView->SetInvocationMessage(new BMessage(InterfaceMessages::K_PREFS_SELECTED_MSG));
	m_prefsListView->SetSelectionMessage(new BMessage(InterfaceMessages::K_PREFS_SELECTED_MSG));
	
	m_prefsscroll = new BScrollView("prefsScroll", m_prefsListView);
	backgroundView->AddChild(m_prefsscroll);
	
	m_prefsListView->AddItem(new BitmapListItem(Common::ConvertBitmap(K_APP_PREFS_BITS, K_BITMAP_WIDTH, K_BITMAP_HEIGHT),"Application"));	
	m_prefsListView->AddItem(new BitmapListItem(Common::ConvertBitmap(K_USER_PREFS_BITS, K_BITMAP_WIDTH, K_BITMAP_HEIGHT), "User"));	
	m_prefsListView->AddItem(new BitmapListItem(Common::ConvertBitmap(K_CONTACT_PREFS_BITS, K_BITMAP_WIDTH, K_BITMAP_HEIGHT), "Contact List"));	
	m_prefsListView->AddItem(new BitmapListItem(Common::ConvertBitmap(K_BACKGROUND_PREFS_BITS, K_BITMAP_WIDTH, K_BITMAP_HEIGHT), "Backgrounds"));	
	m_prefsListView->AddItem(new BitmapListItem(Common::ConvertBitmap(K_SMILEY_PREFS_BITS, K_BITMAP_WIDTH, K_BITMAP_HEIGHT), "Emoticons"));
	//Create layerview with different views for different categories of preferences
	m_prefsLayerView = new LayeredView(BRect(170.0f,13.0f,527.0f,304.0f), "prefsLayerView",B_FOLLOW_ALL, B_WILL_DRAW);
	backgroundView->AddChild(m_prefsLayerView);
	//Add Layers, each layer contains a different view for a different preferences category
	BRect layerRect = m_prefsLayerView->Bounds();
	m_prefsLayerView->addLayer(CreateAppPrefsView(layerRect));
	m_prefsLayerView->addLayer(CreateUserPrefsView(layerRect));		
	m_prefsLayerView->addLayer(CreateContactListPrefsView(layerRect));
	m_prefsLayerView->addLayer(CreateBackgroundPrefsView(layerRect));
	m_prefsLayerView->addLayer(CreateEmoticonPrefsView(layerRect));
}

PrefsWindow::~PrefsWindow()
{
}

void PrefsWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case InterfaceMessages::K_PREFS_SELECTED_MSG:
		{			
			int32 selected = m_prefsListView->CurrentSelection();
			if (selected >= 0)
			{
				uint32 layer = uint32(selected);
				m_prefsLayerView->setVisible(layer);
			}
		}
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}

void PrefsWindow::SetActiveTab(BString activeTab)
{
}

BView* PrefsWindow::CreateAppPrefsView(BRect frame)
{
	BView *appPrefsView = new BView(frame,"userPrefsView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	appPrefsView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	return appPrefsView;
}

BView* PrefsWindow::CreateUserPrefsView(BRect frame)
{
	BView *userprefsview = new BView(frame,"userPrefsView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	userprefsview->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BRect userPictFrame(0.0f, 0.0f, 91.0f, 91.0f);
	BBitmap *bitmap = new BBitmap(userPictFrame, B_RGB32);		
	m_userpicview = new UserPictView(BRect(10.00, 21.00, 101.00, 111.00), bitmap, "userPicView",new BMessage(InterfaceMessages::K_USER_CHANGE_DISPLAYPIC_MSG), B_FOLLOW_NONE, 0);
	userprefsview->AddChild(m_userpicview);
	
	m_displaypicturecheck = new BCheckBox(BRect(6.00,130.00,151.00,157.00),"displayPictureCheck","Show my Display Picture",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	userprefsview->AddChild(m_displaypicturecheck);	
	//Create user prefs box
	m_userprefsbox = new BBox(BRect(114.00,10.00,354.00,122.00),"userPrefsBox",B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	m_userprefsbox->SetLabel("User Preferences");
	userprefsview->AddChild(m_userprefsbox);
	
	m_usernamecontrol = new BTextControl(BRect(6.00,18.00,215.00,43.00),"userNameControl","User Name:","",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_userprefsbox->AddChild(m_usernamecontrol);
	
	m_personalmessagetext = new BTextControl(BRect(6.00,49.00,215.00,71.00),"personalMessageText","Personal Message:","",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_userprefsbox->AddChild(m_personalmessagetext);
	
	m_musicplayingcheck = new BCheckBox(BRect(6.00,76.00,227.00,96.00),"musicPlayingCheck","Show music playing as personal message",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_userprefsbox->AddChild(m_musicplayingcheck);
	//Create phone number box
	m_phonenumberbox = new BBox(BRect(7.00,179.00,354.00,281.00),"phoneNumberBox",B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	m_phonenumberbox->SetLabel("Phone Numbers");
	userprefsview->AddChild(m_phonenumberbox);
	
	m_homephonecontrol = new BTextControl(BRect(10.00,15.00,268.00,32.00),"homePhoneControl","Home Phone:","",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_phonenumberbox->AddChild(m_homephonecontrol);
	
	m_workphonecontrol = new BTextControl(BRect(10.00,40.00,268.00,64.00),"workPhoneControl","Work Phone:","",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_phonenumberbox->AddChild(m_workphonecontrol);
	
	m_mobilephonecontrol = new BTextControl(BRect(10.00,65.00,268.00,96.00),"mobilePhoneControl","Mobile Phone:","",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_phonenumberbox->AddChild(m_mobilephonecontrol);	
	
	return userprefsview;
}

BView* PrefsWindow::CreateContactListPrefsView(BRect frame)
{
	BView *contactListPrefsView = new BView(frame,"userPrefsView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	contactListPrefsView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// Allocate object
	m_contactlistcolumnview = new BTextView(BRect(11.00,4.00,338.00,156.00),"contactListColumnView",BRect(0.00,0.00,110.00,110.00),B_FOLLOW_ALL_SIDES,B_WILL_DRAW);		
	contactListPrefsView->AddChild(m_contactlistcolumnview);	
		
	m_allowbutton = new BButton(BRect(11.00,161.00,64.00,191.00),"allowButton","Allow",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	contactListPrefsView->AddChild(m_allowbutton);
	
	m_deletebutton = new BButton(BRect(67.00,161.00,120.00,191.00),"deleteButton","Delete",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	contactListPrefsView->AddChild(m_deletebutton);
	
	m_blockbutton = new BButton(BRect(124.00,161.00,177.00,191.00),"blockButton","Block",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	contactListPrefsView->AddChild(m_blockbutton);	
	
	m_contactPrefsBox = new BBox(BRect(7.00,199.00,344.00,290.00),"contactPrefsBox",B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	contactListPrefsView->AddChild(m_contactPrefsBox);
	
	BPopUpMenu *orderingMenu = new BPopUpMenu("orderingMenu");
	orderingMenu->AddItem(new BMenuItem("online status", NULL));
	BMenuItem *groupsItem = new BMenuItem("groups", NULL);
	groupsItem->SetMarked(true);	
	orderingMenu->AddItem(groupsItem);
	
	m_orderingfield = new BMenuField(BRect(8.0f, 4.0f, 160.0f, 24.0f), "orderingField", "Sort contact list by",orderingMenu);
	m_orderingfield->SetDivider(100.0f);
	m_contactPrefsBox->AddChild(m_orderingfield);	
	
	m_contactNamesCheck = new BCheckBox(BRect(8.00,26.00,325.00,46.00),"contactNamesCheck","Allow editing contact names",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_contactPrefsBox->AddChild(m_contactNamesCheck);
	
	m_addcheck = new BCheckBox(BRect(8.00,48.00,325.00,68.00),"addCheck","Alert me when someone adds me to his/her Contact List",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_contactPrefsBox->AddChild(m_addcheck);
	
	m_blpcheck = new BCheckBox(BRect(8.00,68.00,325.00,88.00),"blpCheck","Allow people that are not in my Contact List to chat with me",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	m_contactPrefsBox->AddChild(m_blpcheck);	
	
	return contactListPrefsView;
}

BView* PrefsWindow::CreateBackgroundPrefsView(BRect frame)
{
	BView *backgroundPrefsView = new BView(frame,"userPrefsView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	backgroundPrefsView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BButton *addButton = new BButton(BRect(298.00, 9.00, 355.00, 40.00),"addButton","Add",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	backgroundPrefsView->AddChild(addButton);
	
	BButton *removeButton = new BButton(BRect(298.00, 48.00, 355.00, 79.00),"removeButton","Remove",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	backgroundPrefsView->AddChild(removeButton);
		
	m_downloadButton = new BButton(BRect(298.00, 87.00, 355.00, 118.00),"downloadButton","Download",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	backgroundPrefsView->AddChild(m_downloadButton);
	
	m_backgroundListView = new BOutlineListView(BRect(3.00,3.00,280.00 - B_V_SCROLL_BAR_WIDTH, frame.Height() - 26.0f),"backgroundListView", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE);
	BScrollView *backgroundScroll = new BScrollView("backgroundScroll", m_backgroundListView, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true);
	backgroundPrefsView->AddChild(backgroundScroll);	
	
	BCheckBox *backgroundCheck = new BCheckBox(BRect(25.0f, frame.Height() - 20.0f ,280.0f, frame.Height() - 1.00f), "backgroundCheck","Show selected background in chat window",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	backgroundPrefsView->AddChild(backgroundCheck);
		
	return backgroundPrefsView;
}

BView* PrefsWindow::CreateEmoticonPrefsView(BRect frame)
{
	BView *emoticonPrefsView = new BView(frame,"userPrefsView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	emoticonPrefsView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		
	BButton *addButton = new BButton(BRect(298.00, 9.00, 355.00, 40.00),"addButton","Add",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	emoticonPrefsView->AddChild(addButton);
	
	BButton *removeButton = new BButton(BRect(298.00, 48.00, 355.00, 79.00),"removeButton","Remove",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	emoticonPrefsView->AddChild(removeButton);
		
	m_downloadEmoticonsButton = new BButton(BRect(298.00, 87.00, 355.00, 118.00),"downloadButton","Download",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	emoticonPrefsView->AddChild(m_downloadEmoticonsButton);
	
	BButton *importButton = new BButton(BRect(298.00, 126.00, 355.00, 155.00),"importButton","Import",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	emoticonPrefsView->AddChild(importButton);
		
	BRect emoticonListFrame(3.00,3.00,280.00 - B_V_SCROLL_BAR_WIDTH, frame.Height() - 26.0f);	
	m_emoticonListView = new BOutlineListView(emoticonListFrame,"backgroundListView", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE);
	BScrollView *emoticonScroll = new BScrollView("emoticonScroll", m_emoticonListView, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true);
	emoticonPrefsView->AddChild(emoticonScroll);
		
	BCheckBox *emoticonCheck = new BCheckBox(BRect(25.0f, frame.Height() - 20.0f ,280.0f, frame.Height() - 1.00f), "emoticonCheck","Show emoticons throughout the program",NULL,B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	emoticonPrefsView->AddChild(emoticonCheck);
	
	return emoticonPrefsView;
}
