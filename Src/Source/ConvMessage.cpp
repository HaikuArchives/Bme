#ifndef CONV_MESSAGE_H
#include "ConvMessage.h"
#endif

#include <support/String.h>
#include "Contact.h"

ConvMessage::ConvMessage(Contact* sender)
				:	m_sender(sender)
{
}

ConvMessage::~ConvMessage()
{
}

Contact* ConvMessage::Sender()
{
	return m_sender;
}

void ConvMessage::SetSender(Contact* contact)
{
	m_sender = contact;
}

bigtime_t ConvMessage::Timestamp()
{
	return m_timestamp;
}

void ConvMessage::SetTimeStamp(bigtime_t timestamp)
{
	m_timestamp = timestamp;
}
