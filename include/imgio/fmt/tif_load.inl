
#include "lzw.inl"
#include "tif_swab.inl"
#include "tif_fax3.inl"

typedef struct TIFF_IMAGEFILE_HEADER {
  WORD byteOrder;
  WORD version;
  DWORD offsetToIFD;
} TIFF_IMAGEFILE_HEADER;

typedef struct TIFF_DIRECTORY_ENTRY {
  WORD tag;
  WORD type;
  DWORD length;
  DWORD valueOffset;
} TIFF_DIRECTORY_ENTRY;
typedef struct tif_reader_t {
  WORD tag;
  WORD type;
  DWORD length;
  DWORD valueOffset;
} tif_reader_t;

#define TIF_ERROR(x)  printf("%s\n", x)

static int tif_get_value(stream_t* f, TIFF_DIRECTORY_ENTRY* pTemp, int order, int* len, int* pvalue, int** pvalues)
{
  int ret = 0;
  *len = pTemp->length;

  if (pTemp->length > 1) {
    int j;
    stream_seek(f, pTemp->valueOffset, SEEK_SET);
    *pvalues = (int*)realloc(*pvalues, sizeof(int) * pTemp->length);

    if (4 == pTemp->type) {
      for (j = 0; j < (int)pTemp->length; ++j) {
        int x = stream_getu4(f);
        BYTEORDER(x);
        (*pvalues)[j] = x;
        ret += x;
      }
    }
    else {
      for (j = 0; j < (int)pTemp->length; ++j) {
        short x = stream_getu2(f);
        BYTEORDER(x);
        (*pvalues)[j] = x;
        ret += x;
      }
    }
  }
  else {
    ret = pTemp->valueOffset;
  }

  *pvalue = ret;
  return ret;
}

