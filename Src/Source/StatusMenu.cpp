/*****************************************************************
 * Copyright (c) 2005 Tim de Jong 								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
 
#ifndef STATUS_MENU_H
#include "StatusMenu.h"
#endif

#include <be/interface/MenuItem.h>

StatusMenu::StatusMenu()
				:	BPopUpMenu("Status")
{
	float height = 10.0f;
	SetItemMargins(0.0f,0.0f,100.0f,height);
}

StatusMenu::~StatusMenu()
{
}

void StatusMenu::SetStatus(const char *statusString)
{
	BMenuItem* selected = FindItem(statusString);
	if (selected)
		selected->SetMarked(true);
}


