#ifndef TAG_QUEUE_H
#define TAG_QUEUE_H

#include <support/SupportDefs.h>

class BList;
class Tag;

class TagQueue
{
	public:
		TagQueue();
		virtual				~TagQueue();
		
		virtual void			Rewind();
		virtual bool			HasNext();
		virtual Tag*			Next();
		virtual bool			HasPrevious();
		virtual Tag*			Previous();
		
		virtual Tag*			FirstElement();
		virtual Tag*			TagAt(int32 index);
		virtual void			Add(Tag* tag);
		
		int32					CountItems();
		bool					IsEmpty();		
	
	private:
		int32			m_currentTagIndex;
		BList			*m_tagList;		
};

#endif
