// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_DIRECTX8_TEXTURE_H_INCLUDED__
#define __C_DIRECTX8_TEXTURE_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
//#include "ITexture.h"
//#include "IImage.h"
//#include <d3d8.h>
class CD3D8Driver;
/*!
  interface for a Video Driver dependent Texture.
*/
class CD3D8Texture : public ITexture
{
public:
  //! constructor
  CD3D8Texture(IImage* image, CD3D8Driver* driver,
      u32 flags, const char* name, void* mipmapData = 0);
  //! rendertarget constructor
  CD3D8Texture(CD3D8Driver* driver, const dimension2du& size, const char* name);
  //! destructor
  ~CD3D8Texture();
  //! lock function
  void* lock(E_TEXTURE_LOCK_MODE mode = ETLM_READ_WRITE, u32 mipmapLevel = 0);
  //! unlock function
  void unlock();
  //! Returns original size of the texture.
  const dimension2du& getOriginalSize() const;
  //! Returns (=size) of the texture.
  const dimension2du& getSize() const;
  //! returns driver type of texture (=the driver, who created the texture)
  E_DRIVER_TYPE getDriverType() const;
  //! returns color format of texture
  ECOLOR_FORMAT getColorFormat() const;
  //! returns pitch of texture (in bytes)
  u32 getPitch() const;
  //! returns the DIRECT3D8 Texture
  IDirect3DTexture8* getDX8Texture() const;
  //! returns if texture has mipmap levels
  bool hasMipMaps() const;
  //! Regenerates the mip map levels of the texture. Useful after locking and
  //! modifying the texture
  void regenerateMipMapLevels(void* mipmapData = 0);
  //! returns if it is a render target
  bool isRenderTarget() const;
  //! Returns pointer to the render target surface
  IDirect3DSurface8* getRenderTargetSurface();
private:
  friend class CD3D8Driver;
  void createRenderTarget();
  //! creates the hardware texture
  bool createTexture(u32 flags, IImage* Image);
  //! copies the image to the texture
  bool copyTexture(IImage* Image);
  //! convert color formats
  ECOLOR_FORMAT getColorFormatFromD3DFormat(D3DFORMAT format);
  bool createMipMaps(u32 level = 1);
  void copy16BitMipMap(char* src, char* tgt,
      s32 width, s32 height, s32 pitchsrc, s32 pitchtgt) const;
  void copy32BitMipMap(char* src, char* tgt,
      s32 width, s32 height, s32 pitchsrc, s32 pitchtgt) const;
  IDirect3DDevice8* Device;
  IDirect3DTexture8* Texture;
  IDirect3DSurface8* RTTSurface;
  CD3D8Driver* Driver;
  dimension2du TextureSize;
  dimension2du ImageSize;
  s32 Pitch;
  u32 MipLevelLocked;
  ECOLOR_FORMAT ColorFormat;
  bool HasMipMaps;
  bool IsRenderTarget;
};
#endif // _IRR_COMPILE_WITH_DIRECT3D_8_
#endif // __C_DIRECTX8_TEXTURE_H_INCLUDED__

