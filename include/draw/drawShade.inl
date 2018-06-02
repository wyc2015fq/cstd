enum {
  SHS_NOISE = 0, SHS_DIAGSHADE, SHS_HSHADE, SHS_VSHADE, SHS_HBUMP, SHS_VBUMP, SHS_SOFTBUMP, SHS_HARDBUMP, SHS_METAL
};
int TDrawShade(HDDC hDC, uint shadeID, BYTE granularity, COLORREF color)
{
  long sXSize, sYSize, bytes, j, i, k, h;
  long grainx2, a, x, y, d, xs, idxmax, idxMIN;
  DWORD* iDst, *posDst;
  COLORREF hicr = _GetSysColor(COLOR_BTNHIGHLIGHT); //get the button base colors
  COLORREF midcr = _GetSysColor(COLOR_BTNFACE);
  COLORREF locr = _GetSysColor(COLOR_BTNSHADOW);
  long r, g, b; //build the shaded palette
  DWORD pal[256];
  sYSize = hDC->h;
  sXSize = hDC->w;

  for (i = 0; i < 129; i++) {
    r = ((128 - i) * GetRValue(locr) + i * GetRValue(midcr)) / 128;
    g = ((128 - i) * GetGValue(locr) + i * GetGValue(midcr)) / 128;
    b = ((128 - i) * GetBValue(locr) + i * GetBValue(midcr)) / 128;
    pal[i] = _RGB(r, g, b);
  }

  for (i = 1; i < 129; i++) {
    r = ((128 - i) * GetRValue(midcr) + i * GetRValue(hicr)) / 128;
    g = ((128 - i) * GetGValue(midcr) + i * GetGValue(hicr)) / 128;
    b = ((128 - i) * GetBValue(midcr) + i * GetBValue(hicr)) / 128;
    pal[i + 127] = _RGB(r, g, b);
  }

  iDst = hDC->data; //build the horiz. dotted focus bitmap
  j = hDC->w;
  bytes = hDC->bw >> 2;

  for (i = 0; i < j; i++) {
    // iDst[i]=64+127*(i%2); //soft
    iDst[i] = 255 * (i % 2); //hard
  }

  j = (long)hDC->h;

  for (i = 0; i < j; i++) {
    // *iDst=64+127*(i%2); //soft
    iDst[i * bytes] = 255 * (i % 2); //hard
  }

  posDst = iDst;
  grainx2 = RAND_MAX / (MAX(1, 2 * granularity));
  idxmax = 255 - granularity;
  idxMIN = granularity;

  switch (shadeID) {
    //----------------------------------------------------
  case 8:
    //SHS_METAL
    // create the strokes
    k = 40; //stroke granularity
#define SetPixelIndex(_x, _y, _i) _SetPixel(hDC, _x, _y, pal[_i])

    for (a = 0; a < 200; a++) {
      x = rand() / (RAND_MAX / sXSize); //stroke postion
      y = rand() / (RAND_MAX / sYSize); //stroke position
      xs = rand() / (RAND_MAX / MIN(sXSize, sYSize)) / 2; //stroke lenght
      d = rand() / (RAND_MAX / k); //stroke color

      for (i = 0; i < xs; i++) {
        if (((x - i) > 0) && ((y + i) < sYSize)) {
          SetPixelIndex(x - i, y + i, (BYTE)d);
        }

        if (((x + i) < sXSize) && ((y - i) > 0)) {
          SetPixelIndex(sXSize - x + i, y - i, (BYTE)d);
        }
      }
    }

    //blend strokes with SHS_DIAGONAL
    posDst = iDst;
    a = (idxmax - idxMIN - k) / 2;

    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        d = posDst[j] + ((a * i) / sYSize + (a * (sXSize - j)) / sXSize);
        d += rand() / grainx2;
        posDst[j] = d;
      }

      posDst += bytes;
    }

    break;

    //----------------------------------------------------
  case 7:

    // SHS_HARDBUMP
    //set horizontal bump
    for (i = 0; i < sYSize; i++) {
      k = (255 * i / sYSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - granularity * 2)) / 128 + 128;

      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += rand() / grainx2 - granularity;
      }

      posDst += bytes;
    }

    //set vertical bump
    d = MIN(16, sXSize / 6); //max edge=16
    a = sYSize * sYSize / 4;
    posDst = iDst;

    for (i = 0; i < sYSize; i++) {
      y = i - sYSize / 2;

      for (j = 0; j < sXSize; j++) {
        x = j - sXSize / 2;
        xs = sXSize / 2 - d + (y * y * d) / a;

        if (x > xs) {
          posDst[j] = idxMIN + (BYTE)(((sXSize - j) * 128) / d);
        }

        if ((x + xs) < 0) {
          posDst[j] = idxmax - (BYTE)((j * 128) / d);
        }

        posDst[j] += rand() / grainx2 - granularity;
      }

      posDst += bytes;
    }

    break;

    //----------------------------------------------------
  case 6:

    //SHS_SOFTBUMP
    for (i = 0; i < sYSize; i++) {
      h = (255 * i / sYSize) - 127;

      for (j = 0; j < sXSize; j++) {
        k = (255 * (sXSize - j) / sXSize) - 127;
        k = (h * (h * h) / 128) / 128 + (k * (k * k) / 128) / 128;
        k = k * (128 - granularity) / 128 + 128;

        if (k < idxMIN) {
          k = idxMIN;
        }

        if (k > idxmax) {
          k = idxmax;
        }

        posDst[j] = (BYTE)k;
        posDst[j] += rand() / grainx2 - granularity;
      }

      posDst += bytes;
    }

    break;

    //----------------------------------------------------
  case 5:

    // SHS_VBUMP
    for (j = 0; j < sXSize; j++) {
      k = (255 * (sXSize - j) / sXSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - granularity)) / 128 + 128;

      for (i = 0; i < sYSize; i++) {
        posDst[j + i * bytes] = (BYTE)k;
        posDst[j + i * bytes] += rand() / grainx2 - granularity;
      }
    }

    break;

    //----------------------------------------------------
  case 4:

    //SHS_HBUMP
    for (i = 0; i < sYSize; i++) {
      k = (255 * i / sYSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - granularity)) / 128 + 128;

      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += rand() / grainx2 - granularity;
      }

      posDst += bytes;
    }

    break;

    //----------------------------------------------------
  case 1:
    //SHS_DIAGSHADE
    a = (idxmax - idxMIN) / 2;

    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)(idxMIN + a * i / sYSize + a * (sXSize - j) / sXSize);
        posDst[j] += rand() / grainx2 - granularity;
      }

      posDst += bytes;
    }

    break;

    //----------------------------------------------------
  case 2:
    //SHS_HSHADE
    a = idxmax - idxMIN;

    for (i = 0; i < sYSize; i++) {
      k = a * i / sYSize + idxMIN;

      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += rand() / grainx2 - granularity;
      }

      posDst += bytes;
    }

    break;

    //----------------------------------------------------
  case 3:
    //SHS_VSHADE:
    a = idxmax - idxMIN;

    for (j = 0; j < sXSize; j++) {
      k = a * (sXSize - j) / sXSize + idxMIN;

      for (i = 0; i < sYSize; i++) {
        posDst[j + i * bytes] = (BYTE)k;
        posDst[j + i * bytes] += rand() / grainx2 - granularity;
      }
    }

    break;

    //----------------------------------------------------
  default:

    //SHS_NOISE
    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        posDst[j] = 128 + rand() / grainx2 - granularity;
      }

      posDst += bytes;
    }
  }

  posDst = iDst;

  for (i = 0; i < sYSize; i++) {
    for (j = 0; j < sXSize; j++) {
      //ASSERT(posDst[j]<256);
      posDst[j] = pal[(BYTE)posDst[j]];
    }

    posDst += bytes;
  }

  return 0;
}
int test_DrawShade(HDDC hDC, int ch)
{
  static int t = SHS_DIAGSHADE;

  if (' ' == ch) {
    t = (t + 1) % 9;
  }

  TDrawShade(hDC, t, 8, 0);
  return 0;
}

