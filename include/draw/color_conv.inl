
#ifndef __COLOR_CONV_INL__
#define __COLOR_CONV_INL__

typedef void (*color_conv_row_f)(int8u* dst, const int8u* src, unsigned width);
//--------------------------------------------------------------color_conv
void color_conv(uchar* dst, int dst_step, const uchar* src, int src_step, unsigned width, unsigned height, color_conv_row_f copy_row_functor) {
  do {
    copy_row_functor(dst, src, width);
    dst += dst_step;
    src += src_step;
  } while(--height);
}

//---------------------------------------------------------color_conv_same
#define color_conv_same(BPP, name) void name(int8u* dst, const int8u* src, unsigned width) { memmove(dst, src, (width)*(BPP)); }

//-----------------------------------------------------color_conv_rgb24
void color_conv_rgb24(int8u* dst, const int8u* src, unsigned width) {
  do {
    dst[0] = src[2];
    dst[1] = src[1];
    dst[2] = src[0];
    src += 3; dst += 3;
  } while(--width);
}

#define color_conv_rgb24_to_bgr24 color_conv_rgb24
#define color_conv_bgr24_to_rgb24 color_conv_rgb24
color_conv_same(3, color_conv_bgr24_to_bgr24)
color_conv_same(3, color_conv_rgb24_to_rgb24)

//------------------------------------------------------color_conv_rgba32
#define color_conv_rgba32(I1, I2, I3, I4, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
  do { \
    dst[0] = src[I1]; \
    dst[1] = src[I2]; \
    dst[2] = src[I3]; \
    dst[3] = src[I4]; \
    src += 4; dst += 4; \
  } while(--width); \
}


color_conv_rgba32(0,3,2,1, color_conv_argb32_to_abgr32) //----color_conv_argb32_to_abgr32
color_conv_rgba32(3,2,1,0, color_conv_argb32_to_bgra32) //----color_conv_argb32_to_bgra32
color_conv_rgba32(1,2,3,0, color_conv_argb32_to_rgba32) //----color_conv_argb32_to_rgba32
color_conv_rgba32(3,0,1,2, color_conv_bgra32_to_abgr32) //----color_conv_bgra32_to_abgr32
color_conv_rgba32(3,2,1,0, color_conv_bgra32_to_argb32) //----color_conv_bgra32_to_argb32
color_conv_rgba32(2,1,0,3, color_conv_bgra32_to_rgba32) //----color_conv_bgra32_to_rgba32
color_conv_rgba32(3,2,1,0, color_conv_rgba32_to_abgr32) //----color_conv_rgba32_to_abgr32
color_conv_rgba32(3,0,1,2, color_conv_rgba32_to_argb32) //----color_conv_rgba32_to_argb32
color_conv_rgba32(2,1,0,3, color_conv_rgba32_to_bgra32) //----color_conv_rgba32_to_bgra32
color_conv_rgba32(0,3,2,1, color_conv_abgr32_to_argb32) //----color_conv_abgr32_to_argb32
color_conv_rgba32(1,2,3,0, color_conv_abgr32_to_bgra32) //----color_conv_abgr32_to_bgra32
color_conv_rgba32(3,2,1,0, color_conv_abgr32_to_rgba32) //----color_conv_abgr32_to_rgba32


color_conv_same(4, color_conv_rgba32_to_rgba32) //----color_conv_rgba32_to_rgba32
color_conv_same(4, color_conv_argb32_to_argb32) //----color_conv_argb32_to_argb32
color_conv_same(4, color_conv_bgra32_to_bgra32) //----color_conv_bgra32_to_bgra32
color_conv_same(4, color_conv_abgr32_to_abgr32) //----color_conv_abgr32_to_abgr32


//--------------------------------------------color_conv_rgb24_rgba32
#define color_conv_rgb24_rgba32(I1, I2, I3, A, name) \
  void name(int8u* dst, const int8u* src, unsigned width) { \
    do { \
      dst[I1] = src[0]; \
      dst[I2] = src[1]; \
      dst[I3] = src[2]; \
      dst[A]  = 255;  \
      src += 3; dst += 4; \
    } while(--width); \
  }



color_conv_rgb24_rgba32(1,2,3,0, color_conv_rgb24_to_argb32) //----color_conv_rgb24_to_argb32
color_conv_rgb24_rgba32(3,2,1,0, color_conv_rgb24_to_abgr32) //----color_conv_rgb24_to_abgr32
color_conv_rgb24_rgba32(2,1,0,3, color_conv_rgb24_to_bgra32) //----color_conv_rgb24_to_bgra32
color_conv_rgb24_rgba32(0,1,2,3, color_conv_rgb24_to_rgba32) //----color_conv_rgb24_to_rgba32
color_conv_rgb24_rgba32(3,2,1,0, color_conv_bgr24_to_argb32) //----color_conv_bgr24_to_argb32
color_conv_rgb24_rgba32(1,2,3,0, color_conv_bgr24_to_abgr32) //----color_conv_bgr24_to_abgr32
color_conv_rgb24_rgba32(0,1,2,3, color_conv_bgr24_to_bgra32) //----color_conv_bgr24_to_bgra32
color_conv_rgb24_rgba32(2,1,0,3, color_conv_bgr24_to_rgba32) //----color_conv_bgr24_to_rgba32



