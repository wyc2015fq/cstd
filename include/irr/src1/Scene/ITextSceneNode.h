// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_TEXT_SCENE_NODE_H_INCLUDED__
#define __I_TEXT_SCENE_NODE_H_INCLUDED__
//#include "ISceneNode.h"
//! A scene node for displaying 2d text at a position in three dimensional space
class ITextSceneNode : public ISceneNode
{
public:
  //! constructor
  ITextSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position = float3(0, 0, 0))
    : ISceneNode(parent, mgr, id, position) {}
  //! sets the text string
  void setText(const char* text) = 0;
  //! sets the color of the text
  void setTextColor(SColor color) = 0;
};
#endif

