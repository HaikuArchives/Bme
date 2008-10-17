/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef NS_LOOPER_H
#define NS_LOOPER_H

#include <map>
#include "ServerLooper.h"
#include "SBLooper.h"

/** Class that handles the communication with both the Notification as the 
	Dispatch server.
	@author Tim de Jong
 */
class NSLooper : public ServerLooper
{
	public:
		NSLooper(BLooper *owner);
		virtual				~NSLooper();
		
		virtual void		MessageReceived(BMessage *message);
		virtual void		init();	
		
	private:	
		map<BString,SBLooper*>		sbLoopers;
};

#endif
