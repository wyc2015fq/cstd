// Copyright (C) 2002-2012 Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_WAD_READER_H_INCLUDED__
#define __C_WAD_READER_H_INCLUDED__
#ifdef __IRR_COMPILE_WITH_WAD_ARCHIVE_LOADER_
//#include "IReadFile.h"
//#include "irrArray.h"
//#include "irrString.h"
//#include "IFileSystem.h"
//#include "CFileList.h"
enum eWADFileTypes {
  WAD_FORMAT_UNKNOWN  = 0,
  WAD_FORMAT_QUAKE2 = 1,
  WAD_FORMAT_HALFLIFE = 2,
  WAD_CMP_NONE  = 0,
  WAD_CMP_LZSS  = 1,
  WAD_TYP_NONE  = 0,
  WAD_TYP_LABEL = 1,
  WAD_TYP_LUMPY = 64,       // 64 + grab command number
  WAD_TYP_PALETTE = 64,
  WAD_TYP_QTEX  = 65,
  WAD_TYP_QPIC  = 66,
  WAD_TYP_SOUND = 67,
  WAD_TYP_MIPTEX  = 68,
  WAD_TYP_MIPTEX_HALFLIFE = 67,
  WAD_TYP_FONT  = 70,
};
// byte-align structures
//#include "irrpack.h"
struct SWADFileHeader {
  c8 tag[4];      // type of WAD format WAD2 = quake2, WAD3 = halflife
  u32 numlumps;
  u32 infotableofs;
} PACK_STRUCT;
struct SWADFileEntryOriginal {
  u32 filepos;
  u32 disksize;
  u32 size;         // uncompressed
  u8 type;
  u8 compression;
  u8 pad[2];
  u8 name[16];        // must be null terminated
} PACK_STRUCT;
// Default alignment
//#include "irrunpack.h"
struct SWADFileEntry {
  char* simpleFileName;
  bool operator < (const SWADFileEntry& other) const {
    return simpleFileName < other.simpleFileName;
  }
  char* wadFileName;
  SWADFileEntryOriginal header;
};
//! Archiveloader capable of loading WAD Archives
class CArchiveLoaderWAD : public IArchiveLoader
{
public:
  //! Constructor
  CArchiveLoaderWAD(IFileSystem* fs);
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
//! reads from WAD
class CWADReader : public IFileArchive, CFileList
{
public:
  CWADReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
  ~CWADReader();
  // file archive methods
  //! return the id of the file Archive
  const char* getArchiveName() const;
  //! opens a file by file name
  IReadFile* createAndOpenFile(const char* filename);
  //! opens a file by index
  IReadFile* createAndOpenFile(u32 index);
  //! returns the list of files
  const IFileList* getFileList() const;
  //! get the class Type
  E_FILE_ARCHIVE_TYPE getType() const {
    return EFAT_WAD;
  }
private:
  char* Type;
  //! scans for a local header, returns false if there is no more local file header.
  bool scanLocalHeader();
  //! splits filename from zip file into useful filenames and paths
  void extractFilename(SWADFileEntry* entry);
  char* Base;
  char* MountPoint;
  IReadFile* File;
  eWADFileTypes WadType;
  SWADFileHeader Header;
  //SWADFileEntry> FileInfo;
  IFileSystem* FileSystem;
};
#endif
#endif // #ifdef __IRR_COMPILE_WITH_WAD_ARCHIVE_LOADER_

