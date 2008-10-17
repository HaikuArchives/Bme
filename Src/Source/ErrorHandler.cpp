/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef ERROR_HANDLER_H
#include "ErrorHandler.h"
#endif

#include <be/interface/Alert.h>
#include <be/support/String.h>
#include <stdlib.h>
#include "ProtocolConstants.h"
#include "ProtocolErrors.h"
#include "constants.h"
#include "MessageFields.h"

ErrorHandler::ErrorHandler()
				:	ServerHandler()
{
	AddFilter(new ErrorFilter());
}

ErrorHandler::~ErrorHandler()
{
}

void ErrorHandler::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case ProtocolConstants::K_ERROR_MESSAGE:
		{
			BString errorString;
			message->FindString(K_ERROR_STRING, &errorString);
			
			bool critical;
			message->FindBool(K_CRITICAL_BOOL, &critical);
			
			BString title;
			if (critical)
			{	
				title = "A critical error occurred!";
				//sign user out
				BMessage *errorMessage = new BMessage(InterfaceMessages::K_USER_DISCONNECTED_MSG);
				errorMessage->AddString(K_ERROR_STRING,errorString);
				SendCommandMessage(errorMessage);
			}
			else
			{
				title =	"An error occurred!";
			}				
			//show error alert			
			BAlert *alert = new BAlert(title.String(),errorString.String(),"Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
			alert->Go();			
		}
		break;
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);
			for (uint32 i = 0; i < K_NUMBER_ERRORS; i++)
			{
				error err = K_MSN_ERROR_MSGS[i];
				uint32 errNr = atoi(command.String());
				if (errNr == err.m_errorNumber)
				{
					//construct error message
					BString errorString = "Error ";
					errorString << errNr << ": ";
					errorString << err.m_errorMessage;
					//show error alert
					BAlert *alert = new BAlert("A critical error occurred!",errorString.String(),"Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
					alert->Go();
					//sign user out
					BMessage *errorMessage = new BMessage(InterfaceMessages::K_USER_DISCONNECTED_MSG);
					errorMessage->AddString(K_ERROR_STRING,errorString);
					SendCommandMessage(errorMessage);
					
					return;
				}
			}
		}
		break;	
		default:
			BHandler::MessageReceived(message);
		break;
	}
}

//==========================ErrorFilter================================
ErrorFilter::ErrorFilter()
						:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE)
{
}

ErrorFilter::~ErrorFilter()
{
}

filter_result ErrorFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result;
	switch (message->what)
	{
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);

			result = B_SKIP_MESSAGE;
			//find out if it is an error message!
			for (uint32 i = 0; i < K_NUMBER_ERRORS; i++)
			{
				error err = K_MSN_ERROR_MSGS[i];
				uint32 errNr = atoi(command.String());
				if (errNr == err.m_errorNumber)
				{
					result = B_DISPATCH_MESSAGE;
					break;
				}
			}			
		}
		break;		
		case ProtocolConstants::K_ERROR_MESSAGE:
		{
			result = B_DISPATCH_MESSAGE;
		}
		break;
		default:
			result = B_SKIP_MESSAGE;
		break;
	}
	return result;
}
