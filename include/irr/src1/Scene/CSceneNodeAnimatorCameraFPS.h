// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_NODE_ANIMATOR_CAMERA_FPS_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_CAMERA_FPS_H_INCLUDED__
//#include "ISceneNodeAnimatorCameraFPS.h"
//#include "SKeyMap.h"
//#include "irrArray.h"
class ICursorControl;
}
//! Special scene node animator for FPS cameras
class CSceneNodeAnimatorCameraFPS : public ISceneNodeAnimatorCameraFPS
{
public:
  //! Constructor
  CSceneNodeAnimatorCameraFPS(ICursorControl* cursorControl,
      f32 rotateSpeed = 100.0f, f32 moveSpeed = .5f, f32 jumpSpeed = 0.f,
      SKeyMap* keyMapArray = 0, u32 keyMapSize = 0, bool noVerticalMovement = false,
      bool invertY = false);
  //! Destructor
  ~CSceneNodeAnimatorCameraFPS();
  //! Animates the scene node, currently only works on cameras
  void animateNode(ISceneNode* node, u32 timeMs);
  //! Event receiver
  bool OnEvent(const SEvent& event);
  //! Returns the speed of movement in units per second
  f32 getMoveSpeed() const;
  //! Sets the speed of movement in units per second
  void setMoveSpeed(f32 moveSpeed);
  //! Returns the rotation speed
  f32 getRotateSpeed() const;
  //! Set the rotation speed
  void setRotateSpeed(f32 rotateSpeed);
  //! Sets the keyboard mapping for this animator (old style)
  //! \param keymap: an array of keyboard mappings, see SKeyMap
  //! \param count: the size of the keyboard map array
  void setKeyMap(SKeyMap* map, u32 count);
  //! Sets the keyboard mapping for this animator
  //! \param keymap The new keymap array
  void setKeyMap(const SKeyMap > & keymap);
  //! Gets the keyboard mapping for this animator
  const SKeyMap > & getKeyMap() const;
  //! Sets whether vertical movement should be allowed.
  void setVerticalMovement(bool allow);
  //! Sets whether the Y axis of the mouse should be inverted.
  /** If enabled then moving the mouse down will cause
  the camera to look up. It is disabled by default. */
  void setInvertMouse(bool invert);
  //! This animator will receive events when attached to the active camera
  bool isEventReceiverEnabled() const {
    return true;
  }
  //! Returns the type of this animator
  ESCENE_NODE_ANIMATOR_TYPE getType() const {
    return ESNAT_CAMERA_FPS;
  }
  //! Creates a clone of this animator.
  /** Please note that you will have to drop
  (IReferenceCounted::drop()) the returned pointer once you're
  done with it. */
  ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
private:
  void allKeysUp();
  ICursorControl* CursorControl;
  f32 MaxVerticalAngle;
  f32 MoveSpeed;
  f32 RotateSpeed;
  f32 JumpSpeed;
  // -1.0f for inverted mouse, defaults to 1.0f
  f32 MouseYDirection;
  s32 LastAnimationTime;
  SKeyMap > KeyMap;
  position2df CenterCursor, CursorPos;
  bool CursorKeys[EKA_COUNT];
  bool firstUpdate;
  bool firstInput;
  bool NoVerticalMovement;
};
#endif // __C_SCENE_NODE_ANIMATOR_CAMERA_FPS_H_INCLUDED__

