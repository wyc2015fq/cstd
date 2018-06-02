#include "MeshManipulator.h"

//! constructor
void CNullDriver_init(IVideoDriver* s, ISIZE screenSize)
{
  int i;
  s->ScreenSize = screenSize;
  s->MinVertexCountForVBO = (500);
  s->OverrideMaterial2DEnabled = (false);
  s->AllowZWriteOnTransparent = (false);

  s->setFog(s, 0, EFT_FOG_LINEAR, 50, 100, 0.01f, false, false);
  s->setTextureCreationFlag(s, ETCF_ALWAYS_32_BIT, true);
  s->setTextureCreationFlag(s, ETCF_CREATE_MIP_MAPS, true);
  s->ViewPort = iRECT2(0, 0, screenSize.w, screenSize.h);
  // create manipulator
  //MeshManipulator = new CMeshManipulator();
  // set ExposedData to 0
  memset(&s->ExposedData, 0, sizeof(s->ExposedData));
  for (i = 0; i < EVDF_COUNT; ++i) {
    s->FeatureEnabled[i] = true;
  }
  s->InitMaterial2D.AntiAliasing = EAAM_OFF;
  s->InitMaterial2D.Lighting = false;
  s->InitMaterial2D.ZWriteEnable = false;
  s->InitMaterial2D.ZBuffer = ECFN_NEVER;
  s->InitMaterial2D.UseMipMaps = false;
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    s->InitMaterial2D.TextureLayer[i].BilinearFilter = false;
    s->InitMaterial2D.TextureLayer[i].TextureWrapU = ETC_REPEAT;
    s->InitMaterial2D.TextureLayer[i].TextureWrapV = ETC_REPEAT;
  }
  s->OverrideMaterial2D = s->InitMaterial2D;
}
//! deletes all textures
void CNullDriver_deleteAllTextures(IVideoDriver* s) {
}
//! deletes all material renderers
void CNullDriver_deleteMaterialRenders() {
  // delete material renderers
}
//! Remove all hardware buffers
void CNullDriver_removeAllHardwareBuffers() {
  //while (HWBufferMap_size) {    deleteHardwareBuffer(HWBufferMap.getRoot()->getValue());  }
}
//! destructor
void CNullDriver_free(IVideoDriver* s) {
  CNullDriver_deleteAllTextures(s);
  // delete material renderers
  CNullDriver_deleteMaterialRenders(s);
  // delete hardware mesh buffers
  CNullDriver_removeAllHardwareBuffers(s);
}
//! applications must call this method before performing any rendering. returns false if failed.
bool CNullDriver_beginScene(IVideoDriver* s, bool backBuffer, bool zBuffer, SColor color,
                            const SExposedVideoData* videoData, IRECT * sourceRect) {
  clearFPUException();
  s->PrimitivesDrawn = 0;
  return true;
}
#define ceil32(x)   (int)ceil(x)
CFPSCounter* registerFrame(u32 now, u32 primitivesDrawn)
{
  static CFPSCounter FPSCounter[1] = {0};
  ++FPSCounter->FramesCounted;
  FPSCounter->PrimitiveTotal += primitivesDrawn;
  FPSCounter->PrimitivesCounted += primitivesDrawn;
  FPSCounter->Primitive = primitivesDrawn;
  {
    const u32 milliseconds = now - FPSCounter->StartTime;
    if (milliseconds >= 1500) {
      const f32 invMilli = reciprocal((f32) milliseconds);
      FPSCounter->FPS = ceil32((1000 * FPSCounter->FramesCounted) * invMilli);
      FPSCounter->PrimitiveAverage = ceil32((1000 * FPSCounter->PrimitivesCounted) * invMilli);
      FPSCounter->FramesCounted = 0;
      FPSCounter->PrimitivesCounted = 0;
      FPSCounter->StartTime = now;
    }
  }
  return FPSCounter;
}

