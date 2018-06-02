// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_SCROLL_BAR_H_INCLUDED__
#define __I_GUI_SCROLL_BAR_H_INCLUDED__
//#include "IGUIElement.h"
//! Default scroll bar GUI element.
/** \par This element can create the following events of type EGUI_EVENT_TYPE:
\li EGET_SCROLL_BAR_CHANGED
*/
class IGUIScrollBar : public IGUIElement
{
public:
  //! constructor
  IGUIScrollBar(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_SCROLL_BAR, environment, parent, id, rectangle) {}
  //! sets the maximum value of the scrollbar.
  void setMax(s32 max) = 0;
  //! gets the maximum value of the scrollbar.
  s32 getMax() = 0;
  //! sets the minimum value of the scrollbar.
  void setMin(s32 min) = 0;
  //! gets the minimum value of the scrollbar.
  s32 getMin() = 0;
  //! gets the small step value
  s32 getSmallStep() = 0;
  //! Sets the small step
  /** That is the amount that the value changes by when clicking
  on the buttons or using the cursor keys. */
  void setSmallStep(s32 step) = 0;
  //! gets the large step value
  s32 getLargeStep() = 0;
  //! Sets the large step
  /** That is the amount that the value changes by when clicking
  in the tray, or using the page up and page down keys. */
  void setLargeStep(s32 step) = 0;
  //! gets the current position of the scrollbar
  s32 getPos() = 0;
  //! sets the current position of the scrollbar
  void setPos(s32 pos) = 0;
};
#endif

