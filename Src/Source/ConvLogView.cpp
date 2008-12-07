/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef CONV_LOG_VIEW_H
#include "ConvLogView.h"
#endif

#include <be/app/Cursor.h>
#include <be/app/Messenger.h>
#include <be/interface/Region.h>
#include <be/interface/Window.h>
#include <be/translation/TranslationUtils.h>
#include "constants.h"
#include "MessageFields.h"
#include "MessageItem.h"
#include "ChatMessageItem.h"
#include "Contact.h"

ConvLogView::ConvLogView(BRect frame,  BRect textRect) 
				:	BListView(frame, "ConvLogView", B_MULTIPLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE)
{
}

ConvLogView::~ConvLogView()
{
}

void ConvLogView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{		
		case InterfaceMessages::K_ADD_CHAT_TEXT:
		{
			//get text message
			
			//get emoticons from message and and them to the emoticonlist
			
			//get url from message and add them to urlInfoList
		}
		break;
		default:
			BListView::MessageReceived(message);
		break;
	}
}

void ConvLogView::MouseMoved(BPoint point, uint32 transit, const BMessage *message)
{
	bool overURL = false;

	/*if(transit == B_INSIDE_VIEW) 
	{
		typedef vector<URLInfo*>::const_iterator CI;
		for(CI p = m_urlInfoList.begin(); p != m_urlInfoList.end(); ++p) 
		{
			if((*p)->Contains(this, point)) 
			{
				// Mouse is over a link. Display "URL hover cursor".
				// That cursor is identical to the BeOS hand cursor
				// but contains a little arrow (like in Net+).
				#if B_BEOS_VERSION >= B_BEOS_VERSION_5
					const BCursor URLHoverCursor(K_CURSOR_HYPERLINK);				
					SetViewCursor(&URLHoverCursor);
				#else
					be_app->SetCursor(K_CURSOR_HYPERLINK);
				#endif
				
				overURL = true;
				
				break;
			}
		}
		
		#if B_BEOS_VERSION >= B_BEOS_VERSION_5
		if(!overURL) 
		{
			SetViewCursor(B_CURSOR_I_BEAM);
		}
		#endif
	}*/
	
	if(!overURL)
		BListView::MouseMoved(point, transit, message);
}

void ConvLogView::MouseDown(BPoint point)
{
	//check if an url has been clicked!
/*	typedef vector<URLInfo*>::const_iterator CI;
	for(CI p = m_urlInfoList.begin(); p != m_urlInfoList.end(); ++p) 
	{
		URLInfo *urlInfo = (*p);
		if(urlInfo->Contains(this, point)) 
		{
			//User clicked on an anchor. Send message to open webbrowser 
			//or eMail client.
			BMessage *urlOpenMessage = new BMessage(InterfaceMessages::K_OPEN_URL_MSG);
			urlOpenMessage->AddString(K_URL_STRING, urlInfo->GetURL());
			BMessenger owner(Window());
			owner.SendMessage(urlOpenMessage);
			return;
		}
	}	*/
	BListView::MouseDown(point);
}

void ConvLogView::AddMessage(ConvMessage *message)
{
	bool followUp = false;
	//add a new message
	if (CountItems() > 0)
	{
		MessageItem *lastMessageItem  = dynamic_cast<MessageItem*>(LastItem());
		Contact *lastContact = lastMessageItem->Message()->Sender();
		Contact *messageContact = message->Sender();
	
		if (lastContact->Passport() == messageContact->Passport())
		{
			followUp = true;
		}		
	}
	
	if (followUp)
	{
			//the contact of the new message is the same as the contact of the last message
			//therefore, treat the message as a follow up
			ChatMessageItem *chatItem = new ChatMessageItem(message, true);
			AddItem(chatItem);
	}
	else
	{
		ChatMessageItem *chatItem = new ChatMessageItem(message, false);			
		AddItem(chatItem);
	}
	//scroll to last item in list
	int32 lastItemIndex = CountItems() - 1; 
	Select(lastItemIndex);
	ScrollToSelection();
}

/*//=========================ConvLogView::URLInfo===============================
ConvLogView::URLInfo::URLInfo(int32 start, int32 end, BString href) 
						:	m_start(start), 
							m_end(end),
							m_href(href)
{
}

ConvLogView::URLInfo::~URLInfo()
{	
}


bool ConvLogView::URLInfo::Contains(ConvLogView *view, BPoint point)
{
	// Returns true, if the point is over this URL.
	BRegion region;
	view->GetTextRegion(m_start, m_end, &region);
	
	return region.Contains(point);
}

BString	ConvLogView::URLInfo::GetURL()
{
	return m_href;
}

//==================ConvLogView::CTextRun ===================================
ConvLogView::TextRun::TextRun(const text_run &other)
{ 
	(text_run &)(*this) = other; 
}

ConvLogView::TextRun::TextRun(int32 _offset, BFont _font, rgb_color _color) 
{
	offset	= _offset;
	font	= _font;
	color	= _color;
}*/
