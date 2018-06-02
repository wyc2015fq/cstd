// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_OPEN_GL_TEXTURE_H_INCLUDED__
#define __C_OPEN_GL_TEXTURE_H_INCLUDED__
//#include "ITexture.h"
//#include "IImage.h"
#ifdef _IRR_COMPILE_WITH_OPENGL_
#if defined(_IRR_OPENGL_USE_EXTPOINTER_)
#define GL_GLEXT_LEGACY 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif
#ifdef _IRR_WINDOWS_API_
// include windows headers for HWND
#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#include <GL/gl.h>
#ifdef _MSC_VER
#pragma comment(lib, "OpenGL32.lib")
#endif
#elif defined(_IRR_OSX_PLATFORM_)
//#include <OpenGL/gl.h>
#elif defined(_IRR_COMPILE_WITH_SDL_DEVICE_)
#define NO_SDL_GLEXT
//#include <SDL/SDL_video.h>
//#include <SDL/SDL_opengl.h>
#else
#if defined(_IRR_OSX_PLATFORM_)
//#include <OpenGL/gl.h>
#else
//#include <GL/gl.h>
#endif
#endif
class COpenGLDriver;
//! OpenGL texture.
class COpenGLTexture : public ITexture
{
public:
  //! constructor
  COpenGLTexture(IImage* surface, const char* name, void* mipmapData = 0, COpenGLDriver* driver = 0);
  //! destructor
  ~COpenGLTexture();
  //! lock function
  void* lock(E_TEXTURE_LOCK_MODE mode = ETLM_READ_WRITE, u32 mipmapLevel = 0);
  //! unlock function
  void unlock();
  //! Returns original size of the texture (image).
  const dimension2du& getOriginalSize() const;
  //! Returns size of the texture.
  const dimension2du& getSize() const;
  //! returns driver type of texture (=the driver, that created it)
  E_DRIVER_TYPE getDriverType() const;
  //! returns color format of texture
  ECOLOR_FORMAT getColorFormat() const;
  //! returns pitch of texture (in bytes)
  u32 getPitch() const;
  //! return open gl texture name
  GLuint getOpenGLTextureName() const;
  //! return whether this texture has mipmaps
  bool hasMipMaps() const;
  //! Regenerates the mip map levels of the texture.
  /** Useful after locking and modifying the texture
  \param mipmapData Pointer to raw mipmap data, including all necessary mip levels, in the same format as the main texture image. If not set the mipmaps are derived from the main image. */
  void regenerateMipMapLevels(void* mipmapData = 0);
  //! Is it a render target?
  bool isRenderTarget() const;
  //! Is it a FrameBufferObject?
  bool isFrameBufferObject() const;
  //! Bind RenderTargetTexture
  void bindRTT();
  //! Unbind RenderTargetTexture
  void unbindRTT();
  //! sets whether this texture is intended to be used as a render target.
  void setIsRenderTarget(bool isTarget);
protected:
  //! protected constructor with basic setup, no GL texture name created, for derived classes
  COpenGLTexture(const char* name, COpenGLDriver* driver);
  //! get the desired color format based on texture creation flags and the input format.
  ECOLOR_FORMAT getBestColorFormat(ECOLOR_FORMAT format);
  //! Get the OpenGL color format parameters based on the given Irrlicht color format
  GLint getOpenGLFormatAndParametersFromColorFormat(
      ECOLOR_FORMAT format, GLint& filtering, GLenum& colorformat, GLenum& type);
  //! get important numbers of the image and hw texture
  void getImageValues(IImage* image);
  //! copies the texture into an OpenGL texture.
  /** \param newTexture True if method is called for a newly created texture for the first time. Otherwise call with false to improve memory handling.
  \param mipmapData Pointer to raw mipmap data, including all necessary mip levels, in the same format as the main texture image.
  \param mipLevel If set to non-zero, only that specific miplevel is updated, using the MipImage member. */
  void uploadTexture(bool newTexture = false, void* mipmapData = 0, u32 mipLevel = 0);
  dimension2du ImageSize;
  dimension2du TextureSize;
  ECOLOR_FORMAT ColorFormat;
  COpenGLDriver* Driver;
  IImage* Image;
  IImage* MipImage;
  GLuint TextureName;
  GLint InternalFormat;
  GLenum PixelFormat;
  GLenum PixelType;
  u8 MipLevelStored;
  bool HasMipMaps;
  bool MipmapLegacyMode;
  bool IsRenderTarget;
  bool AutomaticMipmapUpdate;
  bool ReadOnlyLock;
  bool KeepImage;
};
//! OpenGL FBO texture.
class COpenGLFBOTexture : public COpenGLTexture
{
public:
  //! FrameBufferObject constructor
  COpenGLFBOTexture(const dimension2du& size, const char* name,
      COpenGLDriver* driver = 0, ECOLOR_FORMAT format = ECF_UNKNOWN);
  //! destructor
  ~COpenGLFBOTexture();
  //! Is it a FrameBufferObject?
  bool isFrameBufferObject() const;
  //! Bind RenderTargetTexture
  void bindRTT();
  //! Unbind RenderTargetTexture
  void unbindRTT();
  ITexture* DepthTexture;
protected:
  GLuint ColorFrameBuffer;
};
//! OpenGL FBO depth texture.
class COpenGLFBODepthTexture : public COpenGLTexture
{
public:
  //! FrameBufferObject depth constructor
  COpenGLFBODepthTexture(const dimension2du& size, const char* name, COpenGLDriver* driver = 0, bool useStencil = false);
  //! destructor
  ~COpenGLFBODepthTexture();
  //! Bind RenderTargetTexture
  void bindRTT();
  //! Unbind RenderTargetTexture
  void unbindRTT();
  bool attach(ITexture*);
protected:
  GLuint DepthRenderBuffer;
  GLuint StencilRenderBuffer;
  bool UseStencil;
};
#endif
#endif // _IRR_COMPILE_WITH_OPENGL_

