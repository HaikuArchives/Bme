/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#include <be/app/Application.h>
#include <be/interface/View.h>
#include <be/interface/ScrollView.h>
#include <cstdio>
#include <iostream>
#include "NetWatchWin.h"
#include "NetListItem.h"
#include "NetListView.h"

NetWatchWin::NetWatchWin(BRect frame, const char *title)
				:	BWindow(frame, title, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	float minWidth, maxWidth, minHeight, maxHeight;
	GetSizeLimits(&minWidth, &maxWidth, &minHeight, &maxHeight);
	minWidth = 200;
	minHeight = 100;
	//SetSizeLimits(minWidth, maxWidth, minHeight, maxHeight);

	BRect r = Bounds();
	r.right -= 15; 

	m_listView = new NetListView(r, "trafficlog", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS);
	m_listView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(new BScrollView("log_scroller", m_listView, B_FOLLOW_ALL_SIDES, B_FULL_UPDATE_ON_RESIZE, false, true, B_NO_BORDER));

	BFont viewFont;
	m_listView->GetFont(&viewFont);

	m_listView->AddItem(new NetListItem(INFO_EVENT, "BNetEndpoint Created", &viewFont));
}

void NetWatchWin::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case 'ADMS':
		{
			const char* string = "";
			message->FindString("Text", &string);		
			int8 type = 0;
			message->FindInt8("Type", &type);
			BFont viewFont;
			m_listView->GetFont(&viewFont);
			switch(type)
			{
				case INFO_EVENT:
					m_listView->AddItem(new NetListItem(INFO_EVENT, string, &viewFont));
				break;
				case DATA_RECEIVED_EVENT:
					m_listView->AddItem(new NetListItem(DATA_RECEIVED_EVENT, string, &viewFont));
				break;
				case DATA_SENT_EVENT:
					m_listView->AddItem(new NetListItem(DATA_SENT_EVENT, string, &viewFont));
				break;
			}
			//scroll to last item in list
			m_listView->Select(m_listView->CountItems() - 1);
			m_listView->ScrollToSelection();
		}
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}
