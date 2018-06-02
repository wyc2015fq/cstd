// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_MENU_H_INCLUDED__
#define __C_GUI_MENU_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "CGUIContextMenu.h"
//! GUI menu interface.
class CGUIMenu : public CGUIContextMenu
{
public:
  //! constructor
  CGUIMenu(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle);
  //! draws the element and its children
  void draw();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! Updates the absolute position.
  void updateAbsolutePosition();
protected:
  void recalculateSize();
  //! returns the item highlight-area
  IRECT getHRect(const SItem& i, const IRECT& absolute) const;
  //! Gets drawing rect of Item
  IRECT getRect(const SItem& i, const IRECT& absolute) const;
};
#endif //  __C_GUI_MENU_H_INCLUDED__
#endif // _IRR_COMPILE_WITH_GUI_

