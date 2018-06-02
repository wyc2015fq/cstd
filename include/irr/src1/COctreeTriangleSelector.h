// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_OCTREE_TRIANGLE_SELECTOR_H_INCLUDED__
#define __C_OCTREE_TRIANGLE_SELECTOR_H_INCLUDED__
//#include "CTriangleSelector.h"
class ISceneNode;
//! Stupid triangle selector without optimization
class COctreeTriangleSelector : public CTriangleSelector
{
public:
  //! Constructs a selector based on a mesh
  COctreeTriangleSelector(const IMesh* mesh, ISceneNode* node, s32 minimalPolysPerNode);
  ~COctreeTriangleSelector();
  //! Gets all triangles which lie within a specific bounding box.
  void getTriangles(triangle3df* triangles, s32 arraySize, s32& outTriangleCount,
      const F3AABBOX& box, const matrix4* transform = 0) const;
  //! Gets all triangles which have or may have contact with a 3d line.
  void getTriangles(triangle3df* triangles, s32 arraySize,
      s32& outTriangleCount, const F3LINE& line,
      const matrix4* transform = 0) const;
private:
  struct SOctreeNode {
    SOctreeNode() {
      int i;
      for (i = 0; i != 8; ++i) {
        Child[i] = 0;
      }
    }
    ~SOctreeNode() {
      int i;
      for (i = 0; i != 8; ++i) {
        delete Child[i];
      }
    }
    triangle3df > Triangles;
    SOctreeNode* Child[8];
    F3AABBOX Box;
  };
  void constructOctree(SOctreeNode* node);
  void deleteEmptyNodes(SOctreeNode* node);
  void getTrianglesFromOctree(SOctreeNode* node, s32& trianglesWritten,
      s32 maximumSize, const F3AABBOX& box,
      const matrix4* transform,
      triangle3df* triangles) const;
  void getTrianglesFromOctree(SOctreeNode* node, s32& trianglesWritten,
      s32 maximumSize, const F3LINE& line,
      const matrix4* transform,
      triangle3df* triangles) const;
  SOctreeNode* Root;
  s32 NodeCount;
  s32 MinimalPolysPerNode;
};
#endif

