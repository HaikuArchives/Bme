/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef ICON_LOADER_H
#define ICON_LOADER_H

#include <be/storage/Path.h>
#include <vector.h>
#include "Emoticon.h"

/**	Opens an iconprefs file, parses it, and loads the emoticons found.
	@author Tim de Jong
*/
class IconLoader
{
	public:		
		//return vector with Emoticon structures?
		IconLoader(BPath iconPath);
		
		vector<Emoticon*>				getEmoticons();
		//vector<Status*>					getStatusses();

	private:
		vector<Emoticon*>				m_emoticons;
		//vector<Status*>					m_statusses;
};

#endif
