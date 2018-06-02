// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_MESSAGE_BOX_H_INCLUDED__
#define __C_GUI_MESSAGE_BOX_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "CGUIWindow.h"
//#include "IGUIStaticText.h"
//#include "IGUIImage.h"
//#include "irrArray.h"
class CGUIMessageBox : public CGUIWindow
{
public:
  //! constructor
  CGUIMessageBox(IGUIEnvironment* environment, const char* caption,
      const char* text, s32 flag,
      IGUIElement* parent, s32 id, IRECT rectangle, ITexture* image = 0);
  //! destructor
  ~CGUIMessageBox();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  void refreshControls();
  void setButton(IGUIButton* button, bool isAvailable, const IRECT& btnRect, const char* text, IGUIElement* focusMe);
  IGUIButton* OkButton;
  IGUIButton* CancelButton;
  IGUIButton* YesButton;
  IGUIButton* NoButton;
  IGUIStaticText* StaticText;
  IGUIImage* Icon;
  ITexture* IconTexture;
  s32 Flags;
  char* MessageText;
  bool Pressed;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif

