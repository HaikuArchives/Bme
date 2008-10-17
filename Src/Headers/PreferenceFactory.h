#ifndef PREFERENCE_FACTORY_H
#define PREFERENCE_FACTORY_H

#include <app/Message.h>
#include <support/Locker.h>
#include "Preferences.h"

class PreferenceFactory
{
	public:
		PreferenceFactory();		
		virtual				~PreferenceFactory();
				
		virtual BMessage	GetPreferences(BString prefsName);		
		virtual void		ChangePreferences(BMessage prefsMsg);
		
		virtual status_t	Save(const char *path);
		virtual status_t	Load(const char *path);
	
	private:
		virtual BMessage	CreatePreferences(BString prefsName);
	
	private:
		BLocker 			m_prefsLock;
		BMessage			m_preferences;
};

#endif
