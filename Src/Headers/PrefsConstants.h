#ifndef PREFS_CONSTANTS_H
#define PREFS_CONSTANTS_H

#include "Preferences.h"

//message fields
#define				K_PREFS_NAME_FIELD			"prefsName"
#define				K_PREFS_PATH_FIELD			"prefsPath"
#define				K_PREFS_MSG_FIELD			"prefsMsg"
//message types
namespace PrefsMessages
{
	const uint32
					K_GET_PREFS_MSG				= 'GTpr',
					K_CHANGE_PREFS_MSG			= 'CHpr',
					K_SAVE_PREFS_MSG			= 'SApr',
					K_LOAD_PREFS_MSG			= 'LOpr'
					;		
};

#endif
