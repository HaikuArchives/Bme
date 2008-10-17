/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef CONV_LOG_VIEW_H
#define CONV_LOG_VIEW_H

#include <app/Message.h>
#include <interface/Rect.h>
#include <interface/Point.h>
#include <interface/ListView.h>
#include <interface/Bitmap.h>
#include <support/String.h>
#include <vector.h>
#include "ConvMessage.h"

class ConvLogView : public BListView
{
	public:
		ConvLogView(BRect frame, BRect textRect);
		virtual				~ConvLogView();
		
		virtual void		MessageReceived(BMessage *message);
		virtual void		MouseMoved(BPoint point, uint32 transit, const BMessage *message);
		virtual void		MouseDown(BPoint point);		
				
		void				AddMessage(ConvMessage *message);		
		void				ChangeStatus(BString);
		
	protected:
		// This class holds information about an URL. It holds the
		// position of the anchor inside the text view and the href. 
		/*class URLInfo
		{
			public:
				URLInfo(int32 start, int32 end, BString href);
				virtual				~URLInfo();
			
				bool				Contains(ConvLogView *view, BPoint point);
				BString				GetURL();
									
			protected:
				//start and end index inside the text view
				int32 				m_start, 
									m_end;
										
				BString				m_href;			
		};
		
		// simple wrapper class for text_run
		class TextRun : public text_run
		{
			public:
				TextRun(const text_run &other); 
				TextRun(int32 offset, BFont font, rgb_color color);
		};
		
	protected:	
		vector <URLInfo*>				m_urlInfoList;
		vector <BBitmap*>				m_emoticonList;*/
};

#endif
