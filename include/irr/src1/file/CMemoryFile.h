// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_MEMORY_READ_FILE_H_INCLUDED__
#define __C_MEMORY_READ_FILE_H_INCLUDED__
//#include "IReadFile.h"
//#include "IWriteFile.h"
//#include "irrString.h"
/*!
  Class for reading and writing from memory.
*/
class CMemoryFile : public IReadFile, public IWriteFile
{
public:
  //! Constructor
  CMemoryFile(void* memory, long len, const char* fileName, bool deleteMemoryWhenDropped);
  //! Destructor
  ~CMemoryFile();
  //! returns how much was read
  s32 read(void* buffer, u32 sizeToRead);
  //! returns how much was written
  s32 write(const void* buffer, u32 sizeToWrite);
  //! changes position in file, returns true if successful
  bool seek(long finalPos, bool relativeMovement = false);
  //! returns size of file
  long getSize() const;
  //! returns where in the file we are.
  long getPos() const;
  //! returns name of file
  const char* getFileName() const;
private:
  void* Buffer;
  long Len;
  long Pos;
  char* Filename;
  bool deleteMemoryWhenDropped;
};
#endif

