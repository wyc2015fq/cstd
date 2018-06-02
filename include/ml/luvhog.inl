
// icvBGRA2Luv_8u
enum {
  sell_size = 4,
};

static const max3idx_table[] = {0, 1, 0, 1, 0, 2, 2, 2};
#define MAX3IDX(a, b, c)  max3idx_table[((a<b) | ((a<c)<<1) | ((b<c)<<2))]

// ai >= 3
// bi >= 10
int luvhog_feat(int h, int w, const uchar* A, int al, int ai, uchar* B, int bl, int bi)
{
  int i, j, bufsize = 0, al0, bl0;
  uchar* tmp_buf = 0;
  uchar* A0 = 0;
  uchar* B0 = 0;
  ASSERT(ai >= 3 && bi >= 10);
  bufsize = (h + 2) * (w + 2) * ai + h * w * 10;
  B0 = tmp_buf = MALLOC(uchar, bufsize);
  MEMSET(tmp_buf, 0, bufsize);
  A0 = B0 + h * w * 10;
  al0 = (w + 2) * ai;
  bl0 = w * 10;
  mem_fill_border(h, w, A, al, h + 2, w + 2, A0, al0, ai, 1, 1, 0, BD_REPLICATE);
  A0 += al + ai;
  //cvShowImage("adsf", h, w, B0, bl0, 1); cvWaitKey(-1);

  for (i = 0; i < h; ++i) {
    const uchar* a0 = A0 + al0 * i;
    uchar* b0 = B0 + bl0 * i;

    for (j = 0; j < w; ++j, a0 += ai, b0 += 10) {
#define soble_macro(src, _L, _I)  ( (src[_I-_L]+(src[_I]<<1)+src[_L+_I]) - (src[_L-_I]+(src[-(_I)]<<1)+src[-_L-_I]) )
      int gx[3], gy[3], mag[3], angle, idx = 0, imag;
      const uchar* src = a0;
      uchar* dst = b0;
      const uchar* src0 = a0 + 0;
      const uchar* src1 = a0 + 1;
      const uchar* src2 = a0 + 2;
      gx[0] = soble_macro(src0, al0, ai), gy[0] = soble_macro(src0, ai, al0);
      gx[1] = soble_macro(src1, al0, ai), gy[1] = soble_macro(src1, ai, al0);
      gx[2] = soble_macro(src2, al0, ai), gy[2] = soble_macro(src2, ai, al0);
      mag[0] = MAG2(gy[0], gx[0]);
      mag[1] = MAG2(gy[1], gx[1]);
      mag[2] = MAG2(gy[2], gx[2]);
      idx = MAX3IDX(mag[0], mag[1], mag[2]);
      angle = (fixatan2(gy[idx], gx[idx]) / 60 + 6) % 6;
      ASSERT(angle >= 0 && angle < 6);
      imag = fixmag(gy[idx], gx[idx]);
      imag = BOUND(imag, 0, 255);
      //ASSERT(imag>=0 && imag<256);
      dst[4 + angle] = dst[3] = imag;
#include "icvBGRA2Luv_32f_impl.inl"
#undef soble_macro
    }
  }

  h /= sell_size, w /= sell_size;

  for (i = 0; i < h; ++i) {
    uchar* b0 = B0 + bl0 * i * sell_size;
    uchar* b1 = B + bl * i;

    for (j = 0; j < w; ++j, b0 += sell_size * 10, b1 += bi) {
#define sum_block_macro_2x2(src, _L, _I)  ((src)[0]+(src)[_I]+(src)[_L]+(src)[_L+_I])
#define sum_block_macro_4x4(src, _L, _I)  (sum_block_macro_2x2(src, _L, _I)+sum_block_macro_2x2(src+2*_I, _L, _I)+sum_block_macro_2x2(src+2*_L, _L, _I)+sum_block_macro_2x2(src+2*(_L+_I), _L, _I)+8)>>4
      b1[0] = sum_block_macro_4x4(b0 + 0, bl0, 10);
      b1[1] = sum_block_macro_4x4(b0 + 1, bl0, 10);
      b1[2] = sum_block_macro_4x4(b0 + 2, bl0, 10);
      b1[3] = sum_block_macro_4x4(b0 + 3, bl0, 10);
      b1[4] = sum_block_macro_4x4(b0 + 4, bl0, 10);
      b1[5] = sum_block_macro_4x4(b0 + 5, bl0, 10);
      b1[5] = sum_block_macro_4x4(b0 + 5, bl0, 10);
      b1[6] = sum_block_macro_4x4(b0 + 6, bl0, 10);
      b1[7] = sum_block_macro_4x4(b0 + 7, bl0, 10);
      b1[8] = sum_block_macro_4x4(b0 + 8, bl0, 10);
      b1[9] = sum_block_macro_4x4(b0 + 9, bl0, 10);
    }
  }

  FREE(tmp_buf);
  return 0;
}
