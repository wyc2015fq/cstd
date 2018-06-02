// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// Code for this scene node has been contributed by Anders la Cour-Harbo (alc)
#ifndef __C_SKY_DOME_SCENE_NODE_H_INCLUDED__
#define __C_SKY_DOME_SCENE_NODE_H_INCLUDED__
//#include "ISceneNode.h"
//#include "SMeshBuffer.h"
class CSkyDomeSceneNode : public ISceneNode
{
public:
  CSkyDomeSceneNode(ITexture* texture, u32 horiRes, u32 vertRes,
      f32 texturePercentage, f32 spherePercentage, f32 radius,
      ISceneNode* parent, ISceneManager* smgr, s32 id);
  ~CSkyDomeSceneNode();
  void OnRegisterSceneNode();
  void render();
  const F3AABBOX& getBoundingBox() const;
  SMaterial& getMaterial(u32 i);
  u32 getMaterialCount() const;
  ESCENE_NODE_TYPE getType() const {
    return ESNT_SKY_DOME;
  }
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
  ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);
private:
  void generateMesh();
  SMeshBuffer* Buffer;
  u32 HorizontalResolution, VerticalResolution;
  f32 TexturePercentage, SpherePercentage, Radius;
};
}
}
#endif

