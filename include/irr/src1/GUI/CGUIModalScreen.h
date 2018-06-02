// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_MODAL_SCREEN_H_INCLUDED__
#define __C_GUI_MODAL_SCREEN_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIElement.h"
class CGUIModalScreen : public IGUIElement
{
public:
  //! constructor
  CGUIModalScreen(IGUIEnvironment* environment, IGUIElement* parent, s32 id);
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! Removes a child.
  void removeChild(IGUIElement* child);
  //! Adds a child
  void addChild(IGUIElement* child);
  //! draws the element and its children
  void draw();
  //! Updates the absolute position.
  void updateAbsolutePosition();
  //! Modalscreen is not a typical element, but rather acts like a state for it's children.
  //! isVisible is overriden to give this a useful behavior, so that a modal will no longer
  //! be active when its parent is invisible or all its children are invisible.
  bool isVisible() const;
  //! Modals are infinite so every point is inside
  bool isPointInside(const position2di& point) const;
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
protected:
  bool canTakeFocus(IGUIElement* target) const;
private:
  u32 MouseDownTime;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif

