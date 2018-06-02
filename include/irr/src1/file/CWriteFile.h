// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_WRITE_FILE_H_INCLUDED__
#define __C_WRITE_FILE_H_INCLUDED__
//#include <stdio.h>
//#include "IWriteFile.h"
//#include "irrString.h"
/*!
  Class for writing a real file to disk.
*/
class CWriteFile : public IWriteFile
{
public:
  CWriteFile(const char* fileName, bool append);
  ~CWriteFile();
  //! Reads an amount of bytes from the file.
  s32 write(const void* buffer, u32 sizeToWrite);
  //! Changes position in file, returns true if successful.
  bool seek(long finalPos, bool relativeMovement = false);
  //! Returns the current position in the file.
  long getPos() const;
  //! Returns name of file.
  const char* getFileName() const;
  //! returns if file is open
  bool isOpen() const;
private:
  //! opens the file
  void openFile(bool append);
  char* Filename;
  FILE* File;
  long FileSize;
};
#endif

