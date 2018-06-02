
int FillBitmapInfo1(BITMAPINFO* bmi, int width, int height, int bpp, int origin, const RGBQUAD* inpal)
{
  BITMAPINFOHEADER* bmih;
  assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
  bmih = &(bmi->bmiHeader);
  memset(bmih, 0, sizeof(*bmih));
  bmih->biSize = sizeof(BITMAPINFOHEADER);
  bmih->biWidth = width;
  bmih->biHeight = origin ? abs(height) : -abs(height);
  bmih->biPlanes = 1;
  bmih->biBitCount = (unsigned short) bpp;
  bmih->biCompression = BI_RGB;

  if (bpp <= 8) {
    RGBQUAD* palette = bmi->bmiColors;

    if (inpal) {
      memcpy(palette, inpal, 256 * sizeof(RGBQUAD));
    }
    else {
      int i;

      for (i = 0; i < 256; i++) {
        palette[ i ].rgbBlue = palette[ i ].rgbGreen = palette[ i ].rgbRed = (BYTE) i;
        palette[ i ].rgbReserved = 0;
      }
    }
  }

  return 0;
}
HBITMAP mkHBITMAP(int height, int width, const unsigned char* arr, int step, int cn)
{
  HBITMAP hBmp;
  uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
  BITMAPINFO* binfo = (BITMAPINFO*) buffer;
  HDC hdc = GetDC(0);
  void* dst_ptr = 0;
  FillBitmapInfo1(binfo, width, height, cn * 8, 1, 0);
  hBmp = CreateDIBSection(hdc, binfo, DIB_RGB_COLORS, &dst_ptr, 0, 0);
  memcpy(dst_ptr, arr, height * width * cn);
  ReleaseDC(0, hdc);
  return hBmp;
}
BOOL bitmap2im(HBITMAP hBmp, img_t* im)
{
  int nChannels, n;
  int depth;
  BITMAP bmp;
  GetObject(hBmp, sizeof(BITMAP), &bmp);
  nChannels = bmp.bmBitsPixel == 1 ? 1 : 4;
  depth = bmp.bmBitsPixel == 1 ? 1 : 8;
  imsetsize(im, bmp.bmWidth, bmp.bmHeight, nChannels, 1);
  n = GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, im->data);

  return 0;
}

// 二次线性插值 Bilinear Bicubic
#define INTER_BILINEARITY(in, x0, y0, wl, bi, B)                              \
  do {                                                                          \
    int x1=(x0)& SHIFT_MARK, y1=(y0)& SHIFT_MARK, bii;                          \
    int pos = (((y0)>>SHIFT) * wl + ((x0)>>SHIFT) * bi);                        \
    int pm3 = SHIFT_MUL(x1, y1);                                                \
    int pm2 = y1-pm3, pm1 = x1-pm3, pm0 = SHIFT1-y1-x1+pm3;                     \
    for (bii=0; bii<bi; ++pos, ++bii)                                           \
      *(B+bii) = (int)(pm0 * in[pos] + pm1 * in[pos + bi]                       \
          + pm2 * in[pos + wl] + pm3 * in[pos + wl + bi] + (SHIFT1-1))>>SHIFT;\
  } while(0)

// 进行缩放变换，使用例子：
// INTER - 插值宏
// IMRESIZE(h, w, A, al, ai, h*sc, w*sc, A0, w*sc, 1, INTER_BILINEARITY);
#define IMRESIZE(ah, aw, A, al, ai, bh, bw, B, bl, bi, _INTER)                \
  do {                                                                          \
    if ((ah)==(bh) && (aw)==(bw) && (ai)==(bi)) {                               \
      SKIP_COPY(ah, aw*ai, A, al, 1, B, bl, 1);                                 \
    } else {                                                                    \
      int _I, _J, _bh=(int)(bh);                                                \
      int _X = (int)SHIFT_DIV(aw-1, bw);                                          \
      int _Y = (int)SHIFT_DIV(ah-1, bh);                                          \
      int bwi = (int)(bw * bi);                                                 \
      int bll = (int)((int)(bl) - bwi);                                         \
      int _B = 0;                                                               \
      int Be;                                                                   \
      for (_I=0; _bh--; _I+=_Y, _B+=bll) {                                      \
        for (_J=0, Be=_B+bwi; _B!=Be; _J+=_X, _B+=bi) {                         \
          _INTER((A), _J, _I, al, ai, (B+_B));                                  \
        }                                                                       \
      }                                                                         \
    }                                                                           \
  } while(0)

int imresize(const img_t* s, int dh, int dw, img_t* d)
{
  int sw = s->width, sh = s->height;
  int tw = dw, th = dh;

  if (NULL == s->data || 1 >= s->height || 1 >= s->width || 1 >= s->step) {
    return 0;
  }

  if (1) {
    int x, y;
    tw = sw * dh;
    th = sh * dw;

    if (tw > th) {
      tw = dw, th /= sw, y = (dh - th) / 2;
    }
    else {
      th = dh, tw /= sh, x = (dw - tw) / 2;
    }
  }

  imsetsize(d, th, tw, s->cn, s->frames);
  IMRESIZE((s->height), s->width, s->data, s->step, s->cn, (d->height), d->width, d->data, d->step, d->cn, INTER_BILINEARITY);
  return 0;
}

HBITMAP HBITMAP_resize(HBITMAP hBmp, double ss)
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  int h, w;
  bitmap2im(hBmp, im);
  h = (int)(ss * im->height);
  w = (int)(ss * im->width);
  imresize(im, h, w, im2);
  hBmp = mkHBITMAP(h, w, im2->data, im2->step, im2->cn);
  imfree(im);
  imfree(im2);
  return hBmp;
}
int test_HBITMAP()
{
  int h = 100, w = 100, al = w * 4;
  unsigned char arr[100 * 100 * 4] = {0};
  HBITMAP hBmp = 0;
  HBITMAP hBmp2 = 0;
  img_t im[1] = {0};
  memset(arr, 255, 100 * 100 * 2);
  hBmp = mkHBITMAP(h, w, arr, al, 4);
  hBmp2 = HBITMAP_resize(hBmp, 2);
  bitmap2im(hBmp2, im);
  //imshow(im);
  //cvWaitKey(-1);
  return 0;
}