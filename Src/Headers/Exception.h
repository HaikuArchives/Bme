#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <app/Message.h>
#include <support/String.h>
#include <support/Archivable.h>
#include <exception>
#define K_ERROR_MSG_FIELD				"Exception::errorMessage"

using namespace std;

class Exception : public exception, BArchivable
{
	public:
		Exception();
		Exception(BString errorMessage);
		Exception(BMessage *archive);
		virtual				~Exception();
		
		virtual BString		ErrorMessage();
		virtual void		SetErrorMessage(BString errorMessage);
		
		virtual status_t 		Archive(BMessage* archive, bool deep = true) const;
		static BArchivable* 	Instantiate(BMessage* archive);
		
		virtual const char* what() const throw();
		
	private:
		BString				m_errorMessage;
};

#endif
