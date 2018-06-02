// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CZBuffer.h"
//#include "irrString.h"
#ifdef _IRR_COMPILE_WITH_SOFTWARE_
//! constructor
CZBuffer::CZBuffer(const dimension2du& size)
  : Buffer(0), BufferEnd(0), Size(0, 0), TotalSize(0)
{
#ifdef _DEBUG
  setDebugName("CZBuffer");
#endif
  setSize(size);
}
//! destructor
CZBuffer::~CZBuffer()
{
  delete [] Buffer;
}
//! clears the zbuffer
void CZBuffer::clear()
{
  memset(Buffer, 0, (BufferEnd - Buffer)*sizeof(TZBufferType));
}
//! sets the new size of the zbuffer
void CZBuffer::setSize(const dimension2du& size)
{
  if (size == Size) {
    return;
  }
  Size = size;
  delete [] Buffer;
  TotalSize = size.Width * size.Height;
  Buffer = new TZBufferType[TotalSize];
  BufferEnd = Buffer + TotalSize;
}
//! returns the size of the zbuffer
const dimension2du& CZBuffer::getSize() const
{
  return Size;
}
//! locks the zbuffer
TZBufferType* CZBuffer::lock()
{
  return Buffer;
}
//! unlocks the zbuffer
void CZBuffer::unlock()
{
}
#endif // _IRR_COMPILE_WITH_SOFTWARE_
//! creates a ZBuffer
IZBuffer* createZBuffer(const dimension2du& size)
{
#ifdef _IRR_COMPILE_WITH_SOFTWARE_
  return new CZBuffer(size);
#else
  return 0;
#endif // _IRR_COMPILE_WITH_SOFTWARE_
}

