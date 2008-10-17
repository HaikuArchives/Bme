#ifndef PREFS_METHODS_H
#include "PrefsMethods.h"
#endif

#include <app/Application.h>
#include "PrefsConstants.h"

Preferences* PrefsMethods::GetPreferences(BString prefsName)
{
	BMessage *prefsMsg = new BMessage(PrefsMessages::K_GET_PREFS_MSG);
	
	BMessage replyMsg;
	be_app_messenger.SendMessage(prefsMsg, &replyMsg);
	return GetPrefsFromMessage(replyMsg);
}

void PrefsMethods::ChangePreferences(Preferences* prefs)
{
	BMessage *changeMsg = new BMessage(PrefsMessages::K_CHANGE_PREFS_MSG);
	
	BMessage prefsMsg;
	if (prefs->Archive(&prefsMsg) == B_OK)
	{
		changeMsg->AddMessage(K_PREFS_MSG_FIELD, &prefsMsg); 
		be_app_messenger.SendMessage(changeMsg);
	}
}

Preferences* PrefsMethods::GetPrefsFromMessage(BMessage msg)
{
	BArchivable *arch = Preferences::Instantiate(&msg);
	if (arch)
	{
		Preferences* prefs = dynamic_cast<Preferences*>(arch);
		return prefs;
	}
	return NULL;
}
