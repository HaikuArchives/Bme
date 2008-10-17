#ifndef NOTIFICATION_WINDOW_H
#define NOTIFICATION_WINDOW_H

#include <app/Message.h>
#include <app/MessageRunner.h>
#include <interface/View.h>
#include <interface/Window.h>
#include <support/String.h>
#include "GradientView.h"	

#define K_CLOSE_NOTIFICATION_MSG 			'CLnt'

class NotificationWindow;

class NotificationWindowFactory
{
	public:
		NotificationWindowFactory();
		virtual				~NotificationWindowFactory();
		
		void 				ShowNotification(BString notificationString, BMessage *notificationMsg);
		BRect 				CalculateNotificationFrame();
		
	private:
		NotificationWindow	*m_previousWindow;		
};

class NotificationWindow : public BWindow
{
	public:
		NotificationWindow(BRect frame, BString notificationString, BMessage *notificationMsg, bigtime_t notificationLength = 2000000);
		virtual				~NotificationWindow();	
		
		virtual void		MessageReceived(BMessage *message);
	
	private:
		BMessageRunner* 	m_runner;
};

class NotificationView : public GradientView
{	
	public:
		NotificationView(BRect frame, BString notificationString, BMessage *notificationMsg);
		virtual				~NotificationView();
		
		virtual void		Draw(BRect updateRect);	
	
	private:		
		BString				m_notificationString;		
		BMessage			m_notificationMsg;
};

#endif
