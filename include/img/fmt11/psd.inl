// PSD图像读写模块实现文件
//读取功能：
//BITMAP色,1位,RLE压缩与无压缩;
//RGB色,8位,RLE压缩与无压缩;
//灰度,8位,RLE压缩与无压缩;
//Duotone, 8位,RLE压缩与无压缩;
//CMYK色,8位,RLE压缩与无压缩;
//Lab色,8位,RLE压缩与无压缩;
//支持16位/通道，支持多通道图象
//保存功能：不支持

/* 文件判别标志 */
#define PSD_SIGNATURE_MARKER      0x53504238  /* "8BPS" */
#define PSD_VERSION_MARKER        0x100   /* 256 ，这两个标志都为MOTO字序。*/

/* PSD图像的存储方式 */
enum PSDIMAGEMODE {
  PSD_BITMAP = 0,     /* Bitmap 是位图,每个像素只要一个位(黑白) */
  PSD_GRAYSCALE = 1,    /* 灰度图(有8位和16位) */
  PSD_INDEXED = 2,    /* 索引图，有色盘(只有8位) */
  PSD_RGB = 3,      /* 分R,G,B三色的图( RRR..GGG..BBB.. ) */
  PSD_CMYK = 4,     /* 分C,M,Y,K四色的图( CCC..MMM..YYY..KKK.. ) */
  PSD_MULTICHANNEL = 7, /* 多通道 */
  PSD_DUOTONE = 8,    /* 双色 */
  PSD_LAB = 9       /* Lab色 */
};

/* 文件信息头结构 */
typedef struct {
  unsigned long   Signature;  /* 标签："8BPS" */
  unsigned short    Version;  /* 版本号。总是等于1 */
  unsigned short    Reserved0;
  unsigned short    Reserved1;  /* 保留6个字节 */
  unsigned short    Reserved2;
  unsigned short    Channels; /* 通道数(范围：1～24) */
  unsigned long   Rows;   /* 行数 (范围: 1 ～ 30000) */
  unsigned long   Columns;  /* 列数 (范围: 1 ～ 30000) */
  unsigned short    Depth;    /* 每通道的位数(可能的值是: 1, 8, 16) */
  unsigned short    Mode;   /* 色彩模式，其值可能是下面的一种:*/
  /*  Bitmap    =0
    Grayscale =1
    Indexed   =2
    RGB     =3
    CMYK    =4
    Multichannel=7
    Duotone   =8
    Lab     =9 */
} PSDFILEHEADER, *LPPSDFILEHEADER;


static double* lab_red_map = 0, *lab_green_map = 0, *lab_blue_map = 0;


/***********************************************************************************\
$                                                                                   $
$    函数： enum EXERESULT read_psd( ISFILE *pfile,                 $
$                   LPINFOSTR pInfo,              $
$                   LPPSDFILEHEADER psdheader)                  $
$           将指定文件读入目标缓冲区                                                $
$                                                                                   $
$    参数： file        指定文件                                                    $
$           pInfo       图像读写数据包结构指针                                      $
$     psdheader PSD文件信息头                       $
$                                                                                   $
$    返回：enum EXERESULT 型，参见iseeirw.h中相应定义                               $
$                                                                                   $
\***********************************************************************************/
static enum EXERESULT CALLAGREEMENT read_psd(ISFILE* pfile, LPINFOSTR pInfo, LPPSDFILEHEADER psdheader)
{
  unsigned long temp_data;

  int       i, j, rleline, declinesize;
  unsigned long datasize;
  unsigned char*   pfilebuf = 0, *pdecbuf = 0, *pbuf;
  unsigned short  compression, *prlelinearray = 0;

