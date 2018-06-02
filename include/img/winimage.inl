
// WINIMAGE.H : Generic classes for raster images(MSWindows specialization)
// Content: Class declarations of:
// - class C_Image             : Storage class for single images
// - class ImageSet          : Storage class for sets of images
// - class C_AnimationWindow   : Window Class to display animations
//(Includes declarations of routines to Load and Save BMP files and to load
// GIF files into these classes).

#ifdef _WIN32
#pragma pack(1)
#endif
#define freadbytes(buf, size, pf)  fread(buf, 1, size, pf)
#define fwritebytes(buf, size, pf)  fwrite(buf, 1, size, pf)
// Windows specific types and constants:
#include <windows.h>
// Windows GDI expects 4bytes per color
typedef struct COLOR {
  unsigned char b, g, r, x;
} COLOR;

#define ALIGN sizeof(int)    // Windows GDI expects all int-aligned
void TransparentBlt2(HDC hdcDest,       // 目标DC
    int nXOriginDest,  // 目标X偏移
    int nYOriginDest,  // 目标Y偏移
    int nWidthDest,    // 目标宽度
    int nHeightDest,   // 目标高度
    HDC hdcSrc,        // 源DC
    int nXOriginSrc,   // 源X起点
    int nYOriginSrc,   // 源Y起点
    int nWidthSrc,     // 源宽度
    int nHeightSrc,    // 源高度
    UINT crTransparent // 透明色,COLORREF类型
                    )
{
  HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);   // 创建兼容位图
  HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);   // 创建单色掩码位图
  HDC hImageDC = CreateCompatibleDC(hdcDest);
  HDC hMaskDC = CreateCompatibleDC(hdcDest);
  hOldImageBMP = (HBITMAP) SelectObject(hImageDC, hImageBMP);
  hOldMaskBMP = (HBITMAP) SelectObject(hMaskDC, hMaskBMP);

  // 将源DC中的位图拷贝到临时DC中
  if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc) {
    BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
  }
  else {
    StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
  }

  // 设置透明色
  SetBkColor(hImageDC, crTransparent);
  // 生成透明区域为白色，其它区域为黑色的掩码位图
  BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
  // 生成透明区域为黑色，其它区域保持不变的位图
  SetBkColor(hImageDC, RGB(0, 0, 0));
  SetTextColor(hImageDC, RGB(255, 255, 255));
  BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
  // 透明部分保持屏幕不变，其它部分变成黑色
  SetBkColor(hdcDest, RGB(255, 255, 255));
  SetTextColor(hdcDest, RGB(0, 0, 0));
  BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
  // "或"运算,生成最终效果
  BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
  // 清理、恢复
  SelectObject(hImageDC, hOldImageBMP);
  DeleteDC(hImageDC);
  SelectObject(hMaskDC, hOldMaskBMP);
  DeleteDC(hMaskDC);
  DeleteObject(hImageBMP);
  DeleteObject(hMaskBMP);
}
// C_Image
//    Storage class for single images
typedef struct C_Image {
  // standard members:
  int Width, Height; // Dimensions in pixels
  int BPP;           // Bits Per Pixel
  char* Raster;      // Bits of Raster Data(Byte Aligned)
  COLOR* Palette;    // Color Map
  int BytesPerRow;   //img->Width(in bytes) including alignment!
  int Transparent;   // Index of Transparent color(-1 for none)
  // Extra members for animations:
  int xPos, yPos;    // Relative Position
  int Delay;     // Delay after image in 1/1000 seconds.
  int Transparency;   // Animation Transparency.
  // Windows GDI specific:
  BITMAPINFO* pbmi;    // BITMAPINFO structure
} C_Image;
// Image initializer(allocates space for raster and palette):
// Init: Allocates space for raster and palette in GDI-compatible structures.
int C_Image_Init(C_Image* img, int iWidth, int iHeight, int iBPP, char* Raster, COLOR* Palette)
{
  //memset(img, 0, sizeof(C_Image));
  img->Raster = 0;
  img->Palette = 0;
  img->pbmi = 0;

  if (img->Raster) {
    free_s(img->Raster);
    img->Raster = 0;
  }

  if (img->pbmi) {
    free_s(img->pbmi);
    img->pbmi = 0;
  }

  // Standard members setup
  img->Transparent = -1;
  img->BytesPerRow = img->Width = iWidth;
  img->Height = iHeight;
  img->BPP = iBPP;
  // Animation Extra members setup:
  img->xPos = img->yPos = img->Delay = 0;

  if (img->BPP == 24) {
    img->BytesPerRow *= 3;
    img->pbmi = NEW(BITMAPINFO, 1);
  }
  else {
    img->pbmi = (BITMAPINFO*) NEW(char, sizeof(BITMAPINFOHEADER) + (1 << img->BPP) * sizeof(COLOR));
    img->Palette = (COLOR*)((char*) img->pbmi + sizeof(BITMAPINFOHEADER));
  }

  img->BytesPerRow += (ALIGN - img->Width % ALIGN) % ALIGN;   // Align BytesPerRow
  img->Raster = NEW(char, img->BytesPerRow * img->Height);
  img->pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  img->pbmi->bmiHeader.biWidth = img->Width;
  img->pbmi->bmiHeader.biHeight = -img->Height;   // negative means up-to-bottom
  img->pbmi->bmiHeader.biPlanes = 1;
  img->pbmi->bmiHeader.biBitCount = (img->BPP < 8 ? 8 : img->BPP);   // Our raster is byte-aligned
  img->pbmi->bmiHeader.biCompression = BI_RGB;
  img->pbmi->bmiHeader.biSizeImage = 0;
  img->pbmi->bmiHeader.biXPelsPerMeter = 11811;
  img->pbmi->bmiHeader.biYPelsPerMeter = 11811;
  img->pbmi->bmiHeader.biClrUsed = 0;
  img->pbmi->bmiHeader.biClrImportant = 0;

  if (Raster) {
    memcpy(img->Raster, Raster, img->BytesPerRow * img->Height);
  }

  if (Palette) {
    memcpy(img->Palette, Palette, (1 << img->BPP) * sizeof(*Palette));
  }

  return 0;
}
int C_Image_Free(C_Image* img)
{
  free_s(img->pbmi);
  free_s(img->Raster);
  return 0;
}
// Error processing macro(NO-OP by default):
#define ERRORMSG(PARAM) {}
#define Pixel( x, y ) img->Raster[ y * img->BytesPerRow + x ]

