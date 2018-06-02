// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_TAB_CONTROL_H_INCLUDED__
#define __I_GUI_TAB_CONTROL_H_INCLUDED__
//#include "IGUIElement.h"
//#include "IGUISkin.h"
//! A tab-page, onto which other gui elements could be added.
/** IGUITab refers to the page itself, not to the tab in the tabbar of an IGUITabControl. */
class IGUITab : public IGUIElement
{
public:
  //! constructor
  IGUITab(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_TAB, environment, parent, id, rectangle) {}
  //! Returns zero based index of tab if in tabcontrol.
  /** Can be accessed later IGUITabControl::getTab() by this number.
    Note that this number can change when other tabs are inserted or removed .
  */
  s32 getNumber() = 0;
  //! sets if the tab should draw its background
  void setDrawBackground(bool draw = true) = 0;
  //! sets the color of the background, if it should be drawn.
  void setBackgroundColor(SColor c) = 0;
  //! returns true if the tab is drawing its background, false if not
  bool isDrawingBackground() = 0;
  //! returns the color of the background
  SColor getBackgroundColor() = 0;
  //! sets the color of the text
  void setTextColor(SColor c) = 0;
  //! gets the color of the text
  SColor getTextColor() = 0;
};
//! A standard tab control
/** \par This element can create the following events of type EGUI_EVENT_TYPE:
\li EGET_TAB_CHANGED
*/
class IGUITabControl : public IGUIElement
{
public:
  //! constructor
  IGUITabControl(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_TAB_CONTROL, environment, parent, id, rectangle) {}
  //! Adds a tab
  IGUITab* addTab(const char* caption, s32 id = -1) = 0;
  //! Insert the tab at the given index
  /** \return The tab on success or NULL on failure. */
  IGUITab* insertTab(s32 idx, const char* caption, s32 id = -1) = 0;
  //! Removes a tab from the tabcontrol
  void removeTab(s32 idx) = 0;
  //! Clears the tabcontrol removing all tabs
  void clear() = 0;
  //! Returns amount of tabs in the tabcontrol
  s32 getTabCount() = 0;
  //! Returns a tab based on zero based index
  /** \param idx: zero based index of tab. Is a value betwenn 0 and getTabcount()-1;
  \return Returns pointer to the Tab. Returns 0 if no tab
  is corresponding to this tab. */
  IGUITab* getTab(s32 idx) = 0;
  //! Brings a tab to front.
  /** \param idx: number of the tab.
  \return Returns true if successful. */
  bool setActiveTab(s32 idx) = 0;
  //! Brings a tab to front.
  /** \param tab: pointer to the tab.
  \return Returns true if successful. */
  bool setActiveTab(IGUITab* tab) = 0;
  //! Returns which tab is currently active
  s32 getActiveTab() = 0;
  //! get the the id of the tab at the given absolute coordinates
  /** \return The id of the tab or -1 when no tab is at those coordinates*/
  s32 getTabAt(s32 xpos, s32 ypos) = 0;
  //! Set the height of the tabs
  void setTabHeight(s32 height) = 0;
  //! Get the height of the tabs
  /** return Returns the height of the tabs */
  s32 getTabHeight() = 0;
  //! set the maximal width of a tab. Per default width is 0 which means "no width restriction".
  void setTabMaxWidth(s32 width) = 0;
  //! get the maximal width of a tab
  s32 getTabMaxWidth() = 0;
  //! Set the alignment of the tabs
  /** Use EGUIA_UPPERLEFT or EGUIA_LOWERRIGHT */
  void setTabVerticalAlignment(EGUI_ALIGNMENT alignment) = 0;
  //! Get the alignment of the tabs
  /** return Returns the alignment of the tabs */
  EGUI_ALIGNMENT getTabVerticalAlignment() = 0;
  //! Set the extra width added to tabs on each side of the text
  void setTabExtraWidth(s32 extraWidth) = 0;
  //! Get the extra width added to tabs on each side of the text
  /** return Returns the extra width of the tabs */
  s32 getTabExtraWidth() = 0;
};
#endif

