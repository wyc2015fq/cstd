// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_NODE_ANIMATOR_FOLLOW_SPLINE_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_FOLLOW_SPLINE_H_INCLUDED__
//#include "ISceneNode.h"
//#include "irrArray.h"
//#include "ISceneNodeAnimatorFinishing.h"
//! Scene node animator based free code Matthias Gall wrote and sent in. (Most of
//! this code is written by him, I only modified bits.)
class CSceneNodeAnimatorFollowSpline : public ISceneNodeAnimatorFinishing
{
public:
  //! constructor
  CSceneNodeAnimatorFollowSpline(u32 startTime,
      const  float3 > & points,
      f32 speed = 1.0f, f32 tightness = 0.5f, bool loop = true, bool pingpong = false);
  //! animates a scene node
  void animateNode(ISceneNode* node, u32 timeMs);
  //! Writes attributes of the scene node animator.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node animator.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Returns type of the scene node animator
  ESCENE_NODE_ANIMATOR_TYPE getType() const {
    return ESNAT_FOLLOW_SPLINE;
  }
  //! Creates a clone of this animator.
  /** Please note that you will have to drop
  (IReferenceCounted::drop()) the returned pointer after calling
  this. */
  ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
protected:
  //! clamps a the value idx to fit into range 0..size-1
  s32 clamp(s32 idx, s32 size);
  float3 > Points;
  f32 Speed;
  f32 Tightness;
  u32 StartTime;
  bool Loop;
  bool PingPong;
};
#endif

