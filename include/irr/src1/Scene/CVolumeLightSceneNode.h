// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// created by Dean Wadsworth aka Varmint Dec 31 2007
#ifndef __VOLUME_LIGHT_SCENE_NODE_H_INCLUDED__
#define __VOLUME_LIGHT_SCENE_NODE_H_INCLUDED__
//#include "IVolumeLightSceneNode.h"
class CVolumeLightSceneNode : public IVolumeLightSceneNode
{
public:
  //! constructor
  CVolumeLightSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      const u32 subdivU = 32, const u32 subdivV = 32,
      const SColor foot = SColor(51, 0, 230, 180),
      const SColor tail = SColor(0, 0, 0, 0),
      const float3 position = float3(0, 0, 0),
      const float3 rotation = float3(0, 0, 0),
      const float3 scale = float3(1.0f, 1.0f, 1.0f));
  ~CVolumeLightSceneNode();
  void OnRegisterSceneNode();
  //! renders the node.
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! returns the material based on the zero based index i.
  SMaterial& getMaterial(u32 i);
  //! returns amount of materials used by this scene node.
  u32 getMaterialCount() const;
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_VOLUME_LIGHT;
  }
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Creates a clone of this scene node and its children.
  ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);
  void setSubDivideU(const u32 inU);
  void setSubDivideV(const u32 inV);
  u32 getSubDivideU() const {
    return SubdivideU;
  }
  u32 getSubDivideV() const {
    return SubdivideV;
  }
  void setFootColor(const SColor inColor);
  void setTailColor(const SColor inColor);
  SColor getFootColor() const {
    return FootColor;
  }
  SColor getTailColor() const {
    return TailColor;
  }
private:
  void constructLight();
  IMesh* Mesh;
  f32  LPDistance;    // Distance to hypothetical lightsource point -- affects fov angle
  u32  SubdivideU;    // Number of subdivisions in U and V space.
  u32  SubdivideV;    // Controls the number of "slices" in the volume.
  // NOTE : Total number of polygons = 2 + ((SubdivideU + 1) + (SubdivideV + 1)) * 2
  // Each slice being a quad plus the rectangular plane at the bottom.
  SColor FootColor;  // Color at the source
  SColor TailColor;  // Color at the end.
  float3 LightDimensions; // LightDimensions.Y Distance of shooting -- Length of beams
  // LightDimensions.X and LightDimensions.Z determine the size/dimension of the plane
};
#endif

