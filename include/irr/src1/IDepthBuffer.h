// Copyright (C) 2002-2012 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_Z2_BUFFER_H_INCLUDED__
#define __I_Z2_BUFFER_H_INCLUDED__
//#include "dimension2d.h"
//#include "S4DVertex.h"
class IDepthBuffer
{
public:
  //! destructor
  ~IDepthBuffer() {};
  //! clears the zbuffer
  void clear() = 0;
  //! sets the new size of the zbuffer
  void setSize(const dimension2du& size) = 0;
  //! returns the size of the zbuffer
  const dimension2du& getSize() = 0;
  //! locks the zbuffer
  void* lock() = 0;
  //! unlocks the zbuffer
  void unlock() = 0;
  //! returns pitch of depthbuffer (in bytes)
  u32 getPitch() = 0;
};
//! creates a ZBuffer
IDepthBuffer* createDepthBuffer(const dimension2du& size);
class IStencilBuffer
{
public:
  //! destructor
  ~IStencilBuffer() {};
  //! clears the zbuffer
  void clear() = 0;
  //! sets the new size of the zbuffer
  void setSize(const dimension2du& size) = 0;
  //! returns the size of the zbuffer
  const dimension2du& getSize() = 0;
  //! locks the zbuffer
  void* lock() = 0;
  //! unlocks the zbuffer
  void unlock() = 0;
  //! returns pitch of depthbuffer (in bytes)
  u32 getPitch() = 0;
};
//! creates a Stencil Buffer
IStencilBuffer* createStencilBuffer(const dimension2du& size);
#endif

