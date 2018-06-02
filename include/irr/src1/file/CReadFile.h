// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_READ_FILE_H_INCLUDED__
#define __C_READ_FILE_H_INCLUDED__
//#include <stdio.h>
//#include "IReadFile.h"
//#include "irrString.h"
/*!
  Class for reading a real file from disk.
*/
class CReadFile : public IReadFile
{
public:
  CReadFile(const char* fileName);
  ~CReadFile();
  //! returns how much was read
  s32 read(void* buffer, u32 sizeToRead);
  //! changes position in file, returns true if successful
  bool seek(long finalPos, bool relativeMovement = false);
  //! returns size of file
  long getSize() const;
  //! returns if file is open
  bool isOpen() const {
    return File != 0;
  }
  //! returns where in the file we are.
  long getPos() const;
  //! returns name of file
  const char* getFileName() const;
private:
  //! opens the file
  void openFile();
  FILE* File;
  long FileSize;
  char* Filename;
};
#endif

