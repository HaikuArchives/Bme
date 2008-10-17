/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef NS_UI_FILTER_H
#define NS_UI_FILTER_H

#include <be/app/Handler.h>
#include <be/app/Message.h>
#include <be/app/MessageFilter.h>

class NSUIFilter : public BMessageFilter
{
	public:
		NSUIFilter();
		virtual					~NSUIFilter();
		
		virtual filter_result	Filter(BMessage *message, BHandler **target);
	
};

#endif
