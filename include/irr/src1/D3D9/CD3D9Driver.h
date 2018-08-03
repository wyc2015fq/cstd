// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_VIDEO_DIRECTX_9_H_INCLUDED__
#define __C_VIDEO_DIRECTX_9_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
#ifdef _IRR_WINDOWS_
#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#endif
//#include "CNullDriver.h"
//#include "SIrrCreationParameters.h"
//#include "IMaterialRendererServices.h"
#if defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
//#include "irrMath.h"    // needed by borland for sqrtf define
#endif
//#include <d3d9.h>
#ifdef _IRR_COMPILE_WITH_CG_
//#include "Cg/cg.h"
//#include "Cg/cgD3D9.h"
#endif
struct SDepthSurface  {
  SDepthSurface() : Surface(0) {
#ifdef _DEBUG
    setDebugName("SDepthSurface");
#endif
  }
  ~SDepthSurface() {
    if (Surface) {
      Surface->Release();
    }
  }
  IDirect3DSurface9* Surface;
  dimension2du Size;
};
class CD3D9Driver : public CNullDriver, IMaterialRendererServices
{
public:
  friend class CD3D9Texture;
  //! constructor
  CD3D9Driver(const SIrrlichtCreationParameters& params, IFileSystem* io);
  //! destructor
  ~CD3D9Driver();
  //! applications must call this method before performing any rendering. returns false if failed.
  bool beginScene(bool backBuffer = true, bool zBuffer = true,
      SColor color = SColor(255, 0, 0, 0),
      const SExposedVideoData& videoData = SExposedVideoData(),
      IRECT* sourceRect = 0);
  //! applications must call this method after performing any rendering. returns false if failed.
  bool endScene();
  //! queries the features of the driver, returns true if feature is available
  bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const;
  //! sets transformation
  void setTransform(E_TRANSFORMATION_STATE state, const matrix4& mat);
  //! sets a material
  void setMaterial(const SMaterial& material);
  //! sets a render target
  bool setRenderTarget(ITexture* texture,
      bool clearBackBuffer = true, bool clearZBuffer = true,
      SColor color = SColor(0, 0, 0, 0));
  //! Sets multiple render targets
  bool setRenderTarget(const IRenderTarget texture,
      bool clearBackBuffer = true, bool clearZBuffer = true,
      SColor color = SColor(0, 0, 0, 0));
  //! sets a viewport
  void setViewPort(const IRECT& area);
  //! gets the area of the current viewport
  const IRECT& getViewPort() const;
  struct SHWBufferLink_d3d9 : public CNullDriver::SHWBufferLink {
    SHWBufferLink_d3d9(const IMeshBuffer* _MeshBuffer):
      SHWBufferLink(_MeshBuffer),
      vertexBuffer(0), indexBuffer(0),
      vertexBufferSize(0), indexBufferSize(0) {}
    IDirect3DVertexBuffer9* vertexBuffer;
    IDirect3DIndexBuffer9* indexBuffer;
    u32 vertexBufferSize;
    u32 indexBufferSize;
  };
  bool updateVertexHardwareBuffer(SHWBufferLink_d3d9* HWBuffer);
  bool updateIndexHardwareBuffer(SHWBufferLink_d3d9* HWBuffer);
  //! updates hardware buffer if needed
  bool updateHardwareBuffer(SHWBufferLink* HWBuffer);
  //! Create hardware buffer from mesh
  SHWBufferLink* createHardwareBuffer(const IMeshBuffer* mb);
  //! Delete hardware buffer (only some drivers can)
  void deleteHardwareBuffer(SHWBufferLink* HWBuffer);
  //! Draw hardware buffer
  void drawHardwareBuffer(SHWBufferLink* HWBuffer);
  //! Create occlusion query.
  /** Use node for identification and mesh for occlusion test. */
  void addOcclusionQuery(ISceneNode* node,
      const IMesh* mesh = 0);
  //! Remove occlusion query.
  void removeOcclusionQuery(ISceneNode* node);
  //! Run occlusion query. Draws mesh stored in query.
  /** If the mesh shall not be rendered visible, use
  overrideMaterial to disable the color and depth buffer. */
  void runOcclusionQuery(ISceneNode* node, bool visible = false);
  //! Update occlusion query. Retrieves results from GPU.
  /** If the query shall not block, set the flag to false.
  Update might not occur in this case, though */
  void updateOcclusionQuery(ISceneNode* node, bool block = true);
  //! Return query result.
  /** Return value is the number of visible pixels/fragments.
  The value is a safe approximation, i.e. can be larger then the
  actual value of pixels. */
  u32 getOcclusionQueryResult(ISceneNode* node) const;
  //! draws a vertex primitive list
  void drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
      const void* indexList, u32 primitiveCount,
      E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType,
      E_INDEX_TYPE iType);
  //! draws a vertex primitive list in 2d
  void draw2DVertexPrimitiveList(const void* vertices, u32 vertexCount,
      const void* indexList, u32 primitiveCount,
      E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType,
      E_INDEX_TYPE iType);
  //! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
  void draw2DImage(const ITexture* texture, const position2di& destPos,
      const IRECT& sourceRect, const IRECT* clipRect = 0,
      SColor color = SColor(255, 255, 255, 255), bool useAlphaChannelOfTexture = false);
  //! Draws a part of the texture into the rectangle.
  void draw2DImage(const ITexture* texture, const IRECT& destRect,
      const IRECT& sourceRect, const IRECT* clipRect = 0,
      const SColor* const colors = 0, bool useAlphaChannelOfTexture = false);
  //! Draws a set of 2d images, using a color and the alpha channel of the texture.
  void draw2DImageBatch(const ITexture* texture,
      const position2di > & positions,
      const IRECT* sourceRects,
      const IRECT* clipRect = 0,
      SColor color = SColor(255, 255, 255, 255),
      bool useAlphaChannelOfTexture = false);
  //!Draws an 2d rectangle with a gradient.
  void draw2DRectangle(const IRECT& pos,
      SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
      const IRECT* clip);
  //! Draws a 2d line.
  void draw2DLine(const position2di& start,
      const position2di& end,
      SColor color = SColor(255, 255, 255, 255));
  //! Draws a pixel.
  void drawPixel(u32 x, u32 y, const SColor& color);
  //! Draws a 3d line.
  void draw3DLine(const float3 start,
      const float3 end, SColor color = SColor(255, 255, 255, 255));
  //! initialises the Direct3D API
  bool initDriver(HWND hwnd, bool pureSoftware);
  //! \return Returns the name of the video driver. Example: In case of the DIRECT3D8
  //! driver, it would return "Direct3D8.1".
  const char* getName() const;
  //! deletes all dynamic lights there are
  void deleteAllDynamicLights();
  //! adds a dynamic light, returning an index to the light
  //! \param light: the light data to use to create the light
  //! \return An index to the light, or -1 if an error occurs
  s32 addDynamicLight(const SLight& light);
  //! Turns a dynamic light on or off
  //! \param lightIndex: the index returned by addDynamicLight
  //! \param turnOn: true to turn the light on, false to turn it off
  void turnLightOn(s32 lightIndex, bool turnOn);
  //! returns the maximal amount of dynamic lights the device can handle
  u32 getMaximalDynamicLightAmount() const;
  //! Sets the dynamic ambient light color. The default color is
  //! (0,0,0,0) which means it is dark.
  //! \param color: New color of the ambient light.
  void setAmbientLight(const SColorf& color);
  //! Draws a shadow volume into the stencil buffer.
  void drawStencilShadowVolume(const float3 > & triangles, bool zfail = true, u32 debugDataVisible = 0);
  //! Fills the stencil shadow with color.
  void drawStencilShadow(bool clearStencilBuffer = false,
      SColor leftUpEdge = SColor(0, 0, 0, 0),
      SColor rightUpEdge = SColor(0, 0, 0, 0),
      SColor leftDownEdge = SColor(0, 0, 0, 0),
      SColor rightDownEdge = SColor(0, 0, 0, 0));
  //! Returns the maximum amount of primitives (mostly vertices) which
  //! the device is able to render with one drawIndexedTriangleList
  //! call.
  u32 getMaximalPrimitiveCount() const;
  //! Enables or disables a texture creation flag.
  void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled);
  //! Sets the fog mode.
  void setFog(SColor color, E_FOG_TYPE fogType, f32 start,
      f32 end, f32 density, bool pixelFog, bool rangeFog);
  //! Only used by the internal engine. Used to notify the driver that
  //! the window was resized.
  void OnResize(const dimension2du& size);
  //! Can be called by an IMaterialRenderer to make its work easier.
  void setBasicRenderStates(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates);
  //! Returns type of video driver
  E_DRIVER_TYPE getDriverType() const;
  //! Returns the transformation set by setTransform
  const matrix4& getTransform(E_TRANSFORMATION_STATE state) const;
  //! Sets a vertex shader constant.
  void setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount = 1);
  //! Sets a pixel shader constant.
  void setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount = 1);
  //! Sets a constant for the vertex shader based on a name.
  bool setVertexShaderConstant(const c8* name, const f32* floats, int count);
  //! Bool interface for the above.
  bool setVertexShaderConstant(const c8* name, const bool* bools, int count);
  //! Int interface for the above.
  bool setVertexShaderConstant(const c8* name, const s32* ints, int count);
  //! Sets a constant for the pixel shader based on a name.
  bool setPixelShaderConstant(const c8* name, const f32* floats, int count);
  //! Bool interface for the above.
  bool setPixelShaderConstant(const c8* name, const bool* bools, int count);
  //! Int interface for the above.
  bool setPixelShaderConstant(const c8* name, const s32* ints, int count);
  //! Returns a pointer to the IVideoDriver interface. (Implementation for
  //! IMaterialRendererServices)
  IVideoDriver* getVideoDriver();
  //! Creates a render target texture.
  ITexture* addRenderTargetTexture(const dimension2du& size,
      const char* name, const ECOLOR_FORMAT format = ECF_UNKNOWN);
  //! Clears the ZBuffer.
  void clearZBuffer();
  //! Returns an image created from the last rendered frame.
  IImage* createScreenShot(ECOLOR_FORMAT format = ECF_UNKNOWN, E_RENDER_TARGET target = ERT_FRAME_BUFFER);
  //! Set/unset a clipping plane.
  bool setClipPlane(u32 index, const plane3df& plane, bool enable = false);
  //! Enable/disable a clipping plane.
  void enableClipPlane(u32 index, bool enable);
  //! Returns the graphics card vendor name.
  char* getVendorInfo() {
    return VendorName;
  }
  //! Enable the 2d override material
  void enableMaterial2D(bool enable = true);
  //! Check if the driver was recently reset.
  bool checkDriverReset() {
    return DriverWasReset;
  }
  // removes the depth struct from the DepthSurface array
  void removeDepthSurface(SDepthSurface* depth);
  //! Get the current color format of the color buffer
  /** \return Color format of the color buffer. */
  ECOLOR_FORMAT getColorFormat() const;
  //! Returns the maximum texture size supported.
  dimension2du getMaxTextureSize() const;
  //! Get the current color format of the color buffer
  /** \return Color format of the color buffer as D3D color value. */
  D3DFORMAT getD3DColorFormat() const;
  //! Get D3D color format from Irrlicht color format.
  D3DFORMAT getD3DFormatFromColorFormat(ECOLOR_FORMAT format) const;
  //! Get Irrlicht color format from D3D color format.
  ECOLOR_FORMAT getColorFormatFromD3DFormat(D3DFORMAT format) const;
  //! Get Cg context
