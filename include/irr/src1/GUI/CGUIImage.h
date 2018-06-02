// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GUI_IMAGE_H_INCLUDED__
#define __C_GUI_IMAGE_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUIImage.h"
class CGUIImage : public IGUIImage
{
public:
  //! constructor
  CGUIImage(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle);
  //! destructor
  ~CGUIImage();
  //! sets an image
  void setImage(ITexture* image);
  //! Gets the image texture
  ITexture* getImage() const;
  //! sets the color of the image
  void setColor(SColor color);
  //! sets if the image should scale to fit the element
  void setScaleImage(bool scale);
  //! draws the element and its children
  void draw();
  //! sets if the image should use its alpha channel to draw itself
  void setUseAlphaChannel(bool use);
  //! Gets the color of the image
  SColor getColor() const;
  //! Returns true if the image is scaled to fit, false if not
  bool isImageScaled() const;
  //! Returns true if the image is using the alpha channel, false if not
  bool isAlphaChannelUsed() const;
  //! Writes attributes of the element.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the element
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  ITexture* Texture;
  SColor Color;
  bool UseAlphaChannel;
  bool ScaleImage;
};
#endif // _IRR_COMPILE_WITH_GUI_
#endif // __C_GUI_IMAGE_H_INCLUDED__

