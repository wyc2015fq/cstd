// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_NODE_ANIMATOR_FLY_CIRCLE_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_FLY_CIRCLE_H_INCLUDED__
//#include "ISceneNode.h"
class CSceneNodeAnimatorFlyCircle : public ISceneNodeAnimator
{
public:
  //! constructor
  CSceneNodeAnimatorFlyCircle(u32 time,
      const float3 center, f32 radius,
      f32 speed, const float3 direction,
      f32 radiusEllipsoid);
  //! animates a scene node
  void animateNode(ISceneNode* node, u32 timeMs);
  //! Writes attributes of the scene node animator.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node animator.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Returns type of the scene node animator
  ESCENE_NODE_ANIMATOR_TYPE getType() const {
    return ESNAT_FLY_CIRCLE;
  }
  //! Creates a clone of this animator.
  /** Please note that you will have to drop
  (IReferenceCounted::drop()) the returned pointer after calling
  this. */
  ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
private:
  // do some initial calculations
  void init();
  // circle center
  float3 Center;
  // up-vector, normal to the circle's plane
  float3 Direction;
  // Two helper vectors
  float3 VecU;
  float3 VecV;
  f32 Radius;
  f32 RadiusEllipsoid;
  f32 Speed;
  u32 StartTime;
};
#endif

