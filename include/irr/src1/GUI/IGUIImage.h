// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_GUI_IMAGE_H_INCLUDED__
#define __I_GUI_IMAGE_H_INCLUDED__
//#include "IGUIElement.h"
class ITexture;
}
//! GUI element displaying an image.
class IGUIImage : public IGUIElement
{
public:
  //! constructor
  IGUIImage(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
    : IGUIElement(EGUIET_IMAGE, environment, parent, id, rectangle) {}
  //! Sets an image texture
  void setImage(ITexture* image) = 0;
  //! Gets the image texture
  ITexture* getImage() = 0;
  //! Sets the color of the image
  void setColor(SColor color) = 0;
  //! Sets if the image should scale to fit the element
  void setScaleImage(bool scale) = 0;
  //! Sets if the image should use its alpha channel to draw itself
  void setUseAlphaChannel(bool use) = 0;
  //! Gets the color of the image
  SColor getColor() = 0;
  //! Returns true if the image is scaled to fit, false if not
  bool isImageScaled() = 0;
  //! Returns true if the image is using the alpha channel, false if not
  bool isAlphaChannelUsed() = 0;
};
#endif

