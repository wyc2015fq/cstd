// #define MR(Y,U,V) (Y + (1.4075)*(V-128))
// #define MG(Y,U,V) (Y - (0.3455)*(U-128) - (0.7169)*(V-128))
// #define MB(Y,U,V) (Y + (1.779)*(U-128))
#include "yuv2bmp.h"
#define MY(R,G,B) (( R*(0.2989) + G*(0.5866) + B*(0.1145)))
#define MU(R,G,B) (( R*(-0.1688) + G*(-0.3312) + B*(0.5000) + 128))
#define MV(R,G,B) (( R*(0.5000) + G*(-0.4184) + B*(-0.0816) + 128))
// #define MR(Y,U,V) ((1024*Y + (1440)*(V-128))>>10)
// #define MG(Y,U,V) ((1024*Y - (354)*(U-128) - (734)*(V-128))>>10)
// #define MB(Y,U,V) ((1024*Y + (1821)*(U-128))>>10)
#define MR(Y,U,V) (Y + (((1440)*(V-128))>>10))
#define MG(Y,U,V) (Y - (((354)*(U-128))>>10) - (((734)*(V-128))>>10))
#define MB(Y,U,V) (Y + (((1821)*(U-128))>>10))
int table_y[256];
int table_vr[256]; //MR中(1440)*(V-128))
int table_ug[256]; //MG中(354)*(U-128)
int table_vg[256]; //MG中(734)*(V-128))
int table_ub[256]; //MB中(1821)*(U-128))>>10
void InitialTable()
{
  for (int i = 0; i < 256; ++i) {
    table_y[i] = i ;
  }

  for (int j = 0; j < 256; ++j) {
    table_vr[j] = (1440 * (j - 128)) >> 10;
    table_ug[j] = (354 * (j - 128)) >> 10;
    table_vg[j] = (734 * (j - 128)) >> 10;
    table_ub[j] = (1821 * (j - 128)) >> 10;
  }
}
void RGB_C_YUV420(unsigned char* pRGB, unsigned char* pYUV, int height, int width)
{
  unsigned char* pY = pYUV;
  unsigned char* pU = pYUV + height * width;
  unsigned char* pV = pU + height * (width >> 2);
  unsigned char* pBGR = NULL;
  int Y, U, V;
  int R, G, B;
  unsigned char* uu = new unsigned char[width * height];
  memset(uu, 0x0, width * height);
  unsigned char* vv = new unsigned char[width * height];
  memset(vv, 0x0, width * height);

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      pBGR = pRGB + i * width * 3 + j * 3;
      B = *pBGR;
      G = *(pBGR + 1);
      R = *(pBGR + 2);
      //Y
      Y = MY(R, G, B);
      Y = (Y < 0) ? 0 : (Y > 255 ? 255 : Y);
      //U
      U = MU(R, G, B);
      U = (U < 0) ? 0 : (U > 255 ? 255 : U);
      //V
      V = MV(R, G, B);
      V = (V < 0) ? 0 : (V > 255 ? 255 : V);
      *(pY + i * width + j) = Y;
      *(uu + i * width + j) = U;
      *(vv + i * width + j) = V;
    }
  }

  unsigned char U00, U01, U10, U11;
  unsigned char V00, V01, V10, V11;

  for (int y = 0; y < height; y = y + 2) {
    for (int x = 0; x < width; x = x + 2) {
      U00 = *(uu + y * width + x);
      U01 = *(uu + y * width + x + 1);
      U10 = *(uu + (y + 1) * width + x);
      U11 = *(uu + (y + 1) * width + x + 1);
      V00 = *(vv + y * width + x);
      V01 = *(vv + y * width + x + 1);
      V10 = *(vv + (y + 1) * width + x);
      V11 = *(vv + (y + 1) * width + x + 1);
      *pU = (U00 + U01 + U10 + U11) / 4;
      *pV = (V00 + V01 + V10 + V11) / 4;
      pU++;
      pV++;
    }
  }

  delete [] uu;
  delete [] vv;
}
void YUV420_C_RGB(unsigned char* pYUV, unsigned char* pRGB, int height, int width)
{
  unsigned char* pY = pYUV;
  unsigned char* pU = pYUV + height * width;
  unsigned char* pV = pU + ((height * width) >> 2);
  unsigned char* pBGR = NULL;
  int Y, U, V;
  int R, G, B;
  double tmp = 0;

  for (int i = 0; i < height; i = i + 2) {
    for (int j = 0; j < width; j = j + 2) {
      U = *pU++;
      V = *pV++;
      //up left
      Y = *(pY + i * width + j);
      pBGR = pRGB + i * width * 3 + j * 3;
      //B
      B = MB(Y, U, V);
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      //G
      G = MG(Y, U, V);
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      //R
      R = MR(Y, U, V);
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
      //up right
      Y = *(pY + i * width + j + 1);
      pBGR = pRGB + i * width * 3 + (j + 1) * 3;
      //B
      B = MB(Y, U, V);
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      //G
      G = MG(Y, U, V);
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      //R
      R = MR(Y, U, V);
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
      //down left
      Y = *(pY + (i + 1) * width + j);
      pBGR = pRGB + (i + 1) * width * 3 + j * 3;
      //B
      B = MB(Y, U, V);
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      //G
      G = MG(Y, U, V);
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      //R
      R = MR(Y, U, V);
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
      //down right
      pBGR = pRGB + (i + 1) * width * 3 + (j + 1) * 3;
      Y = *(pY + (i + 1) * width + j + 1);
      //B
      B = MB(Y, U, V);
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      //G
      G = MG(Y, U, V);
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      //R
      R = MR(Y, U, V);
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
    }
  }
}
void RGB_C_YUV422p(unsigned char* rgb, unsigned char* yuv, int height, int width)
{
  unsigned char* pY = yuv;
  unsigned char* pU = yuv + width * height;
  unsigned char* pV = pU + ((width * height) >> 1);
  unsigned char* pBGR = NULL;
  int Y, U, V;
  int R, G, B;
  unsigned char* uu = new unsigned char[width * height];
  memset(uu, 0x0, width * height);
  unsigned char* vv = new unsigned char[width * height];
  memset(vv, 0x0, width * height);

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      pBGR = rgb + i * width * 3 + j * 3;
      B = *pBGR;
      G = *(pBGR + 1);
      R = *(pBGR + 2);
      //Y
      Y = MY(R, G, B);
      Y = (Y < 0) ? 0 : (Y > 255 ? 255 : Y);
      //U
      U = MU(R, G, B);
      U = (U < 0) ? 0 : (U > 255 ? 255 : U);
      //V
      V = MV(R, G, B);
      V = (V < 0) ? 0 : (V > 255 ? 255 : V);
      *(pY + i * width + j) = Y;
      *(uu + i * width + j) = U;
      *(vv + i * width + j) = V;
    }
  }

  unsigned char U00, U01;
  unsigned char V00, V01;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x = x + 2) {
      U00 = *(uu + y * width + x);
      U01 = *(uu + y * width + x + 1);
      V00 = *(vv + y * width + x);
      V01 = *(vv + y * width + x + 1);
      *pU = (U00 + U01) >> 1;
      *pV = (V00 + V01) >> 1;
      pU++;
      pV++;
    }
  }

  delete [] uu;
  delete [] vv;
}
void YUV422p_C_RGB(unsigned char* yuv, unsigned char* rgb, int height, int width)
{
  unsigned char* pY = yuv;
  unsigned char* pU = yuv + height * width;
  unsigned char* pV = pU + ((height * width) >> 1);
  unsigned char* pBGR = NULL;
  int Y, U, V;
  int R, G, B;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j = j + 2) {
      U = *pU++;
      V = *pV++;
      //up left
      Y = *(pY + i * width + j);
      pBGR = rgb + i * width * 3 + j * 3;
      //B
      B = MB(Y, U, V);
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      //G
      G = MG(Y, U, V);
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      //R
      R = MR(Y, U, V);
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
      //up right
      Y = *(pY + i * width + j + 1);
      pBGR = rgb + i * width * 3 + (j + 1) * 3;
      //B
      B = MB(Y, U, V);
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      //G
      G = MG(Y, U, V);
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      //R
      R = MR(Y, U, V);
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
    }
  }
}
void RGB_C_YUV422(unsigned char* rgb, unsigned char* yuv, int height, int width)
{
  unsigned char* pBGR = NULL;
  unsigned char* pYUV = yuv;
  int Y00, Y01, U00, U01, V00, V01;
  int R, G, B;
  int U, V;

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; j = j + 2) {
      pBGR = rgb + i * width * 3 + j * 3;
      B = *pBGR;
      G = *(pBGR + 1);
      R = *(pBGR + 2);
      //Y
      Y00 = MY(R, G, B);
      Y00 = (Y00 < 0) ? 0 : (Y00 > 255 ? 255 : Y00);
      //U
      U00 = MU(R, G, B);
      U00 = (U00 < 0) ? 0 : (U00 > 255 ? 255 : U00);
      //V
      V00 = MV(R, G, B);
      V00 = (V00 < 0) ? 0 : (V00 > 255 ? 255 : V00);
      pBGR = rgb + i * width * 3 + (j + 1) * 3;
      B = *pBGR;
      G = *(pBGR + 1);
      R = *(pBGR + 2);
      //Y
      Y01 = MY(R, G, B);
      Y01 = (Y01 < 0) ? 0 : (Y01 > 255 ? 255 : Y01);
      //U
      U01 = MU(R, G, B);
      U01 = (U01 < 0) ? 0 : (U01 > 255 ? 255 : U01);
      //V
      V01 = MV(R, G, B);
      V01 = (V01 < 0) ? 0 : (V01 > 255 ? 255 : V01);
      U = (U00 + U01) >> 1;
      V = (V00 + V01) >> 1;
      *pYUV++ = Y00;
      *pYUV++ = U;
      *pYUV++ = Y01;
      *pYUV++ = V;
    }
  }
}
void YUV422_C_RGB(unsigned char* yuv, unsigned char* rgb, int height, int width)
{
  unsigned char* pYUV = yuv;
  unsigned char* pBGR = NULL;
  int Y00, Y01, U, V;
  int R, G, B;

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; j = j + 2) {
      U = *pYUV++;
      Y00 = *pYUV++;
      V = *pYUV++;
      Y01 = *pYUV++;
      //up left
      pBGR = rgb + i * width * 3 + j * 3;
      R = table_y[Y00] + table_vr[V];
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      G = table_y[Y00] - table_ug[U] - table_vg[V];
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      B = table_y[Y00] + table_ub[U];
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
      //up right
      pBGR = rgb + i * width * 3 + (j + 1) * 3;
      R = table_y[Y01] + table_vr[V];
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      G = table_y[Y01] - table_ug[U] - table_vg[V];
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      B = table_y[Y01] + table_ub[U];
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
    }
  }
}
void YUYV_C_RGB(unsigned char* yuv, unsigned char* rgb, int height, int width)
{
  unsigned char* pYUV = yuv;
  unsigned char* pBGR = NULL;
  int Y00, Y01, U, V;
  int R, G, B;

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; j = j + 2) {
      Y00 = *pYUV++;
      U = *pYUV++;
      Y01 = *pYUV++;
      V = *pYUV++;
      //up left
      pBGR = rgb + (height - i - 1) * width * 3 + (width - 1 - j) * 3;
      R = table_y[Y00] + table_vr[V];
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      G = table_y[Y00] - table_ug[U] - table_vg[V];
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      B = table_y[Y00] + table_ub[U];
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
      //up right
      pBGR = rgb + (height - i - 1) * width * 3 + (width - 2 - j) * 3;
      R = table_y[Y01] + table_vr[V];
      R = (R < 0) ? 0 : (R > 255 ? 255 : R);
      G = table_y[Y01] - table_ug[U] - table_vg[V];
      G = (G < 0) ? 0 : (G > 255 ? 255 : G);
      B = table_y[Y01] + table_ub[U];
      B = (B < 0) ? 0 : (B > 255 ? 255 : B);
      *(pBGR + 0) = B;
      *(pBGR + 1) = G;
      *(pBGR + 2) = R;
    }
  }
}
BOOL YUV422TORGB24(BYTE* pDest, int width, int height, int channel, const BYTE* pSrc)
{
  int offset1 = width * height;
  int offset2 = offset1 + (width >> 1) * height;

  for (int i = 0; i < height; i++) {
    int widthOffset1 = i * width;
    int index1 = widthOffset1 * channel;

    for (int j = 0; j < width; j += 2) {
      {
        int y = pSrc[widthOffset1 + j];
        int u = pSrc[offset1 + ((widthOffset1 + j) >> 1)];
        int v = pSrc[offset2 + ((widthOffset1 + j) >> 1)];
        int yy = y - 16;
        int b = table_y[yy] + table_ub[u];
        int g = table_y[yy] - table_ug[u] - table_vg[v];
        int r = table_y[yy] + table_vr[v];

        if (r > 255) {
          r = 255;
        }

        if (r < 0) {
          r = 0;
        }

        if (g > 255) {
          g = 255;
        }

        if (g < 0) {
          g = 0;
        }

        if (b > 255) {
          b = 255;
        }

        if (b < 0) {
          b = 0;
        }

        int index = index1 + j * channel;
        pDest[index] = b;
        pDest[index + 1] = g;
        pDest[index + 2] = r;
      }
      {
        int y = pSrc[widthOffset1 + (j + 1)];
        int u = pSrc[offset1 + ((widthOffset1 + (j + 1)) >> 1)];
        int v = pSrc[offset2 + ((widthOffset1 + (j + 1)) >> 1)];
        int yy = y - 16;
        int b = table_y[yy] + table_ub[u];
        int g = table_y[yy] - table_ug[u] - table_vg[v];
        int r = table_y[yy] + table_vr[v];

        if (r > 255) {
          r = 255;
        }

        if (r < 0) {
          r = 0;
        }

        if (g > 255) {
          g = 255;
        }

        if (g < 0) {
          g = 0;
        }

        if (b > 255) {
          b = 255;
        }

        if (b < 0) {
          b = 0;
        }

        int index = index1 + (j + 1) * channel;
        pDest[index] = b;
        pDest[index + 1] = g;
        pDest[index + 2] = r;
      }
    }
  }

  return TRUE;
}
#ifndef WIN32
typedef unsigned short WORD;
#define BI_RGB 0
#endif
#pragma pack(2)
typedef struct tBITMAPFILEHEADER {
  WORD bfType;
  unsigned int bfSize;
  WORD bfReserved1;
  WORD bfReserved2;
  unsigned int bfOffBits;
} tBITMAPFILEHEADER;
#pragma pack()
typedef struct tBITMAPINFOHEADER {
  unsigned int biSize;
  unsigned int biWidth;
  unsigned int biHeight;
  WORD biPlanes;
  WORD biBitCount;
  unsigned int biCompression;
  unsigned int biSizeImage;
  unsigned int biXPelsPerMeter;
  unsigned int biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
} tBITMAPINFOHEADER;
void SaveRGB(unsigned char* rgb, int height, int width, const char* path)
{
  tBITMAPFILEHEADER bmpFHdr = {0};
  tBITMAPINFOHEADER bmpIHdr = {0};
  bmpFHdr.bfType = 0x4D42;
  bmpFHdr.bfOffBits = 54;
  bmpFHdr.bfSize = sizeof(tBITMAPFILEHEADER) + sizeof(tBITMAPINFOHEADER) + width * 3 * height;
  bmpIHdr.biSize = sizeof(tBITMAPINFOHEADER);
  bmpIHdr.biWidth = width;
  bmpIHdr.biHeight = height;
  bmpIHdr.biPlanes = 1;
  bmpIHdr.biBitCount = 24;
  bmpIHdr.biCompression = BI_RGB;
  bmpIHdr.biSizeImage = width * 3 * height;
  bmpIHdr.biXPelsPerMeter = 0;
  bmpIHdr.biYPelsPerMeter = 0;
  bmpIHdr.biClrUsed = 0;
  bmpIHdr.biClrImportant = 0;
  FILE* pfile = fopen(path, "wb");

  if (pfile) {
    fwrite(&bmpFHdr, sizeof(tBITMAPFILEHEADER), 1, pfile);
    fwrite(&bmpIHdr, sizeof(tBITMAPINFOHEADER), 1, pfile);
    fwrite(rgb, width * 3 * height, 1, pfile);
    fclose(pfile);
  }
}
void RGB_EDDY(unsigned char* src_rgb, int height, int width, unsigned char* dest_rgb)
{
  int i, j = 0;

  for (i = 0; i < width; i += 2) {
    for (j = 0; j < height; ++j) {
      dest_rgb[i * height * 3 + j * 3 + 0] = src_rgb[j * width * 3 + (width - 1 - i) * 3 + 0];
      dest_rgb[i * height * 3 + j * 3 + 1] = src_rgb[j * width * 3 + (width - 1 - i) * 3 + 1];
      dest_rgb[i * height * 3 + j * 3 + 2] = src_rgb[j * width * 3 + (width - 1 - i) * 3 + 2];
      dest_rgb[(i + 1)*height * 3 + j * 3 + 0] = src_rgb[j * width * 3 + (width - 1 - (i + 1)) * 3 + 0];
      dest_rgb[(i + 1)*height * 3 + j * 3 + 1] = src_rgb[j * width * 3 + (width - 1 - (i + 1)) * 3 + 1];
      dest_rgb[(i + 1)*height * 3 + j * 3 + 2] = src_rgb[j * width * 3 + (width - 1 - (i + 1)) * 3 + 2];
    }
  }
}

