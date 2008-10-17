/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef BME_APP_H
#define BME_APP_H

#include <be/app/Message.h>
#include <be/app/Application.h>
#include <be/interface/Rect.h>
#include <be/storage/Path.h>
#include "ProtocolLooper.h"
#include "PreferenceFactory.h"

/**	Application object for the Bme, BeOS msn-messenger.
	@author Tim de Jong
 */
class BmeApp : public BApplication
{
	public:
		BmeApp();
		virtual				~BmeApp();
		
		virtual void		MessageReceived(BMessage* message);
		virtual void		AboutRequested();		
		
	private:
		ProtocolLooper		*m_protocolLooper;
		PreferenceFactory	*m_prefsFactory;
		BRect				m_mainWindowFrame;
		BPath				m_prefsPath;		
};

#endif
