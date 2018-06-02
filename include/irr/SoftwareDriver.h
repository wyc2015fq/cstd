
#include "CBlit.h"
#include "CColorConverter.h"
//#include "CImage_impl.h"
#define X x
#define Y y
#include "TriangleRenderer/TriangleRenderer.inl"
#undef X
#undef Y

#define TRIANGLERENDERER_DEF(TT) \
  TT(ETR_FLAT, CTRFlat) \
  TT(ETR_FLAT_WIRE, CTRFlatWire) \
  TT(ETR_GOURAUD, CTRGouraud) \
  TT(ETR_GOURAUD_WIRE, CTRGouraudWire) \
  TT(ETR_TEXTURE_FLAT, CTRTextureFlat) \
  TT(ETR_TEXTURE_FLAT_WIRE, CTRTextureFlatWire) \
  TT(ETR_TEXTURE_GOURAUD, CTRTextureGouraud) \
  TT(ETR_TEXTURE_GOURAUD_WIRE, CTRTextureGouraudWire) \
  TT(ETR_TEXTURE_GOURAUD_NOZ, CTRTextureGouraudNoZ) \
TT(ETR_TEXTURE_GOURAUD_ADD,CTRTextureGouraudAdd)

void CSoftwareDriver_free(IVideoDriver* s)
{
  IImage_free(s->RenderTargetSurface);
  IImage_free(s->ZBuffer);
}

typedef void (*drawIndexedTriangleList_t)(ITriangleRenderer* s, S2DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount);

void CSoftwareDriver_drawIndexedTriangleList(IVideoDriver* s, S2DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount) {
  static int drawIndexedTriangleList_inited = 0;
  ETriangleRenderer renderer = ETR_FLAT;
  static drawIndexedTriangleList_t TriangleRenderers[20] = {0};
  ITriangleRenderer Renderer[1] = {0};
  if (!drawIndexedTriangleList_inited) {
    drawIndexedTriangleList_inited = 1;
#define TT(a, b)   TriangleRenderers[a] = b ## _drawIndexedTriangleList;
    TRIANGLERENDERER_DEF(TT)
#undef TT
  }
  
  if (s->Texture) {
    if (!s->Material.GouraudShading) {
      renderer = (!s->Material.Wireframe) ? ETR_TEXTURE_FLAT : ETR_TEXTURE_FLAT_WIRE;
    }
    else {
      if (s->Material.Wireframe) {
        renderer = ETR_TEXTURE_GOURAUD_WIRE;
      }
      else {
        if (s->Material.MaterialType == EMT_TRANSPARENT_ADD_COLOR ||
          s->Material.MaterialType == EMT_TRANSPARENT_ALPHA_CHANNEL ||
          s->Material.MaterialType == EMT_TRANSPARENT_VERTEX_ALPHA) {
          // simply draw all transparent stuff with the same renderer. at
          // least it is transparent then.
          renderer = ETR_TEXTURE_GOURAUD_ADD;
        }
        else if ((s->Material.ZBuffer == ECFN_NEVER) && !s->Material.ZWriteEnable) {
          renderer = ETR_TEXTURE_GOURAUD_NOZ;
        }
        else {
          renderer = ETR_TEXTURE_GOURAUD;
        }
      }
    }
  }
  else {
    if (!s->Material.GouraudShading) {
      renderer = (!s->Material.Wireframe) ? ETR_FLAT : ETR_FLAT_WIRE;
    }
    else {
      renderer = (!s->Material.Wireframe) ? ETR_GOURAUD : ETR_GOURAUD_WIRE;
    }
  }
  //! switches to a triangle renderer
  if (s->Texture) {
    IImage* im = s->Texture->Texture;
    Renderer->lockedTexture = (u16*)im->Data;
    Renderer->lockedTextureWidth = im->Pitch;
    Renderer->textureYMask = im->Size.h-1;
    Renderer->textureXMask = im->Size.w-1;
  }
  Renderer->lockedZBuffer = (u16*)s->ZBuffer->Data;
  Renderer->BackFaceCullingEnabled = (s->Material.BackfaceCulling == true);
  Renderer->ViewPortRect = s->ViewPort;
  Renderer->SurfaceWidth = s->RenderTargetSurface->Size.w;
  Renderer->SurfaceHeight = s->RenderTargetSurface->Size.h;
  TriangleRenderers[renderer](Renderer, vertices, vertexCount, indexList, triangleCount);
}

