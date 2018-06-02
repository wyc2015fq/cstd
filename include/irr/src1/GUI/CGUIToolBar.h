// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_TOOL_BAR_H_INCLUDED__
#define __C_GUI_TOOL_BAR_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIToolbar.h"
//! Stays at the top of its parent like the menu bar and contains tool buttons
class CGUIToolBar : public IGUIToolBar
{
public:
  //! constructor
  CGUIToolBar(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle);
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  //! Updates the absolute position.
  void updateAbsolutePosition();
  //! Adds a button to the tool bar
  IGUIButton* addButton(s32 id = -1, const char* text = 0, const char* tooltiptext = 0,
      ITexture* img = 0, ITexture* pressed = 0,
      bool isPushButton = false, bool useAlphaChannel = false);
private:
  s32 ButtonX;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif

