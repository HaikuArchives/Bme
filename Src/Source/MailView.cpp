/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MAIL_VIEW_H
#include "MailView.h"
#endif

#include <be/interface/Window.h>
#include <be/interface/InterfaceDefs.h>
#include <be/interface/GraphicsDefs.h>
#include <be/app/Cursor.h>
#include <AppDefs.h>

#include "constants.h"

MailView::MailView(BRect frame)
			:	BView(frame,"MailView",B_FOLLOW_LEFT | B_FOLLOW_RIGHT,B_WILL_DRAW),
				m_numberOfEmails(0),
				m_emailText("No New E-mail Messages")
				
{
	//set background colour to the grey panel background colour
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	//make bitmap out of constant in header
	BRect letterBounds(0.0,0.0,Letter::K_LETTER_WIDTH-1.0,Letter::K_LETTER_HEIGHT-1.0);
	m_letterBitmap = new BBitmap(letterBounds,0,Letter::K_LETTER_FORMAT);
	if (m_letterBitmap && m_letterBitmap->IsValid()) 
	{
    	memcpy(m_letterBitmap->Bits(), Letter::K_LETTER_BITS, m_letterBitmap->BitsLength());
    }
}

MailView::~MailView()
{
}

void MailView::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		//handle new mail message here!
		default:
			BView::MessageReceived(message);
		break;
	}
}

void MailView::Draw(BRect updateRect)
{
	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	DrawBitmap(m_letterBitmap, Bounds().LeftTop());
	SetDrawingMode(B_OP_OVER);
	
	float textX = Letter::K_LETTER_WIDTH + 5;
	float textY = Bounds().bottom-5;
	MovePenTo(textX,textY);
	
	SetFont(be_bold_font);
	SetFontSize(10);		

	rgb_color blue = {0,0,250,0};
	SetHighColor(blue);
		
	DrawString(m_emailText.String());
	float width = StringWidth(m_emailText.String());
	BPoint p1(textX,textY + 1);
	BPoint p2(textX + width,textY + 1);
	StrokeLine(p1,p2);
}

void MailView::MouseMoved(BPoint point, uint32 transit, const BMessage *message)
{
	BView::MouseMoved(point,transit,message);
	if ((transit == B_ENTERED_VIEW || transit == B_INSIDE_VIEW) && Window()->IsActive())
	{
		//set hyperlink cursor
		BCursor hyperlinkCursor(K_CURSOR_HYPERLINK);
		SetViewCursor(&hyperlinkCursor);
	}
	else if (transit == B_EXITED_VIEW)
	{
		//set to normal cursor
		SetViewCursor(B_CURSOR_SYSTEM_DEFAULT);
	}
}

void MailView::MouseDown(BPoint point)
{
	BView::MouseDown(point);
	//send open mail message!
}

void MailView::setNumberOfEmails(int32 emails)
{
	m_numberOfEmails = emails;
	//make sure the number of emails isn't negative
	if (m_numberOfEmails < 0)
		m_numberOfEmails = 0;
	
	if (m_numberOfEmails == 0)
	 	m_emailText = "No New E-mail Messages";
	else if (m_numberOfEmails == 1) 
		m_emailText = "One New E-mail Message";
	else  
	{	
		m_emailText = "";
		m_emailText << m_numberOfEmails << " New E-mail Messages";		
	}
}
