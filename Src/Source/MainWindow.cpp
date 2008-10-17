/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MAIN_WINDOW_H
#include "MainWindow.h"
#endif

#include <app/Application.h>
#include <app/Messenger.h>
#include <interface/Bitmap.h>
#include <interface/Font.h>
#include <interface/MenuBar.h>
#include <interface/Menu.h>
#include <interface/MenuItem.h>
#include <interface/ScrollView.h>
#include <interface/ListItem.h>
#include <interface/Alert.h>
#include <support/String.h>
#include <support/ClassInfo.h>
#include <translation/TranslationUtils.h>
#include "constants.h"
#include "GradientView.h"
#include "Preferences.h"
#include "MailView.h"
#include "UserPictView.h"
#include "Statusses.h"
#include "StatusItem.h"
#include "MessageFields.h"
#include "ProtocolConstants.h"
#include "ContactItem.h"
#include "PrefsMethods.h"
//#include "EditableListItem.h"

#include <iostream>

#include "AddNewContactWindow.h"

MainWindow::MainWindow(BRect frame, ProtocolLooper *protocolLooper)
			:	BWindow(frame, "Bme",B_TITLED_WINDOW,0),
				m_protocolLooper(protocolLooper),
				m_signedIn(false)	
{	
	//set protocolLooper owner to this MainWindow!
	if (m_protocolLooper->Lock())
	{
		m_protocolLooper->setOwner(this);
		m_protocolLooper->Unlock();
	}
	//construct menu
	BMenuBar *menuBar = new BMenuBar(BRect(0,0,0,0),"mainmenubar");	
	AddChild(menuBar);
	
	//file menu
	BMenu *fileMenu = new BMenu("File");
	
	BMenuItem *prefsItem = new BMenuItem("Preferences",new BMessage(InterfaceMessages::K_PREFERENCES_MSG));
	prefsItem->SetTarget(be_app);
	fileMenu->AddItem(prefsItem);
	fileMenu->AddSeparatorItem();
	
	BMenuItem *quitItem = new BMenuItem("Quit",new BMessage(B_QUIT_REQUESTED));
	quitItem->SetTarget(be_app);
	fileMenu->AddItem(quitItem);
	
	menuBar->AddItem(fileMenu);
	
	//help menu
	BMenu *helpMenu = new BMenu("Help");
	
	BMenuItem *aboutItem = new BMenuItem("About",new BMessage(B_ABOUT_REQUESTED));
	aboutItem->SetTarget(be_app);
	helpMenu->AddItem(aboutItem);
	
	menuBar->AddItem(helpMenu);
	//get password list
	BMessage pwdMessage(InterfaceMessages::K_GET_LOGIN_NAMES);
	
	//slows down program startup???	
	BMessenger msgr(m_protocolLooper);
	msgr.SendMessage(&pwdMessage,&m_passwordList);	
	
	//construct layered view to hold login screen and mainview
	BRect layeredViewFrame = Bounds();
	layeredViewFrame.top = (menuBar->Bounds()).bottom + 1;
	m_layeredView = new LayeredView(layeredViewFrame,"layeredview",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	AddChild(m_layeredView);
		
	m_layeredView->addLayer(constructLoginView(m_layeredView->Bounds()));
	m_layeredView->addLayer(constructContactView(m_layeredView->Bounds()));			
	//m_layeredView->setVisible(MainWindow::K_CONTACT_VIEW_LAYER);	
}

MainWindow::~MainWindow()
{
}

BView *MainWindow::constructLoginView(BRect frame)
{
	//construct gradientView	
	GradientView *loginView = new GradientView(frame, "gradient", B_FOLLOW_ALL_SIDES, AppConstants::K_TOP_COLOUR, AppConstants::K_BOTTOM_COLOUR);
	//loginView->SetViewColor(B_TRANSPARENT_COLOR);
	//sign in statusView
	Status *offlineStatus = FindStatus(Statusses::K_OFFLINE);
	m_statusBitmap = offlineStatus->GetStatusIcon(63);
	BRect bitmapBounds = m_statusBitmap->Bounds();
	float statusBitmapWidth = bitmapBounds.Width();
	float statusBitmapHeight = bitmapBounds.Height();
	float xStatus = (frame.Width() - statusBitmapWidth) / 2;
	float yStatus = 50.0f;
	BRect statusBitmapFrame(xStatus,yStatus,xStatus + statusBitmapWidth, yStatus + statusBitmapHeight);
	
	m_signinStatusView = new AnimatedBitmapView(statusBitmapFrame,"signinStatusView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE|B_FRAME_EVENTS,m_statusBitmap);
	loginView->AddChild(m_signinStatusView);		
	//construct loginView's contents
		
	//status bar for errors and login status
	const char* statusBarText = "Logged out...";
	float statusBarWidth = 100.0f;
	float xStatusBar = (frame.Width() - statusBarWidth) / 2;
	float yStatusBar = statusBitmapFrame.bottom + 5.0f;
	BRect statusBarFrame(xStatusBar,yStatusBar,xStatusBar + statusBarWidth,yStatusBar + 15.0f);
	
	m_statusBarView = new BStringView(statusBarFrame,"statusBarView",statusBarText);
	m_statusBarView->SetAlignment(B_ALIGN_CENTER);	
	//set status bar font to baskerville
	BFont baskerville;
	const font_family family = "Baskerville";
	const font_style style = "Bold";
	baskerville.SetFamilyAndStyle(family,style);
	baskerville.SetSize(14.0f);
	
	m_statusBarView->SetFont(&baskerville);
	loginView->AddChild(m_statusBarView);
	//passport combobox(for user name)
	const char* userName = "User name";
	float usernameStringWidth = loginView->StringWidth(userName);
		
	float usernameWidth = usernameStringWidth + 150.0f;
	float ulStart = (frame.Width() - usernameWidth - 20.0f) / 2.0f;
	float unStart = ulStart + usernameStringWidth + 6.0f;
	BRect unLabelFrame(ulStart,statusBarFrame.bottom + 20.0f,ulStart + usernameStringWidth,statusBarFrame.bottom + 35.0f);
	BRect usernameFrame(unStart,statusBarFrame.bottom + 20.0f,unStart + 140.0f,statusBarFrame.bottom + 37.0f);
		
	BStringView *usernameLabel = new BStringView(unLabelFrame,"usernameLabel",userName);	
	
	m_username = new ComboBox(usernameFrame,"username","",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW|B_NAVIGABLE);
	m_username->SetSelectionMessage(new BMessage(ComboConstants::K_COMBO_NEW_SELECTION_MSG));
	m_username->SetInvocationMessage(new BMessage(ComboConstants::K_COMBO_NEW_SELECTION_MSG));
	//add found usernames to combobox
	BString loginName;
	int32 i = 0;	
	
	while (m_passwordList.FindString("loginname",i,&loginName) == B_OK)
	{
		m_username->AddItem(new BStringItem(loginName.String()));
		i++;
	}
	
	loginView->AddChild(usernameLabel);
	loginView->AddChild(m_username);
		
	//password textcontrol
	const char* passwordName = "Password";
	float passwordStringWidth = loginView->StringWidth(passwordName);
	float controlWidth = passwordStringWidth + 150.0f;
	float xStart = (frame.Width() - controlWidth - 20.0f) / 2.0f;
	BRect passwordFrame(xStart,usernameFrame.bottom + 10.0f,xStart + controlWidth,usernameFrame.bottom + 25.0f);

	m_password = new BTextControl(passwordFrame, "password",passwordName,"",NULL,B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW|B_NAVIGABLE);
	m_password->SetDivider(passwordStringWidth + 5.0f);
	m_password->TextView()->HideTyping(true);	
	loginView->AddChild(m_password);
	//set signin status box
	const char* statusLabel = "Initial Status";
	float statusLabelWidth = loginView->StringWidth(statusLabel);
	float statusWidth = statusLabelWidth + 100.0f;	
	float stStart = (frame.Width() - statusWidth - 7.0f) / 2.0f;
				
	BRect statusFrame(stStart,passwordFrame.bottom + 10.0f,stStart + statusWidth,passwordFrame.bottom + 25.0f);
	
	StatusMenu *statusMenu = new StatusMenu();
	AddStatusItems(statusMenu);
	
	BMenuField *statusField = new BMenuField(statusFrame,"statusMenu",statusLabel,statusMenu);
	statusField->SetDivider(statusLabelWidth + 6.0f);
		
	statusMenu->SetStatus("Online");
	m_currentStatus = statusses[Statusses::K_AVAILABLE];
	statusMenu->SetTargetForItems(this);
	
	loginView->AddChild(statusField);
	//save password checkbox
	const char* passwordCheckLabel = "Save my password";
	float passCheckWidth = loginView->StringWidth(passwordCheckLabel) + 20.0f;
	float xPassCheck = passwordFrame.left + (passwordFrame.Width() - passCheckWidth) / 2.0f;
	BRect passwordCheckFrame(xPassCheck,statusFrame.bottom + 10.0f, xPassCheck + passCheckWidth,statusFrame.bottom + 25.0f);
		
	m_passwordSaveCheck = new BCheckBox(passwordCheckFrame,"passwordSaveCheck",passwordCheckLabel,new BMessage('ChCh'));
	loginView->AddChild(m_passwordSaveCheck);
	//login button
	const char* signinButtonLabel = "Sign in";
	float signinWidth = loginView->StringWidth(signinButtonLabel) + 30.0f;
	float xSignin = passwordFrame.left + (passwordFrame.Width() - signinWidth) / 2.0f;
	BRect signinFrame(xSignin,passwordCheckFrame.bottom + 10.0f,xSignin + signinWidth, passwordCheckFrame.bottom + 30.0f);

	m_signinButton = new BButton(signinFrame,"signinButton",signinButtonLabel,new BMessage(InterfaceMessages::K_USER_SIGN_IN_MSG));
	loginView->AddChild(m_signinButton);
	
	return loginView;
}

BView* MainWindow::constructContactView(BRect frame)
{
	BView *contactView = new BView(frame,"contactView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	//construct gradientView for top!
	BRect gradientFrame(0.0f,0.0f,frame.Width(),0.30f * frame.Height());	
	GradientView *loginView = new GradientView(gradientFrame, "gradient", B_FOLLOW_LEFT_RIGHT, AppConstants::K_TOP_COLOUR, AppConstants::K_BOTTOM_COLOUR);
	contactView->AddChild(loginView);
	
	BRect bitmapFrame(0.0f, 0.0f, 91.0f, 91.0f);
	BBitmap *bitmap = new BBitmap(bitmapFrame, B_RGB32);
	BRect userPictFrame(5.0f,10.0f,96.0f,101.0f);
	UserPictView *userPictView = new UserPictView(userPictFrame,bitmap,"userPictView",new BMessage(InterfaceMessages::K_USER_CHANGE_DISPLAYPIC_MSG),B_FOLLOW_NONE,0);
	loginView->AddChild(userPictView);
		
	BRect userNameFrame(userPictFrame.right + 10.0f,10.0f,gradientFrame.right - 10.0f,25.0f);
	m_userNameView = new EditableStringView(userNameFrame,"userNameView","<Insert Name Here>", new BMessage(InterfaceMessages::K_USER_CHANGE_NAME_MSG));	
	loginView->AddChild(m_userNameView);
	BFont usernameFont(be_bold_font);
	usernameFont.SetSize(12.0f);
	m_userNameView->SetFont(&usernameFont);
	m_userNameView->SetFont(be_bold_font, B_FONT_ALL);
	
	m_statusMenu = new StatusMenu();
	AddStatusItems(m_statusMenu);
	
	BRect statusFrame(userNameFrame.left, userNameFrame.bottom + 5.0f, userNameFrame.left + 100.0f, userNameFrame.bottom + 25.0f);
	BMenuField *statusField = new BMenuField(statusFrame,"statusField","",m_statusMenu);
	statusField->SetDivider(0.0f);
	loginView->AddChild(statusField);
	m_statusMenu->SetTargetForItems(this);
			
	BRect personalMessageFrame(statusFrame.left, statusFrame.bottom + 10.0f, gradientFrame.right - 10.0f, statusFrame.bottom + 25.0f);
	m_personalMessageView = new EditableStringView(personalMessageFrame,"personalMessageView","<Insert Personal Message Here>", new BMessage(InterfaceMessages::K_USER_SET_PERSONAL_MSG));	
	loginView->AddChild(m_personalMessageView);
	BFont personalFont;
	personalFont.SetFace(B_ITALIC_FACE);
	personalFont.SetSize(10.0f);
	m_personalMessageView->SetFont(&personalFont);
	
	BRect mailFrame(statusFrame.left,personalMessageFrame.bottom + 10.0f,gradientFrame.right - 10.0f, personalMessageFrame.bottom + 25.0f);
	MailView *mailView = new MailView(mailFrame);
	loginView->AddChild(mailView);
	
	BRect contactListFrame(0.0f,gradientFrame.bottom + 1.0f, frame.Width() - B_V_SCROLL_BAR_WIDTH, frame.Height());
	m_contactListView = new ContactListView(contactListFrame,"contactListView",B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	m_contactListView->SetInvocationMessage(new BMessage(InterfaceMessages::K_CONTACT_LIST_INVOKE_MSG));
	
	BScrollView *contactScrollView = new BScrollView("contactScroll",m_contactListView, B_FOLLOW_ALL_SIDES, 0,false,true, B_NO_BORDER);
	contactView->AddChild(contactScrollView);
		
	return contactView;
}

void MainWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case InterfaceMessages::K_USER_NAME_CHANGED_MSG:
		{
			BString newName;
			if (message->FindString(K_NEW_NAME_STRING, &newName) == B_OK)
			{
				m_userNameView->SetText(newName.String());
			}
		}
		break;
		case InterfaceMessages::K_USER_SET_PERSONAL_MSG:
		{
			message->AddString(K_PERSONAL_MESSAGE, m_personalMessageView->Text());
			//send message to protocol
			BMessenger protocolMsgr(NULL,m_protocolLooper);
			protocolMsgr.SendMessage(message);
		}
		break;
		//TODO: refactor this code later with a databinding between contact list in protocol and interface!
		case InterfaceMessages::K_ADD_GROUP_MSG:
		{
			BString groupName, groupID;
			if (	(message->FindString(K_GROUP_NAME, &groupName) == B_OK) &&
					(message->FindString(K_GROUP_ID, &groupID) == B_OK)
				)
			{	
				EditableListItem *groupItem = new EditableListItem(groupName.String(), new BMessage(InterfaceMessages::K_USER_RENAME_GROUP_MSG));
				m_contactListView->AddItem(groupItem);
				
				groups[groupID] = groupItem; //TODO:possible memory leak when name already exists, check this later!
			}
		}
		break;
		case InterfaceMessages::K_ADD_CONTACT_MSG:
		{
//TODO: new added contact do not have a group
//TODO: check if all contacts are added correctly, there seems to be something wrong in the protocol handler.
			Contact *contact;
			if (message->FindPointer(K_CONTACT,(void **)&contact) == B_OK)
			{				
				vector<Group*> contactGroups = contact->Groups();
				//iterate to all groups this contact has been added to
				typedef vector<Group*>::const_iterator GI;
				for (GI p = contactGroups.begin(); p != contactGroups.end(); ++p)
				{
					Group *group = *p;
					EditableListItem *groupItem = groups[group->GroupID()];
					//if the groupItem exists add a copy of the contact
					if (groupItem)
					{
						m_contactListView->AddUnder(new ContactItem(contact, new BMessage(InterfaceMessages::K_USER_RENAME_PRINCIPAL), false), groupItem);
						m_contactListView->SortContacts();
					}

				}
			}
		}
		break;
		case InterfaceMessages::K_CONTACT_LIST_INVOKE_MSG:
		{
			int32 selectedIndex = m_contactListView->CurrentSelection();
			//check if an item has been selected
			if (selectedIndex >= 0)
			{
				BListItem* selectedItem = m_contactListView->FullListItemAt(selectedIndex);
				//check if this item is a contact item
				if (is_instance_of(selectedItem, ContactItem))
				{
					//user wants to open a chat to a contact
					ContactItem* contactItem = dynamic_cast<ContactItem*>(selectedItem);
					Contact* chatContact = contactItem->GetContact();
					//a user can only open a chat with online contacts
					if (chatContact->GetStatus()->IsOnline())
					{
						//request procolMsgr to open chat to this contact
						BMessage *message = new BMessage(InterfaceMessages::K_USER_START_CONV_MSG);
						message->AddPointer(K_CONTACT, chatContact);
						BMessenger protocolMsgr(NULL,m_protocolLooper);
						protocolMsgr.SendMessage(message);
					}
					else
					{
						//contact offline, warn user
						BAlert *offlineAlert = new BAlert("Contact Offline", "Cannot open a chat to an offline contact!","Ok");
						offlineAlert->Go();
					}
				}
			}
		};
		break;
		case InterfaceMessages::K_CONTACT_CHANGE_STATUS_MSG:
		{	
				m_contactListView->Invalidate();				
				m_contactListView->SortContacts();
		}
		break;
		case InterfaceMessages::K_USER_SIGN_IN_MSG:
		{
			//get principal name(hotmail address)
			message->AddString("loginname",m_username->Text());
			//get principal password and add it to message
			message->AddString("password",m_password->Text());
			//get save option
			bool save = false;
			if (m_passwordSaveCheck->Value() == B_CONTROL_ON)
				save = true;
			message->AddBool("savepwd",save);
			//get initial status			
			message->AddString(K_NEW_STATUS,m_currentStatus->GetAbbreviation()); //TODO: find out when to send the initial status msg in the protocol
			//send message to protocol
			BMessenger protocolMsgr(NULL,m_protocolLooper);
			protocolMsgr.SendMessage(message);			
		}
		break;
		case InterfaceMessages::K_USER_SIGNED_IN_MSG:
		{
			//user has signed in
			m_signedIn = true;
			//show contact view, when logged in
			m_layeredView->setVisible(MainWindow::K_CONTACT_VIEW_LAYER);
			//Set username
			BString userName;
			if (message->FindString(K_NEW_NAME_STRING, &userName) == B_OK)
			{
				m_userNameView->SetText(userName.String());
			}
			//Set personal message, has to move...
			BString personalMessage;
			if (message->FindString(K_PERSONAL_MESSAGE, &personalMessage) == B_OK)
			{
				m_personalMessageView->SetText(personalMessage.String());
			}
			//Set initial user status
			BString statusString;
			if (message->FindString(K_INITIAL_STATUS, &statusString) == B_OK)
			{
				Status *status = statusses[statusString];
				if (status)
				{
					BString stat = status->GetStatusName();
					m_statusMenu->SetStatus(stat.String());
				}
			}
			//Set user display picture(if available)
		}
		break;		
		case InterfaceMessages::K_USER_SIGNED_OUT_MSG:
		{
			//user signed out again
			m_signedIn = false;
			//show login view, when logged out
			m_layeredView->setVisible(MainWindow::K_LOGIN_VIEW_LAYER);
		}
		break;
		case InterfaceMessages::K_USER_DISCONNECTED_MSG:
		{
			//user signed out again
			m_signedIn = false;
			//find reason for disconnection or failed login
			BString errorMsg;
			message->FindString(K_ERROR_STRING,&errorMsg);
			//find out if still viewing contact view, if so turn to login view
			if (m_layeredView->getVisible() == MainWindow::K_CONTACT_VIEW_LAYER)
				m_layeredView->setVisible(MainWindow::K_LOGIN_VIEW_LAYER);
			//display error in status text message in login view
			m_statusBarView->SetText(errorMsg.String());
		}
		break;				
		case ComboConstants::K_COMBO_NEW_SELECTION_MSG:
		{
			//new loginname selected, set password box to corresponding password(if found) 
			BString loginName(m_username->Text());			
			//loop through loginnames and find corresponding password
			BString foundName;
			bool foundPwd = false;
			
			int32 i = 0;				
			while (m_passwordList.FindString("loginname",i,&foundName) == B_OK)
			{
				if (loginName == foundName)
				{
					BString password;
					if (m_passwordList.FindString("password",i,&password) == B_OK)
					{
						m_password->SetText(password.String());
						foundPwd = true;
					}
				}
				i++;
			}
			//reset password box if no corresponding password is found!
			if(!foundPwd)
				m_password->SetText("");
		}
		break;
		case InterfaceMessages::K_USER_CHANGE_STATUS_MSG:
		{		
			message->FindPointer(K_NEW_STATUS, (void**)&m_currentStatus);
			if (m_signedIn)
			{
				//request the status change in the protocol
				BMessage *message = new BMessage(InterfaceMessages::K_USER_CHANGE_STATUS_MSG);
				message->AddString(K_NEW_STATUS, m_currentStatus->GetAbbreviation());
				BMessenger protocolMsgr(NULL,m_protocolLooper);
				protocolMsgr.SendMessage(message);		
			}
		}
		break;		
		default:
			BWindow::MessageReceived(message);
		break;
	}	
}

void MainWindow::AddStatusItems(StatusMenu *statusMenu)
{
	for (SI p = statusses.begin(); p != statusses.end(); ++p)
	{
		Status *status = (*p).second;
		if (status->IsUserChoice())
		{
			BBitmap *statusIcon = status->GetStatusIcon();
			BMessage *statusMessage = new BMessage(InterfaceMessages::K_USER_CHANGE_STATUS_MSG);
			statusMessage->AddPointer(K_NEW_STATUS,status);
			
			BString statusName = status->GetStatusName();
			StatusItem* statusItem = new StatusItem(statusIcon,statusName.String(),statusMessage);
			statusMenu->AddItem(statusItem);			
		}
	}	
}

bool MainWindow::QuitRequested()
{	
	//Preferences* prefs = PrefsMethods::GetPreferences(AppConstants::K_MAIN_APP_PREFS.String());
	//save current mainwindow position and size to preferences file
	//prefs->ReplaceRect(K_MAIN_WINDOW_RECT, Frame());
	//PrefsMethods::ChangePreferences(prefs);
	//stop the protocol looper
	BMessenger msgr(m_protocolLooper);
	msgr.SendMessage(B_QUIT_REQUESTED);
	//quit the application
	be_app->PostMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();	
}
