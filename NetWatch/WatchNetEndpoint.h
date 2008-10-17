/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#ifndef WATCH_NET_ENDPOINT_H
#define WATCH_NET_ENDPOINT_H

#include <NetEndpoint.h>
#include "NetWatchWin.h"

class WatchNetEndpoint : public BNetEndpoint
{
public:
	WatchNetEndpoint(const char* title, int protocol = SOCK_STREAM);
	virtual int32 Receive(void *buffer, size_t size, int flags = 0);
	virtual int32 Receive(BNetBuffer &buffer, size_t size, int flags = 0);
	virtual int32 Send(const void *buffer, size_t size, int flags = 0);
	virtual int32 Send(BNetBuffer &pack, int flags = 0);
	virtual ~WatchNetEndpoint();
protected:
	NetWatchWin* watchWin;
};

#endif
