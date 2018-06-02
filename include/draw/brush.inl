
///////////////////////////////////////////////////
///////////////////////////////////////////////////
#define PIXBLENDA3_4(dst, _0, _1, _2, _a) (PIXBLEND(dst[0], _0, _a), PIXBLEND(dst[1], _1, _a), PIXBLEND(dst[2], _2, _a))
#define PIXBLENDA4_4(dst, _0, _1, _2, _a) (PIXBLEND(dst[0], _0, _a), PIXBLEND(dst[1], _1, _a), PIXBLEND(dst[2], _2, _a), dst[3] = 255)
#define PIXBLENDA3(dst, _a) (PIXBLEND(dst[0], pclr[0], _a), PIXBLEND(dst[1], pclr[1], _a), PIXBLEND(dst[2], pclr[2], _a))
#define PIXBLENDA4(dst, _a) (PIXBLEND(dst[0], pclr[0], _a), PIXBLEND(dst[1], pclr[1], _a), PIXBLEND(dst[2], pclr[2], _a), dst[3] = 255)

///////////////////////////////////////////////////

#ifndef _BLEND_INL_
#define _BLEND_INL_
#define alpha_cover(a, cover)  ((a) * ((cover) + 1)) >> 8
enum {Order_R = 0, Order_G = 1, Order_B = 2, Order_A = 3,
base_shift = 8, base_mask = (1<<base_shift)-1,
base_shift16=16, base_mask16 = (1<<base_shift16)-1};

typedef unsigned calc_type;
typedef unsigned long_type;
typedef ushort pixel_type16;
typedef uint pixel_type32;

//apply_gamma_dir_rgb
CC_INLINE void apply_gamma_dir_rgb(uchar* p, unsigned width, const uchar* m_gamma_dir)
{
  for ( ;width--; p+=3) {
    p[Order_R] = m_gamma_dir[p[Order_R]];
    p[Order_G] = m_gamma_dir[p[Order_G]];
    p[Order_B] = m_gamma_dir[p[Order_B]];
  }
}
//apply_gamma_inv_rgb
CC_INLINE void apply_gamma_inv_rgb(uchar* p, unsigned width, const uchar* m_gamma_inv)
{
  for ( ;width--; p+=3) {
    p[Order_R] = m_gamma_inv[p[Order_R]];
    p[Order_G] = m_gamma_inv[p[Order_G]];
    p[Order_B] = m_gamma_inv[p[Order_B]];
  }
}

//blender_rgb
CC_INLINE void blender_rgb(uchar* p, const uchar* s, unsigned width, const uchar* covers)
{
  if (covers) {
    for ( ;width--; p+=3, s+=4) {
      unsigned cover = *covers++;
      unsigned alpha = alpha_cover(s[Order_A], cover);
      uchar r = p[Order_R];
      uchar g = p[Order_G];
      uchar b = p[Order_B];
      
      p[Order_R] = (uchar)(((s[Order_R] - r) * alpha + (r << base_shift)) >> base_shift);
      p[Order_G] = (uchar)(((s[Order_G] - g) * alpha + (g << base_shift)) >> base_shift);
      p[Order_B] = (uchar)(((s[Order_B] - b) * alpha + (b << base_shift)) >> base_shift);
    }
  } else {
    for ( ;width--; p+=3, s+=4) {
      unsigned alpha = s[Order_A];
      p[Order_R] += (uchar)(((s[Order_R] - p[Order_R]) * alpha) >> base_shift);
      p[Order_G] += (uchar)(((s[Order_G] - p[Order_G]) * alpha) >> base_shift);
      p[Order_B] += (uchar)(((s[Order_B] - p[Order_B]) * alpha) >> base_shift);
    }
  }
}

//blender_rgb_pre
CC_INLINE void blender_rgb_pre(uchar* p, const uchar* s, unsigned width, const uchar* covers)
{
  if (covers) {
    for ( ;width--; p+=3, s+=4) {
      unsigned alpha = base_mask - s[Order_A];
      unsigned cover = *covers++;
      cover = (cover + 1) << (base_shift - 8);
      p[Order_R] = (uchar)((p[Order_R] * alpha + s[Order_R] * cover) >> base_shift);
      p[Order_G] = (uchar)((p[Order_G] * alpha + s[Order_G] * cover) >> base_shift);
      p[Order_B] = (uchar)((p[Order_B] * alpha + s[Order_B] * cover) >> base_shift);
    }
  } else {
    for ( ;width--; p+=3, s+=4) {
      unsigned alpha = base_mask - s[Order_A];
      p[Order_R] = (uchar)(((p[Order_R] * alpha) >> base_shift) + s[Order_R]);
      p[Order_G] = (uchar)(((p[Order_G] * alpha) >> base_shift) + s[Order_G]);
      p[Order_B] = (uchar)(((p[Order_B] * alpha) >> base_shift) + s[Order_B]);
    }
  }
}

//blender_rgb_gamma
CC_INLINE void blender_rgb_gamma(uchar* p, const uchar* s, unsigned width, const uchar* m_gamma_dir, const uchar* m_gamma_inv)
{
  for ( ;width--; p+=3, s+=4) {
    unsigned alpha = s[Order_A];
    calc_type r = m_gamma_dir[p[Order_R]];
    calc_type g = m_gamma_dir[p[Order_G]];
    calc_type b = m_gamma_dir[p[Order_B]];
    p[Order_R] = m_gamma_inv[(((m_gamma_dir[s[Order_R]] - r) * alpha) >> base_shift) + r];
    p[Order_G] = m_gamma_inv[(((m_gamma_dir[s[Order_G]] - g) * alpha) >> base_shift) + g];
    p[Order_B] = m_gamma_inv[(((m_gamma_dir[s[Order_B]] - b) * alpha) >> base_shift) + b];
  }
}

//blender_rgb555
CC_INLINE pixel_type16 rgb555_make_pix(unsigned r, unsigned g, unsigned b)
{
  return (ushort)(((r & 0xF8) << 7) | ((g & 0xF8) << 2) | (b >> 3) | 0x8000);
}
CC_INLINE COLOR rgb555_make_color(pixel_type16 p)
{
  return _RGB((p >> 7) & 0xF8, (p >> 2) & 0xF8, (p << 3) & 0xF8);
}
CC_INLINE void blender_rgb555(pixel_type16* p, const uchar* s, unsigned width)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type16 rgb = *p;
    calc_type r = (rgb >> 7) & 0xF8;
    calc_type g = (rgb >> 2) & 0xF8;
    calc_type b = (rgb << 3) & 0xF8;
    *p = (pixel_type16)(((((s[Order_R] - r) * alpha + (r << 8)) >> 1)  & 0x7C00) |
      ((((s[Order_G] - g) * alpha + (g << 8)) >> 6)  & 0x03E0) |
      (((s[Order_B] - b) * alpha + (b << 8)) >> 11) | 0x8000);
  }
}
//blender_rgb555_pre
CC_INLINE void blender_rgb555_pre(pixel_type16* p, const uchar* s, unsigned width, const uchar* covers)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = base_mask - s[Order_A];
    unsigned cover = *covers++;
    pixel_type16 rgb = *p;
    calc_type r = (rgb >> 7) & 0xF8;
    calc_type g = (rgb >> 2) & 0xF8;
    calc_type b = (rgb << 3) & 0xF8;
    *p = (pixel_type16)
      ((((r * alpha + s[Order_R] * cover) >> 1)  & 0x7C00) |
      (((g * alpha + s[Order_G] * cover) >> 6)  & 0x03E0) |
      ((b * alpha + s[Order_B] * cover) >> 11) | 0x8000);
  }
}
//blender_rgb555_gamma
CC_INLINE void blender_rgb555_gamma(pixel_type16* p, const uchar* s, unsigned width, const uchar* m_gamma_dir, const uchar* m_gamma_inv)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type16 rgb = *p;
    calc_type r = m_gamma_dir[(rgb >> 7) & 0xF8];
    calc_type g = m_gamma_dir[(rgb >> 2) & 0xF8];
    calc_type b = m_gamma_dir[(rgb << 3) & 0xF8];
    *p = (pixel_type16)
      (((m_gamma_inv[((m_gamma_dir[s[Order_R]] - r) * alpha + (r << 8)) >> 8] << 7) & 0x7C00) |
      ((m_gamma_inv[((m_gamma_dir[s[Order_G]] - g) * alpha + (g << 8)) >> 8] << 2) & 0x03E0) |
      (m_gamma_inv[((m_gamma_dir[s[Order_B]] - b) * alpha + (b << 8)) >> 8] >> 3) | 0x8000);
  }
}

CC_INLINE pixel_type16 rgb565_make_pix(unsigned r, unsigned g, unsigned b)
{
  return (pixel_type16)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}

CC_INLINE COLOR rgb565_make_color(pixel_type16 p)
{
  return _RGB((p >> 8) & 0xF8, (p >> 3) & 0xFC, (p << 3) & 0xF8);
}

//blender_rgb565
CC_INLINE void blender_rgb565(pixel_type16* p, const uchar* s, unsigned width)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type16 rgb = *p;
    calc_type r = (rgb >> 8) & 0xF8;
    calc_type g = (rgb >> 3) & 0xFC;
    calc_type b = (rgb << 3) & 0xF8;
    *p = (pixel_type16)
      (((((s[Order_R] - r) * alpha + (r << 8))     ) & 0xF800) |
      ((((s[Order_G] - g) * alpha + (g << 8)) >> 5) & 0x07E0) |
      (((s[Order_B] - b) * alpha + (b << 8)) >> 11));
  }
}

//blender_rgb565_pre
CC_INLINE void blender_rgb565_pre(pixel_type16* p, const uchar* s, unsigned width, const uchar* covers)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = base_mask - s[Order_A];
    unsigned cover = *covers++;
    pixel_type16 rgb = *p;
    calc_type r = (rgb >> 8) & 0xF8;
    calc_type g = (rgb >> 3) & 0xFC;
    calc_type b = (rgb << 3) & 0xF8;
    *p = (pixel_type16)
      ((((r * alpha + s[Order_R] * cover)      ) & 0xF800) |
      (((g * alpha + s[Order_G] * cover) >> 5 ) & 0x07E0) |
      ((b * alpha + s[Order_B] * cover) >> 11));
  }
}

//blender_rgb565_gamma
CC_INLINE void blender_rgb565_gamma(pixel_type16* p, const uchar* s, unsigned width, const uchar* m_gamma_dir, const uchar* m_gamma_inv)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type16 rgb = *p;
    calc_type r = m_gamma_dir[(rgb >> 8) & 0xF8];
    calc_type g = m_gamma_dir[(rgb >> 3) & 0xFC];
    calc_type b = m_gamma_dir[(rgb << 3) & 0xF8];
    *p = (pixel_type16)
      (((m_gamma_inv[((m_gamma_dir[s[Order_R]] - r) * alpha + (r << 8)) >> 8] << 8) & 0xF800) |
      ((m_gamma_inv[((m_gamma_dir[s[Order_G]] - g) * alpha + (g << 8)) >> 8] << 3) & 0x07E0) |
      (m_gamma_inv[((m_gamma_dir[s[Order_B]] - b) * alpha + (b << 8)) >> 8] >> 3));
  }
}
CC_INLINE pixel_type32 rgbAAA_make_pix(unsigned r, unsigned g, unsigned b)
{
  return (pixel_type32)(((r & 0xFFC0) << 14) | ((g & 0xFFC0) << 4) | (b >> 6) | 0xC0000000);
}

