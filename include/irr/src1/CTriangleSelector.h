// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_TRIANGLE_SELECTOR_H_INCLUDED__
#define __C_TRIANGLE_SELECTOR_H_INCLUDED__
//#include "ITriangleSelector.h"
//#include "irrArray.h"
class ISceneNode;
class IAnimatedMeshSceneNode;
//! Stupid triangle selector without optimization
class CTriangleSelector : public ITriangleSelector
{
public:
  //! Constructs a selector based on a mesh
  CTriangleSelector(ISceneNode* node);
  //! Constructs a selector based on a mesh
  CTriangleSelector(const IMesh* mesh, ISceneNode* node);
  //! Constructs a selector based on an animated mesh scene node
  //!\param node An animated mesh scene node, which must have a valid mesh
  CTriangleSelector(IAnimatedMeshSceneNode* node);
  //! Constructs a selector based on a bounding box
  CTriangleSelector(const F3AABBOX& box, ISceneNode* node);
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
  //! Returns amount of all available triangles in this selector
  s32 getTriangleCount() const;
  //! Return the scene node associated with a given triangle.
  ISceneNode* getSceneNodeForTriangle(u32 triangleIndex) const {
    return SceneNode;
  }
  // Get the number of TriangleSelectors that are part of this one
  u32 getSelectorCount() const;
  // Get the TriangleSelector based on index based on getSelectorCount
  ITriangleSelector* getSelector(u32 index);
  // Get the TriangleSelector based on index based on getSelectorCount
  const ITriangleSelector* getSelector(u32 index) const;
protected:
  //! Create from a mesh
  void createFromMesh(const IMesh* mesh);
  //! Update when the mesh has changed
  void updateFromMesh(const IMesh* mesh) const;
  //! Update the triangle selector, which will only have an effect if it
  //! was built from an animated mesh and that mesh's frame has changed
  //! since the last time it was updated.
  void update(void) const;
  ISceneNode* SceneNode;
  mutable triangle3df > Triangles; // (mutable for CTriangleBBSelector)
  mutable aabbox3df BoundingBox; // Allows for trivial rejection
  IAnimatedMeshSceneNode* AnimatedNode;
  mutable u32 LastMeshFrame;
};
#endif

