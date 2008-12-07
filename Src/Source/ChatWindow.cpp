/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef CHAT_WINDOW_H
#include "ChatWindow.h"
#endif

#include <app/Message.h>
#include <app/Application.h>
#include <interface/MenuBar.h>
#include <interface/Menu.h>
#include <interface/MenuItem.h>
#include <interface/Button.h>
#include <interface/InterfaceDefs.h>
#include "constants.h"
#include "MessageFields.h"
#include "ContactDetailsItem.h"
#include "Statusses.h"
#include "ToolbarButton.h"
#include "ToolbarSeparator.h"
#include "InAppIcons.h"
#include "Common.h"

ChatWindow::ChatWindow(BRect frame)
				:	BWindow(frame, "Chatting with", B_TITLED_WINDOW, 0),
					m_firstMessage(true)
{
	//construct menu
	BMenuBar *chatMenuBar = new BMenuBar(BRect(0,0,0,0),"chatMenuBar");	
	AddChild(chatMenuBar);	
	//file menu
	BMenu *fileMenu = new BMenu("File");
	
	BMenuItem *saveItem = new BMenuItem("Save Conversation",new BMessage(B_SAVE_REQUESTED));
	fileMenu->AddItem(saveItem);
	
	BMessage *closeMsg = new BMessage(InterfaceMessages::K_CLOSE_CHAT_WINDOW);	
	BMenuItem *closeItem = new BMenuItem("Close", closeMsg);
	fileMenu->AddItem(closeItem);
	
	chatMenuBar->AddItem(fileMenu);
	//edit menu
	
	//background view
	float chatMenuBarHeight = (chatMenuBar->Bounds()).Height();
	BRect backgroundFrame(0.0f,chatMenuBarHeight,frame.Width(), frame.Height());
	BView *backgroundView = new BView(backgroundFrame,"backgroundView",B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	backgroundView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backgroundView);
	//Construct list of contact details
	BRect contactDetailsFrame(0.0f, 1.0f, backgroundFrame.Width(), 42.0f);
	m_contactDetailsView = new BListView(contactDetailsFrame, "contactDetailsView", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS| B_FULL_UPDATE_ON_RESIZE);
	m_contactDetailsView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	m_contactScroll = new BScrollView("contactScroll", m_contactDetailsView, B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE, false, false, B_NO_BORDER);	
	backgroundView->AddChild(m_contactScroll);	
	
	//Construct ConvLogView, to show entire conversation	
	BRect convLogFrame(5.0f, contactDetailsFrame.bottom + 10.0f,backgroundFrame.Width() - 121.0f - B_V_SCROLL_BAR_WIDTH, backgroundFrame.Height() - 100.0f);
	BRect convLogTextFrame(0.0f,0.0f,convLogFrame.Width(), convLogFrame.Height());
	m_convLogView = new ConvLogView(convLogFrame, convLogTextFrame);
	m_convScroll = new BScrollView("convScroll", m_convLogView, B_FOLLOW_ALL_SIDES, B_FULL_UPDATE_ON_RESIZE, false, true);
	backgroundView->AddChild(m_convScroll);	
	//construct contact picture view, to display a contact avatar
	BRect contactPictureFrame(convLogFrame.right + 10.0f + B_V_SCROLL_BAR_WIDTH, convLogFrame.top, convLogFrame.right + 102.0f + B_V_SCROLL_BAR_WIDTH, convLogFrame.top + 92.0f);
	m_contactPictureView = new ContactPictureView(contactPictureFrame);
	backgroundView->AddChild(m_contactPictureView);
	//construct toolbar with font settings, etc.	
	BRect toolbarFrame(0.0f, convLogFrame.bottom + 5.0f, backgroundFrame.Width(),convLogFrame.bottom + 35.0f);
	m_toolbarView = new WToolbar(toolbarFrame, "toolbarView",W_TOOLBAR_STYLE_FLAT,W_TOOLBAR_HORIZONTAL,false,B_FANCY_BORDER,B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	m_toolbarView->SetBorder(B_NO_BORDER);
	backgroundView->AddChild(m_toolbarView);
	//add buttons
	BBitmap *fontBitmap = Common::ConvertBitmap(K_FONT_BITS, K_IS_TYPING_WIDTH, K_IS_TYPING_HEIGHT);
	BMessage *fontMessage = new BMessage(InterfaceMessages::K_PREFERENCES_MSG);
	fontMessage->AddString(K_ACTIVE_TAB, "Fonts");
	WToolbarButton* fontButton = new WToolbarButton("fontButton","",fontBitmap,fontMessage);
	fontButton->SetTarget(be_app);	
	m_toolbarView->AddItem(fontButton,0,0);
	
	BBitmap *emoticonBitmap = Common::ConvertBitmap(K_EMOTICON_BITS, K_IS_TYPING_WIDTH, K_IS_TYPING_HEIGHT);
	BMessage *emoticonMessage = new BMessage(InterfaceMessages::K_PREFERENCES_MSG);
	emoticonMessage->AddString(K_ACTIVE_TAB, "Emoticons");
	
	WToolbarButton *emoticonButton =  new WToolbarButton("emoticonButton","",emoticonBitmap,emoticonMessage);	
	emoticonButton->SetTarget(be_app);
	m_toolbarView->AddItem(emoticonButton,0,1);
		
	BBitmap *backgroundBitmap = Common::ConvertBitmap(K_BACKGROUND_IMAGE_BITS, K_IS_TYPING_WIDTH, K_IS_TYPING_HEIGHT);
	BMessage *backgroundMessage = new BMessage(InterfaceMessages::K_PREFERENCES_MSG);
	backgroundMessage->AddString(K_ACTIVE_TAB, "Backgrounds");
	
	WToolbarButton *backgroundButton = new WToolbarButton("backgroundButton","",backgroundBitmap,backgroundMessage);
	backgroundButton->SetTarget(be_app);
	m_toolbarView->AddItem(backgroundButton,0,2);
	
	m_toolbarView->AddItem(new WToolbarSeparator("firstSeparator"),0,3);
	
	BBitmap *fileTransferBitmap = Common::ConvertBitmap(K_FILE_TRANSFER_BITS,K_IS_TYPING_WIDTH, K_IS_TYPING_HEIGHT);
	m_toolbarView->AddItem(new WToolbarButton("fileTransferButton","",fileTransferBitmap),0,4);
	
	BBitmap *recordSoundBitmap = Common::ConvertBitmap(K_RECORD_SOUND_BITS,K_IS_TYPING_WIDTH, K_IS_TYPING_HEIGHT);
	m_toolbarView->AddItem(new WToolbarButton("recordSoundButton","",recordSoundBitmap),0,5);
	
	BBitmap *nudgeBitmap = Common::ConvertBitmap(K_NUDGE_BITS, K_IS_TYPING_WIDTH, K_IS_TYPING_HEIGHT);
	m_toolbarView->AddItem(new WToolbarButton("nudgeButton","",nudgeBitmap),0,6);
	
	//construct ConvTextView, to enter text
	BRect convTextViewFrame(convLogFrame.left, toolbarFrame.bottom + 5.0f, convLogFrame.right, toolbarFrame.bottom + 50.0f); 
	BRect convTextFrame(0.0f,0.0f,convTextViewFrame.Width(),convTextViewFrame.Height() * 10.0f);
	m_convTextView = new BTextView(convTextViewFrame,"convTextView",convTextFrame, B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	m_convTextScroll = new BScrollView("convTextScroll", m_convTextView, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, 0, false, true);		
	
	backgroundView->AddChild(m_convTextScroll);
	//construct send button
	BRect sendButtonFrame(convTextViewFrame.right + 20.0f, convTextViewFrame.top + 10.0f,convTextViewFrame.right + 60.0f, convTextViewFrame.top + 20.0f);
	BButton *sendButton = new BButton(sendButtonFrame, "sendButton", "Send", new BMessage(InterfaceMessages::K_USER_SEND_TEXT_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	backgroundView->AddChild(sendButton);
	sendButton->MakeDefault(true);		
}

ChatWindow::~ChatWindow()
{
}

void ChatWindow::SetOwner(BMessenger owner)
{
	m_owner = owner;
}

void ChatWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{		
		case B_SIMPLE_DATA:
		{
			//find out if contact has been dragged and dropped onto window
		}
		break;		
		case InterfaceMessages::K_CONTACT_QUIT_CONV_MSG:
		{
			BString passport;
			message->FindString(K_CONTACT_PASSPORT, &passport);
			
			RemoveContact(passport);
			m_contactDetailsView->Invalidate();
		}
		break;
		case InterfaceMessages::K_USER_SEND_TEXT_MSG:
		{
			message->AddString(K_MESSAGE_TEXT, m_convTextView->Text());
			m_owner.SendMessage(message);
			
			
			Contact* userContact = new Contact("obelmiks@hotmail.com","Tim", "",0,statusses["FLN"]);
			ConvMessage *userTextMessage = new ConvMessage(userContact);
			m_convLogView->AddMessage(userTextMessage);
			
			m_convTextView->SetText("");
		}
		break;
		case InterfaceMessages::K_CONTACT_SEND_TEXT_MSG:
		{
			BString messageText;
			if (message->FindString(K_MESSAGE_TEXT, &messageText) == B_OK)
			{
				//if the contact initiates the conversation, the window is
				//hidden until the first textmessage
				if(m_firstMessage && IsHidden())//TODO: check if IsHidden() works in this context!
					Show();
				//add message to message view
			}
			m_firstMessage = false;
		}
		break;
		case InterfaceMessages::K_CONTACT_TYPING_MSG:
		{
			BString passport;
			message->FindString(K_CONTACT_PASSPORT, &passport);
			bool isTyping;
			message->FindBool(K_IS_TYPING, &isTyping);
			//turn typing on/off in contact details view
			//ContactDetailsItem *ci = m_contactDetailsView->FindContact(passport);
			//ci->SetTyping(isTyping);
			m_contactDetailsView->Invalidate();
		}
		break;
		case InterfaceMessages::K_CLOSE_CHAT_WINDOW:
		{
			Quit();	
		}
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}

void ChatWindow::AddContact(Contact *contact)
{		
	//add the contact to the contact details listview	
	ContactDetailsItem *ci = new ContactDetailsItem(contact);
	m_contactDetailsView->AddItem(ci);	
	//check if it is the first contact to add to conversation
	int32 noItems = m_contactDetailsView->CountItems();
	if (noItems == 1)
	{
		BString windowTitle = "Chatting with ";
		windowTitle << contact->FriendlyName();
		SetTitle(windowTitle.String());
	}
	//resize m_contactDetailsView to fit the contact details items	
	float contactViewWidth = (m_contactDetailsView->Bounds()).Width();	
	if (noItems <= 3)
	{
		float newHeight = noItems * (ci->Height() + 1.0f);	
		m_contactScroll->ResizeTo(contactViewWidth, newHeight);		
	}
	else
	{
		//resize to 3 times item height(max height)
		m_contactScroll->ResizeTo(contactViewWidth, 3.0f * (ci->Height() + 1.0f));			
		//add scrollbar??				
	}
	
	float convX = (m_convScroll->Frame()).left;
	float convY = m_contactDetailsView->Frame().bottom + 10.0f;
	m_convScroll->MoveTo(convX, convY);
	
	float contactPicX = m_convScroll->Frame().right + 10.0f;
	m_contactPictureView->MoveTo(contactPicX, convY);

	float toolbarX = (m_toolbarView->Frame()).left;
	float toolbarY = (m_convScroll->Frame()).bottom + 5.0f;
	m_toolbarView->MoveTo(toolbarX,toolbarY);
	
	float convTextX = (m_convTextScroll->Frame()).left;
	float convTextY = (m_toolbarView->Frame()).bottom + 10.0f;	
	m_convTextScroll->MoveTo(convTextX,convTextY);		
}

void ChatWindow::RemoveContact(BString contactPassport)
{
	//turn typing on/off in contact details view
	//ContactDetailsItem *ci = m_contactDetailsView->FindContact(passport);
	//ci->SetTyping(isTyping);
}

bool ChatWindow::QuitRequested()
{
	m_owner.SendMessage(new BMessage(InterfaceMessages::K_USER_QUIT_CONV_MSG));
	return BWindow::QuitRequested();
}
