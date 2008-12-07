#ifndef NOTIFICATION_WINDOW_H
#include "NotificationWindow.h"
#endif

#include <app/Messenger.h>
#include <interface/Font.h>
#include <interface/Screen.h>
#include <be_apps/Deskbar/Deskbar.h>
#include "constants.h"
#include "TextWrapper.h"
#include "TaggedText.h"
#include "TextTag.h"
#include <iostream>

NotificationWindowFactory::NotificationWindowFactory()
								:	m_previousWindow(NULL)
{
}

NotificationWindowFactory::~NotificationWindowFactory()
{
}
		
void NotificationWindowFactory::ShowNotification(BString notificationString, BMessage *notificationMsg)
{
	//Notification Window that is 150 pixels wide and 150 pixels deep
	BRect frame = CalculateNotificationFrame();
	
	//if another NotificationWindow is currently available, show the new one above or below it
	if (m_previousWindow)
	{
		BRect m_previousBounds = m_previousWindow->Bounds();
		
		float frameHeight = frame.Height();
		float windowDistance = 3.0f;
		
		BScreen screen;		
		float halfScreenHeight = screen.Frame().Height() / 2.0f;
		//TODO: does not work perfectly yet!
		//find out whether we should display the next notification above or below the previous		
		if (m_previousBounds.bottom < halfScreenHeight)
		{
			//display below
			float yBottom = m_previousBounds.bottom + windowDistance;
			frame.OffsetBy(0.0f, yBottom);
		}
		else
		{
			//display above
			float yTop = m_previousBounds.top - windowDistance - frameHeight;
			frame.OffsetBy(0.0f, yTop);
		}		
	}
	//show the notification window
	NotificationWindow *notificationWindow = new NotificationWindow(frame, notificationString, notificationMsg);
	notificationWindow->Show();
	//this is the last created notification window, assign it to the previousWindow member variable
	m_previousWindow = notificationWindow;
}

BRect NotificationWindowFactory::CalculateNotificationFrame()
{
	float K_NOTIFICATION_WIDTH = 160.0f;
	float K_NOTIFICATION_HEIGHT = 100.0f;
	float K_NOTIFICATION_X_OFFSET = 2.0f;
	float K_NOTIFICATION_Y_OFFSET = 2.0f;
	
	BRect frame(0.0f,0.0f,K_NOTIFICATION_WIDTH,K_NOTIFICATION_HEIGHT);
	
	BDeskbar deskbar;
	BRect deskbarFrame = deskbar.Frame();	
	deskbar_location deskbarLocation = deskbar.Location();

	float leftTopX = 0.0f;
	float leftTopY = 0.0f;
	//deskbar in horizontal position 	
	if (deskbarLocation == B_DESKBAR_TOP)
	{
		//deskbar on top				
		leftTopX = deskbarFrame.right - K_NOTIFICATION_WIDTH - K_NOTIFICATION_X_OFFSET;
		leftTopY = deskbarFrame.bottom + K_NOTIFICATION_Y_OFFSET;				
	}
	else if (deskbarLocation == B_DESKBAR_BOTTOM)
	{
		//deskbar below
		leftTopX = deskbarFrame.right - K_NOTIFICATION_WIDTH - K_NOTIFICATION_X_OFFSET;
		leftTopY = deskbarFrame.top - K_NOTIFICATION_HEIGHT - K_NOTIFICATION_Y_OFFSET;
	}
	else if (deskbarLocation == B_DESKBAR_LEFT_BOTTOM || deskbarLocation == B_DESKBAR_RIGHT_BOTTOM)
	{
		//TODO:goes outside screen bounds! fix!
		leftTopX = deskbarFrame.left + K_NOTIFICATION_X_OFFSET;
		leftTopY = deskbarFrame.top - K_NOTIFICATION_HEIGHT - K_NOTIFICATION_Y_OFFSET;
	}
	else if (deskbarLocation == B_DESKBAR_LEFT_TOP)
	{			
		//deskbar in vertical position
		leftTopY = deskbarFrame.top;				
		leftTopX = deskbarFrame.right + K_NOTIFICATION_X_OFFSET;
	}
	else if (deskbarLocation == B_DESKBAR_RIGHT_TOP)
	{
		//deskbar on the right of the screen
		leftTopY = deskbarFrame.top;
		leftTopX = deskbarFrame.left - K_NOTIFICATION_WIDTH - K_NOTIFICATION_X_OFFSET;
	}		
	frame.OffsetBy(leftTopX,leftTopY);
	
	return frame;
}

//=========================NotificationWindow================================================
NotificationWindow::NotificationWindow(BRect frame, BString notificationString, BMessage *notificationMsg, bigtime_t notificationLength)
						:	BWindow(frame, "NotificationWindow", B_BORDERED_WINDOW_LOOK, B_FLOATING_ALL_WINDOW_FEEL,0)
							
{
	NotificationView *notificationView = new NotificationView(Bounds(), notificationString, notificationMsg);
	AddChild(notificationView);
	
	BMessenger target(NULL,this);
	m_runner = new BMessageRunner(target, new BMessage(K_CLOSE_NOTIFICATION_MSG), notificationLength, 1);
}

NotificationWindow::~NotificationWindow()
{
	delete m_runner;
}

void NotificationWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case K_CLOSE_NOTIFICATION_MSG:
		{
			cout << "closing" << endl;
			Quit();
		}
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}

//=========================NotificationView================================================
NotificationView::NotificationView(BRect frame, BString notificationString, BMessage *notificationMsg)
					:	GradientView(frame, "NotificationView", B_FOLLOW_ALL_SIDES, AppConstants::K_TOP_COLOUR, AppConstants::K_BOTTOM_COLOUR),
						m_notificationString(notificationString)						
{
}

NotificationView::~NotificationView()
{
}

void NotificationView::Draw(BRect updateRect)
{
	PushState();
	//first draw parent
	GradientView::Draw(updateRect);
	PopState();
	//the entire view can be used to draw the text
	BRect enclosingRect = Bounds();	
	enclosingRect.InsetBy(2.0f,2.0f);
	TaggedText* text = new TaggedText();
	text->Add(new TextTag("obelmiks@hotmail.com")); 
	text->Add(new TextTag("has just signed in"));		
	//draw text with wrapping
	TextWrapper textWrapper(this);	
	SetFont(be_bold_font);
	LineBuffer* lineBuffer = textWrapper.CalculateTextWrapping(enclosingRect, text);	
	textWrapper.DrawLineBuffer(enclosingRect,lineBuffer);	
	delete lineBuffer;
	delete text;
}
