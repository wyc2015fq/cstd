// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// created by Dean Wadsworth aka Varmint Dec 31 2007
#ifndef __I_VOLUME_LIGHT_SCENE_NODE_H_INCLUDED__
#define __I_VOLUME_LIGHT_SCENE_NODE_H_INCLUDED__
//#include "ISceneNode.h"
class IMeshBuffer;
class IVolumeLightSceneNode : public ISceneNode
{
public:
  //! constructor
  IVolumeLightSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position,
      const float3 rotation,
      const float3 scale)
    : ISceneNode(parent, mgr, id, position, rotation, scale) {};
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_VOLUME_LIGHT;
  }
  //! Sets the number of segments across the U axis
  void setSubDivideU(const u32 inU) = 0;
  //! Sets the number of segments across the V axis
  void setSubDivideV(const u32 inV) = 0;
  //! Returns the number of segments across the U axis
  u32 getSubDivideU() = 0;
  //! Returns the number of segments across the V axis
  u32 getSubDivideV() = 0;
  //! Sets the color of the base of the light
  void setFootColor(const SColor inColor) = 0;
  //! Sets the color of the tip of the light
  void setTailColor(const SColor inColor) = 0;
  //! Returns the color of the base of the light
  SColor getFootColor() = 0;
  //! Returns the color of the tip of the light
  SColor getTailColor() = 0;
};
#endif

