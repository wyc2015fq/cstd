// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_META_TRIANGLE_SELECTOR_H_INCLUDED__
#define __C_META_TRIANGLE_SELECTOR_H_INCLUDED__
//#include "IMetaTriangleSelector.h"
//#include "irrArray.h"
//! Interface for making multiple triangle selectors work as one big selector.
class CMetaTriangleSelector : public IMetaTriangleSelector
{
public:
  //! constructor
  CMetaTriangleSelector();
  //! destructor
  ~CMetaTriangleSelector();
  //! Get amount of all available triangles in this selector
  s32 getTriangleCount() const;
  //! Gets all triangles.
  void getTriangles(triangle3df* triangles, s32 arraySize,
      s32& outTriangleCount, const matrix4* transform = 0) const;
  //! Gets all triangles which lie within a specific bounding box.
  void getTriangles(triangle3df* triangles, s32 arraySize,
      s32& outTriangleCount, const F3AABBOX& box,
      const matrix4* transform = 0) const;
  //! Gets all triangles which have or may have contact with a 3d line.
  void getTriangles(triangle3df* triangles, s32 arraySize,
      s32& outTriangleCount, const F3LINE& line,
      const matrix4* transform = 0) const;
  //! Adds a triangle selector to the collection of triangle selectors
  //! in this metaTriangleSelector.
  void addTriangleSelector(ITriangleSelector* toAdd);
  //! Removes a specific triangle selector which was added before from the collection.
  bool removeTriangleSelector(ITriangleSelector* toRemove);
  //! Removes all triangle selectors from the collection.
  void removeAllTriangleSelectors();
  //! Get the scene node associated with a given triangle.
  ISceneNode* getSceneNodeForTriangle(u32 triangleIndex) const;
  // Get the number of TriangleSelectors that are part of this one
  u32 getSelectorCount() const;
  // Get the TriangleSelector based on index based on getSelectorCount
  ITriangleSelector* getSelector(u32 index);
  // Get the TriangleSelector based on index based on getSelectorCount
  const ITriangleSelector* getSelector(u32 index) const;
private:
  ITriangleSelector* > TriangleSelectors;
};
#endif

