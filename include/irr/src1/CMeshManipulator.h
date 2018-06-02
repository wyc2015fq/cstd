// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_MESH_MANIPULATOR_H_INCLUDED__
#define __C_MESH_MANIPULATOR_H_INCLUDED__
//#include "IMeshManipulator.h"
//! An interface for easy manipulation of meshes.
/** Scale, set alpha value, flip surfaces, and so on. This exists for fixing
problems with wrong imported or exported meshes quickly after loading. It is
not intended for doing mesh modifications and/or animations during runtime.
*/
class CMeshManipulator : public IMeshManipulator
{
public:
  //! Flips the direction of surfaces.
  /** Changes backfacing triangles to frontfacing triangles and vice versa.
  \param mesh: Mesh on which the operation is performed. */
  void flipSurfaces(IMesh* mesh) const;
  //! Recalculates all normals of the mesh.
  /** \param mesh: Mesh on which the operation is performed.
      \param smooth: Whether to use smoothed normals. */
  void recalculateNormals(IMesh* mesh, bool smooth = false, bool angleWeighted = false) const;
  //! Recalculates all normals of the mesh buffer.
  /** \param buffer: Mesh buffer on which the operation is performed.
      \param smooth: Whether to use smoothed normals. */
  void recalculateNormals(IMeshBuffer* buffer, bool smooth = false, bool angleWeighted = false) const;
  //! Clones a static IMesh into a modifiable SMesh.
  SMesh* createMeshCopy(IMesh* mesh) const;
  //! Creates a planar texture mapping on the mesh
  /** \param mesh: Mesh on which the operation is performed.
  \param resolution: resolution of the planar mapping. This is the value
  specifying which is the relation between world space and
  texture coordinate space. */
  void makePlanarTextureMapping(IMesh* mesh, f32 resolution = 0.001f) const;
  //! Creates a planar texture mapping on the meshbuffer
  void makePlanarTextureMapping(IMeshBuffer* meshbuffer, f32 resolution = 0.001f) const;
  //! Creates a planar texture mapping on the meshbuffer
  void makePlanarTextureMapping(IMeshBuffer* buffer, f32 resolutionS, f32 resolutionT, u8 axis, const float3 offset) const;
  //! Creates a planar texture mapping on the mesh
  void makePlanarTextureMapping(IMesh* mesh, f32 resolutionS, f32 resolutionT, u8 axis, const float3 offset) const;
  //! Recalculates tangents, requires a tangent mesh buffer
  void recalculateTangents(IMeshBuffer* buffer, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false) const;
  //! Recalculates tangents, requires a tangent mesh
  void recalculateTangents(IMesh* mesh, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false) const;
  //! Creates a copy of the mesh, which will only consist of S3DVertex vertices.
  IMesh* createMeshWithTangents(IMesh* mesh, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false, bool recalculateTangents = true) const;
  //! Creates a copy of the mesh, which will only consist of S3D2TCoords vertices.
  IMesh* createMeshWith2TCoords(IMesh* mesh) const;
  //! Creates a copy of the mesh, which will only consist of S3DVertex vertices.
  IMesh* createMeshWith1TCoords(IMesh* mesh) const;
  //! Creates a copy of the mesh, which will only consist of unique triangles, i.e. no vertices are shared.
  IMesh* createMeshUniquePrimitives(IMesh* mesh) const;
  //! Creates a copy of the mesh, which will have all duplicated vertices removed, i.e. maximal amount of vertices are shared via indexing.
  IMesh* createMeshWelded(IMesh* mesh, f32 tolerance = ROUNDING_ERROR_f32) const;
  //! Returns amount of polygons in mesh.
  s32 getPolyCount(IMesh* mesh) const;
  //! Returns amount of polygons in mesh.
  s32 getPolyCount(IAnimatedMesh* mesh) const;
  //! create a new AnimatedMesh and adds the mesh to it
  IAnimatedMesh* createAnimatedMesh(IMesh* mesh, E_ANIMATED_MESH_TYPE type) const;
  //! create a mesh optimized for the vertex cache
  IMesh* createForsythOptimizedMesh(const IMesh* mesh) const;
};
#endif

