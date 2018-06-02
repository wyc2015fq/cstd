// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_NODE_ANIMATOR_FLY_STRAIGHT_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_FLY_STRAIGHT_H_INCLUDED__
//#include "ISceneNodeAnimatorFinishing.h"
class CSceneNodeAnimatorFlyStraight : public ISceneNodeAnimatorFinishing
{
public:
  //! constructor
  CSceneNodeAnimatorFlyStraight(const float3 startPoint,
      const float3 endPoint,
      u32 timeForWay,
      bool loop, u32 now, bool pingpong);
  //! animates a scene node
  void animateNode(ISceneNode* node, u32 timeMs);
  //! Writes attributes of the scene node animator.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node animator.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Returns type of the scene node animator
  ESCENE_NODE_ANIMATOR_TYPE getType() const {
    return ESNAT_FLY_STRAIGHT;
  }
  //! Creates a clone of this animator.
  /** Please note that you will have to drop
  (IReferenceCounted::drop()) the returned pointer after calling this. */
  ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
private:
  void recalculateIntermediateValues();
  float3 Start;
  float3 End;
  float3 Vector;
  f32 TimeFactor;
  u32 StartTime;
  u32 TimeForWay;
  bool Loop;
  bool PingPong;
};
#endif

