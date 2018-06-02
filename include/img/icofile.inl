#ifndef bLittleEndianHost
static short __g_test = 1;
#define bLittleEndianHost  (*((char *) &__g_test) == 1)
#endif // bLittleEndianHost
short _ntohs(const short word)
{
  if (bLittleEndianHost) {
    return word;
  }

  return ((word & 0xff) << 8) | ((word >> 8) & 0xff);
}
long _ntohl(const long dword)
{
  if (bLittleEndianHost) {
    return dword;
  }

  return ((dword & 0xff) << 24) | ((dword & 0xff00) << 8) | ((dword >> 8) & 0xff00) | ((dword >> 24) & 0xff);
}
void bihtoh(BITMAPINFOHEADER* bih)
{
  bih->biSize = _ntohl(bih->biSize);
  bih->biWidth = _ntohl(bih->biWidth);
  bih->biHeight = _ntohl(bih->biHeight);
  bih->biPlanes = _ntohs(bih->biPlanes);
  bih->biBitCount = _ntohs(bih->biBitCount);
  bih->biCompression = _ntohl(bih->biCompression);
  bih->biSizeImage = _ntohl(bih->biSizeImage);
  bih->biXPelsPerMeter = _ntohl(bih->biXPelsPerMeter);
  bih->biYPelsPerMeter = _ntohl(bih->biYPelsPerMeter);
  bih->biClrUsed = _ntohl(bih->biClrUsed);
  bih->biClrImportant = _ntohl(bih->biClrImportant);
}
int loadicomem(const char* aInputStream, int bpp, int revY, Image* im)
{
  DWORD off = 0;
  const char* buf = aInputStream;
  int page = 0;
  BITMAPINFOHEADER bih;
  // read the first part of the header
  ICONHEADER icon_header = *(ICONHEADER*)(buf + off);
  off += sizeof(ICONHEADER);
  icon_header.idType = _ntohs(icon_header.idType);
  icon_header.idCount = _ntohs(icon_header.idCount);

  // check if it's an icon or a cursor
  if ((icon_header.idReserved == 0) && ((icon_header.idType == 1) || (icon_header.idType == 2))) {
    // load the icon descriptions
    ICONDIRENTRY* icon_list = (ICONDIRENTRY*)(buf + off);
    int c;
    RGBQUAD* pal = 0;

    //info.nNumFrames = icon_header.idCount;
    for (c = 0; c < icon_header.idCount; c++) {
      icon_list[c].wPlanes = _ntohs(icon_list[c].wPlanes);
      icon_list[c].wBitCount = _ntohs(icon_list[c].wBitCount);
      icon_list[c].dwBytesInRes = _ntohl(icon_list[c].dwBytesInRes);
      icon_list[c].dwImageOffset = _ntohl(icon_list[c].dwImageOffset);
    }

    // Vista icon support
    if (icon_list[page].bWidth == 0 && icon_list[page].bHeight == 0) {
      ASSERT(0 && "not support Vista icon");
    }

    // get the bit count for the colors in the icon <CoreyRLucier>
    //hFile->Seek(off + icon_list[page].dwImageOffset, SEEK_SET);
    // standard icon
    bih = *((BITMAPINFOHEADER*)(buf + icon_list[page].dwImageOffset));
    off = icon_list[page].dwImageOffset + sizeof(BITMAPINFOHEADER);
    bihtoh(&bih);
    c = bih.biBitCount;
    // allocate memory for one icon
    newImage(icon_list[page].bWidth, icon_list[page].bHeight, bpp >> 3, im); //image creation
    bih.biClrUsed = 256;
    // read the palette
    pal = (RGBQUAD*)(buf + off);
    off += bih.biClrUsed * sizeof(RGBQUAD);

    if (bih.biClrUsed) {}
    else {
      //hFile->Read(pal,biClrUsed*sizeof(RGBQUAD), 1);
      //assert(0);
    }

    {
      //read the icon
      int x, y, step = (((im->w * c) + 31) / (1 << 5)) << 2;
      int maskwdt = ((im->w + 31) / 32) * 4;
      const BYTE* src = (BYTE*)(buf + off);
      const BYTE* mask = (BYTE*)(buf + off + im->h * im->w);

      for (y = 0; y < icon_list[page].bHeight; y++) {
        Color32* dst = getLinePixels(im, y);

        switch (bpp) {
        case 32:
          rgbColorToColor32_line(src, c, (Color32*)(pal), dst, im->w);
          break;

        case 8:
          rgbColorToGray_line(src, c, (Color32*)(pal), (uchar*)dst, im->w);
          break;

        default:
          assert(0);
          break;
        }

        src += step;
      }

      for (y = 0; y < im->h; y++) {
        Color32* dst = getLinePixels(im, y);

        for (x = 0; x < im->w; x++) {
          if (((mask[y * maskwdt + (x >> 3)] >> (7 - x % 8)) & 0x01)) {
            dst[x].a = 255;
          }
        }
      }
    }

    // icon has been loaded successfully!
  }

  return 0;
}
int saveicomem(const Image* src, int bpp, char* aOutputStream, int clSumBit, int revY)
{
  //prepare the palette struct
  RGBQUAD* pal = 0;
  int maskwdt = ((src->w + 31) / 32) * 4; //mask line width
  int masksize = src->h * maskwdt; //size of mask
  int bitcount = bpp;
  int imagesize = 4 * src->h * src->w;
  int biClrUsed = 0;
  char* buf = (char*)aOutputStream;
  //fill the icon headers
  int nPages = 1;
  int bAppend = 0;
  //if (!bAppend)
  int m_dwImageOffset = (bAppend) ? 0 : sizeof(ICONHEADER) + nPages * sizeof(ICONDIRENTRY);
  ICONHEADER* icon_header = (ICONHEADER*)buf;
  ICONDIRENTRY* icon_list = (ICONDIRENTRY*)(icon_header + 1);
  BITMAPINFOHEADER* bi = (BITMAPINFOHEADER*)(buf + m_dwImageOffset);
  DWORD dwBytesInRes = sizeof(BITMAPINFOHEADER) + biClrUsed * sizeof(RGBQUAD) + imagesize + masksize;
  icon_header->idReserved = 0;
  icon_header->idType = 1;
  icon_header->idCount = nPages;
  icon_list->bWidth = (BYTE)src->w;
  icon_list->bHeight = (BYTE)src->h;
  icon_list->bColorCount = (BYTE)biClrUsed;
  icon_list->bReserved = 0;
  icon_list->wPlanes = 0;
  icon_list->wBitCount = (WORD)bitcount;
  icon_list->dwBytesInRes = dwBytesInRes;
  icon_list->dwImageOffset = m_dwImageOffset;
  memset(bi, 0, sizeof(BITMAPINFOHEADER));
  bi->biSize = sizeof(BITMAPINFOHEADER);
  bi->biWidth = src->w;
  bi->biHeight = 2 * src->h;
  bi->biPlanes = 1;
  bi->biBitCount = (WORD)bitcount;
  bi->biSizeImage = imagesize;
  bihtoh(bi);
  return 0;
}

int loadicofile(const char* fname, int bpp, int revY, Image* dst)
{
  int len;
  char* buf;
  buf = (char*)loadfile(fname, &len);

  if (buf) {
    loadicomem(buf, bpp, revY, dst);
    pfree(buf);
  }

  return 0;
}

int test_icofile()
{
  char* fn = "demo.ico";
  //buf_t* t = loadbuf(fn);
  Image im[1] = {0};
  loadicofile(fn, 32, 0, im);
  savebmpfile("asdf.bmp", im, 32, 32, 0);
  return 0;
}
