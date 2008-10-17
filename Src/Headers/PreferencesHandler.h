#ifndef PREFERENCES_HANDLER_H
#define PREFERENCES_HANDLER_H

#include <app/Handler.h>
#include <app/Message.h>
#include "PreferenceFactory.h"

class PreferencesHandler : public BHandler
{
	public:
		PreferencesHandler();
		PreferencesHandler(PreferenceFactory *prefsFactory);
		virtual				~PreferencesHandler();
		
		virtual void		MessageReceived(BMessage* message);
	
	private:
		PreferenceFactory	*m_prefsFactory;
};

#endif
