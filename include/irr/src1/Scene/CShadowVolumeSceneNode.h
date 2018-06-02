// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SHADOW_VOLUME_SCENE_NODE_H_INCLUDED__
#define __C_SHADOW_VOLUME_SCENE_NODE_H_INCLUDED__
//#include "IShadowVolumeSceneNode.h"
//! Scene node for rendering a shadow volume into a stencil buffer.
class CShadowVolumeSceneNode : public IShadowVolumeSceneNode
{
public:
  //! constructor
  CShadowVolumeSceneNode(const IMesh* shadowMesh, ISceneNode* parent, ISceneManager* mgr,
      s32 id,  bool zfailmethod = true, f32 infinity = 10000.0f);
  //! destructor
  ~CShadowVolumeSceneNode();
  //! Sets the mesh from which the shadow volume should be generated.
  /** To optimize shadow rendering, use a simpler mesh for shadows.
  */
  void setShadowMesh(const IMesh* mesh);
  //! Updates the shadow volumes for current light positions.
  /** Called each render cycle from Animated Mesh SceneNode render method. */
  void updateShadowVolumes();
  //! pre render method
  void OnRegisterSceneNode();
  //! renders the node.
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_SHADOW_VOLUME;
  }
private:
  typedef float3 > SShadowVolume;
  void createShadowVolume(const float3 pos, bool isDirectional = false);
  u32 createEdgesAndCaps(const float3 light, SShadowVolume* svp, F3AABBOX* bb);
  //! Generates adjacency information based on mesh indices.
  void calculateAdjacency();
  F3AABBOX Box;
  // a shadow volume for every light
  SShadowVolume > ShadowVolumes;
  // a back cap bounding box for every light
  F3AABBOX* ShadowBBox;
  float3 > Vertices;
  u16* Indices;
  u16* Adjacency;
  u16* Edges;
  // tells if face is front facing
  bool > FaceData;
  const IMesh* ShadowMesh;
  u32 IndexCount;
  u32 VertexCount;
  u32 ShadowVolumesUsed;
  f32 Infinity;
  bool UseZFailMethod;
};
#endif

