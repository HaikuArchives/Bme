#ifndef FILE_MESSAGE_ITEM_H
#include "FileMessageItem.h"
#endif

FileMessageItem::FileMessageItem(ConvMessage* message)
						:	MessageItem(message)
{
}

FileMessageItem::~FileMessageItem()
{
}
		
void FileMessageItem::DrawItem(BView* owner, BRect itemRect, bool drawEverything)
{
}

void FileMessageItem::Update(BView* owner, const BFont* font)
{
}
