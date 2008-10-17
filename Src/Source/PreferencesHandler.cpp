#ifndef PREFERENCES_HANDLER_H
#include "PreferencesHandler.h"
#endif

#include "PrefsConstants.h"

PreferencesHandler::PreferencesHandler()
						:	BHandler()
{
	m_prefsFactory = new PreferenceFactory();
}

PreferencesHandler::PreferencesHandler(PreferenceFactory *prefsFactory)
						:	m_prefsFactory(prefsFactory)
{
}

PreferencesHandler::~PreferencesHandler()
{
	delete m_prefsFactory;
}

void PreferencesHandler::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case PrefsMessages::K_GET_PREFS_MSG:				
		{
			BString prefsName;
			if (message->FindString(K_PREFS_NAME_FIELD, &prefsName))
			{
				BMessage prefsMsg = m_prefsFactory->GetPreferences(prefsName);
				message->SendReply(&prefsMsg);
			}
		}
		break;
		case PrefsMessages::K_CHANGE_PREFS_MSG:
		{
			BMessage prefsMsg;
			if (message->FindMessage(K_PREFS_MSG_FIELD, &prefsMsg))
			{				
				m_prefsFactory->ChangePreferences(prefsMsg);				
			}
		}
		break;
		case PrefsMessages::K_SAVE_PREFS_MSG:
		{
			BString prefsPath;
			if (message->FindString(K_PREFS_PATH_FIELD, &prefsPath))
			{
				//TODO: exception handling if Save goes awry
				m_prefsFactory->Save(prefsPath.String());
			}
		}
		break;
		case PrefsMessages::K_LOAD_PREFS_MSG:
		{
			BString prefsPath;
			if (message->FindString(K_PREFS_PATH_FIELD, &prefsPath))
			{
				//TODO: exception handling if load goes awry
				m_prefsFactory->Load(prefsPath.String());
			}
		}
		break;
		default:
			BHandler::MessageReceived(message);
		break;
	}
}