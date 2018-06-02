// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_NODE_ANIMATOR_CAMERA_MAYA_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_CAMERA_MAYA_H_INCLUDED__
//#include "ISceneNodeAnimatorCameraMaya.h"
//#include "ICameraSceneNode.h"
class ICursorControl;
}
//! Special scene node animator for FPS cameras
/** This scene node animator can be attached to a camera to make it act
like a 3d modelling tool camera
*/
class CSceneNodeAnimatorCameraMaya : public ISceneNodeAnimatorCameraMaya
{
public:
  //! Constructor
  CSceneNodeAnimatorCameraMaya(ICursorControl* cursor, f32 rotateSpeed = -1500.f,
      f32 zoomSpeed = 200.f, f32 translationSpeed = 1500.f, f32 distance = 70.f);
  //! Destructor
  ~CSceneNodeAnimatorCameraMaya();
  //! Animates the scene node, currently only works on cameras
  void animateNode(ISceneNode* node, u32 timeMs);
  //! Event receiver
  bool OnEvent(const SEvent& event);
  //! Returns the speed of movement in units per millisecond
  f32 getMoveSpeed() const;
  //! Sets the speed of movement in units per millisecond
  void setMoveSpeed(f32 moveSpeed);
  //! Returns the rotation speed
  f32 getRotateSpeed() const;
  //! Set the rotation speed
  void setRotateSpeed(f32 rotateSpeed);
  //! Returns the zoom speed
  f32 getZoomSpeed() const;
  //! Set the zoom speed
  void setZoomSpeed(f32 zoomSpeed);
  //! Returns the current distance, i.e. orbit radius
  f32 getDistance() const;
  //! Set the distance
  void setDistance(f32 distance);
  //! This animator will receive events when attached to the active camera
  bool isEventReceiverEnabled() const {
    return true;
  }
  //! Returns type of the scene node
  ESCENE_NODE_ANIMATOR_TYPE getType() const {
    return ESNAT_CAMERA_MAYA;
  }
  //! Creates a clone of this animator.
  /** Please note that you will have to drop
  (IReferenceCounted::drop()) the returned pointer after calling
  this. */
  ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
private:
  void allKeysUp();
  void animate();
  bool isMouseKeyDown(s32 key) const;
  bool MouseKeys[3];
  ICursorControl* CursorControl;
  ICameraSceneNode* OldCamera;
  float3 OldTarget;
  float3 LastCameraTarget; // to find out if the camera target was moved outside this animator
  position2df RotateStart;
  position2df ZoomStart;
  position2df TranslateStart;
  position2df MousePos;
  f32 ZoomSpeed;
  f32 RotateSpeed;
  f32 TranslateSpeed;
  f32 CurrentZoom;
  f32 RotX, RotY;
  bool Zooming;
  bool Rotating;
  bool Moving;
  bool Translating;
};
#endif

