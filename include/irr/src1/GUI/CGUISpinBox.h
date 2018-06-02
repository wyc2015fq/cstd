// Copyright (C) 2006-2012 Michael Zeilfelder
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_SPIN_BOX_H_INCLUDED__
#define __C_GUI_SPIN_BOX_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUISpinBox.h"
class IGUIEditBox;
class IGUIButton;
class CGUISpinBox : public IGUISpinBox
{
public:
  //! constructor
  CGUISpinBox(const char* text, bool border, IGUIEnvironment* environment,
      IGUIElement* parent, s32 id, const IRECT& rectangle);
  //! destructor
  ~CGUISpinBox();
  //! Access the edit box used in the spin control
  /** \param enable: If set to true, the override color, which can be set
  with IGUIEditBox::setOverrideColor is used, otherwise the
  EGDC_BUTTON_TEXT color of the skin. */
  IGUIEditBox* getEditBox() const;
  //! set the current value of the spinbox
  /** \param val: value to be set in the spinbox */
  void setValue(f32 val);
  //! Get the current value of the spinbox
  f32 getValue() const;
  //! set the range of values which can be used in the spinbox
  /** \param min: minimum value
  \param max: maximum value */
  void setRange(f32 min, f32 max);
  //! get the minimum value which can be used in the spinbox
  f32 getMin() const;
  //! get the maximum value which can be used in the spinbox
  f32 getMax() const;
  //! step size by which values are changed when pressing the spin buttons
  /** \param step: stepsize used for value changes when pressing spin buttons */
  void setStepSize(f32 step = 1.f);
  //! returns the step size
  f32 getStepSize() const;
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! Draws the element and its children.
  void draw();
  //! Sets the new caption of the element
  void setText(const char* text);
  //! Returns caption of this element.
  const char* getText() const;
  //! Sets the number of decimal places to display.
  //! Note that this also rounds the range to the same number of decimal places.
  /** \param places: The number of decimal places to display, use -1 to reset */
  void setDecimalPlaces(s32 places);
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
protected:
  void verifyValueRange();
  void refreshSprites();
  IGUIEditBox* EditBox;
  IGUIButton* ButtonSpinUp;
  IGUIButton* ButtonSpinDown;
  SColor CurrentIconColor;
  f32 StepSize;
  f32 RangeMin;
  f32 RangeMax;
  char* FormatString;
  s32 DecimalPlaces;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_SPIN_BOX_H_INCLUDED__

