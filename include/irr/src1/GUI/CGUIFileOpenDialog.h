// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__
#define __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIFileOpenDialog.h"
//#include "IGUIButton.h"
//#include "IGUIListBox.h"
//#include "IGUIEditBox.h"
//#include "IFileSystem.h"
class CGUIFileOpenDialog : public IGUIFileOpenDialog
{
public:
  //! constructor
  CGUIFileOpenDialog(const char* title, IGUIEnvironment* environment,
      IGUIElement* parent, s32 id, bool restoreCWD = false,
      char*::char_type* startDir = 0);
  //! destructor
  ~CGUIFileOpenDialog();
  //! returns the filename of the selected file. Returns NULL, if no file was selected.
  const char* getFileName() const;
  //! Returns the directory of the selected file. Returns NULL, if no directory was selected.
  const char* getDirectoryName();
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
protected:
  //! fills the listbox with files.
  void fillListBox();
  //! sends the event that the file has been selected.
  void sendSelectedEvent(EGUI_EVENT_TYPE type);
  //! sends the event that the file choose process has been canceld
  void sendCancelEvent();
  position2di DragStart;
  char* FileName;
  char* FileDirectory;
  char* RestoreDirectory;
  char* StartDirectory;
  IGUIButton* CloseButton;
  IGUIButton* OKButton;
  IGUIButton* CancelButton;
  IGUIListBox* FileBox;
  IGUIEditBox* FileNameText;
  IGUIElement* EventParent;
  IFileSystem* FileSystem;
  IFileList* FileList;
  bool Dragging;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__

