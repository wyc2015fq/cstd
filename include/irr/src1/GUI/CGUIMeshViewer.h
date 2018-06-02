// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_MESH_VIEWER_H_INCLUDED__
#define __C_GUI_MESH_VIEWER_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIMeshViewer.h"
class CGUIMeshViewer : public IGUIMeshViewer
{
public:
  //! constructor
  CGUIMeshViewer(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle);
  //! destructor
  ~CGUIMeshViewer();
  //! sets the mesh to be shown
  void setMesh(IAnimatedMesh* mesh);
  //! Gets the displayed mesh
  IAnimatedMesh* getMesh() const;
  //! sets the material
  void setMaterial(const SMaterial& material);
  //! gets the material
  const SMaterial& getMaterial() const;
  //! called if an event happened.
  bool OnEvent(const SEvent& event);
  //! draws the element and its children
  void draw();
private:
  SMaterial Material;
  IAnimatedMesh* Mesh;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_MESH_VIEWER_H_INCLUDED__

