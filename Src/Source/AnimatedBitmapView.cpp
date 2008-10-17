/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef ANIMATED_BITMAP_VIEW
#include "AnimatedBitmapView.h"
#endif

AnimatedBitmapView::AnimatedBitmapView(BRect frame, const char *name, uint32 resizingMode, uint32 flags,BBitmap *viewBitmap)
						:	BView(frame,name,resizingMode,flags),
							m_viewBitmap(viewBitmap)
{
}

void AnimatedBitmapView::AttachedToWindow()
{	
}

void AnimatedBitmapView::Draw(BRect updateRect)
{
	SetDrawingMode(B_OP_ALPHA);
	DrawBitmap(m_viewBitmap);
}
