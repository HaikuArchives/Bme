/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef EDITABLE_LIST_ITEM_H
#define EDITABLE_LIST_ITEM_H

#include <be/app/Message.h>
#include <be/interface/ListItem.h>
#include <be/support/String.h>

class EditableListItem : public BStringItem
{
	public:
		EditableListItem(BString text, BMessage *changedMessage, bool editable = true, uint32 level = 0, bool expanded = true);
		virtual				~EditableListItem();
		
		virtual void		SetText(const char *text);
		
		void				SetChangedMessage(BMessage *changedMessage);
		BMessage*			ChangedMessage() const;
			
		void				SetEditable(bool editable);
		bool				IsEditable();
	
	private:
		BMessage			*m_changedMessage;		
		bool				m_isEditable;
			
};

#endif
