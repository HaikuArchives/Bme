/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#include "NetListView.h"
#include "NetListItem.h"

#include <Region.h>
#include <cstdio>

bool NetListView::UpdateItem(BListItem* item, void* data)
{
	//NetListView* curView = (NetListView*)data;
	//BFont curFont;
	//curView->GetFont(&curFont);
	//item->SetWidth(curView->Bounds().Width());
	//float wrapWidth = (curView->Bounds().Width() - 6)/curFont.Size();
	((NetListItem*)item)->CalcWordWrap(20);
	return false;
}

NetListView::NetListView(BRect frame, const char *name, list_view_type type = B_SINGLE_SELECTION_LIST, uint32 resizeMask = B_FOLLOW_LEFT, uint32 flags = B_WILL_DRAW)
	: BListView(frame, name, type, resizeMask, flags)
{
	oldWidth = frame.Width();
}

void NetListView::FrameResized(float width, float height)
{
	BListView::FrameResized(width, height);
	//Ensure the bevel on the right is drawn properly
	if(width < oldWidth)
		oldWidth = width;
	PushState();
	BRect invalRect(oldWidth, 0, oldWidth, height);
	ConvertFromParent(&invalRect);
	BRegion lineRegion(invalRect);
	ConstrainClippingRegion(&lineRegion);
	Draw(invalRect);
	oldWidth = width;
	PopState();
	
	//Do word wrapping
	BFont curFont;
	GetFont(&curFont);
	float itemWidth = Bounds().Width();
	float wrapWidth = (itemWidth - 6)/curFont.Size();
	for(int itemNum = 0; itemNum < CountItems(); itemNum++)
	{
		NetListItem* item = (NetListItem*)(Items()[itemNum]);
		item->SetWidth(itemWidth);
		item->CalcWordWrap(wrapWidth);
	}
	//DoForEach(UpdateItem, (void*)this);
	Invalidate();
	
	BListView::FrameResized(width, height);
}
