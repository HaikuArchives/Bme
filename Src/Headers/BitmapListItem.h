/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef BITMAP_LIST_ITEM_H
#define BITMAP_LIST_ITEM_H

#include <String.h>
#include <be/interface/View.h>
#include <be/interface/ListItem.h>
#include <be/interface/Bitmap.h>

/** Draws a listitem with a bitmap/icon at front and the text after it.
	@author Tim de Jong
 */
class BitmapListItem : public BListItem
{
	public:
		BitmapListItem(BBitmap *bitmap, BString label);
		virtual				~BitmapListItem();
		
		virtual void		DrawItem(BView *owner, BRect frame, bool complete = false); 
		virtual void		Update(BView *owner, const BFont *font);
		
	private:
		BBitmap				*m_bitmap;
		BString				m_label;
};

#endif
