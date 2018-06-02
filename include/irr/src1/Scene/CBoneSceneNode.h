// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_BONE_SCENE_NODE_H_INCLUDED__
#define __C_BONE_SCENE_NODE_H_INCLUDED__
// Used with SkinnedMesh and IAnimatedMeshSceneNode, for boned meshes
//#include "IBoneSceneNode.h"
class CBoneSceneNode : public IBoneSceneNode
{
public:
  //! constructor
  CBoneSceneNode(ISceneNode* parent, ISceneManager* mgr,
      s32 id = -1, u32 boneIndex = 0, const c8* boneName = 0);
  //! Returns the index of the bone
  u32 getBoneIndex() const;
  //! Sets the animation mode of the bone. Returns true if successful.
  bool setAnimationMode(E_BONE_ANIMATION_MODE mode);
  //! Gets the current animation mode of the bone
  E_BONE_ANIMATION_MODE getAnimationMode() const;
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  /*
  //! Returns the relative transformation of the scene node.
  //virtual matrix4 getRelativeTransformation() const;
  */
  void OnAnimate(u32 timeMs);
  void updateAbsolutePositionOfAllChildren();
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! How the relative transformation of the bone is used
  void setSkinningSpace(E_BONE_SKINNING_SPACE space) {
    SkinningSpace = space;
  }
  E_BONE_SKINNING_SPACE getSkinningSpace() const {
    return SkinningSpace;
  }
private:
  void helper_updateAbsolutePositionOfAllChildren(ISceneNode* Node);
  u32 BoneIndex;
  F3AABBOX Box;
  E_BONE_ANIMATION_MODE AnimationMode;
  E_BONE_SKINNING_SPACE SkinningSpace;
};
#endif

