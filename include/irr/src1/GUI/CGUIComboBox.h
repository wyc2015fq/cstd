// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_COMBO_BOX_H_INCLUDED__
#define __C_GUI_COMBO_BOX_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIComboBox.h"
//#include "IGUIStaticText.h"
//#include "irrString.h"
//#include "irrArray.h"
class IGUIButton;
class IGUIListBox;
//! Single line edit box for editing simple text.
class CGUIComboBox : public IGUIComboBox
{
public:
  //! constructor
  CGUIComboBox(IGUIEnvironment* environment, IGUIElement* parent,
      s32 id, IRECT rectangle);
  //! Returns amount of items in box
  u32 getItemCount() const;
  //! returns string of an item. the idx may be a value from 0 to itemCount-1
  const char* getItem(u32 idx) const;
  //! Returns item data of an item. the idx may be a value from 0 to itemCount-1
  u32 getItemData(u32 idx) const;
  //! Returns index based on item data
  s32 getIndexForItemData(u32 data) const;
  //! adds an item and returns the index of it
  u32 addItem(const char* text, u32 data);
  //! Removes an item from the combo box.
  void removeItem(u32 id);
  //! deletes all items in the combo box
  void clear();
  //! returns the text of the currently selected item
  const char* getText() const;
  //! returns id of selected item. returns -1 if no item is selected.
  s32 getSelected() const;
  //! sets the selected item. Set this to -1 if no item should be selected
  void setSelected(s32 idx);
  //! sets the text alignment of the text part
  void setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical);
  //! Set the maximal number of rows for the selection listbox
  void setMaxSelectionRows(u32 max);
  //! Get the maximimal number of rows for the selection listbox
  u32 getMaxSelectionRows() const;
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  void openCloseMenu();
  void sendSelectionChangedEvent();
  IGUIButton* ListButton;
  IGUIStaticText* SelectedText;
  IGUIListBox* ListBox;
  IGUIElement* LastFocus;
  struct SComboData {
    SComboData(const char* text, u32 data)
      : Name(text), Data(data) {}
    char* Name;
    u32 Data;
  };
  SComboData > Items;
  s32 Selected;
  EGUI_ALIGNMENT HAlign, VAlign;
  u32 MaxSelectionRows;
  bool HasFocus;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_COMBO_BOX_H_INCLUDED__

