/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef STATUSSES_H
#define STATUSSES_H

#include <be/storage/Path.h>
#include <be/support/Locker.h>
#include <be/support/String.h>
#include <map>
#include "Status.h"

typedef map<BString,Status*>::const_iterator SI;

extern BLocker statLock;
extern map<BString,Status*> statusses;

status_t LoadStatusses(BPath statFilePath);
void DeleteStatusses();

Status* FindStatus(BString abbreviation);
void PrintStatusses();

#endif
