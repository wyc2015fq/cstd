// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_SKIN_H_INCLUDED__
#define __C_GUI_SKIN_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUISkin.h"
//#include "irrString.h"
class IVideoDriver;
}
class CGUISkin : public IGUISkin
{
public:
  CGUISkin(EGUI_SKIN_TYPE type, IVideoDriver* driver);
  //! destructor
  ~CGUISkin();
  //! returns default color
  SColor getColor(EGUI_DEFAULT_COLOR color) const;
  //! sets a default color
  void setColor(EGUI_DEFAULT_COLOR which, SColor newColor);
  //! returns size for the given size type
  s32 getSize(EGUI_DEFAULT_SIZE size) const;
  //! sets a default size
  void setSize(EGUI_DEFAULT_SIZE which, s32 size);
  //! returns the default font
  IGUIFont* getFont(EGUI_DEFAULT_FONT which = EGDF_DEFAULT) const;
  //! sets a default font
  void setFont(IGUIFont* font, EGUI_DEFAULT_FONT which = EGDF_DEFAULT);
  //! sets the sprite bank used for drawing icons
  void setSpriteBank(IGUISpriteBank* bank);
  //! gets the sprite bank used for drawing icons
  IGUISpriteBank* getSpriteBank() const;
  //! Returns a default icon
  /** Returns the sprite index within the sprite bank */
  u32 getIcon(EGUI_DEFAULT_ICON icon) const;
  //! Sets a default icon
  /** Sets the sprite index used for drawing icons like arrows,
  close buttons and ticks in checkboxes
  \param icon: Enum specifying which icon to change
  \param index: The sprite index used to draw this icon */
  void setIcon(EGUI_DEFAULT_ICON icon, u32 index);
  //! Returns a default text.
  /** For example for Message box button captions:
  "OK", "Cancel", "Yes", "No" and so on. */
  const char* getDefaultText(EGUI_DEFAULT_TEXT text) const;
  //! Sets a default text.
  /** For example for Message box button captions:
  "OK", "Cancel", "Yes", "No" and so on. */
  void setDefaultText(EGUI_DEFAULT_TEXT which, const char* newText);
  //! draws a standard 3d button pane
  /** Used for drawing for example buttons in normal state.
  It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
  EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
  \param rect: Defining area where to draw.
  \param clip: Clip area.
  \param element: Pointer to the element which wishes to draw this. This parameter
  is usually not used by ISkin, but can be used for example by more complex
  implementations to find out how to draw the part exactly. */
  void draw3DButtonPaneStandard(IGUIElement* element,
      const IRECT& rect,
      const IRECT* clip = 0);
  //! draws a pressed 3d button pane
  /** Used for drawing for example buttons in pressed state.
  It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
  EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
  \param rect: Defining area where to draw.
  \param clip: Clip area.
  \param element: Pointer to the element which wishes to draw this. This parameter
  is usually not used by ISkin, but can be used for example by more complex
  implementations to find out how to draw the part exactly. */
  void draw3DButtonPanePressed(IGUIElement* element,
      const IRECT& rect,
      const IRECT* clip = 0);
  //! draws a sunken 3d pane
  /** Used for drawing the background of edit, combo or check boxes.
  \param element: Pointer to the element which wishes to draw this. This parameter
  is usually not used by ISkin, but can be used for example by more complex
  implementations to find out how to draw the part exactly.
  \param bgcolor: Background color.
  \param flat: Specifies if the sunken pane should be flat or displayed as sunken
  deep into the ground.
  \param rect: Defining area where to draw.
  \param clip: Clip area. */
  void draw3DSunkenPane(IGUIElement* element,
      SColor bgcolor, bool flat,
      bool fillBackGround,
      const IRECT& rect,
      const IRECT* clip = 0);
  //! draws a window background
  /** Used for drawing the background of dialogs and windows.
  \param element: Pointer to the element which wishes to draw this. This parameter
  is usually not used by ISkin, but can be used for example by more complex
  implementations to find out how to draw the part exactly.
  \param titleBarColor: Title color.
  \param drawTitleBar: True to enable title drawing.
  \param rect: Defining area where to draw.
  \param clip: Clip area.
  \param checkClientArea: When set to non-null the function will not draw anything,
  but will instead return the clientArea which can be used for drawing by the calling window.
  That is the area without borders and without titlebar.
  \return Returns rect where it would be good to draw title bar text. This will
  work even when checkClientArea is set to a non-null value.*/
  IRECT draw3DWindowBackground(IGUIElement* element,
      bool drawTitleBar, SColor titleBarColor,
      const IRECT& rect,
      const IRECT* clip,
      IRECT* checkClientArea);
  //! draws a standard 3d menu pane
  /** Used for drawing for menus and context menus.
  It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
  EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
  \param element: Pointer to the element which wishes to draw this. This parameter
  is usually not used by ISkin, but can be used for example by more complex
  implementations to find out how to draw the part exactly.
  \param rect: Defining area where to draw.
  \param clip: Clip area. */
  void draw3DMenuPane(IGUIElement* element,
      const IRECT& rect,
      const IRECT* clip = 0);
  //! draws a standard 3d tool bar
  /** Used for drawing for toolbars and menus.
  \param element: Pointer to the element which wishes to draw this. This parameter
  is usually not used by ISkin, but can be used for example by more complex
  implementations to find out how to draw the part exactly.
  \param rect: Defining area where to draw.
  \param clip: Clip area. */
  void draw3DToolBar(IGUIElement* element,
      const IRECT& rect,
      const IRECT* clip = 0);
  //! draws a tab button
  /** Used for drawing for tab buttons on top of tabs.
  \param element: Pointer to the element which wishes to draw this. This parameter
  is usually not used by ISkin, but can be used for example by more complex
  implementations to find out how to draw the part exactly.
  \param active: Specifies if the tab is currently active.
  \param rect: Defining area where to draw.
  \param clip: Clip area. */
  void draw3DTabButton(IGUIElement* element, bool active,
      const IRECT& rect, const IRECT* clip = 0, EGUI_ALIGNMENT alignment = EGUIA_UPPERLEFT);
  //! draws a tab control body
  /** \param element: Pointer to the element which wishes to draw this. This parameter
  is usually not used by ISkin, but can be used for example by more complex
  implementations to find out how to draw the part exactly.
  \param border: Specifies if the border should be drawn.
  \param background: Specifies if the background should be drawn.
  \param rect: Defining area where to draw.
  \param clip: Clip area. */
  void draw3DTabBody(IGUIElement* element, bool border, bool background,
      const IRECT& rect, const IRECT* clip = 0, s32 tabHeight = -1, EGUI_ALIGNMENT alignment = EGUIA_UPPERLEFT);
  //! draws an icon, usually from the skin's sprite bank
  /** \param element: Pointer to the element which wishes to draw this icon.
  This parameter is usually not used by IGUISkin, but can be used for example
  by more complex implementations to find out how to draw the part exactly.
  \param icon: Specifies the icon to be drawn.
  \param position: The position to draw the icon
  \param starttime: The time at the start of the animation
  \param currenttime: The present time, used to calculate the frame number
  \param loop: Whether the animation should loop or not
  \param clip: Clip area. */
  void drawIcon(IGUIElement* element, EGUI_DEFAULT_ICON icon,
      const position2di position,
      u32 starttime = 0, u32 currenttime = 0,
      bool loop = false, const IRECT* clip = 0);
  //! draws a 2d rectangle.
  /** \param element: Pointer to the element which wishes to draw this icon.
  This parameter is usually not used by IGUISkin, but can be used for example
  by more complex implementations to find out how to draw the part exactly.
  \param color: Color of the rectangle to draw. The alpha component specifies how
  transparent the rectangle will be.
  \param pos: Position of the rectangle.
  \param clip: Pointer to rectangle against which the rectangle will be clipped.
  If the pointer is null, no clipping will be performed. */
  void draw2DRectangle(IGUIElement* element, const SColor& color,
      const IRECT& pos, const IRECT* clip = 0);
  //! get the type of this skin
  EGUI_SKIN_TYPE getType() const;
  //! Writes attributes of the object.
  //! Implement this to expose the attributes of your scene node animator for
  //! scripting languages, editors, debuggers or xml serialization purposes.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the object.
  //! Implement this to set the attributes of your scene node animator for
  //! scripting languages, editors, debuggers or xml deserialization purposes.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
private:
  SColor Colors[EGDC_COUNT];
  s32 Sizes[EGDS_COUNT];
  u32 Icons[EGDI_COUNT];
  IGUIFont* Fonts[EGDF_COUNT];
  IGUISpriteBank* SpriteBank;
  char* Texts[EGDT_COUNT];
  IVideoDriver* Driver;
  bool UseGradient;
  EGUI_SKIN_TYPE Type;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif

