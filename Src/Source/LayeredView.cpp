/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef LAYERED_VIEW_H
#include "LayeredView.h"
#endif

/** Constructor
 */
LayeredView::LayeredView(BRect frame, const char *name, uint32 resizingMode, uint32 flags)
				: BView(frame, name, resizingMode, flags)
{
	m_visible = -1;
}

/** Adds a view to this layered view, adds it as the last view.
	@param layer, the new view to add.
 */
void LayeredView::addLayer(BView* layer)
{
	m_layers.push_back(layer);
	AddChild(layer);
	
	layer->LockLooper();
	if (m_visible != -1) 
		layer->Hide();
	else 
		m_visible = 0;
	layer->UnlockLooper();
}

/**	Returns the view at layer "layer".
	@return the view at layer "layer".
 */
BView* LayeredView::getLayer(uint32 layer)
{
	if (layer >= 0 && layer < m_layers.size())
		return m_layers[layer];
	else 
		return NULL;
}

/**	Get the number of layers/views added to this layeredview.
	@return the number of layers in this view. 
 */
int32 LayeredView::getNumberOfLayers()
{
	return m_layers.size();	
}

/** Sets the visible view to the view found under layernumber "layer".
	@param layer, the view to be set to visible.
 */
void LayeredView::setVisible(uint32 layer)
{
	//check if valid layer number
	if (layer >= 0 && layer < m_layers.size())	
	{
		//only change visibility if layer is changed
		if (layer != m_visible)	
		{
			BView* visible = m_layers[m_visible];
			visible->Hide();
			visible = m_layers[layer];
			visible->Show();
			m_visible = layer;
		}
	}
}
/**	Gets the number of the view currently visible.
	@return the number of the view currently visible.
 */
int32 LayeredView::getVisible()
{
	return m_visible;	
}