CC_INLINE COLOR64 rgbAAA_make_color(pixel_type32 p)
{
  return _RGB16((p >> 14) & 0xFFC0, (p >> 4)  & 0xFFC0, (p << 6)  & 0xFFC0);
}
//blender_rgbAAA
CC_INLINE void blender_rgbAAA(pixel_type32* p, const ushort* s, unsigned width, unsigned alpha, const uchar* covers)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type32 rgb = *p;
    calc_type r = (rgb >> 14) & 0xFFC0;
    calc_type g = (rgb >> 4)  & 0xFFC0;
    calc_type b = (rgb << 6)  & 0xFFC0;
    *p = (pixel_type32)
      (((((s[Order_R] - r) * alpha + (r << 16)) >> 2)  & 0x3FF00000) |
      ((((s[Order_G] - g) * alpha + (g << 16)) >> 12) & 0x000FFC00) |
      (((s[Order_B] - b) * alpha + (b << 16)) >> 22) | 0xC0000000);
  }
}


//blender_rgbAAA_pre
CC_INLINE void blender_rgbAAA_pre(pixel_type32* p, const ushort* s, unsigned width, const uchar* covers)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = base_mask16 - s[Order_A];
    unsigned cover = *covers++;
    pixel_type32 rgb = *p;
    calc_type r = (rgb >> 14) & 0xFFC0;
    calc_type g = (rgb >> 4)  & 0xFFC0;
    calc_type b = (rgb << 6)  & 0xFFC0;
    cover = (cover + 1) << (base_shift16 - 8);
    *p = (pixel_type32)
      ((((r * alpha + s[Order_R] * cover) >> 2)  & 0x3FF00000) |
      (((g * alpha + s[Order_G] * cover) >> 12) & 0x000FFC00) |
      ((b * alpha + s[Order_B] * cover) >> 22) | 0xC0000000);
  }
}

//blender_rgbAAA_gamma
CC_INLINE void blender_rgbAAA_gamma(pixel_type32* p, const ushort* s, unsigned width, const ushort* m_gamma_dir, const ushort* m_gamma_inv)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type32 rgb = *p;
    calc_type r = m_gamma_dir[(rgb >> 14) & 0xFFC0];
    calc_type g = m_gamma_dir[(rgb >> 4)  & 0xFFC0];
    calc_type b = m_gamma_dir[(rgb << 6)  & 0xFFC0];
    *p = (pixel_type32)
      (((m_gamma_inv[((m_gamma_dir[s[Order_R]] - r) * alpha + (r << 16)) >> 16] << 14) & 0x3FF00000) |
      ((m_gamma_inv[((m_gamma_dir[s[Order_G]] - g) * alpha + (g << 16)) >> 16] << 4 ) & 0x000FFC00) |
      (m_gamma_inv[((m_gamma_dir[s[Order_B]] - b) * alpha + (b << 16)) >> 16] >> 6 ) | 0xC0000000);
  }
}

CC_INLINE pixel_type32 bgrAAA_make_pix(unsigned r, unsigned g, unsigned b)
{
  return (pixel_type32)(((b & 0xFFC0) << 14) | ((g & 0xFFC0) << 4) | (r >> 6) | 0xC0000000);
}

CC_INLINE COLOR64 bgrAAA_make_color(pixel_type32 p)
{
  return _RGB16((p << 6)  & 0xFFC0, (p >> 4)  & 0xFFC0, (p >> 14) & 0xFFC0);
}

//blender_bgrAAA
CC_INLINE void blender_bgrAAA(pixel_type32* p, const ushort* s, unsigned width)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type32 bgr = *p;
    calc_type b = (bgr >> 14) & 0xFFC0;
    calc_type g = (bgr >> 4)  & 0xFFC0;
    calc_type r = (bgr << 6)  & 0xFFC0;
    *p = (pixel_type32)
      (((((s[Order_B] - b) * alpha + (b << 16)) >> 2)  & 0x3FF00000) |
      ((((s[Order_G] - g) * alpha + (g << 16)) >> 12) & 0x000FFC00) |
      (((s[Order_R] - r) * alpha + (r << 16)) >> 22) | 0xC0000000);
  }
}

//blender_bgrAAA_pre
CC_INLINE void blender_bgrAAA_pre(pixel_type32* p, const ushort* s, unsigned width, const uchar* covers)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = base_mask16 - s[Order_A];
    unsigned cover = *covers++;
    pixel_type32 bgr = *p;
    calc_type b = (bgr >> 14) & 0xFFC0;
    calc_type g = (bgr >> 4)  & 0xFFC0;
    calc_type r = (bgr << 6)  & 0xFFC0;
    cover = (cover + 1) << (base_shift16 - 8);
    *p = (pixel_type32)
      ((((b * alpha + s[Order_B] * cover) >> 2)  & 0x3FF00000) |
      (((g * alpha + s[Order_G] * cover) >> 12) & 0x000FFC00) |
      ((r * alpha + s[Order_R] * cover) >> 22) | 0xC0000000);
  }
}

//blender_bgrAAA_gamma
CC_INLINE void blender_bgrAAA_gamma(pixel_type32* p, const ushort* s, unsigned width, const ushort* m_gamma_dir, const ushort* m_gamma_inv)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type32 bgr = *p;
    calc_type b = m_gamma_dir[(bgr >> 14) & 0xFFC0];
    calc_type g = m_gamma_dir[(bgr >> 4)  & 0xFFC0];
    calc_type r = m_gamma_dir[(bgr << 6)  & 0xFFC0];
    *p = (pixel_type32)
      (((m_gamma_inv[((m_gamma_dir[s[Order_B]] - b) * alpha + (b << 16)) >> 16] << 14) & 0x3FF00000) |
      ((m_gamma_inv[((m_gamma_dir[s[Order_G]] - g) * alpha + (g << 16)) >> 16] << 4 ) & 0x000FFC00) |
      (m_gamma_inv[((m_gamma_dir[s[Order_R]] - r) * alpha + (r << 16)) >> 16] >> 6 ) | 0xC0000000);
  }
}

//blender_rgbBBA
CC_INLINE pixel_type32 rgbBBA_make_pix(unsigned r, unsigned g, unsigned b)
{
  return (pixel_type32)(((r & 0xFFE0) << 16) | ((g & 0xFFE0) << 5) | (b >> 6));
}

CC_INLINE COLOR64 rgbBBA_make_color(pixel_type32 p)
{
  return _RGB16((p >> 16) & 0xFFE0, (p >> 5)  & 0xFFE0, (p << 6)  & 0xFFC0);
}
CC_INLINE void blender_rgbBBA(pixel_type32* p, const ushort* s, unsigned width)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type32 rgb = *p;
    calc_type r = (rgb >> 16) & 0xFFE0;
    calc_type g = (rgb >> 5)  & 0xFFE0;
    calc_type b = (rgb << 6)  & 0xFFC0;
    *p = (pixel_type32)
      (((((s[Order_R] - r) * alpha + (r << 16))      ) & 0xFFE00000) |
      ((((s[Order_G] - g) * alpha + (g << 16)) >> 11) & 0x001FFC00) |
      (((s[Order_B] - b) * alpha + (b << 16)) >> 22));
  }
}

//blender_rgbBBA_pre
CC_INLINE void blender_rgbBBA_pre(pixel_type32* p, const ushort* s, unsigned width,
                                  unsigned alpha, 
                                  unsigned cover)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = base_mask16 - s[Order_A];
    pixel_type32 rgb = *p;
    calc_type r = (rgb >> 16) & 0xFFE0;
    calc_type g = (rgb >> 5)  & 0xFFE0;
    calc_type b = (rgb << 6)  & 0xFFC0;
    cover = (cover + 1) << (base_shift16 - 8);
    *p = (pixel_type32)
      ((((r * alpha + s[Order_R] * cover)      ) & 0xFFE00000) |
      (((g * alpha + s[Order_G] * cover) >> 11) & 0x001FFC00) |
      ((b * alpha + s[Order_B] * cover) >> 22));
  }
}

//blender_rgbBBA_gamma
CC_INLINE void blender_rgbBBA_gamma(pixel_type32* p, const ushort* s, unsigned width, const ushort* m_gamma_dir, const ushort* m_gamma_inv)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type32 rgb = *p;
    calc_type r = m_gamma_dir[(rgb >> 16) & 0xFFE0];
    calc_type g = m_gamma_dir[(rgb >> 5)  & 0xFFE0];
    calc_type b = m_gamma_dir[(rgb << 6)  & 0xFFC0];
    *p = (pixel_type32)
      (((m_gamma_inv[((m_gamma_dir[s[Order_R]] - r) * alpha + (r << 16)) >> 16] << 16) & 0xFFE00000) |
      ((m_gamma_inv[((m_gamma_dir[s[Order_G]] - g) * alpha + (g << 16)) >> 16] << 5 ) & 0x001FFC00) |
      (m_gamma_inv[((m_gamma_dir[s[Order_B]] - b) * alpha + (b << 16)) >> 16] >> 6 ));
  }
}

CC_INLINE pixel_type32 bgrABB_make_pix(unsigned r, unsigned g, unsigned b)
{
  return (pixel_type32)(((b & 0xFFC0) << 16) | ((g & 0xFFE0) << 6) | (r >> 5));
}

CC_INLINE COLOR64 bgrABB_make_color(pixel_type32 p)
{
  return _RGB16((p << 5)  & 0xFFE0, (p >> 6)  & 0xFFE0, (p >> 16) & 0xFFC0);
}
//blender_bgrABB
CC_INLINE void blender_bgrABB(pixel_type32* p, const ushort* s, unsigned width)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type32 bgr = *p;
    calc_type b = (bgr >> 16) & 0xFFC0;
    calc_type g = (bgr >> 6)  & 0xFFE0;
    calc_type r = (bgr << 5)  & 0xFFE0;
    *p = (pixel_type32)
      (((((s[Order_B] - b) * alpha + (b << 16))      ) & 0xFFC00000) |
      ((((s[Order_G] - g) * alpha + (g << 16)) >> 10) & 0x003FF800) |
      (((s[Order_R] - r) * alpha + (r << 16)) >> 21));
  }
}
//blender_bgrABB_pre
CC_INLINE void blender_bgrABB_pre(pixel_type32* p, const ushort* s, unsigned width, const uchar* covers)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = base_mask16 - s[Order_A];
    unsigned cover = *covers++;
    pixel_type32 bgr = *p;
    calc_type b = (bgr >> 16) & 0xFFC0;
    calc_type g = (bgr >> 6)  & 0xFFE0;
    calc_type r = (bgr << 5)  & 0xFFE0;
    cover = (cover + 1) << (base_shift16 - 8);
    *p = (pixel_type32)
      ((((b * alpha + s[Order_B] * cover)      ) & 0xFFC00000) |
      (((g * alpha + s[Order_G] * cover) >> 10) & 0x003FF800) |
      ((r * alpha + s[Order_R] * cover) >> 21));
  }
}

//blender_bgrABB_gamma
CC_INLINE void blender_bgrABB_gamma(pixel_type32* p, const ushort* s, unsigned width, const ushort* m_gamma_dir, const ushort* m_gamma_inv)
{
  for ( ;width--; ++p, s+=4) {
    unsigned alpha = s[Order_A];
    pixel_type32 bgr = *p;
    calc_type b = m_gamma_dir[(bgr >> 16) & 0xFFC0];
    calc_type g = m_gamma_dir[(bgr >> 6)  & 0xFFE0];
    calc_type r = m_gamma_dir[(bgr << 5)  & 0xFFE0];
    *p = (pixel_type32)
      (((m_gamma_inv[((m_gamma_dir[s[Order_B]] - b) * alpha + (b << 16)) >> 16] << 16) & 0xFFC00000) |
      ((m_gamma_inv[((m_gamma_dir[s[Order_G]] - g) * alpha + (g << 16)) >> 16] << 6 ) & 0x003FF800) |
      (m_gamma_inv[((m_gamma_dir[s[Order_R]] - r) * alpha + (r << 16)) >> 16] >> 5 ));
  }
}


//multiplier_rgba

