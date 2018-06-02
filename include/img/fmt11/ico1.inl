/*
 * File:  ximaico.cpp
 * Purpose: Platform Independent ICON Image Class Loader and Writer (MS version)
 * 07/Aug/2001 Davide Pizzolato - www.xdp.it
 * CxImage version 6.0.0 02/Feb/2008
 */


typedef struct tagIconDirectoryEntry {
  BYTE  bWidth;
  BYTE  bHeight;
  BYTE  bColorCount;
  BYTE  bReserved;
  WORD  wPlanes;
  WORD  wBitCount;
  DWORD dwBytesInRes;
  DWORD dwImageOffset;
} ICONDIRENTRY;

typedef struct tagIconDir {
  WORD  idReserved;
  WORD  idType;
  WORD  idCount;
} ICONHEADER;

void bihtoh(BITMAPINFOHEADER* bih)
{
  NTOH32(bih->biSize);
  NTOH32(bih->biWidth);
  NTOH32(bih->biHeight);
  NTOH16(bih->biPlanes);
  NTOH16(bih->biBitCount);
  NTOH32(bih->biCompression);
  NTOH32(bih->biSizeImage);
  NTOH32(bih->biXPelsPerMeter);
  NTOH32(bih->biYPelsPerMeter);
  NTOH32(bih->biClrUsed);
  NTOH32(bih->biClrImportant);
}

static int ico_load(stream_t* s, int cn_req, img_t* im)
{
  DWORD off;
  int page = 0; //internal icon structure indexes
  ICONHEADER icon_header;
  int nNumFrames = 0;
  int x, y, i, c;
  ICONDIRENTRY* icon_list;
  uchar m_palette[1024];
  int biHeight, biWidth, m_bpp;
  BITMAPINFOHEADER bih;
  BYTE* src_line = NULL;
  int maskwdt, src_pitch;
  BYTE* mask;

  off = stream_tell(s); //<yuandi>

  // read the first part of the header
  stream_read(s, &icon_header, sizeof(ICONHEADER));
  NTOH16(icon_header.idType);
  NTOH16(icon_header.idCount);

  // check if it's an icon or a cursor
  if (!((icon_header.idReserved == 0) && ((icon_header.idType == 1) || (icon_header.idType == 2)))) {
    return 0;
  }

  nNumFrames = icon_header.idCount;
  // load the icon descriptions
  icon_list = (ICONDIRENTRY*)pmalloc(icon_header.idCount * sizeof(ICONDIRENTRY));
  stream_read(s, icon_list, icon_header.idCount * sizeof(ICONDIRENTRY));

  for (c = 0; c < icon_header.idCount; c++) {
    NTOH16(icon_list[c].wPlanes);
    NTOH16(icon_list[c].wBitCount);
    NTOH32(icon_list[c].dwBytesInRes);
    NTOH32(icon_list[c].dwImageOffset);
  }

  if (!((page >= 0) && (page < icon_header.idCount))) {
    return 0;
  }

#if 0

  if (info.nEscape == -1) {
    // Return output dimensions only
    head.biWidth = icon_list[page].bWidth;
    head.biHeight = icon_list[page].bHeight;
#if CXIMAGE_SUPPORT_PNG

    if (head.biWidth == 0 && head.biHeight == 0) {
      // Vista icon support
      s->Seek(off + icon_list[page].dwImageOffset, SEEK_SET);
      CxImage png;
      png.SetEscape(-1);

      if (png.Decode(s, CXIMAGE_FORMAT_PNG)) {
        Transfer(png);
        info.nNumFrames = icon_header.idCount;
      }
    }

#endif //CXIMAGE_SUPPORT_PNG
    pfree(icon_list);
    info.dwType = CXIMAGE_FORMAT_ICO;
    return TRUE;
  }

#endif

  // get the bit count for the colors in the icon <CoreyRLucier>

  stream_seek(s, off + icon_list[page].dwImageOffset, SEEK_SET);

  if (icon_list[page].bWidth == 0 && icon_list[page].bHeight == 0) {
    // Vista icon support
#if 0
    png_load(s, cn_req, im);
#endif //
    return 0;
  }

  // standard icon
  stream_read(s, &bih, sizeof(BITMAPINFOHEADER));
  bihtoh(&bih);
  m_bpp = bih.biBitCount;

  // allocate memory for one icon
  imsetsize(im, biHeight = icon_list[page].bHeight, biWidth = icon_list[page].bWidth, cn_req, 1);  //image creation

  // read the palette
  if (bih.biClrUsed) {
    stream_read(s, m_palette, 1024);
  }
  else {
    for (i = 0; i < 256; ++i) {
      uchar* p = m_palette + i * 4;
      m_palette[0] = m_palette[1] = m_palette[2] = i;
      m_palette[3] = 255;
    }
  }

  //SetPalette(m_palette,head.biClrUsed); //palette assign
  //read the icon

  src_pitch = (((biWidth * (m_bpp != 15 ? m_bpp : 16) + 7) / 8 + 3) & -4);
  src_line = (BYTE*)pmalloc(src_pitch);

  for (y = 0; y < biHeight; y++) {
    uchar* data = im->tt.data + (biHeight - 1 - y) * im->s;
    stream_read(s, src_line, src_pitch);
    palette_fillrow(biWidth, data, cn_req, src_line, m_bpp, m_palette);
  }

  // apply the AND and XOR masks
  maskwdt = ((im->w + 31) / 32) * 4; //line width of AND mask (always 1 Bpp)
  mask = src_line;

  for (y = 0; y < biHeight; y++) {
    uchar* data = im->tt.data + (biHeight - 1 - y) * im->s;
    stream_read(s, mask, maskwdt);

    for (x = 0; x < biWidth; x++) {
      if (((mask[(x >> 3)] >> (7 - x % 8)) & 0x01)) {
        memset(data + x * cn_req, 255, cn_req);
      }
    }
  }

  pfree(src_line);
  pfree(icon_list);

  //imshowpal("im", im, 0);
  //cvWaitKey(-1);
  // icon has been loaded successfully!
  return FALSE;
}

