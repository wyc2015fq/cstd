// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_MOUNT_READER_H_INCLUDED__
#define __C_MOUNT_READER_H_INCLUDED__
#ifdef __IRR_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_
//#include "IFileSystem.h"
//#include "CFileList.h"
//! Archiveloader capable of loading MountPoint Archives
class CArchiveLoaderMount : public IArchiveLoader
{
public:
  //! Constructor
  CArchiveLoaderMount(IFileSystem* fs);
  //! returns true if the file maybe is able to be loaded by this class
  //! based on the file extension (e.g. ".zip")
  bool isALoadableFileFormat(const char* filename) const;
  //! Check if the file might be loaded by this class
  /** Check might look into the file.
  \param file File handle to check.
  \return True if file seems to be loadable. */
  bool isALoadableFileFormat(IReadFile* file) const;
  //! Check to see if the loader can create archives of this type.
  /** Check based on the archive type.
  \param fileType The archive type to check.
  \return True if the archile loader supports this type, false if not */
  bool isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const;
  //! Creates an archive from the filename
  /** \param file File handle to check.
  \return Pointer to newly created archive, or 0 upon error. */
  IFileArchive* createArchive(const char* filename, bool ignoreCase, bool ignorePaths) const;
  //! creates/loads an archive from the file.
  //! \return Pointer to the created archive. Returns 0 if loading failed.
  IFileArchive* createArchive(IReadFile* file, bool ignoreCase, bool ignorePaths) const;
private:
  IFileSystem* FileSystem;
};
//! A File Archive which uses a mountpoint
class CMountPointReader : public IFileArchive, CFileList
{
public:
  //! Constructor
  CMountPointReader(IFileSystem* parent, const char* basename,
      bool ignoreCase, bool ignorePaths);
  //! opens a file by index
  IReadFile* createAndOpenFile(u32 index);
  //! opens a file by file name
  IReadFile* createAndOpenFile(const char* filename);
  //! returns the list of files
  const IFileList* getFileList() const;
  //! get the class Type
  E_FILE_ARCHIVE_TYPE getType() const {
    return EFAT_FOLDER;
  }
private:
  strv_t RealFileNames;
  IFileSystem* Parent;
  void buildDirectory();
};
} // io
} // irr
#endif // __IRR_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_
#endif // __C_MOUNT_READER_H_INCLUDED__

