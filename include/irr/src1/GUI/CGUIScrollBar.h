// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_SCROLL_BAR_H_INCLUDED__
#define __C_GUI_SCROLL_BAR_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIScrollBar.h"
//#include "IGUIButton.h"
class CGUIScrollBar : public IGUIScrollBar
{
public:
  //! constructor
  CGUIScrollBar(bool horizontal, IGUIEnvironment* environment,
      IGUIElement* parent, s32 id, IRECT rectangle,
      bool noclip = false);
  //! destructor
  ~CGUIScrollBar();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  void OnPostRender(u32 timeMs);
  //! gets the maximum value of the scrollbar.
  s32 getMax() const;
  //! sets the maximum value of the scrollbar.
  void setMax(s32 max);
  //! gets the minimum value of the scrollbar.
  s32 getMin() const;
  //! sets the minimum value of the scrollbar.
  void setMin(s32 min);
  //! gets the small step value
  s32 getSmallStep() const;
  //! sets the small step value
  void setSmallStep(s32 step);
  //! gets the large step value
  s32 getLargeStep() const;
  //! sets the large step value
  void setLargeStep(s32 step);
  //! gets the current position of the scrollbar
  s32 getPos() const;
  //! sets the position of the scrollbar
  void setPos(s32 pos);
  //! updates the rectangle
  void updateAbsolutePosition();
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  void refreshControls();
  s32 getPosFromMousePos(const position2di& p) const;
  IGUIButton* UpButton;
  IGUIButton* DownButton;
  IRECT SliderRect;
  bool Dragging;
  bool Horizontal;
  bool DraggedBySlider;
  bool TrayClick;
  s32 Pos;
  s32 DrawPos;
  s32 DrawHeight;
  s32 Min;
  s32 Max;
  s32 SmallStep;
  s32 LargeStep;
  s32 DesiredPos;
  u32 LastChange;
  SColor CurrentIconColor;
  f32 range() const {
    return (f32)(Max - Min);
  }
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif

