

#if 0
#include "img/imgio.inl"

int test_read_bmp() {
  img_t im[1] = {0};
  const char* fn;
  fn = "D:\\Œ¢–≈Õº∆¨_20180301163840.bmp";
  if (imread(fn, 1, 1, im)) {
    imshow_(im); WaitKey(-1);
  }
  imfrees2(im);
  return 0;
}
#else

//#include "cstd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
#define TRUE  1
#define FALSE 0

typedef struct {
  //IMGHEADFEILD;
  union { int w;    int width; int cols; };
  union { int h;    int height; int rows; };
  union { int s;    int step; int widthStep; };
  union { int c;    int cn; };
  union { int f;    int frame; };
  union { int t;    int tid; };
  union {
    uchar* data;
    uchar* ptr;
    short* s;
    int* i;
    float* fl;
    double* db;
  } tt;
} img_t;

#define IMINIT(IM, H, W, A, S, C, F)  ((IM)->h=H,(IM)->w=W,(IM)->tt.data=(uchar*)(A),(IM)->s=S,(IM)->c=(short)(C),(IM)->f=F)

static int imsetsize(img_t* im, int height, int width, int channels, int frames)
{
  if (NULL != im) {
    int step = width * channels;
    int n = height * step * frames;
    if (im->h == height && im->w == width && im->c == channels && im->f == frames && im->tt.data != NULL) {
    }
    else {
      int oldn = im->h * im->s * im->f;
      //int step = (width*channels+3) & ~3;
      if (n > oldn) {
        im->tt.data = (uchar*)realloc(im->tt.data, n);
        //im->tt.data = (uchar*)fastRealloc(im->tt.data, n);
        assert(im->tt.data != NULL && "");
      }
      IMINIT(im, height, width, im->tt.data, step, channels, frames);
      im->tid = 0;
    }
    memset(im->tt.data, 0, n);
  }
  return 0;
}

static int imfree(img_t* im)
{
  if (im) {
    if (im->tt.data) {
      //fastFree(im->tt.data);
      free(im->tt.data);
      im->tt.data = NULL;
    }
    memset(im, 0, sizeof(img_t));
  }
  return 0;
}
#define MEMSET(_F, _V, _N)     memset(_F, _V, sizeof(*(_F))*(_N))
#define MEMZERO(_F, _N)        memset(_F, 0, sizeof(*(_F))*(_N))
#define MEMCPY(_P, _F, _N)     memcpy(_P, _F, sizeof(*(_F))*(_N))
#ifndef MEMMOVE
#define MEMMOVE(_P, _F, _N)    memmove(_P, _F, sizeof(*(_F))*(_N))
#endif
#define SAFEFCLOSE(_PF)        if (NULL!=_PF) { fclose(_PF); _PF = NULL; }
#define MALLOCSET(_TYPE, _N, _V) (_TYPE*)memset(malloc(_N*sizeof(_TYPE)), _V, _N*sizeof(_TYPE))

#define MEM_GET16B(p, i, x) x = (i+=2, (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET16L(p, i, x) x = (i+=2, (((uchar*)p)[i-1]<<8) | (((uchar*)p)[i-2]))
#define MEM_GET32B(p, i, x) x = (i+=4, (((uchar*)p)[i-4]<<24) | (((uchar*)p)[i-3]<<16) | (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET32L(p, i, x) x = (i+=4, (((uchar*)p)[i-1]<<24) | (((uchar*)p)[i-2]<<16) | (((uchar*)p)[i-3]<<8) | (((uchar*)p)[i-4]))

#define MEM_SET16B(p, i, x) (((uchar*)p)[i+0] = ((x)>>8), ((uchar*)p)[i+1] = (x)&0xff, i+=2)
#define MEM_SET16L(p, i, x) (((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x)&0xff, i+=2)
#define MEM_SET32B(p, i, x) (((uchar*)p)[i+0] = ((x)>>24), ((uchar*)p)[i+1] = ((x)>>16), ((uchar*)p)[i+2] = ((x)>>8), ((uchar*)p)[i+3] = (x), i+=4)
#define MEM_SET32L(p, i, x) (((uchar*)p)[i+3] = ((x)>>24), ((uchar*)p)[i+2] = ((x)>>16), ((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x), i+=4)
#define MEM_SETMEM(p, i, x, n) (memcpy((uchar*)p + i, x, n), i+=n)

