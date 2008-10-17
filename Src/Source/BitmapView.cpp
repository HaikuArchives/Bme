/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef BITMAP_VIEW_H
#include "BitmapView.h"
#endif

BitmapView::BitmapView(BRect frame, BBitmap *bitmap, const char *name, uint32 resizingMode, uint32 flags)
				:	BView(frame,name,resizingMode, flags | B_WILL_DRAW),
					m_bitmap(bitmap)
{
}

BitmapView::~BitmapView()
{
	delete m_bitmap;
}

void BitmapView::Draw(BRect updateRect)
{	
	BRect bounds = Bounds();
	//draw border
	rgb_color black = {0,0,0};
	SetHighColor(black);
	StrokeRect(bounds);
	//draw bitmap
	bounds.InsetBy(1.0f, 1.0f);
	DrawBitmap(m_bitmap, m_bitmap->Bounds(),bounds);
}

void BitmapView::SetBitmap(BBitmap *bitmap)
{
	delete m_bitmap;
	m_bitmap = bitmap;
	Invalidate();
}

BBitmap* BitmapView::GetBitmap() const
{
	return m_bitmap;
}
