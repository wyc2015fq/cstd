#ifndef _IMGIO_INL_
#define _IMGIO_INL_
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
//#include "stb/stb_image.inl"
//#undef e

#define pmalloc malloc
#define pfree free


#include "std/fileio_c.h"
#include "std/imgopt.h"
#include "std/color_c.h"
#include "std/stream_c.h"
#include "std/imgopt.h"
//#include "sys\findfile.h"


#define imio_err(a, b)  0

//typedef unsigned short int uint16;
//typedef unsigned long int uint32;


// 短整型大小端互换
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | (((uint16)(A) & 0x00ff) << 8))
// 长整型大小端互换
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | (((uint32)(A) & 0x00ff0000) >> 8) |(((uint32)(A) & 0x0000ff00) << 8) |(((uint32)(A) & 0x000000ff) << 24))


#ifdef _WIN32
#define NTOH16(x)   (x)
#define NTOH32(x)   (x)
#else
#define NTOH16(x)   ((x) = BigLittleSwap16(x))
#define NTOH32(x)   ((x) = BigLittleSwap32(x))
#endif


// 本机大端返回1，小端返回0
static int checkCPUendian()
{
  union{
    unsigned long int i;
    unsigned char s[4];
  } c;
  c.i = 0x12345678;
  return (0x12 == c.s[0]);
}

// 模拟htonl函数，本机字节序转网络字节序
static unsigned long int t_htonl(unsigned long int h)
{
  // 若本机为大端，与网络字节序同，直接返回
  // 若本机为小端，转换成大端再返回
  return checkCPUendian() ? h : BigLittleSwap32(h);
}

// 模拟ntohl函数，网络字节序转本机字节序
static unsigned long int t_ntohl(unsigned long int n)
{
  // 若本机为大端，与网络字节序同，直接返回
  // 若本机为小端，网络数据转换成小端再返回
  return checkCPUendian() ? n : BigLittleSwap32(n);
}

// 模拟htons函数，本机字节序转网络字节序
static unsigned short int t_htons(unsigned short int h)
{
  // 若本机为大端，与网络字节序同，直接返回
  // 若本机为小端，转换成大端再返回
  return checkCPUendian() ? h : BigLittleSwap16(h);
}

// 模拟ntohs函数，网络字节序转本机字节序
static unsigned short int t_ntohs(unsigned short int n)
{
  // 若本机为大端，与网络字节序同，直接返回
  // 若本机为小端，网络数据转换成小端再返回
  return checkCPUendian() ? n : BigLittleSwap16(n);
}

CC_INLINE int bmp_load(stream_t* s, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  uchar buf[1024];
  int i = 0;
  int m_bpp = 0, m_rle_code = 0, m_offset = 0, hsz, biPlanes;
  int m_width = 0, m_height = 0, m_cn, clr_used = 0;
  int y, src_pitch;
  uchar* m_palette = buf;

  if (!s) {
    return 0;
  }

  stream_seek(s, 0, SEEK_SET);
  stream_read(s, buf, 50);

  if (buf[0] != 'B' || buf[1] != 'M') {
    return 0;
  }

  i = 10;
  MEM_GET32L(buf, i, m_offset);
  MEM_GET32L(buf, i, hsz);

  if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) {
    return imio_err("unknown BMP", "BMP type not supported: unknown");
  }

  if (hsz >= 36) {
    MEM_GET32L(buf, i, m_width);
    MEM_GET32L(buf, i, m_height);
    MEM_GET16L(buf, i, biPlanes);

    if (biPlanes != 1) {
      return imio_err("bad BMP", "bad BMP");
    }

    MEM_GET16L(buf, i, m_bpp);
    MEM_GET32L(buf, i, m_rle_code);
    i = 46;
    MEM_GET32L(buf, i, clr_used);
    i = 7 * sizeof(int) + 22 + hsz - 36;

    if (m_rle_code == 1 || m_rle_code == 2) {
      return imio_err("BMP RLE", "BMP type not supported: RLE");
    }

    clr_used = (clr_used == 0 ? 1 << m_bpp : clr_used);
    m_cn = m_bpp > 24 ? 4 : 3;

    if (m_bpp <= 8) {
      stream_seek(s, i, SEEK_SET);
      stream_read(s, m_palette, clr_used * 4);

      if (palette) {
        memcpy(palette, m_palette, clr_used * 4);
      }

      if (!palette_is_color(1 << m_bpp, m_palette)) {
        m_cn = 1;
      }
    }
    else if (m_bpp == 16 && m_rle_code == 3) {
      int redmask, greenmask, bluemask;
      MEM_GET32L(buf, i, redmask);
      MEM_GET32L(buf, i, greenmask);
      MEM_GET32L(buf, i, bluemask);

      if (bluemask == 0x1f && greenmask == 0x3e0 && redmask == 0x7c00) {
        m_bpp = 15;
      }
      else if (bluemask == 0x1f && greenmask == 0x7e0 && redmask == 0xf800) {}
      else {
        return 0;
      }
    }
    else if (m_bpp == 16 && m_rle_code == 0) {
      m_bpp = 15;
    }
  }
  else {
    MEM_GET16L(buf, i, m_width);
    MEM_GET16L(buf, i, m_height);
    MEM_GET16L(buf, i, biPlanes);
    MEM_GET16L(buf, i, m_bpp);
    ASSERT(0);
  }

  if (cn_req < 1) {
    cn_req = m_cn;
  }

  if (pbpp) {
    *pbpp = m_bpp;
  }

  if (im) {
    uchar* src_line = NULL;
    uchar* data = NULL;
    int step = 0;
    imsetsize(im, m_height, m_width, cn_req, 1);
    data = im->tt.data + (im->h - 1) * im->s;
    step = -im->s;
    stream_seek(s, m_offset, SEEK_SET);
    src_pitch = (((m_width * (m_bpp != 15 ? m_bpp : 16) + 7) / 8 + 3) & -4);
    src_line = (uchar*)pmalloc(src_pitch);

    if (m_bpp <= 8) {
      if (1 == cn_req) {
        IMTRANS1D(clr_used, m_palette, 4, m_palette, 1, BGR2GRAY);
      }

      if (clr_used <= 0) {
        for (i = 0; i < 256; ++i) {
          m_palette[i] = i;
        }
      }
    }

    for (y = 0; y < m_height; y++, data += step) {
      stream_read(s, src_line, src_pitch);
      palette_fillrow(m_width, data, cn_req, src_line, m_bpp, m_palette);
    }

    pfree(src_line);
  }

  return 1;
}