CC_INLINE void rgba_premultiply(uchar* p, unsigned width)
{
  for ( ;width--; p+=4) {
    calc_type a = p[Order_A];
    if(a < base_mask) {
      if(a == 0) {
        p[Order_R] = p[Order_G] = p[Order_B] = 0;
      } else {
        p[Order_R] = ((p[Order_R] * a + base_mask) >> base_shift);
        p[Order_G] = ((p[Order_G] * a + base_mask) >> base_shift);
        p[Order_B] = ((p[Order_B] * a + base_mask) >> base_shift);
      }
    }
  }
}

CC_INLINE void rgba_demultiply(uchar* p, unsigned width)
{
  for ( ;width--; p+=4) {
    calc_type a = p[Order_A];
    if(a < base_mask) {
      if(a == 0) {
        p[Order_R] = p[Order_G] = p[Order_B] = 0;
      } else {
        calc_type r = ((calc_type)(p[Order_R]) * base_mask) / a;
        calc_type g = ((calc_type)(p[Order_G]) * base_mask) / a;
        calc_type b = ((calc_type)(p[Order_B]) * base_mask) / a;
        p[Order_R] = ((r > base_mask) ? base_mask : r);
        p[Order_G] = ((g > base_mask) ? base_mask : g);
        p[Order_B] = ((b > base_mask) ? base_mask : b);
      }
    }
  }
}

//apply_gamma_dir_rgba
CC_INLINE void apply_gamma_dir_rgba(uchar* p, unsigned width, const uchar* m_gamma_dir)
{
  for ( ;width--; p+=4) {
    p[Order_R] = m_gamma_dir[p[Order_R]];
    p[Order_G] = m_gamma_dir[p[Order_G]];
    p[Order_B] = m_gamma_dir[p[Order_B]];
  }
}

//apply_gamma_inv_rgba
CC_INLINE void apply_gamma_inv_rgba(uchar* p, unsigned width, const uchar* m_gamma_inv)
{
  for ( ;width--; p+=4) {
    p[Order_R] = m_gamma_inv[p[Order_R]];
    p[Order_G] = m_gamma_inv[p[Order_G]];
    p[Order_B] = m_gamma_inv[p[Order_B]];
  }
}

//blender_rgba
CC_INLINE void blender_rgba(uchar* p, const uchar* s, unsigned width, const uchar* covers)
{
  typedef uchar value_type;
  int len = width;
  if (covers) {
    for ( ;width--; p+=4, s+=4) {
      unsigned cover = *covers++;
      unsigned alpha = alpha_cover(s[Order_A], cover);
      calc_type r = p[Order_R];
      calc_type g = p[Order_G];
      calc_type b = p[Order_B];
      calc_type a = p[Order_A];
      
      p[Order_R] = (value_type)(((s[Order_R] - r) * alpha + (r << base_shift)) >> base_shift);
      p[Order_G] = (value_type)(((s[Order_G] - g) * alpha + (g << base_shift)) >> base_shift);
      p[Order_B] = (value_type)(((s[Order_B] - b) * alpha + (b << base_shift)) >> base_shift);
      //p[Order_A] = (value_type)(((s[Order_A] - b) * alpha + (b << base_shift)) >> base_shift);
      p[Order_A] = 255;
    }
  } else {
    for ( ;width--; p+=4, s+=4) {
      unsigned alpha = s[Order_A];
      calc_type r = p[Order_R];
      calc_type g = p[Order_G];
      calc_type b = p[Order_B];
      calc_type a = p[Order_A];
      p[Order_R] = (value_type)(((s[Order_R] - r) * alpha + (r << base_shift)) >> base_shift);
      p[Order_G] = (value_type)(((s[Order_G] - g) * alpha + (g << base_shift)) >> base_shift);
      p[Order_B] = (value_type)(((s[Order_B] - b) * alpha + (b << base_shift)) >> base_shift);
      p[Order_A] = (value_type)((alpha + a) - ((alpha * a + base_mask) >> base_shift));
    }
  }
}

CC_INLINE void blender_gray(uchar* p, const uchar* s, unsigned width, const uchar* covers)
{
  //typedef uchar value_type;
  int len = width;
  if (covers) {
    for ( ;width--; ++p, s+=4) {
      unsigned cover = *covers++;
      unsigned alpha = alpha_cover(s[Order_A], cover);
      calc_type r = s[Order_R];
      calc_type g = s[Order_G];
      calc_type b = s[Order_B];
      calc_type a = s[Order_A];
      p[0] = GREYSCALIZE(r, g, b);
    }
  } else {
    for ( ;width--; ++p+=4, s+=4) {
      unsigned alpha = s[Order_A];
      calc_type r = s[Order_R];
      calc_type g = s[Order_G];
      calc_type b = s[Order_B];
      calc_type a = s[Order_A];
      p[0] = GREYSCALIZE(r, g, b);
    }
  }
}

//blender_rgba_bin
CC_INLINE void blender_rgba_bin(uchar* p, const uchar* s, unsigned width, const uchar* covers)
{
  typedef uchar value_type;
  if (covers) {
    for ( ;width--; p+=4, s+=4) {
      unsigned cover = *covers++;
      if (cover) {
        unsigned alpha = (s[Order_A]);
        calc_type r = p[Order_R];
        calc_type g = p[Order_G];
        calc_type b = p[Order_B];
        calc_type a = p[Order_A];
        
        p[Order_R] = (value_type)(((s[Order_R] - r) * alpha + (r << base_shift)) >> base_shift);
        p[Order_G] = (value_type)(((s[Order_G] - g) * alpha + (g << base_shift)) >> base_shift);
        p[Order_B] = (value_type)(((s[Order_B] - b) * alpha + (b << base_shift)) >> base_shift);
        //p[Order_A] = (value_type)(((s[Order_A] - b) * alpha + (b << base_shift)) >> base_shift);
        p[Order_A] = 255;
      }
    }
  } else {
    for ( ;width--; p+=4, s+=4) {
      unsigned alpha = s[Order_A];
      calc_type r = p[Order_R];
      calc_type g = p[Order_G];
      calc_type b = p[Order_B];
      calc_type a = p[Order_A];
      p[Order_R] = (value_type)(((s[Order_R] - r) * alpha + (r << base_shift)) >> base_shift);
      p[Order_G] = (value_type)(((s[Order_G] - g) * alpha + (g << base_shift)) >> base_shift);
      p[Order_B] = (value_type)(((s[Order_B] - b) * alpha + (b << base_shift)) >> base_shift);
      p[Order_A] = (value_type)((alpha + a) - ((alpha * a + base_mask) >> base_shift));
    }
  }
}

CC_INLINE void blender_rgb_bin(uchar* p, const uchar* s, unsigned width, const uchar* covers)
{
  typedef uchar value_type;
  if (covers) {
    for ( ;width--; p+=3, s+=4) {
      unsigned cover = *covers++;
      if (cover) {
        unsigned alpha = (s[Order_A]);
        calc_type r = p[Order_R];
        calc_type g = p[Order_G];
        calc_type b = p[Order_B];
        calc_type a = p[Order_A];
        
        p[Order_R] = (value_type)(((s[Order_R] - r) * alpha + (r << base_shift)) >> base_shift);
        p[Order_G] = (value_type)(((s[Order_G] - g) * alpha + (g << base_shift)) >> base_shift);
        p[Order_B] = (value_type)(((s[Order_B] - b) * alpha + (b << base_shift)) >> base_shift);
        //p[Order_A] = (value_type)(((s[Order_A] - b) * alpha + (b << base_shift)) >> base_shift);
      }
    }
  } else {
    for ( ;width--; p+=3, s+=4) {
      unsigned alpha = s[Order_A];
      calc_type r = p[Order_R];
      calc_type g = p[Order_G];
      calc_type b = p[Order_B];
      calc_type a = p[Order_A];
      calc_type pa = (value_type)((alpha + a) - ((alpha * a + base_mask) >> base_shift));
      p[Order_R] = (value_type)(((s[Order_R] - r) * alpha + (r << base_shift)) >> base_shift);
      p[Order_G] = (value_type)(((s[Order_G] - g) * alpha + (g << base_shift)) >> base_shift);
      p[Order_B] = (value_type)(((s[Order_B] - b) * alpha + (b << base_shift)) >> base_shift);
      p[Order_R] = p[Order_R]*pa>>8;
      p[Order_G] = p[Order_G]*pa>>8;
      p[Order_B] = p[Order_B]*pa>>8;
    }
  }
}

CC_INLINE void blender_gray_bin(uchar* p, const uchar* s, unsigned width, const uchar* covers)
{
  //typedef uchar value_type;
  if (covers) {
    for ( ;width--; ++p, s+=4) {
      unsigned cover = *covers++;
      if (cover) {
        unsigned alpha = (s[Order_A]);
        calc_type r = s[Order_R];
        calc_type g = s[Order_G];
        calc_type b = s[Order_B];
        calc_type a = s[Order_A];
        
        p[0] = GREYSCALIZE(r, g, b);
      }
    }
  } else {
    for ( ;width--; ++p, s+=4) {
      unsigned alpha = s[Order_A];
      calc_type r = s[Order_R];
      calc_type g = s[Order_G];
      calc_type b = s[Order_B];
      calc_type a = s[Order_A];
      p[0] = GREYSCALIZE(r, g, b);
    }
  }
}

//blender_rgba_pre
CC_INLINE void blender_rgba_pre(uchar* p, const uchar* s, unsigned width, const uchar* covers)
{
  typedef uchar value_type;
  if (covers) {
    for ( ;width--; p+=4, s+=4) {
      unsigned alpha = base_mask - s[Order_A];
      unsigned cover = *covers++;
      p[Order_R] = (value_type)((p[Order_R] * alpha + s[Order_R] * cover) >> base_shift);
      p[Order_G] = (value_type)((p[Order_G] * alpha + s[Order_G] * cover) >> base_shift);
      p[Order_B] = (value_type)((p[Order_B] * alpha + s[Order_B] * cover) >> base_shift);
      p[Order_A] = (value_type)(base_mask - ((alpha * (base_mask - p[Order_A])) >> base_shift));
    }
  } else {
    for ( ;width--; p+=4, s+=4) {
      unsigned alpha = base_mask - s[Order_A];
      p[Order_R] = (value_type)(((p[Order_R] * alpha) >> base_shift) + s[Order_R]);
      p[Order_G] = (value_type)(((p[Order_G] * alpha) >> base_shift) + s[Order_G]);
      p[Order_B] = (value_type)(((p[Order_B] * alpha) >> base_shift) + s[Order_B]);
      p[Order_A] = (value_type)(base_mask - ((alpha * (base_mask - p[Order_A])) >> base_shift));
    }
  }
}

//blender_rgba_plain
CC_INLINE void blender_rgba_plain(uchar* p, const uchar* s, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    unsigned alpha = s[Order_A];
    if(alpha) {
      calc_type a = p[Order_A];
      calc_type r = p[Order_R] * a;
      calc_type g = p[Order_G] * a;
      calc_type b = p[Order_B] * a;
      a = ((alpha + a) << base_shift) - alpha * a;
      p[Order_A] = (value_type)(a >> base_shift);
      p[Order_R] = (value_type)((((s[Order_R] << base_shift) - r) * alpha + (r << base_shift)) / a);
      p[Order_G] = (value_type)((((s[Order_G] << base_shift) - g) * alpha + (g << base_shift)) / a);
      p[Order_B] = (value_type)((((s[Order_B] << base_shift) - b) * alpha + (b << base_shift)) / a);
    }
  }
}

//comp_op_rgba_clear
CC_INLINE void comp_op_rgba_clear(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    if(cover < 255) {
      cover = 255 - cover;
      p[Order_R] = (value_type)((p[Order_R] * cover + 255) >> 8);
      p[Order_G] = (value_type)((p[Order_G] * cover + 255) >> 8);
      p[Order_B] = (value_type)((p[Order_B] * cover + 255) >> 8);
      p[Order_A] = (value_type)((p[Order_A] * cover + 255) >> 8);
    }
    else
    {
      p[0] = p[1] = p[2] = p[3] = 0; 
    }
  }
}

