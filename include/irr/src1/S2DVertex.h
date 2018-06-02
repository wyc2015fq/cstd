// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __S_K_2D_VERTEX_H_INCLUDED__
#define __S_K_2D_VERTEX_H_INCLUDED__
typedef signed short TZBufferType;
struct S2DVertex {
  vector2d<s32> Pos;  // position
  vector2d<s32> TCoords;  // texture coordinates
  TZBufferType ZValue;    // zvalue
  u16 Color;
};
#endif

