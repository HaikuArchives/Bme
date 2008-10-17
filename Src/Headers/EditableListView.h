/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef EDITABLE_LIST_VIEW_H
#define EDITABLE_LIST_VIEW_H

#include <be/app/Message.h>
#include <be/app/Handler.h>
#include <be/interface/Rect.h>
#include <be/interface/Point.h>
#include <be/interface/TextView.h>
#include <be/interface/OutlineListView.h>

class TextEditView;

namespace ListConstants
{
	const uint32
			K_EDIT_COMPLETE_MSG =				'ECms'
			;
};

/** Editable ListView, makes it possible to edit the name of the items 
	in a ListView by clicking them and entering their new name.
	Based on YListView in the BeY! code by Catalin Minhai Ghita (okeanu)
	
	@author Catalin Minhai Ghita (okeanu)
	@version 1.00
	
	@author Tim de Jong
	@version 1.10
	@cleaned up the code a bit, made it more general and adapted it to
	the Bme coding style. Added virtual destructors.
	Got rid of the tooltip code.
	
	TODO: 
	4) let user decide about number of characters in EditView, maybe in
	   ListView constructor?	
 */
class EditableListView : public BOutlineListView
{
	public:
		EditableListView(BRect frame, const char *name, list_view_type type = B_SINGLE_SELECTION_LIST, uint32 resizingMode = B_FOLLOW_LEFT|B_FOLLOW_TOP, uint32 flags = B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE);
		virtual				~EditableListView();
	
		virtual void		AttachedToWindow();
		virtual void		MouseDown(BPoint where);
		virtual void 		MessageReceived(BMessage *message);
				
	private:
		TextEditView		*m_editView;
		BPoint				m_mousePos;
		int32				m_editCount,
							m_editItem;
};

class TextEditView : public BTextView
{
	public:	
		TextEditView(BRect textframe, BRect textrect, const BHandler *handler);
		virtual				~TextEditView();
		
		virtual void		KeyDown(const char *bytes, int32 numBytes);
		virtual void		Draw(BRect updateRect);	
		
	private:
		const BHandler		*m_handler;	
};

#endif
