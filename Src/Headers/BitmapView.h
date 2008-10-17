/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef BITMAP_VIEW_H
#define BITMAP_VIEW_H

#include <be/interface/View.h>
#include <be/interface/Bitmap.h>

class BitmapView : public BView
{
	public:
		BitmapView(BRect frame, BBitmap *bitmap, const char *name, uint32 resizingMode, uint32 flags);
		virtual				~BitmapView();
		
		virtual void		Draw(BRect updateRect);
				
		void				SetBitmap(BBitmap *bitmap);
		BBitmap*			GetBitmap() const;
		
	private:
		BBitmap				*m_bitmap;
};

#endif
