/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef LAYERED_VIEW_H
#define LAYERED_VIEW_H

#include <be/interface/Rect.h>
#include <be/interface/View.h>
#include <vector.h>

/**	View that contains a number of other views in different layers and
	displays one of them.
	@author Tim de Jong
 */
class LayeredView : public BView
{
	public:
		LayeredView(BRect frame, const char *name, uint32 resizingMode, uint32 flags);
		
		void				addLayer(BView* layer);
		BView*				getLayer(uint32 layer);
		int32				getNumberOfLayers();
		
		void				setVisible(uint32 layer);
		int32				getVisible();
		
	private:
		vector<BView*>		m_layers;
		int32				m_visible;
};

#endif
