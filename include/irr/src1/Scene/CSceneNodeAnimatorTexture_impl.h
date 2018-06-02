// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CSceneNodeAnimatorTexture.h"
//#include "ITexture.h"
//! constructor
CSceneNodeAnimatorTexture::CSceneNodeAnimatorTexture(const ITexture* > & textures,
    s32 timePerFrame, bool loop, u32 now)
  : ISceneNodeAnimatorFinishing(0),
    TimePerFrame(timePerFrame), StartTime(now), Loop(loop)
{
#ifdef _DEBUG
  setDebugName("CSceneNodeAnimatorTexture");
#endif
  int i;
  for (i = 0; i < textures_size; ++i) {
    if (textures[i]) {
      textures[i]->grab();
    }
    Textures.push_back(textures[i]);
  }
  FinishTime = now + (timePerFrame * Textures_size);
}
//! destructor
CSceneNodeAnimatorTexture::~CSceneNodeAnimatorTexture()
{
  clearTextures();
}
void CSceneNodeAnimatorTexture::clearTextures()
{
  int i;
  for (i = 0; i < Textures_size; ++i)
    if (Textures[i]) {
      Textures[i]->drop();
    }
}
//! animates a scene node
void CSceneNodeAnimatorTexture::animateNode(ISceneNode* node, u32 timeMs)
{
  if (!node) {
    return;
  }
  if (Textures_size) {
    const u32 t = (timeMs - StartTime);
    u32 idx = 0;
    if (!Loop && timeMs >= FinishTime) {
      idx = Textures_size - 1;
      HasFinished = true;
    }
    else {
      idx = (t / TimePerFrame) % Textures_size;
    }
    if (idx < Textures_size) {
      node->setMaterialTexture(0, Textures[idx]);
    }
  }
}
//! Writes attributes of the scene node animator.
void CSceneNodeAnimatorTexture::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
{
  out->addInt("TimePerFrame", TimePerFrame);
  out->addBool("Loop", Loop);
  // add one texture in addition when serializing for editors
  // to make it easier to add textures quickly
  u32 count = Textures_size;
  if (options && (options->Flags & EARWF_FOR_EDITOR)) {
    count += 1;
  }
  int i;
  for (i = 0; i < count; ++i) {
    char* tname = "Texture";
    tname += (int)(i + 1);
    out->addTexture(tname, i < Textures_size ? Textures[i] : 0);
  }
}
//! Reads attributes of the scene node animator.
void CSceneNodeAnimatorTexture::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
{
  TimePerFrame = in->getAttributeAsInt("TimePerFrame");
  Loop = in->getAttributeAsBool("Loop");
  clearTextures();
  int i;
  for (i = 1; true; ++i) {
    char* tname = "Texture";
    tname += (int)i;
    if (in->existsAttribute(tname)) {
      ITexture* tex = in->getAttributeAsTexture(tname);
      if (tex) {
        tex->grab();
        Textures.push_back(tex);
      }
    }
    else {
      break;
    }
  }
}
ISceneNodeAnimator* CSceneNodeAnimatorTexture::createClone(ISceneNode* node, ISceneManager* newManager)
{
  CSceneNodeAnimatorTexture* newAnimator =
      new CSceneNodeAnimatorTexture(Textures, TimePerFrame, Loop, StartTime);
  return newAnimator;
}

