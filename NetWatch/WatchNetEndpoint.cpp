/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#include "WatchNetEndpoint.h"
#include "NetListItem.h"
#include <ListView.h>
#include <String.h>
#include <cstring>

WatchNetEndpoint::WatchNetEndpoint(const char* title, int protocol = SOCK_STREAM)
	: BNetEndpoint(protocol)
{
	watchWin = new NetWatchWin(BRect(50.00,100.00,300.00,400.00), title);
	watchWin->Show();
}

int32 WatchNetEndpoint::Receive(void *buffer, size_t size, int flags = 0)
{
	int32 returnVal = BNetEndpoint::Receive(buffer, size, flags);
	if(returnVal > 0)
	{
		char* recStr = new char[size + 1];
		if (size > 0)
		{
			strncpy(recStr, (char*)buffer, size);
		}
		recStr[size] = 0;
		BMessage recMsg('ADMS');
		recMsg.AddInt8("Type", DATA_RECEIVED_EVENT);
		recMsg.AddString("Text", recStr);
		watchWin->PostMessage(&recMsg);
		delete[] recStr;
	}
	return returnVal;
}

int32 WatchNetEndpoint::Receive(BNetBuffer &buffer, size_t size, int flags = 0)
{
	int32 returnVal = BNetEndpoint::Receive(buffer, size, flags);
	if(returnVal > 0)
	{
		BString recStr;
		if (buffer.Size() > 0)
		{	
			recStr.SetTo((const char*)buffer.Data(), buffer.Size());
		}
		
		BMessage recMsg('ADMS');
		recMsg.AddInt8("Type", DATA_RECEIVED_EVENT);
		recMsg.AddString("Text", recStr);
		watchWin->PostMessage(&recMsg);
	}
	return returnVal;
}


int32 WatchNetEndpoint::Send(const void *buffer, size_t size, int flags = 0)
{
	int32 returnVal = BNetEndpoint::Send(buffer, size, flags);
	if(returnVal > 0)
	{
		char* sentStr = new char[returnVal + 1];
		if (size > 0)
		{
			strncpy(sentStr, (char*)buffer, returnVal);
		}
		sentStr[size] = 0;
		
		BMessage sentMsg('ADMS');
		sentMsg.AddInt8("Type", DATA_SENT_EVENT);
		sentMsg.AddString("Text", sentStr);
		watchWin->PostMessage(&sentMsg);
		delete[] sentStr;
	}
	return returnVal;
}

int32 WatchNetEndpoint::Send(BNetBuffer &pack, int flags)
{
	return Send(pack.Data(),pack.Size(),flags);
}

WatchNetEndpoint::~WatchNetEndpoint()
{
	BMessage endpointDestroyedMsg('ADMS');
	endpointDestroyedMsg.AddInt8("Type", INFO_EVENT);
	endpointDestroyedMsg.AddString("Text", "BNetEndpoint Destroyed");
	watchWin->PostMessage(&endpointDestroyedMsg);
}
