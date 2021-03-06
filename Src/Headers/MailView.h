/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
 
#ifndef MAIL_VIEW_H
#define MAIL_VIEW_H

#include <be/support/SupportDefs.h>
#include <be/support/String.h>
#include <be/interface/Rect.h>
#include <be/interface/Point.h>
#include <be/interface/View.h>
#include <be/interface/Bitmap.h>
#include <be/app/Message.h>

/** Letter bitmap, converted to a constant field by Wonderbrush
 */
namespace Letter
{
	//bitmap dimensions
	const uint32 K_LETTER_WIDTH = 19;
	const uint32 K_LETTER_HEIGHT = 19;
	const color_space K_LETTER_FORMAT = B_RGBA32;	

	const unsigned char K_LETTER_BITS[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x14, 0x17, 0x18, 0xbf,
	0x00, 0x02, 0x04, 0x76, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x20, 0x57, 0x58, 0x58, 0xdc, 0xba, 0xc0, 0xc0, 0xff, 0x7d, 0xac, 0xb9, 0xff,
	0x20, 0x4b, 0x56, 0xe1, 0x00, 0x00, 0x04, 0x79, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1d, 0x47, 0x48, 0x48, 0xd3,
	0xf4, 0xf9, 0xf9, 0xff, 0xcf, 0xdc, 0xda, 0xff, 0xde, 0xde, 0xdd, 0xff, 0xdf, 0xff, 0xff, 0xff,
	0x7c, 0xbd, 0xcb, 0xff, 0x1a, 0x4e, 0x5c, 0xe3, 0x00, 0x01, 0x05, 0x79, 0x00, 0x00, 0x00, 0x16,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1d, 0x47, 0x48, 0x48, 0xd3, 0xe9, 0xf4, 0xf4, 0xff, 0xe8, 0xff, 0xff, 0xff,
	0xd1, 0xef, 0xed, 0xff, 0xbe, 0xc2, 0xc2, 0xff, 0xe8, 0xff, 0xff, 0xff, 0xe5, 0xff, 0xff, 0xff,
	0xc7, 0xff, 0xff, 0xff, 0x71, 0xbd, 0xcc, 0xff, 0x1a, 0x4d, 0x5b, 0xe6, 0x00, 0x01, 0x05, 0x83,
	0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x47, 0x49, 0x49, 0xd3,
	0xe9, 0xf9, 0xf8, 0xff, 0xdb, 0xff, 0xff, 0xff, 0xca, 0xee, 0xec, 0xff, 0xb4, 0xd5, 0xd5, 0xff,
	0xa1, 0xa9, 0xab, 0xff, 0xd8, 0xfc, 0xfb, 0xff, 0xc8, 0xfe, 0xfd, 0xff, 0xcc, 0xfe, 0xfd, 0xff,
	0xd2, 0xff, 0xff, 0xff, 0xb5, 0xff, 0xff, 0xff, 0x68, 0xbf, 0xce, 0xff, 0x18, 0x50, 0x5f, 0xe9,
	0x00, 0x06, 0x0b, 0x82, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x21, 0x3a, 0x3e, 0x3e, 0xcc, 0xc8, 0xd8, 0xd8, 0xff, 0xa3, 0xcc, 0xd1, 0xff,
	0x82, 0xb7, 0xc1, 0xff, 0x77, 0xb5, 0xc3, 0xff, 0x7c, 0xc6, 0xd2, 0xff, 0x8e, 0xb6, 0xbc, 0xff,
	0xb6, 0xce, 0xcd, 0xff, 0xac, 0xe2, 0xe2, 0xff, 0xad, 0xe3, 0xe3, 0xff, 0xac, 0xe5, 0xe5, 0xff,
	0xad, 0xe6, 0xe5, 0xff, 0xb4, 0xf3, 0xf2, 0xff, 0x9a, 0xf0, 0xf7, 0xff, 0x57, 0xb5, 0xc7, 0xff,
	0x1a, 0x4a, 0x59, 0xe9, 0x07, 0x09, 0x0a, 0x89, 0x00, 0x00, 0x00, 0x25, 0x01, 0x01, 0x01, 0x7a,
	0x38, 0x58, 0x61, 0xff, 0x4e, 0xbb, 0xd8, 0xff, 0x79, 0xe6, 0xf8, 0xff, 0x96, 0xf3, 0xfa, 0xff,
	0xa0, 0xf9, 0xfd, 0xff, 0xa9, 0xff, 0xff, 0xff, 0x9f, 0xed, 0xef, 0xff, 0x6b, 0xc0, 0xd0, 0xff,
	0x59, 0xbc, 0xd5, 0xff, 0x5b, 0xc1, 0xd9, 0xff, 0x55, 0xb6, 0xd0, 0xff, 0x5e, 0xbc, 0xd4, 0xff,
	0x5a, 0xc4, 0xdc, 0xff, 0x5e, 0xc7, 0xde, 0xff, 0x64, 0xd2, 0xe9, 0xff, 0x57, 0xd9, 0xf6, 0xff,
	0x29, 0x7c, 0x94, 0xff, 0x01, 0x04, 0x05, 0x8e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35,
	0x03, 0x20, 0x29, 0xa5, 0x2d, 0x7d, 0x8e, 0xfc, 0x6a, 0xdc, 0xeb, 0xff, 0x91, 0xff, 0xff, 0xff,
	0x94, 0xfd, 0xfe, 0xff, 0x8e, 0xf9, 0xfb, 0xff, 0x8a, 0xfb, 0xff, 0xff, 0x84, 0xf9, 0xfe, 0xff,
	0x81, 0xfe, 0xff, 0xff, 0x6b, 0xc3, 0xd4, 0xff, 0x9e, 0xe2, 0xe6, 0xff, 0x7a, 0xfb, 0xff, 0xff,
	0x7a, 0xf7, 0xfd, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x47, 0xb8, 0xc8, 0xff, 0x03, 0x16, 0x1c, 0xad,
	0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x36, 0x00, 0x17, 0x1f, 0xa9, 0x24, 0x76, 0x89, 0xfa, 0x57, 0xd7, 0xea, 0xff,
	0x77, 0xff, 0xff, 0xff, 0x7b, 0xfc, 0xfe, 0xff, 0x76, 0xf6, 0xf9, 0xff, 0x71, 0xfa, 0xfe, 0xff,
	0x66, 0xc1, 0xd1, 0xff, 0x97, 0xdf, 0xe2, 0xff, 0x6b, 0xf8, 0xfd, 0xff, 0x72, 0xff, 0xff, 0xff,
	0x49, 0xbe, 0xca, 0xff, 0x04, 0x12, 0x17, 0xaf, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x14, 0x1c, 0xa6, 0x1e, 0x6f, 0x82, 0xfa,
	0x4a, 0xd5, 0xe8, 0xff, 0x62, 0xfe, 0xff, 0xff, 0x62, 0xff, 0xff, 0xff, 0x5f, 0xc5, 0xd7, 0xff,
	0x98, 0xe0, 0xe4, 0xff, 0x5e, 0xff, 0xff, 0xff, 0x43, 0xc0, 0xca, 0xff, 0x04, 0x14, 0x18, 0xb8,
	0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x01, 0x13, 0x1a, 0xa3,
	0x1b, 0x6f, 0x83, 0xf7, 0x3c, 0xd4, 0xe7, 0xff, 0x5c, 0xd0, 0xe8, 0xff, 0xa3, 0xf3, 0xf7, 0xff,
	0x3d, 0xcd, 0xd4, 0xff, 0x04, 0x14, 0x18, 0xba, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37,
	0x00, 0x10, 0x17, 0xa4, 0x2b, 0x5d, 0x6d, 0xf5, 0x78, 0xb0, 0xb3, 0xff, 0x06, 0x20, 0x22, 0xc1,
	0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

};

/** View that displays if the person using Bme has mail. Should
	open a browser when clicked.
	@author Tim de Jong
 */
class MailView : public BView
{
	public:
		MailView(BRect frame);
		virtual				~MailView();
		
		virtual void		MessageReceived(BMessage *message);
		virtual void		Draw(BRect updateRect);
		virtual void		MouseMoved(BPoint point, uint32 transit, const BMessage *message);
		virtual void		MouseDown(BPoint point);
		
	private:	
		void				setNumberOfEmails(int32 emails);		
			
	private:
		BBitmap				*m_letterBitmap;
		int32				m_numberOfEmails;
		BString				m_emailText;
		
};

#endif

