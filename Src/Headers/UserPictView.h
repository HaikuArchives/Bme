/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef USER_PICT_VIEW_H
#define USER_PICT_VIEW_H

#include <be/app/Message.h>
#include <be/app/Invoker.h>
#include <be/interface/Rect.h>
#include <be/interface/Bitmap.h>
#include "BitmapView.h"

class UserPictView : public BitmapView, BInvoker
{
	public:
		UserPictView(BRect frame, BBitmap *bitmap, const char *name, BMessage *changedMessage, uint32 resizingMode, uint32 flags);
		virtual				~UserPictView();	 
		
		virtual void		AttachedToWindow();
		virtual void		MessageReceived(BMessage *message);
		
	private:
		BMessage*			m_changedMessage;
};

#endif
