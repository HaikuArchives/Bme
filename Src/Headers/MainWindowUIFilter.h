/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MAIN_WINDOW_UI_FILTER_H
#define MAIN_WINDOW_UI_FILTER_H

#include <be/app/Handler.h>
#include <be/app/Message.h>
#include <be/app/MessageFilter.h>

class MainWindowUIFilter : public BMessageFilter
{
	public:
		MainWindowUIFilter();
		virtual					~MainWindowUIFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
};

#endif
