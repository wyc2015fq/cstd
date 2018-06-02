// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_NODE_ANIMATOR_COLLISION_RESPONSE_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_COLLISION_RESPONSE_H_INCLUDED__
//#include "ISceneNodeAnimatorCollisionResponse.h"
//! Special scene node animator for doing automatic collision detection and response.
/** This scene node animator can be attached to any scene node modifying it in that
way, that it cannot move through walls of the world, is influenced by gravity and
acceleration. This animator is useful for example for first person shooter
games. Attach it for example to a first person shooter camera, and the camera will
behave as the player control in a first person shooter game: The camera stops and
slides at walls, walks up stairs, falls down if there is no floor under it, and so on.
*/
class CSceneNodeAnimatorCollisionResponse : public ISceneNodeAnimatorCollisionResponse
{
public:
  //! constructor
  CSceneNodeAnimatorCollisionResponse(ISceneManager* scenemanager,
      ITriangleSelector* world, ISceneNode* object,
      const float3 ellipsoidRadius = float3(30, 60, 30),
      const float3 gravityPerSecond = float3(0, -100.0f, 0),
      const float3 ellipsoidTranslation = float3(0, 0, 0),
      f32 slidingSpeed = 0.0005f);
  //! destructor
  ~CSceneNodeAnimatorCollisionResponse();
  //! Returns if the attached scene node is falling, which means that
  //! there is no blocking wall from the scene node in the direction of
  //! the gravity.
  bool isFalling() const;
  //! Sets the radius of the ellipsoid with which collision detection and
  //! response is done.
  void setEllipsoidRadius(const float3 radius);
  //! Returns the radius of the ellipsoid with which the collision detection and
  //! response is done.
  float3 getEllipsoidRadius() const;
  //! Sets the gravity of the environment.
  void setGravity(const float3 gravity);
  //! 'Jump' the animator, by adding a jump speed opposite to its gravity
  void jump(f32 jumpSpeed);
  //! Should the Target react on collision ( default = true )
  void setAnimateTarget(bool enable);
  bool getAnimateTarget() const;
  //! Returns current vector of gravity.
  float3 getGravity() const;
  //! Sets the translation of the ellipsoid for collision detection.
  void setEllipsoidTranslation(const float3 translation);
  //! Returns the translation of the ellipsoid for collision detection.
  float3 getEllipsoidTranslation() const;
  //! Sets a triangle selector holding all triangles of the world with which
  //! the scene node may collide.
  void setWorld(ITriangleSelector* newWorld);
  //! Returns the current triangle selector containing all triangles for
  //! collision detection.
  ITriangleSelector* getWorld() const;
  //! animates a scene node
  void animateNode(ISceneNode* node, u32 timeMs);
  //! Writes attributes of the scene node animator.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node animator.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Returns type of the scene node animator
  ESCENE_NODE_ANIMATOR_TYPE getType() const {
    return ESNAT_COLLISION_RESPONSE;
  }
  //! Creates a clone of this animator.
  /** Please note that you will have to drop
  (IReferenceCounted::drop()) the returned pointer after calling
  this. */
  ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
  //! Set the single node that this animator will act on.
  void setTargetNode(ISceneNode* node) {
    setNode(node);
  }
  //! Gets the single node that this animator is acting on.
  ISceneNode* getTargetNode(void) const {
    return Object;
  }
  //! Returns true if a collision occurred during the last animateNode()
  bool collisionOccurred() const {
    return CollisionOccurred;
  }
  //! Returns the last point of collision.
  const float3 getCollisionPoint() const {
    return CollisionPoint;
  }
  //! Returns the last triangle that caused a collision.
  const triangle3df& getCollisionTriangle() const {
    return CollisionTriangle;
  }
  const float3 getCollisionResultPosition(void) const {
    return CollisionResultPosition;
  }
  ISceneNode* getCollisionNode(void) const {
    return CollisionNode;
  }
  //! Sets a callback interface which will be called if a collision occurs.
  /** \param callback: collision callback handler that will be called when a collision
  occurs. Set this to 0 to disable the callback.
  */
  void setCollisionCallback(ICollisionCallback* callback);
private:
  void setNode(ISceneNode* node);
  float3 Radius;
  float3 Gravity;
  float3 Translation;
  float3 FallingVelocity; // In the direction of Gravity.
  float3 LastPosition;
  triangle3df RefTriangle;
  ITriangleSelector* World;
  ISceneNode* Object;
  ISceneManager* SceneManager;
  u32 LastTime;
  f32 SlidingSpeed;
  float3 CollisionPoint;
  triangle3df CollisionTriangle;
  float3 CollisionResultPosition;
  ISceneNode* CollisionNode;
  ICollisionCallback* CollisionCallback;
  bool Falling;
  bool IsCamera;
  bool AnimateCameraTarget;
  bool CollisionOccurred;
  bool FirstUpdate;
};
#endif

