#ifndef NUDGE_MESSAGE_ITEM_H
#include "NudgeMessageItem.h"
#endif

NudgeMessageItem::NudgeMessageItem(ConvMessage* message)
						:	MessageItem(message)
{
}

NudgeMessageItem::~NudgeMessageItem()
{
}
		
void NudgeMessageItem::DrawItem(BView* owner, BRect itemRect, bool drawEverything)
{
}

void NudgeMessageItem::Update(BView* owner, const BFont* font)
{
}
