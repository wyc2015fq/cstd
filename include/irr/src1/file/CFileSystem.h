// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_FILE_SYSTEM_H_INCLUDED__
#define __C_FILE_SYSTEM_H_INCLUDED__
//#include "IFileSystem.h"
//#include "irrArray.h"
class CZipReader;
class CPakReader;
class CMountPointReader;
/*!
  FileSystem which uses normal files and one zipfile
*/
class CFileSystem : public IFileSystem
{
public:
  //! constructor
  CFileSystem();
  //! destructor
  ~CFileSystem();
  //! opens a file for read access
  IReadFile* createAndOpenFile(const char* filename);
  //! Creates an IReadFile interface for accessing memory like a file.
  IReadFile* createMemoryReadFile(void* memory, s32 len, const char* fileName, bool deleteMemoryWhenDropped = false);
  //! Creates an IReadFile interface for accessing files inside files
  IReadFile* createLimitReadFile(const char* fileName, IReadFile* alreadyOpenedFile, long pos, long areaSize);
  //! Creates an IWriteFile interface for accessing memory like a file.
  IWriteFile* createMemoryWriteFile(void* memory, s32 len, const char* fileName, bool deleteMemoryWhenDropped = false);
  //! Opens a file for write access.
  IWriteFile* createAndWriteFile(const char* filename, bool append = false);
  //! Adds an archive to the file system.
  bool addFileArchive(const char* filename,
      bool ignoreCase = true, bool ignorePaths = true,
      E_FILE_ARCHIVE_TYPE archiveType = EFAT_UNKNOWN,
      const char* password = "",
      IFileArchive** retArchive = 0);
  //! Adds an archive to the file system.
  bool addFileArchive(IReadFile* file, bool ignoreCase = true,
      bool ignorePaths = true,
      E_FILE_ARCHIVE_TYPE archiveType = EFAT_UNKNOWN,
      const char* password = "",
      IFileArchive** retArchive = 0);
  //! Adds an archive to the file system.
  bool addFileArchive(IFileArchive* archive);
  //! move the hirarchy of the filesystem. moves sourceIndex relative up or down
  bool moveFileArchive(u32 sourceIndex, s32 relative);
  //! Adds an external archive loader to the engine.
  void addArchiveLoader(IArchiveLoader* loader);
  //! Returns the total number of archive loaders added.
  u32 getArchiveLoaderCount() const;
  //! Gets the archive loader by index.
  IArchiveLoader* getArchiveLoader(u32 index) const;
  //! gets the file archive count
  u32 getFileArchiveCount() const;
  //! gets an archive
  IFileArchive* getFileArchive(u32 index);
  //! removes an archive from the file system.
  bool removeFileArchive(u32 index);
  //! removes an archive from the file system.
  bool removeFileArchive(const char* filename);
  //! Removes an archive from the file system.
  bool removeFileArchive(const IFileArchive* archive);
  //! Returns the string of the current working directory
  const char* getWorkingDirectory();
  //! Changes the current Working Directory to the string given.
  //! The string is operating system dependent. Under Windows it will look
  //! like this: "drive:\directory\sudirectory\"
  bool changeWorkingDirectoryTo(const char* newDirectory);
  //! Converts a relative path to an absolute (unique) path, resolving symbolic links
  char* getAbsolutePath(const char* filename) const;
  //! Returns the directory a file is located in.
  /** \param filename: The file to get the directory from */
  char* getFileDir(const char* filename) const;
  //! Returns the base part of a filename, i.e. the name without the directory
  //! part. If no directory is prefixed, the full name is returned.
  /** \param filename: The file to get the basename from */
  char* getFileBasename(const char* filename, bool keepExtension = true) const;
  //! flatten a path and file name for example: "/you/me/../." becomes "/you"
  char* flattenFilename(char* directory, const char* root = "/") const;
  //! Get the relative filename, relative to the given directory
  char* getRelativeFilename(const char* filename, const char* directory) const;
  EFileSystemType setFileListSystem(EFileSystemType listType);
  //! Creates a list of files and directories in the current working directory
  //! and returns it.
  IFileList* createFileList();
  //! Creates an empty filelist
  IFileList* createEmptyFileList(const char* path, bool ignoreCase, bool ignorePaths);
  //! determines if a file exists and would be able to be opened.
  bool existFile(const char* filename) const;
  //! Creates a XML Reader from a file.
  IXMLReader* createXMLReader(const char* filename);
  //! Creates a XML Reader from a file.
  IXMLReader* createXMLReader(IReadFile* file);
  //! Creates a XML Reader from a file.
  IXMLReaderUTF8* createXMLReaderUTF8(const char* filename);
  //! Creates a XML Reader from a file.
  IXMLReaderUTF8* createXMLReaderUTF8(IReadFile* file);
  //! Creates a XML Writer from a file.
  IXMLWriter* createXMLWriter(const char* filename);
  //! Creates a XML Writer from a file.
  IXMLWriter* createXMLWriter(IWriteFile* file);
  //! Creates a new empty collection of attributes, usable for serialization and more.
  IAttributes* createEmptyAttributes(IVideoDriver* driver);
private:
  // don't expose, needs refactoring
  bool changeArchivePassword(const char* filename,
      const char* password,
      IFileArchive** archive = 0);
  //! Currently used FileSystemType
  EFileSystemType FileSystemType;
  //! WorkingDirectory for Native and Virtual filesystems
  char* WorkingDirectory [2];
  //! currently attached ArchiveLoaders
  IArchiveLoader* > ArchiveLoader;
  //! currently attached Archives
  IFileArchive* > FileArchives;
};
#endif

