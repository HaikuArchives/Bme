/*****************************************************************
 * Copyright (c) 2005 Tim de Jong 								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef STATUS_MENU_H
#define STATUS_MENU_H

#include <be/interface/PopUpMenu.h>

class StatusMenu : public BPopUpMenu
{
	public:
		StatusMenu();
		virtual				~StatusMenu();
		
		void				SetStatus(const char* statusString);
};

#endif
