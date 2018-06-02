// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_COMBO_BOX_H_INCLUDED__
#define __I_GUI_COMBO_BOX_H_INCLUDED__
//#include "IGUIElement.h"
//! Combobox widget
/** \par This element can create the following events of type EGUI_EVENT_TYPE:
\li EGET_COMBO_BOX_CHANGED
*/
class IGUIComboBox : public IGUIElement
{
public:
  //! constructor
  IGUIComboBox(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_COMBO_BOX, environment, parent, id, rectangle) {}
  //! Returns amount of items in box
  u32 getItemCount() = 0;
  //! Returns string of an item. the idx may be a value from 0 to itemCount-1
  const char* getItem(u32 idx) = 0;
  //! Returns item data of an item. the idx may be a value from 0 to itemCount-1
  u32 getItemData(u32 idx) = 0;
  //! Returns index based on item data
  s32 getIndexForItemData(u32 data) = 0;
  //! Adds an item and returns the index of it
  u32 addItem(const char* text, u32 data = 0) = 0;
  //! Removes an item from the combo box.
  /** Warning. This will change the index of all following items */
  void removeItem(u32 idx) = 0;
  //! Deletes all items in the combo box
  void clear() = 0;
  //! Returns id of selected item. returns -1 if no item is selected.
  s32 getSelected() = 0;
  //! Sets the selected item. Set this to -1 if no item should be selected
  void setSelected(s32 idx) = 0;
  //! Sets text justification of the text area
  /** \param horizontal: EGUIA_UPPERLEFT for left justified (default),
  EGUIA_LOWEERRIGHT for right justified, or EGUIA_CENTER for centered text.
  \param vertical: EGUIA_UPPERLEFT to align with top edge,
  EGUIA_LOWEERRIGHT for bottom edge, or EGUIA_CENTER for centered text (default). */
  void setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical) = 0;
  //! Set the maximal number of rows for the selection listbox
  void setMaxSelectionRows(u32 max) = 0;
  //! Get the maximimal number of rows for the selection listbox
  u32 getMaxSelectionRows() = 0;
};
#endif

