// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_CONTEXT_MENU_H_INCLUDED__
#define __C_GUI_CONTEXT_MENU_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIContextMenu.h"
//#include "irrString.h"
//#include "irrArray.h"
//#include "IGUIFont.h"
//! GUI Context menu interface.
class CGUIContextMenu : public IGUIContextMenu
{
public:
  //! constructor
  CGUIContextMenu(IGUIEnvironment* environment,
      IGUIElement* parent, s32 id, IRECT rectangle,
      bool getFocus = true, bool allowFocus = true);
  //! destructor
  ~CGUIContextMenu();
  //! set behavior when menus are closed
  void setCloseHandling(ECONTEXT_MENU_CLOSE onClose);
  //! get current behavior when the menue will be closed
  ECONTEXT_MENU_CLOSE getCloseHandling() const;
  //! Returns amount of menu items
  u32 getItemCount() const;
  //! Adds a menu item.
  u32 addItem(const char* text, s32 commandid,
      bool enabled, bool hasSubMenu, bool checked, bool autoChecking);
  //! Insert a menu item at specified position.
  u32 insertItem(u32 idx, const char* text, s32 commandId, bool enabled,
      bool hasSubMenu, bool checked, bool autoChecking);
  //! Find a item which has the given CommandId starting from given index
  s32 findItemWithCommandId(s32 commandId, u32 idxStartSearch) const;
  //! Adds a separator item to the menu
  void addSeparator();
  //! Returns text of the menu item.
  const char* getItemText(u32 idx) const;
  //! Sets text of the menu item.
  void setItemText(u32 idx, const char* text);
  //! Returns if a menu item is enabled
  bool isItemEnabled(u32 idx) const;
  //! Sets if the menu item should be enabled.
  void setItemEnabled(u32 idx, bool enabled);
  //! Returns if a menu item is checked
  bool isItemChecked(u32 idx) const;
  //! Sets if the menu item should be checked.
  void setItemChecked(u32 idx, bool enabled);
  //! Removes a menu item
  void removeItem(u32 idx);
  //! Removes all menu items
  void removeAllItems();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  //! Returns the selected item in the menu
  s32 getSelectedItem() const;
  //! Returns a pointer to the submenu of an item.
  //! \return Pointer to the submenu of an item.
  IGUIContextMenu* getSubMenu(u32 idx) const;
  //! Sets the visible state of this element.
  void setVisible(bool visible);
  //! should the element change the checked status on clicking
  void setItemAutoChecking(u32 idx, bool autoChecking);
  //! does the element change the checked status on clicking
  bool getItemAutoChecking(u32 idx) const;
  //! Returns command id of a menu item
  s32 getItemCommandId(u32 idx) const;
  //! Sets the command id of a menu item
  void setItemCommandId(u32 idx, s32 id);
  //! Adds a sub menu from an element that already exists.
  void setSubMenu(u32 index, CGUIContextMenu* menu);
  //! When an eventparent is set it receives events instead of the usual parent element
  void setEventParent(IGUIElement* parent);
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
protected:
  void closeAllSubMenus();
  bool hasOpenSubMenu() const;
  struct SItem {
    char* Text;
    bool IsSeparator;
    bool Enabled;
    bool Checked;
    bool AutoChecking;
    dimension2du Dim;
    s32 PosY;
    CGUIContextMenu* SubMenu;
    s32 CommandId;
  };
  void recalculateSize();
  //! returns true, if an element was highlighted
  bool highlight(const position2di& p, bool canOpenSubMenu);
  //! sends a click Returns:
  //! 0 if click went outside of the element,
  //! 1 if a valid button was clicked,
  //! 2 if a nonclickable element was clicked
  u32 sendClick(const position2di& p);
  //! returns the item highlight-area
  IRECT getHRect(const SItem& i, const IRECT& absolute) const;
  //! Gets drawing rect of Item
  IRECT getRect(const SItem& i, const IRECT& absolute) const;
  SItem > Items;
  position2di Pos;
  IGUIElement* EventParent;
  IGUIFont* LastFont;
  ECONTEXT_MENU_CLOSE CloseHandling;
  s32 HighLighted;
  u32 ChangeTime;
  bool AllowFocus;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_CONTEXT_MENU_H_INCLUDED__

