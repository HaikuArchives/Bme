#ifndef CHAT_HANDLER_H
#include "ChatHandler.h"
#endif

#include "constants.h"
#include "MessageFields.h"
#include "ProtocolConstants.h"

ChatHandler::ChatHandler(BMessage *message)
				:	ServerHandler()
{	
	AddFilter(new ChatFilter());
	//send initial SB message	
	SendCommandMessageTrID(message);
}

ChatHandler::~ChatHandler()
{
}

void ChatHandler::SwitchBoardIdle()
{
}

void ChatHandler::CloseConversation()
{
	BMessage *message = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
	message->AddString(K_COMMAND, SwitchboardMessages::K_LEAVE_SB_SESSION);
	SendCommandMessage(message);
}

void ChatHandler::ContactInvited(Contact *contact)						
{
}

void ChatHandler::ContactJoined(Contact* contact)
{
}

void ChatHandler::LeftConversation(Contact *contact)
{
}

void ChatHandler::SendMessage(ChatMessage chatMessage)
{
	BMessage *message = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
	message->AddString(K_COMMAND, SwitchboardMessages::K_SWITCHBOARD_MESSAGE);
	message->AddString(K_REMAINING_MSG, AckTypes::K_NEGATIVE_ACK_TYPE);
	
	BString payloadData = chatMessage.String();				
	message->AddInt32(K_PAYLOAD_SIZE, payloadData.Length());
	message->AddString(K_PAYLOAD_DATA, payloadData);
	//TODO: build in acknowledgement
	SendCommandMessageTrID(message);
}

void ChatHandler::ReceivedMessage(Contact* contact, ChatMessage message)
{
}

void ChatHandler::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case InterfaceMessages::K_USER_SEND_TEXT_MSG:
		{
			//user send a text message to peers
			BString messageText;
			if (message->FindString(K_MESSAGE_TEXT, &messageText) == B_OK)
			{
				ChatMessage chatMsg;
				chatMsg.SetMessageText(messageText);
				SendMessage(chatMsg);
			}		
		}
		break;
		case InterfaceMessages::K_USER_QUIT_CONV_MSG: 
		{
			BMessage *message = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
			message->AddString(K_COMMAND, SwitchboardMessages::K_LEAVE_SB_SESSION);
			SendCommandMessage(message);
		}
		break;
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			if (message->FindString(K_COMMAND, &command) == B_OK)
			{
				if (command == SwitchboardMessages::K_SB_USR_COMMAND)
				{
					BMessage *message = new BMessage(InterfaceMessages::K_USER_START_CONV_MSG);
					SendCommandMessage(message);
				}
				else if (command == SwitchboardMessages::K_OPEN_SB_SESSION)
				{
					BMessage *message = new BMessage(InterfaceMessages::K_CONTACT_START_CONV_MSG);
					SendCommandMessage(message);
				}
				else if (command == SwitchboardMessages::K_PRINCIPALS_IN_SESSION)
				{
				}
			}
		}
		break;
		/*case SwitchboardMessages::K_INVITE_PRINCIPAL:
		{
			Contact *contact;
			ContactInvited(contact);
		}
		break;
		case SwitchboardMessages::K_PRINCIPAL_JOINED:
		{
			Contact *contact;
			ContactJoined(Contact* contact);
		}
		break;
		case SwitchboardMessages::K_PRINCIPAL_LEFT:
		{
			Contact *contact;
			LeftConversation(Contact *contact);
		}
		break;
		case SwitchboardMessages::K_SWITCHBOARD_MESSAGE:
		{
			Contact* contact;
			ChatMessage message; 
			ReceivedMessage(contact, message);
		}
		break;*/
		default:
			BHandler::MessageReceived(message);
		break;
	}
}

//=========================ChatFilter========================================
ChatFilter::ChatFilter()
				:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE)
{
}

ChatFilter::~ChatFilter()
{
}

filter_result ChatFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result;
	switch (message->what)
	{
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);
			if (command == SwitchboardMessages::K_SB_USR_COMMAND ||
				command == SwitchboardMessages::K_OPEN_SB_SESSION ||
				command == SwitchboardMessages::K_PRINCIPALS_IN_SESSION
				)
			{
				result = B_DISPATCH_MESSAGE;
			}
		}
		break;
		//AuthenticationHandler only handles the following messages:		
		case InterfaceMessages::K_USER_SEND_TEXT_MSG:	
		case InterfaceMessages::K_USER_QUIT_CONV_MSG:	
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