CC_INLINE int bmp_save(stream_t* s, int height, int width, const uchar* data, int step, int channels, const uchar* palette)
{
  uchar buf[1024];
  int i = 0;
  int fileStep = (width * channels + 3) & -4; //4字节对齐
  int bitmapHeaderSize = 40;
  int paletteSize = channels > 1 ? 0 : 1024;
  const unsigned char zeropad[4] = {0, 0, 0, 0};
  int headerSize = 14 /* fileheader */ + bitmapHeaderSize + paletteSize;
  int val;
  short st;
  memcpy(buf, "BM", 2);
  i = 2;
  val = fileStep * height + headerSize;
  MEM_SET32L(buf, i, val);   // file size
  val = 0;
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, headerSize);

  // write bitmap header
  MEM_SET32L(buf, i, bitmapHeaderSize);
  MEM_SET32L(buf, i, width);
  MEM_SET32L(buf, i, height);
  st = 1;
  MEM_SET16L(buf, i, st);
  st = (short)(channels << 3);
  MEM_SET16L(buf, i, st);
  val = 0;
  MEM_SET32L(buf, i, val);
  val = 0;
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, val);
  stream_write(s, buf, i);

  if (channels == 1) {
    palette_fillgray(1 << 8, buf, FALSE);
    stream_write(s, buf, 1024);
  }

  width *= channels;
  data += step * (height - 1);

  for (; height--; data -= step) {
    stream_write(s, data, width);

    if (fileStep > width) {
      stream_write(s, zeropad, fileStep - width);
    }
  }

  return 1;
}

#define bmp_load_filename(fname, cn_req, im, pbpp, palette) bmp_load_file_close(fopen(fname, "rb"), cn_req, im, pbpp, palette)
CC_INLINE int bmp_load_file_close(FILE* pf, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = bmp_load(s, cn_req, im, pbpp, palette);
    fclose(pf);
  }

  return ret;
}
#define bmp_save_filename(fname, height, width, data, step, channels, palette) bmp_save_file_close(fopen(fname, "wb"), height, width, data, step, channels, palette)
#define bmp_saveim_filename(fname, im, palette)  bmp_save_filename(fname, im->h, im->w, im->tt.data, im->s, im->c, palette)
CC_INLINE int bmp_save_file_close(FILE* pf, int height, int width, const uchar* data, int step, int channels, const uchar* palette)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = bmp_save(s, height, width, data, step, channels, palette);
    fclose(pf);
  }

  return ret;
}
//
typedef struct jpg_writer_t {
  uint32 buffer;
  uint8 ptr;
  stream_t* fp;
} jpg_writer_t;
#define MARKER_SOI  1
#define MARKER_APP0 2
#define MARKER_DQT  3
#define MARKER_SOF0 4
#define MARKER_DHT  5
#define MARKER_SOS  6
#define MARKER_EOI  7
//定义DCT表的大小
#define DCT_SIZE  8
CC_INLINE int jpg_writer_init(jpg_writer_t* strm, stream_t* fp)
{
  strm->fp = fp;

  if (strm->fp == NULL) {
    return -1;  /*ERR_FILE_CANT_OPEN;*/
  }

  stream_seek(strm->fp, 0, SEEK_SET);
  strm->buffer = 0;
  strm->ptr = 0;
  return 0;
}
CC_INLINE int jpg_writer_write_direct(jpg_writer_t* strm, const uint8* buffer, uint32 length)
{
  if (strm->ptr != 0) {
    return -1;  //ERR_STREAM_NOT_EMPTY
  }

  stream_write(strm->fp, buffer, length);
  return 0;
}
CC_INLINE void jpg_writer_write_stream(jpg_writer_t* strm, uint32 code, uint8 length, int flag)
{
  int i, k, len = length;
  uint8 fillbyte = 0;

  while (len > 0) {
    while (strm->ptr == 32) {
      for (i = 3; i >= 0; i--) {
        stream_write(strm->fp, ((uint8*) & (strm->buffer)) + i, 1);
        strm->ptr -= 8;

        if (flag) {
          if (*(((uint8*) & (strm->buffer)) + i) == 0xff) {
            stream_write(strm->fp, &fillbyte, 1);
          }
        }
      }

      strm->buffer = 0;
    }

    if (len <= 32 - strm->ptr) {
      k = len;
    }
    else {
      k = 32 - strm->ptr;
    }

    strm->buffer <<= k;
    strm->buffer |= (code >> (len - k)) & ((1 << k) - 1);
    strm->ptr += k;
    len -= k;
  }
}
CC_INLINE void jpg_writer_flush_stream(jpg_writer_t* strm, int flag)
{
  int i;
  uint8 fillbyte = 0;

  while (strm->ptr % 8 != 0) {
    jpg_writer_write_stream(strm, 1, 1, 1);
  }

  for (i = (strm->ptr) / 8 - 1; i >= 0; i--) {
    stream_write(strm->fp, ((uint8*) & (strm->buffer)) + i, 1);
    strm->ptr -= 8;

    if (flag) {
      if (*(((uint8*) & (strm->buffer)) + i) == 0xff) {
        stream_write(strm->fp, &fillbyte, 1);
      }
    }
  }

  strm->buffer = 0;
}
//亮度量化表,教材<<多媒体技术原理及应用>>p28表2.2
static const uint8 g_lumQuantTableDef[DCT_SIZE][DCT_SIZE] = {
  { 16, 11, 10, 16, 24, 40, 51, 61 },
  { 12, 12, 14, 19, 26, 58, 60, 55 },
  { 14, 13, 16, 24, 40, 57, 69, 56 },
  { 14, 17, 22, 29, 51, 87, 80, 62 },
  { 18, 22, 37, 56, 68, 109, 103, 77 },
  { 24, 35, 55, 64, 81, 104, 113, 92 },
  { 49, 64, 78, 87, 103, 121, 120, 101},
  { 72, 92, 95, 98, 112, 100, 103, 99 }
};
//色度量化表,教材<<多媒体技术原理及应用>>p28表2.3
static const uint8 g_chrQuantTableDef[DCT_SIZE][DCT_SIZE] = {
  { 17, 18, 24, 47, 99, 99, 99, 99 },
  { 18, 21, 26, 66, 99, 99, 99, 99 },
  { 24, 26, 56, 99, 99, 99, 99, 99 },
  { 47, 66, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 }
};
//z型扫描表,教材<<多媒体技术原理及应用>>p29图2.12
static const uint8 g_rleZigZagTable[DCT_SIZE][DCT_SIZE] = {
  { 0, 1, 5, 6, 14, 15, 27, 28 },
  { 2, 4, 7, 13, 16, 26, 29, 42 },
  { 3, 8, 12, 17, 25, 30, 41, 43 },
  { 9, 11, 18, 24, 31, 40, 44, 53 },
  { 10, 19, 23, 32, 39, 45, 52, 54 },
  { 20, 22, 33, 38, 46, 51, 55, 60 },
  { 21, 34, 37, 47, 50, 56, 59, 61 },
  { 35, 36, 48, 49, 57, 58, 62, 63 }
};
//离散余弦变换函数,公式教材<<多媒体技术原理及应用>>p27
CC_INLINE void DCT_AAN(double dct[], double pic[])
{
  static double x01, x11, x21, x31, x41, x51, x61, x71;
  static double x0, x1, x2, x3;
  x01 = pic[0] + pic[7];
  x11 = pic[1] + pic[6];
  x21 = pic[2] + pic[5];
  x31 = pic[3] + pic[4];
  x41 = pic[3] - pic[4];
  x51 = pic[2] - pic[5];
  x61 = pic[1] - pic[6];
  x71 = pic[0] - pic[7];
  x0 = x01 + x31;
  x1 = x11 + x21;
  x2 = x11 - x21;
  x3 = x01 - x31;
  dct[0] = 0.35355339 * (x0 + x1);
  dct[4] = 0.35355339 * (x0 - x1);
  x2 = 0.707106781 * (x2 + x3);
  dct[2] = 0.27059805 * (x2 + x3);
  dct[6] = 0.653281482 * (x3 - x2);
  x0 = x41 + x51;
  x1 = x51 + x61;
  x2 = x61 + x71;
  x3 = 0.382683432 * (x0 - x2);
  x41 = 0.541196100 * x0 + x3;
  x51 = 0.707106781 * x1;
  x61 = 1.306562965 * x2 + x3;
  x1 = x51 + x71;
  x3 = x71 - x51;
  dct[5] = 0.449988111 * (x41 + x3);
  dct[1] = 0.254897789 * (x1 + x61);
  dct[7] = 1.281457724 * (x1 - x61);
  dct[3] = 0.300672443 * (x3 - x41);
}
//离散余弦变换调用函数
CC_INLINE void DCT_8x8(double dct[][DCT_SIZE], double pic[][DCT_SIZE])
{
  int i, j;
  double res[DCT_SIZE][DCT_SIZE], out[DCT_SIZE], in[DCT_SIZE];

  for (i = 0; i < DCT_SIZE; i++) {
    for (j = 0; j < DCT_SIZE; j++) {
      in[j] = pic[i][j] - 128;  //注意此处
    }

    DCT_AAN(res[i], in);
  }

  for (i = 0; i < DCT_SIZE; i++) {
    for (j = 0; j < DCT_SIZE; j++) {
      in[j] = res[j][i];
    }

    DCT_AAN(out, in);

    for (j = 0; j < DCT_SIZE; j++) {
      dct[j][i] = out[j];
    }
  }
}
//建立量化表
CC_INLINE void InitQuantTable(uint8 compQuantTablePtr[][DCT_SIZE], const uint8 compQuantTableDef[][DCT_SIZE], int QValue)
{
  int i, j, val;

  if (QValue < 1) {
    QValue = 1;
  }

  if (QValue > 100) {
    QValue = 100;
  }

  if (QValue < 50) {       //nonlinear mapping 1->5000, 10->500, 25->200, 50->100, 75->50, 100->0
    QValue = 5000 / QValue;
  }
  else {
    QValue = 200 - QValue * 2;
  }

  for (i = 0; i < DCT_SIZE; i++) {
    for (j = 0; j < DCT_SIZE; j++) {
      val = (compQuantTableDef[i][j] * QValue + 50) / 100;

      if (val < 1) {
        val = 1;
      }

      if (val > 255) {
        val = 255;
      }

      compQuantTablePtr[i][j] = val;
    }
  }

  return ;
}
//量化处理函数
CC_INLINE void QuantDCTValue(int quant[][DCT_SIZE], double dct[][DCT_SIZE], uint8 compQuantTable[][DCT_SIZE])
{
  int i, j;

  for (i = 0; i < DCT_SIZE; i++) {
    for (j = 0; j < DCT_SIZE; j++) {
      if (dct[i][j] >= 0) {
        quant[i][j] = (int)(dct[i][j] / compQuantTable[i][j] + 0.5);
      }
      else {
        quant[i][j] = (int)(dct[i][j] / compQuantTable[i][j] - 0.5);
      }
    }
  }

  return ;
}
//z型扫描函数
CC_INLINE void ZigZagScan(int scan[], int quant[][DCT_SIZE])
{
  int i, j;

  for (i = 0; i < DCT_SIZE; i++) {
    for (j = 0; j < DCT_SIZE; j++) {
      scan[g_rleZigZagTable[i][j]] = quant[i][j];
    }
  }

  return ;
}
//行程编码函数
CC_INLINE int RunLengthEncode(int zlen[], int num[], int* cnt, int scan[])
{
  int k = 0, z = 0;
  (*cnt) = 0;

  do {
    k++;

    if (scan[k] == 0) {
      if (k == 63) {
        zlen[*cnt] = 0;
        num[*cnt] = 0;
        (*cnt)++;
      }
      else {
        z++;
      }
    }
    else {
      while (z > 15) {
        zlen[*cnt] = 15;
        num[*cnt] = 0;
        (*cnt)++;
        z -= 16;
      }

      zlen[*cnt] = z;
      num[*cnt] = scan[k];
      (*cnt)++;
      z = 0;
    }
  }
  while (k < 63);

  return 0;
}
// Define Default Huffman Bit and Val Tables
// Define Default Huffman Bit & Val Table (DC coef) For Luminance Component
const uint8 g_lumDCHuffBitDef[17] = { 0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };  // [0] not used
const uint8 g_lumDCHuffValDef[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
//  Define Default Huffman Bit & Val Table (AC coef) For Luminance Component
static const uint8 g_lumACHuffBitDef[17] =  { 0, 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d };  // [0] not used
static const uint8 g_lumACHuffValDef[162] = {
  0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
  0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
  0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
  0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
  0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
  0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
  0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
  0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
  0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
  0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
  0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
  0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
  0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
  0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
  0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
  0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
  0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
  0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
  0xf9, 0xfa
};
//  Define Default Huffman Bit & Val Table (DC coef) For Chrominance Component
static const uint8 g_chrDCHuffBitDef[17] = { 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };  // [0] not used
static const uint8 g_chrDCHuffValDef[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
//  Define Default Huffman Bit & Val Table (AC coef) For Chrominance Component
static const uint8 g_chrACHuffBitDef[17] =  { 0, 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 };  // [0] not used
static const uint8 g_chrACHuffValDef[162] = {
  0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
  0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
  0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
  0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
  0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
  0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
  0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
  0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
  0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
  0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
  0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
  0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
  0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
  0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
  0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
  0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
  0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
  0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
  0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
  0xf9, 0xfa
};
// Define Huffman CodeWord and Size Tables For Encoder & Decoder
// Define Huffman Trees For Decoder
static uint8 g_groupVLILookupTable[1 << 15]; // 2^15 = 32768
CC_INLINE void MakeHuffTable(uint32* code, uint8* size, const uint8* bit, const uint8* val)
{
  int i, j, k = 0;
  uint32 mode = 0;
  uint32 tmpcode[162];
  uint8 tmpsize[162];

  for (i = 0; i < 256; i++) {
    code[i] = 0;
    size[i] = 0;
  }

  for (i = 1; i <= 16; i++) {
    if (bit[i] > 0) {
      for (j = 0; j < bit[i]; j++) {
        tmpsize[k] = (unsigned char)i;
        tmpcode[k] = mode;
        mode++;
        k++;
      }
    }

    mode <<= 1;
  }

  for (i = 0; i < k; i++) {
    code[val[i]] = tmpcode[i];
    size[val[i]] = tmpsize[i];
  }

  return ;
}
//确保数值在0-255
CC_INLINE uint8 BYTE_SCOPE(double val)
{
  int ival = (int)val;

  if (ival < 0) {
    return 0;
  }

  if (ival > 255) {
    return 255;
  }

  return (uint8)ival;
}
//转换彩色空间,公式如下
//Y = 0.299*R + 0.587*G + 0.114*B (Lum)
//Cb = - 0.1687*R - 0.3313*G + 0.5*B + 128 (Chr Blue)
//Cr = 0.5*R - 0.4187*G - 0.0813*B + 128 (Chr Red)
static int RGB2YCrCb(uint8* Y, uint8* Cr, uint8* Cb, const uint8* R, const uint8* G, const uint8* B, int width, int height)
{
  int i, j;
  int step = width;

  if (width % 2 != 0 || height % 2 != 0) {
    return -1;
  }

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      int r = R[i * step + j];
      int g = G[i * step + j];
      int b = B[i * step + j];
      *Y++ = BYTE_SCOPE(0.299 * r + 0.587 * g + 0.114 * b);
    }
  }

  for (i = 0; i < height; i += 2) {
    for (j = 0; j < width; j += 2) {
      int r = R[i * step + j];
      int g = G[i * step + j];
      int b = B[i * step + j];
      *Cr++ = BYTE_SCOPE(0.5 * r - 0.4187 * g - 0.0813 * b + 128);
      *Cb++ = BYTE_SCOPE(-0.1687 * r - 0.3313 * g + 0.5 * b + 128);
    }
  }

  return 0;
}
static int EncodeMarker(jpg_writer_t* strm, int mark, uint32 param1, uint32 param2)
{
  static uint8 SOI[2]   = { 0xff, 0xd8 };
  static uint8 APP0[18] = { 0xff, 0xe0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00};
  static uint8 DQT[5]   = { 0xff, 0xdb, 0x00, 0x43, 0 };
  static uint8 SOF0[19] = { 0xff, 0xc0, 0x00, 0x11, 0x08, 0,    0,    0,    0, 0x03, 0x01, 0x22, 0x00, 0x02, 0x11, 0x01, 0x03, 0x11, 0x01};
  static uint8 DHT[5]   = { 0xff, 0xc4, 0,    0,    0 };
  static uint8 SOS[14]  = { 0xff, 0xda, 0x00, 0x0c, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3f, 0x00};
  static uint8 EOI[2]   = { 0xff, 0xd9 };

  switch (mark) {
  case MARKER_SOI:
    jpg_writer_write_direct(strm, SOI, sizeof(SOI));
    break;

  case MARKER_APP0:
    jpg_writer_write_direct(strm, APP0, sizeof(APP0));
    break;

  case MARKER_DQT:
    DQT[4] = (uint8)param1;
    jpg_writer_write_direct(strm, DQT, sizeof(DQT));
    break;

  case MARKER_SOF0:
    SOF0[5] = (uint8)(param1 / 256);
    SOF0[6] = (uint8)(param1 % 256);
    SOF0[7] = (uint8)(param2 / 256);
    SOF0[8] = (uint8)(param2 % 256);
    jpg_writer_write_direct(strm, SOF0, sizeof(SOF0));
    break;

  case MARKER_DHT:
    DHT[2] = (uint8)((3 + param1) / 256);
    DHT[3] = (uint8)((3 + param1) % 256);
    DHT[4] = (uint8)param2;
    jpg_writer_write_direct(strm, DHT, sizeof(DHT));
    break;

  case MARKER_SOS:
    jpg_writer_write_direct(strm, SOS, sizeof(SOS));
    break;

  case MARKER_EOI:
    jpg_writer_write_direct(strm, EOI, sizeof(EOI));
    break;

  default:
    return -1; //ERR_MARKER_INVALID
  }

  return 0;
}
static void VLIntegerEncode(int* code, int* group, int val)
{
  int aval = abs(val);
  *group = g_groupVLILookupTable[aval];

  if (val >= 0) {
    *code = val;
  }
  else {
    *code = (1 << (*group)) - 1 - aval;
  }
}
static int EncodeBlock8x8(jpg_writer_t* strm, int* preCompDC, uint8 block[][DCT_SIZE],
    uint8 compQuantTablePtr[][DCT_SIZE],
    uint32* compDCHuffCodePtr, uint8* compDCHuffSizePtr,
    uint32* compACHuffCodePtr, uint8* compACHuffSizePtr)
{
  int i, j, k;
  double dct[DCT_SIZE][DCT_SIZE], arr[DCT_SIZE][DCT_SIZE];
  int quant[DCT_SIZE][DCT_SIZE], scan[DCT_SIZE * DCT_SIZE];
  int zlen[DCT_SIZE * DCT_SIZE], num[DCT_SIZE * DCT_SIZE], cnt;
  int difCompDC, code, group, val;

  for (i = 0; i < DCT_SIZE; i++)
    for (j = 0; j < DCT_SIZE; j++) {
      arr[i][j] = (double)block[i][j];
    }

  DCT_8x8(dct, arr);
  QuantDCTValue(quant, dct, compQuantTablePtr);
  difCompDC = quant[0][0] - (*preCompDC);
  (*preCompDC) = quant[0][0];
  VLIntegerEncode(&code, &group, difCompDC);
  jpg_writer_write_stream(strm, compDCHuffCodePtr[group], compDCHuffSizePtr[group], 1);

  if (difCompDC != 0) {
    jpg_writer_write_stream(strm, code, group, 1);
  }

  ZigZagScan(scan, quant);
  RunLengthEncode(zlen, num, &cnt, scan);

  for (k = 0; k < cnt; k++) {
    VLIntegerEncode(&code, &group, num[k]);
    val = (zlen[k] << 4) | group;
    jpg_writer_write_stream(strm, compACHuffCodePtr[val], compACHuffSizePtr[val], 1);

    if (zlen[k] != 0 || num[k] != 0) {
      jpg_writer_write_stream(strm, code, group, 1);
    }
  }

  return 0;
}

static int jpg_save(stream_t* s, int im_h, int im_w, const uchar* im_data, int im_s, int im_c, int QValue)
{
  int i, j, k, l, m, n, width, height, step, step2;
  int preYDC, preCbDC, preCrDC;
  uint8 block[DCT_SIZE][DCT_SIZE];
  uint8 arr[DCT_SIZE * DCT_SIZE];
  uint8* g_R, *g_G, *g_B;
  uint8* g_Y, *g_Cr, *g_Cb;
  int rw = im_w % 16, rh = im_h % 16;
  jpg_writer_t writer[1] = {0};
  static int inited = 0;
  static uint32 g_lumDCHuffCode[256], g_lumACHuffCode[256];
  static uint8 g_lumDCHuffSize[256], g_lumACHuffSize[256];
  static uint32 g_chrDCHuffCode[256], g_chrACHuffCode[256];
  static uint8 g_chrDCHuffSize[256], g_chrACHuffSize[256];
  //亮度系数表
  uint8 g_lumQuantTable[DCT_SIZE][DCT_SIZE];
  //色度系数表
  uint8 g_chrQuantTable[DCT_SIZE][DCT_SIZE];
  jpg_writer_init(writer, s);
  width = rw ? (im_w + (16 - rw)) : im_w;
  height = rh ? (im_h + (16 - rh)) : im_h;

  if (!inited) {
    inited = 1;
    //InitEntropyCodec(g_lumDCHuffBitDef, g_lumDCHuffValDef, g_lumACHuffBitDef, g_lumACHuffValDef, g_chrDCHuffBitDef, g_chrDCHuffValDef, g_chrACHuffBitDef, g_chrACHuffValDef);
    //int InitEntropyCodec(const uint8* g_lumDCHuffBitDef, const uint8* lumDCHuffValPtr, const uint8* lumACHuffBitPtr, const uint8* lumACHuffValPtr, const uint8* chrDCHuffBitPtr, const uint8* chrDCHuffValPtr, const uint8* chrACHuffBitPtr, const uint8* chrACHuffValPtr)
    MakeHuffTable(g_lumDCHuffCode, g_lumDCHuffSize, g_lumDCHuffBitDef, g_lumDCHuffValDef);
    MakeHuffTable(g_lumACHuffCode, g_lumACHuffSize, g_lumACHuffBitDef, g_lumACHuffValDef);
    MakeHuffTable(g_chrDCHuffCode, g_chrDCHuffSize, g_chrDCHuffBitDef, g_chrDCHuffValDef);
    MakeHuffTable(g_chrACHuffCode, g_chrACHuffSize, g_chrACHuffBitDef, g_chrACHuffValDef);
    {
      int lastpw = 1, pw = 1;
      g_groupVLILookupTable[0] = 0;

      for (i = 1; i < 16; i++) {
        pw *= 2;

        for (j = lastpw; j < pw; j++) {
          g_groupVLILookupTable[j] = i;
        }

        lastpw = pw;
      }
    }
  }

  //int Compact2Bitplane( uint8* R, uint8* G, uint8* B, const uint8* pBGR, int width, int height, int step, int cn )
  {
    int npix = width * height;
    step = width;
    step2 = width >> 1;
    g_R = (uint8*)pmalloc(npix * 5);
    g_G = g_R + npix;
    g_B = g_R + npix * 2;
    g_Y = g_R + npix * 3;
    g_Cr = g_R + npix * 4;
    g_Cb = g_R + npix * 4 + (npix >> 2);

    //获取RGB值
    //部分采样
    //int AlignBitplane16x16( uint8* R, uint8* G, uint8* B,int *wid, int *hei, int width, int height)
    for (i = 0; i < im_h; i++) {
      const uint8* pBGR_line = im_data + i * im_s;
      uint8* R = g_R + i * width;
      uint8* G = g_G + i * width;
      uint8* B = g_B + i * width;

      for (j = 0; j < im_w; j++) {
        *R++ = pBGR_line[2];
        *G++ = pBGR_line[1];
        *B++ = pBGR_line[0];
        pBGR_line += im_c;
      }
    }

    for (i = 0; i < im_h; i++) {
      for (j = im_w; j < width; j++) {
        g_R[i * step + j] = g_R[i * step + im_w - 1];
        g_G[i * step + j] = g_G[i * step + im_w - 1];
        g_B[i * step + j] = g_B[i * step + im_w - 1];
      }
    }

    for (j = 0; j < width; j++) {
      for (i = im_h; i < height; i++) {
        g_R[i * step + j] = g_R[(height - 1) * j];
        g_G[i * step + j] = g_G[(height - 1) * j];
        g_B[i * step + j] = g_B[(height - 1) * j];
      }
    }
  }
  RGB2YCrCb(g_Y, g_Cr, g_Cb, g_R, g_G, g_B, width, height);
  InitQuantTable(g_lumQuantTable, g_lumQuantTableDef, QValue);
  InitQuantTable(g_chrQuantTable, g_chrQuantTableDef, QValue);
  EncodeMarker(writer, MARKER_SOI, 0xff, 0xff);
  EncodeMarker(writer, MARKER_APP0, 0xff, 0xff);
  EncodeMarker(writer, MARKER_DQT, 0, 0xff);

  for (i = 0; i < DCT_SIZE; i++)
    for (j = 0; j < DCT_SIZE; j++) {
      arr[g_rleZigZagTable[i][j]] = g_lumQuantTable[i][j];
    }

  jpg_writer_write_direct(writer, arr, DCT_SIZE * DCT_SIZE);
  EncodeMarker(writer, MARKER_DQT, 1, 0xff);

  for (i = 0; i < DCT_SIZE; i++) {
    for (j = 0; j < DCT_SIZE; j++) {
      arr[g_rleZigZagTable[i][j]] = g_chrQuantTable[i][j];
    }
  }

  jpg_writer_write_direct(writer, arr, DCT_SIZE * DCT_SIZE);
  EncodeMarker(writer, MARKER_SOF0, im_h, im_w);
  EncodeMarker(writer, MARKER_DHT, sizeof(g_lumDCHuffBitDef) - 1 + sizeof(g_lumDCHuffValDef), 0x00);
  jpg_writer_write_direct(writer, g_lumDCHuffBitDef + 1, sizeof(g_lumDCHuffBitDef) - 1);
  jpg_writer_write_direct(writer, g_lumDCHuffValDef, sizeof(g_lumDCHuffValDef));
  EncodeMarker(writer, MARKER_DHT, sizeof(g_lumACHuffBitDef) - 1 + sizeof(g_lumACHuffValDef), 0x10);
  jpg_writer_write_direct(writer, g_lumACHuffBitDef + 1, sizeof(g_lumACHuffBitDef) - 1);
  jpg_writer_write_direct(writer, g_lumACHuffValDef, sizeof(g_lumACHuffValDef));
  EncodeMarker(writer, MARKER_DHT, sizeof(g_chrDCHuffBitDef) - 1 + sizeof(g_chrDCHuffValDef), 0x01);
  jpg_writer_write_direct(writer, g_chrDCHuffBitDef + 1, sizeof(g_chrDCHuffBitDef) - 1);
  jpg_writer_write_direct(writer, g_chrDCHuffValDef, sizeof(g_chrDCHuffValDef));
  EncodeMarker(writer, MARKER_DHT, sizeof(g_chrACHuffBitDef) - 1 + sizeof(g_chrACHuffValDef), 0x11);
  jpg_writer_write_direct(writer, g_chrACHuffBitDef + 1, sizeof(g_chrACHuffBitDef) - 1);
  jpg_writer_write_direct(writer, g_chrACHuffValDef, sizeof(g_chrACHuffValDef));
  EncodeMarker(writer, MARKER_SOS, 0xff, 0xff);
  preYDC = preCbDC = preCrDC = 0;

  for (i = 0; i < height; i += (DCT_SIZE << 1)) {
    for (j = 0; j < width; j += (DCT_SIZE << 1)) {
      for (k = 0; k < (DCT_SIZE << 1); k += DCT_SIZE) {
        for (l = 0; l < (DCT_SIZE << 1); l += DCT_SIZE) {
          for (m = 0; m < DCT_SIZE; m++) {
            for (n = 0; n < DCT_SIZE; n++) {
              block[m][n] = g_Y[(i + k + m) * width + j + l + n];
            }
          }

          EncodeBlock8x8(writer, &preYDC, block, g_lumQuantTable, g_lumDCHuffCode,
              g_lumDCHuffSize, g_lumACHuffCode, g_lumACHuffSize);
        }
      }

      for (m = 0; m < DCT_SIZE; m++) {
        for (n = 0; n < DCT_SIZE; n++) {
          block[m][n] = g_Cb[((i >> 1) + m) * step2 + (j >> 1) + n];
        }
      }

      EncodeBlock8x8(writer, &preCbDC, block, g_chrQuantTable, g_chrDCHuffCode,
          g_chrDCHuffSize, g_chrACHuffCode, g_chrACHuffSize);

      for (m = 0; m < DCT_SIZE; m++) {
        for (n = 0; n < DCT_SIZE; n++) {
          block[m][n] = g_Cr[((i >> 1) + m) * step2 + (j >> 1) + n];
        }
      }

      EncodeBlock8x8(writer, &preCrDC, block, g_chrQuantTable, g_chrDCHuffCode,
          g_chrDCHuffSize, g_chrACHuffCode, g_chrACHuffSize);
    }
  }

  jpg_writer_flush_stream(writer, 1);
  EncodeMarker(writer, MARKER_EOI, 0xff, 0xff);

  if (writer->ptr > 0) {
    jpg_writer_flush_stream(writer, 0);
  }

  pfree(g_R);
  return 1;
}
#define jpg_save_filename(fname, height, width, data, step, channels, QValue) jpg_save_file_close(fopen(fname, "wb"), height, width, data, step, channels, QValue)
#define jpg_saveim_filename(fname, im, QValue)  jpg_save_filename(fname, im->h, im->w, im->tt.data, im->s, im->c, QValue)
static int jpg_save_file_close(FILE* pf, int height, int width, const uchar* data, int step, int channels, int QValue)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = jpg_save(s, height, width, data, step, channels, QValue);
    fclose(pf);
  }

  return ret;
}

#define STBIDEF static
#include "fmt/jpg_load_stb.inl"
#include "fmt/jpg_load_opencv.inl"
#include "fmt/jpg_save_opencv.inl"
#include "fmt/png_load_stb.inl"
#include "fmt/png_save_stb.inl"
#include "fmt/gif.inl"
#include "fmt/tif_load.inl"
#if 0
#include "fmt1/irw.h"
typedef stream_t ISFILE;
#define CALLAGREEMENT
#define isirw_malloc pmalloc
#define isirw_free pfree
#define isio_seek stream_seek
#define isio_tell stream_tell
#define isio_length stream_filesize
#define isio_read(buf, i, j, pf) stream_read(pf, buf, (i)*(j))
#define isio_write(buf, i, j, pf) stream_write(pf, buf, (i)*(j))
#include "fmt1/rgbtab.inl"
#include "fmt1/xpm.inl"
//#include "fmt1/ani.inl"
#include "fmt1/cur.inl"
//#include "fmt1/ico1.inl"

#define xpm_load_filename(fname, cn_req, im, pbpp, palette) xpm_load_file_close(fopen(fname, "rb"), cn_req, im, pbpp, palette)
CC_INLINE int xpm_load_file_close(FILE* pf, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  int ret = 0;
  
  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = xpm_load(s, cn_req, im, pbpp, palette);
    fclose(pf);
  }
  
  return ret;
}

#define xpm_save_filename(fname, im, palette) xpm_save_file_close(fopen(fname, "wb"), im, fname, palette)
CC_INLINE int xpm_save_file_close(FILE* pf, const img_t* im, const char* name, const uchar* palette)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = xpm_save(s, im, name, palette);
    fclose(pf);
  }

  return ret;
}
#endif

#ifdef TEST

static int test_jpg()
{
  img_t im[1] = {0};
  int i;

  for (i = 0; i < 30; ++i) {
    remove("aaa.jpg");
    remove("aaa.bmp");

    if (bmp_load_filename("20120215161904_76607.BMP", 4, im, 0, 0)) {
      {
        utime_start(_start_time);
        jpg_save2_filename("aaa.jpg", im, 50);
        printf("jpg_save_filename %f\n", utime_elapsed(_start_time));
      }
      //imwrite("aaa.jpg", im);
    }

    imfree(im);

    if (jpg_load_filename("aaa.jpg", 3, im)) {
      int cn = 4;
      imfree(im);
      {
        utime_start(_start_time);
        //imload("aaa.jpg", im);
        jpg_load2_filename("aaa.jpg", 3, im);
        //imread("aaa.jpg", 3, 1, im);
        printf("jpg_load_filename %f\n", utime_elapsed(_start_time));
      }
      bmp_saveim_filename("aaa.bmp", im, 0);
      //IMINIT(im, Y_image, X_image, our_image_buffer, X_image * cn, cn, 1);
      // bmp_save_filename("aaa.bmp", im, 0);
    }

    imfree(im);
  }

  return 0;
}

static int test_gif()
{
  img_t im[1] = {0};
  int i;

  for (i = 0; i < 30; ++i) {
    remove("aaa.gif");
    remove("aaa.bmp");

    if (gif_load_filename("20110120030714810046.gif", 3, im)) {
      int cn = 4;
      bmp_saveim_filename("aaa.bmp", im, 0);
    }

    {
      int ch = 0, j = 0;

      for (; ch != 'q';) {
        uchar* data = im->tt.data + (j % im->f) * im->h * im->s;
        //cvShowImage("gif", im->h, im->w, data, im->s, im->c);
        //ch = cvWaitKey(50);
        ++j;
      }
    }

    imfree(im);
  }

  return 0;
}

static int test_png()
{
  img_t im[1] = {0};
  int i;

  for (i = 0; i < 30; ++i) {
    remove("aaa.jpg");
    remove("aaa.bmp");
    remove("aaa.png");

    if (png_load_filename("bb05.png", 4, im)) {
      {
        utime_start(_start_time);
        bmp_saveim_filename("aaa.bmp", im, 0);
        printf("jpg_save_filename %f\n", utime_elapsed(_start_time));
      }
      //imwrite("aaa.jpg", im);
    }

    if (bmp_load_filename("aaa.bmp", 3, im, 0, 0)) {
      int cn = 4;
      {
        utime_start(_start_time);
        //imload("aaa.jpg", im);
        png_saveim_filename("aaa.png", im);
        //imread("aaa.jpg", 3, 1, im);
        printf("png_save_filename %f\n", utime_elapsed(_start_time));
      }
      //IMINIT(im, Y_image, X_image, our_image_buffer, X_image * cn, cn, 1);
      // bmp_save_filename("aaa.bmp", im, 0);
    }

    imfree(im);
  }

  return 0;
}
#endif // TEST

static img_t* imload_stream(stream_t* s, int cn_req, img_t* im)
{
  int ret = bmp_load(s, cn_req, im, 0, 0)
    || jpg_load(s, cn_req, im)
    //|| jpg_load2(s, cn_req, im)
    || png_load(s, cn_req, im)
    || tif_load(s, cn_req, im)
    || gif_load(s, cn_req, im);
  return ret ? im : NULL;
}

#define imload_filename(fname, cn_req, im) imload_file_close(fopen(fname, "rb"), cn_req, im)

static img_t* imload_file_close(FILE* pf, int cn_req, img_t* im)
{
  img_t* ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = imload_stream(s, cn_req, im);
    fclose(pf);
  }

  return ret;
}

// filename 
static img_t* imread(const char* picname, int cn_req, int frames, img_t* im) {
  return imload_file_close(fopen(picname, "rb"), cn_req, im);
}
static img_t* imload_mem(void* buf, int len, int cn_req, img_t* im)
{
  img_t* ret = 0;
  
  if (buf) {
    stream_t s[1] = {0};
    memstream_init(s, buf, len);
    ret = imload_stream(s, cn_req, im);
  }
  
  return ret;
}

#if 0
static int imfile_to_inlfile(const char* imfile, const char* inlfile, uchar* map1)
{
  img_t im[1] = {0};
  img_t im1[1] = {0};
  imload_filename(imfile, 0, im);
  imto1(im, im1, map1);
  savedata_inl(inlfile, im1->tt.data, im1->h * im1->s);
  return 0;
}
static int test_imfile_to_inlfile()
{
  uchar map[256] = {0};
  map[0] = 1;
  imfile_to_inlfile("D:/code/c/GUI/bcgsoft/BCG/BCGCBPro/res/menuimg-pro.bmp", "menuimg_9x324.inl", map);
  return 0;
}
#endif
// 把横向排列的图片序列换成列向的
static int imlist_trans(const img_t* im, img_t* im2, int w)
{
  int i, j, n = im->w / w;
  img_t im1[1] = {0};
  im = (im==im2) ? (imclone2(im, im1), im1) : im;
  imsetsize(im2, im->h, w, im->c, n);

  for (i = 0; i < im->h; ++i) {
    for (j = 0; j < n; ++j) {
      memcpy(im2->tt.data + (i + j * im->h)*im2->s, im->tt.data + i * im->s + j * w * im2->c, im2->w * im2->c);
    }
  }
  imfree(im1);
  return 0;
}
#define imload_imagelist(pstrText, im, transparent_color)  imload_imagelist1(pstrText, im, transparent_color, 0)
static int imload_imagelist1(const char* pstrText, int req_cn, img_t* im, COLOR transparent_color, int w, int n)
{
  imload_filename(pstrText, req_cn, im);
  imsettranscolor(im, transparent_color);

  if (im->h >= im->w) {
    im->f = n<=0 ? im->h/im->w : n;
    im->h = im->h/im->f;
  }
  else if (im->w >= im->h) {
    w = w <= 0 ? (n ? im->w / n : im->h) : w;
    imlist_trans(im, im, w);
  }
  else {
    imfree(im);
  }

  return 0;
}
static int imload_images(img_t* im, int n, const char** pstrText, COLOR transparent_color)
{
  int i, ret = 0;
  img_t im2[1] = {0};
  imload_filename(pstrText[0], 4, im2);
  imsettranscolor(im2, transparent_color);
  imsetsize(im, im2->h, im2->w, 4, n);
  memcpy(im->tt.data, im2->tt.data, im2->s * im2->h);

  for (i = 1; i < n; ++i) {
    imload_filename(pstrText[i], 4, im2);
    imsettranscolor(im2, transparent_color);
    ASSERT(im2->h == im->h && im2->w == im->w);
    memcpy(im->tt.data + i * im2->s * im2->h, im2->tt.data, im2->s * im2->h);
  }

  imfree(im2);
  return 0;
}
static int imload_images2(img_t* im, int cn_req, int n, const str_t* pstrText, COLOR transparent_color)
{
  int i, ret = 0;
  img_t im2[1] = {0};
  imload_filename(pstrText[0].s, cn_req, im2);
  imsettranscolor(im2, transparent_color);
  imsetsize(im, im2->h, im2->w, cn_req, n);
  memcpy(im->tt.data, im2->tt.data, im2->s * im2->h);
  
  for (i = 1; i < n; ++i) {
    imload_filename(pstrText[i].s, cn_req, im2);
    imsettranscolor(im2, transparent_color);
    ASSERT(im2->h == im->h && im2->w == im->w);
    memcpy(im->tt.data + i * im2->s * im2->h, im2->tt.data, im2->s * im2->h);
  }
  
  imfree(im2);
  return 0;
}

#if 0
static int test_stbi()
{
  unsigned char* pImage;
  unsigned char* pData;
  int dwSize, x, y, n;
  sys_chdir("D:/pub/bin/imgproc/draw/res");
  pData = (unsigned char*)loaddata0("Progress_Background.png", &dwSize);
  pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
  pfree(pData);
  pfree(pImage);

  {
    const char* path = "E:/pub/bin/face/bin/20151123-20151126二幼总/1103_0F0086_20151124170536294.algo.bmp";
    img_t im[1] = {0};
    imread(path, 3, 1, im);
    imshow(im);
    //cvShowMat("asdf", "");
    cvWaitKey(-1);
  }
  return 0;
}
#endif

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb/stb_image_write.inl"
#define IMWRITE(NAME, h, w, p, s, c)  {img_t im[1] = {0};IMINIT(im, h, w, p, s, c, 1); imwrite(NAME, im);}

static int imsave_stream(stream_t* s, const char* ext, int height, int width, const uchar* data, int step, int channels) {
  int ret = 0;
  if (ext) {
    int type = splitfind_c("|bmp|png|jpg|jpeg|", -1, ext, -1, 1, 0);
    
    switch (type) {
    case 0:
      ret = bmp_save(s, height, width, data, step, channels, 0);
      break;
      
    case 1:
      ret = png_save(s, height, width, data, step, channels);
      break;
      
    case 2:
    case 3:
      ret = jpg_save(s, height, width, data, step, channels, 70);
      break;
    }
  }
  return ret;
}
static int imsave(const char* filename, int height, int width, const uchar* data, int step, int channels) {
  const char* dotext = strrchr(filename, '.');
  int ret = 0;
  if (dotext) {
    FILE* pf = fopen(filename, "wb");
    if (pf) {
      stream_t s[1] = {0};
      fstream_init(s, pf);
      ret = imsave_stream(s, dotext+1, height, width, data, step, channels);
      fclose(pf);
    }
  }
  return ret;
}
static int imsave_mem(void* buf, int len, const char* ext, int height, int width, const uchar* data, int step, int channels) {
  stream_t s[1] = {0};
  //int ret = 0;
  memstream_init(s, buf, len);
  imsave_stream(s, ext, height, width, data, step, channels);
  return s->x1;
}
static int imwrite(const char* filename, const img_t* im)
{
  return imsave(filename, im->h, im->w, im->tt.data, im->s, im->c);
}

#if 0
static int imsavetxt(const char* filename, const img_t* im, const char* fmt) {
  FILE* pf = fopen(filename, "wb");
  if (pf) {
    int i, j;
    char buf[256];
    fprintf(pf, "[%d %d %d %d]\n", im->h, im->w, im->c, im->f);
    for (j = 0; j < im->h; ++j) {
      const uchar* im1 = img_row(uchar, im, j);
      for (i = 0; i < im->w; ++i) {
        char* arglist = (char*)im1 + i*im->c;
        arglist = vsnprintf(buf, 256, fmt, arglist);
        fprintf(pf, "%s", buf);
      }
      fprintf(pf, "\n");
    }
    fprintf(pf, "\n");
    fclose(pf);
  }
  return 0;
}
#endif
#define imwrite2(filename, im)   imwrite_file(fopen(filename, "wb"), im, 0)

static int imwrite4(const char* filename, int h, int w, const unsigned char* data, int step, int cn)
{
  img_t im[1] = {0};
  IMINIT(im, h, w, data, step, cn, 1);
  imwrite(filename, im);
  return 0;
}


#endif // _IMGIO_INL_

