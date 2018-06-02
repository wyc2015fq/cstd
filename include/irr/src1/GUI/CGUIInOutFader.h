// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_IN_OUT_FADER_H_INCLUDED__
#define __C_GUI_IN_OUT_FADER_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIInOutFader.h"
class CGUIInOutFader : public IGUIInOutFader
{
public:
  //! constructor
  CGUIInOutFader(IGUIEnvironment* environment, IGUIElement* parent,
      s32 id, IRECT rectangle);
  //! draws the element and its children
  void draw();
  //! Gets the color to fade out to or to fade in from.
  SColor getColor() const;
  //! Sets the color to fade out to or to fade in from.
  void setColor(SColor color);
  void setColor(SColor source, SColor dest);
  //! Starts the fade in process.
  void fadeIn(u32 time);
  //! Starts the fade out process.
  void fadeOut(u32 time);
  //! Returns if the fade in or out process is done.
  bool isReady() const;
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  enum EFadeAction {
    EFA_NOTHING = 0,
    EFA_FADE_IN,
    EFA_FADE_OUT
  };
  u32 StartTime;
  u32 EndTime;
  EFadeAction Action;
  SColor Color[2];
  SColor FullColor;
  SColor TransColor;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_IN_OUT_FADER_H_INCLUDED__

