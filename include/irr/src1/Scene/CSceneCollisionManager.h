// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_COLLISION_MANAGER_H_INCLUDED__
#define __C_SCENE_COLLISION_MANAGER_H_INCLUDED__
//#include "ISceneCollisionManager.h"
//#include "ISceneManager.h"
//#include "IVideoDriver.h"
//! The Scene Collision Manager provides methods for performing collision tests and picking on scene nodes.
class CSceneCollisionManager : public ISceneCollisionManager
{
public:
  //! constructor
  CSceneCollisionManager(ISceneManager* smanager, IVideoDriver* driver);
  //! destructor
  ~CSceneCollisionManager();
  //! Returns the scene node, which is currently visible at the given
  //! screen coordinates, viewed from the currently active camera.
  ISceneNode* getSceneNodeFromScreenCoordinatesBB(const position2di& pos,
      s32 idBitMask = 0, bool bNoDebugObjects = false, ISceneNode* root = 0);
  //! Returns the nearest scene node which collides with a 3d ray and
  //! whose id matches a bitmask.
  ISceneNode* getSceneNodeFromRayBB(const F3LINE& ray,
      s32 idBitMask = 0, bool bNoDebugObjects = false,
      ISceneNode* root = 0);
  //! Returns the scene node, at which the overgiven camera is looking at and
  //! which id matches the bitmask.
  ISceneNode* getSceneNodeFromCameraBB(ICameraSceneNode* camera,
      s32 idBitMask = 0, bool bNoDebugObjects = false);
  //! Finds the collision point of a line and lots of triangles, if there is one.
  bool getCollisionPoint(const F3LINE& ray,
      ITriangleSelector* selector, float3 outCollisionPoint,
      triangle3df& outTriangle,
      ISceneNode* outNode);
  //! Collides a moving ellipsoid with a 3d world with gravity and returns
  //! the resulting new position of the ellipsoid.
  float3 getCollisionResultPosition(
      ITriangleSelector* selector,
      const float3 ellipsoidPosition,
      const float3 ellipsoidRadius,
      const float3 ellipsoidDirectionAndSpeed,
      triangle3df& triout,
      float3 hitPosition,
      bool& outFalling,
      ISceneNode* outNode,
      f32 slidingSpeed,
      const float3 gravityDirectionAndSpeed);
  //! Returns a 3d ray which would go through the 2d screen coodinates.
  F3LINE getRayFromScreenCoordinates(
      const position2di& pos, ICameraSceneNode* camera = 0);
  //! Calculates 2d screen position from a 3d position.
  position2di getScreenCoordinatesFrom3DPosition(
      const float3 pos, ICameraSceneNode* camera = 0, bool useViewPort = false);
  //! Gets the scene node and nearest collision point for a ray based on
  //! the nodes' id bitmasks, bounding boxes and triangle selectors.
  ISceneNode* getSceneNodeAndCollisionPointFromRay(
      line3df ray,
      float3 outCollisionPoint,
      triangle3df& outTriangle,
      s32 idBitMask = 0,
      ISceneNode* collisionRootNode = 0,
      bool noDebugObjects = false);
private:
  //! recursive method for going through all scene nodes
  void getPickedNodeBB(ISceneNode* root, line3df& ray, s32 bits,
      bool bNoDebugObjects,
      f32& outbestdistance, ISceneNode* outbestnode);
  //! recursive method for going through all scene nodes
  void getPickedNodeFromBBAndSelector(ISceneNode* root,
      line3df& ray,
      s32 bits,
      bool noDebugObjects,
      f32& outBestDistanceSquared,
      ISceneNode* outBestNode,
      float3 outBestCollisionPoint,
      triangle3df& outBestTriangle);
  struct SCollisionData {
    float3 eRadius;
    float3 R3Velocity;
    float3 R3Position;
    float3 velocity;
    float3 normalizedVelocity;
    float3 basePoint;
    bool foundCollision;
    f32 nearestDistance;
    float3 intersectionPoint;
    triangle3df intersectionTriangle;
    s32 triangleIndex;
    s32 triangleHits;
    f32 slidingSpeed;
    ITriangleSelector* selector;
  };
  //! Tests the current collision data against an individual triangle.
  /**
  \param colData: the collision data.
  \param triangle: the triangle to test against.
  \return true if the triangle is hit (and is the closest hit), false otherwise */
  bool testTriangleIntersection(SCollisionData* colData,
      const triangle3df& triangle);
  //! recursive method for doing collision response
  float3 collideEllipsoidWithWorld(ITriangleSelector* selector,
      const float3 position,
      const float3 radius,  const float3 velocity,
      f32 slidingSpeed,
      const float3 gravity, triangle3df& triout,
      float3 hitPosition,
      bool& outFalling,
      ISceneNode* outNode);
  float3 collideWithWorld(s32 recursionDepth, SCollisionData& colData,
      float3 pos, float3 vel);
  CV_INLINE bool getLowestRoot(f32 a, f32 b, f32 c, f32 maxR, f32* root);
  ISceneManager* SceneManager;
  IVideoDriver* Driver;
  triangle3df > Triangles; // triangle buffer
};
#endif

