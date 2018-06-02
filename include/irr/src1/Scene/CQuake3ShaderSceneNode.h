// Copyright (C) 2002-2012 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_QUAKE3_SCENE_NODE_H_INCLUDED__
#define __C_QUAKE3_SCENE_NODE_H_INCLUDED__
//#include "IMeshSceneNode.h"
//#include "IQ3Shader.h"
//#include "IFileSystem.h"
//#include "SMeshBuffer.h"
//#include "SMeshBufferLightMap.h"
//#include "SMesh.h"
//#include "ISceneManager.h"
//! Scene node which is a quake3 shader.
class CQuake3ShaderSceneNode : public IMeshSceneNode
{
public:
  CQuake3ShaderSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      IFileSystem* fileSystem,
      const IMeshBuffer* original,
      const quake3::IShader* shader
                        );
  ~CQuake3ShaderSceneNode();
  void OnRegisterSceneNode();
  void render();
  void OnAnimate(u32 timeMs);
  const F3AABBOX& getBoundingBox() const;
  u32 getMaterialCount() const;
  SMaterial& getMaterial(u32 i);
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_Q3SHADER_SCENE_NODE;
  }
  void setMesh(IMesh* mesh) {}
  IMesh* getMesh() {
    return Mesh;
  }
  void setReadOnlyMaterials(bool readonly) {}
  bool isReadOnlyMaterials() const {
    return true;
  }
  //! Creates shadow volume scene node as child of this node
  //! and returns a pointer to it.
  IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh,
      s32 id, bool zfailmethod = true, f32 infinity = 10000.0f);
  //! Removes a child from this scene node.
  //! Implemented here, to be able to remove the shadow properly, if there is one,
  //! or to remove attached childs.
  bool removeChild(ISceneNode* child);
private:
  const quake3::IShader* Shader;
  SMesh* Mesh;
  IShadowVolumeSceneNode* Shadow;
  const SMeshBufferLightMap* Original;
  SMeshBuffer* MeshBuffer;
  float3 MeshOffset;
  struct SQ3Texture {
    SQ3Texture() :
      TextureIndex(0),
      TextureFrequency(0.f),
      TextureAddressMode(ETC_REPEAT) {
      Texture.setAllocStrategy(ALLOC_STRATEGY_SAFE);
    }
    quake3::tTexArray Texture;
    u32 TextureIndex;
    f32 TextureFrequency;
    E_TEXTURE_CLAMP TextureAddressMode;  // Wrapping/Clamping
  };
  SQ3Texture > Q3Texture;
  void loadTextures(IFileSystem* fileSystem);
  void addBuffer(SMeshBufferLightMap* buffer);
  void cloneBuffer(SMeshBuffer* dest, const SMeshBufferLightMap* buffer, bool translateCenter);
  void deformvertexes_wave(f32 dt, quake3::SModifierFunction& function);
  void deformvertexes_move(f32 dt, quake3::SModifierFunction& function);
  void deformvertexes_bulge(f32 dt, quake3::SModifierFunction& function);
  void deformvertexes_autosprite(f32 dt, quake3::SModifierFunction& function);
  void deformvertexes_autosprite2(f32 dt, quake3::SModifierFunction& function);
  void deformvertexes_normal(f32 dt, quake3::SModifierFunction& function);
  void vertextransform_tcgen(f32 dt, quake3::SModifierFunction& function);
  void vertextransform_rgbgen(f32 dt, quake3::SModifierFunction& function);
  void vertextransform_alphagen(f32 dt, quake3::SModifierFunction& function);
  void transformtex(const matrix4& m, const u32 clamp);
  f32 TimeAbs;
  void animate(u32 stage, matrix4& texture);
  E_SCENE_NODE_RENDER_PASS getRenderStage() const;
};
#endif

