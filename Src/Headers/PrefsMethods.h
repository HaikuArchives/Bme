#ifndef PREFS_METHODS_H
#define PREFS_METHODS_H

#include "Preferences.h"

//convenience methods
namespace PrefsMethods
{
	Preferences*	GetPreferences(BString prefsName);
	Preferences*	GetPrefsFromMessage(BMessage msg);
	void			ChangePreferences(Preferences* prefs);
};

#endif
