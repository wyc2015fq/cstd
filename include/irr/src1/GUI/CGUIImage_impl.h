// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CGUIImage.h"
#ifdef _IRR_COMPILE_WITH_GUI_
//#include "IGUISkin.h"
//#include "IGUIEnvironment.h"
//#include "IVideoDriver.h"
//! constructor
CGUIImage::CGUIImage(IGUIEnvironment* environment, IGUIElement* parent, s32 id, IRECT rectangle)
  : IGUIImage(environment, parent, id, rectangle), Texture(0), Color(255, 255, 255, 255),
    UseAlphaChannel(false), ScaleImage(false)
{
#ifdef _DEBUG
  setDebugName("CGUIImage");
#endif
}
//! destructor
CGUIImage::~CGUIImage()
{
  if (Texture) {
    Texture->drop();
  }
}
//! sets an image
void CGUIImage::setImage(ITexture* image)
{
  if (image == Texture) {
    return;
  }
  if (Texture) {
    Texture->drop();
  }
  Texture = image;
  if (Texture) {
    Texture->grab();
  }
}
//! Gets the image texture
ITexture* CGUIImage::getImage() const
{
  return Texture;
}
//! sets the color of the image
void CGUIImage::setColor(SColor color)
{
  Color = color;
}
//! Gets the color of the image
SColor CGUIImage::getColor() const
{
  return Color;
}
//! draws the element and its children
void CGUIImage::draw()
{
  if (!IsVisible) {
    return;
  }
  IGUISkin* skin = Environment->getSkin();
  IVideoDriver* driver = Environment->getVideoDriver();
  if (Texture) {
    if (ScaleImage) {
      const SColor Colors[] = {Color, Color, Color, Color};
      driver->draw2DImage(Texture, AbsoluteRect,
          IRECT(position2di(0, 0), dimension2di(Texture->getOriginalSize())),
          &AbsoluteClippingRect, Colors, UseAlphaChannel);
    }
    else {
      driver->draw2DImage(Texture, AbsoluteRect.UpperLeftCorner,
          IRECT(position2di(0, 0), dimension2di(Texture->getOriginalSize())),
          &AbsoluteClippingRect, Color, UseAlphaChannel);
    }
  }
  else {
    skin->draw2DRectangle(this, skin->getColor(EGDC_3D_DARK_SHADOW), AbsoluteRect, &AbsoluteClippingRect);
  }
  IGUIElement::draw();
}
//! sets if the image should use its alpha channel to draw itself
void CGUIImage::setUseAlphaChannel(bool use)
{
  UseAlphaChannel = use;
}
//! sets if the image should use its alpha channel to draw itself
void CGUIImage::setScaleImage(bool scale)
{
  ScaleImage = scale;
}
//! Returns true if the image is scaled to fit, false if not
bool CGUIImage::isImageScaled() const
{
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return ScaleImage;
}
//! Returns true if the image is using the alpha channel, false if not
bool CGUIImage::isAlphaChannelUsed() const
{
  _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
  return UseAlphaChannel;
}
//! Writes attributes of the element.
void CGUIImage::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
{
  IGUIImage::serializeAttributes(out, options);
  out->addTexture("Texture", Texture);
  out->addBool("UseAlphaChannel", UseAlphaChannel);
  out->addColor("Color", Color);
  out->addBool("ScaleImage", ScaleImage);
}
//! Reads attributes of the element
void CGUIImage::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
{
  IGUIImage::deserializeAttributes(in, options);
  setImage(in->getAttributeAsTexture("Texture"));
  setUseAlphaChannel(in->getAttributeAsBool("UseAlphaChannel"));
  setColor(in->getAttributeAsColor("Color"));
  setScaleImage(in->getAttributeAsBool("ScaleImage"));
}
#endif // _IRR_COMPILE_WITH_GUI_

