#ifndef WARNING_MESSAGE_ITEM_H
#include "WarningMessageItem.h"
#endif

WarningMessageItem::WarningMessageItem(ConvMessage* message)
						:	MessageItem(message)
{
}

WarningMessageItem::~WarningMessageItem()
{
}
		
void WarningMessageItem::DrawItem(BView* owner, BRect itemRect, bool drawEverything)
{
}

void WarningMessageItem::Update(BView* owner, const BFont* font)
{
}