// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_TEXT_SCENE_NODE_H_INCLUDED__
#define __C_TEXT_SCENE_NODE_H_INCLUDED__
//#include "ITextSceneNode.h"
//#include "IBillboardTextSceneNode.h"
//#include "IGUIFont.h"
//#include "IGUIFontBitmap.h"
//#include "ISceneCollisionManager.h"
//#include "SMesh.h"
class CTextSceneNode : public ITextSceneNode
{
public:
  //! constructor
  CTextSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      IGUIFont* font, ISceneCollisionManager* coll,
      const float3 position = float3(0, 0, 0), const char* text = 0,
      SColor color = SColor(100, 0, 0, 0));
  //! destructor
  ~CTextSceneNode();
  void OnRegisterSceneNode();
  //! renders the node.
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! sets the text string
  void setText(const char* text);
  //! sets the color of the text
  void setTextColor(SColor color);
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_TEXT;
  }
private:
  char* Text;
  SColor Color;
  IGUIFont* Font;
  ISceneCollisionManager* Coll;
  F3AABBOX Box;
};
class CBillboardTextSceneNode : public IBillboardTextSceneNode
{
public:
  CBillboardTextSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      IGUIFont* font, const char* text,
      const float3 position, const dimension2df& size,
      SColor colorTop, SColor shade_bottom);
  //! destructor
  ~CBillboardTextSceneNode();
  //! sets the vertex positions etc
  void OnAnimate(u32 timeMs);
  //! registers the node into the transparent pass
  void OnRegisterSceneNode();
  //! renders the node.
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! sets the text string
  void setText(const char* text);
  //! sets the color of the text
  void setTextColor(SColor color);
  //! sets the size of the billboard
  void setSize(const dimension2df& size);
  //! gets the size of the billboard
  const dimension2df& getSize() const;
  SMaterial& getMaterial(u32 i);
  //! returns amount of materials used by this scene node.
  u32 getMaterialCount() const;
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_TEXT;
  }
  //! Set the color of all vertices of the billboard
  //! \param overallColor: the color to set
  void setColor(const SColor& overallColor);
  //! Set the color of the top and bottom vertices of the billboard
  //! \param topColor: the color to set the top vertices
  //! \param bottomColor: the color to set the bottom vertices
  void setColor(const SColor& topColor, const SColor& bottomColor);
  //! Gets the color of the top and bottom vertices of the billboard
  //! \param topColor: stores the color of the top vertices
  //! \param bottomColor: stores the color of the bottom vertices
  void getColor(SColor& topColor, SColor& bottomColor) const;
  void setSize(f32 height, f32 bottomEdgeWidth, f32 topEdgeWidth) {
    setSize(dimension2df(bottomEdgeWidth, height));
  }
  void getSize(f32& height, f32& bottomEdgeWidth, f32& topEdgeWidth) const {
    height = Size.Height;
    bottomEdgeWidth = Size.Width;
    topEdgeWidth = Size.Width;
  }
private:
  char* Text;
  SColor Color;
  IGUIFontBitmap* Font;
  dimension2df Size;
  F3AABBOX BBox;
  SMaterial Material;
  SColor ColorTop;
  SColor ColorBottom;
  struct SSymbolInfo {
    u32 bufNo;
    f32 Width;
    f32 Kerning;
    u32 firstInd;
    u32 firstVert;
  };
  SSymbolInfo > Symbol;
  SMesh* Mesh;
};
#endif

