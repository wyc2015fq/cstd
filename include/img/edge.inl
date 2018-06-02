
const static BYTE saturated_sum[256 * 2] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
  0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
#define adds(var, val)  ((var) = saturated_sum[(var) + (val) + 128])
#define subs(var, val)  ((var) = saturated_sum[(var) - (val) + 128])


void EnhanceEdges(int height, int width, uchar* lpPixels, int step, int cn, int g_Enhance, int g_UseSubPixel)
{
  int  w = step;
  int  difstep = width * 4;
  int x, y;
  char* _hdif, *_vdif;
  _hdif = (char*)pmalloc(difstep * height);
  _vdif = (char*)pmalloc(difstep * height);

  memset(_hdif, 0, difstep * height);
  memset(_vdif, 0, difstep * height);

  if (g_Enhance <= 0) {
    return;
  }
  else if (g_Enhance > 4) {
    g_Enhance = 4;
  }

  for (y = 1 ; y < height - 1 ; y++) {
    register BYTE* dst = lpPixels + y * step + 4;
    register char* hdif = _hdif + y * difstep + 4;
    register char* vdif = _vdif + y * difstep + 4;

    for (x = 1 ; x < width - 1 ; x++) {
      int ratio = (7 - g_Enhance) * 2;

      if (g_UseSubPixel == 0) {
        hdif[0] = ((dst[0] << 1) - dst[-4]   - dst[4])   / ratio;
        hdif[1] = ((dst[1] << 1) - dst[-3]   - dst[5])   / ratio;
        hdif[2] = ((dst[2] << 1) - dst[-2]   - dst[6])   / ratio;
      }

      vdif[0] = ((dst[0] << 1) - dst[-w]   - dst[w])   / ratio;
      vdif[1] = ((dst[1] << 1) - dst[-w + 1] - dst[w + 1]) / ratio;
      vdif[2] = ((dst[2] << 1) - dst[-w + 2] - dst[w + 2]) / ratio;
      dst += 4, hdif += 4, vdif += 4;
    }
  }

  for (y = 1 ; y < height - 1 ; y++) {
    register BYTE* dst = lpPixels + y * step + 4;
    register char* hdif = _hdif + y * difstep + 4;
    register char* vdif = _vdif + y * difstep + 4;

    for (x = 1 ; x < width - 1 ; x++) {
      if (g_UseSubPixel == 0) {
        subs(dst[-4], hdif[0] / 2);
        adds(dst[0], hdif[0]);
        subs(dst[4],  hdif[0] / 2);
        subs(dst[-3], hdif[1] / 2);
        adds(dst[1], hdif[1]);
        subs(dst[5],  hdif[1] / 2);
        subs(dst[-2], hdif[2] / 2);
        adds(dst[2], hdif[2]);
        subs(dst[6],  hdif[2] / 2);
      }

      subs(dst[-w],  vdif[0] / 2);
      adds(dst[0], vdif[0]);
      subs(dst[w],   vdif[0] / 2);
      subs(dst[1 - w], vdif[1] / 2);
      adds(dst[1], vdif[1]);
      subs(dst[w + 1], vdif[1] / 2);
      subs(dst[2 - w], vdif[2] / 2);
      adds(dst[2], vdif[2]);
      subs(dst[w + 2], vdif[2] / 2);
      dst += 4;
      hdif += 4;
      vdif += 4;
    }
  }

  pfree(_hdif);
  pfree(_vdif);
}
#undef adds
#undef subs

void  ScaleDIB(int height, int width, BYTE* lpPixels, int step, int g_Scale, int g_UseSubPixel, int g_SubPixelDirection)
{
  int x, y;
  int  srcWidthBytes = step;
  int  srcSkipBytes  = srcWidthBytes - (width * 4);
  int  dstSkipBytes  = srcWidthBytes - (width * 4 / g_Scale);
#if defined(__SSE2__) || defined(__SSE3__)
  __m128i PZERO = _mm_setzero_si128();
#endif
  register BYTE* dst  = lpPixels;
  register BYTE* src0 = lpPixels;
  width  /= g_Scale;
  height /= g_Scale;

  if (g_Scale == 4) {
    for (y = 0 ; y < height ; y++) {
      register BYTE* src1 = src0 + srcWidthBytes;
      register BYTE* src2 = src1 + srcWidthBytes;
      register BYTE* src3 = src2 + srcWidthBytes;

      for (x = 0 ; x < width ; x++) {
        dst[0] = (src0[0] + src0[4] + src0[8] + src0[12] +
            src1[0] + src1[4] + src1[8] + src1[12] +
            src2[0] + src2[4] + src2[8] + src2[12] +
            src3[0] + src3[4] + src3[8] + src3[12]) >> 4;
        dst[1] = (src0[1] + src0[5] + src0[9] + src0[13] +
            src1[1] + src1[5] + src1[9] + src1[13] +
            src2[1] + src2[5] + src2[9] + src2[13] +
            src3[1] + src3[5] + src3[9] + src3[13]) >> 4;
        dst[2] = (src0[2] + src0[6] + src0[10] + src0[14] +
            src1[2] + src1[6] + src1[10] + src1[14] +
            src2[2] + src2[6] + src2[10] + src2[14] +
            src3[2] + src3[6] + src3[10] + src3[14]) >> 4;
        dst += 4;
        src0 += 4 * 4;
        src1 += 4 * 4;
        src2 += 4 * 4;
        src3 += 4 * 4;
      }

      dst += dstSkipBytes;
      src0 = src3 + srcSkipBytes;
    }
  }
  else if (g_Scale == 3) {
    if (g_UseSubPixel == 0) {
      for (y = 0 ; y < height ; y++) {
        // Don't use subpixels
        register BYTE* src1 = src0 + srcWidthBytes;
        register BYTE* src2 = src1 + srcWidthBytes;

        for (x = 0 ; x < width; x++) {
          dst[0] = (src0[0] + src0[4] + src0[8] +
              src1[0] + src1[4] + src1[8] +
              src2[0] + src2[4] + src2[8]) / 9;
          dst[1] = (src0[1] + src0[5] + src0[9] +
              src1[1] + src1[5] + src1[9] +
              src2[1] + src2[5] + src2[9]) / 9;
          dst[2] = (src0[2] + src0[6] + src0[10] +
              src1[2] + src1[6] + src1[10] +
              src2[2] + src2[6] + src2[10]) / 9;
          dst += 4;
          src0 += 4 * 3;
          src1 += 4 * 3;
          src2 += 4 * 3;
        }

        dst += dstSkipBytes;
        src0 = src2 + srcSkipBytes;
      }
    }
    else {
      if (g_SubPixelDirection == 0) {
        for (y = 0 ; y < height ; y++) {
          // RGB
          register BYTE* src1 = src0 + srcWidthBytes;
          register BYTE* src2 = src1 + srcWidthBytes;
          dst[0] = (src0[0] + src0[4] + src0[8] +
              src1[0] + src1[4] + src1[8] +
              src2[0] + src2[4] + src2[8]) / 9;
          dst[1] = (src0[1] + src0[5] + src0[9] +
              src1[1] + src1[5] + src1[9] +
              src2[1] + src2[5] + src2[9]) / 9;
          dst[2] = (src0[2] + src0[6] + src0[10] +
              src1[2] + src1[6] + src1[10] +
              src2[2] + src2[6] + src2[10]) / 9;
          dst += 4;
          src0 += 4 * 3;
          src1 += 4 * 3;
          src2 += 4 * 3;

          for (x = 1 ; x < width - 1; x++) {
            dst[0] = (src0[8] + src0[12] + src0[16] +
                src1[8] + src1[12] + src1[16] +
                src2[8] + src2[12] + src2[16]) / 9;
            dst[1] = (src0[5] + src0[9] + src0[13] +
                src1[5] + src1[9] + src1[13] +
                src2[5] + src2[9] + src2[13]) / 9;
            dst[2] = (src0[2] + src0[6] + src0[10] +
                src1[2] + src1[6] + src1[10] +
                src2[2] + src2[6] + src2[10]) / 9;
            dst += 4;
            src0 += 4 * 3;
            src1 += 4 * 3;
            src2 += 4 * 3;
          }

          dst[0] = (src0[0] + src0[4] + src0[8] +
              src1[0] + src1[4] + src1[8] +
              src2[0] + src2[4] + src2[8]) / 9;
          dst[1] = (src0[1] + src0[5] + src0[9] +
              src1[1] + src1[5] + src1[9] +
              src2[1] + src2[5] + src2[9]) / 9;
          dst[2] = (src0[2] + src0[6] + src0[10] +
              src1[2] + src1[6] + src1[10] +
              src2[2] + src2[6] + src2[10]) / 9;
          //dst+=4; src0+=4*3; src1+=4*3; src2+=4*3;
          dst += dstSkipBytes + 4;
          src0 = src2 + (4 * 3) + srcSkipBytes;
        }
      }
      else {
        for (y = 0 ; y < height ; y++) {
          // BGR
          register BYTE* src1 = src0 + srcWidthBytes;
          register BYTE* src2 = src1 + srcWidthBytes;
          dst[0] = (src0[0] + src0[4] + src0[8] +
              src1[0] + src1[4] + src1[8] +
              src2[0] + src2[4] + src2[8]) / 9;
          dst[1] = (src0[1] + src0[5] + src0[9] +
              src1[1] + src1[5] + src1[9] +
              src2[1] + src2[5] + src2[9]) / 9;
          dst[2] = (src0[2] + src0[6] + src0[10] +
              src1[2] + src1[6] + src1[10] +
              src2[2] + src2[6] + src2[10]) / 9;
          dst += 4;
          src0 += 4 * 3;
          src1 += 4 * 3;
          src2 += 4 * 3;

          for (x = 1 ; x < width - 1; x++) {
            dst[0] = (src0[0] + src0[4] + src0[8] +
                src1[0] + src1[4] + src1[8] +
                src2[0] + src2[4] + src2[8]) / 9;
            dst[1] = (src0[5] + src0[9] + src0[13] +
                src1[5] + src1[9] + src1[13] +
                src2[5] + src2[9] + src2[13]) / 9;
            dst[2] = (src0[10] + src0[14] + src0[18] +
                src1[10] + src1[14] + src1[18] +
                src2[10] + src2[14] + src2[18]) / 9;
            dst += 4;
            src0 += 4 * 3;
            src1 += 4 * 3;
            src2 += 4 * 3;
          }

          dst[0] = (src0[0] + src0[4] + src0[8] +
              src1[0] + src1[4] + src1[8] +
              src2[0] + src2[4] + src2[8]) / 9;
          dst[1] = (src0[1] + src0[5] + src0[9] +
              src1[1] + src1[5] + src1[9] +
              src2[1] + src2[5] + src2[9]) / 9;
          dst[2] = (src0[2] + src0[6] + src0[10] +
              src1[2] + src1[6] + src1[10] +
              src2[2] + src2[6] + src2[10]) / 9;
          //dst+=4; src0+=4*3; src1+=4*3; src2+=4*3;
          dst += dstSkipBytes + 4;
          src0 = src2 + (4 * 3) + srcSkipBytes;
        }
      }
    }
  }
  else if (g_Scale == 2) {
    for (y = 0 ; y < height ; y++) {
      register BYTE* src1 = src0 + srcWidthBytes;

      for (x = 0 ; x < width ; x++) {
        dst[0] = (src0[0] + src0[4] +
            src1[0] + src1[4]) >> 2;
        dst[1] = (src0[1] + src0[5] +
            src1[1] + src1[5]) >> 2;
        dst[2] = (src0[2] + src0[6] +
            src1[2] + src1[6]) >> 2;
        dst += 4;
        src0 += 4 * 2;
        src1 += 4 * 2;
      }

      dst += dstSkipBytes;
      src0 = src1 + srcSkipBytes;
    }
  }
}


