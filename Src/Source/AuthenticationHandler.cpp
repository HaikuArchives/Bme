/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef AUTHENTICATION_HANDLER_H
#include "AuthenticationHandler.h"
#endif

#include <app/Looper.h>
#include <app/MessageQueue.h>
#include <app/Messenger.h>
#include <interface/Alert.h>
//============================
#include <net/NetEndpoint.h>
#include <openssl/ssl.h>
#include <iostream>
#include <stdlib.h>
#include "constants.h"
#include "ProtocolConstants.h"
#include "ServerLooper.h"
#include "Preferences.h"
#include "Common.h"
#include "PingHandler.h"
#include "MessageFields.h"

AuthenticationHandler::AuthenticationHandler()
						:	ServerHandler(),
							m_dispatch(true)
{
	//initialize library
	SSL_library_init(); 
	AddFilter(new AuthenticationFilter());
}

AuthenticationHandler::~AuthenticationHandler()
{
}

void AuthenticationHandler::MessageReceived(BMessage *message)
{
	switch(message->what)
	{	
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);
			if (command == NotificationMessages::K_PROTOCOLS_SUPPORTED)
			{
				//received an answer to our VER message
				//check if our protocol is supported!
				BString remainingMsg;
				int32 rIndex = 0;
				bool protocolSupported = false;
				while (message->FindString(K_REMAINING_MSG,rIndex,&remainingMsg) == B_OK)
				{
					//is our protocol supported?
					if (remainingMsg == ProtocolConstants::K_PROTOCOL_VERSION)
					{
						
						protocolSupported = true;
						break;
					}					
					rIndex++;							
				}
				
				if (protocolSupported)
				{
					//construct next message (CVR)
					BMessage *cvrMessage = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
					
					cvrMessage->AddString(K_COMMAND,NotificationMessages::K_CLIENT_INFORMATION);
					
					cvrMessage->AddString(K_REMAINING_MSG,"0x0409 win");
					cvrMessage->AddString(K_REMAINING_MSG,ProtocolConstants::K_WINDOWS_XP_SP2_VERSION);
					cvrMessage->AddString(K_REMAINING_MSG,"i386 MSNMSGR"); 
					cvrMessage->AddString(K_REMAINING_MSG,ProtocolConstants::K_MSN_VERSION);
					cvrMessage->AddString(K_REMAINING_MSG, "MSMSGS");
					cvrMessage->AddString(K_REMAINING_MSG,m_loginName);
					
					SendCommandMessageTrID(cvrMessage);						
				}
				else
				{
					//error, protocol used by Bme not supported
					BAlert *alert = new BAlert("Error","Bme's using an old protocol not supported by the current MSN servers","OK",NULL,NULL,B_WIDTH_AS_USUAL,B_WARNING_ALERT);
					alert->Go();
				}
			}
			else if (command == NotificationMessages::K_CLIENT_INFORMATION)
			{
				//received an answer to our CVR message							
				BMessage *usrMessage = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
				usrMessage->AddString(K_COMMAND,NotificationMessages::K_NS_USR_COMMAND);
				
				//send initial USR
				usrMessage->AddString(K_REMAINING_MSG,"TWN");				
				usrMessage->AddString(K_REMAINING_MSG,"I");
				usrMessage->AddString(K_REMAINING_MSG,m_loginName);
				
				SendCommandMessageTrID(usrMessage);				
			}
			else if (command == NotificationMessages::K_NS_DOWN)
			{
				//received XFR command 
				BString nsString;
				if(message->FindString(K_REMAINING_MSG,0,&nsString) == B_OK)
				{
					if (nsString == "NS")
					{
						//reroute to NS server
						BString nsServerAddress;
						if(message->FindString(K_REMAINING_MSG,1,&nsServerAddress) == B_OK)
						{
							//separate Notification Server address and port
							int32 colonIndex = nsServerAddress.FindFirst(":");
							int nsPort;							
							if (colonIndex != B_ERROR)
							{
								BString nsPortString;
								nsServerAddress.CopyInto(nsPortString, colonIndex + 1,nsServerAddress.CountChars() - colonIndex);
								nsPort = atoi(nsPortString.String());
								nsServerAddress.Remove(colonIndex,nsServerAddress.CountChars() - colonIndex);	
								//connect to Notification Server
								if (startLogin(m_loginName, m_password,nsServerAddress.String(),nsPort))
								{
								}
								else
								{
									//error
								}
							}
							
						}
						else
						{
							//error! can't find Notification Server address
						}
					}
					else if (nsString == "SB")
					{
						HandleSwitchboard(message);
					}
				}
				//connect to new server suggested
			}
			else if (command == NotificationMessages::K_NS_USR_COMMAND)
			{
				//one of the USR commands, check which one
				BString usrFirstString;
				if(message->FindString(K_REMAINING_MSG,0,&usrFirstString) == B_OK)
				{
					if (usrFirstString == "TWN")
					{
						BString challenge;
						if(message->FindString(K_REMAINING_MSG,2,&challenge) == B_OK)
						{							
							//still authenticating 
							BString ticket = TweenerAuthenticate(challenge);
							//respond to command with proper ticket						
							BMessage *usrReply = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
							usrReply->AddString(K_COMMAND,NotificationMessages::K_NS_USR_COMMAND);
							
							usrReply->AddString(K_REMAINING_MSG,"TWN");
							usrReply->AddString(K_REMAINING_MSG,"S");
							usrReply->AddString(K_REMAINING_MSG,ticket);							
							
							SendCommandMessageTrID(usrReply);
						}						
					}
					else if (usrFirstString == "OK")
					{
						//authentication completed! Change to contact view and set displayname
						BString displayName;
						if(message->FindString(K_REMAINING_MSG,2,&displayName) == B_OK)
						{							
							//convert displayname from URL-encoded string to normal Be String
							displayName = Common::decodeURL(displayName);//???
							//set initial presence
							BMessage *initialPresenceMsg = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
							initialPresenceMsg->AddString(K_COMMAND,NotificationMessages::K_CHANGE_STATUS);							
							initialPresenceMsg->AddString(K_REMAINING_MSG,m_initialStatus);
							//find client ID 
							BString clientID;
							int64 id = 0;
							/*prefsLock.Lock();
							preferences.FindInt64(K_CLIENT_ID,&id);
							prefsLock.Unlock();	*/						
							id = ClientIdentification::K_MSNC4;
							cout << "id= " << id << endl;
							
							clientID << id;
							initialPresenceMsg->AddString(K_REMAINING_MSG,clientID);
							//add msn object!
							
							SendCommandMessageTrID(initialPresenceMsg);
													
							//add PingHandler to NS Looper
							if (Looper()->Lock())
							{
								Looper()->AddHandler(new PingHandler());
								Looper()->Unlock();								
							}
							
							//synchronise lists, need some info here???
							SendCommandMessage(new BMessage(ProtocolConstants::K_SYNCHRONISE_MSG));													
							//user logged in, show in interface!
							BMessage *loginMsg = new BMessage(InterfaceMessages::K_USER_SIGNED_IN_MSG);
							loginMsg->AddString(K_INITIAL_STATUS, m_initialStatus);//??
							SendCommandMessage(loginMsg);							
						}
					}
				}
			}			
		}
		break;
		case InterfaceMessages::K_USER_SIGN_IN_MSG:
		{				
			message->FindString("loginname",&m_loginName);
			message->FindString("password",&m_password);
			message->FindString(K_NEW_STATUS,&m_initialStatus);
			if (startLogin(m_loginName, m_password, (ProtocolConstants::K_DS_ADDRESS).String(),ProtocolConstants::K_DS_PORT))
			{	
				//login successful
				
			}
			else
			{
				//error!
				
			}						
		}
		break;
		case InterfaceMessages::K_USER_START_CONV_MSG:
		{			
			//trId is saved, so we can look up the contact belonging to the conversation later.
			int32 currentTrId = 0;
			//start a conversation with a contact
			BMessage *startConvMessage = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);
			startConvMessage->AddString(K_COMMAND,NotificationMessages::K_REQUEST_SWITCHBOARD_SESSION);
			startConvMessage->AddString(K_REMAINING_MSG, "SB");
			SendCommandMessageTrID(startConvMessage, &currentTrId);
			//get contact info from message and save it for later use!
			Contact *contact;
			if (message->FindPointer(K_CONTACT,(void **)&contact) == B_OK)
			{
				m_sbUserRequests[currentTrId] = contact;
			}
			else
			{
				//error!
			}						
		}
		break;
		default:
			BHandler::MessageReceived(message);
		break;
	}
}

