/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MSN_MESSAGE_H
#include "MsnMessage.h"
#endif

MsnMessage::MsnMessage(BString contentType)				
				//:	BArchivable()
{
}

/*MsnMessage::MsnMessage(BString payloadData)
{
	//parse message
}*/

MsnMessage::MsnMessage(BString header, BString body)	
				//:	BArchivable()			
{
}

MsnMessage::~MsnMessage()
{
}

void MsnMessage::AddLineToHeader(BString line)
{
	m_header.push_back(line);
}

BString	MsnMessage::GetLineFromHeader(BString key)
{
}

void MsnMessage::AddLineToBody(BString line)
{
	m_body.push_back(line);
}

BString MsnMessage::GetLineFromBody(BString )
{
}

BString	MsnMessage::GetMessage()
{
	typedef vector<BString>::const_iterator CI;
	BString message;
	for (CI p = m_header.begin(); p != m_header.end(); ++p)
	{
		message << *p << "\r\n";
	}
	//add extra \r\n separator between header and body
	message << "\r\n";
	for (CI p = m_body.begin(); p != m_body.end(); ++p)
	{
		message << *p << "\r\n";
	}
	
	return message;
}