#if 0

CC_INLINE int ico_save(stream_t* s, const img_t* im, const uchar* m_palette)
{
  //prepare the palette struct
  if (head.biBitCount <= 8 && m_palette == NULL) {
    return FALSE;
  }

  int maskwdt = ((head.biWidth + 31) / 32) * 4; //mask line width
  int masksize = head.biHeight * maskwdt; //size of mask
  int bitcount = head.biBitCount;
  int imagesize = head.biSizeImage;
#if CXIMAGE_SUPPORT_ALPHA

  if (AlphaIsValid() && head.biClrUsed == 0) {
    bitcount = 32;
    imagesize = 4 * head.biHeight * head.biWidth;
  }

#endif

  //fill the icon headers
  int nPages = nPageCount;

  if (nPages < 1) {
    nPages = 1;
  }

  ICONHEADER icon_header = {0, 1, nPages};

  if (!bAppend) {
    m_dwImageOffset = sizeof(ICONHEADER) + nPages * sizeof(ICONDIRENTRY);
  }

  DWORD dwBytesInRes = sizeof(BITMAPINFOHEADER) + head.biClrUsed * sizeof(RGBQUAD) + imagesize + masksize;

  ICONDIRENTRY icon_list = {
    (BYTE)head.biWidth,
    (BYTE)head.biHeight,
    (BYTE)head.biClrUsed,
    0, 0,
    (WORD)bitcount,
    dwBytesInRes,
    m_dwImageOffset
  };

  BITMAPINFOHEADER bi = {
    sizeof(BITMAPINFOHEADER),
    head.biWidth,
    2 * head.biHeight,
    1,
    (WORD)bitcount,
    0, imagesize,
    0, 0, 0, 0
  };

#if CXIMAGE_SUPPORT_PNG // Vista icon support
  CxImage png(*this);
  CxMemFile memfile;

  if (head.biWidth > 255 || head.biHeight > 255) {
    icon_list.bWidth = icon_list.bHeight = 0;
    memfile.Open();
    png.Encode(&memfile, CXIMAGE_FORMAT_PNG);
    icon_list.dwBytesInRes = dwBytesInRes = memfile.Size();
  }

#endif //CXIMAGE_SUPPORT_PNG

  if (!bAppend) {
    icon_header.idType = ntohs(icon_header.idType);
    icon_header.idCount = ntohs(icon_header.idCount);
    s->Write(&icon_header, sizeof(ICONHEADER), 1); //write the file header
    icon_header.idType = ntohs(icon_header.idType);
    icon_header.idCount = ntohs(icon_header.idCount);
  }


  if ((bAppend && nPageCount == info.nNumFrames) || (!bAppend && nPageCount == 0)) {
    icon_list.wPlanes = ntohs(icon_list.wPlanes);
    icon_list.wBitCount = ntohs(icon_list.wBitCount);
    icon_list.dwBytesInRes = ntohl(icon_list.dwBytesInRes);
    icon_list.dwImageOffset = ntohl(icon_list.dwImageOffset);
    s->Write(&icon_list, sizeof(ICONDIRENTRY), 1); //write the image entry
    icon_list.wPlanes = ntohs(icon_list.wPlanes);
    icon_list.wBitCount = ntohs(icon_list.wBitCount);
    icon_list.dwBytesInRes = ntohl(icon_list.dwBytesInRes);
    icon_list.dwImageOffset = ntohl(icon_list.dwImageOffset);

    m_dwImageOffset += dwBytesInRes;      //update offset for next header
  }

  if ((bAppend && nPageCount < info.nNumFrames) || (!bAppend && nPageCount == 0)) {
#if CXIMAGE_SUPPORT_PNG

    if (icon_list.bWidth == 0 && icon_list.bHeight == 0) { // Vista icon support
      s->Write(memfile.GetBuffer(FALSE), dwBytesInRes, 1);
    }
    else
#endif //CXIMAGE_SUPPORT_PNG
    {
      // standard icon
      bihtoh(&bi);
      s->Write(&bi, sizeof(BITMAPINFOHEADER), 1);   //write the image header
      bihtoh(&bi);

      BOOL bTransparent = info.nBkgndIndex >= 0;
      RGBQUAD ct = GetTransColor();

      if (m_palette) {
        if (bTransparent) {
          SetPaletteColor((BYTE)info.nBkgndIndex, 0, 0, 0, 0);
        }

        s->Write(m_palette, head.biClrUsed * sizeof(RGBQUAD), 1); //write palette

        if (bTransparent) {
          SetPaletteColor((BYTE)info.nBkgndIndex, ct);
        }
      }

#if CXIMAGE_SUPPORT_ALPHA

      if (AlphaIsValid() && head.biClrUsed == 0) {
        BYTE* buf = (BYTE*)pmalloc(imagesize);
        BYTE* dst = buf;

        for (long y = 0; y < head.biHeight; y++) {
          BYTE* src = GetBits(y);

          for (long x = 0; x < head.biWidth; x++) {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = AlphaGet(x, y);
          }
        }

        s->Write(buf, imagesize, 1);
        pfree(buf);
      }
      else {
        s->Write(info.pImage, imagesize, 1); //write image
      }

#else
      s->Write(info.pImage, imagesize, 1); //write image
#endif

      //save transparency mask
      BYTE* mask = (BYTE*)calloc(masksize, 1); //create empty AND/XOR masks

      if (!mask) {
        return FALSE;
      }

      //prepare the variables to build the mask
      BYTE* iDst;
      int pos, i;
      RGBQUAD c = {0, 0, 0, 0};
      long* pc = (long*)&c;
      long* pct = (long*)&ct;
#if CXIMAGE_SUPPORT_ALPHA
      BOOL bAlphaPaletteIsValid = AlphaPaletteIsValid();
      BOOL bAlphaIsValid = AlphaIsValid();
#endif

      //build the mask
      for (int y = 0; y < head.biHeight; y++) {
        for (int x = 0; x < head.biWidth; x++) {
          i = 0;
#if CXIMAGE_SUPPORT_ALPHA

          if (bAlphaIsValid && AlphaGet(x, y) == 0) {
            i = 1;
          }

          if (bAlphaPaletteIsValid && BlindGetPixelColor(x, y).rgbReserved == 0) {
            i = 1;
          }

#endif
          c = GetPixelColor(x, y, FALSE);

          if (bTransparent && *pc == *pct) {
            i = 1;
          }

          iDst = mask + y * maskwdt + (x >> 3);
          pos = 7 - x % 8;
          *iDst &= ~(0x01 << pos);
          *iDst |= ((i & 0x01) << pos);
        }
      }

      //write AND/XOR masks
      s->Write(mask, masksize, 1);
      pfree(mask);
    }
  }

  return TRUE;
}


#endif



#define ico_load_filename(fname, cn_req, im, pbpp, palette) ico_load_file_close(fopen(fname, "rb"), cn_req, im, pbpp, palette)
CC_INLINE int ico_load_file_close(FILE* pf, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = ico_load(s, cn_req, im);
    fclose(pf);
  }

  return ret;
}
