// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//New skinned mesh
#ifndef __C_SKINNED_MESH_H_INCLUDED__
#define __C_SKINNED_MESH_H_INCLUDED__
//#include "ISkinnedMesh.h"
//#include "SMeshBuffer.h"
//#include "irrString.h"
//#include "quaternion.h"
class IAnimatedMeshSceneNode;
class IBoneSceneNode;
class CSkinnedMesh: public ISkinnedMesh
{
public:
  //! constructor
  CSkinnedMesh();
  //! destructor
  ~CSkinnedMesh();
  //! returns the amount of frames. If the amount is 1, it is a static (=non animated) mesh.
  u32 getFrameCount() const;
  //! Gets the default animation speed of the animated mesh.
  /** \return Amount of frames per second. If the amount is 0, it is a static, non animated mesh. */
  f32 getAnimationSpeed() const;
  //! Gets the frame count of the animated mesh.
  /** \param fps Frames per second to play the animation with. If the amount is 0, it is not animated.
  The actual speed is set in the scene node the mesh is instantiated in.*/
  void setAnimationSpeed(f32 fps);
  //! returns the animated mesh based on a detail level (which is ignored)
  IMesh* getMesh(s32 frame, s32 detailLevel = 255, s32 startFrameLoop = -1, s32 endFrameLoop = -1);
  //! Animates this mesh's joints based on frame input
  //! blend: {0-old position, 1-New position}
  void animateMesh(f32 frame, f32 blend);
  //! Preforms a software skin on this mesh based of joint positions
  void skinMesh();
  //! returns amount of mesh buffers.
  u32 getMeshBufferCount() const;
  //! returns pointer to a mesh buffer
  IMeshBuffer* getMeshBuffer(u32 nr) const;
  //! Returns pointer to a mesh buffer which fits a material
  /** \param material: material to search for
  \return Returns the pointer to the mesh buffer or
  NULL if there is no such mesh buffer. */
  IMeshBuffer* getMeshBuffer(const SMaterial& material) const;
  //! returns an axis aligned bounding box
  const F3AABBOX& getBoundingBox() const;
  //! set user axis aligned bounding box
  void setBoundingBox(const aabbox3df& box);
  //! sets a flag of all contained materials to a new value
  void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue);
  //! set the hardware mapping hint, for driver
  void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX);
  //! flags the meshbuffer as changed, reloads hardware buffers
  void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX);
  //! Returns the type of the animated mesh.
  E_ANIMATED_MESH_TYPE getMeshType() const;
  //! Gets joint count.
  u32 getJointCount() const;
  //! Gets the name of a joint.
  const c8* getJointName(u32 number) const;
  //! Gets a joint number from its name
  s32 getJointNumber(const c8* name) const;
  //! uses animation from another mesh
  bool useAnimationFrom(const ISkinnedMesh* mesh);
  //! Update Normals when Animating
  //! False= Don't (default)
  //! True = Update normals, slower
  void updateNormalsWhenAnimating(bool on);
  //! Sets Interpolation Mode
  void setInterpolationMode(E_INTERPOLATION_MODE mode);
  //! Convertes the mesh to contain tangent information
  void convertMeshToTangents();
  //! Does the mesh have no animation
  bool isStatic();
  //! (This feature is not implemented in irrlicht yet)
  bool setHardwareSkinning(bool on);
  //Interface for the mesh loaders (finalize should lock these functions, and they should have some prefix like loader_
  //these functions will use the needed arrays, set values, etc to help the loaders
  //! exposed for loaders to add mesh buffers
  SSkinMeshBuffer* > & getMeshBuffers();
  //! alternative method for adding joints
  SJoint** getAllJoints();
  //! alternative method for adding joints
  const SJoint** getAllJoints() const;
  //! loaders should call this after populating the mesh
  void finalize();
  //! Adds a new meshbuffer to the mesh, access it as last one
  SSkinMeshBuffer* addMeshBuffer();
  //! Adds a new joint to the mesh, access it as last one
  SJoint* addJoint(SJoint* parent = 0);
  //! Adds a new position key to the mesh, access it as last one
  SPositionKey* addPositionKey(SJoint* joint);
  //! Adds a new rotation key to the mesh, access it as last one
  SRotationKey* addRotationKey(SJoint* joint);
  //! Adds a new scale key to the mesh, access it as last one
  SScaleKey* addScaleKey(SJoint* joint);
  //! Adds a new weight to the mesh, access it as last one
  SWeight* addWeight(SJoint* joint);
  void updateBoundingBox(void);
  //! Recovers the joints from the mesh
  void recoverJointsFromMesh(IBoneSceneNode* > & jointChildSceneNodes);
  //! Tranfers the joint data to the mesh
  void transferJointsToMesh(const IBoneSceneNode* > & jointChildSceneNodes);
  //! Tranfers the joint hints to the mesh
  void transferOnlyJointsHintsToMesh(const IBoneSceneNode* > & jointChildSceneNodes);
  //! Creates an array of joints from this mesh as children of node
  void addJoints(IBoneSceneNode* > & jointChildSceneNodes,
      IAnimatedMeshSceneNode* node,
      ISceneManager* smgr);
private:
  void checkForAnimation();
  void normalizeWeights();
  void buildAllLocalAnimatedMatrices();
  void buildAllGlobalAnimatedMatrices(SJoint* Joint = 0, SJoint* ParentJoint = 0);
  void getFrameData(f32 frame, SJoint* Node,
      float3 position, s32& positionHint,
      float3 scale, s32& scaleHint,
      quaternion& rotation, s32& rotationHint);
  void calculateGlobalMatrices(SJoint* Joint, SJoint* ParentJoint);
  void skinJoint(SJoint* Joint, SJoint* ParentJoint);
  void calculateTangents(float3 normal,
      float3 tangent, float3 binormal,
      float3 vt1, float3 vt2, float3 vt3,
      vector2df& tc1, vector2df& tc2, vector2df& tc3);
  SSkinMeshBuffer* > * SkinningBuffers; //Meshbuffer to skin, default is to skin localBuffers
  SSkinMeshBuffer* > LocalBuffers;
  SJoint** AllJoints;
  SJoint** RootJoints;
  bool > > Vertices_Moved;
  F3AABBOX BoundingBox;
  f32 AnimationFrames;
  f32 FramesPerSecond;
  f32 LastAnimatedFrame;
  bool SkinnedLastFrame;
  E_INTERPOLATION_MODE InterpolationMode: 8;
  bool HasAnimation;
  bool PreparedForSkinning;
  bool AnimateNormals;
  bool HardwareSkinning;
};
#endif

