// Copyright (C) 2002-2012 Nikolaus Gebhardt
// Copyright (C) 2009-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// Based on the NPK reader from Irrlicht
//#include "CNPKReader.h"
#ifdef __IRR_COMPILE_WITH_NPK_ARCHIVE_LOADER_
//#include "os.h"
#ifdef _DEBUG
#define IRR_DEBUG_NPK_READER
#endif
bool isHeaderValid(const SNPKHeader& header)
{
  const c8* const tag = header.Tag;
  return tag[0] == '0' &&
      tag[1] == 'K' &&
      tag[2] == 'P' &&
      tag[3] == 'N';
}
// end
//! Constructor
CArchiveLoaderNPK::CArchiveLoaderNPK(IFileSystem* fs)
  : FileSystem(fs)
{
#ifdef _DEBUG
  setDebugName("CArchiveLoaderNPK");
#endif
}
//! returns true if the file maybe is able to be loaded by this class
bool CArchiveLoaderNPK::isALoadableFileFormat(const char* filename) const
{
  return hasFileExtension(filename, "npk");
}
//! Check to see if the loader can create archives of this type.
bool CArchiveLoaderNPK::isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const
{
  return fileType == EFAT_NPK;
}
//! Creates an archive from the filename
/** \param file File handle to check.
\return Pointer to newly created archive, or 0 upon error. */
IFileArchive* CArchiveLoaderNPK::createArchive(const char* filename, bool ignoreCase, bool ignorePaths) const
{
  IFileArchive* archive = 0;
  IReadFile* file = FileSystem->createAndOpenFile(filename);
  if (file) {
    archive = createArchive(file, ignoreCase, ignorePaths);
    file->drop();
  }
  return archive;
}
//! creates/loads an archive from the file.
//! \return Pointer to the created archive. Returns 0 if loading failed.
IFileArchive* CArchiveLoaderNPK::createArchive(IReadFile* file, bool ignoreCase, bool ignorePaths) const
{
  IFileArchive* archive = 0;
  if (file) {
    file->seek(0);
    archive = new CNPKReader(file, ignoreCase, ignorePaths);
  }
  return archive;
}
//! Check if the file might be loaded by this class
/** Check might look into the file.
\param file File handle to check.
\return True if file seems to be loadable. */
bool CArchiveLoaderNPK::isALoadableFileFormat(IReadFile* file) const
{
  SNPKHeader header;
  file->read(&header, sizeof(header));
  return isHeaderValid(header);
}
/*!
  NPK Reader
*/
CNPKReader::CNPKReader(IReadFile* file, bool ignoreCase, bool ignorePaths)
  : CFileList((file ? file->getFileName() : char * ("")), ignoreCase, ignorePaths), File(file)
{
#ifdef _DEBUG
  setDebugName("CNPKReader");
#endif
  if (File) {
    File->grab();
    if (scanLocalHeader()) {
      sort();
    }
    else {
      os::Printer::log("Failed to load NPK archive.");
    }
  }
}
CNPKReader::~CNPKReader()
{
  if (File) {
    File->drop();
  }
}
const IFileList* CNPKReader::getFileList() const
{
  return this;
}
bool CNPKReader::scanLocalHeader()
{
  SNPKHeader header;
  // Read and validate the header
  File->read(&header, sizeof(header));
  if (!isHeaderValid(header)) {
    return false;
  }
  // Seek to the table of contents
#ifdef __BIG_ENDIAN__
  header.Offset = os::Byteswap::BYTESWAP(header.Offset);
  header.Length = os::Byteswap::BYTESWAP(header.Length);
#endif
  header.Offset += 8;
  char* dirName;
  bool inTOC = true;
  // Loop through each entry in the table of contents
  while (inTOC && (File->getPos() < File->getSize())) {
    // read an entry
    char tag[4] = {0};
    SNPKFileEntry entry;
    File->read(tag, 4);
    const int numTag = MAKE_IRR_ID(tag[3], tag[2], tag[1], tag[0]);
    int size;
    bool isDir = true;
    switch (numTag) {
    case MAKE_IRR_ID('D', 'I', 'R', '_'): {
      File->read(&size, 4);
      readString(entry.Name);
      entry.Length = 0;
      entry.Offset = 0;
#ifdef IRR_DEBUG_NPK_READER
      os::Printer::log("Dir", entry.Name);
#endif
    }
    break;
    case MAKE_IRR_ID('F', 'I', 'L', 'E'): {
      File->read(&size, 4);
      File->read(&entry.Offset, 4);
      File->read(&entry.Length, 4);
      readString(entry.Name);
      isDir = false;
#ifdef IRR_DEBUG_NPK_READER
      os::Printer::log("File", entry.Name);
#endif
#ifdef __BIG_ENDIAN__
      entry.Offset = os::Byteswap::BYTESWAP(entry.Offset);
      entry.Length = os::Byteswap::BYTESWAP(entry.Length);
#endif
    }
    break;
    case MAKE_IRR_ID('D', 'E', 'N', 'D'): {
      File->read(&size, 4);
      entry.Name = "";
      entry.Length = 0;
      entry.Offset = 0;
      const s32 pos = dirName.findLast('/', dirName_size - 2);
      if (pos == -1) {
        dirName = "";
      }
      else {
        dirName = dirName.subString(0, pos);
      }
#ifdef IRR_DEBUG_NPK_READER
      os::Printer::log("Dirend", dirName);
#endif
    }
    break;
    default:
      inTOC = false;
    }
    // skip root dir
    if (isDir) {
      if (!entry.Name_size || (entry.Name == ".") || (entry.Name == "<noname>")) {
        continue;
      }
      dirName += entry.Name;
      dirName += "/";
    }
#ifdef IRR_DEBUG_NPK_READER
    os::Printer::log("Name", entry.Name);
#endif
    addItem((isDir ? dirName : dirName + entry.Name), entry.Offset + header.Offset, entry.Length, isDir);
  }
  return true;
}
//! opens a file by file name
IReadFile* CNPKReader::createAndOpenFile(const char* filename)
{
  s32 index = findFile(filename, false);
  if (index != -1) {
    return createAndOpenFile(index);
  }
  return 0;
}
//! opens a file by index
IReadFile* CNPKReader::createAndOpenFile(u32 index)
{
  if (index >= Files_size) {
    return 0;
  }
  const SFileListEntry& entry = Files[index];
  return createLimitReadFile(entry.FullName, File, entry.Offset, entry.Size);
}
void CNPKReader::readString(char* name)
{
  short stringSize;
  char buf[256];
  File->read(&stringSize, 2);
#ifdef __BIG_ENDIAN__
  stringSize = os::Byteswap::BYTESWAP(stringSize);
#endif
  name.reserve(stringSize);
  while (stringSize) {
    const short next = min_(stringSize, (short)255);
    File->read(buf, next);
    buf[next] = 0;
    name.append(buf);
    stringSize -= next;
  }
}
#endif // __IRR_COMPILE_WITH_NPK_ARCHIVE_LOADER_

