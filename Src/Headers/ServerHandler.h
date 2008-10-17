/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include <be/app/Handler.h>
#include <be/support/String.h>

/**	ServerHandler adds some convenience functions to a standard BHandler.
	@author Tim de Jong
 */
class ServerHandler : public BHandler
{
	public:
		ServerHandler();
		virtual				~ServerHandler();
		
		virtual void		SendCommandMessage(BMessage *message);
		virtual void		SendCommandMessageTrID(BMessage *message);
		virtual void		SendCommandMessageTrID(BMessage *message, int32 *currentTrId);
		virtual void		Error(BString error, bool critical);
};

#endif
