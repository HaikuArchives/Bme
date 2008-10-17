#ifndef CHAT_WINDOW_HANDLER_H
#include "ChatWindowHandler.h"
#endif

#include "constants.h"
#include "MessageFields.h"
#include "ChatWindow.h"
#include "Common.h"
#include "SBLooper.h"
#include "ChatWindow.h"

ChatWindowHandler::ChatWindowHandler()
						:	BHandler()
{
	AddFilter(new ChatWindowFilter());
	
	BRect chatFrame(100.0f,100.0f, 600.0f, 600.0f);
	ChatWindow* chatWindow = new ChatWindow(chatFrame);
	chatWindow->Show();
}

ChatWindowHandler::~ChatWindowHandler()
{
}

void ChatWindowHandler::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case InterfaceMessages::K_CONNECT_SB_MSG:
		{		
			BString sbUrl, authString;
			int32 sbPort;
			if	(
					(message->FindString(K_SB_ADDRESS,&sbUrl) == B_OK) &&
					(message->FindInt32(K_SB_PORT,&sbPort) == B_OK) &&
					(message->FindString(K_AUTHENTICATION_STRING,&authString) == B_OK)
				)	
			{			
				BString sessionId, contactPassport, friendlyName;
				if (
						(message->FindString(K_SB_SESSION_ID, &sessionId) == B_OK) &&
						(message->FindString(K_CONTACT_PASSPORT,&contactPassport) == B_OK) &&
						(message->FindString(K_CONTACT_FRIENDLY_NAME, &friendlyName) == B_OK)
					) 							
				{
					//look up contact here
					Contact* contact = Common::FindContact(contactPassport); 
					if (contact)
					{
						//start looper
						SBLooper *sbLooper = new SBLooper(this, m_passport, sbUrl,sbPort, sessionId, authString);							
						sbLooper->Run();						 						
					}
				}
				else
				{
					Contact* contact;
					if (message->FindPointer(K_CONTACT,(void **)&contact) == B_OK)
					{						
						//start looper
						SBLooper *sbLooper = new SBLooper(this, m_passport, sbUrl,sbPort,authString);							
						sbLooper->Run();
					}
				}
			}					
		}
		break;
		case InterfaceMessages::K_USER_START_CONV_MSG:
		case InterfaceMessages::K_CONTACT_START_CONV_MSG:
		{		
			//conversation has been started, open the chat window
			ChatWindow *chatWindow = new ChatWindow(CalculateChatWindowFrame());
			chatWindow->SetOwner(BMessenger(this));
			//get the contacts this person is chatting with
			Contact* contact;
			if (message->FindPointer(K_CONTACT,(void **)&contact) == B_OK)
			{				
				chatWindow->AddContact(contact);				
			}
			chatWindow->Show();
		}
		break;					
		default:
			BHandler::MessageReceived(message);
		break;
	}
}

BRect ChatWindowHandler::CalculateChatWindowFrame()
{
	BRect chatFrame(100.0f,100.0f,400.0f,500.0f);
	return chatFrame;
}

//=============================ChatWindowFilter=================================
ChatWindowFilter::ChatWindowFilter()
					:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE)
{
}

ChatWindowFilter::~ChatWindowFilter()
{
}

filter_result ChatWindowFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result;
	switch (message->what)
	{
		//send these messages through to the NSLooper
		case InterfaceMessages::K_USER_START_CONV_MSG:
		case InterfaceMessages::K_USER_QUIT_CONV_MSG:
		case InterfaceMessages::K_CONTACT_START_CONV_MSG:
		case InterfaceMessages::K_CONTACT_QUIT_CONV_MSG:
		case InterfaceMessages::K_CONTACT_SEND_TEXT_MSG:
		case InterfaceMessages::K_CONTACT_TYPING_MSG:	
		case InterfaceMessages::K_CONNECT_SB_MSG:			
		{			
			result = B_DISPATCH_MESSAGE;
		}
		break;
		default:
			result = B_SKIP_MESSAGE;
		break;
	}
	
	return result;
}
