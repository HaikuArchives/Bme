#ifndef CONTACT_PICTURE_VIEW_H
#define CONTACT_PICTURE_VIEW_H

#include <be/app/Message.h>
#include <be/interface/Rect.h>
#include <be/interface/View.h>
#include <be/interface/Bitmap.h>
#include "Contact.h"

class ContactPictureView : public BView
{
	public:
		ContactPictureView(BRect frame);
		virtual				~ContactPictureView();

		virtual void		MessageReceived(BMessage *message);
		virtual void		MouseDown(BPoint where);		
		virtual void		Draw(BRect updateRect);
		virtual void		FrameResized(float width, float height);
		virtual void		FrameMoved(BPoint newLocation);
		
		void				SetContact(Contact* contactPicture);
		Contact*			GetContact();
		
	private:
		void				AddDragFormats(BMessage* message);
		BMessage			HandleDragFormat(BMessage* negotiationMessage);
		status_t			WriteBitmapFile(BMessage* negotiationMessage);
		
	private:
		Contact				*m_contact;
		BBitmap				*m_contactPicture;
};

#endif
