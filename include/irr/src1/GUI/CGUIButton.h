// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_BUTTON_H_INCLUDED__
#define __C_GUI_BUTTON_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIButton.h"
//#include "IGUISpriteBank.h"
class CGUIButton : public IGUIButton
{
public:
  //! constructor
  CGUIButton(IGUIEnvironment* environment, IGUIElement* parent,
      s32 id, IRECT rectangle, bool noclip = false);
  //! destructor
  ~CGUIButton();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  //! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
  void setOverrideFont(IGUIFont* font = 0);
  //! Gets the override font (if any)
  IGUIFont* getOverrideFont() const;
  //! Get the font which is used right now for drawing
  IGUIFont* getActiveFont() const;
  //! Sets an image which should be displayed on the button when it is in normal state.
  void setImage(ITexture* image = 0);
  //! Sets an image which should be displayed on the button when it is in normal state.
  void setImage(ITexture* image, const IRECT& pos);
  //! Sets an image which should be displayed on the button when it is in pressed state.
  void setPressedImage(ITexture* image = 0);
  //! Sets an image which should be displayed on the button when it is in pressed state.
  void setPressedImage(ITexture* image, const IRECT& pos);
  //! Sets the sprite bank used by the button
  void setSpriteBank(IGUISpriteBank* bank = 0);
  //! Sets the animated sprite for a specific button state
  /** \param index: Number of the sprite within the sprite bank, use -1 for no sprite
  \param state: State of the button to set the sprite for
  \param index: The sprite number from the current sprite bank
  \param color: The color of the sprite
  */
  void setSprite(EGUI_BUTTON_STATE state, s32 index,
      SColor color = SColor(255, 255, 255, 255), bool loop = false);
  //! Sets if the button should behave like a push button. Which means it
  //! can be in two states: Normal or Pressed. With a click on the button,
  //! the user can change the state of the button.
  void setIsPushButton(bool isPushButton = true);
  //! Checks whether the button is a push button
  bool isPushButton() const;
  //! Sets the pressed state of the button if this is a pushbutton
  void setPressed(bool pressed = true);
  //! Returns if the button is currently pressed
  bool isPressed() const;
  //! Sets if the button should use the skin to draw its border
  void setDrawBorder(bool border = true);
  //! Checks if the button face and border are being drawn
  bool isDrawingBorder() const;
  //! Sets if the alpha channel should be used for drawing images on the button (default is false)
  void setUseAlphaChannel(bool useAlphaChannel = true);
  //! Checks if the alpha channel should be used for drawing images on the button
  bool isAlphaChannelUsed() const;
  //! Sets if the button should scale the button images to fit
  void setScaleImage(bool scaleImage = true);
  //! Checks whether the button scales the used images
  bool isScalingImage() const;
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  struct ButtonSprite {
    s32 Index;
    SColor Color;
    bool Loop;
  };
  ButtonSprite ButtonSprites[EGBS_COUNT];
  IGUISpriteBank* SpriteBank;
  IGUIFont* OverrideFont;
  ITexture* Image;
  ITexture* PressedImage;
  IRECT ImageRect;
  IRECT PressedImageRect;
  u32 ClickTime, HoverTime, FocusTime;
  bool IsPushButton;
  bool Pressed;
  bool UseAlphaChannel;
  bool DrawBorder;
  bool ScaleImage;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_BUTTON_H_INCLUDED__

