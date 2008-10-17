/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MSN_OBJECT_H
#include "MsnObject.h"
#endif

#include "ProtocolConstants.h"
#include "base64.h"

/** Default Constructor
	Creates an empty MsnObject.
 */
MsnObject::MsnObject()
			:	BMessage(ProtocolConstants::K_MSN_OBJECT_MSG)
{
}

/** Constructor 
	Takes received object code as argument and creates
	a BMessage representing the MsnObject.
 */
MsnObject::MsnObject(BString objectCode)
			:	BMessage(ProtocolConstants::K_MSN_OBJECT_MSG)
{
	parseObjectString(objectCode);
}

/** Destructor
 */
MsnObject::~MsnObject()
{
}

/**
 */
void MsnObject::computeSHA1D()
{
	//calculate SHA-1 hash
	
	//encode with BASE64
	
	//add result to message
}

/**
 */
void MsnObject::computeSHA1C()
{
	//calculate SHA-1 hash
	
	//encode with BASE64
	
	//add result to message
}

/**
 */
void MsnObject::parseObjectString(BString objectString)
{
}
