#ifndef TAG_QUEUE_H
#include "TagQueue.h"
#endif

#include <support/List.h>
#include "Tag.h"

TagQueue::TagQueue()
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
		
Tag* TagQueue::Pop()
{
	if (!IsEmpty())
	{
		int32 firstIndex = 0;
		Tag* tag = static_cast<Tag*>(m_tagList->RemoveItem(firstIndex));
		return tag;
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
