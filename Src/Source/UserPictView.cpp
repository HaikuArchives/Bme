/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef USER_PICT_VIEW_H
#include "UserPictView.h"
#endif

#include <be/storage/Entry.h>
#include <be/translation/TranslationUtils.h>
#include <iostream>

UserPictView::UserPictView(BRect frame, BBitmap *bitmap, const char *name, BMessage *changedMessage, uint32 resizingMode, uint32 flags)
				:	BitmapView(frame, bitmap, name, resizingMode, flags),
					BInvoker(),
					m_changedMessage(changedMessage)
{
}

UserPictView::~UserPictView()
{
}

void UserPictView::AttachedToWindow()
{	
	SetMessage(m_changedMessage);
	//set message target to Parent View
	BMessenger owner(Parent());
	SetTarget(owner);
}

void UserPictView::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case B_SIMPLE_DATA:
		{
			entry_ref ref;
			if (message->FindRef("refs",&ref) == B_OK)
			{				
				BBitmap *bitmap = BTranslationUtils::GetBitmap(&ref);				
				SetBitmap(bitmap);
				Invoke();
			}	
		}
		break;
		default:
			BitmapView::MessageReceived(message);
		break;
	}
}
