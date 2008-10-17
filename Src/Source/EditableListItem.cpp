/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef EDITABLE_LIST_ITEM_H
#include "EditableListItem.h"
#endif

EditableListItem::EditableListItem(BString text, BMessage *changedMessage, bool editable, uint32 level, bool expanded)
					:	BStringItem(text.String(), level, expanded),
						m_changedMessage(changedMessage),
						m_isEditable(editable)
{
}

EditableListItem::~EditableListItem()
{
	delete m_changedMessage;
}

void EditableListItem::SetText(const char *text)
{
	//only set text if allowed!
	if (m_isEditable)
	{
		BStringItem::SetText(text);
	}
}

void EditableListItem::SetChangedMessage(BMessage *changedMessage)
{
	delete m_changedMessage;
	m_changedMessage = changedMessage;
}

BMessage* EditableListItem::ChangedMessage() const
{
	return m_changedMessage;
}

void EditableListItem::SetEditable(bool editable)
{
	m_isEditable = editable;
}

bool EditableListItem::IsEditable()
{
	return m_isEditable;
}
