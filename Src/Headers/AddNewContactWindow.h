#ifndef ADD_NEW_CONTACT_WINDOW
#define ADD_NEW_CONTACT_WINDOW

#include <app/Message.h>
#include <interface/Rect.h>
#include <interface/Window.h>
#include <interface/View.h>
#include <interface/Menu.h>

class AddNewContactView;
		
class AddNewContactWindow : public BWindow
{
	public:
		AddNewContactWindow(BRect frame, const char* contactPassport);	
		
	public:		
		enum AddValue
		{
			K_YES_VALUE = 0,
			K_NO_VALUE,
			K_BLOCK_VALUE
		}
		AddValue;
		
		enum AddMessages
		{
			K_YES_MSG			= 'YSms',
			K_NO_MSG 			= 'NOms',
			K_BLOCK_MSG			= 'BLms',
			K_OK_MSG			= 'OKms'
		}
		AddMessages;
		
	
	public:		
		virtual void			MessageReceived(BMessage *message);
		enum AddValue			GetChosenValue();					
	
	private:
		enum AddValue			m_chosenAddValue;
};

class AddNewContactView : public BView
{
	public:
		AddNewContactView(BRect frame, const char* contactPassport);		
	
	private:
		BMenu*					GetContactGroups();					
	
};

#endif
