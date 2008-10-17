/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MESSAGE_FIELDS_H
#define MESSAGE_FIELDS_H

//field constants of a K_ADD_COMMAND_MESSAGE message
#define K_COMMAND				"command"
#define K_TR_ID					"trID"
#define K_REMAINING_MSG			"remainingMsg"
#define K_PAYLOAD_SIZE			"payloadSize"
#define K_PAYLOAD_DATA			"payloadData"
//field constants of a K_ERROR_MESSAGE message
#define K_ERROR_STRING			"errorMsg"
#define K_CRITICAL_BOOL			"critical"
//field constants of a K_OPEN_SB_SESSION message
#define K_SB_SESSION_ID			"sbSessionId"
#define K_SB_ADDRESS			"sbAddress"
#define K_SB_PORT				"sbPort"
#define K_AUTHENTICATION_STRING	"authString"
#define K_CONTACT_FRIENDLY_NAME	"contactFriendlyName"
//field constants of the K_USER_SIGNED_IN_MSG
#define K_INITIAL_STATUS		"statusString"
//field constants of a K_USER_CHANGE_NAME_MSG message
#define K_NEW_NAME_STRING		"newName"
#define K_PERSONAL_MESSAGE		"personalMessage"
//field constants of a K_USER_CHANGE_STATUS_MSG message
#define K_NEW_STATUS			"newStatus"
//field constants of a message
#define	K_URL_STRING			"urlString"
//field constants for K_CONTACT_TYPING_MSG
#define K_CONTACT_PASSPORT		"contactName"
#define K_IS_TYPING				"isTyping"
//field constants for K_ADD_CONTACT_MSG
#define K_CONTACT				"contact"
//field constants for K_ADD_GROUP_MSG
#define K_GROUP_NAME			"groupName"
#define K_GROUP_ID				"groupId"
//field constants for K_CONTACT_SEND_TEXT_MSG
#define K_MESSAGE_TEXT			"messageText"
//field constants for K_PREFERENCES_MSG
#define K_ACTIVE_TAB			"activeTab"

#endif
