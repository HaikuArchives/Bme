#ifndef MESSAGE_ITEM_H
#define MESSAGE_ITEM_H

#include <interface/ListItem.h>
class ConvMessage;

class MessageItem : public BListItem
{
	public:
		MessageItem(ConvMessage* message);
		virtual				~MessageItem();

		ConvMessage*		Message();					
		virtual	void		SetMessage(ConvMessage* message);		
		virtual void 		DrawItem(BView* owner, BRect itemRect, bool drawEverything = false) = 0;
		virtual void 		Update(BView* owner, const BFont* font) = 0;
		
	private:
		ConvMessage*		m_message;		
};

#endif

