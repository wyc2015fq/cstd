// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_TRIANGLE_BB_SELECTOR_H_INCLUDED__
#define __C_TRIANGLE_BB_SELECTOR_H_INCLUDED__
//#include "CTriangleSelector.h"
//! Stupid triangle selector without optimization
class CTriangleBBSelector : public CTriangleSelector
{
public:
  //! Constructs a selector based on a mesh
  CTriangleBBSelector(ISceneNode* node);
  //! Gets all triangles.
  void getTriangles(triangle3df* triangles, s32 arraySize, s32& outTriangleCount,
      const matrix4* transform = 0) const;
  //! Gets all triangles which lie within a specific bounding box.
  void getTriangles(triangle3df* triangles, s32 arraySize, s32& outTriangleCount,
      const F3AABBOX& box, const matrix4* transform = 0) const;
  //! Gets all triangles which have or may have contact with a 3d line.
  void getTriangles(triangle3df* triangles, s32 arraySize,
      s32& outTriangleCount, const F3LINE& line,
      const matrix4* transform = 0) const;
};
#endif