  static char   palbuf[768];

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pInfo && psdheader);


  __try {
    if (isio_seek(pfile, sizeof(PSDFILEHEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    /* 跳过调色板段、图象资源段、图象层和蒙板段 */
    if (_skip_block(pfile) || _skip_block(pfile) || _skip_block(pfile)) {
      b_status = ER_FILERWERR;
      __leave;
    }

    /* 读压缩算法字 */
    if (isio_read((void*)&compression, sizeof(unsigned short), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      __leave;
    }

    compression = EXCHANGE_WORD(compression);

    if (compression) {
      assert(compression == 1); /* RLE压缩 */

      /* 读出RLE行数组 */
      rleline = psdheader->Channels * psdheader->Rows;

      prlelinearray = isirw_malloc(rleline * sizeof(short));

      if (prlelinearray == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      for (i = 0; i < rleline; ++i) {
        short t;

        if (isio_read(&t, sizeof(short), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        prlelinearray[i] = EXCHANGE_WORD(t);
      }
    }

    /* 获得象素数据偏移 */
    datasize = isio_length(pfile) - isio_tell(pfile);

    pfilebuf = (unsigned char*)isirw_malloc(datasize);

    if (pfilebuf == NULL) {
      b_status = ER_MEMORYERR;
      __leave;
    }

    if (isio_read(pfilebuf, datasize, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      __leave;
    }

    /* 注意，一字节对齐哦 */
    declinesize = ((psdheader->Columns * psdheader->Depth + 7) / 8);

    if (compression) {
      unsigned char* pSource, *pDest;

      pdecbuf = isirw_malloc(declinesize * rleline);

      if (pdecbuf == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      pSource = pfilebuf;
      pDest = pdecbuf;

      for (i = 0; i < rleline; ++i) {
        _decode_rle_line(pSource, prlelinearray[i], pDest, declinesize);

        pSource += prlelinearray[i];
        pDest += declinesize;
      }

      /* 这两东东现在没用啦 */
      isirw_free(prlelinearray);
      prlelinearray = 0;

      isirw_free(pfilebuf);
      pfilebuf = 0;

      pbuf = pdecbuf;
    }
    else { /* 未压缩直接使用文件BUF */
      pbuf = pfilebuf;
    }

    /* 分类读取象素数据 */
    switch (psdheader->Mode) {
    case  PSD_BITMAP:   /* 单色黑白图 */
      pInfo->pal_count = 2;

      pInfo->palette[0] = 0xffffff;
      pInfo->palette[1] = 0x0;

      if ((b_status = _read_bitmap(pbuf, pInfo, psdheader)) != ER_SUCCESS) {
        __leave;
        break;
      }

      break;

    case  PSD_GRAYSCALE:    /* 灰度图 */
      if (psdheader->Depth != 16) {
        pInfo->pal_count = 256;

        /* 人工合成灰度调色板 */
        for (j = 0; j < (int)pInfo->pal_count; j++) {
          pInfo->palette[j] = (unsigned long)((j << 16) | (j << 8) | (j));
        }
      }
      else {
        pInfo->pal_count = 0;
      }

      if ((b_status = _read_grayscale_index_duotone(pbuf, pInfo, psdheader)) != ER_SUCCESS) {
        __leave;
        break;
      }

      break;

    case  PSD_INDEXED:    /* 调色板图 */
      pInfo->pal_count = 256;

      if (isio_seek(pfile, sizeof(PSDFILEHEADER), SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读取调色板尺寸值 */
      if (isio_read((void*)&temp_data, sizeof(unsigned long), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
        break;
      }

      temp_data = EXCHANGE_DWORD(temp_data) ;

      /* 如果调色板尺寸不是768（256×3），则认为是坏的图象 */
      if (temp_data != 768) {
        b_status = ER_BADIMAGE;
        __leave;
        break;
      }

      /* 读取调色板信息 */
      if (isio_read((void*)palbuf, 256 * 3, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
        break;
      }

      /* 导入数据包 */
      for (j = 0; j < (int)pInfo->pal_count; j++) {
        pInfo->palette[j] = (unsigned long)(((unsigned long)(unsigned char)palbuf[j] << 16) | ((unsigned long)(unsigned char)palbuf[j + 256] << 8) | ((unsigned long)(unsigned char)palbuf[j + 512]));
      }

      if ((b_status = _read_grayscale_index_duotone(pbuf, pInfo, psdheader)) != ER_SUCCESS) {
        __leave;
        break;
      }

      break;

    case  PSD_RGB:    /* RGB图 */
      pInfo->pal_count = 0;

      if ((b_status = _read_rgb(pbuf, pInfo, psdheader)) != ER_SUCCESS) {
        __leave;
        break;
      }

      break;

    case  PSD_CMYK:   /* CMYK图 */
      pInfo->pal_count = 0;

      if ((b_status = _read_cmyk(pbuf, pInfo, psdheader)) != ER_SUCCESS) {
        __leave;
        break;
      }

      break;

    case  PSD_DUOTONE:    /* 双色调图 */
      pInfo->pal_count = 256;

      /* 人工合成灰度调色板 */
      for (j = 0; j < (int)pInfo->pal_count; j++) {
        pInfo->palette[j] = (unsigned long)((j << 16) | (j << 8) | (j));
      }

      /* 双色调图将视为灰度图像读取 */
      if ((b_status = _read_grayscale_index_duotone(pbuf, pInfo, psdheader)) != ER_SUCCESS) {
        __leave;
        break;
      }

      break;

    case  PSD_LAB:    /* Lab图 */
      pInfo->pal_count = 0;

      if ((b_status = _read_lab(pbuf, pInfo, psdheader)) != ER_SUCCESS) {
        __leave;
        break;
      }

      break;

    case PSD_MULTICHANNEL:
      pInfo->pal_count = 0;

      if ((b_status = _read_nultichannel(pbuf, pInfo, psdheader)) != ER_SUCCESS) {
        __leave;
        break;
      }

      break;

    default:
      b_status = ER_BADIMAGE;
      __leave;
      break;
    }

    if (b_status == ER_SUCCESS) {
      /* 放这里就不支持用户中断了，谁有空就改改吧 */
      (*pInfo->irwpfun)(pInfo->pater, PFC_PROCESS, 0, 0, pInfo->height, IRWE_CALLBACK_FUNID_LOAD);
    }

  }
  __finally {
    if (pfilebuf) {
      isirw_free(pfilebuf);
    }

    if (prlelinearray) {
      isirw_free(prlelinearray);
    }

    if (pdecbuf) {
      isirw_free(pdecbuf);
    }
  }

  return b_status;
}


/* 读BITMAP型图到数据包 */
static enum EXERESULT CALLAGREEMENT _read_bitmap(unsigned char* pbuf, LPINFOSTR pInfo, LPPSDFILEHEADER psdheader)
{
  unsigned int i, linesize;

  linesize = (psdheader->Columns + 7) / 8;

  for (i = 0; i < pInfo->height; ++i) {
    memcpy(pInfo->pp_line_addr[i], pbuf, linesize);
    pbuf += linesize;
  }

  return ER_SUCCESS;
}


/* 读灰度、索引、双色调图到数据包
 * 灰度8/16(8位用调色板)
 * 双色调改为8位用调色板
 * 索引图8位
 */
static enum EXERESULT CALLAGREEMENT _read_grayscale_index_duotone(unsigned char* pbuf,
    LPINFOSTR pInfo,
    LPPSDFILEHEADER psdheader)
{
  unsigned int i, j, linesize;

  linesize = (psdheader->Columns * psdheader->Depth + 7) / 8;

  if (psdheader->Mode == PSD_GRAYSCALE && psdheader->Depth == 16) {
    assert(pInfo->bitcount == 16);

    /* 16 位灰度转555 */
    for (i = 0; i < psdheader->Rows; ++i) {
      unsigned short color;
      unsigned short* p;

      p = (unsigned short*)pInfo->pp_line_addr[i];

      for (j = 0; j < linesize / sizeof(short); ++j) {
        color = *((unsigned short*)pbuf)++;
        color >>= 11;
        *p++ = color << 10 | color << 5 | color;
      }
    }
  }
  else {
    for (i = 0; i < psdheader->Rows; ++i) {
      memcpy(pInfo->pp_line_addr[i], pbuf, linesize);
      pbuf += linesize;
    }
  }

  return ER_SUCCESS;
}



/* 读RGB图到数据包 有8位和16位图 */
static enum EXERESULT CALLAGREEMENT _read_rgb(unsigned char* pbuf,
    LPINFOSTR pInfo,
    LPPSDFILEHEADER psdheader)
{
  unsigned int i, j;
  unsigned int imagesize;
  unsigned char* pImageR, *pImageG, *pImageB;

  assert(pInfo->bitcount == 24);
  assert(psdheader->Depth == 8 || psdheader->Depth == 16);
  assert(psdheader->Channels > 2);

  imagesize = psdheader->Rows * psdheader->Columns;

  pImageR = pbuf;

  if (psdheader->Depth == 8) {
    pImageG = pImageR + imagesize;
    pImageB = pImageG + imagesize;
  }
  else { /* Depth == 16 */
    pImageG = pImageR + imagesize * 2;
    pImageB = pImageG + imagesize * 2;
  }

  for (i = 0; i < psdheader->Rows; ++i) {
    unsigned char* p = pInfo->pp_line_addr[i];

    for (j = 0; j < psdheader->Columns; ++j) {
      *p++ = *pImageB++;
      *p++ = *pImageG++;
      *p++ = *pImageR++;

      if (psdheader->Depth == 16) {
        pImageR++;
        pImageG++;
        pImageB++;
      }
    }
  }

  return ER_SUCCESS;
}


/* 读CMYK图到数据包 */
static enum EXERESULT CALLAGREEMENT _read_cmyk(unsigned char* pbuf,
    LPINFOSTR pInfo,
    LPPSDFILEHEADER psdheader)
{
  unsigned int  i, j, imagesize;
  unsigned char* pImageC, *pImageM, *pImageY, *pImageK;

  assert(pInfo->bitcount == 24);
  assert(psdheader->Depth == 8 || psdheader->Depth == 16);
  assert(psdheader->Channels >= 4);

  imagesize = psdheader->Rows * psdheader->Columns;

  pImageC = pbuf;

  if (psdheader->Depth == 8) {
    pImageM = pImageC + imagesize;
    pImageY = pImageM + imagesize;
    pImageK = pImageY + imagesize;
  }
  else {
    pImageM = pImageC + imagesize * 2;
    pImageY = pImageM + imagesize * 2;
    pImageK = pImageY + imagesize * 2;
  }

  for (i = 0; i < psdheader->Rows; ++i) {
    unsigned char* p = pInfo->pp_line_addr[i];

    for (j = 0; j < psdheader->Columns; ++j) {
      unsigned long colorK;

      colorK = *pImageK;

      *p++ = (unsigned char)(colorK * (*pImageY) / 255);
      *p++ = (unsigned char)(colorK * (*pImageM) / 255);
      *p++ = (unsigned char)(colorK * (*pImageC) / 255);

      if (psdheader->Depth == 8) {
        pImageC++;
        pImageM++;
        pImageY++;
        pImageK++;
      }
      else {
        pImageC += 2;
        pImageM += 2;
        pImageY += 2;
        pImageK += 2;
      }
    }
  }

  return ER_SUCCESS;
}


/* 读Lab图到数据包 */
static enum EXERESULT CALLAGREEMENT _read_lab(unsigned char* pbuf,
    LPINFOSTR pInfo,
    LPPSDFILEHEADER psdheader)
{
  unsigned int  i, j, imagesize;
  unsigned char* pImageL, *pImageA, *pImageB;

  static const unsigned int MAXRGB = 256;
  static const int R = 256 * 2;
  static const int G = 256;
  static const int B = 0;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert((psdheader->Channels == 3) || (psdheader->Channels == 4));

  __try {
    /* 初始化xyz到RGB映射表 */
    lab_red_map   = (double*)isirw_malloc(3 * MAXRGB * sizeof(double));
    lab_green_map = (double*)isirw_malloc(3 * MAXRGB * sizeof(double));
    lab_blue_map  = (double*)isirw_malloc(3 * MAXRGB * sizeof(double));

    if (lab_red_map == 0 || lab_green_map == 0 || lab_blue_map == 0) {
      b_status = ER_MEMORYERR;
      __leave;
    }

    for (i = 0; i < MAXRGB; i++) {
      lab_red_map[i + R]   = 3.240479 * i;
      lab_green_map[i + R] = (-1.537150) * i;
      lab_blue_map[i + R]  = (-0.498535) * i;
      lab_red_map[i + G]   = (-0.969256) * i;
      lab_green_map[i + G] = 1.875992 * i;
      lab_blue_map[i + G]  = 0.041556 * i;
      lab_red_map[i + B]   = 0.055648 * i;
      lab_green_map[i + B] = (-0.204043) * i;
      lab_blue_map[i + B]  = 1.057311 * i;
    }

    imagesize = psdheader->Rows * psdheader->Columns;
    pImageL = pbuf;

    if (psdheader->Depth == 8) {
      pImageA = pImageL + imagesize;
      pImageB = pImageA + imagesize;
    }
    else {
      pImageA = pImageL + imagesize * 2;
      pImageB = pImageA + imagesize * 2;
    }

    for (i = 0; i < psdheader->Rows; ++i) {
      unsigned char* p = pInfo->pp_line_addr[i];

      for (j = 0; j < psdheader->Columns; ++j) {
        Lab2RGB(*pImageL++, *pImageA++, *pImageB++, p++, p++, p++);

        if (psdheader->Depth == 16) {
          pImageL ++;
          pImageA ++;
          pImageB ++;
        }
      }
    }
  }
  __finally {
    if (lab_red_map) {
      isirw_free(lab_red_map);
      lab_red_map = 0;
    }

    if (lab_green_map) {
      isirw_free(lab_green_map);
      lab_green_map = 0;
    }

    if (lab_blue_map) {
      isirw_free(lab_blue_map);
      lab_blue_map = 0;
    }
  }

  return b_status;
}


/* 多通道，输出近似图 */
static enum EXERESULT CALLAGREEMENT _read_nultichannel(unsigned char* pbuf, LPINFOSTR pInfo, LPPSDFILEHEADER psdheader)
{
  unsigned int i, j;
  unsigned char* lpChannel1, *lpChannel2, *lpChannel3, *p;
  unsigned int channelsize;

  assert(pInfo->bitcount == 24);
  assert(psdheader->Depth == 8);

  channelsize = psdheader->Columns * psdheader->Rows;

  lpChannel1 = pbuf;
  lpChannel2 = lpChannel1 + channelsize;
  lpChannel3 = lpChannel2 + channelsize;

  for (i = 0; i < psdheader->Rows; ++i) {
    p = pInfo->pp_line_addr[i];

    for (j = 0; j < psdheader->Columns; ++j) {
      if (psdheader->Channels > 2) {
        *p++ = *lpChannel3++;
      }
      else {
        *p++ = 0;
      }

      if (psdheader->Channels > 1) {
        *p++ = *lpChannel2++;
      }
      else {
        *p++ = 0;
      }

      if (psdheader->Channels > 0) {
        *p++ = *lpChannel1++;
      }
      else {
        *p++ = 0;
      }
    }
  }

  return ER_SUCCESS;
}


/* 读取一行RLE压缩数据 */
static int CALLAGREEMENT _read_line(ISFILE* pfile, long pos, unsigned short size, signed char* pdata)
{
  assert(pdata);

  if (pos >= 0) {
    if (isio_seek(pfile, pos, SEEK_SET) == -1) {
      return -1;    /* 读流错 */
    }
  }

  /* 读取RLE数据 */
  if (isio_read((void*)pdata, size, 1, pfile) == 0) {
    return -1;    /* 读流错 */
  }

  return 0;
}


/* 解压一行RLE数据 */
static int CALLAGREEMENT _decode_rle_line(signed char* psou, long ssize, unsigned char* pdes, long dsize)
{
  signed char   n;
  unsigned char b;
  int       mark = 0;

  assert(psou && pdes);

  while ((ssize > 0) && (dsize > 0)) {
    /* 提取一字节的源数据 */
    n = *psou++, ssize--;

    if (n < 0) {
      if (n == -128) {
        continue;
      }

      /* 计算出后续字节的重复次数 */
      n = -n + 1;

      /* 这的数据类型转换很棘手，稍有不慎就会掉到冰窟隆里！*/
      if (dsize < (long)(unsigned int)(unsigned char)n) {
        /* 如果目标缓冲区无法容纳 n 字节的数据，则抛弃一些数据。（容错处理） */
        n = (signed char)(unsigned char)dsize, dsize = 0, mark = 1;
      }
      else {
        dsize -= (long)(unsigned int)(unsigned char)n;
      }

      b = *psou++, ssize--;

      /* 写入重复数据 */
      while ((int)(unsigned int)(unsigned char)n-- > 0) {
        *pdes++ = b;
      }
    }
    else {
      if (dsize < ((long)(unsigned int)(unsigned char)n + 1)) {
        /* 如果目标缓冲区无法容纳解压数据，则抛弃部分数据 */
        n = (signed char)((unsigned char)dsize - 1), mark = 1;
      }

      /* 直接拷贝未经压缩的数据 */
      memcpy(pdes, psou, (size_t)(unsigned int)(unsigned char)(++n));

      pdes += (unsigned int)(unsigned char)n;
      dsize -= (unsigned int)(unsigned char)n;
      psou += (unsigned int)(unsigned char)n;
      ssize -= (unsigned int)(unsigned char)n;
    }
  }

  if (dsize > 0) {
    mark = 1;
  }

  /* 返回值：0－成功，1－源数据有错误，但已经过容错处理 */
  return mark;
}


/***********************************************************************************\
$                                                                                   $
$    函数：int _is_support_format( unsigned short, unsigned short ) ;               $
$          检查颜色模式和色深是否被支持                                             $
$                                                                                   $
$    参数：color_mode   颜色的模式                                                  $
$                 Bitmap=0, Grayscale=1, Indexed=2, RGB=3,                          $
$                 CMYK=4, Multichannel=7, Duotone=8, Lab=9                          $
$          color_pixel  颜色的位深                                                  $
$                 PSD 格式只有 1, 8, 16                                             $
$                                                                                   $
$    返回：int 型                                                                   $
$                 成功支持=0, 颜色模式不支持=1, 位深不支持=2                        $
$                                                                                   $
\***********************************************************************************/
int CALLAGREEMENT _is_support_format(unsigned short color_mode, unsigned short color_pixel)
{
  /* 下面是测试 color_mode ,color_pixel 的正确性 */
  switch (color_mode) {
  case PSD_BITMAP:
    if (color_pixel == 1) {
      break ;
    }

    return  2 ;

  case PSD_INDEXED:
    if (color_pixel == 8) {
      break;
    }

    return 2;

  case PSD_GRAYSCALE:
  case PSD_RGB:
  case PSD_CMYK:
  case PSD_DUOTONE:
  case PSD_LAB:
    if (color_pixel == 8 || color_pixel == 16) {
      break ;
    }

    return  2 ;

  case PSD_MULTICHANNEL:
    if (color_pixel == 8) {
      break ;
    }

    return  2 ;

  default:/* Unknow -> Error */
    return  1 ;
  }

  return 0 ;
}


/***********************************************************************************\
$                                                                                   $
$    函数：int _skip_block( ISFILE *pfile ) ;                                       $
$          跳过下一个块，并移动流的读写指针                                         $
$                                                                                   $
$    参数：pfile   图象流指针                                                       $
$                                                                                   $
$    返回：int 型                                                                   $
$                 成功=0, 读失败=-1                                                 $
$                                                                                   $
\***********************************************************************************/
int CALLAGREEMENT _skip_block(ISFILE* pfile)
{
  unsigned long temp_data;

  if (isio_read((void*)&temp_data, sizeof(unsigned long), 1, pfile) == 0) {
    return -1;
  }

  temp_data = EXCHANGE_DWORD(temp_data) ;

  if (temp_data) {
    if (isio_seek(pfile, temp_data, SEEK_CUR) == -1) {
      return -1;
    }
  }

  return 0;
}


/*
 *  Lab -> RGB 的测试函数，不太准确
 */
static void CALLAGREEMENT Lab2RGB(
    unsigned char bL, unsigned char ba, unsigned char bb,
    unsigned char* pR, unsigned char* pG, unsigned char* pB)
{
  double      L, a, b;
  double      X, Y, Z, T, YYn3;
  double      FinalR, FinalG, FinalB;

  static const int MAXRGB = 255;
  static const int R = 256 * 2;
  static const int G = 256;
  static const int B = 0;

  assert(lab_red_map && lab_green_map && lab_blue_map);

  L = bL / 2.55;
  a = ba - 128;
  b = bb - 128;

  /* Lab to XYZ */
  YYn3 = (L + 16) / 116;

  T = YYn3 + a / 500;
  X = T * T * T;
  Y = YYn3 * YYn3 * YYn3;
  T = YYn3 - b / 200;
  Z = T * T * T;

  X *= 235;
  Y *= 235;
  Z *= 235;

  FinalR = lab_red_map[(UINT)X + R] + lab_green_map[(UINT)Y + R] + lab_blue_map[(UINT)Z + R];
  FinalG = lab_red_map[(UINT)X + G] + lab_green_map[(UINT)Y + G] + lab_blue_map[(UINT)Z + G];
  FinalB = lab_red_map[(UINT)X + B] + lab_green_map[(UINT)Y + B] + lab_blue_map[(UINT)Z + B];

  FinalR = FinalR < 0 ? 0 : FinalR > MAXRGB ? MAXRGB : FinalR;
  FinalG = FinalG < 0 ? 0 : FinalG > MAXRGB ? MAXRGB : FinalG;
  FinalB = FinalB < 0 ? 0 : FinalB > MAXRGB ? MAXRGB : FinalB;

  *pR = (unsigned char)FinalR;
  *pG = (unsigned char)FinalG;
  *pB = (unsigned char)FinalB;
}



/* 读取图像位数据 */
PSD_API int CALLAGREEMENT psd_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  PSDFILEHEADER psd_header;

  unsigned short  compression;
  int       linesize, i;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&psd_load_img_critical);

      /* 调用系统回调函数 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 打开流 */
      if ((pfile = isio_open((const char*)psct, "rb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read((void*)&psd_header, sizeof(PSDFILEHEADER), 1, pfile) == 0) {
        b_status = (pinfo_str->data_state == 1) ? ER_FILERWERR : ER_NONIMAGE;
        __leave;
      }

      /* 判断文件头部的PSD文件标志('8BPS')和Version标志(1)
         PSD 的 Version 一定是 1，如果是其他的就错了 */
      if ((psd_header.Signature != PSD_SIGNATURE_MARKER) || \
          (psd_header.Version != PSD_VERSION_MARKER)) {
        b_status = ER_NONIMAGE;
        __leave;
      }

      psd_header.Channels = EXCHANGE_WORD(psd_header.Channels) ;
      psd_header.Rows   = EXCHANGE_DWORD(psd_header.Rows) ;
      psd_header.Columns  = EXCHANGE_DWORD(psd_header.Columns) ;
      psd_header.Depth  = EXCHANGE_WORD(psd_header.Depth) ;
      psd_header.Mode   = EXCHANGE_WORD(psd_header.Mode) ;

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        /* 通道数必须在 1 到 24 之间
           长宽的范围在 1 到 30000 之间 */
        if (psd_header.Channels < 1 || psd_header.Channels > 24 \
            || psd_header.Rows < 1 || psd_header.Rows > 30000 \
            || psd_header.Columns < 1 || psd_header.Columns > 30000) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        if (_is_support_format(psd_header.Mode, psd_header.Depth)) {
          /* 文件的格式不支持(也包括是PSD但不支持的文件) */
          b_status = ER_NONIMAGE;
          __leave;
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
        pinfo_str->imgformat  = IMF_PSD;      /* 图像文件格式（后缀名） */

        /* 填写图像信息 */
        pinfo_str->width  = psd_header.Columns ;
        pinfo_str->height = psd_header.Rows ;
        pinfo_str->order  = 0 ;

        /* 检查各色系的通道数是否被ISee插件接受 */
        switch (psd_header.Mode) {
        case PSD_BITMAP:  /* 这三种色系只允许有一个通道 */
        case PSD_GRAYSCALE:
        case PSD_INDEXED:
        case PSD_DUOTONE: /* 按灰度读取 */
          pinfo_str->bitcount = psd_header.Depth;
          break;

        case PSD_RGB: /* RGB色系通常为3通道，允许附加一个ALPHA通道 */
          if ((psd_header.Channels < 3)) {
            b_status = ER_BADIMAGE;
            __leave;
            break;
          }

          pinfo_str->bitcount = 24; /* 固定输出24位图 */
          break;

        case PSD_CMYK:  /* CMYK色系不允许再附加ALPHA通道 */
          if (psd_header.Channels < 4) {
            b_status = ER_BADIMAGE;
            __leave;
            break;
          }

          pinfo_str->bitcount = 24;
          break;

        case PSD_MULTICHANNEL:  /* 多通道图象 */
          pinfo_str->bitcount = 24;
          break;

        case PSD_LAB: /* Lab */
          if (psd_header.Channels < 3) {
            b_status = ER_BADIMAGE;
            __leave;
            break;
          }

          pinfo_str->bitcount = 24;
          break;

        default:/* Error */
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }


        /* 提取图像的压缩方式 */

        /* 跳过调色板段、图象资源段、图象层和蒙板段 */
        if (_skip_block(pfile) || _skip_block(pfile) || _skip_block(pfile)) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 提取压缩方法信息 */
        if (isio_read((void*)&compression, sizeof(unsigned short), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        compression = EXCHANGE_WORD(compression) ;

        switch (compression) {
        case  1:
          pinfo_str->compression = ICS_RLE8 ;
          break;

        case  0:

          /* 这里也有分 888 的RGB 和只有8位的灰度等 */
          if (psd_header.Depth == 1) {
            pinfo_str->compression = ICS_RGB;  /* 简单了一些，做不得准的 */
          }
          else {
            pinfo_str->compression = ICS_BITFIELDS ;  /* 分位的 */
          }

          break;

        default:
          b_status = ER_BADIMAGE;
          __leave;
          break;     /* 插件暂不支持ZIP压缩 */
        }


        /* 计算图像掩码数据 */
        switch (pinfo_str->bitcount) {
        case  1:
        case  8:
          pinfo_str->r_mask = 0;
          pinfo_str->g_mask = 0;
          pinfo_str->b_mask = 0;
          pinfo_str->a_mask = 0;
          break;

        case  16:
          pinfo_str->r_mask = 0x7C00;
          pinfo_str->g_mask = 0x3E0;
          pinfo_str->b_mask = 0x1F;
          pinfo_str->a_mask = 0;
          break;

        case  24:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0;
          break;

        default:
          assert(0);
          break;
        }

        pinfo_str->data_state = 1;
      }


      /* 取得扫描行尺寸 */
      linesize = DIBSCANLINE_WIDTHBYTES(pinfo_str->width * pinfo_str->bitcount);

      /* 设置图像个数 */
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg  = (LPSUBIMGBLOCK)0;

      assert(pinfo_str->p_bit_data == (unsigned char*)0);
      /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
      pinfo_str->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4);

      if (!pinfo_str->p_bit_data) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      assert(pinfo_str->pp_line_addr == (void**)0);
      /* 分配行首地址数组 */
      pinfo_str->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * pinfo_str->height);

      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* PSD图总是正向的 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
      }

      /* 回调通知 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 读取位数据到内存 */
      if ((b_status = read_psd(pfile, pinfo_str, &psd_header)) != ER_SUCCESS) {
        __leave;
      }

      /* 主图象进度结束 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      pinfo_str->data_state = 2;
    }
    __finally {
      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        if (pinfo_str->p_bit_data) {
          isirw_free(pinfo_str->p_bit_data);
          pinfo_str->p_bit_data = (unsigned char*)0;
        }

        if (pinfo_str->pp_line_addr) {
          isirw_free(pinfo_str->pp_line_addr);
          pinfo_str->pp_line_addr = (void**)0;
        }

        if (pinfo_str->data_state == 2) {
          pinfo_str->data_state = 1;  /* 自动降级 */
        }

        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
      }
      else {
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          break;

        case  2:
          b_status = ER_SYSERR;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }

        if (b_status != ER_SUCCESS) {
          if (pinfo_str->p_bit_data) {
            isirw_free(pinfo_str->p_bit_data);
            pinfo_str->p_bit_data = (unsigned char*)0;
          }

          if (pinfo_str->pp_line_addr) {
            isirw_free(pinfo_str->pp_line_addr);
            pinfo_str->pp_line_addr = (void**)0;
          }

          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1;
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&psd_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}

