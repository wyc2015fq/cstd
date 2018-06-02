// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_MESH_VIEWER_H_INCLUDED__
#define __I_GUI_MESH_VIEWER_H_INCLUDED__
//#include "IGUIElement.h"
class SMaterial;
class IAnimatedMesh;
//! 3d mesh viewing GUI element.
class IGUIMeshViewer : public IGUIElement
{
public:
  //! constructor
  IGUIMeshViewer(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_MESH_VIEWER, environment, parent, id, rectangle) {}
  //! Sets the mesh to be shown
  void setMesh(IAnimatedMesh* mesh) = 0;
  //! Gets the displayed mesh
  IAnimatedMesh* getMesh() = 0;
  //! Sets the material
  void setMaterial(const SMaterial& material) = 0;
  //! Gets the material
  const SMaterial& getMaterial() = 0;
};
#endif