static int palette_is_color(int n, const uchar* palette)
{
  int i;

  for (i = 0; i < n; ++i) {
    const uchar* p = palette + i * 4;

    if (p[0] != p[1] || p[2] != p[1]) {
      return TRUE;
    }
  }

  return FALSE;
}

#ifndef _SHIFT
#define _SHIFT                 (13)
#define _SHIFT1                (1<<_SHIFT)
#define _SHIFT_MARK            (_SHIFT1 - 1)
#define _SHIFT_MUL(x, y)       (((x)*(y))>>_SHIFT)
#define _SHIFT_DIV(x, y)       (((int)(x)<<_SHIFT)/(y))
#define _SHIFT_TOFLOAT(x)      (((double)(x))/_SHIFT1)
#define _SHIFT_TOINT(x)        (int)((x)*_SHIFT1)
#define _SHIFT_TOINT2(x, _S)   (int)((x)*(1<<(_S)))
#define _HALFSHIFT1            (1<<12)
#endif

#ifndef GREYSCALIZE
#define _CR                  (299*(1 << _SHIFT)/1000)
#define _CG                  (587*(1 << _SHIFT)/1000)
#define _CB                  (_SHIFT1 - _CR - _CG)
#define GREYSCALIZE(R, G, B)   ((_CR*(R) + _CG*(G) + _CB*(B) )>>_SHIFT)
#endif // GREYSCALIZE

#define BGR2GRAY(bgr, gry)   (*(gry) = (unsigned char)GREYSCALIZE(*(bgr+2), *(bgr+1), *(bgr)))
#define RGB2GRAY(bgr, gry)   (*(gry) = (unsigned char)GREYSCALIZE(*(bgr), *(bgr+1), *(bgr+2)))

#define BGRA2GRAY(bgr, gry)  BGR2GRAY(bgr, gry)
#define BGRA2BGR(bgra, bgr)  do { *(bgr) = *(bgra); *(bgr+1) = *(bgra+1); *(bgr+2) = *(bgra+2); } while(0)

#define BGR5552GRAY(bgr555, gry) \
  do { \
    *gry = (unsigned char)GREYSCALIZE( \
        ((((ushort*)bgr555)[0] >> 7) & 0xf8), \
        ((((ushort*)bgr555)[0] >> 2) & 0xf8), \
        ((((ushort*)bgr555)[0] << 3) & 0xf8)); \
  } while(0)
#define BGR5652GRAY(bgr565, gry) \
  do { \
    *gry = (unsigned char)GREYSCALIZE( \
        ((((ushort*)bgr565)[0] >> 8) & 0xf8), \
        ((((ushort*)bgr565)[0] >> 3) & 0xfc), \
        ((((ushort*)bgr565)[0] << 3) & 0xf8) ); \
  } while(0)
#define BGR5552BGR(bgr555, bgr) \
  do { \
    int t0 = (*((ushort*)bgr555) << 3) & 0xf8; \
    int t1 = (*((ushort*)bgr555) >> 2) & 0xf8; \
    int t2 = (*((ushort*)bgr555) >> 7) & 0xf8; \
    *(bgr+0) = (unsigned char)t0; \
    *(bgr+1) = (unsigned char)t1; \
    *(bgr+2) = (unsigned char)t2; \
  } while(0)
#define BGR5652BGR(bgr565, bgr) \
  do { \
    int t0 = (*((ushort*)bgr565) << 3) & 0xf8; \
    int t1 = (*((ushort*)bgr565) >> 3) & 0xfc; \
    int t2 = (*((ushort*)bgr565) >> 8) & 0xf8; \
    *(bgr+0) = (unsigned char)t0; \
    *(bgr+1) = (unsigned char)t1; \
    *(bgr+2) = (unsigned char)t2; \
  } while(0)

#define IMTRANS1D(aw, A, ai, B, bi, FUNC) \
  do { \
    int j, As=0, Bs=0; \
    for (j=0; j<aw; ++j, As+=ai, Bs+=bi) { \
      FUNC((A+As), (B+Bs)); \
    } \
  } while(0)

