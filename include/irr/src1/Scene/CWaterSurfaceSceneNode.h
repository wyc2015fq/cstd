// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_WATER_SURFACE_SCENE_NODE_H_INCLUDED__
#define __C_WATER_SURFACE_SCENE_NODE_H_INCLUDED__
//#include "CMeshSceneNode.h"
class CWaterSurfaceSceneNode : public CMeshSceneNode
{
public:
  //! constructor
  CWaterSurfaceSceneNode(f32 waveHeight, f32 waveSpeed, f32 waveLength,
      IMesh* mesh, ISceneNode* parent, ISceneManager* mgr,  s32 id,
      const float3 position = float3(0, 0, 0),
      const float3 rotation = float3(0, 0, 0),
      const float3 scale = float3(1.0f, 1.0f, 1.0f));
  //! destructor
  ~CWaterSurfaceSceneNode();
  //! frame registration
  void OnRegisterSceneNode();
  //! animated update
  void OnAnimate(u32 timeMs);
  //! Update mesh
  void setMesh(IMesh* mesh);
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_WATER_SURFACE;
  }
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  CV_INLINE f32 addWave(const float3 source, f32 time) const;
  f32 WaveLength;
  f32 WaveSpeed;
  f32 WaveHeight;
  IMesh* OriginalMesh;
};
#endif

