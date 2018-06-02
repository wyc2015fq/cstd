
#ifndef __FMTIMG_H_
#define __FMTIMG_H_

#include "cstd.h"
#include "xfile.h"
//#include "../../include/tchar_x.h"
//#include "../../include/macro.h"

#define READ_NBYTE(_A, _N)  xfread((_A), 1, _N, (ihand_t)m_strm)
#define WRITE_NBYTE(_A, _N) xfwrite((_A), 1, _N, (ihand_t)m_strm)
#define SEEK(_N, _W)        xfseek((ihand_t)m_strm, (_N), _W)
#define GetPos()            xftell((ihand_t)m_strm)
//#define GetLength()         myGetSize((ihand_t)m_strm)
#define STRM_CLOSE(_s)      xfclose((ihand_t)(_s))
#define STRM_OPEN_FILE      xfopen_file

#define READ(A)             READ_NBYTE(&(A), sizeof(A))
#define WRITE(A)            WRITE_NBYTE(&(A), sizeof(A))
#define SetPos(pos)         SEEK((pos), SEEK_SET)
#define PutBytes WRITE_NBYTE
#define GetBytes READ_NBYTE

#define RBS_BAD_HEADER     -125  /* invalid image header */
#define BAD_HEADER_ERR()   goto bad_header_exit

#define RBS_THROW_EOS      -123  /* <end of stream> exception code */
#define RBS_THROW_FORB     -124  /* <forrbidden huffman code> exception code */
#define RBS_HUFF_FORB      2047  /* forrbidden huffman code "value" */

#define WRITE_PIX(ptr, clr)  (((unsigned char*)(ptr))[0]=(clr).b,((unsigned char*)(ptr))[1]=(clr).g,((unsigned char*)(ptr))[2] = (clr).r)

int getsign(ihand_t m_strm, char* psign, int m_sign_len)
{
  int re = 0;

  if (!m_sign_len) {
    return 0;
  }

  //m_filename = picname;
  if (m_strm) {
    SetPos(0);
    re = READ_NBYTE(psign, m_sign_len);
  }

  return re;
}

int checksign(ihand_t pf, const char* m_signature, int m_sign_len)
{
  int re = 0;
  char psign[256] = {0};
  ASSERT(m_sign_len<countof(psign));
  re = getsign(pf, psign, m_sign_len);

  if (re) {
    re = !memcmp(psign, m_signature, m_sign_len);
  }

  return re;
}

int IsColorPalette(const PaletteEntry* f, const PaletteEntry* l)
{
  for (; f != l; ++f) {
    if (f->b != f->g || f->b != f->r) {
      return TRUE;
    }
  }

  return FALSE;
}

void FillGrayPalette(PaletteEntry* palette, int bpp, int negative)
{
  int i, length = 1 << bpp;
  int xor_mask = negative ? 255 : 0;

  for (i = 0; i < length; i++) {
    int val = (i * 255 / (length - 1)) ^ xor_mask;
    palette[ i ].b = palette[ i ].g = palette[ i ].r = (unsigned char) val;
    palette[ i ].a = 0;
  }
}

unsigned char* FillUniColor(unsigned char* data, unsigned char** line_end, int step, int width3,
    int* y, int height, int count3, PaletteEntry clr)
{
  do {
    unsigned char* end = data + count3;

    if (end > *line_end) {
      end = *line_end;
    }

    count3 -= (int)(end - data);

    for (; data < end; data += 3) {
      *(data + 0) = clr.b;
      *(data + 1) = clr.g;
      *(data + 2) = clr.r;
    }

    if (data >= *line_end) {
      *line_end += step;
      data = *line_end - width3;

      if (++(*y) >= height) {
        break;
      }
    }
  }
  while (count3 > 0);

  return data;
}

unsigned char* FillUniGray(unsigned char* data, unsigned char** line_end, int step, int width,
    int* y, int height, int count, unsigned char clr)
{
  do {
    unsigned char* end = data + count;

    if (end > *line_end) {
      end = *line_end;
    }

    count -= (int)(end - data);

    for (; data < end; data++) {
      *data = clr;
    }

    if (data >= *line_end) {
      *line_end += step;
      data = *line_end - width;

      if (++(*y) >= height) {
        break;
      }
    }
  }
  while (count > 0);

  return data;
}

unsigned char* FillColorRow8(unsigned char* data, unsigned char* indices, int len, PaletteEntry* palette)
{
  PaletteEntry clr;
  unsigned char* end = data + len * 3;

  while ((data += 3) < end) {
    clr = palette[ *indices++ ];
    WRITE_PIX(data - 3, clr);
  }

  clr = palette[ indices[ 0 ] ];
  WRITE_PIX(data - 3, clr);
  return data;
}

unsigned char* FillGrayRow8(unsigned char* data, unsigned char* indices, int len, unsigned char* palette)
{
  int i;

  for (i = 0; i < len; i++) {
    data[ i ] = palette[ indices[ i ] ];
  }

  return data + len;
}

unsigned char* FillColorRow4(unsigned char* data, unsigned char* indices, int len, PaletteEntry* palette)
{
  int idx;
  PaletteEntry clr;
  unsigned char* end = data + len * 3;

  while ((data += 6) < end) {
    int idx = *indices++;
    *((PaletteEntry*)(data - 6)) = palette[ idx >> 4 ];
    *((PaletteEntry*)(data - 3)) = palette[ idx & 15 ];
  }

  idx = indices[ 0 ];
  clr = palette[ idx >> 4 ];
  WRITE_PIX(data - 6, clr);

  if (data == end) {
    clr = palette[ idx & 15 ];
    WRITE_PIX(data - 3, clr);
  }

  return end;
}

unsigned char* FillGrayRow4(unsigned char* data, unsigned char* indices, int len, unsigned char* palette)
{
  int idx;
  unsigned char clr;
  unsigned char* end = data + len;

  while ((data += 2) < end) {
    int idx = *indices++;
    data[ -2 ] = palette[ idx >> 4 ];
    data[ -1 ] = palette[ idx & 15 ];
  }

  idx = indices[ 0 ];
  clr = palette[ idx >> 4 ];
  data[ -2 ] = clr;

  if (data == end) {
    clr = palette[ idx & 15 ];
    data[ -1 ] = clr;
  }

  return end;
}

unsigned char* FillColorRow1(unsigned char* data, unsigned char* indices, int len, PaletteEntry* palette)
{
  int idx;
  unsigned char* end = data + len * 3;

  while ((data += 24) < end) {
    int idx = *indices++;
    *((PaletteEntry*)(data - 24)) = palette[(idx & 128) != 0 ];
    *((PaletteEntry*)(data - 21)) = palette[(idx & 64) != 0 ];
    *((PaletteEntry*)(data - 18)) = palette[(idx & 32) != 0 ];
    *((PaletteEntry*)(data - 15)) = palette[(idx & 16) != 0 ];
    *((PaletteEntry*)(data - 12)) = palette[(idx & 8) != 0 ];
    *((PaletteEntry*)(data - 9)) = palette[(idx & 4) != 0 ];
    *((PaletteEntry*)(data - 6)) = palette[(idx & 2) != 0 ];
    *((PaletteEntry*)(data - 3)) = palette[(idx & 1) != 0 ];
  }

  idx = indices[ 0 ] << 24;

  for (data -= 24; data < end; data += 3, idx += idx) {
    PaletteEntry clr = palette[ idx < 0 ];
    WRITE_PIX(data, clr);
  }

  return data;
}

unsigned char* FillGrayRow1(unsigned char* data, unsigned char* indices, int len, unsigned char* palette)
{
  int idx;
  unsigned char* end = data + len;

  while ((data += 8) < end) {
    int idx = *indices++;
    *((unsigned char*)(data - 8)) = palette[(idx & 128) != 0 ];
    *((unsigned char*)(data - 7)) = palette[(idx & 64) != 0 ];
    *((unsigned char*)(data - 6)) = palette[(idx & 32) != 0 ];
    *((unsigned char*)(data - 5)) = palette[(idx & 16) != 0 ];
    *((unsigned char*)(data - 4)) = palette[(idx & 8) != 0 ];
    *((unsigned char*)(data - 3)) = palette[(idx & 4) != 0 ];
    *((unsigned char*)(data - 2)) = palette[(idx & 2) != 0 ];
    *((unsigned char*)(data - 1)) = palette[(idx & 1) != 0 ];
  }

  idx = indices[ 0 ] << 24;

  for (data -= 8; data < end; data++, idx += idx) {
    data[ 0 ] = palette[ idx < 0 ];
  }

  return data;
}

#include "img/color_quantization.inl"

#endif // __FMTIMG_H_
