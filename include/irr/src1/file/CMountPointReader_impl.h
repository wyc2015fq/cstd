// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CMountPointReader.h"
#ifdef __IRR_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_
//#include "CReadFile.h"
//#include "os.h"
//! Constructor
CArchiveLoaderMount::CArchiveLoaderMount(IFileSystem* fs)
  : FileSystem(fs)
{
#ifdef _DEBUG
  setDebugName("CArchiveLoaderMount");
#endif
}
//! returns true if the file maybe is able to be loaded by this class
bool CArchiveLoaderMount::isALoadableFileFormat(const char* filename) const
{
  char* fname(filename);
  deletePathFromFilename(fname);
  if (!fname_size) {
    return true;
  }
  IFileList* list = FileSystem->createFileList();
  bool ret = false;
  if (list) {
    // check if name is found as directory
    if (list->findFile(filename, true)) {
      ret = true;
    }
    list->drop();
  }
  return ret;
}
//! Check to see if the loader can create archives of this type.
bool CArchiveLoaderMount::isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const
{
  return fileType == EFAT_FOLDER;
}
//! Check if the file might be loaded by this class
bool CArchiveLoaderMount::isALoadableFileFormat(IReadFile* file) const
{
  return false;
}
//! Creates an archive from the filename
IFileArchive* CArchiveLoaderMount::createArchive(const char* filename, bool ignoreCase, bool ignorePaths) const
{
  IFileArchive* archive = 0;
  EFileSystemType current = FileSystem->setFileListSystem(FILESYSTEM_NATIVE);
  const char* save = FileSystem->getWorkingDirectory();
  char* fullPath = FileSystem->getAbsolutePath(filename);
  FileSystem->flattenFilename(fullPath);
  if (FileSystem->changeWorkingDirectoryTo(fullPath)) {
    archive = new CMountPointReader(FileSystem, fullPath, ignoreCase, ignorePaths);
  }
  FileSystem->changeWorkingDirectoryTo(save);
  FileSystem->setFileListSystem(current);
  return archive;
}
//! creates/loads an archive from the file.
//! \return Pointer to the created archive. Returns 0 if loading failed.
IFileArchive* CArchiveLoaderMount::createArchive(IReadFile* file, bool ignoreCase, bool ignorePaths) const
{
  return 0;
}
//! compatible Folder Architecture
CMountPointReader::CMountPointReader(IFileSystem* parent, const char* basename, bool ignoreCase, bool ignorePaths)
  : CFileList(basename, ignoreCase, ignorePaths), Parent(parent)
{
  //! ensure CFileList path ends in a slash
  if (Path.lastChar() != '/') {
    Path.append('/');
  }
  const char* work = Parent->getWorkingDirectory();
  Parent->changeWorkingDirectoryTo(basename);
  buildDirectory();
  Parent->changeWorkingDirectoryTo(work);
  sort();
}
//! returns the list of files
const IFileList* CMountPointReader::getFileList() const
{
  return this;
}
void CMountPointReader::buildDirectory()
{
  IFileList* list = Parent->createFileList();
  if (!list) {
    return;
  }
  const u32 size = list->getFileCount();
  int i;
  for (i = 0; i < size; ++i) {
    char* full = list->getFullFileName(i);
    full = full.subString(Path_size, full_size - Path_size);
    if (!list->isDirectory(i)) {
      addItem(full, list->getFileOffset(i), list->getFileSize(i), false, RealFileNames_size);
      RealFileNames.push_back(list->getFullFileName(i));
    }
    else {
      const char* rel = list->getFileName(i);
      RealFileNames.push_back(list->getFullFileName(i));
      char* pwd  = Parent->getWorkingDirectory();
      if (pwd.lastChar() != '/') {
        pwd.append('/');
      }
      pwd.append(rel);
      if (rel != "." && rel != "..") {
        addItem(full, 0, 0, true, 0);
        Parent->changeWorkingDirectoryTo(pwd);
        buildDirectory();
        Parent->changeWorkingDirectoryTo("..");
      }
    }
  }
  list->drop();
}
//! opens a file by index
IReadFile* CMountPointReader::createAndOpenFile(u32 index)
{
  if (index >= Files_size) {
    return 0;
  }
  return createReadFile(RealFileNames[Files[index].ID]);
}
//! opens a file by file name
IReadFile* CMountPointReader::createAndOpenFile(const char* filename)
{
  s32 index = findFile(filename, false);
  if (index != -1) {
    return createAndOpenFile(index);
  }
  else {
    return 0;
  }
}
} // io
} // irr
#endif // __IRR_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_

