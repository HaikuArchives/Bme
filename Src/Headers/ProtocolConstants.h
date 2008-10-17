/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef PROTOCOL_CONSTANTS_H
#define PROTOCOL_CONSTANTS_H

#include <support/String.h>

namespace Statusses
{
	const BString
			K_AVAILABLE			= "NLN",
			K_BUSY				= "BSY",
			K_BE_RIGHT_BACK		= "BRB",
			K_AWAY				= "AWY",
			K_ON_THE_PHONE		= "PHN",
			K_OUT_TO_LUNCH		= "LUN",
			K_HIDDEN			= "HDN",
			
			K_IDLE				= "IDL",
			K_OFFLINE			= "FLN"
			;
};

namespace ClientIdentification
{
	const int32
			K_WINDOWS_MOBILE				= 0x01,
			K_RESET_CAPABILITIES			= 0x02,
			K_INK_GIF						= 0x04,
			K_INK_ISF						= 0x08,
			K_HAS_WEBCAM					= 0x10,
			K_MULTI_PACKET_MESSAGING		= 0x20,
			K_RUNNING_MSN_MOBILE			= 0x40,
			K_RUNNING_MSN_DIRECT			= 0x80,
			K_RUNNING_WEB_MESSENGER			= 0x100,
			K_SUPPORTS_DIRECT_IM			= 0x4000,
			K_SUPPORTS_WINKS_RECEIVING		= 0x8000,
			K_MSNC1							= 0x10000000,
			K_MSNC2							= 0x20000000,
			K_MSNC3							= 0x30000000,
			K_MSNC4							= 0x40000000,
			K_MSNC5							= 0x50000000
			;
			
};

namespace ContactListTypes
{
	const int32
			K_FORWARD_LIST =					1,
			K_ALLOW_LIST =						2,
			K_BLOCK_LIST =						4,
			K_REVERSE_LIST =					8,
			K_PENDING_LIST =					16
			;
};

namespace SettingTypes
{
	const BString
			K_MY_FRIENDLY_NAME =				"MFN",
			K_HOME_PHONE_NUMBER	=				"PHH",
			K_WORK_PHONE_NUMBER =				"PHW",
			K_MOBILE_PHONE_NUMBER =				"PHM",
			K_HAS_MSN_SPACE_BLOG =				"HSB", //introduced in MSPN11
			K_CAN_PAGE_MOBILE =					"MOB",
			K_CAN_DIRECT_PAGE_MOBILE =			"WWE",
			K_HAS_MOBILE_DEVICE_ENABLED =		"MBE"
			;
};

namespace URLTypes
{
	const BString
			K_HOTMAIL_INBOX =			"INBOX",
			K_MSN_HOME_URL =			"FOLDERS",
			K_COMPOSE_EMAIL =			"COMPOSE",
			K_MOBILE_SETTINGS =			"CHGMOB",
			K_MSN_PROFILE =				"PROFILE",
			K_ACCOUNT_INFO =			"PERSON",
			K_CHAT_ROOMS =				"CHAT"
			;	
};

namespace AckTypes
{
	const BString
			K_NO_ACK_TYPE =				"U",
			K_NEGATIVE_ACK_TYPE =		"N",
			K_ALL_ACK_TYPE =			"A"
			;
};

namespace PersonalMessage
{
	const BString
			K_DOCUMENT_TAG =			"Data",
			K_PERSONAL_MESSAGE_TAG =	"PSM",
			K_CURRENT_MEDIA_TAG =		"CurrentMedia"
			;
};

namespace ProtocolConstants
{
	const uint32
				K_ADD_COMMAND_MESSAGE =				'ADcm',				
				K_ERROR_MESSAGE =					'ERms',
				K_MSN_OBJECT_MSG =					'MSob',
				K_COMMAND_MSG =						'COms',
				K_SYNCHRONISE_MSG =					'SYNs',
				K_MAX_TR_ID =						4294967295 //2^32-1
				;				
	
	const BString
				K_MSN_VERSION =						"7.0.0813",
				K_PROTOCOL_VERSION =				"MSNP11",
				K_DS_ADDRESS =						"messenger.hotmail.com",
				K_MSN7_PRODUCT_KEY =				"CFHUR$52U_{VIX5T",
				K_MSN7_PRODUCT_ID =					"PROD0101{0RM?UBW",
				K_WINDOWS_XP_SP2_VERSION =			"5.1"
				;
				
	const int
				K_DS_PORT =							1863
				;
};

namespace NotificationMessages
{
	const BString
			//authentication
			K_PROTOCOLS_SUPPORTED			= "VER",
			K_CLIENT_INFORMATION			= "CVR",
			K_NS_USR_COMMAND				= "USR",
			K_NS_DOWN						= "XFR",
			K_NS_SHUTTING_DOWN				= "OUT SSD",
			K_NS_LOG_OFF					= "OUT",
			//presence information
			K_PRINCIPAL_STATUS				= "NLN",
			K_CHANGE_STATUS					= "CHG",
			K_CHANGE_DISPLAY_NAME			= "PRP",
			K_CHANGE_PERSONAL_MSG			= "UUX", 
			K_PRINCIPAL_INITIAL_PRESENCE	= "ILN",			
			K_PRINCIPAL_CLOSES_PRESENCE		= "FLN",
			//Pings/Challenges
			K_CLIENT_PING 					= "PNG",
			K_SERVER_PONG 					= "QNG",
			
			K_SERVER_PING 					= "CHL",
			K_CLIENT_PONG 					= "QRY",
			//getting details
			K_SYNCHRONISE_LIST				= "SYN",
			K_ADD_BEHAVIOUR_MSG				= "GTC",
			K_DEFAULT_LIST_MSG				= "BLP",
			K_PHONE_NUMBERS					= "PRP",
			K_GROUP_LIST_MSG				= "LSG",
			K_CONTACT_LIST_MSG				= "LST",
			K_CONTACT_PERSONAL_MSG			= "UBX",
			//setting details
			K_ADD_PRINCIPAL					= "ADC",
			K_REMOVE_PRINCIPAL				= "REM",
			K_RENAME_PRINCIPAL				= "REA",
			//"BPR"
			
			K_ADD_GROUP						= "ADG",
			K_REMOVE_GROUP					= "RMG",
			K_RENAME_GROUP					= "REG",
			//messages
			K_NS_PAYLOAD_MSG				= "MSG",
			//miscellaneous
			K_URL_MSG						= "URL",
			K_EMAIL_INVITATION				= "SDC",
			K_SEND_PAGE_MSG					= "PAG",
			K_RECEIVE_PAGE_MSG				= "IPG",
			K_NOTICE_MSG					= "NOT",
			K_MOBILE_CREDITS_MSG			= "SBS",
			//switchboard session
			K_REQUEST_SWITCHBOARD_SESSION	= "XFR",
			K_INVITED_FOR_SWITCHBOARD		= "RNG"
			;
};

namespace SwitchboardMessages
{
	const BString
			K_SB_USR_COMMAND				= "USR",
			K_OPEN_SB_SESSION				= "ANS",
			K_PRINCIPALS_IN_SESSION			= "IRO",
			
			K_INVITE_PRINCIPAL				= "CAL",
			K_PRINCIPAL_JOINED				= "JOI",
			K_PRINCIPAL_LEFT				= "BYE",
			K_LEAVE_SB_SESSION				= "OUT",
			
			K_SWITCHBOARD_MESSAGE			= "MSG"
			;
};

#endif
