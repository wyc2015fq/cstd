// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_FILE_LIST_H_INCLUDED__
#define __C_FILE_LIST_H_INCLUDED__
//#include "IFileList.h"
//#include "irrString.h"
//#include "irrArray.h"


//! An entry in a list of files, can be a folder or a file.
struct SFileListEntry {
  //! The == operator is provided so that CFileList can slowly search the list!
  bool operator ==(const struct SFileListEntry& other) const {
    if (IsDirectory != other.IsDirectory) {
      return false;
    }

    return FullName.equals_ignore_case(other.FullName);
  }
  //! The < operator is provided so that CFileList can sort and quickly search the list.
  bool operator <(const struct SFileListEntry& other) const {
    if (IsDirectory != other.IsDirectory) {
      return IsDirectory;
    }

    return FullName.lower_ignore_case(other.FullName);
  }
};
//! Implementation of a file list
class CFileList : public IFileList
{
public:
  // CFileList methods
  //! Constructor
  /** \param path The path of this file archive */
  CFileList(const char* path, bool ignoreCase, bool ignorePaths);
  //! Destructor
  ~CFileList();
  //! Add as a file or folder to the list
  /** \param fullPath The file name including path, up to the root of the file list.
  \param isDirectory True if this is a directory rather than a file.
  \param offset The offset where the file is stored in an archive
  \param size The size of the file in bytes.
  \param id The ID of the file in the archive which owns it */
  u32 addItem(const char* fullPath, u32 offset, u32 size, bool isDirectory, u32 id = 0);
  //! Sorts the file list. You should call this after adding any items to the file list
  void sort();
  //! Returns the amount of files in the filelist.
  u32 getFileCount() const;
  //! Gets the name of a file in the list, based on an index.
  const char* getFileName(u32 index) const;
  //! Gets the full name of a file in the list, path included, based on an index.
  const char* getFullFileName(u32 index) const;
  //! Returns the ID of a file in the file list, based on an index.
  u32 getID(u32 index) const;
  //! Returns true if the file is a directory
  bool isDirectory(u32 index) const;
  //! Returns the size of a file
  u32 getFileSize(u32 index) const;
  //! Returns the offest of a file
  u32 getFileOffset(u32 index) const;
  //! Searches for a file or folder within the list, returns the index
  s32 findFile(const char* filename, bool isFolder) const;
  //! Returns the base path of the file list
  const char* getPath() const;
protected:
  //! Ignore paths when adding or searching for files
  bool IgnorePaths;
  //! Ignore case when adding or searching for files
  bool IgnoreCase;
  //! Path to the file list
  char* Path;
  //! List of files
  SFileListEntry> Files;
};


#endif

