// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SOFTWARE2_MATERIAL_RENDERER_H_INCLUDED__
#define __C_SOFTWARE2_MATERIAL_RENDERER_H_INCLUDED__
//#include "SoftwareDriver2_compile_config.h"
//#include "IMaterialRenderer.h"
//#include "CSoftwareDriver2.h"
//! Base class for all internal Software2 material renderers
class CSoftware2MaterialRenderer : public IMaterialRenderer
{
public:
  //! Constructor
  CSoftware2MaterialRenderer(CBurningVideoDriver* driver)
    : Driver(driver) {
  }
protected:
  CBurningVideoDriver* Driver;
};
//! solid material renderer
class CSoftware2MaterialRenderer_SOLID : public CSoftware2MaterialRenderer
{
public:
  CSoftware2MaterialRenderer_SOLID(CBurningVideoDriver* driver)
    : CSoftware2MaterialRenderer(driver) {}
  //! Returns if the material is transparent.
  bool isTransparent() const {
    return false;
  }
};
//! Transparent material renderer
class CSoftware2MaterialRenderer_TRANSPARENT_ADD_COLOR : public CSoftware2MaterialRenderer
{
public:
  CSoftware2MaterialRenderer_TRANSPARENT_ADD_COLOR(CBurningVideoDriver* driver)
    : CSoftware2MaterialRenderer(driver) {}
  //! Returns if the material is transparent.
  bool isTransparent() const {
    return true;
  }
};
//! unsupported material renderer
class CSoftware2MaterialRenderer_UNSUPPORTED : public CSoftware2MaterialRenderer
{
public:
  CSoftware2MaterialRenderer_UNSUPPORTED(CBurningVideoDriver* driver)
    : CSoftware2MaterialRenderer(driver) {}
  s32 getRenderCapability() const {
    return 1;
  }
};
//! unsupported material renderer
class CBurningShader_REFERENCE : public CSoftware2MaterialRenderer
{
public:
  CBurningShader_REFERENCE(CBurningVideoDriver* driver)
    : CSoftware2MaterialRenderer(driver) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
  }
  void OnUnsetMaterial() {
  }
  bool isTransparent() const {
    return false;
  }
  bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype) {
    return true;
  };
  s32 getRenderCapability() const {
    return 1;
  }
};
#endif

