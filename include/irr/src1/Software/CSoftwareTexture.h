// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SOFTWARE_TEXTURE_H_INCLUDED__
#define __C_SOFTWARE_TEXTURE_H_INCLUDED__
//#include "ITexture.h"
//#include "CImage.h"
/*!
  interface for a Video Driver dependent Texture.
*/
class CSoftwareTexture : public ITexture
{
public:
  //! constructor
  CSoftwareTexture(IImage* surface, const char* name,
      bool renderTarget = false, void* mipmapData = 0);
  //! destructor
  ~CSoftwareTexture();
  //! lock function
  void* lock(E_TEXTURE_LOCK_MODE mode = ETLM_READ_WRITE, u32 mipmapLevel = 0);
  //! unlock function
  void unlock();
  //! Returns original size of the texture.
  const dimension2du& getOriginalSize() const;
  //! Returns (=size) of the texture.
  const dimension2du& getSize() const;
  //! returns unoptimized surface
  CImage* getImage();
  //! returns texture surface
  CImage* getTexture();
  //! returns driver type of texture (=the driver, who created the texture)
  E_DRIVER_TYPE getDriverType() const;
  //! returns color format of texture
  ECOLOR_FORMAT getColorFormat() const;
  //! returns pitch of texture (in bytes)
  u32 getPitch() const;
  //! Regenerates the mip map levels of the texture. Useful after locking and
  //! modifying the texture
  void regenerateMipMapLevels(void* mipmapData = 0);
  //! is it a render target?
  bool isRenderTarget() const;
private:
  CImage* Image;
  CImage* Texture;
  dimension2du OrigSize;
  bool IsRenderTarget;
};
#endif

