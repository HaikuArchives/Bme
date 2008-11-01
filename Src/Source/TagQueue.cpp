#ifndef TAG_QUEUE_H
#include "TagQueue.h"
#endif

#include <support/List.h>
#include "Tag.h"

TagQueue::TagQueue()
			:	m_currentTagIndex(-1)
{
	m_tagList = new BList();
}

TagQueue::~TagQueue()
{
	//delete list items
	for (int32 i = 0; i < m_tagList->CountItems(); i++)
	{
		int32 firstIndex = 0;
	   	Tag* tag = static_cast<Tag*>(m_tagList->RemoveItem(firstIndex));
	   	delete tag;	
	}
	//delete list itself
	delete m_tagList;
}

void TagQueue::Rewind()
{
	m_currentTagIndex = -1;
}

bool TagQueue::HasNext()
{
	int32 nextIndex = m_currentTagIndex + 1; 
	if (!IsEmpty() && nextIndex < CountItems())
	{
		return true;
	}
	return false;
}
		
Tag* TagQueue::Next()
{
	if (!IsEmpty() && HasNext())
	{
		m_currentTagIndex++;
		return TagAt(m_currentTagIndex);
	}
	else
	{
		return NULL;
	}
}

bool TagQueue::HasPrevious()
{
	int32 previousIndex = m_currentTagIndex - 1; 
	if (!IsEmpty() && previousIndex > 0)
	{
		return true;
	}
	return false;
}

Tag* TagQueue::Previous()
{
	if (!IsEmpty() && HasPrevious())
	{
		m_currentTagIndex--;
		return TagAt(m_currentTagIndex);
	}
	else
	{
		return NULL;
	}
}

Tag* TagQueue::FirstElement()
{
	int32 firstIndex = 0;
	return TagAt(firstIndex);	
}

Tag* TagQueue::TagAt(int32 index)
{
	Tag* tag = static_cast<Tag*>(m_tagList->ItemAt(index));
	return tag;
}	

void TagQueue::Add(Tag* tag)
{
	m_tagList->AddItem(tag);
}

int32 TagQueue::CountItems()
{
	return m_tagList->CountItems();
}

bool TagQueue::IsEmpty()
{
	return m_tagList->IsEmpty();
}
