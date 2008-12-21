/*****************************************************************
 * Copyright (c) 2005 Tim de Jong 								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef STATUS_H
#include "Status.h"
#endif

#include <be/support/TypeConstants.h>
#include <be/support/ClassInfo.h>
#include "ProtocolConstants.h"

Status::Status()
			:	BArchivable()
{
}

/**	Constructor
	@param statName,
	@param statAbbr,
	@param icon
	@param usrChoice	
 */
Status::Status(BString statName, BString statAbbr, BBitmap* icon, bool usrChoice, rgb_color statusColour)
			:	BArchivable(),
				m_statusName(statName),
				m_statusAbbreviation(statAbbr),
				m_userChoice(usrChoice),
				m_statusColour(statusColour)					
			
{
	AddIcon(icon);		
}

/**	Archiving constructor
	Produces a Status object from a message archive.
	@param archive, the archived object.
 */
Status::Status(BMessage* archive)
			:	BArchivable(archive)
{
	archive->FindString("Status::statusName",&m_statusName);
	archive->FindString("Status::statusAbbreviation",&m_statusAbbreviation);
	archive->FindBool("Status::userChoice",&m_userChoice);
	rgb_color *findColour = &m_statusColour;
	ssize_t size;
	archive->FindData("Status::statusColour",B_RGB_COLOR_TYPE,(const void**)&findColour, &size);
		
	BMessage msg;
	int32 i = 0; 
	while (archive->FindMessage("Status::statusIcon",i,&msg) == B_OK)		
	{
		BArchivable* obj = instantiate_object(&msg);
		if(obj)
			AddIcon(cast_as(obj,BBitmap));
		i++;	
	}	
}

/**	Destructor
 */
Status::~Status()
{	
	typedef map<int32, BBitmap*>::const_iterator II;
	for (II p = m_statusIcons.begin(); p != m_statusIcons.end(); ++p)
	{
		delete (*p).second;
	}
}

void Status::SetStatusName(BString name)
{
	m_statusName = name;
}

/**	Get the user readable name of this status.
	@return a human readable status name.
 */
BString Status::GetStatusName() 
{
	return m_statusName;
}

void Status::SetAbbreviation(BString abbreviation)
{
	m_statusAbbreviation = abbreviation;
}

/**	Gets the abbreviation for this status. This abbreviation is
	used by the protocol to set and identify the status.
	@return an abbreviation representing the status, used by the
	messenger protocol.
 */
BString Status::GetAbbreviation()
{
	return m_statusAbbreviation;
}

/**	Gets the icon representation of this status.
	@return a bitmap representing this status.
 */
BBitmap* Status::GetStatusIcon(int32 width)
{
	return m_statusIcons[width];
}

/**	
 */
void Status::AddIcon(BBitmap *statusIcon)
{
	int32 width = int32((statusIcon->Bounds()).Width());
	m_statusIcons[width] = statusIcon;
}

/**	Returns if this status represents an online status: true if
	it does, false otherwise.
	@return true if the status is one of the online statusses,
	false if it is the offline status.
 */
bool Status::IsOnline() const
{ 	
	return m_statusAbbreviation != Statusses::K_OFFLINE;
}


void Status::SetUserChoice(bool userChoice)
{
	m_userChoice = userChoice;
}

/**	Returns if this status is a status that can be chosen by the user.
	@return true if the status can be set by the user, false if not.
 */
bool Status::IsUserChoice() const
{
	return m_userChoice;
}

void Status::SetStatusColour(rgb_color colour)
{
	m_statusColour = colour;
}

rgb_color Status::StatusColour() const
{
	return m_statusColour; 
}

/** Instantiates a Status object from a message archive.
	@param archive, the archived Status object.
	@return a BArchivable object representing the instantiated object.
 */
BArchivable* Status::Instantiate(BMessage *archive)
{
	if (validate_instantiation(archive, "Status"))
      return new Status(archive);
    return NULL;
}

/**	Archives this object into a message.
	@param archive, the message object the Status object should be archived in.
	@param deep, if true the user bitmap is also saved into the message, if false
	only status name, abbreviation an userChoice are archived.
	@return B_OK if the archive operation was successfull.
 */
status_t Status::Archive(BMessage *archive, bool deep) const
{
	archive->AddString("class","Status");
	archive->AddString("Status::statusName",m_statusName);
	archive->AddString("Status::statusAbbreviation",m_statusAbbreviation);
	archive->AddBool("Status::userChoice",m_userChoice);
	archive->AddData("Status::statusColour",B_RGB_COLOR_TYPE,(void**)&m_statusColour,sizeof(rgb_color));
	if(deep)
	{
		
		typedef map<int32, BBitmap*>::const_iterator II;
		for (II p = m_statusIcons.begin(); p != m_statusIcons.end(); ++p)
		{
			BMessage child;
			BBitmap* statusIcon = (*p).second;
			statusIcon->Archive(&child);
			
			archive->AddMessage("Status::statusIcon",&child);
		}		
	}
	return B_OK;
}
