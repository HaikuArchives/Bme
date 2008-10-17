/*****************************************************************
 * Copyright (c) 2005, Tim de Jong								 *
 *																 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
 
#ifndef STATUS_ITEM_H
#define STATUS_ITEM_H

#include <be/app/Message.h>
#include <be/interface/MenuItem.h>
#include <be/interface/Bitmap.h>

/**	MenuItem that represents the user status at a moment. Displays
	the status text and a status icon.
	@author Tim de Jong
 */
class StatusItem : public BMenuItem
{
	public:
		StatusItem(BBitmap *icon, const char *label, BMessage *message, char shortcut = 0, uint32 modifiers = 0);
		virtual				~StatusItem();
		
		virtual void		DrawContent();		
		
	private:
		BBitmap				*m_statusIcon;
};

#endif
