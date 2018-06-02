// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// The code for the TerrainTriangleSelector is based on the GeoMipMapSelector
// developed by Spintz. He made it available for Irrlicht and allowed it to be
// distributed under this licence. I only modified some parts. A lot of thanks go to him.
#ifndef __C_TERRAIN_TRIANGLE_SELECTOR_H__
#define __C_TERRAIN_TRIANGLE_SELECTOR_H__
//#include "ITriangleSelector.h"
//#include "irrArray.h"
class ITerrainSceneNode;
//! Triangle Selector for the TerrainSceneNode
/** The code for the TerrainTriangleSelector is based on the GeoMipMapSelector
developed by Spintz. He made it available for Irrlicht and allowed it to be
distributed under this licence. I only modified some parts. A lot of thanks go
to him.
*/
class CTerrainTriangleSelector : public ITriangleSelector
{
public:
  //! Constructs a selector based on an ITerrainSceneNode
  CTerrainTriangleSelector(ITerrainSceneNode* node, s32 LOD);
  //! Destructor
  ~CTerrainTriangleSelector();
  //! Clears and sets triangle data
  void setTriangleData(ITerrainSceneNode* node, s32 LOD);
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
  ISceneNode* getSceneNodeForTriangle(u32 triangleIndex) const;
  // Get the number of TriangleSelectors that are part of this one
  u32 getSelectorCount() const;
  // Get the TriangleSelector based on index based on getSelectorCount
  ITriangleSelector* getSelector(u32 index);
  // Get the TriangleSelector based on index based on getSelectorCount
  const ITriangleSelector* getSelector(u32 index) const;
private:
  friend class CTerrainSceneNode;
  struct SGeoMipMapTrianglePatch {
    triangle3df > Triangles;
    s32 NumTriangles;
    aabbox3df Box;
  };
  struct SGeoMipMapTrianglePatches {
    SGeoMipMapTrianglePatches() :
      NumPatches(0), TotalTriangles(0) {
    }
    SGeoMipMapTrianglePatch > TrianglePatchArray;
    s32 NumPatches;
    u32 TotalTriangles;
  };
  ITerrainSceneNode* SceneNode;
  SGeoMipMapTrianglePatches TrianglePatches;
};
#endif // __C_TERRAIN_TRIANGLE_SELECTOR_H__

