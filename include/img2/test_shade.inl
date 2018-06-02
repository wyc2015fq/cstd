enum {
  SHS_NOISE = 0,
  SHS_DIAGSHADE,
  SHS_HSHADE,
  SHS_VSHADE,
  SHS_HBUMP,
  SHS_VBUMP,
  SHS_SOFTBUMP,
  SHS_HARDBUMP,
  SHS_METAL,
  SHS_MAX,
};
int getshade(uint* pseed, int sYSize, int sXSize, BYTE* iDst, int bytes, UINT shadeID, BYTE granularity)
{
  long j, i, k, h = 0;
  BYTE* posDst;
  COLOR m_dNormal_pal[256];
  long r, g, b; //build the shaded palette
  int a, x, y, d, xs, idxmax, idxmin, grainx2;
  posDst = iDst;
  grainx2 = MYRANDMAX16 / (MAX(1, 2 * granularity));
  idxmax = 255 - granularity;
  idxmin = granularity;
  switch (shadeID) {
  case SHS_METAL: //SHS_METAL
    // create the strokes
    k = 40; //stroke granularity
    for (i = 0; i < sYSize; ++i) {
      memset(iDst + i * bytes, 0, sXSize);
    }
    for (a = 0; a < 200; a++) {
      x = sXSize * myrand16(pseed) / MYRANDMAX16; //stroke postion
      y = sYSize * myrand16(pseed) / MYRANDMAX16; //stroke position
      xs = MIN(sXSize, sYSize) * myrand16(pseed) / (2 * MYRANDMAX16); //stroke lenght
      d = myrand16(pseed) / (MYRANDMAX16 / k); //stroke color
      for (i = 0; i < xs; i++) {
        if (((x - i) > 0) && ((y + i) < sYSize)) {
          iDst[(x - i) + bytes * (y + i)] = (BYTE)d;
        }
        if (((x + i) < sXSize) && ((y - i) > 0)) {
          iDst[(sXSize - x + i) + bytes * (y - i)] = (BYTE)d;
        }
      }
    }
    //blend strokes with SHS_DIAGONAL
    posDst = iDst;
    a = (idxmax - idxmin - k) / 2;
    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        d = posDst[j] + ((a * i) / sYSize + (a * (sXSize - j)) / sXSize);
        if (j > 100) {
          int asdf = 0;
        }
        posDst[j] = (BYTE)d;
        posDst[j] += myrand16(pseed) / grainx2;
      }
      posDst += bytes;
    }
    break;
  case SHS_HARDBUMP: //
    //set horizontal bump
    for (i = 0; i < sYSize; i++) {
      k = (255 * i / sYSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - granularity * 2)) / 128 + 128;
      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += myrand16(pseed) / grainx2 - granularity;
      }
      posDst += bytes;
    }
    //set vertical bump
    d = MIN(16, sXSize / 6); //max edge=16
    a = 1 + sYSize * sYSize / 4;
    posDst = iDst;
    for (i = 0; i < sYSize; i++) {
      y = i - sYSize / 2;
      for (j = 0; j < sXSize; j++) {
        x = j - sXSize / 2;
        xs = sXSize / 2 - d + (y * y * d) / a;
        if (x > xs) {
          posDst[j] = idxmin + (BYTE)(((sXSize - j) * 128) / d);
        }
        if ((x + xs) < 0) {
          posDst[j] = idxmax - (BYTE)((j * 128) / d);
        }
        posDst[j] += myrand16(pseed) / grainx2 - granularity;
      }
      posDst += bytes;
    }
    break;
  case SHS_SOFTBUMP: //
    for (i = 0; i < sYSize; i++) {
      h = (255 * i / sYSize) - 127;
      for (j = 0; j < sXSize; j++) {
        k = (255 * (sXSize - j) / sXSize) - 127;
        k = (h * (h * h) / 128) / 128 + (k * (k * k) / 128) / 128;
        k = k * (128 - granularity) / 128 + 128;
        if (k < idxmin) {
          k = idxmin;
        }
        if (k > idxmax) {
          k = idxmax;
        }
        posDst[j] = (BYTE)k;
        posDst[j] += myrand16(pseed) / grainx2 - granularity;
      }
      posDst += bytes;
    }
    break;
  case SHS_VBUMP: //
    for (j = 0; j < sXSize; j++) {
      k = (255 * (sXSize - j) / sXSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - granularity)) / 128 + 128;
      for (i = 0; i < sYSize; i++) {
        posDst[j + i * bytes] = (BYTE)k;
        posDst[j + i * bytes] += myrand16(pseed) / grainx2 - granularity;
      }
    }
    break;
  case SHS_HBUMP: //
    for (i = 0; i < sYSize; i++) {
      k = (255 * i / sYSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - granularity)) / 128 + 128;
      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += myrand16(pseed) / grainx2 - granularity;
      }
      posDst += bytes;
    }
    break;
  case SHS_DIAGSHADE: //
    a = (idxmax - idxmin) / 2;
    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)(idxmin + a * i / sYSize + a * (sXSize - j) / sXSize);
        posDst[j] += myrand16(pseed) / grainx2 - granularity;
      }
      posDst += bytes;
    }
    break;
  case SHS_HSHADE:
    a = idxmax - idxmin;
    for (i = 0; i < sYSize; i++) {
      k = a * i / sYSize + idxmin;
      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += myrand16(pseed) / grainx2 - granularity;
      }
      posDst += bytes;
    }
    break;
  case SHS_VSHADE:
    a = idxmax - idxmin;
    for (j = 0; j < sXSize; j++) {
      k = a * (sXSize - j) / sXSize + idxmin;
      for (i = 0; i < sYSize; i++) {
        posDst[j + i * bytes] = (BYTE)k;
        posDst[j + i * bytes] += myrand16(pseed) / grainx2 - granularity;
      }
    }
    break;
  case SHS_NOISE:
    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        posDst[j] = 128 + myrand16(pseed) / grainx2 - granularity;
      }
      posDst += bytes;
    }
    break;
  default: //SHS_NOISE
    break;
  }
  return 0;
}
int img_map(int h, int w, uchar* dst, int dst_step, const uchar* src, int src_step, const COLOR* pal)
{
  int i, j;
  for (i = 0; i < h; i++) {
    const uchar* src1 = src + i * src_step;
    COLOR* dst1 = (COLOR*)(dst + i * dst_step);
    for (j = 0; j < w; j++) {
      dst1[j] = pal[src1[j]];
    }
  }
  return 0;
}
int test_shade()
{
  img_t im[10] = {0};
  img_t* im1 = im + 1;
  uint pseed[1] = {0};
  int h = 512, w = h, id = 0;
  COLOR clrs[] = {_rgb(1, 0, 0), _rgb(0, 1, 0), _rgb(0, 0, 1)};
  COLOR pal[256] = {0};
  fill_color_array2(countof(pal), pal, countof(clrs), clrs, NULL);
  imsetsize(im, h, w, 4, 1);
  imsetsize(im1, h, w, 1, 1);
  for (;; ++id) {
    id %= SHS_MAX;
    getshade(pseed, h, w, im1->tt.data, im1->s, id, 10);
    img_map(h, w, im->tt.data, im->s, im1->tt.data, im1->s, pal);
    imshow(im1);
    imshow(im);
    cvWaitKey(-1);
  }
  imfrees(im, countof(im));
  return 0;
}

