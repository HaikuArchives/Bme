/*****************************************************************
 * Copyright (c) 2005, Tim de Jong								 *
 *																 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
 #ifndef STATUS_ITEM_H
 #include "StatusItem.h"
 #endif
 
 #include <be/interface/Point.h>
 #include <be/interface/View.h>
 
 StatusItem::StatusItem(BBitmap *icon, const char *label, BMessage *message, char shortcut, uint32 modifiers)
 				:	BMenuItem(label,message,shortcut,modifiers),
 					m_statusIcon(icon)
 				
 { 	
 }
 
 StatusItem::~StatusItem()
 { 	
 }
 
 void StatusItem::DrawContent()
 { 	
 	//center icon and Text
	float width, height = 0;
 	GetContentSize(&width, &height);
 	float dy = (height - (m_statusIcon->Bounds()).Height()) / 2.0f;
 	
 	Menu()->MovePenTo(ContentLocation().x + 2.0f, ContentLocation().y + 2.0f);
 	Menu()->SetDrawingMode(B_OP_ALPHA);
 	Menu()->DrawBitmap(m_statusIcon);
 	Menu()->SetDrawingMode(B_OP_COPY);
 	
 	float x = ContentLocation().x + 10.0f + (m_statusIcon->Bounds()).Width();
 	float y = ContentLocation().y + (m_statusIcon->Bounds()).Height() + dy;
 	BPoint textLocation(x,y);
 	Menu()->DrawString(Label(),textLocation); 	
}
