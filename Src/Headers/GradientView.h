/*****************************************************************
 * Copyright (c) 2005 Simon Taylor, Tim de Jong					 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/

#ifndef GRADIENT_VIEW
#define GRADIENT_VIEW

#include <be/interface/Rect.h>
#include <be/interface/View.h>
#include <be/interface/GraphicsDefs.h>

class GradientView : public BView
{
	public:
		GradientView(BRect frame, const char* name, int32 follow, rgb_color topColor, rgb_color bottomColor);
		
		virtual void		Draw(BRect updateRect);
		virtual void		FrameResized(float width, float height);
	
	private:
		void				CalculateGradient();
		
		rgb_color			m_topColor,
							m_bottomColor;
		
		float				m_height,
							m_redChange,
							m_blueChange,
							m_greenChange;
};

#endif
