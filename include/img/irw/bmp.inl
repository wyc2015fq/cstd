/********************************************************************
 bmp.c
 本文件用途： BMP图像读写模块实现文件
 读取功能：Windows 风格 －1～32位位图、RLE位图
 OS/2 风格 － 1、4、8、24位位图
 保存功能：Windows 风格 －1、4、8、16(555)、24、
 32(888)位位图
 本文件编写人： YZ
 TearShark
 梦晖
 本文件版本： 30618
 最后修改于： 2003-06-18
 ----------------------------------------------------------------
 修正历史：
 2003-6 修改RLE4图的位深为4位()
 2003-3 支持图象读写引擎提供的象素缓冲区功能
 2002-11 网友TearShark修正一处BUG（OS2风格位图的确认代码
 存在的错误）。
 2002-11 去除衍生扩展名"rle"，因这种扩展名极少被使用。
 2002-8 插件接口升级至2.2。
 引入内存防护概念。
 2001-7 修正RLE解码中的一处错误。
 1、4位图像输出符合ISee位流格式
 增加目标图像缓冲区附加字节
 增强了容错性
 2001-6 发布新版代码
 2001-5 重新定义模块的接口及输出功能
 2000-10 修正了一个较为重大的BUG
 2000-7 第一个测试版发布
********************************************************************/
/* 文件判别标志 (windows - 'BM', OS/2 - 'BA')*/
#define DIB_HEADER_MARKER ((ushort)('M'<<8)|'B')
#define DIB_HEADER_MARKER_OS2 ((ushort)('A'<<8)|'B')
/* 位图调色板项结构 （Windows 风格）*/
typedef struct _tag_rgb_quad {
  uchar rgbBlue;
  uchar rgbGreen;
  uchar rgbRed;
  uchar rgbReserved;
} RGB_QUAD, *LPRGB_QUAD;
/* 位图调色板项结构 （OS/2 风格）*/
typedef struct _tag_rgb_triple {
  uchar rgbtBlue;
  uchar rgbtGreen;
  uchar rgbtRed;
} RGB_TRIPLE, *LPRGB_TRIPLE;
/* 位图信息头结构 （Windows 风格）*/
typedef struct _tag_bmp_info_header {
  ulong biSize;
  long biWidth;
  long biHeight;
  ushort biPlanes;
  ushort biBitCount;
  ulong biCompression;
  ulong biSizeImage;
  long biXPelsPerMeter;
  long biYPelsPerMeter;
  ulong biClrUsed;
  ulong biClrImportant;
} BMP_INFO_HEADER, *LPBMP_INFO_HEADER;
/* 位图信息头结构 （OS/2 风格）*/
typedef struct _tag_bmp_core_header {
  ulong bcSize;
  ushort bcWidth;
  ushort bcHeight;
  ushort bcPlanes;
  ushort bcBitCount;
} BMP_CORE_HEADER, *LPBMP_CORE_HEADER;
/* 位图信息结构 （Windows 风格） */
typedef struct _tag_bmp_info {
  BMP_INFO_HEADER bmiHeader;
  RGB_QUAD bmiColors[1];
} BMP_INFO, *LPBMP_INFO;
/* 位图信息结构 （OS/2 风格） */
typedef struct _tag_bmp_core {
  BMP_CORE_HEADER bmciHeader;
  RGB_TRIPLE bmciColors[1];
} BMP_CORE, *LPBMP_CORE;
/* 位图文件头结构 （Windows 、OS/2 公用）*/
typedef struct _tag_bmp_file_header {
  ushort bfType;
  ulong bfSize;
  ushort bfReserved1;
  ushort bfReserved2;
  ulong bfOffBits;
} BMP_FILE_HEADER, *LPBMP_FILE_HEADER;
/*****************************************************************************/
/* 内部辅助函数 */
/* 获取各颜色分量的值 */
int _bmp_get_mask(LPBMP_INFO pInfo, ulong* pb_mask, ulong* pg_mask, ulong* pr_mask, ulong* pa_mask)
{
  int result = 0;
  ulong* pMark;
  assert(pInfo->bmiHeader.biSize);
  switch (pInfo->bmiHeader.biBitCount) {
  case 1:
  case 4:
  case 8:
    /* 调色板位图无掩码 */
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;
  case 16:
    /*******************************************
    16位掩码示意图
    高字节 低字节
    0000 0000 0000 0000
    1 1111--B 555格式
    11 111 -------G
    111 11 --------------R
    0
    1 1111--B 565格式
    111 111 -------G
    1111 1 --------------R
    * Win95 系统只支持以上两种格式（NT支持多种格式）
    *******************************************/
    if (pInfo->bmiHeader.biCompression == 0) {
      *pr_mask = 0x7c00;
      *pg_mask = 0x3e0;
      *pb_mask = 0x1f;
      *pa_mask = 0;
      break;
    }
    else {
      pMark = (ulong*)((LPBMP_INFO_HEADER)pInfo + 1);
      *pr_mask = pMark[0];
      *pg_mask = pMark[1];
      *pb_mask = pMark[2];
      *pa_mask = 0;
      break;
    }
    break;
  case 24:
    *pr_mask = 0xff0000;
    *pg_mask = 0xff00;
    *pb_mask = 0xff;
    *pa_mask = 0x0;
    break;
  case 32:
    /******************************************************
    32位掩码示意图
    高 -> 低
    0000 0000 0000 0000 0000 0000 0000 0000 888格式
    1111 1111 ------------------------R
    1111 1111 -------------G
    1111 1111--B
    * Win95 系统只支持这一种格式
    ******************************************************/
    if (pInfo->bmiHeader.biCompression == 0) {
      *pr_mask = 0xff0000;
      *pg_mask = 0xff00;
      *pb_mask = 0xff;
      *pa_mask = 0x0;
    }
    else {
      pMark = (ulong*)((LPBMP_INFO_HEADER)pInfo + 1);
      *pr_mask = pMark[0];
      *pg_mask = pMark[1];
      *pb_mask = pMark[2];
      *pa_mask = 0;
    }
    break;
  default:
    result = 1;
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;
  }
  return result; /* 0-成功， 1－非法的图像格式 */
}
/* 向4位缓冲区写像素
 形参mark含意：0-初始化变量 1-写像素 2-移动到指定点 3-结束操作 4-获取当前地址
 注：当mark等于2时，参数num将作为像素除2余数被代入，来对iCurOff赋值
*/
uchar* _write2buff4RLE(int mark, int num, uchar pix, uchar* lpDesData)
{
  static uchar* lpDes; /* 缓冲区基地址 */
  static uchar* lpCur; /* 缓冲区当前写地址 */
  static int iCurOff; /* 在字节中的当前位置，0-高半字节 1-低半字节 */
  int i;
  uchar pixa[2];
  switch (mark) {
  case 0: /* 初始化变量 */
    lpDes = lpDesData;
    lpCur = lpDes;
    iCurOff = 0;
    break;
  case 1: /* 写像素 */
    pixa[0] = pix >> 4;
    pixa[1] = pix & 0xf;
    for (i = 0; i < num; i++) {
      if (iCurOff == 0) { /* 左像素 */
        *lpCur = pixa[i % 2] << 4;
        iCurOff = 1;
      }
      else { /* 右像素 */
        assert(iCurOff == 1);
        *lpCur |= pixa[i % 2];
        lpCur++;
        iCurOff = 0;
      }
    }
    break;
  case 2: /* 移动到指定点 */
    lpCur = lpDesData;
    iCurOff = num;
    break;
  case 3: /* 结束操作 */
    lpDes = (uchar*)0;
    lpCur = lpDes;
    iCurOff = 0;
    break;
  case 4: /* 获取当前地址 */
    break;
  default:
    assert(0);
    break;
  }
  return lpCur;
}
/* 向8位缓冲区写像素（只供_from_8_RLE_read()函数内部调用）
 形参mark含意：0-初始化变量 1-写像素 2-移动到指定点 3-结束操作 4-获取当前地址
*/
uchar* _write2buff8RLE(int mark, int num, uchar pix, uchar* lpDesData)
{
  static uchar* lpDes; /* 缓冲区基地址 */
  static uchar* lpCur; /* 缓冲区当前写地址 */
  int i;
  switch (mark) {
  case 0: /* 初始化变量 */
    lpDes = lpDesData;
    lpCur = lpDes;
    break;
  case 1: /* 写像素 */
    for (i = 0; i < num; i++) {
      *lpCur++ = pix;
    }
    break;
  case 2: /* 移动到指定点 */
    lpCur = lpDesData;
    break;
  case 3: /* 结束操作 */
    lpDes = (uchar*)0;
    lpCur = lpDes;
    break;
  case 4: /* 获取当前地址 */
    break;
  default:
    assert(0);
    break;
  }
  return lpCur;
}
/* 判断传入的图像是否可以被保存 */
int _bmp_is_valid_img(INFOSTR* pinfo_str)
{
  enum { bitcount = 1UL | (1UL<<(4-1)) | (1UL<<(8-1)) | (1UL<<(15-1)) | (1UL<<(16-1)) | (1UL<<(24-1)) | (1UL<<(32-1)) };
  /* ## 2001-8-9 YZ 修改：位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* 不支持的位深图像 */
  }
  assert(pinfo_str->imgnumbers);
  if (pinfo_str->imgnumbers != 1) {
    return -2; /* 图像个数不正确 */
  }
  return 0;
}
/* 获取图像信息 */
EXERESULT bmp_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  BMP_FILE_HEADER bfh;
  LPBMP_INFO pdibinfo = (LPBMP_INFO)0;
  ulong dib_info_len = 0;
  int style; /* 位图风格，0－WINDOWS，1－OS/2 */
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
  do {
    /* 读取文件头结构 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    if (isio_read((void*)&bfh, sizeof(BMP_FILE_HEADER), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 判断是否是有效的位图文件 */
    if ((bfh.bfType != DIB_HEADER_MARKER) && (bfh.bfType != DIB_HEADER_MARKER_OS2)) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* 信息头＋颜色表（包括OS/2风格） */
    dib_info_len = bfh.bfOffBits - sizeof(BMP_FILE_HEADER);
    if ((pdibinfo = (LPBMP_INFO)isirw_malloc(dib_info_len)) == (LPBMP_INFO)0) {
      b_status = ER_MEMORYERR;
      break;
    }
    if (isio_seek(pfile, sizeof(BMP_FILE_HEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 读取图象信息结构 */
    if (isio_read((void*)pdibinfo, dib_info_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 确定是windows风格位图还是OS/2风格位图 */
    if (pdibinfo->bmiHeader.biSize == sizeof(BMP_CORE_HEADER)) {
      style = 1; /* OS/2 */
    }
    else {
      style = 0; /* windows */
    }
    /* 数据有效性检验 */
    if (style == 0) {
      if ((pdibinfo->bmiHeader.biHeight == 0) || (pdibinfo->bmiHeader.biWidth == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
    }
    else {
      assert(style == 1);
      if ((((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight == 0) || (((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
    }
    pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
    pinfo_str->imgformat = IMF_BMP; /* 图像文件格式（后缀名） */
    if (style == 0) {
      switch (pdibinfo->bmiHeader.biCompression) { /* 图像的压缩方式 */
      case 0:
        pinfo_str->compression = ICS_RGB;
        break;
      case 1:
        pinfo_str->compression = ICS_RLE8;
        break;
      case 2:
        pinfo_str->compression = ICS_RLE4;
        break;
      case 3:
        pinfo_str->compression = ICS_BITFIELDS;
        break;
      default:
        b_status = ER_NONIMAGE;
        break;
      }
    }
    else {
      pinfo_str->compression = ICS_RGB; /* OS/2 无压缩位图格式 */
    }
    /* 填写图像信息 */
    if (style == 0) {
      pinfo_str->width = pdibinfo->bmiHeader.biWidth;
      pinfo_str->height = (ulong)abs(pdibinfo->bmiHeader.biHeight);
      pinfo_str->order = (pdibinfo->bmiHeader.biHeight < 0) ? 0 : 1;
      /* 有些单色RLE4的图bitcount被设置成1，如Windows 3.0的logo，所以做一下容错处理 */
      if (pinfo_str->compression == ICS_RLE4) {
        pinfo_str->bitcount = 4;
      }
      else if (pinfo_str->compression == ICS_RLE8) {
        pinfo_str->bitcount = 8;
      }
      else {
        pinfo_str->bitcount = pdibinfo->bmiHeader.biBitCount;
      }
      if (_bmp_get_mask(pdibinfo,
          (&pinfo_str->b_mask),
          (&pinfo_str->g_mask),
          (&pinfo_str->r_mask),
          (&pinfo_str->a_mask)) == 1) {
        b_status = ER_NONIMAGE;
        break;
      }
    }
    else {
      pinfo_str->width = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth);
      pinfo_str->height = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight);
      pinfo_str->order = 1; /* OS/2位图都是倒向的 */
      pinfo_str->bitcount = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcBitCount);
      /* OS/2 位图的有效位深只有：1、4、8、24 */
      if (pinfo_str->bitcount == 24) {
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
      }
      else {
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
      }
    }
    /* 设定数据包状态 */
    pinfo_str->data_state = 1;
  }
  while (0);
  if (pdibinfo) {
    isirw_free(pdibinfo);
  }
  return b_status;
}
/* 读取图像位数据 */
EXERESULT bmp_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  BMP_FILE_HEADER bfh;
  LPBMP_INFO pdibinfo = (LPBMP_INFO)0;
  ulong dib_info_len; /* DIB信息结构尺寸 */
  ulong dib_data_len; /* DIB位数据尺寸 */
  unsigned int pal_num; /* 调?逑罡鍪?*/
  int i, linesize;
  int style; /* 位图风格，0－WINDOWS，1－OS/2 */
  int bEOF = 0; /* 图像结尾标志 */
  int line = 0; /* 当前行 */
  uchar pix, pix2; /* 当前值 */
  uchar* pbit = (uchar*)0;
  uchar* prle = (uchar*)0;
  uchar* prlesou = (uchar*)0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  do {
    /* 读文件头结构 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    if (isio_read((void*)&bfh, sizeof(BMP_FILE_HEADER), 1, pfile) == 0) {
      b_status = (pinfo_str->data_state == 1) ? ER_FILERWERR : ER_NONIMAGE;
      break;
    }
    /* 判断是否是有效的位图文件 */
    if ((bfh.bfType != DIB_HEADER_MARKER) && (bfh.bfType != DIB_HEADER_MARKER_OS2)) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* 计算 BMP_INFO 的尺寸 */
    dib_info_len = bfh.bfOffBits - sizeof(BMP_FILE_HEADER);
    /* 分配存放 BMP_INFO 信息的内存块 */
    if ((pdibinfo = (LPBMP_INFO)isirw_malloc(dib_info_len)) == (LPBMP_INFO)0) {
      b_status = ER_MEMORYERR;
      break;
    }
    /* 读信息头结构 */
    if (isio_seek(pfile, sizeof(BMP_FILE_HEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    if (isio_read((void*)pdibinfo, dib_info_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 确定是windows风格位图还是OS/2风格位图 */
    if (pdibinfo->bmiHeader.biSize == sizeof(BMP_CORE_HEADER)) {
      style = 1; /* OS/2 */
    }
    else {
      style = 0; /* windows */
    }
    /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
    if (pinfo_str->data_state == 0) {
      /* 数据有效性检验 */
      if (style == 0) {
        if ((pdibinfo->bmiHeader.biHeight == 0) || (pdibinfo->bmiHeader.biWidth == 0)) {
          b_status = ER_NONIMAGE;
        }
      }
      else {
        assert(style == 1);
        if ((((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight == 0) || (((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth == 0)) {
          b_status = ER_NONIMAGE;
        }
      }
      __LEAVE;
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_BMP; /* 图像文件格式（后缀名） */
      if (style == 0) {
        switch (pdibinfo->bmiHeader.biCompression) { /* 图像的压缩方式 */
        case 0:
          pinfo_str->compression = ICS_RGB;
          break;
        case 1:
          pinfo_str->compression = ICS_RLE8;
          break;
        case 2:
          pinfo_str->compression = ICS_RLE4;
          break;
        case 3:
          pinfo_str->compression = ICS_BITFIELDS;
          break;
        default:
          b_status = ER_NONIMAGE;
          break;
        }
      }
      else {
        pinfo_str->compression = ICS_RGB;
      }
      if (style == 0) {
        pinfo_str->width = pdibinfo->bmiHeader.biWidth;
        pinfo_str->height = (ulong)abs(pdibinfo->bmiHeader.biHeight);
        pinfo_str->order = (pdibinfo->bmiHeader.biHeight < 0) ? 0 : 1;
        if (pinfo_str->compression == ICS_RLE4) {
          pinfo_str->bitcount = 4;
        }
        else if (pinfo_str->compression == ICS_RLE8) {
          pinfo_str->bitcount = 8;
        }
        else {
          pinfo_str->bitcount = pdibinfo->bmiHeader.biBitCount;
        }
        if (_bmp_get_mask(pdibinfo,
            (&pinfo_str->b_mask),
            (&pinfo_str->g_mask),
            (&pinfo_str->r_mask),
            (&pinfo_str->a_mask)) == 1) {
          b_status = ER_NONIMAGE;
        }
        __LEAVE;
      }
      else {
        pinfo_str->width = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcWidth);
        pinfo_str->height = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcHeight);
        pinfo_str->order = 1; /* OS/2位图都是倒向的 */
        pinfo_str->bitcount = (ulong)(((LPBMP_CORE)pdibinfo)->bmciHeader.bcBitCount);
        /* OS/2 位图的有效位深只有：1、4、8、24 */
        if (pinfo_str->bitcount == 24) {
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else {
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
        }
      }
      pinfo_str->data_state = 1;
    }
    /* 设置调色板数据 */
    if (pinfo_str->bitcount <= 8) {
      pinfo_str->pal_count = 1UL << pinfo_str->bitcount;
      if (style == 0) {
        pal_num = (pdibinfo->bmiHeader.biClrUsed == 0) ? pinfo_str->pal_count : pdibinfo->bmiHeader.biClrUsed;
        if (pal_num > pinfo_str->pal_count) {
          b_status = ER_NONIMAGE;
          break;
        }
        memmove((void*)(pinfo_str->palette), (const void*)(pdibinfo->bmiColors), sizeof(RGB_QUAD)*pal_num);
      }
      else {
        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtBlue = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtBlue;
          ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtGreen = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtGreen;
          ((LPRGB_TRIPLE)(&pinfo_str->palette[i]))->rgbtRed = ((LPBMP_CORE)pdibinfo)->bmciColors[i].rgbtRed;
          pinfo_str->palette[i] &= 0xffffff; /* 最高字节置零 */
        }
      }
    }
    else {
      pinfo_str->pal_count = 0;
    }
    /* 取得位数据长度 */
    dib_data_len = isio_length(pfile) - bfh.bfOffBits;
    /* 取得扫描行尺寸 */
    linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
    /* 设置图像个数 */
    pinfo_str->imgnumbers = 1;
    pinfo_str->psubimg = (SUBIMGBLOCK*)0;
    assert(pinfo_str->p_bit_data == (uchar*)0);
    /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
    pinfo_str->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo_str->height + 4);
    if (!pinfo_str->p_bit_data) {
      b_status = ER_MEMORYERR;
      break;
    }
    assert(pinfo_str->pp_line_addr == NULL);
    /* 分配行首地址数组 */
    pinfo_str->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo_str->height);
    if (!pinfo_str->pp_line_addr) {
      b_status = ER_MEMORYERR;
      break;
    }
    __LEAVE;
    /* 是否是RLE */
    if ((pinfo_str->compression == ICS_RLE4) || (pinfo_str->compression == ICS_RLE8)) {
      assert(style == 0);
      /* 分配内存，用于存放RLE原始数据 */
      prlesou = prle = (uchar*)isirw_malloc(dib_data_len);
      if (!prle) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 读取RLE原始数据 */
      if (isio_seek(pfile, bfh.bfOffBits, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)prle, dib_data_len, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 为RLE解压做准备（暂时作为写指针数组） */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      if (pinfo_str->compression == ICS_RLE4) {
        _write2buff4RLE(0, 0, 0, (uchar*)pinfo_str->pp_line_addr[line]); /* 初始化写函数 */
        /* 先将RLE的?萁庋钩杀曜嫉腂MP数据 */
        while (!bEOF) {
          pix = *prle++;
          if (pix == 0) { /* 绝对方式 */
            pix2 = *prle++;
            switch (pix2) {
            case 0: /* 行结尾 */
              line++;
              _write2buff4RLE(2, 0, 0, (uchar*)pinfo_str->pp_line_addr[line]); /* 指定新的偏移地址 */
              break;
            case 1: /* 图像结尾 */
              bEOF = 1;
              break;
            case 2: /* 当前地址偏移 */
              pix = *prle++;
              pix2 = *prle++;
              line += pix2; /* 下移pix2行 */
              _write2buff4RLE(2, pix % 2, 0, (uchar*)((uchar*)pinfo_str->pp_line_addr[line] + (ulong)pix / 2));
              break;
            default: /* 不能压缩的像素 */
              for (i = 0; i < pix2 / 2; i++) {
                pix = *(prle + i);
                _write2buff4RLE(1, 2, pix, (uchar*)0);
              }
              if (pix2 % 2) {
                /* 补充遗漏的单个像素 */
                pix = *(prle + i);
                _write2buff4RLE(1, 1, pix, (uchar*)0);
              }
              /* (pix2+1)/2是将像素对齐到字节边界上，再加1、除2是对齐到字边界上 */
              prle = (uchar*)(prle + ((pix2 + 1) / 2 + 1) / 2 * 2); /* 字对齐 */
              break;
            }
          }
          else { /* 编码方式 */
            /* pix == 相同像素的个数 */
            pix2 = *prle++; /* pix2 == 像素 */
            _write2buff4RLE(1, (int)pix, pix2, (uchar*)0);
          }
        }
        _write2buff4RLE(3, 0, 0, (uchar*)0); /* 结束写函数 */
      }
      else {
        _write2buff8RLE(0, 0, 0, (uchar*)pinfo_str->pp_line_addr[line]); /* 初始化写函数 */
        while (!bEOF) {
          pix = *prle++;
          if (pix == 0) { /* 绝对方式 */
            pix2 = *prle++;
            switch (pix2) {
            case 0: /* 行结尾 */
              line++;
              _write2buff8RLE(2, 0, 0, (uchar*)pinfo_str->pp_line_addr[line]); /* 指定新的偏移地址 */
              break;
            case 1: /* 图像结尾 */
              bEOF = 1;
              break;
            case 2: /* 当前地址偏移 */
              pix = *prle++;
              pix2 = *prle++;
              line += pix2; /* 下移pix2行 */
              _write2buff8RLE(2, 0, 0, (uchar*)((uchar*)pinfo_str->pp_line_addr[line] + (ulong)pix));
              break;
            default: /* 不能压缩的像素 */
              for (i = 0; i < pix2; i++) {
                pix = *(prle + i);
                _write2buff8RLE(1, 1, pix, (uchar*)0);
              }
              /* (pix2+1)/2是将像素对齐到字节边界上，再加1、除2是对齐到字边界上 */
              prle = (uchar*)(prle + ((pix2 + 1) / 2) * 2); /* 字对齐 */
              break;
            }
          }
          else { /* 编码方式 */
            /* pix == 相同像素的个数 */
            pix2 = *prle++; /* pix2 == 像素 */
            _write2buff8RLE(1, (int)pix, pix2, (uchar*)0);
          }
        }
        _write2buff8RLE(3, 0, 0, (uchar*)0); /* 结束写函数 */
      }
      /* 初始化行首地址（RLE位图都是倒向的） */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }
    }
    else {
      if (pinfo_str->order == 0) { /* 正向 */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
        }
      }
      else { /* 倒向 */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
        }
      }
      /* 读取位数据到内存 */
      if (isio_seek(pfile, bfh.bfOffBits, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      pbit = pinfo_str->p_bit_data;
      /* 逐行读入 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        if (isio_read((void*)pbit, linesize, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        pbit = (uchar*)(pbit + linesize);
      }
      __LEAVE;
    }
    pinfo_str->data_state = 2;
  }
  while (0);
  if (prlesou) {
    isirw_free(prlesou);
  }
  if (pdibinfo) {
    isirw_free(pdibinfo);
  }
  if (b_status != ER_SUCCESS) {
    if (pinfo_str->p_bit_data) {
      isirw_free(pinfo_str->p_bit_data);
      pinfo_str->p_bit_data = (uchar*)0;
    }
    if (pinfo_str->pp_line_addr) {
      isirw_free(pinfo_str->pp_line_addr);
      pinfo_str->pp_line_addr = (uchar**)0;
    }
    if (pinfo_str->data_state == 2) {
      pinfo_str->data_state = 1; /* 自动降级 */
    }
  }
  else {
    if (b_status != ER_SUCCESS) {
      if (pinfo_str->p_bit_data) {
        isirw_free(pinfo_str->p_bit_data);
        pinfo_str->p_bit_data = (uchar*)0;
      }
      if (pinfo_str->pp_line_addr) {
        isirw_free(pinfo_str->pp_line_addr);
        pinfo_str->pp_line_addr = (uchar**)0;
      }
      if (pinfo_str->data_state == 2) {
        pinfo_str->data_state = 1;
      }
    }
  }
  return b_status;
}
/* 保存图像 （只能保存Windows风格的位图）*/
EXERESULT bmp_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  BMP_FILE_HEADER bfh;
  LPBMP_INFO pdibinfo = (LPBMP_INFO)0;
  ulong dib_info_len; /* DIB信息结构尺寸 */
  uchar* plinebuf = (uchar*)0;
  int i, linesize;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  do {
    /* 判断是否是受支持的图像数据 */
    if (_bmp_is_valid_img(pinfo_str) != 0) {
      b_status = ER_NSIMGFOR;
      break;
    }
    /* 计算位图信息头结构的尺寸（包括调色板） */
    switch (pinfo_str->bitcount) {
    case 1:
    case 4:
    case 8:
      dib_info_len = sizeof(BMP_INFO_HEADER) + pinfo_str->pal_count * sizeof(RGB_QUAD);
      break;
    case 24:
      dib_info_len = sizeof(BMP_INFO_HEADER);
      break;
    case 16:
    case 32:
      dib_info_len = sizeof(BMP_INFO_HEADER) + 3 * sizeof(ulong);
      break;
    default:
      assert(0);
      b_status = ER_SYSERR;
      break;
    }
    /* 分配存放 BMP_INFO 信息的内存块 */
    if ((pdibinfo = (LPBMP_INFO)isirw_malloc(dib_info_len)) == (LPBMP_INFO)0) {
      b_status = ER_MEMORYERR;
      break;
    }
    pdibinfo->bmiHeader.biSize = sizeof(BMP_INFO_HEADER);
    pdibinfo->bmiHeader.biPlanes = 1;
    pdibinfo->bmiHeader.biBitCount = (ushort)pinfo_str->bitcount;
    pdibinfo->bmiHeader.biCompression = ((pinfo_str->bitcount == 16) || (pinfo_str->bitcount == 32)) ? 3 : 0;
    pdibinfo->bmiHeader.biWidth = pinfo_str->width;
    /* 全部保存为倒向图 */
    pdibinfo->bmiHeader.biHeight = (long)pinfo_str->height;
    pdibinfo->bmiHeader.biSizeImage = 0;
    pdibinfo->bmiHeader.biClrImportant = 0;
    pdibinfo->bmiHeader.biClrUsed = 0;
    pdibinfo->bmiHeader.biXPelsPerMeter = 0;
    pdibinfo->bmiHeader.biYPelsPerMeter = 0;
    if (pdibinfo->bmiHeader.biCompression == 0) {
      memmove((void*)pdibinfo->bmiColors, (const void*)pinfo_str->palette, pinfo_str->pal_count * sizeof(RGB_QUAD));
    }
    else {
      *((ulong*) & (pdibinfo->bmiColors[0])) = pinfo_str->r_mask;
      *((ulong*) & (pdibinfo->bmiColors[1])) = pinfo_str->g_mask;
      *((ulong*) & (pdibinfo->bmiColors[2])) = pinfo_str->b_mask;
    }
    /* 填写位图文件头结构 */
    bfh.bfSize = sizeof(BMP_FILE_HEADER);
    bfh.bfType = DIB_HEADER_MARKER;
    bfh.bfOffBits = bfh.bfSize + dib_info_len;
    bfh.bfReserved1 = bfh.bfReserved2 = 0;
    /* 取得扫描行尺寸 */
    linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
    /* 申请写数据缓冲区 */
    if ((plinebuf = (uchar*)isirw_malloc(linesize)) == 0) {
      b_status = ER_MEMORYERR;
      break;
    }
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 写入文件头结构 */
    if (isio_write((const void*)&bfh, sizeof(BMP_FILE_HEADER), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 写入信息头结构 */
    if (isio_write((const void*)pdibinfo, dib_info_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 写入图像位数据
    注：传入的图像数据必须是ISee位流格式，否则可能会产生失真
    */
    for (i = 0; i < (int)(pinfo_str->height); i++) {
      memcpy((void*)plinebuf, (const void*)(uchar*) \
          (pinfo_str->pp_line_addr[pinfo_str->height - 1 - i]), linesize);
      /* 一律保存为倒向图 */
      if (isio_write((const void*)plinebuf, linesize, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
    }
  }
  while (0);
  if (plinebuf) {
    isirw_free(plinebuf);
  }
  if (pdibinfo) {
    isirw_free(pdibinfo);
  }
  return b_status;
}

