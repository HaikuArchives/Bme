#ifndef EXCEPTION_H
#include "Exception.h"
#endif

Exception::Exception()
			:	BArchivable()
{
}

Exception::Exception(BString errorMessage)
			:	BArchivable(),
				m_errorMessage(errorMessage)
{
}

Exception::Exception(BMessage *archive)
			:	BArchivable(archive)
{
	archive->FindString(K_ERROR_MSG_FIELD, &m_errorMessage);	
}

Exception::~Exception()
{
}

const char* Exception::what() const throw()
{
    return "My exception happened";
}
  		
BString Exception::ErrorMessage()
{
	return m_errorMessage;
}

void Exception::SetErrorMessage(BString errorMessage)
{
	m_errorMessage = errorMessage;
}

status_t Exception::Archive(BMessage* archive, bool deep) const
{
	archive->AddString("class", "Exception");
	archive->AddString(K_ERROR_MSG_FIELD, m_errorMessage);	
	return B_OK;
}

BArchivable* Exception::Instantiate(BMessage* archive)
{
   if (!validate_instantiation(archive, "Exception"))
      return NULL;
   return new Exception(archive);
}
