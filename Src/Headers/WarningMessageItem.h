#ifndef WARNING_MESSAGE_ITEM_H
#define WARNING_MESSAGE_ITEM_H

#include "MessageItem.h"

class WarningMessageItem : public MessageItem
{
	public:
		WarningMessageItem(ConvMessage* message);
		virtual				~WarningMessageItem();
		
		virtual void 		DrawItem(BView* owner, BRect itemRect, bool drawEverything = false);
		virtual void 		Update(BView* owner, const BFont* font);
};

#endif