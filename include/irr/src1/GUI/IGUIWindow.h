// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_WINDOW_H_INCLUDED__
#define __I_GUI_WINDOW_H_INCLUDED__
//#include "IGUIElement.h"
class IGUIButton;
//! Default moveable window GUI element with border, caption and close icons.
/** \par This element can create the following events of type EGUI_EVENT_TYPE:
\li EGET_ELEMENT_CLOSED
*/
class IGUIWindow : public IGUIElement
{
public:
  //! constructor
  IGUIWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_WINDOW, environment, parent, id, rectangle) {}
  //! Returns pointer to the close button
  /** You can hide the button by calling setVisible(false) on the result. */
  IGUIButton* getCloseButton() = 0;
  //! Returns pointer to the minimize button
  /** You can hide the button by calling setVisible(false) on the result. */
  IGUIButton* getMinimizeButton() = 0;
  //! Returns pointer to the maximize button
  /** You can hide the button by calling setVisible(false) on the result. */
  IGUIButton* getMaximizeButton() = 0;
  //! Returns true if the window can be dragged with the mouse, false if not
  bool isDraggable() = 0;
  //! Sets whether the window can be dragged by the mouse
  void setDraggable(bool draggable) = 0;
  //! Set if the window background will be drawn
  void setDrawBackground(bool draw) = 0;
  //! Get if the window background will be drawn
  bool getDrawBackground() = 0;
  //! Set if the window titlebar will be drawn
  //! Note: If the background is not drawn, then the titlebar is automatically also not drawn
  void setDrawTitlebar(bool draw) = 0;
  //! Get if the window titlebar will be drawn
  bool getDrawTitlebar() = 0;
  //! Returns the rectangle of the drawable area (without border and without titlebar)
  /** The coordinates are given relative to the top-left position of the gui element.<br>
  So to get absolute positions you have to add the resulting rectangle to getAbsolutePosition().UpperLeftCorner.<br>
  To get it relative to the parent element you have to add the resulting rectangle to getRelativePosition().UpperLeftCorner.
  Beware that adding a menu will not change the clientRect as menus are own gui elements, so in that case you might want to subtract
  the menu area additionally. */
  IRECT getClientRect() = 0;
};
#endif

