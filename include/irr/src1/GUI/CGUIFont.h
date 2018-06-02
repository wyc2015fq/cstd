// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_FONT_H_INCLUDED__
#define __C_GUI_FONT_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIFontBitmap.h"
//#include "irrString.h"
//#include "irrMap.h"
//#include "IXMLReader.h"
//#include "IReadFile.h"
//#include "irrArray.h"
class IVideoDriver;
class IImage;
}
class IGUIEnvironment;
class CGUIFont : public IGUIFontBitmap
{
public:
  //! constructor
  CGUIFont(IGUIEnvironment* env, const char* filename);
  //! destructor
  ~CGUIFont();
  //! loads a font from a texture file
  bool load(const char* filename);
  //! loads a font from a texture file
  bool load(IReadFile* file);
  //! loads a font from an XML file
  bool load(IXMLReader* xml);
  //! draws an text and clips it to the specified rectangle if wanted
  void draw(const char* text, const IRECT& position,
      SColor color, bool hcenter = false,
      bool vcenter = false, const IRECT* clip = 0);
  //! returns the dimension of a text
  dimension2du getDimension(const char* text) const;
  //! Calculates the index of the character in the text which is on a specific position.
  s32 getCharacterFromPos(const char* text, s32 pixel_x) const;
  //! Returns the type of this font
  EGUI_FONT_TYPE getType() const {
    return EGFT_BITMAP;
  }
  //! set an Pixel Offset on Drawing ( scale position on width )
  void setKerningWidth(s32 kerning);
  void setKerningHeight(s32 kerning);
  //! set an Pixel Offset on Drawing ( scale position on width )
  s32 getKerningWidth(const char* thisLetter = 0, const char* previousLetter = 0) const;
  s32 getKerningHeight() const;
  //! gets the sprite bank
  IGUISpriteBank* getSpriteBank() const;
  //! returns the sprite number from a given character
  u32 getSpriteNoFromChar(const char* c) const;
  void setInvisibleCharacters(const char* s);
private:
  struct SFontArea {
    SFontArea() : underhang(0), overhang(0), width(0), spriteno(0) {}
    s32       underhang;
    s32       overhang;
    s32       width;
    u32       spriteno;
  };
  //! load & prepare font from ITexture
  bool loadTexture(IImage* image, const char* name);
  void readPositions(IImage* texture, s32& lowerRightPositions);
  s32 getAreaFromCharacter(const int c) const;
  void setMaxHeight();
  SFontArea >    Areas;
  map<int, s32>   CharacterMap;
  IVideoDriver*    Driver;
  IGUISpriteBank*     SpriteBank;
  IGUIEnvironment*    Environment;
  u32       WrongCharacter;
  s32       MaxHeight;
  s32       GlobalKerningWidth, GlobalKerningHeight;
  char* Invisible;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_FONT_H_INCLUDED__