#define PALETTE_FILLROW_IMPL(CN, PUTPIX) \
  switch (bpp) { \
  case 1: \
    for (i=0; i<len/8; ++i, data +=8*CN) { \
      idx = indices[i]; \
      PUTPIX(0,(idx & 128) != 0); \
      PUTPIX(1,(idx & 64) != 0 ); \
      PUTPIX(2,(idx & 32) != 0 ); \
      PUTPIX(3,(idx & 16) != 0 ); \
      PUTPIX(4,(idx & 8) != 0 ); \
      PUTPIX(5,(idx & 4) != 0 ); \
      PUTPIX(6,(idx & 2) != 0 ); \
      PUTPIX(7,(idx & 1)); \
    } \
    idx = indices[i]; \
    for (i=0; i<(len&7); ++i) { \
      PUTPIX(i, (idx>>(7-i))&1); \
    } \
    break; \
  case 2: \
    for (i=0; i<len/4; ++i, data +=4*CN) { \
      idx = indices[i]; \
      PUTPIX(0,(idx>>6)&3); \
      PUTPIX(1,(idx>>4)&3); \
      PUTPIX(2,(idx>>2)&3); \
      PUTPIX(3,idx & 3); \
    } \
    idx = indices[i]; \
    for (i=0; i<(len&3); ++i) { \
      PUTPIX(i, (idx>>(6-i*2))&3); \
    } \
    break; \
  case 4: \
    for (i=0; i<len/2; ++i, data +=2*CN) { \
      idx = indices[i]; \
      PUTPIX(0,idx>>4); \
      PUTPIX(1,idx&15); \
    } \
    if (len&1) { \
      idx = indices[i]; \
      PUTPIX(0, idx>>4); \
    } \
    break; \
  case 8: \
    for (i=0; i<len; ++i, data +=CN) { \
      idx = indices[i]; \
      PUTPIX(0, idx); \
    } \
    break; \
  }



