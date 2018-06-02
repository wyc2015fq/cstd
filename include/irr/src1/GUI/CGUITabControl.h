// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_TAB_CONTROL_H_INCLUDED__
#define __C_GUI_TAB_CONTROL_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUITabControl.h"
//#include "irrArray.h"
//#include "IGUISkin.h"
class CGUITabControl;
class IGUIButton;
// A tab, onto which other gui elements could be added.
class CGUITab : public IGUITab
{
public:
  //! constructor
  CGUITab(s32 number, IGUIEnvironment* environment,
      IGUIElement* parent, const IRECT& rectangle,
      s32 id);
  //! destructor
  //virtual ~CGUITab();
  //! Returns number of this tab in tabcontrol. Can be accessed
  //! later IGUITabControl::getTab() by this number.
  s32 getNumber() const;
  //! Sets the number
  void setNumber(s32 n);
  //! draws the element and its children
  void draw();
  //! sets if the tab should draw its background
  void setDrawBackground(bool draw = true);
  //! sets the color of the background, if it should be drawn.
  void setBackgroundColor(SColor c);
  //! sets the color of the text
  void setTextColor(SColor c);
  //! returns true if the tab is drawing its background, false if not
  bool isDrawingBackground() const;
  //! returns the color of the background
  SColor getBackgroundColor() const;
  SColor getTextColor() const;
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
  //! only for internal use by CGUITabControl
  void refreshSkinColors();
private:
  s32 Number;
  SColor BackColor;
  bool OverrideTextColorEnabled;
  SColor TextColor;
  bool DrawBackground;
};
//! A standard tab control
class CGUITabControl : public IGUITabControl
{
public:
  //! destructor
  CGUITabControl(IGUIEnvironment* environment,
      IGUIElement* parent, const IRECT& rectangle,
      bool fillbackground = true, bool border = true, s32 id = -1);
  //! destructor
  ~CGUITabControl();
  //! Adds a tab
  IGUITab* addTab(const char* caption, s32 id = -1);
  //! Adds a tab that has already been created
  void addTab(CGUITab* tab);
  //! Insert the tab at the given index
  IGUITab* insertTab(s32 idx, const char* caption, s32 id = -1);
  //! Removes a tab from the tabcontrol
  void removeTab(s32 idx);
  //! Clears the tabcontrol removing all tabs
  void clear();
  //! Returns amount of tabs in the tabcontrol
  s32 getTabCount() const;
  //! Returns a tab based on zero based index
  IGUITab* getTab(s32 idx) const;
  //! Brings a tab to front.
  bool setActiveTab(s32 idx);
  //! Brings a tab to front.
  bool setActiveTab(IGUITab* tab);
  //! Returns which tab is currently active
  s32 getActiveTab() const;
  //! get the the id of the tab at the given absolute coordinates
  s32 getTabAt(s32 xpos, s32 ypos) const;
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  //! Removes a child.
  void removeChild(IGUIElement* child);
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Set the height of the tabs
  void setTabHeight(s32 height);
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
  //! Get the height of the tabs
  s32 getTabHeight() const;
  //! set the maximal width of a tab. Per default width is 0 which means "no width restriction".
  void setTabMaxWidth(s32 width);
  //! get the maximal width of a tab
  s32 getTabMaxWidth() const;
  //! Set the alignment of the tabs
  //! note: EGUIA_CENTER is not an option
  void setTabVerticalAlignment(EGUI_ALIGNMENT alignment);
  //! Get the alignment of the tabs
  EGUI_ALIGNMENT getTabVerticalAlignment() const;
  //! Set the extra width added to tabs on each side of the text
  void setTabExtraWidth(s32 extraWidth);
  //! Get the extra width added to tabs on each side of the text
  s32 getTabExtraWidth() const;
  //! Update the position of the element, decides scroll button status
  void updateAbsolutePosition();
private:
  void scrollLeft();
  void scrollRight();
  bool needScrollControl(s32 startIndex = 0, bool withScrollControl = false);
  s32 calcTabWidth(s32 pos, IGUIFont* font, const char* text, bool withScrollControl) const;
  IRECT calcTabPos();
  void recalculateScrollButtonPlacement();
  void recalculateScrollBar();
  void refreshSprites();
  CGUITab* > Tabs; // CGUITab* because we need setNumber (which is certainly not nice)
  s32 ActiveTab;
  bool Border;
  bool FillBackground;
  bool ScrollControl;
  s32 TabHeight;
  EGUI_ALIGNMENT VerticalAlignment;
  IGUIButton* UpButton;
  IGUIButton* DownButton;
  s32 TabMaxWidth;
  s32 CurrentScrollTabIndex;
  s32 TabExtraWidth;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif

