// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_DUMMY_TRANSFORMATION_SCENE_NODE_H_INCLUDED__
#define __C_DUMMY_TRANSFORMATION_SCENE_NODE_H_INCLUDED__
//#include "IDummyTransformationSceneNode.h"
class CDummyTransformationSceneNode : public IDummyTransformationSceneNode
{
public:
  //! constructor
  CDummyTransformationSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id);
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! Returns a reference to the current relative transformation matrix.
  //! This is the matrix, this scene node uses instead of scale, translation
  //! and rotation.
  matrix4& getRelativeTransformationMatrix();
  //! Returns the relative transformation of the scene node.
  matrix4 getRelativeTransformation() const;
  //! does nothing.
  void render() {}
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_DUMMY_TRANSFORMATION;
  }
  //! Creates a clone of this scene node and its children.
  ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);
private:
  // TODO: We can add least add some warnings to find troubles faster until we have
  // fixed bug id 2318691.
  const float3 getScale() const;
  void setScale(const float3 scale);
  const float3 getRotation() const;
  void setRotation(const float3 rotation);
  const float3 getPosition() const;
  void setPosition(const float3 newpos);
  matrix4 RelativeTransformationMatrix;
  F3AABBOX Box;
};
#endif

