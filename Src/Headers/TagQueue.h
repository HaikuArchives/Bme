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
		
		virtual Tag*			Pop();
		virtual Tag*			FirstElement();
		virtual Tag*			TagAt(int32 index);
		virtual void			Add(Tag* tag);
		
		int32				CountItems();
		bool				IsEmpty();		
	
	private:
		BList			*m_tagList;
};

#endif
