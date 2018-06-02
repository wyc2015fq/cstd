// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_LIMIT_READ_FILE_H_INCLUDED__
#define __C_LIMIT_READ_FILE_H_INCLUDED__
//#include "IReadFile.h"
//#include "irrString.h"
class CUnicodeConverter;
/*! this is a read file, which is limited to some boundaries,
  so that it may only start from a certain file position
  and may only read until a certain file position.
  This can be useful, for example for reading uncompressed files
  in an archive (zip, tar).
!*/
class CLimitReadFile : public IReadFile
{
public:
  CLimitReadFile(IReadFile* alreadyOpenedFile, long pos, long areaSize, const char* name);
  ~CLimitReadFile();
  //! returns how much was read
  s32 read(void* buffer, u32 sizeToRead);
  //! changes position in file, returns true if successful
  //! if relativeMovement==true, the pos is changed relative to current pos,
  //! otherwise from begin of file
  bool seek(long finalPos, bool relativeMovement = false);
  //! returns size of file
  long getSize() const;
  //! returns where in the file we are.
  long getPos() const;
  //! returns name of file
  const char* getFileName() const;
private:
  char* Filename;
  long AreaStart;
  long AreaEnd;
  long Pos;
  IReadFile* File;
};
#endif

