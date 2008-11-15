#ifndef PROTOCOL_MESSAGE_H
#include "ProtocolMessage.h"
#endif

#include "MessageFields.h"

//TODO: inherit from BMessage
//TODO: add convenience methods
//TODO: change ServerHandler to use these messages
//TODO: change protocol code to use these messages
ProtocolMessage::ProtocolMessage(const BString& type)
					:	m_commandType(type),												
						m_hasTrId(false),
						m_hasPayload(false),
						m_trId(0),
						m_payloadSize(0)
{
}

ProtocolMessage::~ProtocolMessage()
{
}

void ProtocolMessage::SetCommandType(const BString& type)
{
	m_commandType = type;
}

BString ProtocolMessage::CommandType()
{
	return m_commandType;
}

bool ProtocolMessage::HasTrId()
{
	return m_hasTrId;
}

void ProtocolMessage::SetTrId(uint32 trId)
{
	m_trId = trId;
	m_hasTrId = true;
}

uint32 ProtocolMessage::TrId()
{
	return m_trId;
}	

bool ProtocolMessage::HasPayload()
{
	return m_hasPayload;
}

void ProtocolMessage::SetPayload(const BString& payload)
{
	m_hasPayload = true;
	m_payload = payload;
	m_payloadSize = m_payload.Length();
} 

BString ProtocolMessage::Payload()
{
	return m_payload;
}

void ProtocolMessage::RemovePayload()
{
	m_hasPayload = false;
	m_payload = "";
	m_payloadSize = 0;
}
