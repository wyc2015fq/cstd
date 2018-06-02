// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_STATIC_TEXT_H_INCLUDED__
#define __C_GUI_STATIC_TEXT_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIStaticText.h"
//#include "irrArray.h"
class CGUIStaticText : public IGUIStaticText
{
public:
  //! constructor
  CGUIStaticText(const char* text, bool border, IGUIEnvironment* environment,
      IGUIElement* parent, s32 id, const IRECT& rectangle,
      bool background = false);
  //! destructor
  ~CGUIStaticText();
  //! draws the element and its children
  void draw();
  //! Sets another skin independent font.
  void setOverrideFont(IGUIFont* font = 0);
  //! Gets the override font (if any)
  IGUIFont* getOverrideFont() const;
  //! Get the font which is used right now for drawing
  IGUIFont* getActiveFont() const;
  //! Sets another color for the text.
  void setOverrideColor(SColor color);
  //! Sets another color for the background.
  void setBackgroundColor(SColor color);
  //! Sets whether to draw the background
  void setDrawBackground(bool draw);
  //! Gets the background color
  SColor getBackgroundColor() const;
  //! Checks if background drawing is enabled
  bool isDrawBackgroundEnabled() const;
  //! Sets whether to draw the border
  void setDrawBorder(bool draw);
  //! Checks if border drawing is enabled
  bool isDrawBorderEnabled() const;
  //! Sets alignment mode for text
  void setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical);
  //! Gets the override color
  SColor getOverrideColor() const;
  //! Sets if the static text should use the overide color or the
  //! color in the gui skin.
  void enableOverrideColor(bool enable);
  //! Checks if an override color is enabled
  bool isOverrideColorEnabled() const;
  //! Set whether the text in this label should be clipped if it goes outside bounds
  void setTextRestrainedInside(bool restrainedInside);
  //! Checks if the text in this label should be clipped if it goes outside bounds
  bool isTextRestrainedInside() const;
  //! Enables or disables word wrap for using the static text as
  //! multiline text control.
  void setWordWrap(bool enable);
  //! Checks if word wrap is enabled
  bool isWordWrapEnabled() const;
  //! Sets the new caption of this element.
  void setText(const char* text);
  //! Returns the height of the text in pixels when it is drawn.
  s32 getTextHeight() const;
  //! Returns the width of the current text, in the current font
  s32 getTextWidth() const;
  //! Updates the absolute position, splits text if word wrap is enabled
  void updateAbsolutePosition();
  //! Set whether the string should be interpreted as right-to-left (RTL) text
  /** \note This component does not implement the Unicode bidi standard, the
  text of the component should be already RTL if you call this. The
  main difference when RTL is enabled is that the linebreaks for multiline
  elements are performed starting from the end.
  */
  void setRightToLeft(bool rtl);
  //! Checks if the text should be interpreted as right-to-left text
  bool isRightToLeft() const;
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  //! Breaks the single text line.
  void breakText();
  EGUI_ALIGNMENT HAlign, VAlign;
  bool Border;
  bool OverrideColorEnabled;
  bool OverrideBGColorEnabled;
  bool WordWrap;
  bool Background;
  bool RestrainTextInside;
  bool RightToLeft;
  SColor OverrideColor, BGColor;
  IGUIFont* OverrideFont;
  IGUIFont* LastBreakFont; // stored because: if skin changes, line break must be recalculated.
  char* > BrokenText;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif

