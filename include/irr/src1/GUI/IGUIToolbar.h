// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_TOOL_BAR_H_INCLUDED__
#define __I_GUI_TOOL_BAR_H_INCLUDED__
//#include "IGUIElement.h"
class ITexture;
class IGUIButton;
//! Stays at the top of its parent like the menu bar and contains tool buttons
class IGUIToolBar : public IGUIElement
{
public:
  //! constructor
  IGUIToolBar(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_TOOL_BAR, environment, parent, id, rectangle) {}
  //! Adds a button to the tool bar
  IGUIButton* addButton(s32 id = -1, const char* text = 0, const char* tooltiptext = 0,
      ITexture* img = 0, ITexture* pressedimg = 0,
      bool isPushButton = false, bool useAlphaChannel = false) = 0;
};
#endif

