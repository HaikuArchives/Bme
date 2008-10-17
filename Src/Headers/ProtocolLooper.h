/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef PROTOCOL_LOOPER_H
#define PROTOCOL_LOOPER_H

#include <app/Looper.h>
#include <app/Message.h>
#include <support/String.h>
#include "NSLooper.h"
#include "SBLooper.h"

/**
	@author Tim de Jong
 */
class ProtocolLooper : public BLooper
{
	public:
		ProtocolLooper();
		virtual				~ProtocolLooper();
				
		virtual void		MessageReceived(BMessage *message);
		virtual bool		QuitRequested();
		
		void				setOwner(BLooper *owner);
		BLooper*			getOwner();		
		
	private:
		BLooper				*m_owner;
		NSLooper			*m_nsLooper;			
			
};

#endif
