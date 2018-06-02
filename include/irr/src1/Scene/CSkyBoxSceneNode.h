// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SKY_BOX_SCENE_NODE_H_INCLUDED__
#define __C_SKY_BOX_SCENE_NODE_H_INCLUDED__
//#include "ISceneNode.h"
// Skybox, rendered with zbuffer turned off, before all other nodes.
class CSkyBoxSceneNode : public ISceneNode
{
public:
  //! constructor
  CSkyBoxSceneNode(ITexture* top, ITexture* bottom, ITexture* left,
      ITexture* right, ITexture* front, ITexture* back,
      ISceneNode* parent, ISceneManager* mgr, s32 id);
  void OnRegisterSceneNode();
  //! renders the node.
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! returns the material based on the zero based index i. To get the amount
  //! of materials used by this scene node, use getMaterialCount().
  //! This function is needed for inserting the node into the scene hirachy on a
  //! optimal position for minimizing renderstate changes, but can also be used
  //! to directly modify the material of a scene node.
  SMaterial& getMaterial(u32 i);
  //! returns amount of materials used by this scene node.
  u32 getMaterialCount() const;
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_SKY_BOX;
  }
  //! Creates a clone of this scene node and its children.
  ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);
private:
  F3AABBOX Box;
  u16 Indices[4];
  S3DVertex Vertices[4 * 6];
  SMaterial Material[6];
};
#endif

