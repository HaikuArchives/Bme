#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include <app/Message.h>
#include <support/String.h>
#include "ProtocolMessage.h"

class ChatMessage : public ProtocolMessage
{
	public:
		ChatMessage();
		ChatMessage(BMessage *archive);
		virtual				~ChatMessage();
			
		void				SetMessageText(const BString& messageText);
		BString				GetMessageText();
	
		BString				String();
		
	protected:
		void				ParsePayload(BString payloadMsg);
	
	private:
		BString				m_messageText;
};

#endif