#ifdef _IRR_COMPILE_WITH_CG_
  const CGcontext& getCgContext();
#endif
private:
  //! enumeration for rendering modes such as 2d and 3d for minizing the switching of renderStates.
  enum E_RENDER_MODE {
    ERM_NONE = 0, // no render state has been set yet.
    ERM_2D,     // 2d drawing rendermode
    ERM_3D,     // 3d rendering mode
    ERM_STENCIL_FILL, // stencil fill mode
    ERM_SHADOW_VOLUME_ZFAIL, // stencil volume draw mode
    ERM_SHADOW_VOLUME_ZPASS // stencil volume draw mode
  };
  //! sets right vertex shader
  void setVertexShader(E_VERTEX_TYPE newType);
  //! sets the needed renderstates
  bool setRenderStates3DMode();
  //! sets the needed renderstates
  void setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel);
  //! sets the needed renderstates
  void setRenderStatesStencilFillMode(bool alpha);
  //! sets the needed renderstates
  void setRenderStatesStencilShadowMode(bool zfail, u32 debugDataVisible);
  //! sets the current Texture
  bool setActiveTexture(u32 stage, const ITexture* texture);
  //! resets the device
  bool reset();
  //! returns a device dependent texture from a software surface (IImage)
  //! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
  ITexture* createDeviceDependentTexture(IImage* surface, const char* name, void* mipmapData = 0);
  //! returns the current size of the screen or rendertarget
  const dimension2du& getCurrentRenderTargetSize() const;
  //! Check if a proper depth buffer for the RTT is available, otherwise create it.
  void checkDepthBuffer(ITexture* tex);
  //! Adds a new material renderer to the VideoDriver, using pixel and/or
  //! vertex shaders to render geometry.
  s32 addShaderMaterial(const c8* vertexShaderProgram, const c8* pixelShaderProgram,
      IShaderConstantSetCallBack* callback,
      E_MATERIAL_TYPE baseMaterial, s32 userData);
  //! Adds a new material renderer to the VideoDriver, based on a high level shading
  //! language.
  s32 addHighLevelShaderMaterial(
      const c8* vertexShaderProgram,
      const c8* vertexShaderEntryPointName,
      E_VERTEX_SHADER_TYPE vsCompileTarget,
      const c8* pixelShaderProgram,
      const c8* pixelShaderEntryPointName,
      E_PIXEL_SHADER_TYPE psCompileTarget,
      const c8* geometryShaderProgram,
      const c8* geometryShaderEntryPointName = "main",
      E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
      E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
      E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
      u32 verticesOut = 0,
      IShaderConstantSetCallBack* callback = 0,
      E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
      s32 userData = 0,
      E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT);
  void createMaterialRenderers();
  void draw2D3DVertexPrimitiveList(const void* vertices,
      u32 vertexCount, const void* indexList, u32 primitiveCount,
      E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType,
      E_INDEX_TYPE iType, bool is3D);
  D3DTEXTUREADDRESS getTextureWrapMode(const u8 clamp);
  CV_INLINE D3DCOLORVALUE colorToD3D(const SColor& col) {
    const f32 f = 1.0f / 255.0f;
    D3DCOLORVALUE v;
    v.r = col.getRed() * f;
    v.g = col.getGreen() * f;
    v.b = col.getBlue() * f;
    v.a = col.getAlpha() * f;
    return v;
  }
  E_RENDER_MODE CurrentRenderMode;
  D3DPRESENT_PARAMETERS present;
  SMaterial Material, LastMaterial;
  bool ResetRenderStates; // bool to make all renderstates be reseted if set.
  bool Transformation3DChanged;
  const ITexture* CurrentTexture[MATERIAL_MAX_TEXTURES];
  bool LastTextureMipMapsAvailable[MATERIAL_MAX_TEXTURES];
  matrix4 Matrices[ETS_COUNT]; // matrizes of the 3d mode we need to restore when we switch back from the 2d mode.
  HINSTANCE D3DLibrary;
  IDirect3D9* pID3D;
  IDirect3DDevice9* pID3DDevice;
  IDirect3DSurface9* PrevRenderTarget;
  dimension2du CurrentRendertargetSize;
  HWND WindowId;
  IRECT* SceneSourceRect;
  D3DCAPS9 Caps;
  SIrrlichtCreationParameters Params;
  E_VERTEX_TYPE LastVertexType;
  SColorf AmbientLight;
  char* VendorName;
  u16 VendorID;
  SDepthSurface* > DepthBuffers;
  u32 MaxTextureUnits;
  u32 MaxUserClipPlanes;
  u32 MaxMRTs;
  u32 NumSetMRTs;
  f32 MaxLightDistance;
  s32 LastSetLight;
  enum E_CACHE_2D_ATTRIBUTES {
    EC2D_ALPHA = 0x1,
    EC2D_TEXTURE = 0x2,
    EC2D_ALPHA_CHANNEL = 0x4
  };
  ECOLOR_FORMAT ColorFormat;
  D3DFORMAT D3DColorFormat;
  bool DeviceLost;
  bool DriverWasReset;
  bool OcclusionQuerySupport;
  bool AlphaToCoverageSupport;
#ifdef _IRR_COMPILE_WITH_CG_
  CGcontext CgContext;
#endif
};
#endif // _IRR_COMPILE_WITH_DIRECT3D_9_
#endif // __C_VIDEO_DIRECTX_9_H_INCLUDED__