bool AuthenticationHandler::startLogin(BString loginName, BString password, const char* address, int port)
{
	bool successful = false;
	//connect NSLooper to Server
	ServerLooper *owner = dynamic_cast<ServerLooper*>(Looper());
	owner->init();	
	if (owner->Connect(address, port) == B_OK)
	{
		successful = true;
		//send first command message(VER)
		BMessage *verMsg = new BMessage(ProtocolConstants::K_ADD_COMMAND_MESSAGE);	
		
		verMsg->AddString(K_COMMAND,NotificationMessages::K_PROTOCOLS_SUPPORTED);		
		verMsg->AddString(K_REMAINING_MSG,ProtocolConstants::K_PROTOCOL_VERSION); 
		verMsg->AddString(K_REMAINING_MSG,"CVR0");		
	
		SendCommandMessageTrID(verMsg);		
	}
	
	return successful;
}

//improve error handling!
BString	AuthenticationHandler::TweenerAuthenticate(BString challenge)
{
	BString ticket;	
	const char *passPortNexusAddress = "nexus.passport.com";
		
	HTTPFormatter *send = new HTTPFormatter(passPortNexusAddress,"/rdr/pprdr.asp");
//cout << send->Flatten() << endl;
	HTTPFormatter *recv = NULL;
	
	if (SSLSend(passPortNexusAddress, send, &recv) == B_OK)
	{
//cout << "ssl connection" << endl;
//cout << recv->Flatten() << endl;
		//check if reply is 200 OK
		if (recv->Status() == 200)
		{
			BString passportUrl = recv->HeaderContents("PassportURLs");		
			BString loginKey = "DALogin=";
			int32 loginIndex = passportUrl.FindFirst(loginKey);
			//find login url in received header
			if (loginIndex != B_ERROR)
			{
				loginIndex += loginKey.Length();
				int32 commaIndex = passportUrl.FindFirst(",",loginIndex);
				if (commaIndex != B_ERROR)
				{
					BString loginUrl;
					passportUrl.CopyInto(loginUrl, loginIndex, commaIndex - loginIndex);				
					delete recv;
					//find host and document parts of login url					
					int16 status = 0;
					do
					{
						int32 slashIndex = loginUrl.FindFirst("/");
						if (slashIndex != B_ERROR)
						{
							BString httpsString = "https://";
							int32 httpsIndex = loginUrl.FindFirst(httpsString);
							int32 startIndex = 0;
							if (httpsIndex != B_ERROR)
								startIndex = httpsIndex + httpsString.CountChars();
								
							BString hostString;
							loginUrl.CopyInto(hostString, startIndex, slashIndex);
							
							BString documentString;
							loginUrl.CopyInto(documentString, slashIndex, loginUrl.CountChars() - slashIndex);
									
							//construct a new request to the passport nexus with necessary information
							BString authorisationHeader = "Passport1.4 OrgVerb=GET,OrgURL=http%3A%2F%2Fmessenger%2Emsn%2Ecom,sign-in=";
							authorisationHeader << Common::encodeURL(m_loginName);
							authorisationHeader << ",pwd=" << Common::encodeURL(m_password) << ",";
							authorisationHeader << challenge;
							//set new host for request
							send->Host(hostString.String());
							//set new document for request
							send->Document(documentString.String());
							//add authorisation info to header
							send->AddHeader("Authorization", authorisationHeader.String());							
							if (SSLSend(hostString.String(), send, &recv) == B_OK)
							{
								status = recv->Status();
								//three possible answers from the server are possible
								if (status == 200)
								{
									//response 200 OK, a header with the requested info has been sent back!
									//get ticket
									BString authHeader = recv->HeaderContents("Authentication-Info");
									//when authentication key field has not been found check for WWW-Authenticate!
									if (authHeader == "")
										authHeader = recv->HeaderContents("WWW-Authenticate");									
									int32 ticketIndex = authHeader.FindFirst("'");
									if (ticketIndex != B_ERROR)
									{							
										ticketIndex++;
										int32 ticketEndIndex = authHeader.FindFirst("'",ticketIndex);
										authHeader.CopyInto(ticket, ticketIndex, ticketEndIndex - ticketIndex);							
									}
									else
									{
										cout << "ticket not found" << endl;
									}
								}
								else if (status == 302)
								{
									//client has to be redirected!																		
									loginUrl = recv->HeaderContents("Location");									
								}
								else if (status == 401)
								{
									//unauthorised, failure! 
									BString authHeader = recv->HeaderContents("Authentication-Info");
									//when authentication key field has not been found check for WWW-Authenticate!
									if (authHeader == "")
										authHeader = recv->HeaderContents("WWW-Authenticate");
									//Get error message!
									BString errTxtKey = "cbtxt=";
									int32 errTxtIndex = authHeader.FindFirst(errTxtKey);
//cout << recv->Flatten() << endl;
									if (errTxtIndex != B_ERROR)
									{
										errTxtIndex += errTxtKey.CountChars();
										BString errorMsg;
										authHeader.CopyInto(errorMsg, errTxtIndex, authHeader.CountChars() - errTxtIndex);
										errorMsg = Common::decodeURL(errorMsg);
										//display alert with error!
										BAlert *alert = new BAlert("A critical error occurred!",errorMsg.String(),"Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
										alert->Go();																														
									}									
								}
								else
								{
									cout << "error!" << endl;
								}
								//got the information we need
								delete recv;
							}							
						}
					}
					while (status == 302);
				}
				else
				{
					cout << "comma not found" << endl;
				}			
			}
			else
			{
				cout << "login url not found" << endl;
			}
		}
		else
		{
			cout << "response isn't 200 OK " << endl;
		}
	}
	else
	{
		cout << "login error" << endl;
	}
	delete send;	
	
	return ticket;
}

//inspired by IMkit code
status_t AuthenticationHandler::SSLSend(const char *host, HTTPFormatter *send, HTTPFormatter **recv) 
{	
	status_t error = B_OK;
	//initialise ssl lib
	SSLeay_add_ssl_algorithms();
	// 	
 	SSL_METHOD *meth = SSLv2_client_method();	
 	//readable error messages 
	SSL_load_error_strings(); 
	SSL_CTX *sslCtx = SSL_CTX_new(meth);
	if (sslCtx != NULL)
	{		
		BNetEndpoint tcpConnection;
		int port = 443;
		if ((tcpConnection.InitCheck() == B_OK) && (tcpConnection.Connect(host, port) == B_OK))
		{			
			SSL *ssl = SSL_new(sslCtx);		
			if (ssl)
			{	
				int fdError = SSL_set_fd(ssl, tcpConnection.Socket());
				if (fdError == 1)
				{					
					//initial handshake with server
					int connectError = SSL_connect(ssl);
					if (connectError == 1)
					{						
						int sentBytes = SSL_write(ssl, send->Flatten(), send->Length());
						if (sentBytes > 0 && sentBytes == send->Length())
						{
							int received = 0;
							int numRead = 0;
							int bufSize = 1024*1024;
							char buffer [bufSize];
							do 
							{
//cout << "here1" << endl;
								numRead = SSL_read(ssl, &buffer[received], sizeof(buffer) - 1 - received);														
								if (numRead > 0)
									received += numRead;
//cout << "numread= " << numRead << endl;
							}
							while (received < 571);//numRead > 0 && received < bufSize);
							buffer[received] = '\0';
							*recv = new HTTPFormatter(buffer, received);	
						}
						//close connection
						
					}
					else
					{
						//error
						cout << "ssl connect error" << endl;
						error = B_ERROR;
					}
				}
				//close(tcp_socket);
				tcpConnection.Close();
				//close ssl
				SSL_shutdown(ssl);
			}
			else
			{
				//error
				cout << "ssl error" << endl;
				error = B_ERROR;
			}
			//free allocated memory
			SSL_free (ssl);
			SSL_CTX_free (sslCtx);
		}
		else
		{
			cout << "failed to connect to server" << endl;
			error = B_ERROR;
		}		
	}
	else
	{
		//error
		cout << "ssl ctx error" << endl;
		error = B_ERROR;
	}
	
	return error;
}

void AuthenticationHandler::HandleSwitchboard(BMessage *switchboardMessage)
{	
	BMessage *sbMessage = new BMessage(InterfaceMessages::K_CONNECT_SB_MSG);
	
	BString command;
	if (switchboardMessage->FindString(K_COMMAND,&command) == B_OK)
	{		
		if (command == NotificationMessages::K_REQUEST_SWITCHBOARD_SESSION)
		{
			//user requested the SB session
			//connect to SB server
			BString sbServerAddress;
			if(switchboardMessage->FindString(K_REMAINING_MSG,1,&sbServerAddress) == B_OK)
			{
				//separate Notification Server address and port
				BString sbUrl;
				int32 sbPort;											
				if (Common::SplitURL(sbServerAddress, &sbUrl, &sbPort) == B_OK)
				{					
					//check authentication type
					BString authType;
					if (switchboardMessage->FindString(K_REMAINING_MSG,2,&authType) == B_OK)
					{
						if (authType == "CKI")
						{
							BString authenticationString;
							if (switchboardMessage->FindString(K_REMAINING_MSG,3,&authenticationString) == B_OK)
							{
								int32 trId;
								if (switchboardMessage->FindInt32(K_TR_ID, &trId) == B_OK)
								{
									//connecting details for Switchboard Server								
									sbMessage->AddString(K_SB_ADDRESS,sbUrl);
									sbMessage->AddInt32(K_SB_PORT,sbPort);
									sbMessage->AddString(K_AUTHENTICATION_STRING,authenticationString);								
									//add contact to start the conversation with
									Contact *contact = m_sbUserRequests[trId];
									if (contact)
									{
										sbMessage->AddPointer(K_CONTACT, contact);
									}
									else
									{
										//error
									}
								}
							}
							else
							{
							}
						}
						else
						{
						}
					}
				}
				
			}
			else
			{
				//error! can't find Switchboard Server address
			}
		}
		else if (command == NotificationMessages::K_INVITED_FOR_SWITCHBOARD)
		{
			//contact started an SB session		
			BString sessionId;
			if (switchboardMessage->FindString(K_REMAINING_MSG, 0, &sessionId) == B_OK)
			{
				BString sbServerAddress;
				if (switchboardMessage->FindString(K_REMAINING_MSG, 1, &sbServerAddress) == B_OK)
				{
					BString sbUrl;
					int32 sbPort;			
					//separate Notification Server address and port
					if (Common::SplitURL(sbServerAddress, &sbUrl, &sbPort) == B_OK)
					{	
						//check authentication type
						BString authType;
						if (switchboardMessage->FindString(K_REMAINING_MSG,2,&authType) == B_OK)
						{							
							if (authType == "CKI")
							{
								BString authenticationString,contactPassport, friendlyName;
								if (
									(switchboardMessage->FindString(K_REMAINING_MSG,3,&authenticationString) == B_OK) &&
									(switchboardMessage->FindString(K_REMAINING_MSG,4,&contactPassport) == B_OK) &&
									(switchboardMessage->FindString(K_REMAINING_MSG,5,&friendlyName) == B_OK)
									)
								{	
									//connecting details for Switchboard Server	
									sbMessage->AddString(K_SB_SESSION_ID, sessionId);
									sbMessage->AddString(K_SB_ADDRESS,sbUrl);
									sbMessage->AddInt32(K_SB_PORT,sbPort);
									sbMessage->AddString(K_AUTHENTICATION_STRING,authenticationString);
									sbMessage->AddString(K_CONTACT_PASSPORT,contactPassport);
									sbMessage->AddString(K_CONTACT_FRIENDLY_NAME, Common::decodeURL(friendlyName));
								}
							}
						}
					}
				}
			}
		}
	}
	SendCommandMessage(sbMessage);
}

//==========================AuthenticationFilter================================
AuthenticationFilter::AuthenticationFilter()
						:	BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE)
{
}

AuthenticationFilter::~AuthenticationFilter()
{
}

filter_result AuthenticationFilter::Filter(BMessage *message, BHandler **target)
{
	filter_result result;
	switch (message->what)
	{
		case ProtocolConstants::K_COMMAND_MSG:
		{
			BString command;
			message->FindString(K_COMMAND,&command);
			if (command == NotificationMessages::K_PROTOCOLS_SUPPORTED ||
				command == NotificationMessages::K_CLIENT_INFORMATION ||
				command == NotificationMessages::K_NS_DOWN ||
				command == NotificationMessages::K_NS_USR_COMMAND
				)
			{
				result = B_DISPATCH_MESSAGE;
			}
		}
		break;
		//AuthenticationHandler only handles the following messages:		
		case InterfaceMessages::K_USER_SIGN_IN_MSG:	
		case InterfaceMessages::K_USER_START_CONV_MSG:	
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
