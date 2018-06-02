// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_SCENE_NODE_ANIMATOR_H_INCLUDED__
#define __I_SCENE_NODE_ANIMATOR_H_INCLUDED__
//#include "IAttributeExchangingObject.h"
//#include "IEventReceiver.h"
class IAttributes;
class ISceneNode;
class ISceneManager;
//! Animates a scene node. Can animate position, rotation, material, and so on.
/** A scene node animator is able to animate a scene node in a very simple way. It may
change its position, rotation, scale and/or material. There are lots of animators
to choose from. You can create scene node animators with the ISceneManager interface.
*/
class ISceneNodeAnimator : public IAttributeExchangingObject, public IEventReceiver
{
public:
  //! Animates a scene node.
  /** \param node Node to animate.
  \param timeMs Current time in milli seconds. */
  void animateNode(ISceneNode* node, u32 timeMs) = 0;
  //! Creates a clone of this animator.
  /** Please note that you will have to drop
  (IReferenceCounted::drop()) the returned pointer after calling this. */
  ISceneNodeAnimator* createClone(ISceneNode* node,
      ISceneManager* newManager = 0) = 0;
  //! Returns true if this animator receives events.
  /** When attached to an active camera, this animator will be
  able to respond to events such as mouse and keyboard events. */
  bool isEventReceiverEnabled() const {
    return false;
  }
  //! Event receiver, override this function for camera controlling animators
  bool OnEvent(const SEvent& event) {
    return false;
  }
  //! Returns type of the scene node animator
  ESCENE_NODE_ANIMATOR_TYPE getType() const {
    return ESNAT_UNKNOWN;
  }
  //! Returns if the animator has finished.
  /** This is only valid for non-looping animators with a discrete end state.
  \return true if the animator has finished, false if it is still running. */
  bool hasFinished(void) const {
    return false;
  }
};
#endif

