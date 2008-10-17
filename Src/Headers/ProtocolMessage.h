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

class ProtocolMessage : public BArchivable
{
	public:
		ProtocolMessage(const BString& type);
		ProtocolMessage(BMessage *archive);
		virtual				~ProtocolMessage();

		//archivable methods
		static BArchivable*	Instantiate(BMessage *archive);
		status_t 			Archive(BMessage *archive, bool deep = true) const;
		
		void				SetType(const BString& type);
		BString				Type();		
		
		bool				HasPayload();
		void				SetPayload(const BString& payload); 
		void				RemovePayload();
		BString				GetPayload();
	
	private:
		BString				m_msgType,
							m_payload
							;
		bool				m_hasPayload;
		int32				m_trId,
							m_payloadSize;		
		
};

#endif
