// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_Z_BUFFER_H_INCLUDED__
#define __C_Z_BUFFER_H_INCLUDED__
//#include "IZBuffer.h"
class CZBuffer : public IZBuffer
{
public:
  //! constructor
  CZBuffer(const dimension2du& size);
  //! destructor
  ~CZBuffer();
  //! clears the zbuffer
  void clear();
  //! sets the new size of the zbuffer
  void setSize(const dimension2du& size);
  //! returns the size of the zbuffer
  const dimension2du& getSize() const;
  //! locks the zbuffer
  TZBufferType* lock();
  //! unlocks the zbuffer
  void unlock();
private:
  TZBufferType* Buffer;
  TZBufferType* BufferEnd;
  dimension2du Size;
  s32 TotalSize;
};
#endif

