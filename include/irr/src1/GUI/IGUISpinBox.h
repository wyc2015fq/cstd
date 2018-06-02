// Copyright (C) 2006-2012 Michael Zeilfelder
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_SPIN_BOX_H_INCLUDED__
#define __I_GUI_SPIN_BOX_H_INCLUDED__
//#include "IGUIElement.h"
class IGUIEditBox;
//! Single line edit box + spin buttons
/** \par This element can create the following events of type EGUI_EVENT_TYPE:
\li EGET_SPINBOX_CHANGED
*/
class IGUISpinBox : public IGUIElement
{
public:
  //! constructor
  IGUISpinBox(IGUIEnvironment* environment, IGUIElement* parent,
      s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_SPIN_BOX, environment, parent, id, rectangle) {}
  //! Access the edit box used in the spin control
  IGUIEditBox* getEditBox() = 0;
  //! set the current value of the spinbox
  /** \param val: value to be set in the spinbox */
  void setValue(f32 val) = 0;
  //! Get the current value of the spinbox
  f32 getValue() = 0;
  //! set the range of values which can be used in the spinbox
  /** \param min: minimum value
  \param max: maximum value */
  void setRange(f32 min, f32 max) = 0;
  //! get the minimum value which can be used in the spinbox
  f32 getMin() = 0;
  //! get the maximum value which can be used in the spinbox
  f32 getMax() = 0;
  //! Step size by which values are changed when pressing the spinbuttons
  /** The step size also determines the number of decimal places to display
  \param step: stepsize used for value changes when pressing spinbuttons */
  void setStepSize(f32 step = 1.f) = 0;
  //! Sets the number of decimal places to display.
  //! Note that this also rounds the range to the same number of decimal places.
  /** \param places: The number of decimal places to display, use -1 to reset */
  void setDecimalPlaces(s32 places) = 0;
  //! get the current step size
  f32 getStepSize() = 0;
};
#endif // __I_GUI_SPIN_BOX_H_INCLUDED__

