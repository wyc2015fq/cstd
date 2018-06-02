// Definitions for image types.
#define TGA_Null 0
#define TGA_Map 1
#define TGA_RGB 2
#define TGA_Mono 3
#define TGA_RLEMap 9
#define TGA_RLERGB 10
#define TGA_RLEMono 11
#define TGA_CompMap 32
#define TGA_CompMap4 33
void tga_toh(TGAHEADER* p)
{
  p->CmapIndex = _ntohs(p->CmapIndex);
  p->CmapLength = _ntohs(p->CmapLength);
  p->X_Origin = _ntohs(p->X_Origin);
  p->Y_Origin = _ntohs(p->Y_Origin);
  p->ImageWidth = _ntohs(p->ImageWidth);
  p->ImageHeight = _ntohs(p->ImageHeight);
}
void SetGrayPalette(RGBQUAD* pal, int biClrUsed)
{
  int ni;

  for (ni = 0; ni < biClrUsed; ni++) {
    pal[ni].rgbBlue = pal[ni].rgbGreen = pal[ni].rgbRed = (BYTE)(ni * (255 / (biClrUsed - 1)));
  }
}
void ExpandUncompressedLine(BYTE* pDest, TGAHEADER* ptgaHead, const char* buf, int width, int y, int xoffset)
{
  int x;

  switch (ptgaHead->PixelDepth) {
  case 8:
    memcpy(pDest, buf, width);
    break;

  case 15:
  case 16: {
    BYTE* dst = pDest;
    WORD pixel;

    for (x = 0; x < width; x++) {
      pixel = *(WORD*)buf;
      buf += 2;
      *dst++ = (BYTE)((pixel & 0x1F) * 8); // blue
      *dst++ = (BYTE)((pixel >> 2) & 0x0F8); // green
      *dst++ = (BYTE)((pixel >> 7) & 0x0F8); // red
    }

    break;
  }

  case 24:
    memcpy(pDest, buf, 3 * width);
    break;

  case 32: {
    BYTE* dst = pDest;

    for (x = 0; x < width; x++) {
      RGBQUAD pixel;
      pixel = *(RGBQUAD*)buf;
      buf += 4;
      *dst++ = pixel.rgbBlue;
      *dst++ = pixel.rgbGreen;
      *dst++ = pixel.rgbRed;
      *dst++ = pixel.rgbReserved; //alpha
    }

    break;
  }
  }
}
BYTE ExpandCompressedLine(BYTE* pDest, TGAHEADER* ptgaHead, const BYTE* buf, int width, int y, BYTE rleLeftover)
{
  int x, ix;
  BYTE rle;

  for (x = 0; x < width;) {
    if (rleLeftover != 255) {
      rle = rleLeftover;
      rleLeftover = 255;
    }
    else {
      rle = *(BYTE*)buf;
      buf++;
    }

    if (rle & 128) {
      // RLE-Encoded packet
      rle -= 127; // Calculate real repeat count.

      if ((x + rle) > width) {
        rleLeftover = (BYTE)(128 + (rle - (width - x) - 1));
        rle = (BYTE)(width - x);
      }

      switch (ptgaHead->PixelDepth) {
      case 32: {
        RGBQUAD color;
        color = *(RGBQUAD*)buf;
        buf += sizeof(RGBQUAD);

        for (ix = 0; ix < rle; ix++) {
          memcpy(&pDest[3 * ix], &color, 4);
        }

        break;
      }

      case 24: {
        rgb_color triple;
        triple = *(rgb_color*)buf;
        buf += 3;

        for (ix = 0; ix < rle; ix++) {
          memcpy(&pDest[3 * ix], &triple, 3);
        }

        break;
      }

      case 15:
      case 16: {
        WORD pixel;
        rgb_color triple;
        pixel = *(WORD*)buf;
        buf += 2;
        triple.r = (BYTE)((pixel & 0x1F) * 8); // red
        triple.g = (BYTE)((pixel >> 2) & 0x0F8); // green
        triple.b = (BYTE)((pixel >> 7) & 0x0F8); // blue

        for (ix = 0; ix < rle; ix++) {
          memcpy(&pDest[3 * ix], &triple, 3);
        }

        break;
      }

      case 8: {
        BYTE pixel;
        pixel = *(BYTE*)buf;
        buf += 1;

        for (ix = 0; ix < rle; ix++) {
          pDest[ix] = pixel;
        }
      }
      }
    }
    else {
      // Raw packet
      rle += 1; // Calculate real repeat count.

      if ((x + rle) > width) {
        rleLeftover = (BYTE)(rle - (width - x) - 1);
        rle = (BYTE)(width - x);
      }

      ExpandUncompressedLine(pDest, ptgaHead, buf, rle, y, x);
    }

    x += rle;
  }

  return rleLeftover;
}
int loadtgamem(const char* aInputStream, int bpp, int revY, Image* im)
{
  const char* buf = aInputStream;
  TGAHEADER tgaHead = *(TGAHEADER*)buf;
  BOOL bCompressed;
  RGBQUAD pal[256];
  tga_toh(&tgaHead);

  switch (tgaHead.ImageType) {
  case TGA_Map:
  case TGA_RGB:
  case TGA_Mono:
    bCompressed = FALSE;
    break;

  case TGA_RLEMap:
  case TGA_RLERGB:
  case TGA_RLEMono:
    bCompressed = TRUE;
    break;

  default:
    assert(0 && "Unknown TGA image type");
  }

  if (tgaHead.ImageWidth == 0 || tgaHead.ImageHeight == 0 || tgaHead.PixelDepth == 0 || tgaHead.CmapLength > 256) {
    assert(0 && "bad TGA header");
  }

  if (tgaHead.PixelDepth != 8 && tgaHead.PixelDepth != 15 && tgaHead.PixelDepth != 16 && tgaHead.PixelDepth != 24 && tgaHead.PixelDepth != 32) {
    assert(0 && "bad TGA header");
  }

  if (tgaHead.IdLength > 0) {
    buf += (tgaHead.IdLength);
  }

  //skip descriptor
  newImage(tgaHead.ImageWidth, tgaHead.ImageHeight, tgaHead.PixelDepth, im);

  if (tgaHead.CmapType != 0) {
    // read the palette
    int i;
    rgb_color* pal1 = (rgb_color*)buf;

    for (i = 0; i < tgaHead.CmapLength; i++) {
      pal[i].rgbBlue = pal1[i].b;
      pal[i].rgbGreen = pal1[i].g;
      pal[i].rgbRed = pal1[i].r;
      pal[i].rgbReserved = 0;
    }
  }

  if (tgaHead.ImageType == TGA_Mono || tgaHead.ImageType == TGA_RLEMono) {
    SetGrayPalette(pal, tgaHead.CmapLength);
  }

  // Bits 4 & 5 of the Image Descriptor byte control the ordering of the pixels.
  {
    int y;
    BOOL bXReversed = ((tgaHead.ImagDesc & 16) == 16);
    BOOL bYReversed = ((tgaHead.ImagDesc & 32) == 32);
    BYTE rleLeftover = 255; //for images with illegal packet boundary
    BYTE* pDest;

    for (y = 0; y < tgaHead.ImageHeight; y++) {
      if (bYReversed) {
        pDest = (BYTE*)getLinePixels(im, tgaHead.ImageHeight - y - 1);
      }
      else {
        pDest = (BYTE*)getLinePixels(im, y);
      }

      if (bCompressed) {
        rleLeftover = ExpandCompressedLine(pDest, &tgaHead, buf, tgaHead.ImageWidth, y, rleLeftover);
      }
      else {
        ExpandUncompressedLine(pDest, &tgaHead, buf, tgaHead.ImageWidth, y, 0);
      }
    }
  }
  return im;
}
#if 0
BOOL CxImageTGA::Encode(CxFile* hFile)
{
  if (EncodeSafeCheck(hFile)) {
    return FALSE;
  }

  if (head.biBitCount < 8) {
    strcpy(info.szLastError, "Bit depth must be 8 or 24");
    return FALSE;
  }

  TGAHEADER tgaHead;
  tgaHead.IdLength = 0; // Image ID Field Length
  tgaHead.CmapType = GetPalette() != 0; // Color Map Type
  tgaHead.ImageType = (head.biBitCount == 8) ? (BYTE)TGA_Map : (BYTE)TGA_RGB; // Image Type
  tgaHead.CmapIndex = 0; // First Entry Index
  tgaHead.CmapLength = (head.biBitCount == 8) ? 256 : 0; // Color Map Length
  tgaHead.CmapEntrySize = (head.biBitCount == 8) ? (BYTE)24 : (BYTE)0; // Color Map Entry Size
  tgaHead.X_Origin = 0; // X-origin of Image
  tgaHead.Y_Origin = 0; // Y-origin of Image
  tgaHead.ImageWidth = (WORD)head.biWidth; // Image Width
  tgaHead.ImageHeight = (WORD)head.biHeight; // Image Height
  tgaHead.PixelDepth = (BYTE)head.biBitCount; // Pixel Depth
  tgaHead.ImagDesc = 0; // Image Descriptor

  if (pAlpha && head.biBitCount == 24) {
    tgaHead.PixelDepth = 32;
  }

  tga_toh(&tgaHead);
  hFile->Write(&tgaHead, sizeof(TGAHEADER), 1);
  tga_toh(&tgaHead);

  if (head.biBitCount == 8) {
    rgb_color pal[256];
    RGBQUAD* ppal = GetPalette();

    for (int i = 0; i < 256; i++) {
      pal[i].r = ppal[i].rgbBlue;
      pal[i].g = ppal[i].rgbGreen;
      pal[i].b = ppal[i].rgbRed;
    }

    hFile->Write(&pal, 256 * sizeof(rgb_color), 1);
  }

  BYTE* pDest;

  if (pAlpha == 0 || head.biBitCount == 8) {
    for (int y = 0; y < tgaHead.ImageHeight; y++) {
      pDest = iter.GetRow(y);
      hFile->Write(pDest, tgaHead.ImageWidth * (head.biBitCount >> 3), 1);
    }
  }
  else {
    pDest = (BYTE*)pmalloc(4 * tgaHead.ImageWidth);
    RGBQUAD c;

    for (int y = 0; y < tgaHead.ImageHeight; y++) {
      for (int x = 0, x4 = 0; x < tgaHead.ImageWidth; x++, x4 += 4) {
        c = BlindGetPixelColor(x, y);
        pDest[x4 + 0] = c.rgbBlue;
        pDest[x4 + 1] = c.rgbGreen;
        pDest[x4 + 2] = c.rgbRed;
        pDest[x4 + 3] = 0;
      }

      hFile->Write(pDest, 4 * tgaHead.ImageWidth, 1);
    }

    pfree(pDest);
  }

  return TRUE;
}
#endif
int loadtgafile(const char* fname, int bpp, int revY, Image* dst)
{
  int len;
  char* buf;
  buf = (char*)loadfile(fname, &len);

  if (buf) {
    loadtgamem(buf, bpp, revY, dst);
    pfree(buf);
  }

  return 0;
}
int test_tgafile()
{
  char* fn = "demo.ico";
  //buf_t* t = loadbuf(fn);
  Image im[1] = {0};
  loadtgafile(fn, 32, 0, im);
  savebmpfile("asdf.bmp", im, 32, 32, 0);
  return 0;
}
