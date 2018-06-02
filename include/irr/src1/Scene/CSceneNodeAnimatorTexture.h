// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_NODE_ANIMATOR_TEXTURE_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_TEXTURE_H_INCLUDED__
//#include "irrArray.h"
//#include "ISceneNodeAnimatorFinishing.h"
class CSceneNodeAnimatorTexture : public ISceneNodeAnimatorFinishing
{
public:
  //! constructor
  CSceneNodeAnimatorTexture(const ITexture* > & textures,
      s32 timePerFrame, bool loop, u32 now);
  //! destructor
  ~CSceneNodeAnimatorTexture();
  //! animates a scene node
  void animateNode(ISceneNode* node, u32 timeMs);
  //! Writes attributes of the scene node animator.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node animator.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Returns type of the scene node animator
  ESCENE_NODE_ANIMATOR_TYPE getType() const {
    return ESNAT_TEXTURE;
  }
  //! Creates a clone of this animator.
  /** Please note that you will have to drop
  (IReferenceCounted::drop()) the returned pointer after calling
  this. */
  ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
private:
  void clearTextures();
  ITexture* > Textures;
  u32 TimePerFrame;
  u32 StartTime;
  bool Loop;
};
#endif