// Windows GDI Specific function to paint the image on a DC:
// GDIPaint: Paint the raster image onto a DC
int GDIPaint(C_Image* img, HDC hdc, int x, int y)
{
  RECT rcItem = { x, y, x + img->Width, y + img->Height };
  //RECT rcPaint=rcItem;
  RECT rcBmpPart = { 0, 0, img->Width, img->Height};
  HBITMAP hBitmap;
  HDC m_dcBk = CreateCompatibleDC(hdc);
  hBitmap = CreateCompatibleBitmap(hdc, img->Width * 2, img->Height * 2);
  SelectObject(m_dcBk, hBitmap);
  SetDIBitsToDevice(m_dcBk, 0, 0, img->Width, img->Height, 0, 0, 0, img->Height, (LPVOID) img->Raster, img->pbmi, DIB_RGB_COLORS);
  //::BitBlt(hdc, x, y,img->Width,img->Height, m_dcBk, 0, 0, SRCCOPY);
  TransparentBlt2(hdc, x, y, img->Width, img->Height, m_dcBk, 0, 0, img->Width, img->Height, RGB(0, 0, 0));
  //SetDIBitsToDevice( hdc, x, y,img->Width,img->Height, 0, 0, 0,img->Height,( LPVOID ) Raster, pbmi, DIB_RGB_COLORS );
  //CRenderEngine::DrawImage( hdc, hBitmap, rcItem, rcPaint, rcBmpPart, CRect(0,0,0,0), 1, 0, 0, 0, 0 );
  DeleteObject(hBitmap);
  DeleteDC(m_dcBk);
  return 1;
}
//   Load a BMP File into the C_Image object
int LoadBMP(C_Image* img, LPCTSTR szFileName)
{
  int n;
  // Open file.
  FILE* bmpfile;
  struct BITMAPFILEHEADER {
    unsigned short bfType;
    unsigned long bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long bfOffBits;
  }
  bmfh;
  struct BITMAPINFOHEADER {
    unsigned long biSize;
    long biWidth;
    long biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long biCompression;
    unsigned long biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    unsigned long biClrUsed;
    unsigned long biClrImportant;
  }
  bmih;
  int row, PixelsPerByte, BitMask, RowAlignmentInFile;

  bmpfile = fopen(szFileName , "rb");

  if (bmpfile == NULL) {
    ERRORMSG("File not found");
    return 0;
  }

  // *1* LOAD BITMAP FILE HEADER
  freadbytes((char*) & bmfh, sizeof(bmfh), bmpfile);

  // Check filetype signature
  if (bmfh.bfType != 'MB') {
    ERRORMSG("Not a valid BMP File");
    return 0;
  }

  // *2* LOAD BITMAP INFO HEADER
  freadbytes((char*) & bmih, sizeof(bmih), bmpfile);

  // Check for supported Color depths
  if ((bmih.biBitCount != 1) &&
      (bmih.biBitCount != 4) &&
      (bmih.biBitCount != 8) &&
      (bmih.biBitCount != 24)) {
    ERRORMSG("Color depth not supported");
    return 0;
  }

  // Check if file is compressed
  if (bmih.biCompression != 0) {
    ERRORMSG("File uses unsupported compression");
    return 0;
  }

  // Set: Allocate memory to contain Data
  C_Image_Init(img, bmih.biWidth, (bmih.biHeight > 0) ? bmih.biHeight : -bmih.biHeight, bmih.biBitCount, 0, 0);

  // *3* IFimg->BPP AREN'T 24, LOAD PALETTE.
  if (img->BPP != 24) {
    for (n = 0; n < 1 << img->BPP; n++) {
      char buf[4];
      // 4th byte of RGBQUAD discarded
      freadbytes(buf, 4, bmpfile);
      img->Palette[ n ].b = buf[0];
      img->Palette[ n ].g = buf[1];
      img->Palette[ n ].r = buf[2];
    }
  }

  // *4* LOAD RASTER DATA
  // Seek Raster Data in file
  fseek(bmpfile, bmfh.bfOffBits, SEEK_SET);
  PixelsPerByte = 8 / img->BPP; //used only ifimg->BPP are less than 8
  BitMask = (1 << img->BPP) - 1;  //used only ifimg->BPP are less than 8
  // Raster Data Rows are 32bit aligned in BMP files.
  RowAlignmentInFile = ((4 - ((img->Width * img->BPP + 7) / 8) % 4) % 4);       //(bytes)

  for (row = 0; row < img->Height; row++) {
    int m;
    char* pPixel;
    char buf[1];
    // If height is positive the bmp is bottom-up, set adequate row info:
    pPixel = img->Raster + img->BytesPerRow *
        ((bmih.biHeight > 0) ? img->Height - row - 1 : row);

    if (img->BPP >= 8) {                                             // 8 or moreimg->BPP: Read as block.
      freadbytes(pPixel, img->Width * img->BPP / 8, bmpfile);
    }
    else {
      // Less than 8BPP: Read and store byte aligned.
      int col, charGot, bit;

      for (col = 0; col < img->Width; col += PixelsPerByte) {
        freadbytes(buf, 1, bmpfile);
        charGot = buf[0];

        for (bit = 8 ; bit > 0 ; bit -= img->BPP) {                                            // high to low
          *pPixel++ = (charGot >> (bit - img->BPP)) & BitMask;
        }
      }
    }

    // Ignore aligment bytes of file:
    for (m = 0; m < RowAlignmentInFile; m++) {
      freadbytes(buf, 1, bmpfile);
    }
  }

  fclose(bmpfile);
  return 1;
}
//   Save the content of a C_Image object into a BMP file
int SaveBMP(C_Image* img, LPCTSTR szFileName)
{
  int n;
  int SaveBPP;
  char buf[4];
  struct BITMAPFILEHEADER {
    unsigned short bfType;
    unsigned long bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long bfOffBits;
  }
  bmfh;
  struct BITMAPINFOHEADER {
    unsigned long biSize;
    long biWidth;
    long biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long biCompression;
    unsigned long biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    unsigned long biClrUsed;
    unsigned long biClrImportant;
  }
  bmih;

  // Create file.
  FILE* bmpfile = fopen(szFileName , "wb");

  if (bmpfile == NULL) {
    ERRORMSG("Error creating file");
    return 0;
  }

  // determineimg->BPP for file:
  if (img->BPP == 1) {
    SaveBPP = 1;
  }
  else if (img->BPP <= 4) {
    SaveBPP = 4;
  }
  else if (img->BPP <= 8) {
    SaveBPP = 8;
  }
  else {
    SaveBPP = 24;
  }

  // *1* SAVE BITMAP FILE HEADER
  bmfh.bfType = 'MB';
  bmfh.bfSize = 0; // TO DO
  bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
  bmfh.bfOffBits = 54 + ((SaveBPP == 24) ? 0 : (1 << SaveBPP) * 4);
  fwritebytes((char*) & bmfh, sizeof(bmfh), bmpfile);
  // *2* SAVE BITMAP INFO HEADER
  bmih.biSize = sizeof(bmih);
  bmih.biWidth = img->Width;
  bmih.biHeight = img->Height; // down-top
  bmih.biPlanes = 1;
  bmih.biBitCount = SaveBPP;
  bmih.biCompression = 0; // BI_RGB?
  bmih.biSizeImage = (img->Width * img->BPP) / 8;
  bmih.biSizeImage += (4 - (bmih.biSizeImage) % 4) % 4;
  bmih.biXPelsPerMeter = 11811;
  bmih.biYPelsPerMeter = 11811;
  bmih.biClrUsed = 0;
  bmih.biClrImportant = 0;
  fwritebytes((char*) & bmih, sizeof(bmih), bmpfile);

  // *3* IFimg->BPP AREN'T 24, SAVE PALETTE.
  if (img->BPP != 24) {
    buf[3] = ((char) 0);

    for (n = 0; n < 1 << img->BPP; n++) {
      buf[0] = (img->Palette[ n ].b);
      buf[1] = (img->Palette[ n ].g);
      buf[2] = (img->Palette[ n ].r);
      fwritebytes(buf, 4, bmpfile);
    }

    // in case SaveBPP is higher thanimg->BPP
    for (; n < 1 << SaveBPP; n++) {
      fwritebytes((char*) '\0\0\0\0', 4, bmpfile);
    }
  }

  // *4* SAVE RASTER DATA

  {
    int PixelsPerByte = 8 / SaveBPP; //used only ifimg->BPP are less than 8
    int BitMask = (1 << SaveBPP) - 1;   //used only ifimg->BPP are less than 8
    // Raster Data Rows are 32bit aligned in BMP files.
    int row, RowAlignmentInFile = ((4 - ((img->Width * SaveBPP + 7) / 8) % 4) % 4);        //(bytes)

    for (row = 0; row < img->Height; row++) {
      char* pPixel;
      int m;
      // If height is positive the bmp is bottom-up, set adequate row info:
      pPixel = (char*) img->Raster + img->BytesPerRow *
          ((bmih.biHeight > 0) ? img->Height - row - 1 : row);

      if (SaveBPP >= 8) {                                             // 8 or moreimg->BPP: Save as block.
        fwritebytes(pPixel, img->Width * SaveBPP / 8, bmpfile);
      }
      else {  // Less than 8BPP: Save packing bytes.
        int col, bit;
        unsigned char charToPut;

        for (col = 0; col < img->Width; col += PixelsPerByte) {
          charToPut = 0;

          for (bit = 8 ; bit > 0 ; bit -= img->BPP) {                                            // high to low
            charToPut |= (*pPixel++ & BitMask) << (bit - img->BPP);
          }

          buf[0] = charToPut;
          fwritebytes(buf, 1, bmpfile);
        }
      }

      // Ignore aligment bytes of file:
      buf[0] = 0;

      for (m = 0; m < RowAlignmentInFile; m++) {
        fwritebytes(buf, 1, bmpfile);
      }
    }
  }
  fclose(bmpfile);
  return 1;
}