//comp_op_rgba_src
CC_INLINE void comp_op_rgba_src(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    if(cover < 255)
    {
      unsigned alpha = 255 - cover;
      p[Order_R] = (value_type)(((p[Order_R] * alpha + 255) >> 8) + ((s[Order_R] * cover + 255) >> 8));
      p[Order_G] = (value_type)(((p[Order_G] * alpha + 255) >> 8) + ((s[Order_G] * cover + 255) >> 8));
      p[Order_B] = (value_type)(((p[Order_B] * alpha + 255) >> 8) + ((s[Order_B] * cover + 255) >> 8));
      p[Order_A] = (value_type)(((p[Order_A] * alpha + 255) >> 8) + ((s[Order_A] * cover + 255) >> 8));
    } else {
      p[Order_R] = s[Order_R];
      p[Order_G] = s[Order_G];
      p[Order_B] = s[Order_B];
      p[Order_A] = s[Order_A];
    }
  }
}

//comp_op_rgba_dst
CC_INLINE void comp_op_rgba_dst(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
}

//comp_op_rgba_src_over
//   Dca' = Sca + Dca.(1 - Sa)
//   Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_src_over(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    {calc_type s1a = base_mask - sa;
    p[Order_R] = (value_type)(sr + ((p[Order_R] * s1a + base_mask) >> base_shift));
    p[Order_G] = (value_type)(sg + ((p[Order_G] * s1a + base_mask) >> base_shift));
    p[Order_B] = (value_type)(sb + ((p[Order_B] * s1a + base_mask) >> base_shift));
    p[Order_A] = (value_type)(sa + p[Order_A] - ((sa * p[Order_A] + base_mask) >> base_shift));
    }
  }
}

//comp_op_rgba_dst_over
// Dca' = Dca + Sca.(1 - Da)
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_dst_over(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    {calc_type d1a = base_mask - p[Order_A];
    p[Order_R] = (value_type)(p[Order_R] + ((sr * d1a + base_mask) >> base_shift));
    p[Order_G] = (value_type)(p[Order_G] + ((sg * d1a + base_mask) >> base_shift));
    p[Order_B] = (value_type)(p[Order_B] + ((sb * d1a + base_mask) >> base_shift));
    p[Order_A] = (value_type)(sa + p[Order_A] - ((sa * p[Order_A] + base_mask) >> base_shift));}
  }
}


//comp_op_rgba_src_in
// Dca' = Sca.Da
// Da'  = Sa.Da 
CC_INLINE void comp_op_rgba_src_in(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    calc_type da = p[Order_A];
    if(cover < 255)
    {
      unsigned alpha = 255 - cover;
      p[Order_R] = (value_type)(((p[Order_R] * alpha + 255) >> 8) + ((((sr * da + base_mask) >> base_shift) * cover + 255) >> 8));
      p[Order_G] = (value_type)(((p[Order_G] * alpha + 255) >> 8) + ((((sg * da + base_mask) >> base_shift) * cover + 255) >> 8));
      p[Order_B] = (value_type)(((p[Order_B] * alpha + 255) >> 8) + ((((sb * da + base_mask) >> base_shift) * cover + 255) >> 8));
      p[Order_A] = (value_type)(((p[Order_A] * alpha + 255) >> 8) + ((((sa * da + base_mask) >> base_shift) * cover + 255) >> 8));
    }
    else
    {
      p[Order_R] = (value_type)((sr * da + base_mask) >> base_shift);
      p[Order_G] = (value_type)((sg * da + base_mask) >> base_shift);
      p[Order_B] = (value_type)((sb * da + base_mask) >> base_shift);
      p[Order_A] = (value_type)((sa * da + base_mask) >> base_shift);
    }
  }
}

//comp_op_rgba_dst_in
// Dca' = Dca.Sa
// Da'  = Sa.Da 
CC_INLINE void comp_op_rgba_dst_in(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sa = base_mask - ((cover * (base_mask - sa) + 255) >> 8);
    }
    p[Order_R] = (value_type)((p[Order_R] * sa + base_mask) >> base_shift);
    p[Order_G] = (value_type)((p[Order_G] * sa + base_mask) >> base_shift);
    p[Order_B] = (value_type)((p[Order_B] * sa + base_mask) >> base_shift);
    p[Order_A] = (value_type)((p[Order_A] * sa + base_mask) >> base_shift);
  }
}

//comp_op_rgba_src_out
// Dca' = Sca.(1 - Da)
// Da'  = Sa.(1 - Da) 

CC_INLINE void comp_op_rgba_src_out(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    calc_type da = base_mask - p[Order_A];
    if(cover < 255)
    {
      unsigned alpha = 255 - cover;
      p[Order_R] = (value_type)(((p[Order_R] * alpha + 255) >> 8) + ((((sr * da + base_mask) >> base_shift) * cover + 255) >> 8));
      p[Order_G] = (value_type)(((p[Order_G] * alpha + 255) >> 8) + ((((sg * da + base_mask) >> base_shift) * cover + 255) >> 8));
      p[Order_B] = (value_type)(((p[Order_B] * alpha + 255) >> 8) + ((((sb * da + base_mask) >> base_shift) * cover + 255) >> 8));
      p[Order_A] = (value_type)(((p[Order_A] * alpha + 255) >> 8) + ((((sa * da + base_mask) >> base_shift) * cover + 255) >> 8));
    }
    else
    {
      p[Order_R] = (value_type)((sr * da + base_mask) >> base_shift);
      p[Order_G] = (value_type)((sg * da + base_mask) >> base_shift);
      p[Order_B] = (value_type)((sb * da + base_mask) >> base_shift);
      p[Order_A] = (value_type)((sa * da + base_mask) >> base_shift);
    }
  }
}

//comp_op_rgba_dst_out
// Dca' = Dca.(1 - Sa) 
// Da'  = Da.(1 - Sa) 
CC_INLINE void comp_op_rgba_dst_out(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sa = (sa * cover + 255) >> 8;
    }
    sa = base_mask - sa;
    p[Order_R] = (value_type)((p[Order_R] * sa + base_shift) >> base_shift);
    p[Order_G] = (value_type)((p[Order_G] * sa + base_shift) >> base_shift);
    p[Order_B] = (value_type)((p[Order_B] * sa + base_shift) >> base_shift);
    p[Order_A] = (value_type)((p[Order_A] * sa + base_shift) >> base_shift);
  }
}

//comp_op_rgba_src_atop
// Dca' = Sca.Da + Dca.(1 - Sa)
// Da'  = Da
CC_INLINE void comp_op_rgba_src_atop(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    {calc_type da = p[Order_A];
    sa = base_mask - sa;
    p[Order_R] = (value_type)((sr * da + p[Order_R] * sa + base_mask) >> base_shift);
    p[Order_G] = (value_type)((sg * da + p[Order_G] * sa + base_mask) >> base_shift);
    p[Order_B] = (value_type)((sb * da + p[Order_B] * sa + base_mask) >> base_shift);}
  }
}

//comp_op_rgba_dst_atop
// Dca' = Dca.Sa + Sca.(1 - Da)
// Da'  = Sa 
CC_INLINE void comp_op_rgba_dst_atop(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    calc_type da = base_mask - p[Order_A];
    if(cover < 255)
    {
      unsigned alpha = 255 - cover;
      sr = (p[Order_R] * sa + sr * da + base_mask) >> base_shift;
      sg = (p[Order_G] * sa + sg * da + base_mask) >> base_shift;
      sb = (p[Order_B] * sa + sb * da + base_mask) >> base_shift;
      p[Order_R] = (value_type)(((p[Order_R] * alpha + 255) >> 8) + ((sr * cover + 255) >> 8));
      p[Order_G] = (value_type)(((p[Order_G] * alpha + 255) >> 8) + ((sg * cover + 255) >> 8));
      p[Order_B] = (value_type)(((p[Order_B] * alpha + 255) >> 8) + ((sb * cover + 255) >> 8));
      p[Order_A] = (value_type)(((p[Order_A] * alpha + 255) >> 8) + ((sa * cover + 255) >> 8));
      
    }
    else
    {
      p[Order_R] = (value_type)((p[Order_R] * sa + sr * da + base_mask) >> base_shift);
      p[Order_G] = (value_type)((p[Order_G] * sa + sg * da + base_mask) >> base_shift);
      p[Order_B] = (value_type)((p[Order_B] * sa + sb * da + base_mask) >> base_shift);
      p[Order_A] = (value_type)sa;
    }
  }
}

//comp_op_rgba_xor
// Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
// Da'  = Sa + Da - 2.Sa.Da 
CC_INLINE void comp_op_rgba_xor(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type s1a = base_mask - sa;
      calc_type d1a = base_mask - p[Order_A];
      p[Order_R] = (value_type)((p[Order_R] * s1a + sr * d1a + base_mask) >> base_shift);
      p[Order_G] = (value_type)((p[Order_G] * s1a + sg * d1a + base_mask) >> base_shift);
      p[Order_B] = (value_type)((p[Order_B] * s1a + sb * d1a + base_mask) >> base_shift);
      p[Order_A] = (value_type)(sa + p[Order_A] - ((sa * p[Order_A] + base_mask/2) >> (base_shift - 1)));
    }
  }
}

//comp_op_rgba_plus
// Dca' = Sca + Dca
// Da'  = Sa + Da 
CC_INLINE void comp_op_rgba_plus(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type dr = p[Order_R] + sr;
      calc_type dg = p[Order_G] + sg;
      calc_type db = p[Order_B] + sb;
      calc_type da = p[Order_A] + sa;
      p[Order_R] = (dr > base_mask) ? (value_type)base_mask : dr;
      p[Order_G] = (dg > base_mask) ? (value_type)base_mask : dg;
      p[Order_B] = (db > base_mask) ? (value_type)base_mask : db;
      p[Order_A] = (da > base_mask) ? (value_type)base_mask : da;
    }
  }
}

//comp_op_rgba_minus
// Dca' = Dca - Sca
// Da' = 1 - (1 - Sa).(1 - Da)
CC_INLINE void comp_op_rgba_minus(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type dr = p[Order_R] - sr;
      calc_type dg = p[Order_G] - sg;
      calc_type db = p[Order_B] - sb;
      p[Order_R] = (dr > base_mask) ? 0 : dr;
      p[Order_G] = (dg > base_mask) ? 0 : dg;
      p[Order_B] = (db > base_mask) ? 0 : db;
      p[Order_A] = (value_type)(sa + p[Order_A] - ((sa * p[Order_A] + base_mask) >> base_shift));
      //p[Order_A] = (value_type)(base_mask - (((base_mask - sa) * (base_mask - p[Order_A]) + base_mask) >> base_shift));
    }
  }
}

//comp_op_rgba_multiply
// Dca' = Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_multiply(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type s1a = base_mask - sa;
      calc_type d1a = base_mask - p[Order_A];
      calc_type dr = p[Order_R];
      calc_type dg = p[Order_G];
      calc_type db = p[Order_B];
      p[Order_R] = (value_type)((sr * dr + sr * d1a + dr * s1a + base_mask) >> base_shift);
      p[Order_G] = (value_type)((sg * dg + sg * d1a + dg * s1a + base_mask) >> base_shift);
      p[Order_B] = (value_type)((sb * db + sb * d1a + db * s1a + base_mask) >> base_shift);
      p[Order_A] = (value_type)(sa + p[Order_A] - ((sa * p[Order_A] + base_mask) >> base_shift));
    }
  }
}

//comp_op_rgba_screen
// Dca' = Sca + Dca - Sca.Dca
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_screen(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type dr = p[Order_R];
      calc_type dg = p[Order_G];
      calc_type db = p[Order_B];
      calc_type da = p[Order_A];
      p[Order_R] = (value_type)(sr + dr - ((sr * dr + base_mask) >> base_shift));
      p[Order_G] = (value_type)(sg + dg - ((sg * dg + base_mask) >> base_shift));
      p[Order_B] = (value_type)(sb + db - ((sb * db + base_mask) >> base_shift));
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}

//comp_op_rgba_overlay
// if 2.Dca < Da
//   Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
// otherwise
//   Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
// 
// Da' = Sa + Da - Sa.Da
CC_INLINE void comp_op_rgba_overlay(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type d1a  = base_mask - p[Order_A];
      calc_type s1a  = base_mask - sa;
      calc_type dr   = p[Order_R];
      calc_type dg   = p[Order_G];
      calc_type db   = p[Order_B];
      calc_type da   = p[Order_A];
      calc_type sada = sa * p[Order_A];
      
      p[Order_R] = (value_type)(((2*dr < da) ? 
        2*sr*dr + sr*d1a + dr*s1a : 
      sada - 2*(da - dr)*(sa - sr) + sr*d1a + dr*s1a + base_mask) >> base_shift);
      
      p[Order_G] = (value_type)(((2*dg < da) ? 
        2*sg*dg + sg*d1a + dg*s1a : 
      sada - 2*(da - dg)*(sa - sg) + sg*d1a + dg*s1a + base_mask) >> base_shift);
      
      p[Order_B] = (value_type)(((2*db < da) ? 
        2*sb*db + sb*d1a + db*s1a : 
      sada - 2*(da - db)*(sa - sb) + sb*d1a + db*s1a + base_mask) >> base_shift);
      
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}


CC_INLINE int sd_min(int a, int b) { return (a < b) ? a : b; }
CC_INLINE int sd_max(int a, int b) { return (a > b) ? a : b; }

//comp_op_rgba_darken
// Dca' = min(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_darken(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type d1a = base_mask - p[Order_A];
      calc_type s1a = base_mask - sa;
      calc_type dr  = p[Order_R];
      calc_type dg  = p[Order_G];
      calc_type db  = p[Order_B];
      calc_type da  = p[Order_A];
      
      p[Order_R] = (value_type)((sd_min(sr * da, dr * sa) + sr * d1a + dr * s1a + base_mask) >> base_shift);
      p[Order_G] = (value_type)((sd_min(sg * da, dg * sa) + sg * d1a + dg * s1a + base_mask) >> base_shift);
      p[Order_B] = (value_type)((sd_min(sb * da, db * sa) + sb * d1a + db * s1a + base_mask) >> base_shift);
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}


//comp_op_rgba_lighten
// Dca' = max(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_lighten(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type d1a = base_mask - p[Order_A];
      calc_type s1a = base_mask - sa;
      calc_type dr  = p[Order_R];
      calc_type dg  = p[Order_G];
      calc_type db  = p[Order_B];
      calc_type da  = p[Order_A];
      
      p[Order_R] = (value_type)((sd_max(sr * da, dr * sa) + sr * d1a + dr * s1a + base_mask) >> base_shift);
      p[Order_G] = (value_type)((sd_max(sg * da, dg * sa) + sg * d1a + dg * s1a + base_mask) >> base_shift);
      p[Order_B] = (value_type)((sd_max(sb * da, db * sa) + sb * d1a + db * s1a + base_mask) >> base_shift);
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}
//comp_op_rgba_color_dodge

// if Sca.Da + Dca.Sa >= Sa.Da
//   Dca' = Sa.Da + Sca.(1 - Da) + Dca.(1 - Sa)
// otherwise
//   Dca' = Dca.Sa/(1-Sca/Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
//
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_color_dodge(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type d1a  = base_mask - p[Order_A];
      calc_type s1a  = base_mask - sa;
      calc_type dr   = p[Order_R];
      calc_type dg   = p[Order_G];
      calc_type db   = p[Order_B];
      calc_type da   = p[Order_A];
      long_type drsa = dr * sa;
      long_type dgsa = dg * sa;
      long_type dbsa = db * sa;
      long_type srda = sr * da;
      long_type sgda = sg * da;
      long_type sbda = sb * da;
      long_type sada = sa * da;
      
      p[Order_R] = (value_type)((srda + drsa >= sada) ? 
        (sada + sr * d1a + dr * s1a + base_mask) >> base_shift :
      drsa / (base_mask - (sr << base_shift) / sa) + ((sr * d1a + dr * s1a + base_mask) >> base_shift));
      
      p[Order_G] = (value_type)((sgda + dgsa >= sada) ? 
        (sada + sg * d1a + dg * s1a + base_mask) >> base_shift :
      dgsa / (base_mask - (sg << base_shift) / sa) + ((sg * d1a + dg * s1a + base_mask) >> base_shift));
      
      p[Order_B] = (value_type)((sbda + dbsa >= sada) ? 
        (sada + sb * d1a + db * s1a + base_mask) >> base_shift :
      dbsa / (base_mask - (sb << base_shift) / sa) + ((sb * d1a + db * s1a + base_mask) >> base_shift));
      
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}
//comp_op_rgba_color_burn
// if Sca.Da + Dca.Sa <= Sa.Da
//   Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
// otherwise
//   Dca' = Sa.(Sca.Da + Dca.Sa - Sa.Da)/Sca + Sca.(1 - Da) + Dca.(1 - Sa)
// 
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_color_burn(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type d1a  = base_mask - p[Order_A];
      calc_type s1a  = base_mask - sa;
      calc_type dr   = p[Order_R];
      calc_type dg   = p[Order_G];
      calc_type db   = p[Order_B];
      calc_type da   = p[Order_A];
      long_type drsa = dr * sa;
      long_type dgsa = dg * sa;
      long_type dbsa = db * sa;
      long_type srda = sr * da;
      long_type sgda = sg * da;
      long_type sbda = sb * da;
      long_type sada = sa * da;
      
      p[Order_R] = (value_type)(((srda + drsa <= sada) ? 
        sr * d1a + dr * s1a :
      sa * (srda + drsa - sada) / sr + sr * d1a + dr * s1a + base_mask) >> base_shift);
      
      p[Order_G] = (value_type)(((sgda + dgsa <= sada) ? 
        sg * d1a + dg * s1a :
      sa * (sgda + dgsa - sada) / sg + sg * d1a + dg * s1a + base_mask) >> base_shift);
      
      p[Order_B] = (value_type)(((sbda + dbsa <= sada) ? 
        sb * d1a + db * s1a :
      sa * (sbda + dbsa - sada) / sb + sb * d1a + db * s1a + base_mask) >> base_shift);
      
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}


//comp_op_rgba_hard_light
// if 2.Sca < Sa
//    Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
// otherwise
//    Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
// 
// Da'  = Sa + Da - Sa.Da
CC_INLINE void comp_op_rgba_hard_light(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type d1a  = base_mask - p[Order_A];
      calc_type s1a  = base_mask - sa;
      calc_type dr   = p[Order_R];
      calc_type dg   = p[Order_G];
      calc_type db   = p[Order_B];
      calc_type da   = p[Order_A];
      calc_type sada = sa * da;
      
      p[Order_R] = (value_type)(((2*sr < sa) ? 
        2*sr*dr + sr*d1a + dr*s1a : 
      sada - 2*(da - dr)*(sa - sr) + sr*d1a + dr*s1a + base_mask) >> base_shift);
      
      p[Order_G] = (value_type)(((2*sg < sa) ? 
        2*sg*dg + sg*d1a + dg*s1a : 
      sada - 2*(da - dg)*(sa - sg) + sg*d1a + dg*s1a + base_mask) >> base_shift);
      
      p[Order_B] = (value_type)(((2*sb < sa) ? 
        2*sb*db + sb*d1a + db*s1a : 
      sada - 2*(da - db)*(sa - sb) + sb*d1a + db*s1a + base_mask) >> base_shift);
      
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
};

//comp_op_rgba_soft_light
// if 2.Sca < Sa
//   Dca' = Dca.(Sa + (1 - Dca/Da).(2.Sca - Sa)) + Sca.(1 - Da) + Dca.(1 - Sa)
// otherwise if 8.Dca <= Da
//   Dca' = Dca.(Sa + (1 - Dca/Da).(2.Sca - Sa).(3 - 8.Dca/Da)) + Sca.(1 - Da) + Dca.(1 - Sa)
// otherwise
//   Dca' = (Dca.Sa + ((Dca/Da)^(0.5).Da - Dca).(2.Sca - Sa)) + Sca.(1 - Da) + Dca.(1 - Sa)
// 
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_soft_light(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    uchar r = s[Order_R], g = s[Order_G], b = s[Order_B], a = s[Order_A];
    
    double sr = (double)(r * cover) / (base_mask * 255);
    double sg = (double)(g * cover) / (base_mask * 255);
    double sb = (double)(b * cover) / (base_mask * 255);
    double sa = (double)(a * cover) / (base_mask * 255);
    if(sa > 0)
    {
      double dr = (double)(p[Order_R]) / base_mask;
      double dg = (double)(p[Order_G]) / base_mask;
      double db = (double)(p[Order_B]) / base_mask;
      double da = (double)(p[Order_A] ? p[Order_A] : 1) / base_mask;
      if(cover < 255)
      {
        a = (a * cover + 255) >> 8;
      }
      
      if(2*sr < sa)       dr = dr*(sa + (1 - dr/da)*(2*sr - sa)) + sr*(1 - da) + dr*(1 - sa);
      else if(8*dr <= da) dr = dr*(sa + (1 - dr/da)*(2*sr - sa)*(3 - 8*dr/da)) + sr*(1 - da) + dr*(1 - sa);
      else                dr = (dr*sa + (sqrt(dr/da)*da - dr)*(2*sr - sa)) + sr*(1 - da) + dr*(1 - sa);
      
      if(2*sg < sa)       dg = dg*(sa + (1 - dg/da)*(2*sg - sa)) + sg*(1 - da) + dg*(1 - sa);
      else if(8*dg <= da) dg = dg*(sa + (1 - dg/da)*(2*sg - sa)*(3 - 8*dg/da)) + sg*(1 - da) + dg*(1 - sa);
      else                dg = (dg*sa + (sqrt(dg/da)*da - dg)*(2*sg - sa)) + sg*(1 - da) + dg*(1 - sa);
      
      if(2*sb < sa)       db = db*(sa + (1 - db/da)*(2*sb - sa)) + sb*(1 - da) + db*(1 - sa);
      else if(8*db <= da) db = db*(sa + (1 - db/da)*(2*sb - sa)*(3 - 8*db/da)) + sb*(1 - da) + db*(1 - sa);
      else                db = (db*sa + (sqrt(db/da)*da - db)*(2*sb - sa)) + sb*(1 - da) + db*(1 - sa);
      
      p[Order_R] = (value_type)uround(dr * base_mask);
      p[Order_G] = (value_type)uround(dg * base_mask);
      p[Order_B] = (value_type)uround(db * base_mask);
      p[Order_A] = (value_type)(a + p[Order_A] - ((a * p[Order_A] + base_mask) >> base_shift));
    }
  }
}

//comp_op_rgba_difference
// Dca' = Sca + Dca - 2.min(Sca.Da, Dca.Sa)
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_difference(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type dr = p[Order_R];
      calc_type dg = p[Order_G];
      calc_type db = p[Order_B];
      calc_type da = p[Order_A];
      p[Order_R] = (value_type)(sr + dr - ((2 * sd_min(sr*da, dr*sa) + base_mask) >> base_shift));
      p[Order_G] = (value_type)(sg + dg - ((2 * sd_min(sg*da, dg*sa) + base_mask) >> base_shift));
      p[Order_B] = (value_type)(sb + db - ((2 * sd_min(sb*da, db*sa) + base_mask) >> base_shift));
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}

