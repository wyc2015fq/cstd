// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_IMAGE_H_INCLUDED__
#define __I_IMAGE_H_INCLUDED__






//! Interface for software image data.
/** Image loaders create these images from files. IVideoDrivers convert
these images into their (hardware) textures.
*/
class IImage
{
public:
  //! Lock function. Use this to get a pointer to the image data.
  /** After you don't need the pointer anymore, you must call unlock().
  \return Pointer to the image data. What type of data is pointed to
  depends on the color format of the image. For example if the color
  format is ECF_A8R8G8B8, it is of u32. Be sure to call unlock() after
  you don't need the pointer any more. */
  void* lock() = 0;
  //! Unlock function.
  /** Should be called after the pointer received by lock() is not
  needed anymore. */
  void unlock() = 0;
  //! Returns width and height of image data.
  const dimension2d<u32>& getDimension() = 0;
  //! Returns bits per pixel.
  u32 getBitsPerPixel() = 0;
  //! Returns bytes per pixel
  u32 getBytesPerPixel() = 0;
  //! Returns image data size in bytes
  u32 getImageDataSizeInBytes() = 0;
  //! Returns image data size in pixels
  u32 getImageDataSizeInPixels() = 0;
  //! Returns a pixel
  SColor getPixel(u32 x, u32 y) = 0;
  //! Sets a pixel
  void setPixel(u32 x, u32 y, const SColor& color, bool blend = false) = 0;
  //! Returns the color format
  ECOLOR_FORMAT getColorFormat() = 0;
  //! Returns mask for red value of a pixel
  u32 getRedMask() = 0;
  //! Returns mask for green value of a pixel
  u32 getGreenMask() = 0;
  //! Returns mask for blue value of a pixel
  u32 getBlueMask() = 0;
  //! Returns mask for alpha value of a pixel
  u32 getAlphaMask() = 0;
  //! Returns pitch of image
  u32 getPitch() = 0;
  //! Copies the image into the target, scaling the image to fit
  void copyToScaling(void* target, u32 width, u32 height, ECOLOR_FORMAT format = ECF_A8R8G8B8, u32 pitch = 0) = 0;
  //! Copies the image into the target, scaling the image to fit
  void copyToScaling(IImage* target) = 0;
  //! copies this surface into another
  void copyTo(IImage* target, const position2di& pos = position2di(0, 0)) = 0;
  //! copies this surface into another
  void copyTo(IImage* target, const position2di& pos, const rect<s32>& sourceRect, const rect<s32>* clipRect = 0) = 0;
  //! copies this surface into another, using the alpha mask and cliprect and a color to add with
  void copyToWithAlpha(IImage* target, const position2di& pos,
      const rect<s32>& sourceRect, const SColor& color,
      const rect<s32>* clipRect = 0) = 0;
  //! copies this surface into another, scaling it to fit, appyling a box filter
  void copyToScalingBoxFilter(IImage* target, s32 bias = 0, bool blend = false) = 0;
  //! fills the surface with given color
  void fill(const SColor& color) = 0;
  //! get the amount of Bits per Pixel of the given color format
  static u32 getBitsPerPixelFromFormat(const ECOLOR_FORMAT format) {
    switch (format) {
    case ECF_A1R5G5B5:
      return 16;

    case ECF_R5G6B5:
      return 16;

    case ECF_R8G8B8:
      return 24;

    case ECF_A8R8G8B8:
      return 32;

    case ECF_R16F:
      return 16;

    case ECF_G16R16F:
      return 32;

    case ECF_A16B16G16R16F:
      return 64;

    case ECF_R32F:
      return 32;

    case ECF_G32R32F:
      return 64;

    case ECF_A32B32G32R32F:
      return 128;

    default:
      return 0;
    }
  }
  //! test if the color format is only viable for RenderTarget textures
  /** Since we don't have support for e.g. floating point IImage formats
  one should test if the color format can be used for arbitrary usage, or
  if it is restricted to RTTs. */
  static bool isRenderTargetOnlyFormat(const ECOLOR_FORMAT format) {
    switch (format) {
    case ECF_A1R5G5B5:
    case ECF_R5G6B5:
    case ECF_R8G8B8:
    case ECF_A8R8G8B8:
      return false;

    default:
      return true;
    }
  }
};


#endif

