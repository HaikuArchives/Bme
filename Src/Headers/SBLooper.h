/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef SB_LOOPER_H
#define SB_LOOPER_H

#include <app/Message.h>
#include <app/Looper.h>
#include <support/String.h>
#include "ServerLooper.h"

class SBLooper : public ServerLooper
{
	public:
		SBLooper(BHandler *owner, BString passport, BString sbUrl, int32 port, BString authString);
		SBLooper(BHandler *owner, BString passport, BString sbUrl, int32 port, BString sessionId, BString authString);
		virtual				~SBLooper();
		
		virtual void		MessageReceived(BMessage* message);
		
		virtual void		init();
		virtual void		init(BMessage *message);
		void				SetAuthString(BString authString);				
		
	private:
		BString				m_passport,
							m_sbUrl,
							m_sessionId,
							m_authString;
		int32				m_port;
};

#endif
