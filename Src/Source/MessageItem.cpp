#ifndef MESSAGE_ITEM_H
#include "MessageItem.h"
#endif

#include "ConvMessage.h"

MessageItem::MessageItem(ConvMessage* message)
				:	m_message(message)
{
}

MessageItem::~MessageItem()
{
	delete m_message;
}

ConvMessage* MessageItem::Message()
{
	return m_message;
}
					
void MessageItem::SetMessage(ConvMessage* message)
{
	if (m_message)
	{
		//clean up the old message		
		delete m_message;
	}
	m_message = message;
}