//-------------------------------------------------color_conv_rgba32_rgb24
#define color_conv_rgba32_rgb24(I1, I2, I3, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
  do { \
    *dst++ = src[I1]; \
    *dst++ = src[I2]; \
    *dst++ = src[I3]; \
    src += 4; \
  } while(--width); \
}




color_conv_rgba32_rgb24(1,2,3, color_conv_argb32_to_rgb24) //----color_conv_argb32_to_rgb24
color_conv_rgba32_rgb24(3,2,1, color_conv_abgr32_to_rgb24) //----color_conv_abgr32_to_rgb24
color_conv_rgba32_rgb24(2,1,0, color_conv_bgra32_to_rgb24) //----color_conv_bgra32_to_rgb24
color_conv_rgba32_rgb24(0,1,2, color_conv_rgba32_to_rgb24) //----color_conv_rgba32_to_rgb24
color_conv_rgba32_rgb24(3,2,1, color_conv_argb32_to_bgr24) //----color_conv_argb32_to_bgr24
color_conv_rgba32_rgb24(1,2,3, color_conv_abgr32_to_bgr24) //----color_conv_abgr32_to_bgr24
color_conv_rgba32_rgb24(0,1,2, color_conv_bgra32_to_bgr24) //----color_conv_bgra32_to_bgr24
color_conv_rgba32_rgb24(2,1,0, color_conv_rgba32_to_bgr24) //----color_conv_rgba32_to_bgr24


//------------------------------------------------color_conv_rgb555_rgb24
#define color_conv_rgb555_rgb24(R, B, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
do { \
    unsigned rgb = *(int16u*)src; \
    dst[R] =(int8u)((rgb >> 7) & 0xF8); \
    dst[1] =(int8u)((rgb >> 2) & 0xF8); \
    dst[B] =(int8u)((rgb << 3) & 0xF8); \
    src += 2; dst += 3; \
  } \
  while(--width); \
}



color_conv_rgb555_rgb24(2,0, color_conv_rgb555_to_bgr24) //----color_conv_rgb555_to_bgr24
color_conv_rgb555_rgb24(0,2, color_conv_rgb555_to_rgb24) //----color_conv_rgb555_to_rgb24

//-------------------------------------------------color_conv_rgb24_rgb555
#define color_conv_rgb24_rgb555(R, B, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
do { \
    *(int16u*)dst =(int16u)((((src[R]) << 7) & 0x7C00) | (((src[1]) << 2) & 0x3E0) | (((src[B]) >> 3))); \
    src += 3; dst += 2; \
  } while(--width); \
}


color_conv_rgb24_rgb555(2,0, color_conv_bgr24_to_rgb555) //----color_conv_bgr24_to_rgb555
color_conv_rgb24_rgb555(0,2, color_conv_rgb24_to_rgb555) //----color_conv_rgb24_to_rgb555


//-------------------------------------------------color_conv_rgb565_rgb24
#define color_conv_rgb565_rgb24(R, B, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
  do { \
    unsigned rgb = *(int16u*)src; \
    dst[R] =(rgb >> 8) & 0xF8; \
    dst[1] =(rgb >> 3) & 0xFC; \
    dst[B] =(rgb << 3) & 0xF8; \
    src += 2; dst += 3; \
  } while(--width); \
}



color_conv_rgb565_rgb24(2,0, color_conv_rgb565_to_bgr24) //----color_conv_rgb565_to_bgr24
color_conv_rgb565_rgb24(0,2, color_conv_rgb565_to_rgb24) //----color_conv_rgb565_to_rgb24


//-------------------------------------------------color_conv_rgb24_rgb565
#define color_conv_rgb24_rgb565(R, B, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
  do { \
    *(int16u*)dst =(int16u)((((src[R]) << 8) & 0xF800) | (((src[1]) << 3) & 0x7E0) | (((src[B]) >> 3))); \
    src += 3; dst += 2; \
  } while(--width); \
}


color_conv_rgb24_rgb565(2,0, color_conv_bgr24_to_rgb565) //----color_conv_bgr24_to_rgb565
color_conv_rgb24_rgb565(0,2, color_conv_rgb24_to_rgb565) //----color_conv_rgb24_to_rgb565



//-------------------------------------------------color_conv_rgb555_rgba32
#define color_conv_rgb555_rgba32(R, G, B, A, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
  do { \
    int rgb = *(int16*)src; \
    dst[R] =(int8u)((rgb >> 7) & 0xF8); \
    dst[G] =(int8u)((rgb >> 2) & 0xF8); \
    dst[B] =(int8u)((rgb << 3) & 0xF8); \
    dst[A] =(int8u)(rgb >> 15); \
    src += 2; dst += 4; \
  } while(--width); \
}


