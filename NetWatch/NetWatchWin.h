/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#ifndef _NET_WATCH_WIN_H
#define _NET_WATCH_WIN_H

#include <Window.h>

class BListView;
class BView;

class NetWatchWin : public BWindow
{
	public:
		//publics functions
		NetWatchWin(BRect frame, const char *title);
		virtual void				MessageReceived(BMessage* message);
		
	protected:
		BListView*					m_listView;
};

#endif
