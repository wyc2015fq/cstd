// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_OPENGL_NORMAL_MAP_RENDERER_H_INCLUDED__
#define __C_OPENGL_NORMAL_MAP_RENDERER_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_OPENGL_
//#include "COpenGLShaderMaterialRenderer.h"
//#include "IShaderConstantSetCallBack.h"
//! Class for rendering normal maps with OpenGL
class COpenGLNormalMapRenderer : public COpenGLShaderMaterialRenderer, public IShaderConstantSetCallBack
{
public:
  //! Constructor
  COpenGLNormalMapRenderer(COpenGLDriver* driver,
      s32& outMaterialTypeNr, IMaterialRenderer* baseMaterial);
  //! Destructor
  ~COpenGLNormalMapRenderer();
  //! Called by the engine when the vertex and/or pixel shader constants for an
  //! material renderer should be set.
  void OnSetConstants(IMaterialRendererServices* services, s32 userData);
  //! Returns the render capability of the material.
  s32 getRenderCapability() const;
protected:
  bool CompiledShaders;
};
#endif
#endif

