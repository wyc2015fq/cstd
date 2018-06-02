// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_WINDOW_H_INCLUDED__
#define __C_GUI_WINDOW_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIWindow.h"
class IGUIButton;
class CGUIWindow : public IGUIWindow
{
public:
  //! constructor
  CGUIWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle);
  //! destructor
  ~CGUIWindow();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! update absolute position
  void updateAbsolutePosition();
  //! draws the element and its children
  void draw();
  //! Returns pointer to the close button
  IGUIButton* getCloseButton() const;
  //! Returns pointer to the minimize button
  IGUIButton* getMinimizeButton() const;
  //! Returns pointer to the maximize button
  IGUIButton* getMaximizeButton() const;
  //! Returns true if the window is draggable, false if not
  bool isDraggable() const;
  //! Sets whether the window is draggable
  void setDraggable(bool draggable);
  //! Set if the window background will be drawn
  void setDrawBackground(bool draw);
  //! Get if the window background will be drawn
  bool getDrawBackground() const;
  //! Set if the window titlebar will be drawn
  //! Note: If the background is not drawn, then the titlebar is automatically also not drawn
  void setDrawTitlebar(bool draw);
  //! Get if the window titlebar will be drawn
  bool getDrawTitlebar() const;
  //! Returns the rectangle of the drawable area (without border and without titlebar)
  IRECT getClientRect() const;
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
protected:
  void updateClientRect();
  void refreshSprites();
  IGUIButton* CloseButton;
  IGUIButton* MinButton;
  IGUIButton* RestoreButton;
  IRECT ClientRect;
  SColor CurrentIconColor;
  position2di DragStart;
  bool Dragging, IsDraggable;
  bool DrawBackground;
  bool DrawTitlebar;
  bool IsActive;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif

