
#ifndef _BMPFILE_INL_
#define _BMPFILE_INL_

#include <assert.h>
#include <Windows.h>
#include <stdio.h>

//#include "math/math.def"

#ifndef CC_INLINE
#define CC_INLINE static
#endif

//压缩方法
enum {
  csBI_RGB        = 0, // 未压缩
  csBI_RLE8       = 1, // 使用行程编码压缩的8-bpp图像，只适用于由下向上的DIB
  csBI_RLE4       = 2, // 使用行程编码压缩的4-bpp图像，只适用于由下向上的DIB
  csBI_BITFIELDS  = 3, // 16bpp、32bpp的未压缩图像，有三个屏蔽用于确定如何从像素中提取RGB成分
  csBI_JPEG       = 4, // 像素阵列是JPEG格式的嵌入图像，只用于Window98/2000
  csBI_PNG        = 5, // 像素阵列是PNG格式的嵌入图像，只用于Window98/2000
};

#define getLinePixels(IMG, Y)  (Color32*)(((char*)(IMG)->tt.data)+(IMG)->s*(Y))
#define Pixels(IMG, T, X, Y)  ((T*)(((char*)(IMG)->tt.data)+(IMG)->s*(Y)))[X]
#define newImage(w, h, elem_bytes, im)  imsetsize(im, h, w, elem_bytes, 1)

//                        位图文件结构
//+---------------------------------------------------------+
//|                       位图文件头                        |
//|                 (BITMAPFILEHEADER：14字节)              |
//+----+----------------------------------------------------+
//|    |                   位图信息头                       |
//| 位 |     (目前有4个版本：OS/2, 版本3，版本4，版本5)     |
//|    +----------------------------------------------------+
//| 图 |                   位屏蔽                           |
//|    +----------------------------------------------------+
//| 信 |                   颜色表                           |
//|    |       (只有当每个像素的位数小于等于8时，才存在)    |
//| 息 | (如果采用颜色表，像素阵列中的像素是颜色表达的索引) |
//|    | (有三种格式：OS/2(RGBTRIPLE)，其它（RGBQUAD））... |
//+----+----------------------------------------------------+
//|             像素阵列(Pixel[][])                         |
//|     (通常情况下，图像是对其到32位边界的扫描线序列)      |
//+---------------------------------------------------------+

// 位图文件头
typedef struct tBITMAPFILEHEADER_NO_TYPE {
  //uint16    bfType;
  uint32   bfSize;
  uint16   bfReserved1;
  uint16   bfReserved2;
  uint32   bfOffBits;
} tBITMAPFILEHEADER_NO_TYPE;

//位图信息头(目前有4个版本：OS/2, 版本3，版本4，版本5)
typedef struct tBITMAPINFOHEADER {
  uint32       biSize;              //信息头长度：识别各种版本的唯一方法
  uint32       biWidth;             //像素宽度
  uint32       biHeight;            //像素高度，正:由下向上，负：由上至下
  uint16       biPlanes;            //位平面，必须为1
  uint16       biBitCount;          //每个像素的位数，如256色，8bit
  uint32       biCompression;       //压缩方法
  uint32       biSizeImage;         //图像的像素阵列大小
  uint32       biXPelsPerMeter;
  uint32       biYPelsPerMeter;
  uint32       biClrUsed;
  uint32       biClrImportant;
} tBITMAPINFOHEADER;

//32bitARGB
typedef struct Color32 {
  uint8 b, g, r, a;
} Color32;

typedef struct Color24 {
  uint8 b, g, r;
} Color24;

#define TCOLOR16BITDEF(rbit, gbit, bbit, TColor16bit) typedef struct TColor16bit{ \
  uint16  b:bbit;  uint16  g:gbit;  uint16  r:rbit;}TColor16bit;

TCOLOR16BITDEF(4, 4, 4, Color16_444);
TCOLOR16BITDEF(5, 5, 5, Color16_555);
TCOLOR16BITDEF(5, 6, 5, Color16_565);
TCOLOR16BITDEF(6, 6, 6, Color16_666);
#undef TCOLOR16BITDEF

#define PALLETT_LEN(bpp)     ((bpp)<=8?(1<<bpp):0)
#define ALIGN_DIV(X, N)      ((X+N-1)/N)
#define DIBSET(D, H, W, S, B)   ((D)->h=H, (D)->w=W, (D)->s=S, (D)->b=B)

#define SETCOLOR32(COL, R, G, B, A)  (COL.r=R,COL.g=G,COL.b=B,COL.a=A)
#define SETCOLOR24(COL, R, G, B)     (COL.r=R,COL.g=G,COL.b=B)

#ifndef GREYSCALIZE
#define _CR (299*(1 << 16)/1000)
#define _CG (587*(1 << 16)/1000)
#define _CB ((1<<16) - _CR - _CG)
#define GREYSCALIZE(R, G, B)   ((_CR*(R) + _CG*(G) + _CB*(B) )>>16)
#endif // GREYSCALIZE
#define GREYSCALIZE2(COLR)   GREYSCALIZE((COLR).r, (COLR).g, (COLR).b)

typedef struct tagIconDir {
  WORD          idReserved;
  WORD          idType;
  WORD          idCount;
} ICONHEADER;

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

#pragma pack(1)
typedef struct tagTgaHeader {
  BYTE   IdLength;            // Image ID Field Length
  BYTE   CmapType;            // Color Map Type
  BYTE   ImageType;           // Image Type

  WORD   CmapIndex;           // First Entry Index
  WORD   CmapLength;          // Color Map Length
  BYTE   CmapEntrySize;       // Color Map Entry Size

  WORD   X_Origin;            // X-origin of Image
  WORD   Y_Origin;            // Y-origin of Image
  WORD   ImageWidth;          // Image Width
  WORD   ImageHeight;         // Image Height
  BYTE   PixelDepth;          // Pixel Depth
  BYTE   ImagDesc;            // Image Descriptor
} TGAHEADER;
#pragma pack()

#define RGBTOSHORT(R, G, B, _r, _g, _b)    (((R) >> (8-_r)) | (((G) & ~((1<_g)-1)) << (_r+_g-8)) | (((B) & ~((1<<_b)-1)) << (_r+_g+_b-8)))
#define SHORTTORGB(PIX, R, G, B, _r, _g, _b)    (R = (uchar)(PIX<<(8-_r)), G = (uchar)((PIX>>(_r+_g-8)) & ~((1<_g)-1)), B = (uchar)((PIX>>(_r+_g+_b-8)) & ~((1<_b)-1))  )

CC_INLINE int rgb2rgb(int h, int w, const uchar* A0, int al, int abpp, uchar* B0, int bl, int bbpp)
{
  if (abpp == bbpp) {
    //memcpy2d;
  }

#define GET_IDX_MASK(i, x)   (((x)&__mask2)<<(8-__bb-i*__bb))

  //#define GET_IDX_MASK(i, x)   (((x)&__mask2)<<(i*__bb))
  switch (abpp) {
#if 1

  case 8: {
#define GET_RGB()  (r=g=b=A[0])
#define GET_IDX(i)  GET_IDX_MASK(i, A[i])
    enum {ai = 1};
    typedef uchar a_type;
#include "impl/rgb2rgb_case_bpp.inl"
    break;
  }

#else

  case 32: {
#define GET_RGB()  (r=A[0], g=A[1], b=A[2])
#define GET_IDX(i)  GET_IDX_MASK(i, (A[ai*i+0] + A[ai*i+1] + A[ai*i+1] + A[ai*i+2])>>2)
    enum {ai = 4};
    typedef uchar a_type;
#include "impl/rgb2rgb_case_bpp.inl"
  }
  break;

  case 24: {
#define GET_RGB()  (r=A[0], g=A[1], b=A[2])
#define GET_IDX(i)  GET_IDX_MASK(i, (A[ai*i+0] + A[ai*i+1] + A[ai*i+1] + A[ai*i+2])>>2)
    enum {ai = 3};
    typedef uchar a_type;
#include "impl/rgb2rgb_case_bpp.inl"
  }
  break;

  case 16: {
#define GET_RGB()  SHORTTORGB(A[0], r, g, b, 5, 6, 5)
#define GET_IDX(i)  GET_IDX_MASK(i, A[i])
    enum {ai = 1};
    typedef ushort a_type;
#include "impl/rgb2rgb_case_bpp.inl"
  }
  break;

  case 15: {
#define GET_RGB()  SHORTTORGB(A[0], r, g, b, 5, 6, 5)
#define GET_IDX(i)  GET_IDX_MASK(i, A[i])
    enum {ai = 1};
    typedef ushort a_type;
#include "impl/rgb2rgb_case_bpp.inl"
  }
  break;

  case 12: {
#define GET_RGB()  SHORTTORGB(A[0], r, g, b, 5, 6, 5)
#define GET_IDX(i)  GET_IDX_MASK(i, A[i])
    enum {ai = 1};
    typedef ushort a_type;
#include "impl/rgb2rgb_case_bpp.inl"
  }
  break;

  case 8: {
#define GET_RGB()  (r=g=b=A[0])
#define GET_IDX(i)  GET_IDX_MASK(i, A[i])
    enum {ai = 1};
    typedef uchar a_type;
#include "impl/rgb2rgb_case_bpp.inl"
  }
  break;
#endif

  default:
    ASSERT(0);
    break;
  }

  return 0;
}

CC_INLINE int idx2rgb(int h, int w, const uchar* A0, int al, int abpp, uchar* B0, int bl, int bbpp, const Color32* pal)
{
  int k, x = 0, y = 0;
  enum {ai = 1};
  uchar* rgbpal = (uchar*)pal;
  typedef uchar a_type;
  uchar grypal[256] = {0};
  int npal = 1 << abpp;

  if (bbpp <= 8) {
    for (k = 0; k < npal; ++k) {
      grypal[k] = GREYSCALIZE(pal[k].r, pal[k].g, pal[k].b);
    }
  }

  switch (bbpp) {
#if 1

  case 8: {
#define GET_RGB(i, j)           (grypal[i])
#define PUT_RGB(i, r, g, b)  (B[i]=r)
    enum {bi = 1};
    typedef uchar b_type;
#include "impl/idx2rgb_case.inl"
  }
  break;
#else

  case 32: {
#define GET_RGB(i, j)     (pal[(i)+j])
#define PUT_RGB(i, r, g, b)  (B[i]=r)
    enum {bi = 1};
    typedef Color32 b_type;
#include "impl/idx2rgb_case.inl"
  }
  break;

  case 24: {
#define GET_RGB(i, j)     (rgbpal[((i)<<2)+j])
#define PUT_RGB(i, r, g, b)  (B[i*bi+0]=r, B[i*bi+1]=g, B[i*bi+2]=b)
    enum {bi = 3};
    typedef uchar b_type;
#include "impl/idx2rgb_case.inl"
  }
  break;

  case 16: {
#define GET_RGB(i, j)       (rgbpal[((i)<<2)+j])
#define PUT_RGB(i, r, g, b)  B[i]=(ushort)RGBTOSHORT(r, g, b, 5, 6, 5)
    enum {bi = 1};
    typedef ushort b_type;
#include "impl/idx2rgb_case.inl"
  }
  break;

  case 8: {
#define GET_RGB(i, j)           (grypal[i])
#define PUT_RGB(i, r, g, b)  (B[i]=r)
    enum {bi = 1};
    typedef uchar b_type;
#include "impl/idx2rgb_case.inl"
  }
  break;
#endif

  default:
    ASSERT(0);
    break;
  }


  return 0;
}

#define abssub(A, B)  ((A>B)?(A-B):(B-A))
#define sub2(A, B)  ((A-B)*(A-B))
#define sub2rgb(A, B)  (sub2(A.r, B.r)+sub2(A.g, B.g)+sub2(A.b, B.b)+sub2(A.a, B.a))

#ifndef MIN
#define MIN(a, b)  ((a)<(b)?(a):(b))
#endif // MIN

//与已排序的数逐一比较，大于t时，该数移后
#define InsertSort(T, _A, _n, OP) {int _i=1,_j;T* _a=(T*)(_A);for (;_i<(_n);++_i) {T _t = _a[_i];for (_j=_i; _j > 0 && _t OP _a[_j-1] ; --_j) {_a[_j]=_a[_j-1];}_a[_j]=_t;}}
#define SortFind(_A, _N, _V, _I, OP) {int _b[2],_i=-1;_b[0]=_N, _b[1]=0;for (;_b[1]<_b[0]-1;) {_i=(_b[0]+_b[1])>>1;_b[_A[_i] OP (_V)]=_i;}_I=(_b[0]+_b[1])>>1;}
#define lenof(_A)  (sizeof(_A)/sizeof((_A)[0]))

CC_INLINE int test_InsertSort()
{
  int i, aa[] = {4, 9, 23, 1, 45, 27, 5, 2}, n = lenof(aa), j;
  InsertSort(int, aa, n, <);
  SortFind(aa, n, aa[0], i, <);
  assert(aa[i] <= aa[0] && aa[i + 1] >= aa[0]);

  for (j = 1; j < n; ++j) {
    SortFind(aa, n, aa[j], i, <);
    assert(aa[i] < aa[j] && aa[i + 1] >= aa[j]);
  }

  return 0;
}

CC_INLINE int GrayMakePal(int n, uchar* A, uchar* pal, int pl)
{
  int i, j, k;
  pal[0] = pal[1] = A[0];

  for (i = 1; i < n; ++i) {
    if (A[i] < pal[0]) {
      pal[0] = A[i];
    }

    if (A[i] > pal[1]) {
      pal[1] = A[i];
    }
  }

  for (j = 2; j < pl; ++j) {
    int maxs = 0;

    for (i = 0; i < n; ++i) {
      int s = 10000;

      for (k = 0; k < j; ++k) {
        int s2 = sub2(A[i], pal[k]);
        s = MIN(s2, s);
      }

      if (s > maxs) {
        maxs = s, pal[j] = A[i];
      }
    }
  }

  return 0;
}

CC_INLINE int GrayFind(int n, const Color32* colorTable, int col)
{
  int mins = 100000;
  int i = 0, j = -1;

  for (; i < n; ++i) {
    int s = sub2(col, colorTable[i].r);

    if (s < mins) {
      mins = s, j = i;
    }
  }

  return j;
}
CC_INLINE int RgbFind(int n, Color32* colorTable, Color32 col)
{
  int mins = 100000;
  int i = 0, j = -1;

  for (; i < n; ++i) {
    int s = sub2rgb(col, colorTable[i]);

    if (s < mins) {
      mins = s, j = i;
    }
  }

  return j;
}
CC_INLINE int RgbMakePal(int n, Color32* A, int cn, Color32* pal, int pl)
{
  int i, j, k;
  DWORD* dA = (DWORD*)A;
  DWORD* dpal = (DWORD*)pal;
  pal[0] = pal[1] = A[0];

  if (cn < 1) {
    cn = 1;
  }

  for (i = 0; i < n; i += cn) {
    if (dA[i] < dpal[0]) {
      pal[0] = A[i];
    }

    if (dA[i] > dpal[1]) {
      pal[1] = A[i];
    }
  }

  for (j = 2; j < pl; ++j) {
    int maxs = 0;

    for (i = 0; i < n; i += cn) {
      int s = 10000;

      for (k = 0; k < j; ++k) {
        int s2 = sub2rgb(A[i], pal[k]);
        s = MIN(s2, s);
      }

      if (s > maxs) {
        maxs = s, pal[j] = A[i];
      }
    }
  }

  return 0;
}

//
const uint16 csBmpTag = 'B' | ('M' << 8);
const long bmpTagSize = sizeof(csBmpTag); //BM

CC_INLINE BOOL checkHeadType(const uchar* aInputStream)
{
  const char* tag = (const char*)aInputStream;
  return ((tag[0] == 'b') || (tag[0] == 'B')) && ((tag[1] == 'm') || (tag[1] == 'M'));
}

CC_INLINE int loadbmpmem(const uchar* aInputStream, int bpp, int revY, img_t* dst)
{
  long ReadPos = 0;
  long biSize;
  BOOL IsRevY;
  uint8* pLinePixels;
  long i, y;
  long incBmpByteWidth;
  Color32* Pallette;
  uint32 npal;
  tBITMAPFILEHEADER_NO_TYPE bmFileHeader;//读出BITMAPFILEHEADER结构
  tBITMAPINFOHEADER bmpInfo;//

  if (!checkHeadType(aInputStream)) {
    return 0;  //("bmp file \"BM\" tag error!");
  }

  ReadPos += bmpTagSize;

  //位图文件头结构和位图信息结构
  bmFileHeader = *(tBITMAPFILEHEADER_NO_TYPE*)(aInputStream + ReadPos);
  ReadPos += sizeof(bmFileHeader);

  bmpInfo = *(tBITMAPINFOHEADER*)(aInputStream + ReadPos);
  ReadPos += bmpInfo.biSize;
  biSize = bmpInfo.biSize;

  if (sizeof(bmpInfo) < biSize) {
    biSize = sizeof(bmpInfo);
  }

  //aInputStream->read((void*)((uchar *)&bmpInfo.biSize+sizeof(uint32)),(long)(biSize-sizeof(uint32)));

  IsRevY = bmpInfo.biHeight > 0;
  bmpInfo.biHeight = abs(bmpInfo.biHeight);

  npal = 0;

  if (bmpInfo.biBitCount <= 8) {
    npal = bmpInfo.biClrUsed;

    if (npal == 0) {
      npal = 1 << bmpInfo.biBitCount;
    }
  }

  incBmpByteWidth = (((bmpInfo.biWidth * bmpInfo.biBitCount) + 31) / (1 << 5)) << 2;

  if (bmpInfo.biSizeImage == 0) {
    bmpInfo.biSizeImage = abs(incBmpByteWidth * bmpInfo.biHeight);
  }

  //调色板
  if (npal > 0) {
    Pallette = (Color32*)(aInputStream + ReadPos);
    ReadPos += npal * sizeof(Color32);

    for (i = 0; i < (long)npal; ++i) {
      //Pallette[i].a = 0x00;
    }
  }
  else if (bmpInfo.biBitCount == 18) {
    bmpInfo.biBitCount = 18;
  }
  else if (bmpInfo.biBitCount == 32) {
    if (bmpInfo.biCompression == csBI_BITFIELDS) {
      uint32 RMask, GMask, BMask;
      bmpInfo.biBitCount = 18;
      RMask = ((uint32*)(aInputStream + ReadPos))[0];
      GMask = ((uint32*)(aInputStream + ReadPos))[1];
      BMask = ((uint32*)(aInputStream + ReadPos))[2];
      ReadPos += (3 * sizeof(uint32));

      if (GMask == (((1 << 6) - 1) << 6)) {
        bmpInfo.biBitCount = 18;
      }
    }
  }
  else if (bmpInfo.biBitCount == 16) {
    //区分 RGB 15bit,RGB 16bit
    if (bmpInfo.biCompression == csBI_BITFIELDS) {
      uint32 RMask, GMask, BMask;
      RMask = ((uint32*)(aInputStream + ReadPos))[0];
      GMask = ((uint32*)(aInputStream + ReadPos))[1];
      BMask = ((uint32*)(aInputStream + ReadPos))[2];
      ReadPos += (3 * sizeof(uint32));

      if (GMask == 0x03E0) {
        assert(RMask == 0x7C00);
        assert(BMask == 0x001F);
        bmpInfo.biBitCount = 15;
      }
      else if (GMask == (((1 << 4) - 1) << 4)) {
        bmpInfo.biBitCount = 12;
      }
      else if (GMask == (((1 << 6) - 1) << 6)) {
        bmpInfo.biBitCount = 18;
      }
      else {
        assert(RMask == 0xF800);
        assert(GMask == 0x07E0);
        assert(BMask == 0x001F);
        bmpInfo.biBitCount = 16;
      }
    }
    else {
      bmpInfo.biBitCount = 15;
    }
  }

  assert(bmpInfo.biPlanes == 1);
  assert(bmpInfo.biCompression != csBI_RLE4); //不支持压缩格式
  assert(bmpInfo.biCompression != csBI_RLE8); //不支持压缩格式

  newImage(bmpInfo.biWidth, bmpInfo.biHeight, (bpp + 7) / 8, dst);

  incBmpByteWidth = abs(incBmpByteWidth);

  //aInputStream->skip(bmFileHeader.bfOffBits-ReadPos);
  ReadPos = bmFileHeader.bfOffBits;
  pLinePixels = (uint8*)(aInputStream + ReadPos); //->read(bmpInfo.biSizeImage);
  ReadPos += bmpInfo.biSizeImage;

  for (y = 0; y < (int)bmpInfo.biHeight; ++y) {
    //long py=y;
    //if (IsRevY) py=bmpInfo.biHeight-1-y;
    Color32* s32 = revY ? getLinePixels(dst, bmpInfo.biHeight - 1 - y) : getLinePixels(dst, y);

    switch (bpp) {
    case 32:
      //rgbColorToColor32_line(pLinePixels, bmpInfo.biBitCount, (Color32*)(&Pallette[0]), s32, dst->w);
      break;

    case 8:
      //rgbColorToGray_line(pLinePixels, bmpInfo.biBitCount, (Color32*)(&Pallette[0]), (uchar*)s32, dst->w);
      break;

    default:
      assert(0);
      break;
    }

    pLinePixels += incBmpByteWidth;
  }

  return 0;
}

CC_INLINE long get_bmp_byte_width(long colorLineByteWidth)
{
  return (colorLineByteWidth + 3) / 4 * 4; //四字节对齐
}

CC_INLINE int imsavebmpmem(buf_t* bf, const img_t* src, int clSumBit, int revY, const Color32* pal)
{
  tBITMAPFILEHEADER_NO_TYPE bmFileHeader;//位图文件头结构和位图信息结构
  tBITMAPINFOHEADER bmpInfo;
  long clBit, colorLineByteWidth, BmpByteWidth, rbit, gbit, bbit;
  uint32 dwDibBitsSize, dwOffBits, dwFileSize;
  uint32 npal;
  uint16 bfType;
  int biCompression;
  uchar* aOutputStream = 0;

  if (clSumBit <= 8) {
    rbit = 8, gbit = 0, bbit = 0;
  }
  else {
    if (12 == clSumBit) {
      rbit = 4, gbit = 4, bbit = 4;
    }

    if (15 == clSumBit) {
      rbit = 5, gbit = 5, bbit = 5;
    }

    if (16 == clSumBit) {
      rbit = 5, gbit = 6, bbit = 5;
    }

    if (18 == clSumBit) {
      rbit = 6, gbit = 6, bbit = 6;
    }

    if (24 <= clSumBit) {
      rbit = 8, gbit = 8, bbit = 8;
    }
  }

  clBit = (clSumBit > 8) ? ((clSumBit + 7) / 8) * 8 : clSumBit;
  npal = clBit <= 8 ? (1 << clBit) : 0;

  biCompression = (16 == clBit || 18 == clSumBit) ? csBI_BITFIELDS : csBI_RGB;
  colorLineByteWidth = (src->w * clBit + 7) / 8;
  BmpByteWidth = get_bmp_byte_width(colorLineByteWidth);

  dwDibBitsSize = src->h * abs(BmpByteWidth);
  dwOffBits = sizeof(bfType) + sizeof(tBITMAPFILEHEADER_NO_TYPE) + sizeof(tBITMAPINFOHEADER) + (16 == clBit || 18 == clSumBit) * 3 * sizeof(uint32) + npal * sizeof(Color32); //+palette_byte_size();
  dwFileSize = dwOffBits + dwDibBitsSize;

  bfsetsize(bf, dwFileSize);
  aOutputStream = bf->data;
  bfType = csBmpTag;
  //写入类型信息
  //aOutputStream->write(&bfType, sizeof(bfType));
  *((uint16*)aOutputStream) = bfType;
  aOutputStream += sizeof(uint16);

  //填充BITMAPFILEHEADER结构
  memset(&bmFileHeader, 0, sizeof(bmFileHeader));
  bmFileHeader.bfSize = dwFileSize;
  bmFileHeader.bfReserved1 = 0;
  bmFileHeader.bfReserved2 = 0;
  bmFileHeader.bfOffBits = dwOffBits;
  //写入文件头和位图信息
  *((tBITMAPFILEHEADER_NO_TYPE*)aOutputStream) = bmFileHeader;
  aOutputStream += sizeof(tBITMAPFILEHEADER_NO_TYPE);
  //
  memset(&bmpInfo, 0, sizeof(bmpInfo));
  bmpInfo.biSize = sizeof(tBITMAPINFOHEADER);
  bmpInfo.biWidth = src->w;
  bmpInfo.biHeight = src->h;
  bmpInfo.biPlanes = 1;
  bmpInfo.biBitCount = (uint16)clBit;
  bmpInfo.biCompression = biCompression;
  bmpInfo.biSizeImage = 0;
  bmpInfo.biXPelsPerMeter = 0;
  bmpInfo.biYPelsPerMeter = 0;
  bmpInfo.biClrUsed = 0;
  bmpInfo.biClrImportant = 0;
  //aOutputStream->write(&bmpInfo, sizeof(tBITMAPINFOHEADER));
  *((tBITMAPINFOHEADER*)aOutputStream) = bmpInfo;
  aOutputStream += sizeof(tBITMAPINFOHEADER);

  if (csBI_BITFIELDS == bmpInfo.biCompression) {
    ((uint32*)aOutputStream)[0] = ((1 << rbit) - 1) << (gbit + bbit);
    ((uint32*)aOutputStream)[1] = ((1 << gbit) - 1) << bbit;
    ((uint32*)aOutputStream)[2] = ((1 << bbit) - 1);
    aOutputStream += 3 * sizeof(aOutputStream);
  }

  //write_palette(aOutputStream);
  MEMCPY((Color32*)(aOutputStream), pal, npal);
  aOutputStream += npal * sizeof(Color32);

#if 0

  if (npal > 0) {
    if (32 == bpp) {
      Color32* pal = (Color32*)Pallette;
      RgbMakePal(bmpInfo.biHeight * bmpInfo.biWidth, (Color32*)(src + 1), bmpInfo.biHeight * bmpInfo.biWidth / 1000, pal, npal);
    }
    else if (8 == bpp) {
      if (npal < 256) {
        uchar* pal = (uchar*)Pallette;
        GrayMakePal(bmpInfo.biHeight * bmpInfo.biWidth, (uchar*)(src + 1), pal, npal);

        for (i = npal - 1; i >= 0; --i) {
          Pallette[i].b = Pallette[i].g = Pallette[i].r = pal[i];
          Pallette[i].a = 0xFF;
        }

      }
      else {
        for (i = 0; i < (long)npal; ++i) {
          Pallette[i].b = Pallette[i].g = Pallette[i].r = (uchar)(i * 256 / npal);
          Pallette[i].a = 0xFF;
        }
      }
    }

    aOutputStream += npal * sizeof(Color32);
  }

#endif

  //写入数据
  //rgb2rgb(1, bmpInfo.biWidth, src->tt.data, src->s, bpp, aOutputStream, BmpByteWidth, clSumBit);
  if (revY) {
    memflip(aOutputStream, BmpByteWidth, src->tt.data, src->s, src->h, MIN(BmpByteWidth, src->s));
  }
  else {
    memcpy2d(aOutputStream, BmpByteWidth, src->tt.data, src->s, src->h, MIN(BmpByteWidth, src->s));
  }

  aOutputStream += BmpByteWidth * src->h;
  return 0;
}

CC_INLINE int rgb2gry(const img_t* src, img_t* dst)
{
  //img_t aa[1] = newImage(src->w, src->h, sizeof(uchar), dst);
  Color32* s = (Color32*)(src->tt.data);
  uchar* d = (uchar*)(dst->tt.data);
  int i, n = src->w * src->h;

  for (i = 0; i < n; ++i) {
    d[i] = GREYSCALIZE(s[i].r, s[i].g, s[i].b);
  }

  return 0;
}

CC_INLINE int gry2rgb(const img_t* src, img_t* dst)
{
  //img_t* aa = newImage(src->w, src->h, sizeof(Color32), dst);
  uchar* s = (uchar*)(src + 1);
  Color32* d = (Color32*)(dst + 1);
  int i, n = src->w * src->h;

  for (i = 0; i < n; ++i) {
    d[i].r = d[i].g = d[i].b = s[i];
  }

  return 0;
}

//#define FIND(N, A, AI, OP, I) {int _ia=AI; I=-1; if (N) for (I=0;_ia<(AI)*(N); _ia+=(AI)) {if (*((A)+_ia) OP *((A)+I)){I=_ia;} }}

CC_INLINE void* memset4(void* p, int v, size_t n)
{
  int* s = (int*)p, *e = s + n;

  for (; s != e;) {
    *s++ = v;
  }

  return p;
}

//#include "imopt.inl"

#define dib_loadfile(fname, dib)  {buf_t bf[1]={0}; buf_load(fname, bf); if (bf->len) {dib_loadmem(bf->data, bf->len, dib);} bffree(bf); }
#define imsavebmpfile(fname, dib, bpp, pal)  {buf_t bf[1]={0}; imsavebmpmem(bf, dib, bpp, 1, pal); buf_save(fname, bf); bffree(bf); }



#endif // _BMPFILE_INL_
