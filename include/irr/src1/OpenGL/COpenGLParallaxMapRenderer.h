// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_OPENGL_PARALLAX_MAP_RENDERER_H_INCLUDED__
#define __C_OPENGL_PARALLAX_MAP_RENDERER_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_OPENGL_
//#include "COpenGLShaderMaterialRenderer.h"
//#include "IShaderConstantSetCallBack.h"
//! Class for rendering normal maps with OpenGL
class COpenGLParallaxMapRenderer : public COpenGLShaderMaterialRenderer, public IShaderConstantSetCallBack
{
public:
  //! Constructor
  COpenGLParallaxMapRenderer(COpenGLDriver* driver,
      s32& outMaterialTypeNr, IMaterialRenderer* baseMaterial);
  //! Destructor
  ~COpenGLParallaxMapRenderer();
  //! Called by the engine when the vertex and/or pixel shader constants for an
  //! material renderer should be set.
  void OnSetConstants(IMaterialRendererServices* services, s32 userData);
  //! Returns the render capability of the material.
  s32 getRenderCapability() const;
  void OnSetMaterial(const SMaterial& material) { }
  void OnSetMaterial(const SMaterial& material,
      const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services);
protected:
  bool CompiledShaders;
  f32 CurrentScale;
};
#endif
#endif

