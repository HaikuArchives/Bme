#ifndef CHAT_MESSAGE_ITEM_H
#define CHAT_MESSAGE_ITEM_H

#include "MessageItem.h"

class ChatMessageItem : public MessageItem
{
	public:
		ChatMessageItem(ConvMessage* message, bool followUp = false);
		virtual				~ChatMessageItem();
		
		virtual void 		DrawItem(BView* owner, BRect itemRect, bool drawEverything = false);
		virtual void 		Update(BView* owner, const BFont* font);
		
		bool				IsFollowUp();
		void				SetFollowUp(bool followUp);
	
	private:
		bool				m_followUp;
};

#endif
