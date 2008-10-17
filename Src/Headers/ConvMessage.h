#ifndef CONV_MESSAGE_H
#define CONV_MESSAGE_H

#include <support/SupportDefs.h>

class BString;
class Contact;

class ConvMessage // : public MsnMessage
{
	public:
		ConvMessage(Contact* sender);
		virtual				~ConvMessage();	

		Contact*			Sender();
		void				SetSender(Contact* sender);				

		bigtime_t			Timestamp();
		void				SetTimeStamp(bigtime_t timestamp);		
		
	private:
		Contact*			m_sender;
		bigtime_t			m_timestamp;
};

#endif
