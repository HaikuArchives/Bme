#ifndef PROTOCOL_MESSAGE_H
#include "ProtocolMessage.h"
#endif

#include "MessageFields.h"

ProtocolMessage::ProtocolMessage(const BString& type)
					:	BArchivable(),
						m_msgType(type),
						m_trId(-1),
						m_payloadSize(0)
{
}

ProtocolMessage::ProtocolMessage(BMessage *archive)
					:	BArchivable(archive)
{
	archive->FindString(K_COMMAND,&m_msgType);
	archive->FindInt32(K_TR_ID, &m_trId);//TODO: no trId, what then?
	//TODO: K_REMAINING_MSG
	archive->FindBool("ProtocolMessage::hasPayload",&m_hasPayload);//TODO: needed?
	archive->FindInt32(K_PAYLOAD_SIZE, &m_payloadSize); 
	archive->FindString(K_PAYLOAD_DATA,&m_payload);
}

ProtocolMessage::~ProtocolMessage()
{
}

BArchivable* ProtocolMessage::Instantiate(BMessage *archive)
{
	if (validate_instantiation(archive, "ProtocolMessage"))
      return new ProtocolMessage(archive);
    return NULL;
}

status_t ProtocolMessage::Archive(BMessage *archive, bool deep) const
{
	archive->AddString("class","ProtocolMessage");
	archive->AddString(K_COMMAND, m_msgType);
	archive->AddInt32(K_TR_ID, m_trId); //TODO: no trId, what then?
	//TODO: K_REMAINING_MSG
	archive->AddBool("ProtocolMessage::hasPayload",m_hasPayload); //TODO: needed?
	archive->AddInt32(K_PAYLOAD_SIZE, m_payloadSize);
	archive->AddString(K_PAYLOAD_DATA,m_payload);
		
	return B_OK;
}


void ProtocolMessage::SetType(const BString& type)
{
	m_msgType = type;
}

BString ProtocolMessage::Type()
{
	return m_msgType;
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

void ProtocolMessage::RemovePayload()
{
	m_hasPayload = false;
	m_payload = "";
	m_payloadSize = 0;
}

BString ProtocolMessage::GetPayload()
{
	return m_payload;
}
