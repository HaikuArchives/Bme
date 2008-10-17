#ifndef ADD_NEW_CONTACT_WINDOW
#include "AddNewContactWindow.h"
#endif

#include <app/Messenger.h>
#include <interface/Button.h>
#include <interface/RadioButton.h>
#include <interface/StringView.h>
#include <interface/Font.h>
#include <interface/MenuField.h>
#include <support/String.h>
#include "GradientView.h"
#include <iostream>

AddNewContactWindow::AddNewContactWindow(BRect frame, const char* contactPassport)
						:	BWindow(frame, "Someone wants to add you to his/her contact list", B_TITLED_WINDOW,B_NOT_RESIZABLE),
							m_chosenAddValue(K_YES_VALUE)
{
	AddChild(new AddNewContactView(Bounds(), contactPassport));
} 

void AddNewContactWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case K_YES_MSG:		
			m_chosenAddValue = AddNewContactWindow::K_YES_VALUE;
		break;
		case K_NO_MSG:
			m_chosenAddValue = AddNewContactWindow::K_NO_VALUE;
		break;
		case K_BLOCK_MSG:
			m_chosenAddValue = AddNewContactWindow::K_BLOCK_VALUE;
		break;
		case K_OK_MSG:		
			Quit();			
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}

enum AddNewContactWindow::AddValue AddNewContactWindow::GetChosenValue()
{
	return m_chosenAddValue;
}

//===========================AddNewContactView=============================================
AddNewContactView::AddNewContactView(BRect frame, const char* contactPassport)
						:	BView(frame, "AddNewContactView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	rgb_color topColour = {255,255,255,255};
	rgb_color bottomColour = {220,220,255,255};
	GradientView *backgroundView = new GradientView(Bounds(),"backgroundView", B_FOLLOW_ALL_SIDES,topColour,bottomColour);
	AddChild(backgroundView);
	
	BRect backgroundBounds = backgroundView->Bounds();
	
	float x1 = backgroundBounds.left + 20.0f;
	float x2 = backgroundBounds.Width() - 20.0f;
	//question
	BString addQuestion = "\'"; 
	addQuestion.Append(contactPassport);
	addQuestion.Append("\' wants to add you to his or her contact list.");
	
	BRect addQuestionFrame(x1 - 10.0f, 10.0f,x2, 24.0f);
	BStringView *addQuestionView = new BStringView(addQuestionFrame, "addQuestionView", addQuestion.String());
	addQuestionView->SetFont(be_bold_font);
	backgroundView->AddChild(addQuestionView);
	
	BString addQuestionPart2 = "Do you want to add this person to your contactlist too?";
	
	BRect addQuestionFrame2(x1 - 10.0f, 25.0f,x2, 40.0f);
	BStringView *addQuestionView2 = new BStringView(addQuestionFrame2, "addQuestionView2", addQuestionPart2.String());
	addQuestionView2->SetFont(be_bold_font);
	backgroundView->AddChild(addQuestionView2);
	
	//radio options	
	float y1 = addQuestionFrame2.bottom + 15.0f;
	float y2 = y1 + 10.0f;
	BRect yesRadioFrame(x1, y1, x2, y2);
	BString answerYes = "Yes, add this person to the following group in my contact list:";
	BRadioButton *yesRadioButton = new BRadioButton(yesRadioFrame, "yesRadioButton", answerYes.String(), new BMessage(AddNewContactWindow::K_YES_MSG));
	yesRadioButton->SetTarget(Window());
	//Adding the person to a specific group is the default
    yesRadioButton->SetValue(B_CONTROL_ON);
    backgroundView->AddChild(yesRadioButton);
       
    //add person to specific group, make sure top group in contactlistview is also first in this one!    
    BRect groupFieldFrame(x1 + 20.0f, yesRadioFrame.bottom + 10.0f,x2, yesRadioFrame.bottom + 20.0f);
    BMenuField *groupField = new BMenuField(groupFieldFrame, "groupField", "", GetContactGroups());
    groupField->SetDivider(0.0f);
    backgroundView->AddChild(groupField);
    
    //option for not adding the person 
    BRect noRadioFrame(yesRadioFrame.left,groupFieldFrame.bottom + 20.0f,yesRadioFrame.right, groupFieldFrame.bottom + 20.0f + 10.0f);   
	BString answerNo = "No, but allow this person to see my online status.";
	BRadioButton *noRadioButton = new BRadioButton(noRadioFrame, "noRadioButton", answerNo.String(), new BMessage(AddNewContactWindow::K_NO_MSG));
	noRadioButton->SetTarget(Window());
	backgroundView->AddChild(noRadioButton);
	
	//option for blocking the person
	BRect blockRadioFrame(yesRadioFrame.left,noRadioFrame.bottom + 10.0f,yesRadioFrame.right, noRadioFrame.bottom + 10.0f + 10.0f);
	BString answerBlock = "Definitely Not! Please block this person.";
	BRadioButton *blockRadioButton = new BRadioButton(blockRadioFrame, "blockRadioButton", answerBlock.String(), new BMessage(AddNewContactWindow::K_BLOCK_MSG));
	blockRadioButton->SetTarget(Window());
	backgroundView->AddChild(blockRadioButton);
		
	//add okButton to right bottom of the screen
	float okWidth = 70.0f;
	float okHeight = 20.0f;
	float borderSize = 5.0f;	
	
	BRect okFrame(backgroundBounds.Width() - okWidth - borderSize, backgroundBounds.Height() - okHeight - borderSize, backgroundBounds.Width() - borderSize, backgroundBounds.Height() - borderSize);
	BButton *okButton = new BButton(okFrame,"okButton","Ok", new BMessage(AddNewContactWindow::K_OK_MSG));	
	okButton->SetTarget(Window());
	backgroundView->AddChild(okButton);	
	
}

BMenu* AddNewContactView::GetContactGroups()
{
	BMenu *groupMenu = new BMenu("No groups available");
	return groupMenu;
}
