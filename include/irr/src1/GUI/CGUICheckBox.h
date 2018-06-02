// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_CHECKBOX_H_INCLUDED__
#define __C_GUI_CHECKBOX_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUICheckBox.h"
class CGUICheckBox : public IGUICheckBox
{
public:
  //! constructor
  CGUICheckBox(bool checked, IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle);
  //! set if box is checked
  void setChecked(bool checked);
  //! returns if box is checked
  bool isChecked() const;
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  u32 checkTime;
  bool Pressed;
  bool Checked;
};
#endif // __C_GUI_CHECKBOX_H_INCLUDED__
#endif // _IRR_COMPILE_WITH_GUI_

