/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MSN_MESSAGE_H
#define MSN_MESSAGE_H

#include <app/Message.h>
#include <support/String.h>
#include <support/Archivable.h>
#include <vector.h>

namespace ContentTypes
{
	const BString 
			//Notification Server
			K_PROFILE_MSG =						"text/x-msmsgsprofile",
			K_INITIAL_EMAIL_MSG =				"text/x-msmsgsinitialemailnotification",
			K_NEW_EMAIL_MSG =					"text/x-msmsgsemailnotification",
			K_OTHER_MAILBOX_ACTIVITY_MSG =		"text/x-msmsgsactivemailnotification",
			K_SYSTEM_MSG =						"application/x-msmsgssystemmessage", 						
			//Switchboard Server
			K_PLAIN_TEXT_MSG =					"text/plain",
			K_TYPING_MSG =						"text/x-msmsgscontrol",
			K_APPLICATION_INVITATION_MSG =		"text/x-msmsgsinvite",
			K_USER_EMOTICON_MSG =				"text/x-mms-emoticon",
			K_P2P_MSG =							"application/x-msnmsgrp2p"
			;
};

class MsnMessage // : public BArchivable
{
	public:
		MsnMessage(BString contentType);
		//MsnMessage(BString payloadData);
		MsnMessage(BString header, BString body);
		virtual				~MsnMessage();
		
		void				AddLineToHeader(BString line);
		BString				GetLineFromHeader(BString key);
		
		void				AddLineToBody(BString line);
		BString				GetLineFromBody(BString );
		
		BString				GetMessage();
		
	private:
		vector<BString>		m_header,//map!!!
							m_body;
};

#endif
