// Copyright (C) 2002-2012 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_DEPTH_BUFFER_H_INCLUDED__
#define __C_DEPTH_BUFFER_H_INCLUDED__
//#include "IDepthBuffer.h"
class CDepthBuffer : public IDepthBuffer
{
public:
  //! constructor
  CDepthBuffer(const dimension2du& size);
  //! destructor
  ~CDepthBuffer();
  //! clears the zbuffer
  void clear();
  //! sets the new size of the zbuffer
  void setSize(const dimension2du& size);
  //! returns the size of the zbuffer
  const dimension2du& getSize() const;
  //! locks the zbuffer
  void* lock() {
    return (void*) Buffer;
  }
  //! unlocks the zbuffer
  void unlock() {}
  //! returns pitch of depthbuffer (in bytes)
  u32 getPitch() const {
    return Pitch;
  }
private:
  u8* Buffer;
  dimension2du Size;
  u32 TotalSize;
  u32 Pitch;
};
class CStencilBuffer : public IStencilBuffer
{
public:
  //! constructor
  CStencilBuffer(const dimension2du& size);
  //! destructor
  ~CStencilBuffer();
  //! clears the zbuffer
  void clear();
  //! sets the new size of the zbuffer
  void setSize(const dimension2du& size);
  //! returns the size of the zbuffer
  const dimension2du& getSize() const;
  //! locks the zbuffer
  void* lock() {
    return (void*) Buffer;
  }
  //! unlocks the zbuffer
  void unlock() {}
  //! returns pitch of depthbuffer (in bytes)
  u32 getPitch() const {
    return Pitch;
  }
private:
  u8* Buffer;
  dimension2du Size;
  u32 TotalSize;
  u32 Pitch;
};
#endif // __C_DEPTH_BUFFER_H_INCLUDED__

