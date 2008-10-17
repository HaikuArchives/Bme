/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/ 
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <support/String.h>
#include <interface/GraphicsDefs.h>

#define K_ICON_PATH				"Graphics/Icons"
#define K_STANDARD_ICON_PATH	K_ICON_PATH "/Standard"
#define K_BACKGROUNDS_PATH		"Graphics/Backgrounds"
#define K_BME_SETTINGS_PATH		"Bme/"
#define K_CACHE_PATH			"Cache/"	

//constants for preferences msg
#define K_MAIN_WINDOW_RECT		"windowRect"
#define K_CLIENT_ID				"clientID"

static const unsigned char K_CURSOR_HYPERLINK[] = {16,1,2,2, 
  0,0,0,0,56,0,36,0,36,0,19,224,18,92,9,42, 
  8,1,60,33,76,113,66,113,48,249,12,249,2,0,1,0, 
  0,0,0,0,56,0,60,0,60,0,31,224,31,252,15,254, 
  15,255,63,255,127,255,127,255,63,255,15,255,3,254,1,248 
 }; 

namespace AppConstants
{
	const BString 
			K_ABOUT_TEXT =				"By Tim de Jong\n"
										"Icons by Rémi Grumeau. \n\n" 
										"Many thanks to Héctor D. Guajardo for releasing his BeMSN code and " 
										"for the work he has done for previous version of Bme\n\n"
										"I also want to thank Simon Taylor for his GradientView, NetWatcher code\n"
										"and the constructive discussions\n that led to most of the current Bme functionality\n\n"
										
										"This project is published under the MIT-licence\n\n"
										"MSN Protocol information can be found at\n"
										"http://www.hypothetic.org/docs/msn/index.php\n\n";
	const rgb_color K_TOP_COLOUR = 		{255,255,255,255};
	const rgb_color K_BOTTOM_COLOUR = 	{220,220,255,255};	
	
	const BString
				K_MAIN_APP_PREFS =		"MainAppPrefs";									
};
    		
namespace InterfaceMessages
{			
	//interface messages originating from and to the user
	const uint32
			K_USER_SIGN_IN_MSG =				'USsi',
			K_USER_SIGNED_IN_MSG =				'USsg',
			K_USER_SIGN_OUT_MSG =				'USso',
			K_USER_SIGNED_OUT_MSG =				'USsd',
			K_USER_DISCONNECTED_MSG =			'USdi',
			
			K_USER_CHANGE_STATUS_MSG =			'UScs',
			K_USER_CHANGE_NAME_MSG =			'UScn',
			K_USER_NAME_CHANGED_MSG =			'USnh',
			K_USER_CHANGE_DISPLAYPIC_MSG =		'UScd',
			K_USER_SET_PERSONAL_MSG	=			'USpm',			
			
			K_USER_START_CONV_MSG =				'USco',
			K_USER_QUIT_CONV_MSG =				'USqc',
			K_USER_SEND_TEXT_MSG =				'USst',
			K_USER_TYPING_MSG =					'USty',
			K_USER_FAILED_MSG =					'USfi',
			
			K_USER_START_FILE_TRANSFER_MSG =	'USft',
			K_USER_STOP_FILE_TRANSFER_MSG =		'USsf',
			K_USER_ACCEPT_FILE_TRANSFER_MSG =	'USaf', 
			K_USER_REFUSE_FILE_TRANSFER_MSG =	'USrf',
			K_USER_FILE_TRANSFER_UPDATE_MSG =	'USuf',
			
			K_USER_SHARE_BACKGROUNDS_MSG =		'USsb',
			K_USER_SHARE_EMOTICON_MSG =			'USse',
			
			K_USER_ADD_PRINCIPAL =				'USap',
			K_USER_REMOVE_PRINCIPAL =			'USrp',
			K_USER_RENAME_PRINCIPAL =			'USrn',
			
			K_USER_ADD_GROUP_MSG =				'USag',
			K_USER_REMOVE_GROUP_MSG =			'USrg',
			K_USER_RENAME_GROUP_MSG =			'USng',			
			
			K_USER_MAIL_NOTIFICATION_MSG =		'USmn',
			K_USER_CHECK_MAIL_MSG =				'UScm'
			;
	
	//interface messages originating from contact changes		
	const uint32
			K_CONTACT_CHANGE_STATUS_MSG =			'COcs',
			K_CONTACT_CHANGE_NAME_MSG =				'COcn',
			K_CONTACT_CHANGE_DISPLAYPIC_MSG =		'COcd',
			
			K_CONTACT_START_CONV_MSG =				'COco',
			K_CONTACT_QUIT_CONV_MSG =				'COqc',
			K_CONTACT_SEND_TEXT_MSG =				'COst',
			K_CONTACT_TYPING_MSG =					'COty',
			
			K_CONTACT_START_FILE_TRANSFER_MSG =		'COft',
			K_CONTACT_STOP_FILE_TRANSFER_MSG =		'COsf',
			K_CONTACT_ACCEPT_FILE_TRANSFER_MSG =	'COaf', 
			K_CONTACT_REFUSE_FILE_TRANSFER_MSG =	'COrf',
			K_CONTACT_FILE_TRANSFER_UPDATE_MSG =	'COuf',
			
			K_CONTACT_SHARE_BACKGROUNDS_MSG =		'COsb',
			K_CONTACT_SHARE_EMOTICON_MSG =			'COse',
			K_CONTACT_REQUEST_ADD_MSG =				'COra'
			;
			
	//normal UI messages, not needed in protocol
	const uint32
			K_ADD_CONTACT_MSG =						'ACms',
			K_ADD_GROUP_MSG =						'AGms',
			K_CONTACT_LIST_INVOKE_MSG =				'CLiv'
			;
	
	const uint32
			K_CONNECT_SB_MSG =						'CSms',
			K_PREFERENCES_MSG =						'PRms',
			K_PREFS_SELECTED_MSG =					'PRse',
			K_GET_LOGIN_NAMES =						'GEln',
			K_LOGIN_NAMES =							'LOna',
			K_CLOSE_CHAT_WINDOW =					'CLcw',
			K_ADD_CHAT_TEXT =						'ADct',
			K_OPEN_URL_MSG =						'OPur',
			K_INITIAL_PRESENCE_MSG =				'IPpr'
			;					
};

#endif
