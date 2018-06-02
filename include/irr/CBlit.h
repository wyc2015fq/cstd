
//#include "SoftwareDriver2_helper.h"
typedef struct SBlitJob SBlitJob;
struct SBlitJob {
  IRECT Dest[1];
  IRECT Source[1];
  u32 argb;
  void* src;
  void* dst;
  s32 width;
  s32 height;
  u32 srcPitch;
  u32 dstPitch;
  u32 srcPixelMul;
  u32 dstPixelMul;
  bool stretch;
  float x_stretch;
  float y_stretch;
};
// Bitfields Cohen Sutherland
enum eClipCode {
  CLIPCODE_EMPTY  = 0,
  CLIPCODE_BOTTOM = 1,
  CLIPCODE_TOP  = 2,
  CLIPCODE_LEFT = 4,
  CLIPCODE_RIGHT  = 8
};
CV_INLINE u32 GetClipCode(const IRECT* r, IPOINT p)
{
  u32 code = CLIPCODE_EMPTY;
  if (p.x < r->l) {
    code = CLIPCODE_LEFT;
  }
  else if (p.x > r->r) {
    code = CLIPCODE_RIGHT;
  }
  if (p.y < r->t) {
    code |= CLIPCODE_TOP;
  }
  else if (p.y > r->b) {
    code |= CLIPCODE_BOTTOM;
  }
  return code;
}
/*!
  Cohen Sutherland clipping
  @return: 1 if valid
*/
static int ClipLine(const IRECT* clipping, IPOINT* p0_out, IPOINT* p1_out, IPOINT p0, IPOINT p1)
{
  u32 code0;
  u32 code1;
  u32 code;
  code0 = GetClipCode(clipping, p0);
  code1 = GetClipCode(clipping, p1);
  // trivial accepted
  while (code0 | code1) {
    s32 x = 0;
    s32 y = 0;
    // trivial reject
    if (code0 & code1) {
      return 0;
    }
    if (code0) {
      // clip first point
      code = code0;
    }
    else {
      // clip last point
      code = code1;
    }
    if ((code & CLIPCODE_BOTTOM) == CLIPCODE_BOTTOM) {
      // clip bottom viewport
      y = clipping->b;
      x = p0.x + (p1.x - p0.x) * (y - p0.y) / (p1.y - p0.y);
    }
    else if ((code & CLIPCODE_TOP) == CLIPCODE_TOP) {
      // clip to viewport
      y = clipping->t;
      x = p0.x + (p1.x - p0.x) * (y - p0.y) / (p1.y - p0.y);
    }
    else if ((code & CLIPCODE_RIGHT) == CLIPCODE_RIGHT) {
      // clip right viewport
      x = clipping->r;
      y = p0.y + (p1.y - p0.y) * (x - p0.x) / (p1.x - p0.x);
    }
    else if ((code & CLIPCODE_LEFT) == CLIPCODE_LEFT) {
      // clip left viewport
      x = clipping->l;
      y = p0.y + (p1.y - p0.y) * (x - p0.x) / (p1.x - p0.x);
    }
    if (code == code0) {
      // modify first point
      p0.x = x;
      p0.y = y;
      code0 = GetClipCode(clipping, p0);
    }
    else {
      // modify second point
      p1.x = x;
      p1.y = y;
      code1 = GetClipCode(clipping, p1);
    }
  }
  *p0_out = p0;
  *p0_out = p1;
  return 1;
}
/*
*/
CV_INLINE void GetClip(IRECT* clipping, IImage* t)
{
  clipping->l = 0;
  clipping->t = 0;
  clipping->r = t->Size.w - 1;
  clipping->b = t->Size.h - 1;
}
/*
  return alpha in [0;256] Granularity from 32-Bit ARGB
  add highbit alpha ( alpha > 127 ? + 1 )
*/
static CV_INLINE u32 extractAlpha(const u32 c)
{
  return (c >> 24) + (c >> 31);
}
/*
  return alpha in [0;255] Granularity and 32-Bit ARGB
  add highbit alpha ( alpha > 127 ? + 1 )
*/
static CV_INLINE u32 packAlpha(const u32 c)
{
  return (c > 127 ? c - 1 : c) << 24;
}
/*!
  Scale Color by (1/value)
  value 0 - 256 ( alpha )
*/
CV_INLINE u32 PixelLerp32(const u32 source, const u32 value)
{
  u32 srcRB = source & 0x00FF00FF;
  u32 srcXG = (source & 0xFF00FF00) >> 8;
  srcRB *= value;
  srcXG *= value;
  srcRB >>= 8;
  //srcXG >>= 8;
  srcXG &= 0xFF00FF00;
  srcRB &= 0x00FF00FF;
  return srcRB | srcXG;
}
/*
*/
static void RenderLine32_Decal(IImage* t, IPOINT p0, IPOINT p1, u32 argb)
{
  s32 dx = p1.x - p0.x;
  s32 dy = p1.y - p0.y;
  s32 c;
  s32 m;
  s32 d = 0;
  s32 run;
  s32 xInc = 4;
  s32 yInc = (s32) t->Pitch;
  u32* dst;
  if (dx < 0) {
    xInc = -xInc;
    dx = -dx;
  }
  if (dy < 0) {
    yInc = -yInc;
    dy = -dy;
  }
  dst = (u32*)((u8*) t->Data + (p0.y * t->Pitch) + (p0.x << 2));
  if (dy > dx) {
    s32 tmp;
    tmp = dx;
    dx = dy;
    dy = tmp;
    tmp = xInc;
    xInc = yInc;
    yInc = tmp;
  }
  c = dx << 1;
  m = dy << 1;
  run = dx;
  do {
    *dst = argb;
    dst = (u32*)((u8*) dst + xInc);    // x += xInc
    d += m;
    if (d > dx) {
      dst = (u32*)((u8*) dst + yInc);    // y += yInc
      d -= c;
    }
    run -= 1;
  }
  while (run >= 0);
}
/*
*/
static void RenderLine32_Blend(IImage* t,
    IPOINT p0,
    IPOINT p1,
    u32 argb, u32 alpha)
{
  s32 dx = p1.x - p0.x;
  s32 dy = p1.y - p0.y;
  s32 c;
  s32 m;
  s32 d = 0;
  s32 run;
  s32 xInc = 4;
  s32 yInc = (s32) t->Pitch;
  u32* dst;
  u32 packA;
  if (dx < 0) {
    xInc = -xInc;
    dx = -dx;
  }
  if (dy < 0) {
    yInc = -yInc;
    dy = -dy;
  }
  dst = (u32*)((u8*) t->Data + (p0.y * t->Pitch) + (p0.x << 2));
  if (dy > dx) {
    s32 tmp;
    tmp = dx;
    dx = dy;
    dy = tmp;
    tmp = xInc;
    xInc = yInc;
    yInc = tmp;
  }
  c = dx << 1;
  m = dy << 1;
  run = dx;
  packA = packAlpha(alpha);
  do {
    *dst = packA | PixelBlend32(*dst, argb, alpha);
    dst = (u32*)((u8*) dst + xInc);    // x += xInc
    d += m;
    if (d > dx) {
      dst = (u32*)((u8*) dst + yInc);    // y += yInc
      d -= c;
    }
    run -= 1;
  }
  while (run >= 0);
}
/*
*/
static void RenderLine16_Decal(IImage* t, IPOINT p0, IPOINT p1, u32 argb)
{
  s32 dx = p1.x - p0.x;
  s32 dy = p1.y - p0.y;
  s32 c;
  s32 m;
  s32 d = 0;
  s32 run;
  s32 xInc = 2;
  s32 yInc = (s32) t->Pitch;
  u16* dst;
  if (dx < 0) {
    xInc = -xInc;
    dx = -dx;
  }
  if (dy < 0) {
    yInc = -yInc;
    dy = -dy;
  }
  dst = (u16*)((u8*) t->Data + (p0.y * t->Pitch) + (p0.x << 1));
  if (dy > dx) {
    s32 tmp;
    tmp = dx;
    dx = dy;
    dy = tmp;
    tmp = xInc;
    xInc = yInc;
    yInc = tmp;
  }
  c = dx << 1;
  m = dy << 1;
  run = dx;
  do {
    *dst = (u16)argb;
    dst = (u16*)((u8*) dst + xInc);    // x += xInc
    d += m;
    if (d > dx) {
      dst = (u16*)((u8*) dst + yInc);    // y += yInc
      d -= c;
    }
    run -= 1;
  }
  while (run >= 0);
}
/*
*/
static void RenderLine16_Blend(IImage* t, IPOINT p0, IPOINT p1, u16 argb, u16 alpha)
{
  s32 dx = p1.x - p0.x;
  s32 dy = p1.y - p0.y;
  s32 c;
  s32 m;
  s32 d = 0;
  s32 run;
  s32 xInc = 2;
  s32 yInc = (s32) t->Pitch;
  u16* dst;
  u16 packA;
  if (dx < 0) {
    xInc = -xInc;
    dx = -dx;
  }
  if (dy < 0) {
    yInc = -yInc;
    dy = -dy;
  }
  dst = (u16*)((u8*) t->Data + (p0.y * t->Pitch) + (p0.x << 1));
  if (dy > dx) {
    s32 tmp;
    tmp = dx;
    dx = dy;
    dy = tmp;
    tmp = xInc;
    xInc = yInc;
    yInc = tmp;
  }
  c = dx << 1;
  m = dy << 1;
  run = dx;
  packA = alpha ? 0x8000 : 0;
  do {
    *dst = packA | PixelBlend16(*dst, argb, alpha);
    dst = (u16*)((u8*) dst + xInc);    // x += xInc
    d += m;
    if (d > dx) {
      dst = (u16*)((u8*) dst + yInc);    // y += yInc
      d -= c;
    }
    run -= 1;
  }
  while (run >= 0);
}
/*!
*/
static void executeBlit_TextureCopy_x_to_x(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  int dy, dx;
  if (job->stretch) {
    const u32* src = (const u32*)(job->src);
    u32* dst = (u32*)(job->dst);
    const float wscale = 1.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (u32*)((u8*)(job->src) + job->srcPitch * src_y);
      for (dx = 0; dx < w; ++dx) {
        const u32 src_x = (u32)(dx * wscale);
        dst[dx] = src[src_x];
      }
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
  else {
    const u32 widthPitch = job->width * job->dstPixelMul;
    const void* src = (void*) job->src;
    void* dst = (void*) job->dst;
    for (dy = 0; dy != h; ++dy) {
      memcpy(dst, src, widthPitch);
      src = (void*)((u8*)(src) + job->srcPitch);
      dst = (void*)((u8*)(dst) + job->dstPitch);
    }
  }
}
/*!
*/
static void executeBlit_TextureCopy_32_to_16(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  const u32* src = (const u32*)(job->src);
  u16* dst = (u16*)(job->dst);
  int dx, dy;
  if (job->stretch) {
    const float wscale = 1.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (u32*)((u8*)(job->src) + job->srcPitch * src_y);
      for (dx = 0; dx < w; ++dx) {
        const u32 src_x = (u32)(dx * wscale);
        //16 bit Blitter depends on pre-multiplied color
        const u32 s = PixelLerp32(src[src_x] | 0xFF000000, extractAlpha(src[src_x]));
        dst[dx] = A8R8G8B8toA1R5G5B5(s);
      }
      dst = (u16*)((u8*)(dst) + job->dstPitch);
    }
  }
  else {
    for (dy = 0; dy != h; ++dy) {
      for (dx = 0; dx != w; ++dx) {
        //16 bit Blitter depends on pre-multiplied color
        const u32 s = PixelLerp32(src[dx] | 0xFF000000, extractAlpha(src[dx]));
        dst[dx] = A8R8G8B8toA1R5G5B5(s);
      }
      src = (u32*)((u8*)(src) + job->srcPitch);
      dst = (u16*)((u8*)(dst) + job->dstPitch);
    }
  }
}
/*!
*/
static void executeBlit_TextureCopy_24_to_16(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  const u8* src = (const u8*)(job->src);
  u16* dst = (u16*)(job->dst);
  int dx, dy;
  if (job->stretch) {
    const float wscale = 3.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (u8*)(job->src) + job->srcPitch * src_y;
      for (dx = 0; dx < w; ++dx) {
        const u8* src_x = src + (u32)(dx * wscale);
        dst[dx] = RGB16(src_x[0], src_x[1], src_x[2]);
      }
      dst = (u16*)((u8*)(dst) + job->dstPitch);
    }
  }
  else {
    for (dy = 0; dy != h; ++dy) {
      const u8* s = src;
      for (dx = 0; dx != w; ++dx) {
        dst[dx] = RGB16(s[0], s[1], s[2]);
        s += 3;
      }
      src = src + job->srcPitch;
      dst = (u16*)((u8*)(dst) + job->dstPitch);
    }
  }
}
/*!
*/
static void executeBlit_TextureCopy_16_to_32(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  const u16* src = (const u16*)(job->src);
  u32* dst = (u32*)(job->dst);
  int dx, dy;
  if (job->stretch) {
    const float wscale = 1.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (u16*)((u8*)(job->src) + job->srcPitch * src_y);
      for (dx = 0; dx < w; ++dx) {
        const u32 src_x = (u32)(dx * wscale);
        dst[dx] = A1R5G5B5toA8R8G8B8(src[src_x]);
      }
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
  else {
    for (dy = 0; dy != h; ++dy) {
      for (dx = 0; dx != w; ++dx) {
        dst[dx] = A1R5G5B5toA8R8G8B8(src[dx]);
      }
      src = (u16*)((u8*)(src) + job->srcPitch);
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
}
static void executeBlit_TextureCopy_16_to_24(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  const u16* src = (const u16*)(job->src);
  u8* dst = (u8*)(job->dst);
  int dx, dy;
  if (job->stretch) {
    const float wscale = 1.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (u16*)((u8*)(job->src) + job->srcPitch * src_y);
      for (dx = 0; dx < w; ++dx) {
        const u32 src_x = (u32)(dx * wscale);
        u32 color = A1R5G5B5toA8R8G8B8(src[src_x]);
        u8* writeTo = &dst[dx * 3];
        *writeTo++ = (color >> 16) & 0xFF;
        *writeTo++ = (color >> 8) & 0xFF;
        *writeTo++ = color & 0xFF;
      }
      dst += job->dstPitch;
    }
  }
  else {
    for (dy = 0; dy != h; ++dy) {
      for (dx = 0; dx != w; ++dx) {
        u32 color = A1R5G5B5toA8R8G8B8(src[dx]);
        u8* writeTo = &dst[dx * 3];
        *writeTo++ = (color >> 16) & 0xFF;
        *writeTo++ = (color >> 8) & 0xFF;
        *writeTo++ = color & 0xFF;
      }
      src = (u16*)((u8*)(src) + job->srcPitch);
      dst += job->dstPitch;
    }
  }
}
/*!
*/
static void executeBlit_TextureCopy_24_to_32(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  const u8* src = (const u8*)(job->src);
  u32* dst = (u32*)(job->dst);
  int dx, dy;
  if (job->stretch) {
    const float wscale = 3.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (const u8*)job->src + (job->srcPitch * src_y);
      for (dx = 0; dx < w; ++dx) {
        const u8* s = src + (u32)(dx * wscale);
        dst[dx] = 0xFF000000 | s[0] << 16 | s[1] << 8 | s[2];
      }
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
  else {
    for (dy = 0; dy != job->height; ++dy) {
      const u8* s = src;
      for (dx = 0; dx != job->width; ++dx) {
        dst[dx] = 0xFF000000 | s[0] << 16 | s[1] << 8 | s[2];
        s += 3;
      }
      src = src + job->srcPitch;
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
}
static void executeBlit_TextureCopy_32_to_24(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  const u32* src = (const u32*)(job->src);
  u8* dst = (u8*)(job->dst);
  int dx, dy;
  if (job->stretch) {
    const float wscale = 1.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (u32*)((u8*)(job->src) + job->srcPitch * src_y);
      for (dx = 0; dx < w; ++dx) {
        const u32 src_x = src[(u32)(dx * wscale)];
        u8* writeTo = &dst[dx * 3];
        *writeTo++ = (src_x >> 16) & 0xFF;
        *writeTo++ = (src_x >> 8) & 0xFF;
        *writeTo++ = src_x & 0xFF;
      }
      dst += job->dstPitch;
    }
  }
  else {
    for (dy = 0; dy != h; ++dy) {
      for (dx = 0; dx != w; ++dx) {
        u8* writeTo = &dst[dx * 3];
        *writeTo++ = (src[dx] >> 16) & 0xFF;
        *writeTo++ = (src[dx] >> 8) & 0xFF;
        *writeTo++ = src[dx] & 0xFF;
      }
      src = (u32*)((u8*)(src) + job->srcPitch);
      dst += job->dstPitch;
    }
  }
}
/*!
*/
static void executeBlit_TextureBlend_16_to_16(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  const u32 rdx = w >> 1;
  const u32* src = (u32*) job->src;
  u32* dst = (u32*) job->dst;
  int dx, dy;
  if (job->stretch) {
    const float wscale = 1.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    const u32 off = if_c_a_else_b(w & 1, (u32)((w - 1) * wscale), 0);
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (u32*)((u8*)(job->src) + job->srcPitch * src_y);
      for (dx = 0; dx < rdx; ++dx) {
        const u32 src_x = (u32)(dx * wscale);
        dst[dx] = PixelBlend16_simd(dst[dx], src[src_x]);
      }
      if (off) {
        ((u16*) dst)[off] = PixelBlend16_1(((u16*) dst)[off], ((u16*) src)[off]);
      }
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
  else {
    const u32 off = if_c_a_else_b(w & 1, w - 1, 0);
    for (dy = 0; dy != h; ++dy) {
      for (dx = 0; dx != rdx; ++dx) {
        dst[dx] = PixelBlend16_simd(dst[dx], src[dx]);
      }
      if (off) {
        ((u16*) dst)[off] = PixelBlend16_1(((u16*) dst)[off], ((u16*) src)[off]);
      }
      src = (u32*)((u8*)(src) + job->srcPitch);
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
}
/*!
*/
static void executeBlit_TextureBlend_32_to_32(const SBlitJob* job)
{
  const u32 w = job->width;
  const u32 h = job->height;
  const u32* src = (u32*) job->src;
  u32* dst = (u32*) job->dst;
  int dx, dy;
  if (job->stretch) {
    const float wscale = 1.f / job->x_stretch;
    const float hscale = 1.f / job->y_stretch;
    for (dy = 0; dy < h; ++dy) {
      const u32 src_y = (u32)(dy * hscale);
      src = (u32*)((u8*)(job->src) + job->srcPitch * src_y);
      for (dx = 0; dx < w; ++dx) {
        const u32 src_x = (u32)(dx * wscale);
        dst[dx] = PixelBlend32_1(dst[dx], src[src_x]);
      }
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
  else {
    for (dy = 0; dy != h; ++dy) {
      for (dx = 0; dx != w; ++dx) {
        dst[dx] = PixelBlend32_1(dst[dx], src[dx]);
      }
      src = (u32*)((u8*)(src) + job->srcPitch);
      dst = (u32*)((u8*)(dst) + job->dstPitch);
    }
  }
}
/*!
*/
static void executeBlit_TextureBlendColor_16_to_16(const SBlitJob* job)
{
  u16* src = (u16*) job->src;
  u16* dst = (u16*) job->dst;
  u16 blend = A8R8G8B8toA1R5G5B5(job->argb);
  int dx, dy;
  for (dy = 0; dy != job->height; ++dy) {
    for (dx = 0; dx != job->width; ++dx) {
      if (0 == (src[dx] & 0x8000)) {
        continue;
      }
      dst[dx] = PixelMul16_2(src[dx], blend);
    }
    src = (u16*)((u8*)(src) + job->srcPitch);
    dst = (u16*)((u8*)(dst) + job->dstPitch);
  }
}
/*!
*/
static void executeBlit_TextureBlendColor_32_to_32(const SBlitJob* job)
{
  u32* src = (u32*) job->src;
  u32* dst = (u32*) job->dst;
  int dx, dy;
  for (dy = 0; dy != job->height; ++dy) {
    for (dx = 0; dx != job->width; ++dx) {
      dst[dx] = PixelBlend32_1(dst[dx], PixelMul32_2(src[dx], job->argb));
    }
    src = (u32*)((u8*)(src) + job->srcPitch);
    dst = (u32*)((u8*)(dst) + job->dstPitch);
  }
}
/*!
*/
static void executeBlit_Color_16_to_16(const SBlitJob* job)
{
  const u16 c = A8R8G8B8toA1R5G5B5(job->argb);
  u16* dst = (u16*) job->dst;
  int dy;
  for (dy = 0; dy != job->height; ++dy) {
    memset16(dst, c, job->srcPitch);
    dst = (u16*)((u8*)(dst) + job->dstPitch);
  }
}
/*!
*/
static void executeBlit_Color_32_to_32(const SBlitJob* job)
{
  u32* dst = (u32*) job->dst;
  int dy;
  for (dy = 0; dy != job->height; ++dy) {
    memset32(dst, job->argb, job->srcPitch);
    dst = (u32*)((u8*)(dst) + job->dstPitch);
  }
}
/*!
*/
static void executeBlit_ColorAlpha_16_to_16(const SBlitJob* job)
{
  u16* dst = (u16*) job->dst;
  const u16 alpha = extractAlpha(job->argb) >> 3;
  int dx, dy;
  u32 src;
  if (0 == alpha) {
    return;
  }
  src = A8R8G8B8toA1R5G5B5(job->argb);
  for (dy = 0; dy != job->height; ++dy) {
    for (dx = 0; dx != job->width; ++dx) {
      dst[dx] = 0x8000 | PixelBlend16(dst[dx], src, alpha);
    }
    dst = (u16*)((u8*)(dst) + job->dstPitch);
  }
}
/*!
*/
static void executeBlit_ColorAlpha_32_to_32(const SBlitJob* job)
{
  u32* dst = (u32*) job->dst;
  const u32 alpha = extractAlpha(job->argb);
  const u32 src = job->argb;
  int dx, dy;
  for (dy = 0; dy != job->height; ++dy) {
    for (dx = 0; dx != job->width; ++dx) {
      dst[dx] = (job->argb & 0xFF000000) | PixelBlend32(dst[dx], src, alpha);
    }
    dst = (u32*)((u8*)(dst) + job->dstPitch);
  }
}
// Blitter Operation
typedef enum eBlitter {
  BLITTER_INVALID = 0,
  BLITTER_COLOR,
  BLITTER_COLOR_ALPHA,
  BLITTER_TEXTURE,
  BLITTER_TEXTURE_ALPHA_BLEND,
  BLITTER_TEXTURE_ALPHA_COLOR_BLEND
} eBlitter;
typedef void (*tExecuteBlit)(const SBlitJob* job);
/*!
*/
typedef struct blitterTable {
  eBlitter operation;
  s32 destFormat;
  s32 sourceFormat;
  tExecuteBlit func;
} blitterTable;
static const blitterTable blitTable[] = {
  { BLITTER_TEXTURE, -2, -2, executeBlit_TextureCopy_x_to_x },
  { BLITTER_TEXTURE, ECF_A1R5G5B5, ECF_A8R8G8B8, executeBlit_TextureCopy_32_to_16 },
  { BLITTER_TEXTURE, ECF_A1R5G5B5, ECF_R8G8B8, executeBlit_TextureCopy_24_to_16 },
  { BLITTER_TEXTURE, ECF_A8R8G8B8, ECF_A1R5G5B5, executeBlit_TextureCopy_16_to_32 },
  { BLITTER_TEXTURE, ECF_A8R8G8B8, ECF_R8G8B8, executeBlit_TextureCopy_24_to_32 },
  { BLITTER_TEXTURE, ECF_R8G8B8, ECF_A1R5G5B5, executeBlit_TextureCopy_16_to_24 },
  { BLITTER_TEXTURE, ECF_R8G8B8, ECF_A8R8G8B8, executeBlit_TextureCopy_32_to_24 },
  { BLITTER_TEXTURE_ALPHA_BLEND, ECF_A1R5G5B5, ECF_A1R5G5B5, executeBlit_TextureBlend_16_to_16 },
  { BLITTER_TEXTURE_ALPHA_BLEND, ECF_A8R8G8B8, ECF_A8R8G8B8, executeBlit_TextureBlend_32_to_32 },
  { BLITTER_TEXTURE_ALPHA_COLOR_BLEND, ECF_A1R5G5B5, ECF_A1R5G5B5, executeBlit_TextureBlendColor_16_to_16 },
  { BLITTER_TEXTURE_ALPHA_COLOR_BLEND, ECF_A8R8G8B8, ECF_A8R8G8B8, executeBlit_TextureBlendColor_32_to_32 },
  { BLITTER_COLOR, ECF_A1R5G5B5, -1, executeBlit_Color_16_to_16 },
  { BLITTER_COLOR, ECF_A8R8G8B8, -1, executeBlit_Color_32_to_32 },
  { BLITTER_COLOR_ALPHA, ECF_A1R5G5B5, -1, executeBlit_ColorAlpha_16_to_16 },
  { BLITTER_COLOR_ALPHA, ECF_A8R8G8B8, -1, executeBlit_ColorAlpha_32_to_32 },
  { BLITTER_INVALID, -1, -1, 0 }
};
static CV_INLINE tExecuteBlit getBlitter2(eBlitter operation, const IImage* dest, const IImage* source)
{
  ECOLOR_FORMAT sourceFormat = (ECOLOR_FORMAT)(source ? source->Format : -1);
  ECOLOR_FORMAT destFormat = (ECOLOR_FORMAT)(dest ? dest->Format : -1);
  const blitterTable* b = blitTable;
  while (b->operation != BLITTER_INVALID) {
    if (b->operation == operation) {
      if ((b->destFormat == -1 || b->destFormat == destFormat) &&
          (b->sourceFormat == -1 || b->sourceFormat == sourceFormat)) {
        return b->func;
      }
      else if (b->destFormat == -2 && (sourceFormat == destFormat)) {
        return b->func;
      }
    }
    b += 1;
  }
  return 0;
}
// bounce clipping to texture
CV_INLINE void setClip(IRECT* out, const IRECT* clip, const IImage* tex, s32 passnative)
{
  if (clip && 0 == tex && passnative) {
    *out = *clip;
    return;
  } else {
    const s32 w = tex ? tex->Size.w : 0;
    const s32 h = tex ? tex->Size.h : 0;
    if (clip) {
      out->l = s32_clamp(clip->l, 0, w);
      out->r = s32_clamp(clip->r, out->l, w);
      out->t = s32_clamp(clip->t, 0, h);
      out->b = s32_clamp(clip->b, out->t, h);
    }
    else {
      out->l = 0;
      out->t = 0;
      out->r = w;
      out->b = h;
    }
  }
}
/*!
  a generic 2D Blitter
*/
static s32 Blit(eBlitter operation,IImage* dest,const IRECT* destClipping,const IPOINT* destPos,
                IImage* const source,const IRECT* sourceClipping,u32 argb)
{
  IRECT sourceClip[1], destClip[1], v[1];
  SBlitJob job;
  tExecuteBlit blitter = getBlitter2(operation, dest, source);
  if (0 == blitter) {
    return 0;
  }
  // Clipping
  setClip(sourceClip, sourceClipping, source, 1);
  setClip(destClip, destClipping, dest, 0);
  v->l = destPos ? destPos->x : 0;
  v->t = destPos ? destPos->y : 0;
  v->r = v->l + (sourceClip->r - sourceClip->l);
  v->b = v->t + (sourceClip->b - sourceClip->t);
  if (!iRect_intersect(job.Dest, destClip, v)) {
    return 0;
  }
  job.width = job.Dest->r - job.Dest->l;
  job.height = job.Dest->b - job.Dest->t;
  job.Source->l = sourceClip->l + (job.Dest->l - v->l);
  job.Source->r = job.Source->l + job.width;
  job.Source->t = sourceClip->t + (job.Dest->t - v->t);
  job.Source->b = job.Source->t + job.height;
  job.argb = argb;
  if (source) {
    job.srcPitch = source->Pitch;
    job.srcPixelMul = source->BytesPerPixel;
    job.src = (void*)((u8*) source->Data + (job.Source->t * job.srcPitch) + (job.Source->l * job.srcPixelMul));
  }
  else {
    // use srcPitch for color operation on dest
    job.srcPitch = job.width * dest->BytesPerPixel;
  }
  job.dstPitch = dest->Pitch;
  job.dstPixelMul = dest->BytesPerPixel;
  job.dst = (void*)((u8*) dest->Data + (job.Dest->t * job.dstPitch) + (job.Dest->l * job.dstPixelMul));
  blitter(&job);
  return 1;
}
static s32 StretchBlit(eBlitter operation, IImage* dest, const IRECT* destRect,
    const IRECT* srcRect, IImage* const source, u32 argb)
{
  SBlitJob job;
  tExecuteBlit blitter = getBlitter2(operation, dest, source);
  if (0 == blitter) {
    return 0;
  }
  // Clipping
  setClip(job.Source, srcRect, source, 1);
  setClip(job.Dest, destRect, dest, 0);
  job.width = job.Dest->r - job.Dest->l;
  job.height = job.Dest->b - job.Dest->t;
  job.argb = argb;
  // use original dest size, despite any clipping
  job.x_stretch = (float)RCW(destRect) / (float)(job.Source->r - job.Source->l);
  job.y_stretch = (float)RCH(destRect) / (float)(job.Source->b - job.Source->t);
  job.stretch = (job.x_stretch != 1.f) || (job.y_stretch != 1.f);
  if (source) {
    job.srcPitch = source->Pitch;
    job.srcPixelMul = source->BytesPerPixel;
    job.src = (void*)((u8*) source->Data + (job.Source->t * job.srcPitch) + (job.Source->l * job.srcPixelMul));
  }
  else {
    // use srcPitch for color operation on dest
    job.srcPitch = job.width * dest->BytesPerPixel;
  }
  job.dstPitch = dest->Pitch;
  job.dstPixelMul = dest->BytesPerPixel;
  job.dst = (void*)((u8*) dest->Data + (job.Dest->t * job.dstPitch) + (job.Dest->l * job.dstPixelMul));
  blitter(&job);
  return 1;
}
// Methods for Software drivers
//! draws a rectangle
static void drawRectangle(IImage* img, const IRECT* rect, SColor color)
{
  Blit(getA(color) == 0xFF ? BLITTER_COLOR : BLITTER_COLOR_ALPHA,
      img, 0, (IPOINT*)(&rect->l), 0, rect, color);
}
//! draws a line from to with color
static void drawLine(IImage* img, IPOINT from, IPOINT to, SColor color)
{
  IRECT clip[1];
  IPOINT p[2];
  GetClip(clip, img);
  if (ClipLine(clip, p, p+1, from, to)) {
    u32 alpha = extractAlpha(color);
    switch (img->Format) {
    case ECF_A1R5G5B5:
      if (alpha == 256) {
        RenderLine16_Decal(img, p[0], p[1], A8R8G8B8toA1R5G5B5(color));
      }
      else {
        RenderLine16_Blend(img, p[0], p[1], A8R8G8B8toA1R5G5B5(color), alpha >> 3);
      }
      break;
    case ECF_A8R8G8B8:
      if (alpha == 256) {
        RenderLine32_Decal(img, p[0], p[1], color);
      }
      else {
        RenderLine32_Blend(img, p[0], p[1], color, alpha);
      }
      break;
    default:
      break;
    }
  }
}

