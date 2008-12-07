/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef GRADIENT_VIEW
#include "GradientView.h"
#endif

GradientView::GradientView(BRect frame, const char* name, int32 follow, rgb_color topColor, rgb_color bottomColor)
				:	BView(frame, name, follow, B_WILL_DRAW|B_DRAW_ON_CHILDREN|B_FRAME_EVENTS|B_FULL_UPDATE_ON_RESIZE),
					m_topColor(topColor),
					m_bottomColor(bottomColor),	
					m_height(frame.Height())
{
	//Work out the gradient parameters
	CalculateGradient();
}

void GradientView::CalculateGradient()
{
	m_redChange = (m_bottomColor.red - m_topColor.red)/m_height;
	m_blueChange = (m_bottomColor.blue - m_topColor.blue)/m_height;
	m_greenChange = (m_bottomColor.green - m_topColor.green)/m_height;
	
	float maxChange = fabs(m_redChange);
	if(fabs(m_greenChange) > maxChange) maxChange = fabs(m_greenChange);
	if(fabs(m_blueChange) > maxChange) maxChange = fabs(m_blueChange);
	
	int thickness = 1;
	if(maxChange > 0)
	{
		if(maxChange <= 0.5)
		{
			//Can get away with thicker pen...
			thickness = static_cast<int>(1/maxChange);
		}
	} else {
		//Special case - top and bottom colors the same.
		//Make the pen very thick (having pens too thick might
		//not be good for the app server - just guessing though).
		thickness = 50;
	}
	
	//Adjust the change values so instead of per-pixel changes
	//they are now per-line changes
	m_redChange = m_redChange * thickness;
	m_blueChange = m_blueChange * thickness;
	m_greenChange = m_greenChange * thickness;
	//Set the pen size
	SetPenSize(thickness);
}

void GradientView::FrameResized(float width, float height)
{
	//Need to update whole view if the height has changed
	if(height != m_height)
	{
		m_height = height;
		CalculateGradient();
		Invalidate();
	}
}

void GradientView::Draw(BRect updateRect)
{
	float penSize = PenSize();
	int startLine = static_cast<int>(updateRect.top / penSize);
	int endLine = static_cast<int>(updateRect.bottom / penSize);

	BPoint leftPoint = BPoint(Bounds().left, (int)(penSize / 2) + penSize * startLine);
	BPoint rightPoint = BPoint(Bounds().right, leftPoint.y);

	float floatRed = m_topColor.red + startLine * m_redChange;
	float floatBlue = m_topColor.blue + startLine * m_blueChange;
	float floatGreen = m_topColor.green + startLine * m_greenChange;

	rgb_color lineColor;

	BeginLineArray(endLine - startLine + 1);
	for(int lineNo = startLine; lineNo <= updateRect.bottom; lineNo++)
	{
		lineColor.red = static_cast<uint8>(floatRed);
		lineColor.green = static_cast<uint8>(floatGreen);
		lineColor.blue = static_cast<uint8>(floatBlue);
		
		AddLine(leftPoint, rightPoint, lineColor);
		
		leftPoint.y += penSize;
		rightPoint.y = leftPoint.y;
		
		floatRed += m_redChange;
		floatBlue += m_blueChange;
		floatGreen += m_greenChange;
	}
	EndLineArray();
}