static int tif_decode(int compression, uchar* pix, int n, const uchar* pbuf, int m, int rowbytes, int rowpixels)
{
  int ret = 0;
  rowbytes = rowbytes ? rowbytes : (rowpixels + 7) / 8;

  switch (compression) {
  case 5:
    ret = LZWDecode(pix, n, pbuf, m);
    break;

  case 4:
    ret = FAX4Decode(pix, n, pbuf, m, rowbytes, rowpixels);
    break;

  default:
    break;
  }

  return ret;
}
static BOOL tif_load(stream_t* f, int cn_req, img_t* im)
{
  //DWORD dwFileLength = 0;
  WORD wDECount = 0;
  char strTemp[256];
  BYTE* pDIB = NULL;
  int y, i = 0;
  TIFF_DIRECTORY_ENTRY* pde = NULL;
  enum {MAX_TAGS = 32};
  int len[MAX_TAGS] = {0};
  int value[MAX_TAGS] = {0};
  int* values[MAX_TAGS] = {0};
  enum {
#define TIFTAGDEF(a, b)  t_##b,
#include "tiftagdef.txt"
#undef TIFTAGDEF
  };
  uchar* pix = NULL;
  int order = 0;
  int bpp = 0;
  int rowbytes = 0;

  TIFF_IMAGEFILE_HEADER ifh[1] = {0};
  memset(&ifh, 0, sizeof(TIFF_IMAGEFILE_HEADER));

  //dwFileLength = file.GetLength();
  if (sizeof(TIFF_IMAGEFILE_HEADER) != stream_read(f, ifh, sizeof(TIFF_IMAGEFILE_HEADER))) {
    TIF_ERROR("读TIF文件头失败");
    return FALSE;
  }

  // 0x4d4d表示该TIFF图是摩托罗拉整数格式 0x4949表示该图是Intel整数格式
  if (0x4d4d != ifh->byteOrder && 0x4949 != ifh->byteOrder) {
    TIF_ERROR("该TIF文件不是IBMPC字节序，读文件失败");
    return FALSE;
  }

  order = 0x4d4d != ifh->byteOrder;
  BYTEORDER(ifh->version);

  if (0x2a != ifh->version) {
    TIF_ERROR("该文件不是TIF格式，读文件失败");
    return FALSE;
  }

  BYTEORDER(ifh->offsetToIFD);
  stream_seek(f, ifh->offsetToIFD, SEEK_SET);

  if (2 != stream_read(f, &wDECount, 2)) {
    TIF_ERROR("无法获得TIF文件目录入口数量");
    return FALSE;
  }

  BYTEORDER(wDECount);
  snprintf(strTemp, 256, "该TIF文件有%d个目录入口", wDECount);
  TIF_ERROR(strTemp);

  pde = MALLOC(TIFF_DIRECTORY_ENTRY, wDECount);

  if ((int)sizeof(TIFF_DIRECTORY_ENTRY)*wDECount != stream_read(f, pde, sizeof(TIFF_DIRECTORY_ENTRY)*wDECount)) {
    TIF_ERROR("读图象文件目录失败");
    FREE(pde);
    return FALSE;
  }

  //显示图象目录入口信息--测试用
  for (i = 0; i < wDECount; i++) {
    TIFF_DIRECTORY_ENTRY* pTemp = pde + i;
    BYTEORDER(pTemp->tag);
    BYTEORDER(pTemp->type);
    BYTEORDER(pTemp->length);

    if (pTemp->length == 1) {
      switch (pTemp->type) {
      case 3:
        BYTEORDER_I2(pTemp->valueOffset);
        break;

      case 4:
        BYTEORDER_I4(pTemp->valueOffset);
        break;
      }
    }

    switch (pTemp->tag) {
#define TIFTAGDEF(a, b)  case a: printf("DE[%2d] %3d %d l=%6d v=%8d %s\n", i, (int)pTemp->tag, (int)pTemp->type, (int)pTemp->length, (int)pTemp->valueOffset,  #b ); break;
#include "tiftagdef.txt"
#undef TIFTAGDEF
    }

    switch (pTemp->tag) {
#define TIFTAGDEF(a, b)  case a: tif_get_value(f, pTemp, order, &len[t_##b], &value[t_##b], &values[t_##b]); break;
#include "tiftagdef.txt"
#undef TIFTAGDEF

    default: {
      int x = pTemp->tag;
      printf("tag = %d\n", x);
      //ASSERT(0);
    }
    break;
    }
  }

  cn_req = cn_req ? cn_req : value[t_samples_per_pixel];
  cn_req = cn_req ? cn_req : len[t_bits_per_sample];
  cn_req = cn_req ? cn_req : 1;
  imsetsize(im, value[t_image_height], value[t_image_width], cn_req, 1);
  //填充所有像素数据, 颠倒图象数据从最后一行开始读起

  stream_seek(f, sizeof(TIFF_IMAGEFILE_HEADER), SEEK_SET);
  bpp = value[t_bits_per_sample];
  bpp = bpp ? bpp : 1;
  rowbytes = (im->w * bpp + 7) / 8;

  if (1 != value[t_compression]) {
    uchar* pbuf = NULL;
    int m, n;
    //rowbytes = ALIGN(rowbytes, 4);
    n = im->h * rowbytes;
    pix = MALLOC(uchar, n);

    if (values[t_strip_offsets]) {
      int* off = values[t_strip_offsets];
      int* cnt = values[t_strip_byte_counts];
      int ll = len[t_strip_offsets];
      int nn = n / ll;
      pbuf = MALLOC(uchar, ll);
      ASSERT(ll == len[t_strip_byte_counts]);

      for (i = 0; i < len[t_strip_offsets]; ++i) {
        stream_seek(f, off[i], SEEK_SET);
        m = stream_read(f, pbuf, cnt[i]);
        n = tif_decode(value[t_compression], pix + i * nn, nn, pbuf, m, rowbytes, im->w);
      }

      {
        img_t im1[1] = {0};
      }

      FREE(pbuf);
    }
    else {
      pbuf = MALLOC(uchar, value[t_strip_byte_counts]);
      m = stream_read(f, pbuf, value[t_strip_byte_counts]);
      n = tif_decode(value[t_compression], pix, n, pbuf, m, rowbytes, im->w);
      FREE(pbuf);
    }
  }

  {
    int src_pitch = rowbytes;
    uchar* src_line = MALLOC(uchar, src_pitch);
    uchar m_palette[1024] = {0};

    if (1 == cn_req) {
      for (i = 0; i < 256; ++i) {
        m_palette[i] = i;
      }
    }
    else {
      for (i = 0; i < 256; ++i) {
        m_palette[i * 4 + 0] = i;
        m_palette[i * 4 + 1] = i;
        m_palette[i * 4 + 2] = i;
        m_palette[i * 4 + 3] = 0;
      }
    }
    
    for (y = 0; y < im->h; ++y) {
      uchar* data = im->tt.data + y * im->s;
      
      if (1 != value[t_compression]) {
        memcpy(src_line, pix + y * src_pitch, src_pitch);
      }
      else {
        stream_read(f, src_line, src_pitch);
      }
      
      palette_fillrow(im->w, data, cn_req, src_line, bpp, m_palette);
    }

    FREE(src_line);
  }

  switch (value[t_predictor]) {
  case 2:
    for (y = 0; y < im->h; y++) {
      uchar* data = im->tt.data + y * im->s;
      hor_acc8(data, im->s, im->c);
    }

    break;

  default:
    break;
  }

  FREE(pix);

  for (i = 0; i < MAX_TAGS; ++i) {
    if (values[i]) {
      free(values[i]);
      values[i] = NULL;
    }
  }

  //imwrite("adsfadf.bmp", im);
#if 0
  imshow(im);
  cvWaitKey(-1);
#endif
  return TRUE;
}

#define tif_load_filename(fname, cn_req, im) tif_load_file_close(fopen(fname, "rb"), cn_req, im)
static int tif_load_file_close(FILE* pf, int cn_req, img_t* im)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = tif_load(s, cn_req, im);
    fclose(pf);
  }

  return ret;
}
