#ifndef PROTOCOL_MESSAGE_H
#define PROTOCOL_MESSAGE_H

#include <app/Message.h>
#include <support/String.h>
#include <support/Archivable.h>

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

class ProtocolMessage
{
	public:
		ProtocolMessage(const BString& type);	
		virtual				~ProtocolMessage();
	
		void				SetCommandType(const BString& type);
		BString				CommandType();
		
		bool				HasTrId();
		void				SetTrId(uint32 trId);
		uint32				TrId();		
		
		bool				HasPayload();
		void				SetPayload(const BString& payload); 
		BString				Payload();
		void				RemovePayload();
	
	private:
		BString				m_commandType,
							m_payload
							;
		bool				m_hasTrId,
							m_hasPayload
							;
		uint32				m_trId,
							m_payloadSize;				
};

#endif
