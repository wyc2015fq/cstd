// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __C_BILLBOARD_SCENE_NODE_H_INCLUDED__
//#include "IBillboardSceneNode.h"
//! Scene node which is a billboard. A billboard is like a 3d sprite: A 2d element,
//! which always looks to the camera.
class CBillboardSceneNode : public IBillboardSceneNode
{
public:
  //! constructor
  CBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position, const dimension2df& size,
      SColor colorTop = SColor(0xFFFFFFFF),
      SColor colorBottom = SColor(0xFFFFFFFF));
  //! pre render event
  void OnRegisterSceneNode();
  //! render
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! sets the size of the billboard
  void setSize(const dimension2df& size);
  //! Sets the widths of the top and bottom edges of the billboard independently.
  void setSize(f32 height, f32 bottomEdgeWidth, f32 topEdgeWidth);
  //! gets the size of the billboard
  const dimension2df& getSize() const;
  //! Gets the widths of the top and bottom edges of the billboard.
  void getSize(f32& height, f32& bottomEdgeWidth, f32& topEdgeWidth) const;
  SMaterial& getMaterial(u32 i);
  //! returns amount of materials used by this scene node.
  u32 getMaterialCount() const;
  //! Set the color of all vertices of the billboard
  //! \param overallColor: the color to set
  void setColor(const SColor& overallColor);
  //! Set the color of the top and bottom vertices of the billboard
  //! \param topColor: the color to set the top vertices
  //! \param bottomColor: the color to set the bottom vertices
  void setColor(const SColor& topColor,
      const SColor& bottomColor);
  //! Gets the color of the top and bottom vertices of the billboard
  //! \param[out] topColor: stores the color of the top vertices
  //! \param[out] bottomColor: stores the color of the bottom vertices
  void getColor(SColor& topColor,
      SColor& bottomColor) const;
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_BILLBOARD;
  }
  //! Creates a clone of this scene node and its children.
  ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);
private:
  //! Size.Width is the bottom edge width
  dimension2df Size;
  f32 TopEdgeWidth;
  F3AABBOX BBox;
  SMaterial Material;
  S3DVertex vertices[4];
  u16 indices[6];
};
#endif

