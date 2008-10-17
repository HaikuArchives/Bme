#ifndef NUDGE_MESSAGE_ITEM_H
#define NUDGE_MESSAGE_ITEM_H

#include "MessageItem.h"

class NudgeMessageItem : public MessageItem
{
	public:
		NudgeMessageItem(ConvMessage* message);
		virtual				~NudgeMessageItem();
		
		virtual void 		DrawItem(BView* owner, BRect itemRect, bool drawEverything = false);
		virtual void 		Update(BView* owner, const BFont* font);
};

#endif
