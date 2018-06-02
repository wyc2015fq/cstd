// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_Z_BUFFER_H_INCLUDED__
#define __I_Z_BUFFER_H_INCLUDED__
//#include "dimension2d.h"
//#include "S2DVertex.h"
class IZBuffer
{
public:
  //! destructor
  ~IZBuffer() {};
  //! clears the zbuffer
  void clear() = 0;
  //! sets the new size of the zbuffer
  void setSize(const dimension2du& size) = 0;
  //! returns the size of the zbuffer
  const dimension2du& getSize() = 0;
  //! locks the zbuffer
  TZBufferType* lock() = 0;
  //! unlocks the zbuffer
  void unlock() = 0;
};
//! creates a ZBuffer
IZBuffer* createZBuffer(const dimension2du& size);
#endif

