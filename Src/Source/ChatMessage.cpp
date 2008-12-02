#ifndef CHAT_MESSAGE_H
#include "ChatMessage.h"
#endif

#include "ProtocolConstants.h"

ChatMessage::ChatMessage()
				:	ProtocolMessage(SwitchboardMessages::K_SWITCHBOARD_MESSAGE)
{
}

ChatMessage::ChatMessage(BMessage *archive)
				:	ProtocolMessage(SwitchboardMessages::K_SWITCHBOARD_MESSAGE)//ProtocolMessage(archive)
{
	//ParsePayload(GetPayload());
}

ChatMessage::~ChatMessage()
{
}

void ChatMessage::SetMessageText(const BString& messageText)
{
	m_messageText = messageText;
	SetPayload(String());
}

BString ChatMessage::GetMessageText()
{
	return m_messageText;
}

BString	ChatMessage::String()
{
	//return a payload protocol representation for this object 	
	BString flattenedMsg;
	//Header
	flattenedMsg.Append("MIME-Version: 1.0\r\n");
	flattenedMsg.Append("Content-Type: text/plain; charset=UTF-8\r\n");
    flattenedMsg.Append("X-MMS-IM-Format: FN=Arial; EF=I; CO=0; CS=0; PF=22\r\n");
    flattenedMsg.Append("\r\n");
    //Body
    flattenedMsg.Append(m_messageText);
    
	return flattenedMsg;
}

void ChatMessage::ParsePayload(BString payloadMsg)
{
	//parse the payload msg into a ChatMessage object
	BString separator = "\r\n\r\n";
	int32 separatorIndex = payloadMsg.FindFirst(separator);
	if (separatorIndex != B_ERROR)
	{
		separatorIndex += separator.CountChars();
		payloadMsg.CopyInto(m_messageText,separatorIndex, separator.CountChars() - separatorIndex);
	}   
}