color_conv_rgb555_rgba32(1,2,3,0, color_conv_rgb555_to_argb32) //----color_conv_rgb555_to_argb32
color_conv_rgb555_rgba32(3,2,1,0, color_conv_rgb555_to_abgr32) //----color_conv_rgb555_to_abgr32
color_conv_rgb555_rgba32(2,1,0,3, color_conv_rgb555_to_bgra32) //----color_conv_rgb555_to_bgra32
color_conv_rgb555_rgba32(0,1,2,3, color_conv_rgb555_to_rgba32) //----color_conv_rgb555_to_rgba32


//------------------------------------------------color_conv_rgba32_rgb555
#define color_conv_rgba32_rgb555(R, G, B, A, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
  do { \
    *(int16u*)dst = (int16u)((((src[R]) << 7) & 0x7C00) |  \
                            (((src[G]) << 2) & 0x3E0)  | \
                            (((src[B]) >> 3)) | \
                            (((src[A]) << 8) & 0x8000)); \
    src += 4; dst += 2; \
  } while(--width); \
}


color_conv_rgba32_rgb555(1,2,3,0, color_conv_argb32_to_rgb555) //----color_conv_argb32_to_rgb555
color_conv_rgba32_rgb555(3,2,1,0, color_conv_abgr32_to_rgb555) //----color_conv_abgr32_to_rgb555
color_conv_rgba32_rgb555(2,1,0,3, color_conv_bgra32_to_rgb555) //----color_conv_bgra32_to_rgb555
color_conv_rgba32_rgb555(0,1,2,3, color_conv_rgba32_to_rgb555) //----color_conv_rgba32_to_rgb555



//------------------------------------------------color_conv_rgb565_rgba32
#define color_conv_rgb565_rgba32(R, G, B, A, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
  do { \
    int rgb = *(int16*)src; \
    dst[R] =(rgb >> 8) & 0xF8; \
    dst[G] =(rgb >> 3) & 0xFC; \
    dst[B] =(rgb << 3) & 0xF8; \
    dst[A] = 255; \
    src += 2; dst += 4; \
  } while(--width); \
}




color_conv_rgb565_rgba32(1,2,3,0, color_conv_rgb565_to_argb32) //----color_conv_rgb565_to_argb32
color_conv_rgb565_rgba32(3,2,1,0, color_conv_rgb565_to_abgr32) //----color_conv_rgb565_to_abgr32
color_conv_rgb565_rgba32(2,1,0,3, color_conv_rgb565_to_bgra32) //----color_conv_rgb565_to_bgra32
color_conv_rgb565_rgba32(0,1,2,3, color_conv_rgb565_to_rgba32) //----color_conv_rgb565_to_rgba32


//------------------------------------------------color_conv_rgba32_rgb565
#define color_conv_rgba32_rgb565(R, G, B, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
do { \
  *(int16u*)dst = (int16u)((((src[R]) << 8) & 0xF800) |  \
                           (((src[G]) << 3) & 0x7E0) | \
                           (((src[B]) >> 3))); \
  src += 4; dst += 2; \
  } while(--width); \
}

color_conv_rgba32_rgb565(1,2,3, color_conv_argb32_to_rgb565) //----color_conv_argb32_to_rgb565
color_conv_rgba32_rgb565(3,2,1, color_conv_abgr32_to_rgb565) //----color_conv_abgr32_to_rgb565
color_conv_rgba32_rgb565(2,1,0, color_conv_bgra32_to_rgb565) //----color_conv_bgra32_to_rgb565
color_conv_rgba32_rgb565(0,1,2, color_conv_rgba32_to_rgb565) //----color_conv_rgba32_to_rgb565
//---------------------------------------------color_conv_rgb555_to_rgb565
void color_conv_rgb555_to_rgb565(int8u* dst, const int8u* src, unsigned width) {
  do {
    unsigned rgb = *(int16u*)src;
    *(int16u*)dst =(int16u)(((rgb << 1) & 0xFFC0) |(rgb & 0x1F));
    src += 2;
    dst += 2;
  } while(--width);
}

//----------------------------------------------color_conv_rgb565_to_rgb555
void color_conv_rgb565_to_rgb555(int8u* dst, const int8u* src, unsigned width) {
  do {
    unsigned rgb = *(int16u*)src;
    *(int16u*)dst =(int16u)(((rgb >> 1) & 0x7FE0) |(rgb & 0x1F));
    src += 2;
    dst += 2;
  } while(--width);
}


color_conv_same(2, color_conv_rgb555_to_rgb555) //----color_conv_rgb555_to_rgb555
color_conv_same(2, color_conv_rgb565_to_rgb565) //----color_conv_rgb565_to_rgb565

#define color_conv_rgb24_gray8(R, B, name) \
void name(int8u* dst, const int8u* src, unsigned width) { \
  do { \
    *dst++ =(src[R]*77 + src[1]*150 + src[B]*29) >> 8; \
    src += 3; \
  } while(--width); \
}

color_conv_rgb24_gray8(0,2, color_conv_rgb24_to_gray8) //----color_conv_rgb24_to_gray8
color_conv_rgb24_gray8(2,0, color_conv_bgr24_to_gray8) //----color_conv_bgr24_to_gray8

int test_color_conv() {
  return 0;
}

#endif // __COLOR_CONV_INL__
