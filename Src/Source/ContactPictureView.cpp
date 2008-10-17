#ifndef CONTACT_PICTURE_VIEW_H
#include "ContactPictureView.h"
#endif

#include <be/translation/TranslationUtils.h>
#include <be/translation/BitmapStream.h>
#include <be/translation/TranslatorRoster.h>
#include <be/interface/Alert.h>
#include <be/storage/Mime.h>
#include <be/storage/Path.h>
#include <be/storage/File.h>
#include <be/storage/Entry.h>
#include "Common.h"

ContactPictureView::ContactPictureView(BRect frame)
						:	BView(frame,"ContactPictureView",B_FOLLOW_TOP | B_FOLLOW_RIGHT,B_WILL_DRAW),
							m_contactPicture(NULL)
{
	m_contactPicture = BTranslationUtils::GetBitmap("/boot/home/Documents/Homer.png");
}

ContactPictureView::~ContactPictureView()
{
}

void ContactPictureView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		//maybe move this to window later
		case B_COPY_TARGET:
		{
			//send the data message as answer to this negotiation message
			BMessage dataMsg = HandleDragFormat(message);			
			message->SendReply(&dataMsg);			
		}
		break;
		default:
			BView::MessageReceived(message);
		break;
	}
}

void ContactPictureView::MouseDown(BPoint where)                            
{ 
    //============code from Hartmut Reh's Haiku Newsletter==================
    BPoint cursor;
    uint32 buttons;
	
    SetMouseEventMask(B_POINTER_EVENTS, 0 );// view can receive mouse-events
    GetMouse(&cursor, &buttons);

    if ((m_contactPicture != NULL) && (buttons & B_PRIMARY_MOUSE_BUTTON))
    {        
        //get boundaries of image
        BRect bitmapRect  = m_contactPicture->Bounds();                         
        //create bitmap for drag-and-drop 
        BBitmap *bitmap = new BBitmap(bitmapRect, B_RGB32,true);       		                                      
        //copy bitmap
        memcpy(bitmap->Bits(), m_contactPicture->Bits(), m_contactPicture->BitsLength());  
        //position relative to bitmap
        //BPoint origBitmap = BPoint(bitmapRect.Width(), bitmapRect.Height());     
        BMessage bitmapMessage(B_SIMPLE_DATA);
        AddDragFormats(&bitmapMessage);
        bitmapMessage.AddInt32("be:actions",B_COPY_TARGET);
        bitmapMessage.AddString("be:clip_name", "test");//m_contact->getFriendlyName());
        //m_contactPicture->Archive(&bitmapMessage);
        DragMessage(&bitmapMessage, bitmap, B_OP_BLEND, where, this); 	
    }
	
    //ContactPictureView::MouseDown(where);                                        
}

void ContactPictureView::Draw(BRect updateRect)
{
	//draw border
	rgb_color black = {0,0,0};
	SetHighColor(black);
	StrokeRect(Bounds());
	//draw bitmap
	if (m_contactPicture)
	{
		BRect pictureRect = Bounds();
		pictureRect.InsetBy(1.0f,1.0f);
		DrawBitmap(m_contactPicture, pictureRect);
	}
}

void ContactPictureView::SetContact(Contact *contact)
{
	m_contact = contact;
	//m_contactPicture = contact->;
}

Contact* ContactPictureView::GetContact()
{
	return m_contact;
}
//maybe move this to window later
void ContactPictureView::AddDragFormats(BMessage* message)
{
	//accepts communication by file	
	message->AddString("be:types", "image/png");	
	message->AddString("be:types", B_FILE_MIME_TYPE);		
	//if so, also add the following
	message->AddString("be:filetypes", B_FILE_MIME_TYPE);
	//accepts message communication
	//message->AddString("be:types", );
}

BMessage ContactPictureView::HandleDragFormat(BMessage* negotiationMessage)
{
	//construct an answer to the negotiation message sent
	BString typeString;
	BMessage dataMsg(B_MIME_DATA);
	if (negotiationMessage->FindString("be:types", &typeString) == B_OK)
	{
		if (typeString == B_FILE_MIME_TYPE)
		{
			WriteBitmapFile(negotiationMessage);
		}
	}
	return dataMsg;
}
//move to protocol layer!!!?
status_t ContactPictureView::WriteBitmapFile(BMessage* negotiationMessage)
{
	//other program wants a file
	entry_ref dirRef;
	//compose file path and filename 
	if (negotiationMessage->FindRef("directory", &dirRef) == B_OK)
	{
		BEntry dirEntry(&dirRef);
		BPath newFilePath(&dirEntry);
		BString nameString;
		if (negotiationMessage->FindString("name", &nameString) == B_OK)
		{
			newFilePath.Append(nameString.String());
			
			BTranslatorRoster *translatorRoster = BTranslatorRoster::Default();
			BBitmapStream bitmapStream(m_contactPicture);
			//write file data
			BFile newFile(newFilePath.Path(), B_CREATE_FILE | B_WRITE_ONLY);
			if (newFile.InitCheck() == B_OK)
			{
				uint32 type = Common::FindTranslatorConstant(translatorRoster, "image/png");
				if (type != 0 && translatorRoster->Translate(&bitmapStream, NULL, NULL, &newFile, type, 0) != B_OK)
				{
					BAlert *alert = new BAlert(NULL, "Could not drop image to Tracker.", "OK");
					alert->Go();
				}
			}
			bitmapStream.DetachBitmap(&m_contactPicture);
		}
	}
	return B_OK;
}

void ContactPictureView::FrameResized(float width, float height)
{
	//Invalidate();
	BView::FrameResized(width, height);
}

void ContactPictureView::FrameMoved(BPoint newLocation)
{
	//Invalidate();
	BView::FrameMoved(newLocation);
}
		