//comp_op_rgba_exclusion
// Dca' = (Sca.Da + Dca.Sa - 2.Sca.Dca) + Sca.(1 - Da) + Dca.(1 - Sa)
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_exclusion(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type d1a = base_mask - p[Order_A];
      calc_type s1a = base_mask - sa;
      calc_type dr = p[Order_R];
      calc_type dg = p[Order_G];
      calc_type db = p[Order_B];
      calc_type da = p[Order_A];
      p[Order_R] = (value_type)((sr*da + dr*sa - 2*sr*dr + sr*d1a + dr*s1a + base_mask) >> base_shift);
      p[Order_G] = (value_type)((sg*da + dg*sa - 2*sg*dg + sg*d1a + dg*s1a + base_mask) >> base_shift);
      p[Order_B] = (value_type)((sb*da + db*sa - 2*sb*db + sb*d1a + db*s1a + base_mask) >> base_shift);
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}

//comp_op_rgba_contrast
CC_INLINE void comp_op_rgba_contrast(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    {long_type dr = p[Order_R];
    long_type dg = p[Order_G];
    long_type db = p[Order_B];
    int       da = p[Order_A];
    long_type d2a = da >> 1;
    unsigned s2a = sa >> 1;
    
    int r = (int)((((dr - d2a) * (int)((sr - s2a)*2 + base_mask)) >> base_shift) + d2a); 
    int g = (int)((((dg - d2a) * (int)((sg - s2a)*2 + base_mask)) >> base_shift) + d2a); 
    int b = (int)((((db - d2a) * (int)((sb - s2a)*2 + base_mask)) >> base_shift) + d2a); 
    
    r = (r < 0) ? 0 : r;
    g = (g < 0) ? 0 : g;
    b = (b < 0) ? 0 : b;
    
    p[Order_R] = (value_type)((r > da) ? da : r);
    p[Order_G] = (value_type)((g > da) ? da : g);
    p[Order_B] = (value_type)((b > da) ? da : b);}
  }
}

//comp_op_rgba_invert
// Dca' = (Da - Dca) * Sa + Dca.(1 - Sa)
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_invert(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    sa = (sa * cover + 255) >> 8;
    if(sa)
    {
      calc_type da = p[Order_A];
      calc_type dr = ((da - p[Order_R]) * sa + base_mask) >> base_shift;
      calc_type dg = ((da - p[Order_G]) * sa + base_mask) >> base_shift;
      calc_type db = ((da - p[Order_B]) * sa + base_mask) >> base_shift;
      calc_type s1a = base_mask - sa;
      p[Order_R] = (value_type)(dr + ((p[Order_R] * s1a + base_mask) >> base_shift));
      p[Order_G] = (value_type)(dg + ((p[Order_G] * s1a + base_mask) >> base_shift));
      p[Order_B] = (value_type)(db + ((p[Order_B] * s1a + base_mask) >> base_shift));
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}

//comp_op_rgba_invert_rgb
// Dca' = (Da - Dca) * Sca + Dca.(1 - Sa)
// Da'  = Sa + Da - Sa.Da 
CC_INLINE void comp_op_rgba_invert_rgb(uchar* p, const uchar* s, const uchar* covers, unsigned width)
{
  typedef uchar value_type;
  for ( ;width--; p+=4, s+=4) {
    uchar cover = *covers++;
    unsigned sr = s[Order_R], sg = s[Order_G], sb = s[Order_B], sa = s[Order_A];
    
    if(cover < 255)
    {
      sr = (sr * cover + 255) >> 8;
      sg = (sg * cover + 255) >> 8;
      sb = (sb * cover + 255) >> 8;
      sa = (sa * cover + 255) >> 8;
    }
    if(sa)
    {
      calc_type da = p[Order_A];
      calc_type dr = ((da - p[Order_R]) * sr + base_mask) >> base_shift;
      calc_type dg = ((da - p[Order_G]) * sg + base_mask) >> base_shift;
      calc_type db = ((da - p[Order_B]) * sb + base_mask) >> base_shift;
      calc_type s1a = base_mask - sa;
      p[Order_R] = (value_type)(dr + ((p[Order_R] * s1a + base_mask) >> base_shift));
      p[Order_G] = (value_type)(dg + ((p[Order_G] * s1a + base_mask) >> base_shift));
      p[Order_B] = (value_type)(db + ((p[Order_B] * s1a + base_mask) >> base_shift));
      p[Order_A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
    }
  }
}

#define COMP_OP_RGBA_DEF_DEF(COMP_OP_RGBA_DEF) \
  COMP_OP_RGBA_DEF(clear) \
  COMP_OP_RGBA_DEF(src) \
  COMP_OP_RGBA_DEF(dst) \
  COMP_OP_RGBA_DEF(src_over) \
  COMP_OP_RGBA_DEF(dst_over) \
  COMP_OP_RGBA_DEF(src_in) \
  COMP_OP_RGBA_DEF(dst_in) \
  COMP_OP_RGBA_DEF(src_out) \
  COMP_OP_RGBA_DEF(dst_out) \
  COMP_OP_RGBA_DEF(src_atop) \
  COMP_OP_RGBA_DEF(dst_atop) \
  COMP_OP_RGBA_DEF(xor) \
  COMP_OP_RGBA_DEF(plus) \
  COMP_OP_RGBA_DEF(minus) \
  COMP_OP_RGBA_DEF(multiply) \
  COMP_OP_RGBA_DEF(screen) \
  COMP_OP_RGBA_DEF(overlay) \
  COMP_OP_RGBA_DEF(darken) \
  COMP_OP_RGBA_DEF(lighten) \
  COMP_OP_RGBA_DEF(color_dodge) \
  COMP_OP_RGBA_DEF(color_burn) \
  COMP_OP_RGBA_DEF(hard_light) \
  COMP_OP_RGBA_DEF(soft_light) \
  COMP_OP_RGBA_DEF(difference) \
  COMP_OP_RGBA_DEF(exclusion) \
  COMP_OP_RGBA_DEF(contrast) \
  COMP_OP_RGBA_DEF(invert) \
COMP_OP_RGBA_DEF(invert_rgb)

//comp_op_e
typedef enum {
#define COMP_OP_RGBA_DEF(name) comp_op_ ## name, 
  COMP_OP_RGBA_DEF_DEF(COMP_OP_RGBA_DEF)
#undef COMP_OP_RGBA_DEF
    end_of_comp_op_e,
} comp_op_e;
//g_comp_op_func

//comp_op_table_rgba
CC_INLINE void comp_op_table_rgba(uchar* p, const uchar* s, const uchar* covers, unsigned width, comp_op_e op) {
  typedef void (*comp_op_func_type)(uchar* p, const uchar* s, const uchar* covers, unsigned width);
  static comp_op_func_type g_comp_op_func[] = {
#define COMP_OP_RGBA_DEF(name) comp_op_rgba_ ## name,
    COMP_OP_RGBA_DEF_DEF(COMP_OP_RGBA_DEF)
#undef COMP_OP_RGBA_DEF
  };
  g_comp_op_func[op](p, s, covers, width);
};


#if 0

//comp_op_adaptor_rgba
template<class ColorT, class Order> struct comp_op_adaptor_rgba
{
  typedef Order  order_type;
  typedef ColorT color_type;
  typedef typename value_type value_type;
  enum base_scale_e
  {  
    base_shift = base_shift,
      base_mask  = base_mask 
  };
  
  CC_INLINE void blend_pix(unsigned op, value_type* p, 
    const uchar* s, unsigned width,
    unsigned ca,
    unsigned cover)
  {
    comp_op_table_rgba<ColorT, Order>::g_comp_op_func[op]
      (p, (s[Order_R] * ca + base_mask) >> base_shift, 
      (s[Order_G] * ca + base_mask) >> base_shift,
      (s[Order_B] * ca + base_mask) >> base_shift,
      ca, cover);
  }
};

//comp_op_adaptor_clip_to_dst_rgba
template<class ColorT, class Order> struct comp_op_adaptor_clip_to_dst_rgba
{
  typedef Order  order_type;
  typedef ColorT color_type;
  typedef typename value_type value_type;
  enum base_scale_e
  {  
    base_shift = base_shift,
      base_mask  = base_mask 
  };
  
  CC_INLINE void blend_pix(unsigned op, value_type* p, 
    const uchar* s, unsigned width,
    unsigned ca,
    unsigned cover)
  {
    s[Order_R] = (s[Order_R] * ca + base_mask) >> base_shift;
    s[Order_G] = (s[Order_G] * ca + base_mask) >> base_shift;
    s[Order_B] = (s[Order_B] * ca + base_mask) >> base_shift;
    unsigned da = p[Order_A];
    comp_op_table_rgba<ColorT, Order>::g_comp_op_func[op]
      (p, (s[Order_R] * da + base_mask) >> base_shift, 
      (s[Order_G] * da + base_mask) >> base_shift, 
      (s[Order_B] * da + base_mask) >> base_shift, 
      (ca * da + base_mask) >> base_shift, 
      cover);
  }
};

//comp_op_adaptor_rgba_pre
template<class ColorT, class Order> struct comp_op_adaptor_rgba_pre
{
  typedef Order  order_type;
  typedef ColorT color_type;
  typedef typename value_type value_type;
  enum base_scale_e
  {  
    base_shift = base_shift,
      base_mask  = base_mask 
  };
  
  CC_INLINE void blend_pix(unsigned op, value_type* p, 
    const uchar* s, unsigned width,
    unsigned ca,
    unsigned cover)
  {
    comp_op_table_rgba<ColorT, Order>::g_comp_op_func[op](p, s[Order_R], s[Order_G], s[Order_B], ca, cover);
  }
};

//comp_op_adaptor_clip_to_dst_rgba_pre
template<class ColorT, class Order> struct comp_op_adaptor_clip_to_dst_rgba_pre
{
  typedef Order  order_type;
  typedef ColorT color_type;
  typedef typename value_type value_type;
  enum base_scale_e
  {  
    base_shift = base_shift,
      base_mask  = base_mask 
  };
  
  CC_INLINE void blend_pix(unsigned op, value_type* p, 
    const uchar* s, unsigned width,
    unsigned ca,
    unsigned cover)
  {
    unsigned da = p[Order_A];
    comp_op_table_rgba<ColorT, Order>::g_comp_op_func[op]
      (p, (s[Order_R] * da + base_mask) >> base_shift, 
      (s[Order_G] * da + base_mask) >> base_shift, 
      (s[Order_B] * da + base_mask) >> base_shift, 
      (ca * da + base_mask) >> base_shift, 
      cover);
  }
};

//comp_adaptor_rgba
template<class BlenderPre> struct comp_adaptor_rgba
{
  typedef typename BlenderPre::order_type order_type;
  typedef typename BlenderPre::color_type color_type;
  typedef typename value_type value_type;
  enum base_scale_e
  {  
    base_shift = base_shift,
      base_mask  = base_mask 
  };
  
  CC_INLINE void blend_pix(unsigned op, value_type* p, 
    const uchar* s, unsigned width,
    unsigned ca,
    unsigned cover)
  {
    BlenderPre::blend_pix(p, 
      (s[Order_R] * ca + base_mask) >> base_shift, 
      (s[Order_G] * ca + base_mask) >> base_shift,
      (s[Order_B] * ca + base_mask) >> base_shift,
      ca, cover);
  }
};

//comp_adaptor_clip_to_dst_rgba
template<class BlenderPre> struct comp_adaptor_clip_to_dst_rgba
{
  typedef typename BlenderPre::order_type order_type;
  typedef typename BlenderPre::color_type color_type;
  typedef typename value_type value_type;
  enum base_scale_e
  {  
    base_shift = base_shift,
      base_mask  = base_mask 
  };
  
  CC_INLINE void blend_pix(unsigned op, value_type* p, 
    const uchar* s, unsigned width,
    unsigned ca,
    unsigned cover)
  {
    s[Order_R] = (s[Order_R] * ca + base_mask) >> base_shift;
    s[Order_G] = (s[Order_G] * ca + base_mask) >> base_shift;
    s[Order_B] = (s[Order_B] * ca + base_mask) >> base_shift;
    unsigned da = p[order_type::A];
    BlenderPre::blend_pix(p, 
      (s[Order_R] * da + base_mask) >> base_shift, 
      (s[Order_G] * da + base_mask) >> base_shift, 
      (s[Order_B] * da + base_mask) >> base_shift, 
      (ca * da + base_mask) >> base_shift, 
      cover);
  }
};

//comp_adaptor_clip_to_dst_rgba_pre
template<class BlenderPre> struct comp_adaptor_clip_to_dst_rgba_pre
{
  typedef typename BlenderPre::order_type order_type;
  typedef typename BlenderPre::color_type color_type;
  typedef typename value_type value_type;
  enum base_scale_e
  {  
    base_shift = base_shift,
      base_mask  = base_mask 
  };
  
  CC_INLINE void blend_pix(unsigned op, value_type* p, 
    const uchar* s, unsigned width,
    unsigned ca,
    unsigned cover)
  {
    unsigned da = p[order_type::A];
    BlenderPre::blend_pix(p, 
      (s[Order_R] * da + base_mask) >> base_shift, 
      (s[Order_G] * da + base_mask) >> base_shift, 
      (s[Order_B] * da + base_mask) >> base_shift, 
      (ca * da + base_mask) >> base_shift, 
      cover);
  }
};

#endif

void gamma_power(uchar* m_gamma, double gamma)
{ 
  int i;
  for(i = 0; i < aa_scale; i++) {
    double y =  pow((double)i / aa_mask, gamma);
    m_gamma[i] = uround(y * aa_mask);
  }
}

void gamma_threshold(uchar* m_gamma, double m_threshold)
{ 
  int i;
  for(i = 0; i < aa_scale; i++) {
    double y, x = (double)i / aa_mask;
    y = (x < m_threshold) ? 0.0 : 1.0;
    m_gamma[i] = uround(y * aa_mask);
  }
}

void gamma_linear(uchar* m_gamma, double m_start, double m_end)
{ 
  int i;
  for(i = 0; i < aa_scale; i++) {
    double y, x = (double)i / aa_mask;
    if(x < m_start) {
      y = 0.0;
    } else if(x > m_end) {
      y = 1.0;
    } else {
      y = (x - m_start) / (m_end - m_start);
    }
    m_gamma[i] = uround(y * aa_mask);
  }
}

void gamma_multiply(uchar* m_gamma, double m_mul)
{ 
  int i;
  for(i = 0; i < aa_scale; i++) {
    double y, x = (double)i / aa_mask;
    y = x * m_mul;
    if(y > 1.0) y = 1.0;
    m_gamma[i] = uround(y * aa_mask);
  }
}

void gamma_set(uchar* m_dir_gamma, uchar* m_inv_gamma, double g) 
{
  enum {
    GammaShift = 8,
      HiResShift = 8,
      gamma_shift = GammaShift,
      gamma_size  = 1 << gamma_shift,
      gamma_mask  = gamma_size - 1,
      hi_res_shift = HiResShift,
      hi_res_size  = 1 << hi_res_shift,
      hi_res_mask  = hi_res_size - 1,
  };
  unsigned i;
  typedef uchar HiResT;
  typedef uchar LoResT;
  double m_gamma = g, inv_g;
  for(i = 0; i < gamma_size; i++)
  {
    m_dir_gamma[i] = (HiResT)
      uround(pow(i / (double)(gamma_mask), m_gamma) * (double)(hi_res_mask));
  }
  
  inv_g = 1.0 / g;
  for(i = 0; i < hi_res_size; i++)
  {
    m_inv_gamma[i] = (LoResT)
      uround(pow(i / (double)(hi_res_mask), inv_g) * (double)(gamma_mask));
  }
}

#endif // _BLEND_INL_
///////////////////////////////////////////////////
#define BRUSH_FUN_TABLE(name) br->fillline = name##_fillline
#define PIXBLENDA1(dst, _a) (dst[0] = (dst[0] * _a)/255)
int solid_fillline(const brush_t* br, COLOR* pix, int x, int e, int y)
{
  COLOR clr = br->solid.clr;
  for (; x < e; ++x, ++pix) {
    *pix = clr;
  }
  return 0;
}
int brush_set_solid(brush_t* br, COLOR clr)
{
  br->solid.clr = clr;
  BRUSH_FUN_TABLE(solid);
  return 0;
}

static const uchar g_hatch_datas[][8] = {
#define HATCHSTYLE_DEF(data0, data1, data2, data3, data4, data5, data6, data7, name, str) {data0, data1, data2, data3, data4, data5, data6, data7},
  HATCHSTYLEDEF_DEF(HATCHSTYLE_DEF)
#undef HATCHSTYLE_DEF
    };
int hatch_fillline(const brush_t* br, COLOR* pix, int b, int e, int y)
{
  int x;
  int hy = (y + br->hatch.fill_area_y) & 7;
  int datay = br->hatch.hatch_data[7 - hy];
  for (x = b; x < e; ++x, ++pix) {
    int hx = (x + br->hatch.fill_area_x) & 7;
    *pix = br->hatch.clrs[(datay & (0x80 >> hx)) != 0];
  }
  return 0;
}
//#define brush_set_hatch2(br, fill_area_x, fill_area_y, forecol, backcol, hatchstyle)  brush_set_hatch(br, fill_area_x, fill_area_y, forecol, backcol, g_hatch_datas[hatchstyle])
int brush_set_hatch(brush_t* br, int fill_area_x, int fill_area_y, COLOR forecol, COLOR backcol, const uchar* hatch_data)
{
  br->hatch.clrs[1] = forecol;
  br->hatch.clrs[0] = backcol;
  br->hatch.fill_area_x = fill_area_x;
  br->hatch.fill_area_y = fill_area_y;
  br->hatch.hatch_data = hatch_data;
  br->hatch.datay = 0;
  BRUSH_FUN_TABLE(hatch);
  return 0;
}
int lut_gradient_fillline(const brush_t* br, COLOR* pix, int b, int e, int y)
{
  int x, d=0;
  int ix = br->lut_gradient.ix;
  int datay;
  const COLOR* clrs = br->lut_gradient.clrs;
  datay = y * br->lut_gradient.iy + br->lut_gradient.it;
  for (x = b; x < e; ++x, ++pix) {
    d = (x * ix + datay) >> 8;
    d = WrapPix1D(d, 256, br->wrapX);
    if (d>=0 && d<256) {
      *pix = clrs[d];
    }
  }
  return 0;
}
int brush_set_lut_gradient(brush_t* br, double x1, double y1, double x2, double y2, const COLOR* clrs)
{
  double ds = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))/256;
  double dx = (x2 - x1) / ds;
  double dy = (y2 - y1) / ds;
  //int ix, iy;
  //it = br->lut_gradient.it = -(int)((x1 * dx + y1 * dy) * 256);
  //ix = br->lut_gradient.ix = (int)(dx * 256);
  //iy = br->lut_gradient.iy = (int)(dy * 256);
  //int n1 = MAX(1, (int)(ds + 0.0001));
  if (br->lut_gradient.clrs != clrs) {
    MEMCPY(br->lut_gradient.clrs, clrs, 256);
  }
  BRUSH_FUN_TABLE(lut_gradient);
  return 0;
}