//! Update all hardware buffers, remove unused ones
void CNullDriver_updateAllHardwareBuffers(IVideoDriver* s) {
#if 0
  map<const IMeshBuffer*, SHWBufferLink*>::ParentFirstIterator Iterator = HWBufferMap.getParentFirstIterator();
  for (; !Iterator.atEnd(); Iterator++) {
    SHWBufferLink* Link = Iterator.getNode()->getValue();
    Link->LastUsed++;
    if (Link->LastUsed > 20000) {
      deleteHardwareBuffer(Link);
      // todo: needs better fix
      Iterator = HWBufferMap.getParentFirstIterator();
    }
  }
#endif
}
//! Update occlusion query. Retrieves results from GPU.
void CNullDriver_updateOcclusionQuery(IVideoDriver* s, ISceneNode * node, bool block DEFAULT(true)) {
}
//! Update all occlusion queries. Retrieves results from GPU.
void CNullDriver_updateAllOcclusionQueries(IVideoDriver* s, bool block) {
#if 0
  int i;
  for (i = 0; i < OcclusionQueries_size; ++i) {
    if (OcclusionQueries[i].Run == (u32)(~0)) {
      continue;
    }
    updateOcclusionQuery(OcclusionQueries[i].Node, block);
    ++OcclusionQueries[i].Run;
    if (OcclusionQueries[i].Run > 1000) {
      removeOcclusionQuery(OcclusionQueries[i].Node);
    }
  }
#endif
}
//! applications must call this method after performing any rendering. returns false if failed.
bool CNullDriver_endScene(IVideoDriver* s) {
  registerFrame(getRealTime(), s->PrimitivesDrawn);
  CNullDriver_updateAllHardwareBuffers(s);
  CNullDriver_updateAllOcclusionQueries(s, true);
  return true;
}
//! Disable a feature of the driver.
void CNullDriver_disableFeature(IVideoDriver* s, E_VIDEO_DRIVER_FEATURE feature, bool flag) {
  s->FeatureEnabled[feature] = !flag;
}
//! queries the features of the driver, returns true if feature is available
bool CNullDriver_queryFeature(IVideoDriver* s, E_VIDEO_DRIVER_FEATURE feature) {
  return false;
}
//! sets transformation
void CNullDriver_setTransform(IVideoDriver* s, E_TRANSFORMATION_STATE state, const float* mat) {
}
//! sets a material
void CNullDriver_setMaterial(IVideoDriver* s, const SMaterial* material) {
}
//! Removes a texture from the texture cache and deletes it, freeing lot of
//! memory.
void CNullDriver_removeTexture(IVideoDriver* s, ITexture * texture) {
}
//! Removes all texture from the texture cache and deletes them, freeing lot of
//! memory.
void CNullDriver_removeAllTextures(IVideoDriver* s) {
}
//! Returns a texture by index
ITexture* CNullDriver_getTextureByIndex(IVideoDriver* s, u32 i) {
  return 0;
}
//! Returns amount of textures currently loaded
u32 CNullDriver_getTextureCount(IVideoDriver* s) {
  return 0;
}
//! Renames a texture
void CNullDriver_renameTexture(IVideoDriver* s, ITexture * texture, const char * newName) {
}
//! loads a Texture
ITexture* CNullDriver_getTexture(IVideoDriver* s, const char* filename) {
  // Identify textures by their absolute filenames if possible.
  char256 absolutePath = {0};
  ITexture* texture = NULL;
  llogprintf(ELL_ERROR, "Could not load texture %s", filename);
  llogprintf(ELL_WARNING, "Could not open file of texture %s", filename);
  return texture;
}
//! draws a vertex primitive list
void CNullDriver_drawVertexPrimitiveList(IVideoDriver* s, const void * vertices, u32 vertexCount, const void * indexList, u32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType) {
  if ((iType == EIT_16BIT) && (vertexCount > 65536)) {
    llogprintf(ELL_NONE, "Too many vertices for 16bit index type, render artifacts may occur.");
  }
  s->PrimitivesDrawn += primitiveCount;
}
//! draws a vertex primitive list in 2d
void CNullDriver_draw2DVertexPrimitiveList(IVideoDriver* s, const void * vertices, u32 vertexCount, const void * indexList, u32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType) {
  if ((iType == EIT_16BIT) && (vertexCount > 65536)) {
    llogprintf(ELL_NONE, "Too many vertices for 16bit index type, render artifacts may occur.");
  }
  s->PrimitivesDrawn += primitiveCount;
}
//! Draws a 3d line.
void CNullDriver_draw3DLine(IVideoDriver* s, const float3 start,
                            const float3 end, SColor color) {
}
//! Draws a 3d triangle.
void CNullDriver_draw3DTriangle(IVideoDriver* s, const triangle3df* triangle, SColor color) {
#if 0
  S3DVertex vertices[3];
  const u16 indexList[] = {0, 1, 2};
  vertices[0].Pos = triangle.pointA;
  vertices[0].Color = color;
  vertices[0].Normal = triangle.getNormal().normalize();
  vertices[0].TCoords.set(0.f, 0.f);
  vertices[1].Pos = triangle.pointB;
  vertices[1].Color = color;
  vertices[1].Normal = vertices[0].Normal;
  vertices[1].TCoords.set(0.5f, 1.f);
  vertices[2].Pos = triangle.pointC;
  vertices[2].Color = color;
  vertices[2].Normal = vertices[0].Normal;
  vertices[2].TCoords.set(1.f, 0.f);
  drawVertexPrimitiveList(vertices, 3, indexList, 1, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
#endif
}

// Edges are stored in this way:
// Hey, am I an ascii artist, or what? :) niko.
//      /3--------/7
//     / |       / |
//    /  |      /  |
//   1---------5   |
//   |  /2- - -|- -6
//   | /       |  /
//   |/        | /
//   0---------4/
void F3AABBOX_getEdges(const F3AABBOX* s, float3* edges) {
  float3 middle, diag;
  VADD3(middle, s->MinEdge, s->MaxEdge);
  VSCALE3(middle, middle, 0.5f);
  VSUB3(diag, middle, s->MaxEdge);

  VSET3(edges[0], middle[0] + diag[0], middle[1] + diag[1], middle[2] + diag[2]);
  VSET3(edges[1], middle[0] + diag[0], middle[1] - diag[1], middle[2] + diag[2]);
  VSET3(edges[2], middle[0] + diag[0], middle[1] + diag[1], middle[2] - diag[2]);
  VSET3(edges[3], middle[0] + diag[0], middle[1] - diag[1], middle[2] - diag[2]);
  VSET3(edges[4], middle[0] - diag[0], middle[1] + diag[1], middle[2] + diag[2]);
  VSET3(edges[5], middle[0] - diag[0], middle[1] - diag[1], middle[2] + diag[2]);
  VSET3(edges[6], middle[0] - diag[0], middle[1] + diag[1], middle[2] - diag[2]);
  VSET3(edges[7], middle[0] - diag[0], middle[1] - diag[1], middle[2] - diag[2]);
}
//! Draws a 3d axis aligned box.
void CNullDriver_draw3DBox(IVideoDriver* s, const F3AABBOX* box, SColor color) {
  float3 edges[8];
  F3AABBOX_getEdges(box, edges);
  // TODO: optimize into one big drawIndexPrimitive call.
  s->draw3DLine(s, edges+5, edges+1, color);
  s->draw3DLine(s, edges+1, edges+3, color);
  s->draw3DLine(s, edges+3, edges+7, color);
  s->draw3DLine(s, edges+7, edges+5, color);
  s->draw3DLine(s, edges+0, edges+2, color);
  s->draw3DLine(s, edges+2, edges+6, color);
  s->draw3DLine(s, edges+6, edges+4, color);
  s->draw3DLine(s, edges+4, edges+0, color);
  s->draw3DLine(s, edges+1, edges+0, color);
  s->draw3DLine(s, edges+3, edges+2, color);
  s->draw3DLine(s, edges+7, edges+6, color);
  s->draw3DLine(s, edges+5, edges+4, color);
}
//! draws an 2d image
void CNullDriver_draw2DImage(IVideoDriver* s, const ITexture * texture, IPOINT destPos) {
  if (!texture) {
    return;
  }
  //s->draw2DImage(s, texture, destPos, iRECT2(0, 0, texture->getOriginalSize()));
}
//! draws a set of 2d images, using a color and the alpha channel of the
//! texture if desired. The images are drawn beginning at pos and concatenated
//! in one line. All drawings are clipped against clipRect (if != 0).
//! The subtextures are defined by the array of sourceRects and are chosen
//! by the indices given.
void CNullDriver_draw2DImageBatch(IVideoDriver* s, const ITexture * texture,
                                  IPOINT pos,
                                  const IRECT* sourceRects,
                                  const s32* indices, int indices_size,
                                  s32 kerningWidth,
                                  const IRECT* clipRect, SColor color,
                                  bool useAlphaChannelOfTexture) {
  IPOINT target = (pos);
  int i;
  for (i = 0; i < indices_size; ++i) {
    const IRECT* prc = sourceRects+indices[i];
    s->draw2DImage(s, texture, target, prc,
      clipRect, color, useAlphaChannelOfTexture);
    target.x += RCW(prc);
    target.x += kerningWidth;
  }
}

//! draws a set of 2d images, using a color and the alpha channel of the
//! texture if desired.
void CNullDriver_draw2DImageBatch2(IVideoDriver* s, const ITexture * texture, int drawCount,
                                  const IPOINT* positions, 
                                  const IRECT * sourceRects,
                                  const IRECT * clipRect,
                                  SColor color,
                                  bool useAlphaChannelOfTexture) {
  int i;
  for (i = 0; i < drawCount; ++i) {
    s->draw2DImage(s, texture, positions[i], sourceRects+i,
      clipRect, color, useAlphaChannelOfTexture);
  }
}
#if 0
//! Draws a part of the texture into the rectangle.
void CNullDriver_draw2DImage1(IVideoDriver* s, const ITexture * texture, const IRECT* destRect,
                             const IRECT* sourceRect, const IRECT* clipRect,
                             const SColor* const colors, bool useAlphaChannelOfTexture) {
  if (destRect.isValid())
    draw2DImage(texture, IPOINT(destRect.UpperLeftCorner),
    sourceRect, clipRect, colors ? colors[0] : SColor(0xffffffff),
    useAlphaChannelOfTexture);
}
//! Draws a 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
void CNullDriver_draw2DImage2(IVideoDriver* s, const ITexture * texture, const IPOINT & destPos,
                             const IRECT & sourceRect,
                             const IRECT * clipRect, SColor color,
                             bool useAlphaChannelOfTexture) {
}
//! Draws the outline of a 2d rectangle
void CNullDriver_draw2DRectangleOutline(IVideoDriver* s, const recti & pos, SColor color) {
  draw2DLine(pos.UpperLeftCorner, IPOINT(pos.LowerRightCorner.X, pos.UpperLeftCorner.Y), color);
  draw2DLine(IPOINT(pos.LowerRightCorner.X, pos.UpperLeftCorner.Y), pos.LowerRightCorner, color);
  draw2DLine(pos.LowerRightCorner, IPOINT(pos.UpperLeftCorner.X, pos.LowerRightCorner.Y), color);
  draw2DLine(IPOINT(pos.UpperLeftCorner.X, pos.LowerRightCorner.Y), pos.UpperLeftCorner, color);
}
//! Draw a 2d rectangle
void CNullDriver_draw2DRectangle(IVideoDriver* s, SColor color, const IRECT & pos, const IRECT * clip) {
  draw2DRectangle(pos, color, color, color, color, clip);
}
#endif
//! Draws a non filled concyclic regular 2d polyon.
void CNullDriver_draw2DPolygon(IVideoDriver* s, IPOINT center, f32 radius, SColor color, s32 count) {
  IPOINT first, a, b;
  s32 j;
  if (count < 2) {
    return;
  }
  for (j = 0; j < count; ++j) {
    f32 p;
    b = a;
    p = j / (f32)count * (PI * 2);
    a.x = center.x + (s32)(sin(p) * radius);
    a.y = center.y + (s32)(cos(p) * radius);
    if (j == 0) {
      first = a;
    }
    else {
      s->draw2DLine(s, a, b, color);
    }
  }
  s->draw2DLine(s, a, first, color);
}
//! Creates a boolean alpha channel of the texture based of an color key.
void CNullDriver_makeColorKeyTexture(IVideoDriver* s, ITexture * texture, SColor color, bool zeroTexels) {
  int pixel;
  if (texture) {
    return;
  }
  switch (texture->ColorFormat) {
  default:
    llogprintf(ELL_ERROR, "Error: Unsupported texture color format for making color key channel.");
    break;
  case ECF_A1R5G5B5:
    {
      u16* p = (u16*)texture->lock(texture, ETLM_READ_WRITE, 0);
      const ISIZE dim = texture->TextureSize;
      const u32 pitch = texture->Pitch / 2;
      // color with alpha disabled (i.e. fully transparent)
      const u16 refZeroAlpha = (0x7fff & A8R8G8B8toA1R5G5B5(color));
      const u32 pixels = pitch * dim.h;
      for (pixel = 0; pixel < pixels; ++ pixel) {
        // If the color matches the reference color, ignoring alphas,
        // set the alpha to zero.
        if (((*p) & 0x7fff) == refZeroAlpha) {
          if (zeroTexels) {
            (*p) = 0;
          }
          else {
            (*p) = refZeroAlpha;
          }
        }
        ++p;
      }
      texture->unlock(texture);
    }
    break;
  case ECF_A8R8G8B8:
    {
      u32* p = (u32*)texture->lock(texture, ETLM_READ_WRITE, 0);
      ISIZE dim = texture->TextureSize;
      u32 pitch = texture->Pitch / 4;
      // color with alpha disabled (fully transparent)
      const u32 refZeroAlpha = 0x00ffffff & color;
      const u32 pixels = pitch * dim.h;
      for (pixel = 0; pixel < pixels; ++ pixel) {
        // If the color matches the reference color, ignoring alphas,
        // set the alpha to zero.
        if (((*p) & 0x00ffffff) == refZeroAlpha) {
          if (zeroTexels) {
            (*p) = 0;
          }
          else {
            (*p) = refZeroAlpha;
          }
        }
        ++p;
      }
      texture->unlock(texture);
    }
    break;
  }
  texture->regenerateMipMapLevels(texture, 0);
}
//! Creates an boolean alpha channel of the texture based of an color key position.
void CNullDriver_makeColorKeyTexture2(IVideoDriver* s, ITexture * texture, IPOINT colorKeyPixelPos, bool zeroTexels) {
#if 0
  SColor colorKey;
  if (!texture) {
    return;
  }
  if (texture->getColorFormat() != ECF_A1R5G5B5 && texture->getColorFormat() != ECF_A8R8G8B8) {
    llogprintf(ELL_NONE, "Error: Unsupported texture color format for making color key channel.", ELL_ERROR);
    return;
  }
  if (texture->getColorFormat() == ECF_A1R5G5B5) {
    u16* p = (u16*)texture->lock(ETLM_READ_ONLY);
    if (!p) {
      llogprintf(ELL_NONE, "Could not lock texture for making color key channel.", ELL_ERROR);
      return;
    }
    u32 pitch = texture->getPitch() / 2;
    const u16 key16Bit = 0x7fff & p[colorKeyPixelPos.Y * pitch + colorKeyPixelPos.X];
    colorKey = A1R5G5B5toA8R8G8B8(key16Bit);
  }
  else {
    u32* p = (u32*)texture->lock(ETLM_READ_ONLY);
    if (!p) {
      llogprintf(ELL_NONE, "Could not lock texture for making color key channel.", ELL_ERROR);
      return;
    }
    u32 pitch = texture->getPitch() / 4;
    colorKey = 0x00ffffff & p[colorKeyPixelPos.Y * pitch + colorKeyPixelPos.X];
  }
  texture->unlock();
  makeColorKeyTexture(texture, colorKey, zeroTexels);
#endif
}
//! normal map lookup 32 bit version
CV_INLINE f32 nml32(int x, int y, int pitch, int height, s32* p) {
  if (x < 0) {
    x = pitch - 1;
  }
  if (x >= pitch) {
    x = 0;
  }
  if (y < 0) {
    y = height - 1;
  }
  if (y >= height) {
    y = 0;
  }
  return (f32)(((p[(y * pitch) + x]) >> 16) & 0xff);
}
//! normal map lookup 16 bit version
CV_INLINE f32 nml16(int x, int y, int pitch, int height, s16* p) {
  if (x < 0) {
    x = pitch - 1;
  }
  if (x >= pitch) {
    x = 0;
  }
  if (y < 0) {
    y = height - 1;
  }
  if (y >= height) {
    y = 0;
  }
  return (f32) getAverage(p[(y * pitch) + x]);
  }
//! Creates a normal map from a height map texture.
//! \param amplitude: Constant value by which the height information is multiplied.
void CNullDriver_makeNormalMapTexture(IVideoDriver* s, ITexture * texture, f32 amplitude) {
  if (!texture) {
    return;
  }
  if (texture->ColorFormat != ECF_A1R5G5B5 &&
    texture->ColorFormat != ECF_A8R8G8B8) {
    llogprintf(ELL_NONE, "Error: Unsupported texture color format for making normal map.", ELL_ERROR);
    return;
  }
  {
    ISIZE dim = texture->TextureSize;
    f32 vh = dim.h / (f32)dim.w;
    f32 hh = dim.w / (f32)dim.h;
    s32 x, y, height;
    amplitude = amplitude / 255.0f;
    if (texture->ColorFormat == ECF_A8R8G8B8) {
      // ECF_A8R8G8B8 version
      s32* p = (s32*)texture->lock(texture, ETLM_READ_WRITE, 0);
      // copy texture
      u32 pitch = texture->Pitch / 4;
      s32* in = MALLOC(s32, dim.h * pitch);
      if (!p) {
        llogprintf(ELL_NONE, "Could not lock texture for making normal map.", ELL_ERROR);
        return;
      }
      memcpy(in, p, dim.h * pitch * 4);
      for (x = 0; x < (s32)(pitch); ++x) {
        for (y = 0; y < (s32)(dim.h); ++y) {
          // TODO: this could be optimized really a lot
          float3 h1={(x - 1)*hh, nml32(x - 1, y, pitch, dim.h, in)*amplitude, y * vh};
          float3 h2={(x + 1)*hh, nml32(x + 1, y, pitch, dim.h, in)*amplitude, y * vh};
          //float3 v1(x*hh, nml32(x, y-1, pitch, dim.h, in)*amplitude, (y-1)*vh);
          //float3 v2(x*hh, nml32(x, y+1, pitch, dim.h, in)*amplitude, (y+1)*vh);
          float3 v1={x * hh, nml32(x, y + 1, pitch, dim.h, in)*amplitude, (y - 1)*vh};
          float3 v2={x * hh, nml32(x, y - 1, pitch, dim.h, in)*amplitude, (y + 1)*vh};
          float3 v, h, n;
          VSUB3(v, v1, v2);
          VSUB3(h, h1, h2);
          VCROSS3(n, v, h);
          normalize3fv(n, n);
          n[0] = (n[0] * 127.5f + 127.5f);
          n[1] = (n[1] * 127.5f + 127.5f);
          n[2] = (n[2] * 127.5f + 127.5f);
          height = (s32)nml32(x, y, pitch, dim.h, in);
          // store height in alpha
          p[y * pitch + x] = _ARGB(height, (s32)n[0], (s32)n[2], (s32)n[1]); // X Z Y
        }
      }
      FREE(in);
      texture->unlock(texture);
    }
    else {
      // ECF_A1R5G5B5 version
      s16* p = (s16*)texture->lock(texture, ETLM_READ_WRITE, 0);
      u32 pitch = texture->Pitch / 2;
      // copy texture
      s16* in = MALLOC(s16, dim.h * pitch);
      if (!p) {
        llogprintf(ELL_NONE, "Could not lock texture for making normal map.", ELL_ERROR);
        return;
      }
      memcpy(in, p, dim.h * pitch * 2);
      for (x = 0; x < (s32)(pitch); ++x)
        for (y = 0; y < (s32)(dim.h); ++y) {
          // TODO: this could be optimized really a lot
          float3 h1 = {(x - 1)*hh, nml16(x - 1, y, pitch, dim.h, in)*amplitude, y * vh};
          float3 h2 = {(x + 1)*hh, nml16(x + 1, y, pitch, dim.h, in)*amplitude, y * vh};
          float3 v1 = {x * hh, nml16(x, y - 1, pitch, dim.h, in)*amplitude, (y - 1)*vh};
          float3 v2 = {x * hh, nml16(x, y + 1, pitch, dim.h, in)*amplitude, (y + 1)*vh};
          float3 v, h, n;
          VSUB3(v, v1, v2);
          VSUB3(h, h1, h2);
          VCROSS3(n, v, h);
          normalize3fv(n, n);
          n[0] = (n[0] * 127.5f + 127.5f);
          n[1] = (n[1] * 127.5f + 127.5f);
          n[2] = (n[2] * 127.5f + 127.5f);
          p[y * pitch + x] = RGB16((u32)n[0], (u32)n[2], (u32)n[1]);
        }
        FREE(in);
        texture->unlock(texture);
    }
    texture->regenerateMipMapLevels(texture, 0);
  }
}
//! Returns the maximum amount of primitives (mostly vertices) which
//! the device is able to render with one drawIndexedTriangleList
//! call.
u32 CNullDriver_getMaximalPrimitiveCount(IVideoDriver* s) {
  return 0xFFFFFFFF;
}
//! checks triangle count and print warning if wrong
bool CNullDriver_checkPrimitiveCount(IVideoDriver* s, u32 prmCount) {
  const u32 m = s->getMaximalPrimitiveCount(s);
  if (prmCount > m) {
    llogprintf(ELL_ERROR, "Could not draw triangles, too many primitives(%u), maxium is %u.", prmCount, m);
    return false;
  }
  return true;
}
//! Enables or disables a texture creation flag.
void CNullDriver_setTextureCreationFlag(IVideoDriver* s, E_TEXTURE_CREATION_FLAG flag, bool enabled) {
  if (enabled && ((flag == ETCF_ALWAYS_16_BIT) || (flag == ETCF_ALWAYS_32_BIT)
    || (flag == ETCF_OPTIMIZED_FOR_QUALITY) || (flag == ETCF_OPTIMIZED_FOR_SPEED))) {
    // disable other formats
    s->setTextureCreationFlag(s, ETCF_ALWAYS_16_BIT, false);
    s->setTextureCreationFlag(s, ETCF_ALWAYS_32_BIT, false);
    s->setTextureCreationFlag(s, ETCF_OPTIMIZED_FOR_QUALITY, false);
    s->setTextureCreationFlag(s, ETCF_OPTIMIZED_FOR_SPEED, false);
  }
  // set flag
  s->TextureCreationFlags = (s->TextureCreationFlags & (~flag)) | ((((u32)!enabled) - 1) & flag);
}
//! Returns if a texture creation flag is enabled or disabled.
bool CNullDriver_getTextureCreationFlag(IVideoDriver* s, E_TEXTURE_CREATION_FLAG flag) {
  return (s->TextureCreationFlags & flag) != 0;
}
//! Sets the fog mode.
void CNullDriver_setFog(IVideoDriver* s, SColor color, E_FOG_TYPE fogType, f32 start, f32 end,
                        f32 density, bool pixelFog, bool rangeFog) {
  s->FogColor = color;
  s->FogType = fogType;
  s->FogStart = start;
  s->FogEnd = end;
  s->FogDensity = density;
  s->PixelFog = pixelFog;
  s->RangeFog = rangeFog;
}
//! Draws a mesh buffer
void CNullDriver_drawMeshBuffer(IVideoDriver* s, const IMeshBuffer * mb) {
#if 0
  if (!mb) {
    //IVertexBuffer and IIndexBuffer later
    SHWBufferLink* HWBuffer = s->getBufferLink(s, mb);
    if (HWBuffer) {
      drawHardwareBuffer(HWBuffer);
    }
    else {
      drawVertexPrimitiveList(mb->getVertices(), mb->getVertexCount(), mb->getIndices(), mb->getIndexCount() / 3, mb->getVertexType(), EPT_TRIANGLES, mb->getIndexType());
    }
  }
#endif
}
#if 0
//! Draws the normals of a mesh buffer
void CNullDriver_drawMeshBufferNormals(IVideoDriver* s, const IMeshBuffer * mb, f32 length, SColor color) {
  const u32 count = mb->getVertexCount();
  const bool normalize = mb->getMaterial().NormalizeNormals;
  int i;
  for (i = 0; i < count; ++i) {
    float3 normalizedNormal = mb->getNormal(i);
    if (normalize) {
      normalizedNormal.normalize();
    }
    const float3& pos = mb->getPosition(i);
    draw3DLine(pos, pos + (normalizedNormal * length), color);
  }
}

SHWBufferLink* CNullDriver_getBufferLink(IVideoDriver* s, const IMeshBuffer * mb) {
  if (!mb || !isHardwareBufferRecommend(mb)) {
    return 0;
  }
  //search for hardware links
  map< const IMeshBuffer*, SHWBufferLink* >::Node* node = HWBufferMap.find(mb);
  if (node) {
    return node->getValue();
  }
  return createHardwareBuffer(mb); //no hardware links, and mesh wants one, create it
}

void CNullDriver_deleteHardwareBuffer(IVideoDriver* s, SHWBufferLink * HWBuffer) {
  if (!HWBuffer) {
    return;
  }
  HWBufferMap.remove(HWBuffer->MeshBuffer);
  delete HWBuffer;
}
//! Remove hardware buffer
void CNullDriver_removeHardwareBuffer(IVideoDriver* s, const IMeshBuffer * mb) {
  map<const IMeshBuffer*, SHWBufferLink*>::Node* node = HWBufferMap.find(mb);
  if (node) {
    deleteHardwareBuffer(node->getValue());
  }
}
bool CNullDriver_isHardwareBufferRecommend(IVideoDriver* s, const IMeshBuffer * mb) {
  if (!mb || (mb->getHardwareMappingHint_Index() == EHM_NEVER && mb->getHardwareMappingHint_Vertex() == EHM_NEVER)) {
    return false;
  }
  if (mb->getVertexCount() < MinVertexCountForVBO) {
    return false;
  }
  return true;
}
//! Create occlusion query.
/** Use node for identification and mesh for occlusion test. */
void CNullDriver_addOcclusionQuery(IVideoDriver* s, ISceneNode * node, const IMesh * mesh) {
  if (!node) {
    return;
  }
  if (!mesh) {
    if ((node->getType() != ESNT_MESH) && (node->getType() != ESNT_ANIMATED_MESH)) {
      return;
    }
    else if (node->getType() == ESNT_MESH) {
      mesh = static_cast<IMeshSceneNode*>(node)->getMesh();
    }
    else {
      mesh = static_cast<IAnimatedMeshSceneNode*>(node)->getMesh()->getMesh(0);
    }
    if (!mesh) {
      return;
    }
  }
  //search for query
  s32 index = OcclusionQueries.linear_search(SOccQuery(node));
  if (index != -1) {
    if (OcclusionQueries[index].Mesh != mesh) {
      OcclusionQueries[index].Mesh->drop();
      OcclusionQueries[index].Mesh = mesh;
      mesh->grab();
    }
  }
  else {
    OcclusionQueries.push_back(SOccQuery(node, mesh));
    node->setAutomaticCulling(node->getAutomaticCulling() | EAC_OCC_QUERY);
  }
}
//! Remove occlusion query.
void CNullDriver_removeOcclusionQuery(IVideoDriver* s, ISceneNode * node) {
  //search for query
  s32 index = OcclusionQueries.linear_search(SOccQuery(node));
  if (index != -1) {
    node->setAutomaticCulling(node->getAutomaticCulling() & ~EAC_OCC_QUERY);
    OcclusionQueries.erase(index);
  }
}
//! Remove all occlusion queries.
void CNullDriver_removeAllOcclusionQueries(IVideoDriver* s) {
  for (s32 i = OcclusionQueries_size - 1; i >= 0; --i) {
    removeOcclusionQuery(OcclusionQueries[i].Node);
  }
}
//! Run occlusion query. Draws mesh stored in query.
/** If the mesh shall be rendered visible, use
flag to enable the proper material setting. */
void CNullDriver_runOcclusionQuery(IVideoDriver* s, ISceneNode * node, bool visible) {
  if (!node) {
    return;
  }
  s32 index = OcclusionQueries.linear_search(SOccQuery(node));
  if (index == -1) {
    return;
  }
  OcclusionQueries[index].Run = 0;
  if (!visible) {
    SMaterial mat;
    mat.Lighting = false;
    mat.AntiAliasing = 0;
    mat.ColorMask = ECP_NONE;
    mat.GouraudShading = false;
    mat.ZWriteEnable = false;
    setMaterial(mat);
  }
  setTransform(ETS_WORLD, node->getAbsoluteTransformation());
  const IMesh* mesh = OcclusionQueries[index].Mesh;
  int i;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    if (visible) {
      setMaterial(mesh->getMeshBuffer(i)->getMaterial());
    }
    drawMeshBuffer(mesh->getMeshBuffer(i));
  }
}
//! Run all occlusion queries. Draws all meshes stored in queries.
/** If the meshes shall not be rendered visible, use
overrideMaterial to disable the color and depth buffer. */
void CNullDriver_runAllOcclusionQueries(IVideoDriver* s, bool visible) {
  int i;
  for (i = 0; i < OcclusionQueries_size; ++i) {
    runOcclusionQuery(OcclusionQueries[i].Node, visible);
  }
}
//! Return query result.
/** Return value is the number of visible pixels/fragments.
The value is a safe approximation, i.e. can be larger then the
actual value of pixels. */
u32 CNullDriver_getOcclusionQueryResult(IVideoDriver* s, ISceneNode * node) {
  return ~0;
}
//! Only used by the internal engine. Used to notify the driver that
//! the window was resized.
void CNullDriver_OnResize(IVideoDriver* s, const ISIZE & size) {
  if (ViewPort.getWidth() == (s32)ScreenSize.w &&
    ViewPort.getHeight() == (s32)ScreenSize.h)
    ViewPort = IRECT(IPOINT(0, 0),
    dimension2di(size));
  ScreenSize = size;
}
// adds a material renderer and drops it afterwards. To be used for internal creation
s32 CNullDriver_addAndDropMaterialRenderer(IVideoDriver* s, IMaterialRenderer * m) {
  s32 i = addMaterialRenderer(m);
  if (m) {
    m->drop();
  }
  return i;
}
//! Adds a new material renderer to the video device.
s32 CNullDriver_addMaterialRenderer(IVideoDriver* s, IMaterialRenderer * renderer, const char * name) {
  if (!renderer) {
    return -1;
  }
  SMaterialRenderer r;
  r.Renderer = renderer;
  r.Name = name;
  if (name == 0 && (MaterialRenderers_size < (sizeof(sBuiltInMaterialTypeNames) / sizeof(char*)) - 1)) {
    // set name of built in renderer so that we don't have to implement name
    // setting in all available renderers.
    r.Name = sBuiltInMaterialTypeNames[MaterialRenderers_size];
  }
  MaterialRenderers.push_back(r);
  renderer->grab();
  return MaterialRenderers_size - 1;
}
//! Sets the name of a material renderer.
void CNullDriver_setMaterialRendererName(IVideoDriver* s, s32 idx, const char * name) {
  if (idx < s32(sizeof(sBuiltInMaterialTypeNames) / sizeof(char*)) - 1 ||
    idx >= (s32)MaterialRenderers_size) {
    return;
  }
  MaterialRenderers[idx].Name = name;
}
//! Creates material attributes list from a material, usable for serialization and more.
IAttributes* CNullDriver_createAttributesFromMaterial(IVideoDriver* s, const SMaterial & material,
                                                      SAttributeReadWriteOptions * options) {
  CAttributes* attr = new CAttributes(this);
  attr->addEnum("Type", material.MaterialType, sBuiltInMaterialTypeNames);
  attr->addColor("Ambient", material.AmbientColor);
  attr->addColor("Diffuse", material.DiffuseColor);
  attr->addColor("Emissive", material.EmissiveColor);
  attr->addColor("Specular", material.SpecularColor);
  attr->addFloat("Shininess", material.Shininess);
  attr->addFloat("Param1", material.MaterialTypeParam);
  attr->addFloat("Param2", material.MaterialTypeParam2);
  char* prefix = "Texture";
  u32 i;
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    if (options && (options->Flags & EARWF_USE_RELATIVE_PATHS) && options->Filename && material.getTexture(i)) {
      char* path = FileSystem->getRelativeFilename(
        FileSystem->getAbsolutePath(material.getTexture(i)->getName()), options->Filename);
      attr->addTexture((prefix + char * (i + 1)), material.getTexture(i), path);
    }
    else {
      attr->addTexture((prefix + char * (i + 1)), material.getTexture(i));
    }
  }
  attr->addBool("Wireframe", material.Wireframe);
  attr->addBool("GouraudShading", material.GouraudShading);
  attr->addBool("Lighting", material.Lighting);
  attr->addBool("ZWriteEnable", material.ZWriteEnable);
  attr->addInt("ZBuffer", material.ZBuffer);
  attr->addBool("BackfaceCulling", material.BackfaceCulling);
  attr->addBool("FrontfaceCulling", material.FrontfaceCulling);
  attr->addBool("FogEnable", material.FogEnable);
  attr->addBool("NormalizeNormals", material.NormalizeNormals);
  attr->addBool("UseMipMaps", material.UseMipMaps);
  attr->addInt("AntiAliasing", material.AntiAliasing);
  attr->addInt("ColorMask", material.ColorMask);
  attr->addInt("ColorMaterial", material.ColorMaterial);
  attr->addInt("PolygonOffsetFactor", material.PolygonOffsetFactor);
  attr->addEnum("PolygonOffsetDirection", material.PolygonOffsetDirection, PolygonOffsetDirectionNames);
  prefix = "BilinearFilter";
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    attr->addBool((prefix + char * (i + 1)), material.TextureLayer[i].BilinearFilter);
  }
  prefix = "TrilinearFilter";
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    attr->addBool((prefix + char * (i + 1)), material.TextureLayer[i].TrilinearFilter);
  }
  prefix = "AnisotropicFilter";
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    attr->addInt((prefix + char * (i + 1)), material.TextureLayer[i].AnisotropicFilter);
  }
  prefix = "TextureWrapU";
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    attr->addEnum((prefix + char * (i + 1)), material.TextureLayer[i].TextureWrapU, aTextureClampNames);
  }
  prefix = "TextureWrapV";
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    attr->addEnum((prefix + char * (i + 1)), material.TextureLayer[i].TextureWrapV, aTextureClampNames);
  }
  prefix = "LODBias";
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    attr->addInt((prefix + char * (i + 1)), material.TextureLayer[i].LODBias);
  }
  return attr;
}
//! Fills an SMaterial structure from attributes.
void CNullDriver_fillMaterialStructureFromAttributes(IVideoDriver* s, SMaterial & outMaterial, IAttributes * attr) {
  outMaterial.MaterialType = EMT_SOLID;
  char* name = attr->getAttributeAsString("Type");
  u32 i;
  for (i = 0; i < MaterialRenderers_size; ++i) {
    if (name == MaterialRenderers[i].Name) {
      outMaterial.MaterialType = (E_MATERIAL_TYPE)i;
      break;
    }
  }
  outMaterial.AmbientColor = attr->getAttributeAsColor("Ambient");
  outMaterial.DiffuseColor = attr->getAttributeAsColor("Diffuse");
  outMaterial.EmissiveColor = attr->getAttributeAsColor("Emissive");
  outMaterial.SpecularColor = attr->getAttributeAsColor("Specular");
  outMaterial.Shininess = attr->getAttributeAsFloat("Shininess");
  outMaterial.MaterialTypeParam = attr->getAttributeAsFloat("Param1");
  outMaterial.MaterialTypeParam2 = attr->getAttributeAsFloat("Param2");
  char* prefix = "Texture";
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    outMaterial.setTexture(i, attr->getAttributeAsTexture((prefix + char * (i + 1))));
  }
  outMaterial.Wireframe = attr->getAttributeAsBool("Wireframe");
  outMaterial.GouraudShading = attr->getAttributeAsBool("GouraudShading");
  outMaterial.Lighting = attr->getAttributeAsBool("Lighting");
  outMaterial.ZWriteEnable = attr->getAttributeAsBool("ZWriteEnable");
  outMaterial.ZBuffer = (u8)attr->getAttributeAsInt("ZBuffer");
  outMaterial.BackfaceCulling = attr->getAttributeAsBool("BackfaceCulling");
  outMaterial.FrontfaceCulling = attr->getAttributeAsBool("FrontfaceCulling");
  outMaterial.FogEnable = attr->getAttributeAsBool("FogEnable");
  outMaterial.NormalizeNormals = attr->getAttributeAsBool("NormalizeNormals");
  if (attr->existsAttribute("UseMipMaps")) { // legacy
    outMaterial.UseMipMaps = attr->getAttributeAsBool("UseMipMaps");
  }
  else {
    outMaterial.UseMipMaps = true;
  }
  // default 0 is ok
  outMaterial.AntiAliasing = attr->getAttributeAsInt("AntiAliasing");
  if (attr->existsAttribute("ColorMask")) {
    outMaterial.ColorMask = attr->getAttributeAsInt("ColorMask");
  }
  if (attr->existsAttribute("ColorMaterial")) {
    outMaterial.ColorMaterial = attr->getAttributeAsInt("ColorMaterial");
  }
  if (attr->existsAttribute("PolygonOffsetFactor")) {
    outMaterial.PolygonOffsetFactor = attr->getAttributeAsInt("PolygonOffsetFactor");
  }
  if (attr->existsAttribute("PolygonOffsetDirection")) {
    outMaterial.PolygonOffsetDirection = (E_POLYGON_OFFSET)attr->getAttributeAsEnumeration("PolygonOffsetDirection", PolygonOffsetDirectionNames);
  }
  prefix = "BilinearFilter";
  if (attr->existsAttribute(prefix)) { // legacy
    outMaterial.setFlag(EMF_BILINEAR_FILTER, attr->getAttributeAsBool(prefix));
  }
  else {
    for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
      outMaterial.TextureLayer[i].BilinearFilter = attr->getAttributeAsBool((prefix + char * (i + 1)));
    }
  }
  prefix = "TrilinearFilter";
  if (attr->existsAttribute(prefix)) { // legacy
    outMaterial.setFlag(EMF_TRILINEAR_FILTER, attr->getAttributeAsBool(prefix));
  }
  else {
    for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
      outMaterial.TextureLayer[i].TrilinearFilter = attr->getAttributeAsBool((prefix + char * (i + 1)));
    }
  }
  prefix = "AnisotropicFilter";
  if (attr->existsAttribute(prefix)) { // legacy
    outMaterial.setFlag(EMF_ANISOTROPIC_FILTER, attr->getAttributeAsBool(prefix));
  }
  else {
    for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
      outMaterial.TextureLayer[i].AnisotropicFilter = attr->getAttributeAsInt((prefix + char * (i + 1)));
    }
  }
  prefix = "TextureWrap";
  if (attr->existsAttribute(prefix)) { // legacy
    for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
      outMaterial.TextureLayer[i].TextureWrapU = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + char * (i + 1)), aTextureClampNames);
      outMaterial.TextureLayer[i].TextureWrapV = outMaterial.TextureLayer[i].TextureWrapU;
    }
  }
  else {
    for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
      outMaterial.TextureLayer[i].TextureWrapU = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + "U" + char * (i + 1)), aTextureClampNames);
      outMaterial.TextureLayer[i].TextureWrapV = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + "V" + char * (i + 1)), aTextureClampNames);
    }
  }
  // default 0 is ok
  prefix = "LODBias";
  for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
    outMaterial.TextureLayer[i].LODBias = attr->getAttributeAsInt((prefix + char * (i + 1)));
  }
}
//! Returns driver and operating system specific data about the IVideoDriver.
const SExposedVideoData& CNullDriver_getExposedVideoData(IVideoDriver* s) {
  return ExposedData;
}
//! Returns type of video driver
E_DRIVER_TYPE CNullDriver_getDriverType(IVideoDriver* s) {
  return EDT_NULL;
}
//! Returns pointer to material renderer or null
IMaterialRenderer* CNullDriver_getMaterialRenderer(IVideoDriver* s, u32 idx) {
  if (idx < MaterialRenderers_size) {
    return MaterialRenderers[idx].Renderer;
  }
  else {
    return 0;
  }
}
//! Returns amount of currently available material renderers.
u32 CNullDriver_getMaterialRendererCount(IVideoDriver* s) {
  return MaterialRenderers_size;
}
//! Returns name of the material renderer
const char* CNullDriver_getMaterialRendererName(IVideoDriver* s, u32 idx) {
  if (idx < MaterialRenderers_size) {
    return MaterialRenderers[idx].Name;
  }
  return 0;
}
//! Returns pointer to the IGPUProgrammingServices interface.
IGPUProgrammingServices* CNullDriver_getGPUProgrammingServices(IVideoDriver* s) {
  return this;
}
//! Adds a new material renderer to the VideoDriver, based on a high level shading language.
s32 CNullDriver_addHighLevelShaderMaterial(IVideoDriver* s, 
                                           const c8 * vertexShaderProgram,
                                           const c8 * vertexShaderEntryPointName,
                                           E_VERTEX_SHADER_TYPE vsCompileTarget,
                                           const c8 * pixelShaderProgram,
                                           const c8 * pixelShaderEntryPointName,
                                           E_PIXEL_SHADER_TYPE psCompileTarget,
                                           const c8 * geometryShaderProgram,
                                           const c8 * geometryShaderEntryPointName,
                                           E_GEOMETRY_SHADER_TYPE gsCompileTarget,
                                           E_PRIMITIVE_TYPE inType, E_PRIMITIVE_TYPE outType,
                                           u32 verticesOut,
                                           IShaderConstantSetCallBack * callback,
                                           E_MATERIAL_TYPE baseMaterial,
                                           s32 userData, E_GPU_SHADING_LANGUAGE shadingLang) {
  llogprintf(ELL_NONE, "High level shader materials not available (yet) in this driver, sorry");
  return -1;
}
//! Like IGPUProgrammingServices::addShaderMaterial() (look there for a detailed description),
//! but tries to load the programs from files.
s32 CNullDriver_addHighLevelShaderMaterialFromFiles(IVideoDriver* s, 
                                                    const char * vertexShaderProgramFileName,
                                                    const c8 * vertexShaderEntryPointName,
                                                    E_VERTEX_SHADER_TYPE vsCompileTarget,
                                                    const char * pixelShaderProgramFileName,
                                                    const c8 * pixelShaderEntryPointName,
                                                    E_PIXEL_SHADER_TYPE psCompileTarget,
                                                    const char * geometryShaderProgramFileName,
                                                    const c8 * geometryShaderEntryPointName,
                                                    E_GEOMETRY_SHADER_TYPE gsCompileTarget,
                                                    E_PRIMITIVE_TYPE inType, E_PRIMITIVE_TYPE outType,
                                                    u32 verticesOut,
                                                    IShaderConstantSetCallBack * callback,
                                                    E_MATERIAL_TYPE baseMaterial,
                                                    s32 userData, E_GPU_SHADING_LANGUAGE shadingLang) {
  IReadFile* vsfile = 0;
  IReadFile* psfile = 0;
  IReadFile* gsfile = 0;
  if (vertexShaderProgramFileName_size) {
    vsfile = FileSystem->createAndOpenFile(vertexShaderProgramFileName);
    if (!vsfile) {
      llogprintf(ELL_NONE, "Could not open vertex shader program file",
        vertexShaderProgramFileName, ELL_WARNING);
    }
  }
  if (pixelShaderProgramFileName_size) {
    psfile = FileSystem->createAndOpenFile(pixelShaderProgramFileName);
    if (!psfile) {
      llogprintf(ELL_NONE, "Could not open pixel shader program file",
        pixelShaderProgramFileName, ELL_WARNING);
    }
  }
  if (geometryShaderProgramFileName_size) {
    gsfile = FileSystem->createAndOpenFile(geometryShaderProgramFileName);
    if (!gsfile) {
      llogprintf(ELL_NONE, "Could not open geometry shader program file",
        geometryShaderProgramFileName, ELL_WARNING);
    }
  }
  s32 result = addHighLevelShaderMaterialFromFiles(
    vsfile, vertexShaderEntryPointName, vsCompileTarget,
    psfile, pixelShaderEntryPointName, psCompileTarget,
    gsfile, geometryShaderEntryPointName, gsCompileTarget,
    inType, outType, verticesOut,
    callback, baseMaterial, userData, shadingLang);
  if (psfile) {
    psfile->drop();
  }
  if (vsfile) {
    vsfile->drop();
  }
  if (gsfile) {
    gsfile->drop();
  }
  return result;
}
//! Like IGPUProgrammingServices::addShaderMaterial() (look there for a detailed description),
//! but tries to load the programs from files.
s32 CNullDriver_addHighLevelShaderMaterialFromFiles(IVideoDriver* s, 
                                                    IReadFile * vertexShaderProgram,
                                                    const c8 * vertexShaderEntryPointName,
                                                    E_VERTEX_SHADER_TYPE vsCompileTarget,
                                                    IReadFile * pixelShaderProgram,
                                                    const c8 * pixelShaderEntryPointName,
                                                    E_PIXEL_SHADER_TYPE psCompileTarget,
                                                    IReadFile * geometryShaderProgram,
                                                    const c8 * geometryShaderEntryPointName,
                                                    E_GEOMETRY_SHADER_TYPE gsCompileTarget,
                                                    E_PRIMITIVE_TYPE inType, E_PRIMITIVE_TYPE outType,
                                                    u32 verticesOut,
                                                    IShaderConstantSetCallBack * callback,
                                                    E_MATERIAL_TYPE baseMaterial,
                                                    s32 userData, E_GPU_SHADING_LANGUAGE shadingLang) {
  c8* vs = 0;
  c8* ps = 0;
  c8* gs = 0;
  if (vertexShaderProgram) {
    const long size = vertexShaderProgram->getSize();
    if (size) {
      vs = new c8[size + 1];
      vertexShaderProgram->read(vs, size);
      vs[size] = 0;
    }
  }
  if (pixelShaderProgram) {
    const long size = pixelShaderProgram->getSize();
    if (size) {
      // if both handles are the same we must reset the file
      if (pixelShaderProgram == vertexShaderProgram) {
        pixelShaderProgram->seek(0);
      }
      ps = new c8[size + 1];
      pixelShaderProgram->read(ps, size);
      ps[size] = 0;
    }
  }
  if (geometryShaderProgram) {
    const long size = geometryShaderProgram->getSize();
    if (size) {
      // if both handles are the same we must reset the file
      if ((geometryShaderProgram == vertexShaderProgram) ||
        (geometryShaderProgram == pixelShaderProgram)) {
        geometryShaderProgram->seek(0);
      }
      gs = new c8[size + 1];
      geometryShaderProgram->read(gs, size);
      gs[size] = 0;
    }
  }
  s32 result = this->addHighLevelShaderMaterial(
    vs, vertexShaderEntryPointName, vsCompileTarget,
    ps, pixelShaderEntryPointName, psCompileTarget,
    gs, geometryShaderEntryPointName, gsCompileTarget,
    inType, outType, verticesOut,
    callback, baseMaterial, userData, shadingLang);
  delete [] vs;
  delete [] ps;
  delete [] gs;
  return result;
}
//! Adds a new material renderer to the VideoDriver, using pixel and/or
//! vertex shaders to render geometry.
s32 CNullDriver_addShaderMaterial(IVideoDriver* s, const c8 * vertexShaderProgram,
                                  const c8 * pixelShaderProgram,
                                  IShaderConstantSetCallBack * callback,
                                  E_MATERIAL_TYPE baseMaterial,
                                  s32 userData) {
  llogprintf(ELL_NONE, "Shader materials not implemented yet in this driver, sorry.");
  return -1;
}
//! Like IGPUProgrammingServices::addShaderMaterial(), but tries to load the
//! programs from files.
s32 CNullDriver_addShaderMaterialFromFiles(IVideoDriver* s, IReadFile * vertexShaderProgram,
                                           IReadFile * pixelShaderProgram,
                                           IShaderConstantSetCallBack * callback,
                                           E_MATERIAL_TYPE baseMaterial,
                                           s32 userData) {
  c8* vs = 0;
  c8* ps = 0;
  if (vertexShaderProgram) {
    const long size = vertexShaderProgram->getSize();
    if (size) {
      vs = new c8[size + 1];
      vertexShaderProgram->read(vs, size);
      vs[size] = 0;
    }
  }
  if (pixelShaderProgram) {
    const long size = pixelShaderProgram->getSize();
    if (size) {
      ps = new c8[size + 1];
      pixelShaderProgram->read(ps, size);
      ps[size] = 0;
    }
  }
  s32 result = addShaderMaterial(vs, ps, callback, baseMaterial, userData);
  delete [] vs;
  delete [] ps;
  return result;
}
//! Like IGPUProgrammingServices::addShaderMaterial(), but tries to load the
//! programs from files.
s32 CNullDriver_addShaderMaterialFromFiles(IVideoDriver* s, const char * vertexShaderProgramFileName,
                                           const char * pixelShaderProgramFileName,
                                           IShaderConstantSetCallBack * callback,
                                           E_MATERIAL_TYPE baseMaterial,
                                           s32 userData) {
  IReadFile* vsfile = 0;
  IReadFile* psfile = 0;
  if (vertexShaderProgramFileName_size) {
    vsfile = FileSystem->createAndOpenFile(vertexShaderProgramFileName);
    if (!vsfile) {
      llogprintf(ELL_NONE, "Could not open vertex shader program file",
        vertexShaderProgramFileName, ELL_WARNING);
      return -1;
    }
  }
  if (pixelShaderProgramFileName_size) {
    psfile = FileSystem->createAndOpenFile(pixelShaderProgramFileName);
    if (!psfile) {
      llogprintf(ELL_NONE, "Could not open pixel shader program file",
        pixelShaderProgramFileName, ELL_WARNING);
      if (vsfile) {
        vsfile->drop();
      }
      return -1;
    }
  }
  s32 result = addShaderMaterialFromFiles(vsfile, psfile, callback,
    baseMaterial, userData);
  if (psfile) {
    psfile->drop();
  }
  if (vsfile) {
    vsfile->drop();
  }
  return result;
}
//! Creates a render target texture.
ITexture* CNullDriver_addRenderTargetTexture(IVideoDriver* s, const ISIZE & size,
                                             const char * name, const ECOLOR_FORMAT format) {
  return 0;
}
//! Clears the ZBuffer.
void CNullDriver_clearZBuffer() {
}
//! Returns a pointer to the mesh manipulator.
IMeshManipulator* CNullDriver_getMeshManipulator(IVideoDriver* s) {
  return MeshManipulator;
}
//! Returns an image created from the last rendered frame.
IImage* CNullDriver_createScreenShot(IVideoDriver* s, ECOLOR_FORMAT format, E_RENDER_TARGET target) {
  return 0;
}
// prints renderer version
void CNullDriver_printVersion(IVideoDriver* s) {
  char* namePrint = "Using renderer: ";
  namePrint += getName();
  llogprintf(namePrint, ELL_INFORMATION);
}
//! creates a video driver
IVideoDriver* createNullDriver(IVideoDriver* s, IFileSystem * io, const ISIZE & screenSize) {
  CNullDriver* nullDriver = new CNullDriver(io, screenSize);
  // create empty material renderers
  int i;
  for (i = 0; sBuiltInMaterialTypeNames[i]; ++i) {
    IMaterialRenderer* imr = new IMaterialRenderer();
    nullDriver->addMaterialRenderer(imr);
    imr->drop();
  }
  return nullDriver;
}
//! Set/unset a clipping plane.
//! There are at least 6 clipping planes available for the user to set at will.
//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
//! \param plane: The plane itself.
//! \param enable: If true, enable the clipping plane else disable it.
bool CNullDriver_setClipPlane(IVideoDriver* s, u32 index, const plane3df & plane, bool enable) {
  return false;
}
//! Enable/disable a clipping plane.
void CNullDriver_enableClipPlane(IVideoDriver* s, u32 index, bool enable) {
  // not necessary
}
ITexture* CNullDriver_createRenderTargetTexture(IVideoDriver* s, const ISIZE & size,
                                                const c8 * name) {
  llogprintf(ELL_NONE, "createRenderTargetTexture is deprecated, use addRenderTargetTexture instead");
  ITexture* tex = addRenderTargetTexture(size, name);
  tex->grab();
  return tex;
}
void CNullDriver_setMinHardwareBufferVertexCount(IVideoDriver* s, u32 count) {
  MinVertexCountForVBO = count;
}
SOverrideMaterial& CNullDriver_getOverrideMaterial(IVideoDriver* s) {
  return OverrideMaterial;
}
//! Get the 2d override material for altering its values
SMaterial& CNullDriver_getMaterial2D(IVideoDriver* s) {
  return OverrideMaterial2D;
}
//! Enable the 2d override material
void CNullDriver_enableMaterial2D(bool enable) {
  OverrideMaterial2DEnabled = enable;
}
ISIZE CNullDriver_getMaxTextureSize(IVideoDriver* s) {
  return ISIZE(0x10000, 0x10000); // maybe large enough
}
//! Color conversion convenience function
void CNullDriver_convertColor(IVideoDriver* s, const void * sP, ECOLOR_FORMAT sF, s32 sN, void * dP, ECOLOR_FORMAT dF) {
  CColorConverter::convert_viaFormat(sP, sF, sN, dP, dF);
}

#endif