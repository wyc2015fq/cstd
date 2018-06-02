// Copyright (C) 2002-2012 Nikolaus Gebhardt/ Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_FILE_ARCHIVE_H_INCLUDED__
#define __I_FILE_ARCHIVE_H_INCLUDED__
//#include "IReadFile.h"
//#include "IFileList.h"
//! FileSystemType: which Filesystem should be used for e.g. browsing
enum EFileSystemType {
  FILESYSTEM_NATIVE = 0,  // Native OS FileSystem
  FILESYSTEM_VIRTUAL  // Virtual FileSystem
};
//! Contains the different types of archives
enum E_FILE_ARCHIVE_TYPE {
  //! A PKZIP archive
  EFAT_ZIP     = MAKE_IRR_ID('Z', 'I', 'P', 0),
  //! A gzip archive
  EFAT_GZIP    = MAKE_IRR_ID('g', 'z', 'i', 'p'),
  //! A directory
  EFAT_FOLDER  = MAKE_IRR_ID('f', 'l', 'd', 'r'),
  //! An ID Software PAK archive
  EFAT_PAK     = MAKE_IRR_ID('P', 'A', 'K', 0),
  //! A Nebula Device archive
  EFAT_NPK     = MAKE_IRR_ID('N', 'P', 'K', 0),
  //! A Tape ARchive
  EFAT_TAR     = MAKE_IRR_ID('T', 'A', 'R', 0),
  //! A wad Archive, Quake2, Halflife
  EFAT_WAD     = MAKE_IRR_ID('W', 'A', 'D', 0),
  //! The type of this archive is unknown
  EFAT_UNKNOWN = MAKE_IRR_ID('u', 'n', 'k', 'n')
};
//! The FileArchive manages archives and provides access to files inside them.
class IFileArchive
{
public:
  //! Opens a file based on its name
  /** Creates and returns a new IReadFile for a file in the archive.
  \param filename The file to open
  \return Returns A pointer to the created file on success,
  or 0 on failure. */
  IReadFile* createAndOpenFile(const char* filename) = 0;
  //! Opens a file based on its position in the file list.
  /** Creates and returns
  \param index The zero based index of the file.
  \return Returns a pointer to the created file on success, or 0 on failure. */
  IReadFile* createAndOpenFile(u32 index) = 0;
  //! Returns the complete file tree
  /** \return Returns the complete directory tree for the archive,
  including all files and folders */
  const IFileList* getFileList() = 0;
  //! get the archive type
  E_FILE_ARCHIVE_TYPE getType() const {
    return EFAT_UNKNOWN;
  }
  //! An optionally used password string
  /** This variable is publicly accessible from the interface in order to
  avoid single access patterns to this place, and hence allow some more
  obscurity.
  */
  char Password[256];
};
//! Class which is able to create an archive from a file.
/** If you want the Irrlicht Engine be able to load archives of
currently unsupported file formats (e.g .wad), then implement
this and add your new Archive loader with
IFileSystem::addArchiveLoader() to the engine. */
class IArchiveLoader
{
public:
  //! Check if the file might be loaded by this class
  /** Check based on the file extension (e.g. ".zip")
  \param filename Name of file to check.
  \return True if file seems to be loadable. */
  bool isALoadableFileFormat(const char* filename) = 0;
  //! Check if the file might be loaded by this class
  /** This check may look into the file.
  \param file File handle to check.
  \return True if file seems to be loadable. */
  bool isALoadableFileFormat(IReadFile* file) = 0;
  //! Check to see if the loader can create archives of this type.
  /** Check based on the archive type.
  \param fileType The archive type to check.
  \return True if the archile loader supports this type, false if not */
  bool isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) = 0;
  //! Creates an archive from the filename
  /** \param filename File to use.
  \param ignoreCase Searching is performed without regarding the case
  \param ignorePaths Files are searched for without checking for the directories
  \return Pointer to newly created archive, or 0 upon error. */
  IFileArchive* createArchive(const char* filename, bool ignoreCase, bool ignorePaths) = 0;
  //! Creates an archive from the file
  /** \param file File handle to use.
  \param ignoreCase Searching is performed without regarding the case
  \param ignorePaths Files are searched for without checking for the directories
  \return Pointer to newly created archive, or 0 upon error. */
  IFileArchive* createArchive(IReadFile* file, bool ignoreCase, bool ignorePaths) = 0;
};
#endif