void CSoftwareDriver_init(IVideoDriver* s, ISIZE windowSize, ITexture* texture)
{
  CNullDriver_init(s, windowSize);
  // create backbuffer
  IImage_setsize(s->RenderTargetSurface, ECF_A1R5G5B5, windowSize);
  IImage_setsize(s->ZBuffer, ECF_A1R5G5B5, windowSize);
  s->Texture = texture;
}
#if 0

//! sets a material
void CSoftwareDriver::setMaterial(const SMaterial& material)
{
  s->Material = material;
  OverrideMaterial.apply(s->Material);
  int i;
  for (i = 0; i < 1; ++i) {
    setActiveTexture(i, s->Material.getTexture(i));
    setTransform((E_TRANSFORMATION_STATE)(ETS_TEXTURE_0 + i),
        material.getTextureMatrix(i));
  }
}
//! clears the zbuffer
bool CSoftwareDriver::beginScene(bool backBuffer, bool zBuffer, SColor color,
    const SExposedVideoData& videoData, IRECT* sourceRect)
{
  CNullDriver::beginScene(backBuffer, zBuffer, color, videoData, sourceRect);
  WindowId = videoData.D3D9.HWnd;
  SceneSourceRect = sourceRect;
  if (backBuffer && BackBuffer) {
    BackBuffer->fill(color);
  }
  if (ZBuffer && zBuffer) {
    ZBuffer->clear();
  }
  return true;
}
//! presents the rendered scene on the screen, returns false if failed
bool CSoftwareDriver::endScene()
{
  CNullDriver::endScene();
  return Presenter->present(BackBuffer, WindowId, SceneSourceRect);
}
//! sets a viewport
void CSoftwareDriver::setViewPort(const IRECT& area)
{
  ViewPort = area;
  //TODO: the clipping is not correct, because the projection is affected.
  // to correct this, ViewPortSize and Render2DTranslation will have to be corrected.
  IRECT rendert(0, 0, RenderTargetSize.Width, RenderTargetSize.Height);
  ViewPort.clipAgainst(rendert);
  ViewPortSize = ISIZE(ViewPort.getSize());
  Render2DTranslation.X = (ViewPortSize.Width / 2) + ViewPort.UpperLeftCorner.X;
  Render2DTranslation.Y = ViewPort.UpperLeftCorner.Y + ViewPortSize.Height - (ViewPortSize.Height / 2);// + ViewPort.UpperLeftCorner.Y;
  if (CurrentTriangleRenderer) {
    CurrentTriangleRenderer->setRenderTarget(RenderTargetSurface, ViewPort);
  }
}
void CSoftwareDriver::drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
    const void* indexList, u32 primitiveCount,
    E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
{
  switch (iType) {
  case (EIT_16BIT): {
    drawVertexPrimitiveList16(vertices, vertexCount, (const u16*)indexList, primitiveCount, vType, pType);
    break;
  }
  case (EIT_32BIT): {
    os::Printer::log("Software driver can not render 32bit buffers", ELL_ERROR);
    break;
  }
  }
}
//! draws a vertex primitive list
void CSoftwareDriver::drawVertexPrimitiveList16(const void* vertices, u32 vertexCount, const u16* indexList, u32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType)
{
  const u16* indexPointer = 0;
  u16* newBuffer;
  switch (pType) {
  case EPT_LINE_STRIP: {
    switch (vType) {
    case EVT_STANDARD: {
      int i;
      for (i = 0; i < primitiveCount - 1; ++i)
        draw3DLine(((S3DVertex*)vertices)[indexList[i]].Pos,
            ((S3DVertex*)vertices)[indexList[i + 1]].Pos,
            ((S3DVertex*)vertices)[indexList[i]].Color);
    }
    break;
    case EVT_2TCOORDS: {
      int i;
      for (i = 0; i < primitiveCount - 1; ++i)
        draw3DLine(((S3DVertex*)vertices)[indexList[i]].Pos,
            ((S3DVertex*)vertices)[indexList[i + 1]].Pos,
            ((S3DVertex*)vertices)[indexList[i]].Color);
    }
    break;
    case EVT_TANGENTS: {
      int i;
      for (i = 0; i < primitiveCount - 1; ++i)
        draw3DLine(((S3DVertex*)vertices)[indexList[i]].Pos,
            ((S3DVertex*)vertices)[indexList[i + 1]].Pos,
            ((S3DVertex*)vertices)[indexList[i]].Color);
    }
    break;
    }
  }
  return;
  case EPT_LINE_LOOP:
    drawVertexPrimitiveList16(vertices, vertexCount, indexList, primitiveCount - 1, vType, EPT_LINE_STRIP);
    switch (vType) {
    case EVT_STANDARD:
      draw3DLine(((S3DVertex*)vertices)[indexList[primitiveCount - 1]].Pos,
          ((S3DVertex*)vertices)[indexList[0]].Pos,
          ((S3DVertex*)vertices)[indexList[primitiveCount - 1]].Color);
      break;
    case EVT_2TCOORDS:
      draw3DLine(((S3DVertex*)vertices)[indexList[primitiveCount - 1]].Pos,
          ((S3DVertex*)vertices)[indexList[0]].Pos,
          ((S3DVertex*)vertices)[indexList[primitiveCount - 1]].Color);
      break;
    case EVT_TANGENTS:
      draw3DLine(((S3DVertex*)vertices)[indexList[primitiveCount - 1]].Pos,
          ((S3DVertex*)vertices)[indexList[0]].Pos,
          ((S3DVertex*)vertices)[indexList[primitiveCount - 1]].Color);
      break;
    }
    return;
  case EPT_LINES: {
    switch (vType) {
    case EVT_STANDARD: {
      int i;
      for (i = 0; i < 2 * primitiveCount; i += 2)
        draw3DLine(((S3DVertex*)vertices)[indexList[i]].Pos,
            ((S3DVertex*)vertices)[indexList[i + 1]].Pos,
            ((S3DVertex*)vertices)[indexList[i]].Color);
    }
    break;
    case EVT_2TCOORDS: {
      int i;
      for (i = 0; i < 2 * primitiveCount; i += 2)
        draw3DLine(((S3DVertex*)vertices)[indexList[i]].Pos,
            ((S3DVertex*)vertices)[indexList[i + 1]].Pos,
            ((S3DVertex*)vertices)[indexList[i]].Color);
    }
    break;
    case EVT_TANGENTS: {
      int i;
      for (i = 0; i < 2 * primitiveCount; i += 2)
        draw3DLine(((S3DVertex*)vertices)[indexList[i]].Pos,
            ((S3DVertex*)vertices)[indexList[i + 1]].Pos,
            ((S3DVertex*)vertices)[indexList[i]].Color);
    }
    break;
    }
  }
  return;
  case EPT_TRIANGLE_FAN: {
    // TODO: don't convert fan to list
    newBuffer.reallocate(primitiveCount * 3);
    for (int t = 0; t < primitiveCount; ++t) {
      newBuffer.push_back(indexList[0]);
      newBuffer.push_back(indexList[t + 1]);
      newBuffer.push_back(indexList[t + 2]);
    }
    indexPointer = newBuffer;
  }
  break;
  case EPT_TRIANGLES:
    indexPointer = indexList;
    break;
  default:
    return;
  }
  switch (vType) {
  case EVT_STANDARD:
    drawClippedIndexedTriangleListT((S3DVertex*)vertices, vertexCount, indexPointer, primitiveCount);
    break;
  case EVT_2TCOORDS:
    drawClippedIndexedTriangleListT((S3DVertex*)vertices, vertexCount, indexPointer, primitiveCount);
    break;
  case EVT_TANGENTS:
    drawClippedIndexedTriangleListT((S3DVertex*)vertices, vertexCount, indexPointer, primitiveCount);
    break;
  }
}
template<class VERTEXTYPE>
void CSoftwareDriver::drawClippedIndexedTriangleListT(const VERTEXTYPE* vertices,
    s32 vertexCount, const u16* indexList, s32 triangleCount)
{
  if (!RenderTargetSurface || !ZBuffer || !triangleCount) {
    return;
  }
  if (!checkPrimitiveCount(triangleCount)) {
    return;
  }
  // arrays for storing clipped vertices
  VERTEXTYPE > clippedVertices;
  u16* clippedIndices;
  // calculate inverse world transformation
  matrix4 worldinv(TransformationMatrix[ETS_WORLD]);
  worldinv.makeInverse();
  // calculate view frustum planes
  SViewFrustum frustum(TransformationMatrix[ETS_PROJECTION] * TransformationMatrix[ETS_VIEW]);
  // copy and transform clipping planes ignoring far plane
  plane3df planes[5]; // ordered by near, left, right, bottom, top
  for (int p = 0; p < 5; ++p) {
    worldinv.transformPlane(frustum.planes[p + 1], planes[p]);
  }
  EIntersectionRelation3D inout[3]; // is point in front or back of plane?
  // temporary buffer for vertices to be clipped by all planes
  VERTEXTYPE > tClpBuf;
  int t;
  int i;
  for (i = 0; i < triangleCount; ++i) { // for all input triangles
    // add next triangle to tempClipBuffer
    for (t = 0; t < 3; ++t) {
      tClpBuf.push_back(vertices[indexList[(i * 3) + t]]);
    }
    for (int p = 0; p < 5; ++p) // for all clip planes
      for (int v = 0; v < (int)tClpBuf_size; v += 3) { // for all vertices in temp clip buffer
        int inside = 0;
        int outside = 0;
        // test intersection relation of the current vertices
        for (t = 0; t < 3; ++t) {
          inout[t] = planes[p].classifyPointRelation(tClpBuf[v + t].Pos);
          if (inout[t] != ISREL3D_FRONT) {
            ++inside;
          }
          else if (inout[t] == ISREL3D_FRONT) {
            ++outside;
          }
        }
        if (!outside) {
          // add all vertices to new buffer, this triangle needs no clipping.
          // so simply don't change this part of the temporary triangle buffer
          continue;
        }
        if (!inside) {
          // all vertices are outside, don't add this triangle, so erase this
          // triangle from the tClpBuf
          tClpBuf.erase(v, 3);
          v -= 3;
          continue;
        }
#ifndef __SOFTWARE_CLIPPING_PROBLEM__
        tClpBuf.erase(v, 3);
        v -= 3;
#endif
      } // end for all clip planes
    // now add all remaining triangles in tempClipBuffer to clippedIndices
    // and clippedVertices array.
    if (clippedIndices_size + tClpBuf_size < 65535)
      for (t = 0; t < (int)tClpBuf_size; ++t) {
        clippedIndices.push_back(clippedVertices_size);
        clippedVertices.push_back(tClpBuf[t]);
      }
    tClpBuf.clear();
  } // end for all input triangles
  // draw newly created triangles.
  // -----------------------------------------------------------
  // here all triangles are being drawn. I put this in a separate
  // method, but the visual studio 6 compiler has great problems
  // with templates and didn't accept two template methods in this
  // class.
  // draw triangles
  CNullDriver::drawVertexPrimitiveList(clippedVertices, clippedVertices_size,
      clippedIndices, clippedIndices_size / 3, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
  if (TransformedPoints_size < clippedVertices_size) {
    TransformedPoints.set_used(clippedVertices_size);
  }
  if (TransformedPoints.empty()) {
    return;
  }
  const VERTEXTYPE* currentVertex = clippedVertices;
  S2DVertex* tp = &TransformedPoints[0];
  ISIZE textureSize(0, 0);
  f32 zDiv;
  if (s->Texture) {
    textureSize = ((CSoftwareTexture*)s->Texture)->getTexture()->getDimension();
  }
  f32 transformedPos[4]; // transform all points in the list
  matrix4 matrix(TransformationMatrix[ETS_PROJECTION]);
  matrix *= TransformationMatrix[ETS_VIEW];
  matrix *= TransformationMatrix[ETS_WORLD];
  s32 ViewTransformWidth = (ViewPortSize.Width >> 1);
  s32 ViewTransformHeight = (ViewPortSize.Height >> 1);
  for (i = 0; i < (int)clippedVertices_size; ++i) {
    transformedPos[0] = currentVertex->Pos.X;
    transformedPos[1] = currentVertex->Pos.Y;
    transformedPos[2] = currentVertex->Pos.Z;
    transformedPos[3] = 1.0f;
    matrix.multiplyWith1x4Matrix(transformedPos);
    zDiv = transformedPos[3] == 0.0f ? 1.0f : (1.0f / transformedPos[3]);
    tp->Pos.X = (s32)(ViewTransformWidth * (transformedPos[0] * zDiv) + (Render2DTranslation.X));
    tp->Pos.Y = (Render2DTranslation.Y - (s32)(ViewTransformHeight * (transformedPos[1] * zDiv)));
    tp->Color = currentVertex->Color.toA1R5G5B5();
    tp->ZValue = (TZBufferType)(32767.0f * zDiv);
    tp->TCoords.X = (s32)(currentVertex->TCoords.X * textureSize.Width);
    tp->TCoords.X <<= 8;
    tp->TCoords.Y = (s32)(currentVertex->TCoords.Y * textureSize.Height);
    tp->TCoords.Y <<= 8;
    ++currentVertex;
    ++tp;
  }
  // draw all transformed points from the index list
  CurrentTriangleRenderer->drawIndexedTriangleList(&TransformedPoints[0],
      clippedVertices_size, clippedIndices, clippedIndices_size / 3);
}
//! Draws a 3d line.
void CSoftwareDriver::draw3DLine(const float3 start,
    const float3 end, SColor color)
{
  float3 vect = start.crossProduct(end);
  vect.normalize();
  vect *= s->Material.Thickness * 0.3f;
  S3DVertex vtx[4];
  vtx[0].Color = color;
  vtx[1].Color = color;
  vtx[2].Color = color;
  vtx[3].Color = color;
  vtx[0].Pos = start;
  vtx[1].Pos = end;
  vtx[2].Pos = start + vect;
  vtx[3].Pos = end + vect;
  u16 idx[12] = {0, 1, 2, 0, 2, 1, 0, 1, 3, 0, 3, 1};
  drawIndexedTriangleList(vtx, 4, idx, 4);
}
//! clips a triangle against the viewing frustum
void CSoftwareDriver::clipTriangle(f32* transformedPos)
{
}
//! Only used by the internal engine. Used to notify the driver that
//! the window was resized.
void CSoftwareDriver::OnResize(const ISIZE& size)
{
  // make sure width and height are multiples of 2
  ISIZE realSize(size);
  if (realSize.Width % 2) {
    realSize.Width += 1;
  }
  if (realSize.Height % 2) {
    realSize.Height += 1;
  }
  if (ScreenSize != realSize) {
    if (ViewPort.getWidth() == (s32)ScreenSize.Width &&
        ViewPort.getHeight() == (s32)ScreenSize.Height) {
      ViewPort = IRECT(position2di(0, 0),
          dimension2di(realSize));
    }
    ScreenSize = realSize;
    bool resetRT = (RenderTargetSurface == BackBuffer);
    if (BackBuffer) {
      BackBuffer->drop();
    }
    BackBuffer = new CImage(ECF_A1R5G5B5, realSize);
    if (resetRT) {
      setRenderTarget(BackBuffer);
    }
  }
}
//! returns the current render target size
const ISIZE& CSoftwareDriver::getCurrentRenderTargetSize() const
{
  return RenderTargetSize;
}
//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
void CSoftwareDriver::draw2DImage(const ITexture* texture, const position2di& destPos,
    const IRECT& sourceRect,
    const IRECT* clipRect, SColor color,
    bool useAlphaChannelOfTexture)
{
  if (texture) {
    if (texture->getDriverType() != EDT_SOFTWARE) {
      os::Printer::log("Fatal Error: Tried to copy from a surface not owned by this driver.", ELL_ERROR);
      return;
    }
    if (useAlphaChannelOfTexture)
      ((CSoftwareTexture*)texture)->getImage()->copyToWithAlpha(
          RenderTargetSurface, destPos, sourceRect, color, clipRect);
    else
      ((CSoftwareTexture*)texture)->getImage()->copyTo(
          RenderTargetSurface, destPos, sourceRect, clipRect);
  }
}
//! Draws a 2d line.
void CSoftwareDriver::draw2DLine(const position2di& start,
    const position2di& end,
    SColor color)
{
  drawLine(RenderTargetSurface, start, end, color);
}
//! Draws a pixel
void CSoftwareDriver::drawPixel(u32 x, u32 y, const SColor& color)
{
  BackBuffer->setPixel(x, y, color, true);
}
//! draw a 2d rectangle
void CSoftwareDriver::draw2DRectangle(SColor color, const IRECT& pos,
    const IRECT* clip)
{
  if (clip) {
    IRECT p(pos);
    p.clipAgainst(*clip);
    if (!p.isValid()) {
      return;
    }
    drawRectangle(RenderTargetSurface, p, color);
  }
  else {
    if (!pos.isValid()) {
      return;
    }
    drawRectangle(RenderTargetSurface, pos, color);
  }
}
//!Draws an 2d rectangle with a gradient.
void CSoftwareDriver::draw2DRectangle(const IRECT& pos,
    SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
    const IRECT* clip)
{
  // TODO: implement
  draw2DRectangle(colorLeftUp, pos, clip);
}
//! \return Returns the name of the video driver. Example: In case of the Direct3D8
//! driver, it would return "Direct3D8.1".
const char* CSoftwareDriver::getName() const
{
  return "Irrlicht Software Driver 1.0";
}
//! Returns type of video driver
E_DRIVER_TYPE CSoftwareDriver::getDriverType() const
{
  return EDT_SOFTWARE;
}
//! returns color format
ECOLOR_FORMAT CSoftwareDriver::getColorFormat() const
{
  if (BackBuffer) {
    return BackBuffer->getColorFormat();
  }
  else {
    return CNullDriver::getColorFormat();
  }
}
//! Returns the transformation set by setTransform
const matrix4& CSoftwareDriver::getTransform(E_TRANSFORMATION_STATE state) const
{
  return TransformationMatrix[state];
}
//! Creates a render target texture.
ITexture* CSoftwareDriver::addRenderTargetTexture(const ISIZE& size,
    const char* name,
    const ECOLOR_FORMAT format)
{
  IImage* img = createImage(ECF_A1R5G5B5, size);
  ITexture* tex = new CSoftwareTexture(img, name, true);
  img->drop();
  addTexture(tex);
  tex->drop();
  return tex;
}
//! Returns an image created from the last rendered frame.
IImage* CSoftwareDriver::createScreenShot(ECOLOR_FORMAT format, E_RENDER_TARGET target)
{
  if (target != ERT_FRAME_BUFFER) {
    return 0;
  }
  if (BackBuffer) {
    IImage* tmp = createImage(BackBuffer->getColorFormat(), BackBuffer->getDimension());
    BackBuffer->copyTo(tmp);
    return tmp;
  }
  else {
    return 0;
  }
}
//! Returns the maximum amount of primitives (mostly vertices) which
//! the device is able to render with one drawIndexedTriangleList
//! call.
u32 CSoftwareDriver::getMaximalPrimitiveCount() const
{
  return 0x00800000;
}


#endif