int brush_set_lut_gradient2(brush_t* br, double x1, double y1, double x2, double y2, const COLOR* clrs)
{
  double dx = (x2 - x1);
  double dy = (y2 - y1);
  double dis = (dx*dx+dy*dy)/256;
  double invdis = 1./dis;
  double A = (dx) * invdis;
  double B = (dy) * invdis;
  double C  = -x1 * A - y1 * B;
  //int it, ix, iy;
  //it = br->lut_gradient.it = (int)(C * 256);
  //ix = br->lut_gradient.ix = (int)(A * 256);
  //iy = br->lut_gradient.iy = (int)(B * 256);
  if (br->lut_gradient.clrs != clrs) {
    MEMCPY(br->lut_gradient.clrs, clrs, 256);
  }
  BRUSH_FUN_TABLE(lut_gradient);
  return 0;
}

CC_INLINE int brush_set_gradient(brush_t* br, double x1, double y1, double x2, double y2, COLOR clr1, COLOR clr2)
{
  COLOR* clrs = br->lut_gradient.clrs;
  fill_color_array(256, clrs, clr1, clr2);
  brush_set_lut_gradient(br, x1, y1, x2, y2, clrs);
  return 0;
}
CC_INLINE int brush_set_gradient2(brush_t* br, double x1, double y1, double x2, double y2, int m, const COLOR* clrs1, const float* pos)
{
  COLOR* clrs = br->lut_gradient.clrs;
  fill_color_array2(256, clrs, m, clrs1, pos);
  brush_set_lut_gradient(br, x1, y1, x2, y2, clrs);
  return 0;
}
int colormatrix_fillline(const brush_t* br, COLOR* pix, int x, int e, int y)
{
  const int* iA = br->colormatrix.iA;
  int ry, gy, by, ay;
  ry = iA[1] * y + iA[2];
  gy = iA[4] * y + iA[5];
  by = iA[7] * y + iA[8];
  ay = iA[10] * y + iA[11];
  for (; x < e; ++x, ++pix) {
    int r = (iA[0] * x + ry) >> 8;
    int g = (iA[3] * x + gy) >> 8;
    int b = (iA[6] * x + by) >> 8;
    int a = (iA[9] * x + ay) >> 8;
    r = BOUND(r, 0, 255);
    g = BOUND(g, 0, 255);
    b = BOUND(b, 0, 255);
    a = BOUND(a, 0, 255);
    *pix = _RGBA(r, g, b, a);
  }
  return 0;
}
CC_INLINE int brush_set_colormatrix(brush_t* br, const double* x)
{
  int* iA = br->colormatrix.iA;
  int i;
  for (i = 0; i < 12; ++i) {
    iA[i] = F2FIXN(x[i], 8);
  }
  BRUSH_FUN_TABLE(colormatrix);
  return 0;
}