static int palette_fillrow(int len, uchar* data, int cn, const uchar* src, int bpp, const uchar* palette)
{
  int i, idx;
  uchar mm4[] = {6, 4, 2, 1};
  
  if (bpp<=8) {
    const uchar* indices = src;
    if (1 == cn) {
#define PUTPIX(_I, _J)   data[_I] = palette[_J]
      PALETTE_FILLROW_IMPL(1, PUTPIX);
#undef PUTPIX
    }
    else {
#define PUTPIX(_I, _J)   memcpy(data+(_I)*cn, palette+(_J)*4, 3)
      PALETTE_FILLROW_IMPL(cn, PUTPIX);
#undef PUTPIX
    }
  } else {
    if (1 == cn) {
      switch (bpp) {
      case 15:
        IMTRANS1D(len, src, 2, data, 1, BGR5552GRAY);
        break;
        
      case 16:
        IMTRANS1D(len, src, 2, data, 1, BGR5652GRAY);
        break;
        
      case 24:
        IMTRANS1D(len, src, 3, data, 1, BGR2GRAY);
        break;
        
      case 32:
        IMTRANS1D(len, src, 4, data, 1, BGRA2GRAY);
        break;
      }
    }
    else {
      switch (bpp) {
      case 15:
        IMTRANS1D(len, src, 2, data, cn, BGR5552BGR);
        break;
        
      case 16:
        IMTRANS1D(len, src, 2, data, cn, BGR5652BGR);
        break;
        
      case 24:
        IMTRANS1D(len, src, 3, data, cn, BGRA2BGR);
        break;
        
      case 32:
        IMTRANS1D(len, src, 4, data, cn, BGRA2BGR);
        break;
      }
    }
  }
  
  return 0;
}
static int bmp_load(FILE* s, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  uchar buf[1024];
  int i = 0;
  int m_bpp = 0, m_rle_code = 0, m_offset = 0, hsz, biPlanes;
  int m_width = 0, m_height = 0, m_cn, clr_used = 0;
  int y, src_pitch;
  uchar* m_palette = buf;

  if (!s) {
    return 0;
  }

  fseek(s, 0, SEEK_SET);
  fread(buf, 50, 1, s);

  if (buf[0] != 'B' || buf[1] != 'M') {
    return 0;
  }

  i = 10;
  MEM_GET32L(buf, i, m_offset);
  MEM_GET32L(buf, i, hsz);

  if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) {
    return printf("unknown BMP", "BMP type not supported: unknown");
  }

  if (hsz >= 36) {
    MEM_GET32L(buf, i, m_width);
    MEM_GET32L(buf, i, m_height);
    MEM_GET16L(buf, i, biPlanes);

    if (biPlanes != 1) {
      return printf("bad BMP", "bad BMP");
    }

    MEM_GET16L(buf, i, m_bpp);
    MEM_GET32L(buf, i, m_rle_code);
    i = 46;
    MEM_GET32L(buf, i, clr_used);
    i = 7 * sizeof(int) + 22 + hsz - 36;

    if (m_rle_code == 1 || m_rle_code == 2) {
      return printf("BMP RLE", "BMP type not supported: RLE");
    }

    clr_used = (clr_used == 0 ? 1 << m_bpp : clr_used);
    m_cn = m_bpp > 24 ? 4 : 3;

    if (m_bpp <= 8) {
      fseek(s, i, SEEK_SET);
      fread(m_palette, clr_used * 4, 1, s);

      if (palette) {
        memcpy(palette, m_palette, clr_used * 4);
      }

      if (!palette_is_color(1 << m_bpp, m_palette)) {
        m_cn = 1;
      }
    }
    else if (m_bpp == 16 && m_rle_code == 3) {
      int redmask, greenmask, bluemask;
      MEM_GET32L(buf, i, redmask);
      MEM_GET32L(buf, i, greenmask);
      MEM_GET32L(buf, i, bluemask);

      if (bluemask == 0x1f && greenmask == 0x3e0 && redmask == 0x7c00) {
        m_bpp = 15;
      }
      else if (bluemask == 0x1f && greenmask == 0x7e0 && redmask == 0xf800) {}
      else {
        return 0;
      }
    }
    else if (m_bpp == 16 && m_rle_code == 0) {
      m_bpp = 15;
    }
  }
  else {
    MEM_GET16L(buf, i, m_width);
    MEM_GET16L(buf, i, m_height);
    MEM_GET16L(buf, i, biPlanes);
    MEM_GET16L(buf, i, m_bpp);
    assert(0);
  }

  if (cn_req < 1) {
    cn_req = m_cn;
  }

  if (pbpp) {
    *pbpp = m_bpp;
  }

  if (im) {
    uchar* src_line = NULL;
    uchar* data = NULL;
    int step = 0;
    imsetsize(im, m_height, m_width, cn_req, 1);
    data = im->tt.data + (im->h - 1) * im->s;
    step = -im->s;
    fseek(s, m_offset, SEEK_SET);
    src_pitch = (((m_width * (m_bpp != 15 ? m_bpp : 16) + 7) / 8 + 3) & -4);
    src_line = (uchar*)malloc(src_pitch);

    if (m_bpp <= 8) {
      if (1 == cn_req) {
        IMTRANS1D(clr_used, m_palette, 4, m_palette, 1, BGR2GRAY);
      }

      if (clr_used <= 0) {
        for (i = 0; i < 256; ++i) {
          m_palette[i] = i;
        }
      }
    }

    for (y = 0; y < m_height; y++, data += step) {
      fread(src_line, src_pitch, 1, s);
      palette_fillrow(m_width, data, cn_req, src_line, m_bpp, m_palette);
    }

    free(src_line);
  }

  return 1;
}


static img_t* imread(const char* picname, int cn_req, int frames, img_t* im) {
  FILE* s = fopen(picname, "rb");
  int ret = bmp_load(s, cn_req, im, 0, 0);
  return im;
}

#ifdef TEST
int test_read_bmp() {
  img_t im[1] = {0};
  const char* fn;
  fn = "D:\\Œ¢–≈Õº∆¨_20180301163840.bmp";
  if (imread(fn, 1, 1, im)) {
    //imshow_(im); WaitKey(-1);
  }
  imfree(im);
  return 0;
}
#endif TEST

#endif
