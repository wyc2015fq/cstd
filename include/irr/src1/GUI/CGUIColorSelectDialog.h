// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_COLOR_SELECT_DIALOG_H_INCLUDED__
#define __C_GUI_COLOR_SELECT_DIALOG_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIColorSelectDialog.h"
//#include "IGUIButton.h"
//#include "IGUISpinBox.h"
//#include "IGUIImage.h"
//#include "irrArray.h"
class CGUIColorSelectDialog : public IGUIColorSelectDialog
{
public:
  //! constructor
  CGUIColorSelectDialog(const char* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id);
  //! destructor
  ~CGUIColorSelectDialog();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  SColor getColor();
  SColorHSL getColorHSL();
private:
  //! sends the event that the file has been selected.
  void sendSelectedEvent();
  //! sends the event that the file choose process has been canceld
  void sendCancelEvent();
  position2di DragStart;
  bool Dragging;
  IGUIButton* CloseButton;
  IGUIButton* OKButton;
  IGUIButton* CancelButton;
  IGUISpinBox* > Battery;
  struct SColorCircle {
    IGUIImage* Control;
    ITexture* Texture;
  };
  SColorCircle ColorRing;
  void buildColorRing(const dimension2du& dim, s32 supersample, const SColor& borderColor);
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_COLOR_SELECT_DIALOG_H_INCLUDED__

