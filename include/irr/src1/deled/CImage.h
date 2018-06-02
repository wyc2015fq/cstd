// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_IMAGE_H_INCLUDED__
#define __C_IMAGE_H_INCLUDED__
//#include "IImage.h"



//! IImage implementation with a lot of special image operations for
//! 16 bit A1R5G5B5/32 Bit A8R8G8B8 images, which are used by the SoftwareDevice.
class CImage : public IImage
{
public:
  //! constructor from raw image data
  /** \param useForeignMemory: If true, the image will use the data pointer
  directly and own it from now on, which means it will also try to delete [] the
  data when the image will be destructed. If false, the memory will by copied. */
  CImage(ECOLOR_FORMAT format, const dimension2d<u32>& size,
      void* data, bool ownForeignMemory = true, bool deleteMemory = true);
  //! constructor for empty image
  CImage(ECOLOR_FORMAT format, const dimension2d<u32>& size);
  //! destructor
  ~CImage();
  //! Lock function.
  void* lock() {
    return Data;
  }
  //! Unlock function.
  void unlock() {}
  //! Returns width and height of image data.
  const dimension2d<u32>& getDimension() const;
  //! Returns bits per pixel.
  u32 getBitsPerPixel() const;
  //! Returns bytes per pixel
  u32 getBytesPerPixel() const;
  //! Returns image data size in bytes
  u32 getImageDataSizeInBytes() const;
  //! Returns image data size in pixels
  u32 getImageDataSizeInPixels() const;
  //! returns mask for red value of a pixel
  u32 getRedMask() const;
  //! returns mask for green value of a pixel
  u32 getGreenMask() const;
  //! returns mask for blue value of a pixel
  u32 getBlueMask() const;
  //! returns mask for alpha value of a pixel
  u32 getAlphaMask() const;
  //! returns a pixel
  SColor getPixel(u32 x, u32 y) const;
  //! sets a pixel
  void setPixel(u32 x, u32 y, const SColor& color, bool blend = false);
  //! returns the color format
  ECOLOR_FORMAT getColorFormat() const;
  //! returns pitch of image
  u32 getPitch() const {
    return Pitch;
  }
  //! copies this surface into another, scaling it to fit.
  void copyToScaling(void* target, u32 width, u32 height, ECOLOR_FORMAT format, u32 pitch = 0);
  //! copies this surface into another, scaling it to fit.
  void copyToScaling(IImage* target);
  //! copies this surface into another
  void copyTo(IImage* target, const position2di& pos = position2di(0, 0));
  //! copies this surface into another
  void copyTo(IImage* target, const position2di& pos, const rect<s32>& sourceRect, const rect<s32>* clipRect = 0);
  //! copies this surface into another, using the alpha mask, an cliprect and a color to add with
  void copyToWithAlpha(IImage* target, const position2di& pos,
      const rect<s32>& sourceRect, const SColor& color,
      const rect<s32>* clipRect = 0);
  //! copies this surface into another, scaling it to fit, appyling a box filter
  void copyToScalingBoxFilter(IImage* target, s32 bias = 0, bool blend = false);
  //! fills the surface with given color
  void fill(const SColor& color);
private:
  //! assumes format and size has been set and creates the rest
  void initData();
  CV_INLINE SColor getPixelBox(s32 x, s32 y, s32 fx, s32 fy, s32 bias) const;
  u8* Data;
  dimension2d<u32> Size;
  u32 BytesPerPixel;
  u32 Pitch;
  ECOLOR_FORMAT Format;
  bool DeleteMemory;
};


#endif

