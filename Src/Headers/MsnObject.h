/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef MSN_OBJECT_H
#define MSN_OBJECT_H

#include <be/app/Message.h>
#include <be/support/String.h>

class MsnObject : public BMessage
{
	public:
		MsnObject();
		MsnObject(BString objectCode);
		virtual				~MsnObject();
		
		void				computeSHA1D();
		void				computeSHA1C();				
	
	public:
		enum TypeConstants
		{
							K_UNKNOWN_TYPE1 = 1,
							K_CUSTOM_EMOTICON,	
							K_DISPLAY_PICTURE,
							K_UNKNOWN_TYPE2,
							K_BACKGROUND_IMAGE,
							K_UNKNOWN_TYPE3,
							K_DYNAMIC_DISPLAY_PICTURE,
							K_WINK
		};
				
	private:
		void				parseObjectString(BString objectString);
};

#endif
