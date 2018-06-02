// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_CHECKBOX_H_INCLUDED__
#define __I_GUI_CHECKBOX_H_INCLUDED__
//#include "IGUIElement.h"
//! GUI Check box interface.
/** \par This element can create the following events of type EGUI_EVENT_TYPE:
\li EGET_CHECKBOX_CHANGED
*/
class IGUICheckBox : public IGUIElement
{
public:
  //! constructor
  IGUICheckBox(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_CHECK_BOX, environment, parent, id, rectangle) {}
  //! Set if box is checked.
  void setChecked(bool checked) = 0;
  //! Returns true if box is checked.
  bool isChecked() = 0;
};
#endif

