// Copyright (C) 2002-2012 Nikolaus Gebhardt
// Copyright (C) 2009-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_NPK_READER_H_INCLUDED__
#define __C_NPK_READER_H_INCLUDED__
#ifdef __IRR_COMPILE_WITH_NPK_ARCHIVE_LOADER_
//#include "IReadFile.h"
//#include "irrArray.h"
//#include "irrString.h"
//#include "IFileSystem.h"
//#include "CFileList.h"
//! File header containing location and size of the table of contents
struct SNPKHeader {
  // Don't change the order of these fields!  They must match the order stored on disk.
  c8 Tag[4];
  u32 Length;
  u32 Offset;
};
//! An entry in the NPK file's table of contents.
struct SNPKFileEntry {
  char* Name;
  u32 Offset;
  u32 Length;
};
// end
//! Archiveloader capable of loading Nebula Device 2 NPK Archives
class CArchiveLoaderNPK : public IArchiveLoader
{
public:
  //! Constructor
  CArchiveLoaderNPK(IFileSystem* fs);
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
  //! Returns the type of archive created by this loader
  E_FILE_ARCHIVE_TYPE getType() const {
    return EFAT_NPK;
  }
private:
  IFileSystem* FileSystem;
};
//! reads from NPK
class CNPKReader : public IFileArchive, CFileList
{
public:
  CNPKReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
  ~CNPKReader();
  // file archive methods
  //! return the id of the file Archive
  const char* getArchiveName() const {
    return File->getFileName();
  }
  //! opens a file by file name
  IReadFile* createAndOpenFile(const char* filename);
  //! opens a file by index
  IReadFile* createAndOpenFile(u32 index);
  //! returns the list of files
  const IFileList* getFileList() const;
  //! get the class Type
  E_FILE_ARCHIVE_TYPE getType() const {
    return EFAT_NPK;
  }
private:
  //! scans for a local header, returns false if the header is invalid
  bool scanLocalHeader();
  void readString(char* name);
  IReadFile* File;
};
#endif // __IRR_COMPILE_WITH_NPK_ARCHIVE_LOADER_
#endif // __C_NPK_READER_H_INCLUDED__