CC_INLINE int brush_set_gouraud(brush_t* br, double x1, double y1, double x2, double y2, double x3, double y3, COLOR c1, COLOR c2, COLOR c3)
{
  double x[12], A[9] = {0}, b[12];
  V9SET(A, x1, x2, x3, y1, y2, y3, 1, 1, 1);
  V12SET(b, GetRV(c1), GetRV(c2), GetRV(c3), GetGV(c1), GetGV(c2), GetGV(c3), GetBV(c1), GetBV(c2), GetBV(c3), GetAV(c1), GetAV(c2), GetAV(c3));
  slove3n(A, b, x, 4);
  brush_set_colormatrix(br, x);
  return 0;
}
int lut_radial_fillline(const brush_t* br, COLOR* pix, int x, int e, int y)
{
  int d, ix1 = br->lut_radial.ix, iy1 = br->lut_radial.iy, invrx = br->lut_radial.invrx, invry = br->lut_radial.invry;
  for (; x < e; ++x, ++pix) {
    d = fixmag(((x)-ix1)*invrx, ((y)-iy1)*invry)>>8;
    d = WrapPix1D(d, 256, br->wrapX);
    //d = BOUND(d, 0, 255);
    *pix = br->lut_radial.clrs[d];
  }
  return 0;
}
CC_INLINE int brush_set_lut_radial(brush_t* br, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  int ix1 = F2FIXN(x1, 0), iy1 = F2FIXN(y1, 0), invrx = F2FIXN(1 / rx, 16), invry = F2FIXN(1 / ry, 16);
  br->lut_radial.ix = ix1, br->lut_radial.iy = iy1, br->lut_radial.invrx = invrx, br->lut_radial.invry = invry;
  if (clrs!=br->lut_radial.clrs) {
    MEMCPY(br->lut_radial.clrs, clrs, 256);
  }
  BRUSH_FUN_TABLE(lut_radial);
  return 0;
}
CC_INLINE int brush_set_radial2(brush_t* br, double x1, double y1, double x2, double y2, int m, const COLOR* clrs1, const float* pos)
{
  COLOR* clrs = br->lut_gradient.clrs;
  fill_color_array2(256, clrs, m, clrs1, pos);
  brush_set_lut_radial(br, x1, y1, x2, y2, clrs);
  return 0;
}
CC_INLINE int brush_set_radial(brush_t* br, double x1, double y1, double rx, double ry, COLOR clr1, COLOR clr2)
{
  fill_color_array(256, br->lut_radial.clrs, clr1, clr2);
  brush_set_lut_radial(br, x1, y1, rx, ry, br->lut_radial.clrs);
  return 0;
}
FPOINT line_point(float x1, float y1, float x2, float y2, float t) {
  FPOINT pt;
  float r = 1-t;
  pt.x = r*x1 + t*x2;
  pt.y = r*y1 + t*y2;
  return pt;
}
int path_radial_fillline(const brush_t* br, COLOR* pix, int x, int e, int y)
{
  int d, ix1 = br->path_radial.ix, iy1 = br->path_radial.iy, a1;
  const int* invr = br->path_gradient.invr;
  for (; x < e; ++x, ++pix) {
    a1 = fixatan256(y-iy1, x-ix1);
    d = fixmag(((x)-ix1)*invr[a1], ((y)-iy1)*invr[a1])>>8;
    d = WrapPix1D(d, 256, br->wrapX);
    *pix = br->path_radial.clrs[d];
  }
  return 0;
}
CC_INLINE int brush_set_path_radial(brush_t* br, const FPOINT* Points, const BYTE* Types, int Count, const COLOR* clrs, const FPOINT* ptcenter) {
  int i, a1;
  float cx, cy;
  FPOINT p1 = Points[Count-1];
  int* invr = br->path_radial.invr;
  if (ptcenter) {
    cx = ptcenter->x;
    cy = ptcenter->y;
  } else {
    cx = 0, cy = 0;
    for (i=0; i<Count; ++i) {
      cx += Points[i].x;
      cy += Points[i].y;
    }
    cx/=Count;
    cy/=Count;
  }
  a1 = fixatan256((int)(p1.y-cy), (int)(p1.x-cx));
  for (i=0; i<Count; ++i) {
    FPOINT p = Points[i];
    int a = fixatan256((int)(p.y-cy), (int)(p.x-cx));
    int j, d=1, n = a-a1;
    if (n) {
      float t = 0, nt;
      if (n<0) {
        n = -n;
        d = -d;
      }
      if (n>128) {
        n = 256-n;
        d = -d;
      }
      nt = 1./n;
      for (j=0;j<n;++j) {
        FPOINT p2 = line_point(p1.x, p1.y, p.x, p.y, j*nt);
        double r = calc_distance(p2.x, p2.y, cx, cy);
        int k = (a1+d*j)&0xff;
        invr[k] = F2FIXN(1. / r, 16);
      }
    }
    a1 = a;
    p1 = p;
  }
  br->path_radial.ix = (int)cx;
  br->path_radial.iy = (int)cy;
  if (clrs!=br->path_radial.clrs) {
    MEMCPY(br->path_radial.clrs, clrs, 256);
  }
  BRUSH_FUN_TABLE(path_radial);
  return 0;
}
CC_INLINE int brush_set_path_radial2(brush_t* br, const FPOINT* Points, const BYTE* Types, int Count, int nclrs, const COLOR* clrs, const float* pos, const FPOINT* ptcenter) {
  fill_color_array2(256, br->path_radial.clrs, nclrs, clrs, pos);
  brush_set_path_radial(br, Points, Types, Count, br->path_radial.clrs, ptcenter);
  return 0;
}
CC_INLINE int brush_set_path_radial3(brush_t* br, const FPOINT* Points, const BYTE* Types, int Count, COLOR clr1, COLOR clr2, const FPOINT* ptcenter) {
  fill_color_array(256, br->path_radial.clrs, clr1, clr2);
  brush_set_path_radial(br, Points, Types, Count, br->path_radial.clrs, ptcenter);
  return 0;
}
int path_gradient_fillline(const brush_t* br, COLOR* pix, int x, int e, int y)
{
  int d, ix1 = br->path_gradient.ix, iy1 = br->path_gradient.iy, a1;
  COLOR clr1 = br->path_gradient.clrcenter;
  const int* invr = br->path_gradient.invr;
  const COLOR* clrs = br->path_gradient.clrs;
  for (; x < e; ++x, ++pix) {
    a1 = fixatan256(y-iy1, x-ix1);
    d = fixmag(((x)-ix1)*invr[a1], ((y)-iy1)*invr[a1])>>8;
    d = WrapPix1D(d, 256, br->wrapX);
    *pix = RGBABLEN2(clr1, clrs[a1], d, 255);
  }
  return 0;
}
CC_INLINE int brush_set_path_gradient(brush_t* br, const FPOINT* Points, const BYTE* Types, int Count, const COLOR* colors, int nclrs, const FPOINT* ptcenter, COLOR clrcenter)
{
  double minr = 100000000;
  int i=0, a1;
  COLOR colors1[100], c1;
  FPOINT p1 = Points[Count-1];
  double cx, cy;
  int* invr = br->path_gradient.invr;
  COLOR* clrs = br->path_gradient.clrs;
  if (ptcenter) {
    cx = ptcenter->x;
    cy = ptcenter->y;
  } else {
    cx = 0, cy = 0;
    for (i=0; i<Count; ++i) {
      cx += Points[i].x;
      cy += Points[i].y;
    }
    cx/=Count;
    cy/=Count;
  }
  ASSERT(Count<100);
  if (1==nclrs) {
    for (i=0; i<Count; ++i) {
      colors1[i] = colors[0];
    }
  } else if (nclrs==Count) {
    for (i=0; i<Count; ++i) {
      colors1[i] = colors[i];
    }
  } else if (nclrs<Count) {
    for (i=0; i<Count; ++i) {
      if (i<nclrs) {
        c1 = colors[i];
      }
      colors1[i] = c1;
    }
    fill_color_array2(Count, colors1, nclrs, colors, NULL);
  } else {
    for (i=0; i<Count; ++i) {
      colors1[i] = colors[i*nclrs/Count];
    }
  }
  c1 = colors[Count-1];
  a1 = fixatan256((int)(p1.y-cy), (int)(p1.x-cx));
  for (i=0; i<Count; ++i) {
    FPOINT p = Points[i];
    COLOR c = colors[i];
    int a = fixatan256((int)(p.y-cy), (int)(p.x-cx));
    int j, d=1, n = a-a1;
    if (n) {
      float t = 0, nt;
      if (n<0) {
        n = -n;
        d = -d;
      }
      if (n>128) {
        n = 256-n;
        d = -d;
      }
      nt = 1./n;
      for (j=0;j<n;++j) {
        FPOINT p2 = line_point(p1.x, p1.y, p.x, p.y, j*nt);
        double r = calc_distance(p2.x, p2.y, cx, cy);
        int k = (a1+d*j)&0xff;
        invr[k] = F2FIXN(1. / r, 16);
        clrs[k] = RGBABLEN2(c1, c, j, (n));
      }
    }
    a1 = a;
    c1 = c;
    p1 = p;
  }
  br->path_gradient.clrcenter = clrcenter;
  br->path_gradient.ix = (int)cx;
  br->path_gradient.iy = (int)cy;
  BRUSH_FUN_TABLE(path_gradient);
  return 0;
}
CC_INLINE int brush_set_lut_swirled(brush_t* br, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(rx, 8), invry = F2FIXN(ry, 8);
  br->lut_swirled.ix = ix1, br->lut_swirled.iy = iy1, br->lut_swirled.invrx = invrx, br->lut_swirled.invry = invry;
  if (clrs!=br->lut_swirled.clrs) {
    MEMCPY(br->lut_swirled.clrs, clrs, 256);
  }
  return 0;
}
CC_INLINE int brush_set_lut_charicature(brush_t* br, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(rx, 8), invry = F2FIXN(ry, 8);
  br->lut_swirled.ix = ix1, br->lut_swirled.iy = iy1, br->lut_swirled.invrx = invrx, br->lut_swirled.invry = invry;
  if (clrs!=br->lut_swirled.clrs) {
    MEMCPY(br->lut_swirled.clrs, clrs, 256);
  }
  return 0;
}
CC_INLINE int brush_set_lut_diamond(brush_t* br, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(1 / rx, 8), invry = F2FIXN(1 / ry, 8);
  br->lut_swirled.ix = ix1, br->lut_swirled.iy = iy1, br->lut_swirled.invrx = invrx, br->lut_swirled.invry = invry;
  if (clrs!=br->lut_swirled.clrs) {
    MEMCPY(br->lut_swirled.clrs, clrs, 256);
  }
  return 0;
}
static double m23_det(const double* matrix)
{
  return matrix[0] * matrix[3] - matrix[1] * matrix[2];
}
double m23_invert(double* matrix)
{
  double copy[6];
  double det;
  det = m23_det(matrix);
  if (fabs(det) >= 1e-5) {
    MEMCPY(copy, matrix, 6);
    /* store result */
    matrix[0] = copy[3] / det;
    matrix[1] = -copy[1] / det;
    matrix[2] = -copy[2] / det;
    matrix[3] = copy[0] / det;
    matrix[4] = (copy[2] * copy[5] - copy[3] * copy[4]) / det;
    matrix[5] = -(copy[0] * copy[5] - copy[1] * copy[4]) / det;
    return 1;
  }
  return 0;
}
#include "pixel.inl"
int texture_fillline(const brush_t* br, COLOR* pix, int b, int e, int y)
{
  int x;
  const int* im23 = br->texture.im23;
  COLOR bgclr = br->texture.bgclr;
  WrapMode wrapX = br->wrapX;
  WrapMode wrapY = br->wrapX;
  const texture_t* tex = br->texture.tex;
  int yx = y * im23[2] + im23[4];
  int yy = y * im23[3] + im23[5];
  for (x = b; x < e; ++x, ++pix) {
    int xsrc = (x * im23[0] + yx) >> 16;
    int ysrc = (x * im23[1] + yy) >> 16;
    *pix = bitmap_getpixel(tex, &br->texture.rcSrc, xsrc, ysrc, br->wrapX, br->wrapY, bgclr, br->texture.palette);
  }
  return 0;
}

CC_INLINE BOOL brush_set_texture(brush_t* br, const texture_t* tex, const IRECT* prcSrc, COLOR bgclr, const COLOR* palette, const float* m23)
{
  double m23_[] = {1.f, 0, 0, 1.f, 0, 0};
  IRECT rcSrc_ = {0, 0, tex->w, tex->h};
  int* im23 = br->texture.im23;
  int i;
  BRUSH_FUN_TABLE(texture);
  prcSrc = prcSrc ? prcSrc : &rcSrc_;
  if (m23) {
    for (i = 0; i < 6; ++i) {
      m23_[i] = m23[i];
    }
  }
  if (!m23_invert(m23_)) {
    return FALSE;
  }
  for (i = 0; i < 6; ++i) {
    im23[i] = (int)(m23_[i] * (1 << 16));
  }
  br->texture.bgclr = bgclr;
  br->texture.rcSrc = *prcSrc;
  *br->texture.tex = *tex;
  if (palette) {
    MEMCPY(br->texture.palette, palette, 256);
  }
  else {
    for (i = 0; i < 256; ++i) {
      br->texture.palette[i] = _RGB(i, i, i);
    }
  }
  return TRUE;
}
