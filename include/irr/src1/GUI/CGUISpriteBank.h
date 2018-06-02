// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_SPRITE_BANK_H_INCLUDED__
#define __C_GUI_SPRITE_BANK_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUISpriteBank.h"
class IVideoDriver;
class ITexture;
}
class IGUIEnvironment;
//! Sprite bank interface.
class CGUISpriteBank : public IGUISpriteBank
{
public:
  CGUISpriteBank(IGUIEnvironment* env);
  ~CGUISpriteBank();
  IRECT > & getPositions();
  SGUISprite* getSprites();
  u32 getTextureCount() const;
  ITexture* getTexture(u32 index) const;
  void addTexture(ITexture* texture);
  void setTexture(u32 index, ITexture* texture);
  //! Add the texture and use it for a single non-animated sprite.
  s32 addTextureAsSprite(ITexture* texture);
  //! clears sprites, rectangles and textures
  void clear();
  //! Draws a sprite in 2d with position and color
  void draw2DSprite(u32 index, const position2di& pos, const IRECT* clip = 0,
      const SColor& color = SColor(255, 255, 255, 255),
      u32 starttime = 0, u32 currenttime = 0, bool loop = true, bool center = false);
  //! Draws a sprite batch in 2d using an array of positions and a color
  void draw2DSpriteBatch(const u32* indices, const position2di > & pos,
      const IRECT* clip = 0,
      const SColor& color = SColor(255, 255, 255, 255),
      u32 starttime = 0, u32 currenttime = 0,
      bool loop = true, bool center = false);
protected:
  struct SDrawBatch {
    position2di > positions;
    recti > sourceRects;
    u32 textureNumber;
  };
  SGUISprite > Sprites;
  IRECT > Rectangles;
  ITexture* > Textures;
  IGUIEnvironment* Environment;
  IVideoDriver* Driver;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_SPRITE_BANK_H_INCLUDED__

