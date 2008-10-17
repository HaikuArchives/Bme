#ifndef PREFERENCE_FACTORY_H
#include "PreferenceFactory.h"
#endif

#include <storage/File.h>

//TODO: add a PreferencesHandler : public BHandler class! this will be added to the be_app

PreferenceFactory::PreferenceFactory()
{
}

PreferenceFactory::~PreferenceFactory()
{
}

//TODO: test!!!
BMessage PreferenceFactory::GetPreferences(BString prefsName)
{
	BMessage msg;
	status_t error = m_preferences.FindMessage(prefsName.String(), &msg);
	if (error == B_OK)
	{
		return msg;
	}	
	//if failed to retrieved the message from the earlier retreived onces
	BMessage prefs = CreatePreferences(prefsName);
	return prefs;	
}

void PreferenceFactory::ChangePreferences(BMessage prefsMsg)
{
	BArchivable *arch = Preferences::Instantiate(&prefsMsg);
	//TODO: throw exception if not succeeded!
	if (arch)
	{
		Preferences *prefs = dynamic_cast<Preferences*>(arch);
		BString prefsName = prefs->GetName();

		m_preferences.ReplaceMessage(prefsName.String(), &prefsMsg);			
	}
}

/**	From SimplyVorbis code
	Get the original code here: http://www.bebits.com/app/4227
	Code under BSD/MIT licence, code by Darkwyrm
 */	
status_t PreferenceFactory::Save(const char *path)
{
	if(!path)
		return B_ERROR;
	
	m_prefsLock.Lock();
	
	BFile file(path, B_READ_WRITE | B_ERASE_FILE | B_CREATE_FILE);
	
	status_t status = file.InitCheck();
	if(status != B_OK)
	{
		m_prefsLock.Unlock();
		return status;
	}
	
	status = m_preferences.Flatten(&file);
		
	m_prefsLock.Unlock();
	return status;
}

/**	From SimplyVorbis code
	Get the original code here: http://www.bebits.com/app/4227
	Code under BSD/MIT licence, code by Darkwyrm
 */	
status_t PreferenceFactory::Load(const char *path)
{
	if(!path)
		return B_ERROR;
	
	m_prefsLock.Lock();
	
	BFile file(path, B_READ_ONLY);

	BMessage msg;
	
	status_t status = file.InitCheck();
	if(status != B_OK)
	{
		m_prefsLock.Unlock();
		return status;
	}
	
	status = msg.Unflatten(&file);
	if(status == B_OK)
		m_preferences = msg;
		
	m_prefsLock.Unlock();
	return status;
	
}
//TODO: could go wrong, possible fix?
BMessage PreferenceFactory::CreatePreferences(BString prefsName)
{
	Preferences prefs(prefsName);
	//add the create preference object to the general preferences message 
	//containing all the preferences.
	BMessage message; 
	if (prefs.Archive(&message) == B_OK)
	{
		m_preferences.AddMessage(prefsName.String(), &message);
	}
	
	return message;
}
