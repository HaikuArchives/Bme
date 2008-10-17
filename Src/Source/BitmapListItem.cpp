/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef BITMAP_LIST_ITEM_H
#include "BitmapListItem.h"
#endif

/**	Constructor 
	@param bitmap, the bitmap that will be draw before the text in
	this ListItem. This listitem takes ownership of this BBitmap: its
	destructor deletes it.
	@param label, the text this ListItem should draw
 */
BitmapListItem::BitmapListItem(BBitmap *bitmap, BString label)
				:	BListItem(),
				 	m_bitmap(bitmap),
				 	m_label(label)
{
}

/** Destructor
 */
BitmapListItem::~BitmapListItem()
{
	delete m_bitmap;
}

/**	Hook function: draws this item.
 */
void BitmapListItem::DrawItem(BView *owner, BRect frame, bool complete = false)
{
	font_height fh;
	owner->GetFontHeight(&fh);
	//draw selection border
	if (IsSelected() || complete) 
	{
		rgb_color color;
      	if (IsSelected()) 
      	{
         	color = ui_color(B_MENU_SELECTION_BACKGROUND_COLOR);
      	}
      	else 
      	{
        	color = owner->ViewColor();
        	
      	}
      	owner->SetHighColor(color);
      	owner->FillRect(frame);
   	}
	
	owner->SetDrawingMode(B_OP_ALPHA);
	rgb_color black = {0,0,0,255};
   	owner->SetHighColor(black);
	//draw the bitmap
	owner->MovePenTo(frame.left+1.0f,frame.top+1.0f);
	owner->DrawBitmap(m_bitmap);
	//draw the label
	float fontHeight = (float)ceil(fh.ascent + fh.leading + fh.descent);
	float dy = (frame.Height() - fontHeight) / 2;
	owner->MovePenTo(frame.left+5.0f+(m_bitmap->Bounds()).Width(),frame.bottom-dy);
	owner->DrawString(m_label.String());
}

void BitmapListItem::Update(BView *owner, const BFont *font)
{
	BListItem::Update(owner,font);
	//make height equal to the bitmap's height
	float height = (m_bitmap->Bounds()).Height() + 2.0f;
	SetHeight(height);
	//set width equal to the bitmap's width + the text width
	float width = (m_bitmap->Bounds()).Width() + owner->StringWidth(m_label.String()) + 3.0f;
	SetWidth(width);
}
