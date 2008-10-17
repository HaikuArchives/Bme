/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef ANIMATED_BITMAP_VIEW
#define ANIMATED_BITMAP_VIEW

#include <be/interface/View.h>
#include <be/interface/Bitmap.h>

class AnimatedBitmapView : public BView
{
	public:
		AnimatedBitmapView(BRect frame, const char *name, uint32 resizingMode, uint32 flags,BBitmap *viewBitmap);
		virtual void		AttachedToWindow();
		virtual void		Draw(BRect updateRect);
		
	private:		
		BBitmap				*m_viewBitmap;
};

#endif