// LZWDecoder (C/C++)
// Codec to perform LZW (GIF Variant) decompression.
//                         (c) Nov2000, Juan Soulie <jsoulie@cplusplus.com> *
// Parameter description:
//  - bufIn: Input buffer containing a "de-blocked" GIF/LZW compressed image.
//  - bufOut: Output buffer where result will be stored.
//  - InitCodeSize: Initial CodeSize to be Used
//    (GIF files include this as the first byte in a picture block)
//  - AlignedWidth :img->Width of a row in memory (including alignment if needed)
//  -img->Width,img->Height: Physical dimensions of image.
//  - Interlace: 1 for Interlaced GIFs.
int LZWDecoder(char* bufIn, char* bufOut,
    short InitCodeSize, int AlignedWidth,
    int Width, int Height, const int Interlace)
{
  int n;
  int row = 0, col = 0;    // used to point output if Interlaced
  int nPixels, maxPixels;   // Output pixel counter
  short CodeSize;     // Current CodeSize (size in bits of codes)
  short ClearCode;    // Clear code : resets decompressor
  short EndCode;     // End code : marks end of information
  long whichBit;     // Index of next bit in bufIn
  long LongCode;     // Temp. var. from which Code is retrieved
  short Code;      // Code extracted
  short PrevCode;     // Previous Code
  short OutCode;     // Code to output
  // Translation Table:
  short Prefix[ 4096 ];    // Prefix: index of another Code
  unsigned char Suffix[ 4096 ];  // Suffix: terminating character
  short FirstEntry;    // Index of first pfree entry in table
  short NextEntry;    // Index of next pfree entry in table
  unsigned char OutStack[ 4097 ]; // Output buffer
  int OutIndex;     // Characters in OutStack
  int RowOffset;     // Offset in output buffer for current row
  // Set up values that depend on InitCodeSize Parameter.
  CodeSize = InitCodeSize + 1;
  ClearCode = (1 << InitCodeSize);
  EndCode = ClearCode + 1;
  NextEntry = FirstEntry = ClearCode + 2;
  whichBit = 0;
  nPixels = 0;
  maxPixels = Width * Height;
  RowOffset = 0;

  while (nPixels < maxPixels) {
    OutIndex = 0;       // Reset Output Stack
    // GET NEXT CODE FROM bufIn:
    // LZW compression uses code items longer than a single byte.
    // For GIF Files, code sizes are variable between 9 and 12 bits
    // That's why we must read data (Code) this way:
    LongCode = *((long*)(bufIn + whichBit / 8));        // Get some bytes from bufIn
    LongCode >>= (whichBit & 7);      // Discard too low bits
    Code = (LongCode & ((1 << CodeSize) - 1));       // Discard too high bits
    whichBit += CodeSize;     // Increase Bit Offset

    // SWITCH, DIFFERENT POSIBILITIES FOR CODE:
    if (Code == EndCode) {                                                 // END CODE
      break;  // Exit LZW Decompression loop
    }

    if (Code == ClearCode) {      // CLEAR CODE:
      CodeSize = InitCodeSize + 1;   // Reset CodeSize
      NextEntry = FirstEntry;    // Reset Translation Table
      PrevCode = Code;    // Prevent next to be added to table.
      continue;       // restart, to get another code
    }

    if (Code < NextEntry) {                                                 // CODE IS IN TABLE
      OutCode = Code;  // Set code to output.
    }
    else {         // CODE IS NOT IN TABLE:
      OutIndex++;   // Keep "first" character of previous output.
      OutCode = PrevCode;     // Set PrevCode to be output
    }

    // EXPAND OutCode IN OutStack
    // - Elements up to FirstEntry are Raw-Codes and are not expanded
    // - Table Prefices contain indexes to other codes
    // - Table Suffices contain the raw codes to be output
    while (OutCode >= FirstEntry) {
      if (OutIndex > 4096) {
        return 0;
      }

      OutStack[ OutIndex++ ] = Suffix[ OutCode ]; // Add suffix to Output Stack
      OutCode = Prefix[ OutCode ];    // Loop with preffix
    }

    // NOW OutCode IS A RAW CODE, ADD IT TO OUTPUT STACK.
    if (OutIndex > 4096) {
      return 0;
    }

    OutStack[ OutIndex++ ] = (unsigned char) OutCode;

    // ADD NEW ENTRY TO TABLE (PrevCode + OutCode)
    // (EXCEPT IF PREVIOUS CODE WAS A CLEARCODE)
    if (PrevCode != ClearCode) {
      Prefix[ NextEntry ] = PrevCode;
      Suffix[ NextEntry ] = (unsigned char) OutCode;
      NextEntry++;

      // Prevent Translation table overflow:
      if (NextEntry >= 4096) {
        return 0;
      }

      // INCREASE CodeSize IF NextEntry IS INVALID WITH CURRENT CodeSize
      if (NextEntry >= (1 << CodeSize)) {
        if (CodeSize < 12) {
          CodeSize++;
        }
        else {}    // Do nothing. Maybe next is Clear Code.
      }
    }

    PrevCode = Code;

    // Avoid the possibility of overflow on 'bufOut'.
    if (nPixels + OutIndex > maxPixels) {
      OutIndex = maxPixels - nPixels;
    }

    // OUTPUT OutStack (LAST-IN FIRST-OUT ORDER)
    for (n = OutIndex - 1; n >= 0; n--) {
      if (col == Width) {                                                 // Check if new row.
        if (Interlace) {      // If interlaced::
          if ((row & 7) == 0) {
            row += 8;

            if (row >= Height) {
              row = 4;
            }
          }
          else if ((row & 3) == 0) {
            row += 8;

            if (row >= Height) {
              row = 2;
            }
          }
          else if ((row & 1) == 0) {
            row += 4;

            if (row >= Height) {
              row = 1;
            }
          }
          else {
            row += 2;
          }
        }
        else {       // If not interlaced:
          row++;
        }

        RowOffset = row * AlignedWidth;  // Set new row offset
        col = 0;
      }

      bufOut[ RowOffset + col ] = OutStack[ n ]; // Write output
      col++;
      nPixels++;     // Increase counters.
    }
  } // while (main decompressor loop)

  return whichBit;
}
// * ImageSet
// *    Storage class for sets of images
typedef struct ImageSet {
  int FrameWidth, FrameHeight; // Dimensions of ImageSet in pixels.
  C_Image** img;   // Images' Vector.
  int nImages;     // Number of images(vector size)
} ImageSet;
// Append new image to vector(push_back)
// AddImage: Adds an image object to the back of the img vector.
void AddImage(ImageSet* ims, C_Image* newimage)
{
  int n;
  C_Image** pTempImg =  NEW(C_Image*, ims->nImages + 1);

  for (n = 0; n < ims->nImages; n++) {
    pTempImg[ n ] = ims->img[ n ];  // (pointer copy)
  }

  free_s(ims->img);
  ims->img = pTempImg;
  ims->img[ n ] = newimage;
  ims->nImages++;
}
// constructor and destructor:
int ImageSet_Init(ImageSet* ims)
{
  ims->img = 0;
  ims->nImages = 0;
  return 0;
}
int ImageSet_Free(ImageSet* ims)
{
  int n;

  for (n = 0; n < ims->nImages; n++) {
    C_Image_Free(ims->img[ n ]);
    free_s(ims->img[ n ]);
  }

  free_s(ims->img);
  return 0;
}
int fgetch(FILE* pf)
{
  unsigned char buf[4];
  fread(buf, 1, 1, pf);
  return buf[0];
}
//   Load a GIF File into the ImageSet object
int LoadGIF(ImageSet* ims, LPCTSTR szFileName)
{
  int n;
  char buf[4];
  // Global GIF variables:
  int GlobalBPP;      // Bits per Pixel.
  COLOR* GlobalColorMap;     // Global colormap (allocate)
  struct GIFGCEtag {    // GRAPHIC CONTROL EXTENSION
    unsigned char BlockSize;  // Block Size: 4 bytes
    unsigned char PackedFields;  // Packed Fields. Bits detail:
    //    0: Transparent Color Flag
    //    1: User Input Flag
    //  2-4: Disposal Method
    unsigned short Delay;   // Delay Time (1/100 seconds)
    unsigned char Transparent;  // Transparent Color Index
  }
  gifgce;
  int GraphicExtensionFound = 0;
  // OPEN FILE
  FILE* giffile = fopen(szFileName, "rb");

  if (NULL == giffile) {
    ERRORMSG("File not found");
    return 0;
  }

  // *1* READ HEADER (SIGNATURE + VERSION)
  {
    char szSignature[ 6 ];    // First 6 bytes (GIF87a or GIF89a)
    freadbytes(szSignature, 6, giffile);

    if (memcmp(szSignature, "GIF", 2) != 0) {
      ERRORMSG("Not a GIF File");
      return 0;
    }
  }
  // *2* READ LOGICAL SCREEN DESCRIPTOR
  {
    struct GIFLSDtag {
      unsigned short ScreenWidth;  // Logical Screenimg->Width
      unsigned short ScreenHeight; // Logical Screenimg->Height
      unsigned char PackedFields;  // Packed Fields. Bits detail:
      //  0-2: Size of Global Color Table
      //    3: Sort Flag
      //  4-6: Color Resolution
      //    7: Global Color Table Flag
      unsigned char Background;  // Background Color Index
      unsigned char PixelAspectRatio; // Pixel Aspect Ratio
    }
    giflsd;
    freadbytes((char*) & giflsd, sizeof(giflsd), giffile);
    GlobalBPP = (giflsd.PackedFields & 0x07) + 1;
    // fill some animation data:
    ims->FrameWidth = giflsd.ScreenWidth;
    ims->FrameHeight = giflsd.ScreenHeight;
    // *3* READ/GENERATE GLOBAL COLOR MAP
    GlobalColorMap = NEW(COLOR , 1 << GlobalBPP);

    if (giflsd.PackedFields & 0x80) {
      // File has global color map?
      for (n = 0; n < (1 << GlobalBPP); n++) {
        freadbytes(buf, 3, giffile);
        GlobalColorMap[ n ].r = buf[0];
        GlobalColorMap[ n ].g = buf[1];
        GlobalColorMap[ n ].b = buf[2];
      }
    }
    else {  // GIF standard says to provide an internal default Palette:
      for (n = 0; n < 256; n++) {
        GlobalColorMap[ n ].r = GlobalColorMap[ n ].g = GlobalColorMap[ n ].b = n;
      }
    }
  }

  // *4* NOW WE HAVE 3 POSSIBILITIES:
  //  4a) Get and Extension Block (Blocks with additional information)
  //  4b) Get an Image Separator (Introductor to an image)
  //  4c) Get the trailer Char (End of GIF File)
  do {
    int nBlockLength, charGot = fgetch(giffile);

    // *A* EXTENSION BLOCK
    if (charGot == 0x21) {
      freadbytes(buf, 1, giffile);

      switch (buf[0]) {
      case 0xF9:
        // Graphic Control Extension
        freadbytes((char*) & gifgce, sizeof(gifgce), giffile);
        GraphicExtensionFound++;
        fgetch(giffile); // Block Terminator (always 0)
        break;

      case 0xFE:              // Comment Extension: Ignored
      case 0x01:              // PlainText Extension: Ignored
      case 0xFF:              // Application Extension: Ignored
      default:                // Unknown Extension: Ignored

        // read (and ignore) data sub-blocks
        while (nBlockLength = fgetch(giffile))
          for (n = 0; n < nBlockLength; n++) {
            fgetch(giffile);
          }

        break;
      }
    }
    else if (charGot == 0x2c) {   // *B* IMAGE (0x2c Image Separator)
      // Read Image Descriptor
      struct GIFIDtag {
        unsigned short xPos;   // Image Left Position
        unsigned short yPos;   // Image Top Position
        unsigned short Width;   // Imageimg->Width
        unsigned short Height;   // Imageimg->Height
        unsigned char PackedFields;  // Packed Fields. Bits detail:
        //  0-2: Size of Local Color Table
        //  3-4: (Reserved)
        //    5: Sort Flag
        //    6: Interlace Flag
        //    7: Local Color Table Flag
      }
      gifid;
      // Create a new Image Object:
      char* pCompressedImage;
      char* pTemp;
      int nBlockLength;
      int LocalColorMap;
      long ImgStart, ImgEnd;
      short firstbyte;
      C_Image* NextImage;
      NextImage = NEW(C_Image, 1);
      memset(NextImage, 0, sizeof(C_Image));
      freadbytes((char*) & gifid, sizeof(gifid), giffile);
      LocalColorMap = (gifid.PackedFields & 0x08) ? 1 : 0;
      C_Image_Init(NextImage, gifid.Width, gifid.Height,
          LocalColorMap ? (gifid.PackedFields & 7) + 1 : GlobalBPP, 0, 0);
      // Fill NextImage Data
      NextImage->xPos = gifid.xPos;
      NextImage->yPos = gifid.yPos;

      if (GraphicExtensionFound) {
        NextImage->Transparent =
            (gifgce.PackedFields & 0x01) ? gifgce.Transparent : -1;
        NextImage->Transparency =
            (gifgce.PackedFields & 0x1c) > 1 ? 1 : 0;
        NextImage->Delay = gifgce.Delay * 10;
      }

      if (LocalColorMap)
        // Read Color Map (if descriptor says so)
      {
        freadbytes((char*) NextImage->Palette, sizeof(COLOR) * (1 << NextImage->BPP), giffile);
      }
      else {   // Otherwise copy Global
        memcpy(NextImage->Palette, GlobalColorMap, sizeof(COLOR) * (1 << NextImage->BPP));
      }

      firstbyte = fgetch(giffile); // 1st byte of img block (CodeSize)
      // Calculate compressed image block size
      // to fix: this allocates an extra byte per block
      ImgEnd = ImgStart = ftell(giffile);

      while (n = fgetch(giffile)) {
        ImgEnd += n + 1;
        fseek(giffile, ImgEnd, SEEK_SET);
      }

      fseek(giffile, ImgStart, SEEK_SET);
      // Allocate Space for Compressed Image
      pCompressedImage = NEW(char , ImgEnd - ImgStart + 4);
      // Read and store Compressed Image
      pTemp = pCompressedImage;

      while (nBlockLength = fgetch(giffile)) {
        freadbytes(pTemp, nBlockLength, giffile);
        pTemp += nBlockLength;
      }

      // Call LZW/GIF decompressor
      n = LZWDecoder(
          (char*) pCompressedImage,
          (char*) NextImage->Raster,
          firstbyte, NextImage->BytesPerRow,                                            //NextImage->AlignedWidth,
          gifid.Width, gifid.Height,
          ((gifid.PackedFields & 0x40) ? 1 : 0)                                                 //Interlaced?
          );

      if (n) {
        AddImage(ims, NextImage);
      }
      else {
        pfree(NextImage);
        ERRORMSG("GIF File Corrupt");
      }

      // Some cleanup
      pfree(pCompressedImage);
      GraphicExtensionFound = 0;
    }
    else if (charGot == 0x3b) {     // *C* TRAILER: End of GIF Info
      break; // Ok. Standard End.
    }
  }
  while (!feof(giffile));

  free_s(GlobalColorMap);
  fclose(giffile);

  if (ims->nImages == 0) {
    ERRORMSG("Premature End Of File");
  }

  return ims->nImages;
}

