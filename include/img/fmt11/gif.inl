// GIF图像读写模块实现文件
// 读取功能：1~8位、静态或动态GIF图像
// 保存功能：暂不提供保存功能

/* GIF版本标识串长度 */
#define GIFVERSIZE          6

struct _tagGIFHEADER;
typedef struct _tagGIFHEADER    GIFHEADER;
typedef struct _tagGIFHEADER*    LPGIFHEADER;

struct _tagGIFINFO;
typedef struct _tagGIFINFO      GIFINFO;
typedef struct _tagGIFINFO*      LPGIFINFO;

struct _tagGIFRGB;
typedef struct _tagGIFRGB     GIFRGB;
typedef struct _tagGIFRGB*     LPGIFRGB;

struct _tagISEERGB;
typedef struct _tagISEERGB      ISEERGB;
typedef struct _tagISEERGB*      LPISEERGB;

struct _tagIMAGEDATAINFO;
typedef struct _tagIMAGEDATAINFO  IMAGEDATAINFO;
typedef struct _tagIMAGEDATAINFO*  LPIMAGEDATAINFO;

struct _tagGIFINSIDEINFO;
typedef struct _tagGIFINSIDEINFO  GIFINSIDEINFO;
typedef struct _tagGIFINSIDEINFO*  LPGIFINSIDEINFO;

struct _tagLZWTABLE;
typedef struct _tagLZWTABLE     LZWTABLE;
typedef struct _tagLZWTABLE*     LPLZWTABLE;

union _tagCODEWORD;
typedef union _tagCODEWORD      CODEWORD;
typedef union _tagCODEWORD*      LPCODEWORD;

struct _tagGRAPHCTRL;
typedef struct _tagGRAPHCTRL    GRAPHCTRL;
typedef struct _tagGRAPHCTRL*    LPGRAPHCTRL;

struct _tagNOTEHCTRL;
typedef struct _tagNOTEHCTRL    NOTEHCTRL;
typedef struct _tagNOTEHCTRL*    LPNOTEHCTRL;

struct _tagTEXTCTRL;
typedef struct _tagTEXTCTRL     TEXTCTRL;
typedef struct _tagTEXTCTRL*     LPTEXTCTRL;

struct _tagAPPCTRL;
typedef struct _tagAPPCTRL      APPCTRL;
typedef struct _tagAPPCTRL*      LPAPPCTRL;


/*@@@@@@@@@@@@@@@GIF87a 和 GIF89a 共用的结构@@@@@@@@@@@@@@@@@@*/

struct _tagGIFINFO {
  unsigned short FileType;    /* 89a or 87a */
  unsigned short ColorNum;    /* 颜色数 */
  unsigned short BitCount;    /* 图像的位深度 */
  unsigned short Width;
  unsigned short Height;
  unsigned short FrameCount;    /* 共有几幅图 */
  unsigned char  bkindex;     /* 背景色索引 */
  unsigned char  InitPixelBits; /* 压缩数据起始数据Bits位数 */
};


/* GIF调色板元素结构 */
struct _tagGIFRGB {
  unsigned char bRed;
  unsigned char bGreen;
  unsigned char bBlue;
};

/* ISee调色板元素结构 */
struct _tagISEERGB {
  unsigned char bBlue;
  unsigned char bGreen;
  unsigned char bRed;
  unsigned char rev;
};

/* GIF图像头结构 */
struct _tagGIFHEADER {
  unsigned char gif_type[GIFVERSIZE]; /* 图像版本(87a or 89a) */
  unsigned short  scr_width;        /* 逻辑屏宽度 */
  unsigned short  scr_height;       /* 逻辑屏高度 */
  unsigned char global_flag;      /* 全局标记 */
  unsigned char bg_color;       /* 背景色索引 */
  unsigned char aspect_radio;     /* 89a版的长宽比 */
};


/* 图像描述符 */
struct _tagIMAGEDATAINFO {
  unsigned char label;          /* 标签值：0x2c */
  unsigned short  left;           /* 子图像左上角在逻辑屏幕中的位置 */
  unsigned short  top;
  unsigned short  width;
  unsigned short  height;
  unsigned char local_flag;       /* 本地位域 */
};

#define GIF_MAXCOLOR  256

/* GIF综合信息结构 */
struct _tagGIFINSIDEINFO {
  int       type;         /* GIF版本：87a = 2 89a = 1 */

  unsigned long scr_width;        /* 逻辑屏信息 */
  unsigned long scr_height;
  unsigned long scr_bitcount;
  GIFRGB      p_pal[GIF_MAXCOLOR];  /* 全局调色板数据 */

  unsigned long first_img_width;    /* 首幅图像信息 */
  unsigned long first_img_height;
  unsigned long first_img_bitcount;
  GIFRGB      p_first_pal[GIF_MAXCOLOR];

  int       img_count;        /* 共有几幅图 */
  int       bk_index;       /* 背景色索引 */
};


#define LZWTABLESIZE  4096  /* GIF-LZW 串表尺寸 */

/*  GIF-LZW 字典单元结构 */
struct _tagLZWTABLE {
  short prefix;     /* 前缀码索引（无前缀码时为-1） */
  short code;     /* 尾码 */
  int   used;     /* 是否被占用标志：0－未占用，1－已占用 */
};


/* GIF-LZW编码单元 */
union _tagCODEWORD {
  unsigned char bcode[sizeof(unsigned long)];
  unsigned long lcode;
};


/*--------------------------------------------------------
  注：以下结构只适用于GIF89a格式的图像文件
--------------------------------------------------------*/

/* 图形控制扩展块 */
struct _tagGRAPHCTRL {
  unsigned char extintr;      /* 0x21 */
  unsigned char label;        /* 0xF9 */
  unsigned char block_size;     /* 0x04 */
  unsigned char field;
  unsigned short  delay_time;     /* 延迟时间 */
  unsigned char tran_color_index; /* 透明色索引 */
  unsigned char block_terminator; /* 0x00 */
};

/* 注释扩展块 */
struct _tagNOTEHCTRL {
  unsigned char extintr;        /* 0x21 */
  unsigned char label;        /* 0xfe */
};

/* 文本扩展块 */
struct _tagTEXTCTRL {
  unsigned char extintr;      /* 0x21 */
  unsigned char label;        /* 0x01 */
  unsigned char block_size;     /* 0x0c */
  unsigned short  left;
  unsigned short  top;
  unsigned short  width;
  unsigned short  height;
  unsigned char fore_color_index; /* 前景色索引 */
  unsigned char bk_color_index;   /* 背景色索引 */
};

/* 应用程序扩展块 */
struct _tagAPPCTRL {
  unsigned char extintr;        /* 0x21 */
  unsigned char label;        /* 0xff */
  unsigned char block_size;     /* 0xb  */
  unsigned char identifier[8];
  unsigned char authentication[3];
};

/* add by menghui 2003.06.18 */
#define MAXGIFBITS 12


/*==================================================================*/
/* 内部助手函数 */
/*==================================================================*/


/* 读入静态GIF图像数据 */
int CALLAGREEMENT _load_static_gif(
    LPINFOSTR pinfo_str,
    unsigned char* stream,
    unsigned long stream_len,
    LPGIFINSIDEINFO p_info)
{
  assert(pinfo_str->imgtype == IMT_RESSTATIC);

  /* 以子图象方式读入主图象 */
  if (_load_subimg_serial(pinfo_str, stream, stream_len) == -1) {
    return -1;
  }

  assert(pinfo_str->imgnumbers == 1);

  /* 将图象数据从子图象结构中移到主图象结构 */
  memcpy((void*)pinfo_str->palette, (const void*)pinfo_str->psubimg->palette, sizeof(unsigned long)*MAX_PALETTE_COUNT);
  pinfo_str->pal_count = pinfo_str->psubimg->pal_count;
  pinfo_str->pp_line_addr = pinfo_str->psubimg->pp_line_addr;
  pinfo_str->p_bit_data = pinfo_str->psubimg->p_bit_data;

  memset((void*)pinfo_str->psubimg, 0, sizeof(SUBIMGBLOCK));

  /* 释放子图象结构 */
  _free_SUBIMGBLOCK(pinfo_str->psubimg);

  pinfo_str->psubimg = 0;

  /* 回调通知 */
  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* 用户中断 */

  case  3:
    return -4;
    break;   /* 内存不足 */

  case  2:
  default:
    return -3;
    break;   /* 系统异常 */
  }

  /* 回调通知 */
  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* 用户中断 */

  case  3:
    return -4;
    break;   /* 内存不足 */

  case  2:
  default:
    return -3;
    break;   /* 系统异常 */
  }

  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* 用户中断 */

  case  3:
    return -4;
    break;   /* 内存不足 */

  case  2:
  default:
    return -3;
    break;   /* 系统异常 */
  }

  return 0;
}



/* 读入动态GIF图像数据 */
int CALLAGREEMENT _load_dyn_gif(
    LPINFOSTR pinfo_str,
    unsigned char* stream,
    unsigned long stream_len,
    LPGIFINSIDEINFO p_info)
{
  LPGIFHEADER   pgif_header = (LPGIFHEADER)stream;
  LPSUBIMGBLOCK subimg_tmp;
  int       i;

  assert(pinfo_str->imgtype == IMT_RESDYN);

  /* 申请背景图 */
  if (_build_bkimg(pinfo_str, p_info) == -1) {
    return -1;
  }

  /* 背景图也将计入图像个数中 */
  pinfo_str->imgnumbers = 1;

  /* 回调通知 */
  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* 用户中断 */

  case  3:
    return -4;
    break;   /* 内存不足 */

  case  2:
  default:
    return -3;
    break;   /* 系统异常 */
  }

  /* 回调通知 */
  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* 用户中断 */

  case  3:
    return -4;
    break;   /* 内存不足 */

  case  2:
  default:
    return -3;
    break;   /* 系统异常 */
  }

  switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
  case  0:
    break;

  case  1:
    return -2;
    break;   /* 用户中断 */

  case  3:
    return -4;
    break;   /* 内存不足 */

  case  2:
  default:
    return -3;
    break;   /* 系统异常 */
  }

  /* 读入子图像序列 */
  if (_load_subimg_serial(pinfo_str, stream, stream_len) == -1) {
    return -1;
  }

  assert(pinfo_str->imgnumbers > 1);

  subimg_tmp = pinfo_str->psubimg;
  assert(subimg_tmp);

  while (subimg_tmp) {
    /* 回调通知 */
    switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_SERT_IMAGE, subimg_tmp->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
    case  0:
      break;

    case  1:
      return -2;
      break;   /* 用户中断 */

    case  3:
      return -4;
      break;   /* 内存不足 */

    case  2:
    default:
      return -3;
      break;   /* 系统异常 */
    }

    /* 回调通知 */
    switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, subimg_tmp->number, 0, subimg_tmp->height, IRWE_CALLBACK_FUNID_LOAD)) {
    case  0:
      break;

    case  1:
      return -2;
      break;   /* 用户中断 */

    case  3:
      return -4;
      break;   /* 内存不足 */

    case  2:
    default:
      return -3;
      break;   /* 系统异常 */
    }

    switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, subimg_tmp->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
    case  0:
      break;

    case  1:
      return -2;
      break;   /* 用户中断 */

    case  3:
      return -4;
      break;   /* 内存不足 */

    case  2:
    default:
      return -3;
      break;   /* 系统异常 */
    }

    subimg_tmp = subimg_tmp->next;
  }

  // 分配播放顺序表
  pinfo_str->play_order = (unsigned long*)isirw_malloc((pinfo_str->imgnumbers - 1) * sizeof(unsigned long*));

  if (pinfo_str->play_order) {
    for (i = 1; i < (int)pinfo_str->imgnumbers; i++) {
      pinfo_str->play_order[i - 1] = i;     // 设置播放顺序（跨过主图象）
    }

    pinfo_str->play_number = pinfo_str->imgnumbers - 1;
  }
  else {
    pinfo_str->play_number = 0;           // 采用缺省播放顺序
  }

  return 0;
}


/* 获取GIF图像信息
/
/ 参数p_filecontent指向的缓冲区必须包括完整的GIF流内容
/ 参数filesize是指明p_filecontent所指向的缓冲区的尺寸（即GIF流的尺寸）
/ 成功返回0，失败返回-1，图像数据有些不正规，但已成功读取，返回1
/ 注：如果是一个动态GIF（图像个数大于1），图像的宽、高数据将以
/   逻辑屏的宽、高为准，如果是静态GIF（单幅图像），则将以图像
/   的宽、高数据为准。
*/
int CALLAGREEMENT _get_imginfo(unsigned char* p_filecontent, unsigned long filesize, LPGIFINSIDEINFO p_info)
{
  unsigned char*  p = p_filecontent;
  int       type = 0, i;
  unsigned char data_len;
  int       color_num;
  int       first_img_mark = 0;
  unsigned long len = 0;

  GIFHEADER   gifHeader;
  IMAGEDATAINFO image_data; /* 图像数据块 */


  assert(p_filecontent);
  assert(p_info);

  memcpy(&gifHeader, p, sizeof(GIFHEADER));

  if (!memcmp((const void*)gifHeader.gif_type, (const char*)GIF_MARKER_89a, strlen((const char*)GIF_MARKER_89a))) {
    type = 1;
  }
  else if (!memcmp((const void*)gifHeader.gif_type, (const char*)GIF_MARKER_87a, strlen((const char*)GIF_MARKER_87a))) {
    type = 2;
  }
  else {
    return -1;  /* 非法图像数据 */
  }

  /* 填写返回数据 */
  p_info->type = type;
  p_info->scr_width    = (unsigned long)gifHeader.scr_width;
  p_info->scr_height   = (unsigned long)gifHeader.scr_height;
  /*p_info->scr_bitcount = (unsigned long)(((gifHeader.global_flag&0x70)>>4)+1);*/ /* GIF文档在此处的说明有些模糊 */
  p_info->bk_index     = (int)gifHeader.bg_color;

  if ((p_info->scr_width == 0) || (p_info->scr_height == 0)) {
    return -1;  /* 非法图像数据 */
  }

  if (gifHeader.global_flag & 0x80) {
    p_info->scr_bitcount = (unsigned long)((gifHeader.global_flag & 0x7) + 1);

    /* 测试背景色索引的合法性 */
    if (p_info->bk_index >= (1L << p_info->scr_bitcount)) {
      p_info->bk_index = (1L << p_info->scr_bitcount) - 1;  /* 非法，容错处理－等于最高索引 */
    }

    /* return -1; 如果对源图像数据要求严格的话，可将以上 */
    /* 一行的代码用本行return -1;语句替换。*/
  }
  else {
    p_info->scr_bitcount = (unsigned long)0;
    p_info->bk_index = 0;
  }


  /* 跨过GIF流头及逻辑屏幕描述块 */
  p += sizeof(GIFHEADER);
  len += sizeof(GIFHEADER);

  /* 判断是否存在全局调色板，如果存在则提取数据并跨过该调色板 */
  if (gifHeader.global_flag & 0x80) {
    color_num = 1 << (int)(unsigned int)((gifHeader.global_flag & 0x7) + 1);
    /*color_num = 1 << p_info->scr_bitcount;*/

    if (color_num > GIF_MAXCOLOR) {
      return -1;  /* 受损的图像 */
    }

    if ((sizeof(GIFHEADER) + color_num * sizeof(GIFRGB)) >= (int)filesize) {
      return -1;  /* 受损的图像 */
    }

    len += color_num * sizeof(GIFRGB);

    for (i = 0; i < color_num; i++) {
      p_info->p_pal[i].bRed = *p++;
      p_info->p_pal[i].bGreen = *p++;
      p_info->p_pal[i].bBlue  = *p++;
    }
  }
  else {
    /* 如果没有全局调色板数据，则用灰度图数据初始化调色板数组 */
    for (i = 0; i < GIF_MAXCOLOR; i++) {
      p_info->p_pal[i].bRed   = (unsigned char)i;
      p_info->p_pal[i].bGreen = (unsigned char)i;
      p_info->p_pal[i].bBlue  = (unsigned char)i;
    }
  }

  /* 分解数据区 */
  while (1) {
    if (p[0] == 0x2c) { /* 图像描述符＋[局部调色板]＋图像数据块 */
      if ((len + sizeof(IMAGEDATAINFO)) >= filesize) {
        return -1;  /* 受损的图像 */
      }

      /* 跨过图像描述符结构 */
      memcpy(&image_data, p, sizeof(IMAGEDATAINFO));
      p += sizeof(IMAGEDATAINFO);
      len += sizeof(IMAGEDATAINFO);

      if (image_data.local_flag & 0x80) {
        if ((len + (1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1))*sizeof(GIFRGB)) > filesize) {
          return -1;
        }
        else {
          len += (1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1)) * sizeof(GIFRGB);
        }
      }

      /* 填写首幅图像信息 */
      if (first_img_mark == 0) {
        p_info->first_img_width = (unsigned long)image_data.width;
        p_info->first_img_height = (unsigned long)image_data.height;

        if (image_data.local_flag & 0x80) {
          p_info->first_img_bitcount = (int)(unsigned int)((image_data.local_flag & 0x7) + 1);
        }
        else if (p_info->scr_bitcount) {
          p_info->first_img_bitcount = p_info->scr_bitcount;
        }
        else {
          p_info->first_img_bitcount = 1;   /* 容错处理 */
        }

        /*
        if (image_data.local_flag&0x80)
          p_info->first_img_bitcount = (unsigned long)((image_data.local_flag&0x7)+1);
        else
          p_info->first_img_bitcount = p_info->scr_bitcount;
        */

        if ((p_info->first_img_width == 0) || (p_info->first_img_height == 0)) {
          return -1;  /* 受损的图像 */
        }

        p_info->img_count = 0;

        /* 提取局部调色板数据 */
        if (image_data.local_flag & 0x80) {
          color_num = 1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1);
          //color_num = 1 << p_info->first_img_bitcount;

          if (color_num > GIF_MAXCOLOR) {
            return -1;    /* 受损的图像 */
          }

          for (i = 0; i < color_num; i++) {
            p_info->p_first_pal[i].bRed   = *p++;
            p_info->p_first_pal[i].bGreen = *p++;
            p_info->p_first_pal[i].bBlue  = *p++;
          }
        }
        else {
          memmove((void*)p_info->p_first_pal, (const void*)p_info->p_pal, sizeof(GIFRGB)*GIF_MAXCOLOR);
        }

        first_img_mark = 1;
      }
      else {
        /* 非首幅图像：如果拥有局部调色板数据，则跨过 */
        if (image_data.local_flag & 0x80) {
          p += (1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1)) * sizeof(GIFRGB);
        }
      }

      if ((len + 1) > filesize) {
        return -1;
      }

      p++;        /* 越过GIF-LZW最小编码值 */
      len++;
      data_len = 1;

      /* 跨过经压缩的图像数据子块 */
      while (data_len) {
        if ((len + 1) > filesize) {
          return -1;  /* 受损的图像流 */
        }

        data_len = *p++;
        p += data_len;
        len += data_len + 1;
      }

      /* 图像个数加1 */
      p_info->img_count++;
    }
    else if (p[0] == 0x21 && p[1] == 0xf9 && p[2] == 0x4) { /* 图形控制扩展块 */
      if ((len + sizeof(GRAPHCTRL)) > filesize) {
        return -1;  /* 受损的图像流 */
      }

      p += sizeof(GRAPHCTRL);
      len += sizeof(GRAPHCTRL);
    }
    else if (p[0] == 0x21 && p[1] == 0x1 && p[2] == 0xc) { /* 文本扩展块 */
      if ((len + sizeof(TEXTCTRL)) > filesize) {
        return -1;  /* 受损的图像流 */
      }

      p += sizeof(TEXTCTRL);
      len += sizeof(TEXTCTRL);

      data_len = 1;

      /* 跨过文本子块序列  */
      while (data_len) {
        if ((len + 1) > filesize) {
          return -1;  /* 受损的图像流 */
        }

        data_len = *p++;
        p += data_len;
        len += data_len + 1;
      }
    }
    else if (p[0] == 0x21 && p[1] == 0xff && p[2] == 0xb) { /* 应用程序扩展块 */
      if ((len + sizeof(APPCTRL)) > filesize) {
        return -1;  /* 受损的图像流 */
      }

      p += sizeof(APPCTRL);
      len += sizeof(APPCTRL);

      data_len = 1;

      /* 跨过文本子块序列  */
      while (data_len) {
        if ((len + 1) > filesize) {
          return -1;  /* 受损的图像流 */
        }

        data_len = *p++;
        p += data_len;
        len += data_len + 1;
      }
    }
    else if (p[0] == 0x21 && p[1] == 0xfe) {        /* 注释扩展块 */
      if ((len + sizeof(NOTEHCTRL)) > filesize) {
        return -1;  /* 受损的图像流 */
      }

      p += sizeof(NOTEHCTRL);
      len += sizeof(NOTEHCTRL);

      data_len = 1;

      /* 跨过文本子块序列  */
      while (data_len) {
        if ((len + 1) > filesize) {
          return -1;  /* 受损的图像流 */
        }

        data_len = *p++;
        p += data_len;
        len += data_len + 1;
      }
    }
    else if (p[0] == 0x0) {               /* 滤除多余的子块结尾符，在正规的文档中没有说明多余字块结尾符，此处为经验代码 */
      p++;
      len++;
    }
    else if (p[0] == 0x3b) {                /* 结束 */
      break;
    }
    else if (first_img_mark == 1) {           /* 容错处理（兼容那些图象数据尾部没有结束标志的GIF）*/
      break;
    }
    else {
      return -1;  /* 受损的图像流 */
    }

    if (len > filesize) {
      return -1;  /* 受损的图像流 */
    }
  }

  if ((len + 1) != filesize) {
    return 1;   /* 此图像流尾部还有数据，但并不是GIF的数据，可能 */
    /* 此图像被附加了另外的信息。不过这不影响图像的  */
    /* 正常读取，属良性错误:)，所以返回1而不是-1     */
  }

  /* 成功返回 */
  return 0;
}


/* 将给定的LZW压缩数据解压为BMP数据，并将这些数据保存在一个新申请的子数据节点中 */
LPSUBIMGBLOCK CALLAGREEMENT _decomp_LZW_to_BMP(
    LPINFOSTR pinfo_str,      /* 主图像块地址 */
    unsigned char* plzw,      /* 图像块首地址 */
    unsigned char* stream,      /* GIF流首地址 */
    LPGRAPHCTRL p_imgctrl,      /* 最近有效的图像控制扩展块 */
    unsigned long* block_size)    /* 返回图像数据块的实际尺寸 */
{
  LZWTABLE    str_table[LZWTABLESIZE];        /* GIF-LZW 串表 */
  unsigned char cw_buf[LZWTABLESIZE];         /* 输出字串时使用的缓冲区 */

  LPGIFHEADER   p_header = (LPGIFHEADER)stream;     /* GIF流头结构 */
  LPIMAGEDATAINFO p_imginfo = (LPIMAGEDATAINFO)plzw;
  unsigned char*   p_data, *p;

  unsigned char first_char;
  int       cw_buf_len, percw;
  int       linesize, init_bit_len;
  unsigned long imgdata_size, imgblock_size;

  int       lzw_clear_code, lzw_eoi_code, cur_bit_len;
  int       cur_insert_pos, cur_code, old_code, old_code_mark;
  /* 目标缓冲区当前写位置相关参数 */
  int       cur_row, cur_x, cur_row_bit_count, intr, pass;
  unsigned short  code_buf;
  unsigned long bit_count;

  unsigned char*   p_lzwdata = 0;
  LPSUBIMGBLOCK p_subimg   = 0;

  assert(plzw && ((*plzw) == 0x2c));
  assert(stream && ((*stream) == 'G'));

  __try {
    /* 申请一个子图像节点 */
    if ((p_subimg = _alloc_SUBIMGBLOCK(pinfo_str, plzw, stream, p_imgctrl)) == 0) {
      return 0;  /* 内存不足或图像数据非法 */
    }

    assert((p_subimg->p_bit_data) && (p_subimg->pp_line_addr));

    /* 计算图像数据偏移 */
    if (p_imginfo->local_flag & 0x80) {
      p_data = plzw + sizeof(IMAGEDATAINFO) + (1 << (int)(unsigned int)((p_imginfo->local_flag & 0x7) + 1)) * sizeof(GIFRGB);
    }
    else {
      p_data = plzw + sizeof(IMAGEDATAINFO);
    }

    /* DIB行尺寸 */
    linesize = DIBSCANLINE_WIDTHBYTES(p_subimg->width * p_subimg->bitcount);

    p = p_data;

    /* 提取初始位长度值 */
    init_bit_len = (int) * p++;

    /* 获取纯LZW数据长度和LZW数据块长度 */
    _get_imgdata_size(p, &imgdata_size, &imgblock_size);

    /* 返回实际图像块（LZW块）的长度 */
    *block_size = imgblock_size;

    /* 将LZW数据提取到一个新的、连续的内存块中 */
    if ((p_lzwdata = _get_lzw_datablock(p, imgdata_size)) == 0) {
      _free_SUBIMGBLOCK(p_subimg);
      return 0;         /* 内存不足 */
    }


    /* 解码前的初始化工作 */
    p = p_lzwdata;

    intr = (p_imginfo->local_flag & 0x40) ? 1 : 0; /* 交叉标志 */
    cur_row = cur_x = cur_row_bit_count = 0;  /* 目标图像缓冲区行、列位置 */
    pass = 1;                 /* 交叉通道值 */

    lzw_clear_code = 1 << init_bit_len;   /* 清除码 */
    lzw_eoi_code   = lzw_clear_code + 1;    /* 终结码 */
    cur_bit_len    = init_bit_len + 1;    /* 当前码长度 */
    cur_insert_pos = lzw_eoi_code + 1;    /* 当前串表插入位置 */
    bit_count      = 0;             /* 从流中读取的数据位总数 */
    old_code       = 0;             /* 前缀码 */
    old_code_mark  = 0;             /* 首标志 */


    /* 开始解码 */
    while ((cur_code = (int)_get_next_codeword(p, imgdata_size, bit_count / 8, bit_count % 8, cur_bit_len)) != lzw_eoi_code) {
      if (cur_code == lzw_clear_code) {     /* 清除码 */
        bit_count      += cur_bit_len;
        cur_bit_len    = init_bit_len + 1;
        cur_insert_pos = lzw_eoi_code + 1;

        old_code       = 0;
        old_code_mark  = 0;

        /* 初始化串表 */
        _init_lzw_table((LZWTABLE*)str_table, LZWTABLESIZE, init_bit_len);
        continue;
      }
      else if (cur_code < cur_insert_pos) {   /* 存在于串表中 */
        /* 输出码字 */
        cw_buf_len = 0;
        percw = cur_code;

        /* 将码字对应的图像数据串存入缓冲区 */
        while (percw != -1) {
          /* assert(str_table[percw].used == 1); */
          cw_buf[cw_buf_len++] = (unsigned char)str_table[percw].code;
          percw = (int)str_table[percw].prefix;
        }

        /* 写入目标图像缓冲区 */
        while (cw_buf_len > 0) {
          if (intr) { /* 是否是交错模式 */
            if (cur_x == p_subimg->width) {
              cur_x = 0;
              cur_row_bit_count = 0;

              /* 交错跳进 */
              if (pass == 1) {
                cur_row += 8;
              }

              if (pass == 2) {
                cur_row += 8;
              }

              if (pass == 3) {
                cur_row += 4;
              }

              if (pass == 4) {
                cur_row += 2;
              }

              if (cur_row >= p_subimg->height) {
                pass++;
                cur_row = 0x10 >> pass;
              }
            }
          }
          else {
            if (cur_x == p_subimg->width) {
              cur_x = 0;
              cur_row_bit_count = 0;
              cur_row++;
            }
          }

          /* 容错处理，当cur_row < height时跳过 by menghui 2003.06.18 */
          if (cur_row < p_subimg->height) {
            /* 提出图像数据并移动到预定位置 */
            code_buf = (unsigned short)(unsigned char)(cw_buf[cw_buf_len - 1]);
            code_buf &= (unsigned short)((1 << (p_subimg->bitcount)) - 1);
            code_buf <<= cur_row_bit_count % 8;

            /* 将象素数据按紧凑位写入图像缓冲区 */
            *((unsigned char*)(((unsigned char*)(p_subimg->pp_line_addr[cur_row])) + cur_row_bit_count / 8)) |= (unsigned char)code_buf;

            if (((cur_row_bit_count % 8) + p_subimg->bitcount) > 8) {
              *((unsigned char*)(((unsigned char*)(p_subimg->pp_line_addr[cur_row])) + cur_row_bit_count / 8 + 1)) |= (unsigned char)(code_buf >> 8);
            }
          }

          cur_x++;
          cur_row_bit_count += p_subimg->bitcount;

          cw_buf_len--;
        }

        /* 是否是第一次读入 */
        if (old_code_mark) {
          /* 向串表中添加新码字（存在码字情况下） */
          percw = cur_code;

          /* 取得当前码字第一个字符 */
          while (percw != -1) {
            /* assert(str_table[percw].used == 1); */
            first_char = (unsigned char)str_table[percw].code;
            percw = (int)str_table[percw].prefix;
          }

          /* 输出新码字到LZW串表中 */
          str_table[cur_insert_pos].used = 1;
          str_table[cur_insert_pos].code = (short)first_char;
          str_table[cur_insert_pos].prefix = old_code;

          /* 插入位置后移 */
          cur_insert_pos++;
        }
        else {
          old_code_mark = 1;  /* 第一次进入 */
        }

        old_code = cur_code;
        bit_count += cur_bit_len;
      }
      else {                  /* 不存在的编码 */
        /* 向串表中添加新码字（不存在码字情况下） */
        percw = old_code;

        /* 取得前缀码字第一个字符 */
        while (percw != -1) {
          /* assert(str_table[percw].used == 1); */
          first_char = (unsigned char)str_table[percw].code;
          percw = (int)str_table[percw].prefix;
        }

        /* 输出新码字 */
        str_table[cur_insert_pos].used = 1;
        str_table[cur_insert_pos].code = (short)first_char;
        str_table[cur_insert_pos].prefix = old_code;

        cur_insert_pos++;

        /* 输出码字 */
        cw_buf_len = 0;
        /* 上面刚刚生成的新码字 */
        percw = cur_insert_pos - 1;

        /* 将码字对应的图像数据串存入缓冲区 */
        while (percw != -1) {
          /* assert(str_table[percw].used == 1); */
          cw_buf[cw_buf_len++] = (unsigned char)str_table[percw].code;
          percw = (int)str_table[percw].prefix;
        }

        while (cw_buf_len > 0) {
          if (intr) { /* 是否是交错模式 */
            if (cur_x == p_subimg->width) {
              cur_x = 0;
              cur_row_bit_count = 0;

              if (pass == 1) {
                cur_row += 8;
              }

              if (pass == 2) {
                cur_row += 8;
              }

              if (pass == 3) {
                cur_row += 4;
              }

              if (pass == 4) {
                cur_row += 2;
              }

              if (cur_row >= p_subimg->height) {
                pass++;
                cur_row = 0x10 >> pass;
              }
            }
          }
          else {
            if (cur_x == p_subimg->width) {
              cur_x = 0;
              cur_row_bit_count = 0;
              cur_row++;
            }
          }

          /* 只能说非标准的GIF太多了!!!
          // 居然有实际高度比自己定义的要长的情况，拷！
          // by menghui 2003.06.18 */
          if (cur_row < p_subimg->height) {
            code_buf = (unsigned short)(cw_buf[cw_buf_len - 1]);
            code_buf &= (unsigned short)((1 << (p_subimg->bitcount)) - 1);
            code_buf <<= cur_row_bit_count % 8;

            /* 将象素数据按紧凑位写入图像缓冲区 */
            *((unsigned char*)(((unsigned char*)(p_subimg->pp_line_addr[cur_row])) + cur_row_bit_count / 8)) |= (unsigned char)code_buf;

            if (((cur_row_bit_count % 8) + p_subimg->bitcount) > 8) {
              *((unsigned char*)(((unsigned char*)(p_subimg->pp_line_addr[cur_row])) + cur_row_bit_count / 8 + 1)) |= (unsigned char)(code_buf >> 8);
            }
          }

          cur_x++;
          cur_row_bit_count += p_subimg->bitcount;

          cw_buf_len--;
        }

        old_code = cur_code;
        bit_count += cur_bit_len;
      }

      if ((cur_insert_pos >> cur_bit_len) > 0) {
        cur_bit_len++;
      }

      if (cur_bit_len > MAXGIFBITS) {
        cur_bit_len = MAXGIFBITS;
      }

      /*
       *  最初这一句为 if(bit_count / 8 > imgblock_size) break;
       *  不知道是不是资料中这么说的，但实际应用中出现问题，我将此问题报告给yzfree处理，
       *  也许yzfree太忙还是另的原因，他没有发现此处的错误而是加入了一个warninglevel的变量来临时将错误压制
       *  这也使得一些GIF图无法显示。
       *  这两天闲来无聊就详细的看了这段代码，发现错误即改之。
       *  by menghui 2003.06.18
       */
      if (bit_count / 8 >= imgdata_size) {
        break;
      }
    }
  }
  __finally {
    if (AbnormalTermination()) {
      if (p_subimg) {
        _free_SUBIMGBLOCK(p_subimg);
      }

      p_subimg = 0;
    }

    if (p_lzwdata) {
      isirw_free(p_lzwdata);
    }

    p_lzwdata = 0;
  }

  return p_subimg;
}


int CALLAGREEMENT _load_subimg_serial(LPINFOSTR pinfo_str, unsigned char* stream, unsigned long stream_len)
{
  unsigned char* p = stream;
  unsigned char* pimg, *ptmp;
  int       type = 0, i, j;
  unsigned char data_len;
  unsigned long block_size;

  GIFHEADER   gifHeader;
  IMAGEDATAINFO image_data;     /* 图像数据块 */
  GRAPHCTRL   cur_craph_ctrl;   /* 当前的图像控制数据 */

  LPSUBIMGBLOCK subimg_list = 0;  /* 子图像链表 */
  LPSUBIMGBLOCK subimg_tail = 0;
  LPSUBIMGBLOCK subimg_tmp  = 0;


  assert(p);
  assert(pinfo_str);

  memset((void*)&cur_craph_ctrl, 0, sizeof(GRAPHCTRL));
  memcpy(&gifHeader, p, sizeof(GIFHEADER));

  /* 跨过GIF流头及逻辑屏幕描述块 */
  p += sizeof(GIFHEADER);

  /* 跨过全局调色板 */
  if (gifHeader.global_flag & 0x80) {
    p += (1 << ((gifHeader.global_flag & 0x7) + 1)) * sizeof(GIFRGB);
  }

  /* 初始化图像控制数据 */
  cur_craph_ctrl.extintr      = 0x21;
  cur_craph_ctrl.label      = 0xf9;
  cur_craph_ctrl.block_size   = 0x4;
  cur_craph_ctrl.delay_time   = 0;
  cur_craph_ctrl.field      = 0;
  cur_craph_ctrl.tran_color_index = -1;
  cur_craph_ctrl.block_terminator = 0;


  /* 分解数据区 */
  while (1) {
    if (p[0] == 0x2c) { /* 图像描述符＋[局部调色板]＋图像数据块 */
      /* 保留图像描述块首地址 */
      pimg = p;

      /* 跨过图像描述符结构 */
      memcpy(&image_data, p, sizeof(IMAGEDATAINFO));
      p += sizeof(IMAGEDATAINFO);

      /* 如果拥有局部调色板数据，则跨过 */
      if (image_data.local_flag & 0x80) {
        p += (1 << (int)(unsigned int)((image_data.local_flag & 0x7) + 1)) * sizeof(GIFRGB);
      }

      /* 对子图像进行解码 */
      subimg_tmp = _decomp_LZW_to_BMP(pinfo_str, pimg, stream, &cur_craph_ctrl, &block_size);

      /* 支持用户中断操作 */
      if (pinfo_str->pater->s_break) {
        _free_SUBIMGBLOCK(subimg_tmp);
        subimg_tmp = 0;
      }

      /* 如果解码失败，释放子图像链表，返回错误 */
      if (subimg_tmp == 0) {
        if (subimg_list != 0) {
          while (subimg_tmp = subimg_list->next) {
            _free_SUBIMGBLOCK(subimg_list);
            subimg_list = subimg_tmp;
          }

          _free_SUBIMGBLOCK(subimg_list);
          subimg_list = 0;
        }

        return -1;
      }

      /* 将1、4位图象转为ISee的显示格式(以符合2.2接口标准) */
      switch (subimg_tmp->bitcount) {
      case  1:
        for (i = 0; i < (int)subimg_tmp->height; i++) {
          ptmp = (unsigned char*)(subimg_tmp->pp_line_addr[i]);

          for (j = 0; j < (int)subimg_tmp->width; j += 8) {
            CVT_BITS1((*ptmp));       /* 颠倒位序 */
            ptmp++;
          }
        }

        break;

      case  4:
        for (i = 0; i < (int)subimg_tmp->height; i++) {
          ptmp = (unsigned char*)(subimg_tmp->pp_line_addr[i]);

          for (j = 0; j < (int)subimg_tmp->width; j += 2) {
            CVT_BITS4((*ptmp));       /* 颠倒位序 */
            ptmp++;
          }
        }

        break;

      default:
        break;
      }

      /* 将子图像加入子图像链表尾部 */
      if (subimg_list == 0) {
        subimg_list = subimg_tmp; /* 链表头 */
        subimg_tail = subimg_tmp; /* 链表尾 */

        subimg_tmp->prev = 0;
        subimg_tmp->next = 0;
        subimg_tmp->number = 1;
        subimg_tmp->parents = pinfo_str;
      }
      else {
        subimg_tmp->prev = subimg_tail;
        subimg_tmp->next = 0;
        subimg_tmp->number = subimg_tail->number + 1;
        subimg_tmp->parents = pinfo_str;

        assert(subimg_tail->next == 0);

        subimg_tail->next = subimg_tmp;
        subimg_tail = subimg_tmp;
      }

      /* 图像个数加1 */
      pinfo_str->imgnumbers++;

      /* 跨过图像数据 */
      p += block_size;
    }
    else if (p[0] == 0x21 && p[1] == 0xf9 && p[2] == 0x4) { /* 图形控制扩展块 */
      memcpy((void*)&cur_craph_ctrl, (const void*)p, sizeof(GRAPHCTRL));
      p += sizeof(GRAPHCTRL);
    }
    else if (p[0] == 0x21 && p[1] == 0x1 && p[2] == 0xc) { /* 文本扩展块 */
      p += sizeof(TEXTCTRL);
      data_len = 1;

      /* 跨过文本子块序列  */
      while (data_len) {
        data_len = *p++;
        p += data_len;
      }
    }
    else if (p[0] == 0x21 && p[1] == 0xff && p[2] == 0xb) { /* 应用程序扩展块 */
      p += sizeof(APPCTRL);
      data_len = 1;

      /* 跨过文本子块序列  */
      while (data_len) {
        data_len = *p++;
        p += data_len;
      }
    }
    else if (p[0] == 0x21 && p[1] == 0xfe) {        /* 注释扩展块 */
      p += sizeof(NOTEHCTRL);
      data_len = 1;

      /* 跨过文本子块序列  */
      while (data_len) {
        data_len = *p++;
        p += data_len;
      }
    }
    else if (p[0] == 0x0) {               /* 滤除多余的子块结尾符，在正规的文档中没有说明多余字块结尾符，此处为经验代码 */
      p++;
    }
    else if (p[0] == 0x3b) {                /* 结束 */
      break;
    }
    else if (pinfo_str->imgnumbers >= 1) {        /* 容错处理（针对于那些图象数据尾部没有结束标志的GIF）*/
      break;
    }
    else {
      /* 释放子图像链表 */
      if (subimg_list != 0) {
        while (subimg_tmp = subimg_list->next) {
          _free_SUBIMGBLOCK(subimg_list);
          subimg_list = subimg_tmp;
        }

        _free_SUBIMGBLOCK(subimg_list);
        subimg_list = 0;
      }

      pinfo_str->imgnumbers = 0;

      return -1;  /* 受损的图像流 */
    }
  }

  pinfo_str->psubimg = subimg_list;

  /* 成功返回 */
  return 0;
}



/* 初始化LZW串表 */
void CALLAGREEMENT _init_lzw_table(LZWTABLE* pt, int table_size, int init_bit_len)
{
  int i, len;

  assert(init_bit_len <= MAXGIFBITS);

  memset((void*)pt, 0, sizeof(LZWTABLE)*table_size);

  len = 1 << init_bit_len;

  /* 根码 */
  for (i = 0; i < len; i++) {
    pt[i].prefix = -1;
    pt[i].code   = (short)i;
    pt[i].used   = 1;
  }

  /* 清除码 */
  pt[len].prefix = -1;
  pt[len].code   = -1;
  pt[len].used   = 1;

  /* 结束码 */
  pt[len + 1].prefix = -1;
  pt[len + 1].code   = -1;
  pt[len + 1].used   = 1;
}


/* 分配一个子图像节点，并初始化节点内数据 */
LPSUBIMGBLOCK CALLAGREEMENT _alloc_SUBIMGBLOCK(
    LPINFOSTR pinfo_str,
    unsigned char* plzw,       /* 图像块首地址 */
    unsigned char* stream,     /* GIF流首地址 */
    LPGRAPHCTRL p_imgctrl)     /* 最近有效的图像控制扩展块 */
{
  LPGIFHEADER   p_header = (LPGIFHEADER)stream;
  LPIMAGEDATAINFO p_imginfo = (LPIMAGEDATAINFO)plzw;
  LPGIFRGB    p_pal;

  int       linesize, i, colorkey;

  LPSUBIMGBLOCK p_subimg = 0;


  /* 申请子图像信息节点(SUBIMGBLOCK) */
  if ((p_subimg = (LPSUBIMGBLOCK)isirw_malloc(sizeof(SUBIMGBLOCK))) == 0) {
    return 0;  /* 内存不足 */
  }

  memset((void*)p_subimg, 0, sizeof(SUBIMGBLOCK));

  /* 设置子图像节点数据 */
  p_subimg->left = (int)p_imginfo->left;
  p_subimg->top = (int)p_imginfo->top;
  p_subimg->width = (int)p_imginfo->width;
  p_subimg->height = (int)p_imginfo->height;

  /* 容错处理，某些软件生成的GIF图像块的global_flag是错误的，所以只能自己从全局bitcount从取得正确的数据 */
  /* p_subimg->bitcount = (int)(unsigned int)(p_header->global_flag&0x7)+1; */
  p_subimg->bitcount = pinfo_str->bitcount;

  if ((p_subimg->width == 0) || (p_subimg->height == 0)) {
    isirw_free(p_subimg);   /* 非法图像数据 */
    return 0;
  }

  /* 计算调色板位置 */
  if (p_imginfo->local_flag & 0x80) {
    p_pal = (LPGIFRGB)(unsigned char*)(plzw + sizeof(IMAGEDATAINFO));
  }
  else {
    if (!(p_header->global_flag & 0x80)) {
      isirw_free(p_subimg);
      return 0;       /* 非法图像（既没有子图像位数也没有全局位数值）*/
    }
    else {
      p_pal = (LPGIFRGB)(unsigned char*)(p_header + 1);
    }
  }

  /* DIB行尺寸 */
  linesize = DIBSCANLINE_WIDTHBYTES(p_subimg->width * p_subimg->bitcount);

  /* 申请目标图像缓冲区（+4 － 附加的4字节缓冲区） */
  if ((p_subimg->p_bit_data = (unsigned char*)isirw_malloc(linesize * p_subimg->height + 4)) == 0) {
    isirw_free(p_subimg);
    return 0;         /* 内存不足 */
  }

  /* 申请行地址数组 */
  if ((p_subimg->pp_line_addr = (void**)isirw_malloc(sizeof(void*)*p_subimg->height)) == 0) {
    isirw_free(p_subimg->p_bit_data);
    isirw_free(p_subimg);
    return 0;         /* 内存不足 */
  }

  /* 初始化图像缓冲区 */
  memset((void*)p_subimg->p_bit_data, 0, linesize * p_subimg->height);

  /* GIF为正向图像 */
  p_subimg->order   = 0;

  for (i = 0; i < p_subimg->height; i++) {
    p_subimg->pp_line_addr[i] = (void*)(p_subimg->p_bit_data + (i * linesize));
  }

  /* GIF为索引图，无掩码数据 */
  p_subimg->r_mask = 0;
  p_subimg->g_mask = 0;
  p_subimg->b_mask = 0;
  p_subimg->a_mask = 0;


  /* 处置方式：0 - 未指定处置方式
         1 - 不必做任何处理动作
         2 - 以背景色擦除
         3 - 回复原来的背景图像 */
  p_subimg->dowith = (int)((p_imgctrl->field & 0xc) >> 2);
  /* 在显示下一幅子图像前是否需要等待某一事件 */
  p_subimg->userinput = (int)((p_imgctrl->field & 0x2) >> 1);
  /* 延迟时间（1/100s 转为 1/1000s，缺省延迟时间暂设为100ms）*/
  /* 感谢 zggeng##163.com 网友的提醒！（2003-6）*/
  p_subimg->time = (p_imgctrl->delay_time == 0) ? 100 : ((int)(unsigned int)p_imgctrl->delay_time * 10);
  /* 设置透明色索引，如果没有透明色，则设为-1 */
  colorkey = (p_imgctrl->field & 0x1) ? p_imgctrl->tran_color_index : -1;


  /* 复制调色板数据 */
  p_subimg->pal_count = 1 << p_subimg->bitcount;

  for (i = 0; i < p_subimg->pal_count; i++) {
    ((LPISEERGB)(&(p_subimg->palette[i])))->bRed  = p_pal[i].bRed;
    ((LPISEERGB)(&(p_subimg->palette[i])))->bGreen  = p_pal[i].bGreen;
    ((LPISEERGB)(&(p_subimg->palette[i])))->bBlue = p_pal[i].bBlue;
    ((LPISEERGB)(&(p_subimg->palette[i])))->rev   = 0;
  }

  /* 设置透明色的实际RGB值 */
  if (colorkey != -1) {
    p_subimg->colorkey = p_subimg->palette[colorkey];

    do {
      /* 判断透明色是否是调色板中唯一的颜色 */
      for (i = 0; i < p_subimg->pal_count; i++) {
        if ((p_subimg->palette[i] == (unsigned long)p_subimg->colorkey) && (i != colorkey)) {
          break;
        }
      }

      /* 如果发生重叠现象则修改透明色 */
      if (i < p_subimg->pal_count) {
        p_subimg->colorkey += 1;
        p_subimg->colorkey &= 0xffffff;
      }
      else {
        /* 修正与透明色对应的调色板项数据 */
        p_subimg->palette[colorkey] = p_subimg->colorkey;
        break;
      }

    }
    while (i < p_subimg->pal_count);

  }
  else {
    p_subimg->colorkey = (unsigned long) - 1;
  }

  p_subimg->number  = 0;      /* 子图像块的序列号暂设为0 */

  p_subimg->parents = 0;
  p_subimg->prev    = 0;
  p_subimg->next    = 0;

  return p_subimg;
}


/* 释放一个子图像节点，包括其中的位缓冲器及行首地址数组 */
void CALLAGREEMENT _free_SUBIMGBLOCK(LPSUBIMGBLOCK p_node)
{
  assert(p_node);

  if (p_node->pp_line_addr) {
    isirw_free(p_node->pp_line_addr);
  }

  if (p_node->p_bit_data) {
    isirw_free(p_node->p_bit_data);
  }

  isirw_free(p_node);
}


/* 建立背景图（针对动态图像） */
int CALLAGREEMENT _build_bkimg(LPINFOSTR pinfo_str, LPGIFINSIDEINFO p_info)
{
  int linesize, i;
  unsigned char* pline = 0;
  unsigned char* pimg  = 0;
  void** ppaddr = 0;
  unsigned char bkindex = p_info->bk_index;

  int byte_pos, bit_pos, bit_count;
  unsigned char c;

  assert((pinfo_str->width > 0) && (pinfo_str->height > 0));

  linesize = DIBSCANLINE_WIDTHBYTES(pinfo_str->width * pinfo_str->bitcount);

  /* 分配样本行缓冲区 */
  if ((pline = (unsigned char*)isirw_malloc(linesize)) == 0) {
    return -1;
  }

  /* 初始化为0 */
  memset((void*)pline, 0, linesize);

  /* 分配背景图像缓冲区（+4 － 附加的4字节缓冲区） */
  if ((pimg = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4)) == 0) {
    isirw_free(pline);
    return -1;
  }

  /* 分配行首地址数组 */
  if ((ppaddr = (void**)isirw_malloc(sizeof(void*)*pinfo_str->height)) == 0) {
    isirw_free(pimg);
    isirw_free(pline);
    return -1;
  }

  /* 初始化行首地址（GIF正向） */
  for (i = 0; i < (int)pinfo_str->height; i++) {
    ppaddr[i] = (void*)(pimg + (i * linesize));
  }

  assert(pinfo_str->bitcount <= 8);
  assert(bkindex < (1 << pinfo_str->bitcount));

  /* 填写样本行 */
  byte_pos = bit_pos = bit_count = 0;

  for (i = 0; i < (int)pinfo_str->width; i++) {
    byte_pos = bit_count / 8;
    bit_pos  = bit_count % 8;

    c = (unsigned char)bkindex;
    c <<= bit_pos;
    pline[byte_pos] |= c;

    if ((bit_pos + pinfo_str->bitcount) > 8) {
      c = (unsigned char)bkindex;
      c >>= 8 - bit_pos;
      pline[byte_pos + 1] |= c;
    }

    bit_count += pinfo_str->bitcount;
  }

  /* 将背景图初始化为背景色的单色图 */
  for (i = 0; i < (int)pinfo_str->height; i++) {
    memmove((void*)ppaddr[i], (const void*)pline, linesize);
  }


  pinfo_str->pp_line_addr = ppaddr;
  pinfo_str->p_bit_data   = pimg;
  pinfo_str->pal_count = 1 << pinfo_str->bitcount;

  /* 复制调色板数据 */
  for (i = 0; i < (int)pinfo_str->pal_count; i++) {
    ((LPISEERGB)(&(pinfo_str->palette[i])))->bRed   = p_info->p_pal[i].bRed;
    ((LPISEERGB)(&(pinfo_str->palette[i])))->bGreen = p_info->p_pal[i].bGreen;
    ((LPISEERGB)(&(pinfo_str->palette[i])))->bBlue  = p_info->p_pal[i].bBlue;
    ((LPISEERGB)(&(pinfo_str->palette[i])))->rev    = 0;
  }

  isirw_free(pline);

  return 0;
}


/* 计算图像位数据的尺寸（包括纯图像数据尺寸和图像数据块的整体尺寸）
/
/ 注：块尺寸将包括首部的初始位置的字节。
*/
void CALLAGREEMENT _get_imgdata_size(
    unsigned char* stream,      /* 指向图像数据块首地址的指针（已跨过了初始位长度字节）*/
    unsigned long* imgdata_size,
    unsigned long* imgblock_size)
{
  unsigned char* p = stream;
  unsigned long data_count, block_count;
  unsigned char data_len;

  assert(p);

  data_count = block_count = 0UL;

  while (1) {
    data_len = *p++;        /* 提取长度描述字节值，并后移指针 */
    p += data_len;
    data_count += (unsigned long)data_len;
    block_count += ((unsigned long)data_len + 1);

    /* 加1是附带了数据子块的长度字节 */
    if (data_len == 0) {
      break;
    }
  }

  block_count++;            /* 跨过LZW初始位值的一个字节 */

  *imgdata_size  = data_count;
  *imgblock_size = block_count;
}


/* 将GIF中的纯LZW图像数据提取到新的内存块中
/
/ 如果内存分配失败，返回0
*/
unsigned char* CALLAGREEMENT _get_lzw_datablock(
    unsigned char* stream,        /* 图像块首地址（已跨过初始位长度字节）*/
    unsigned long lzw_len)
{
  unsigned char* p1, *p2, *pdata, data_len;
  unsigned long count = 0UL;

  assert(stream);

  /* 分配内存块，用于存放纯LZW数据 */
  if ((pdata = (unsigned char*)isirw_malloc(lzw_len + 1)) == (unsigned char*)0) {
    return (unsigned char*)0;
  }

  p1 = stream;
  p2 = pdata;

  while (1) {
    data_len = *p1++;

    if (data_len == 0x00) {
      break;
    }

    /* 防止错误数据干扰程序 */
    count += (unsigned long)data_len;

    if (count > lzw_len) {
      isirw_free(pdata);
      return (unsigned char*)0;
    }

    /* 将LZW数据复制到新分配的内存块中 */
    memcpy(p2, p1, data_len);

    p1 += data_len;
    p2 += data_len;
  }

  assert(count == lzw_len);

  return pdata;
}


/* 取得LZW中下一个编码字  */
unsigned short CALLAGREEMENT _get_next_codeword(
    unsigned char* code_stream,     /* LZW数据流首地址 */
    unsigned long code_stream_len,    /* 流长度 */
    unsigned long offset_byte,      /* 当前码所在字节偏移 */
    unsigned long offset_bits,      /* 当前码所在位偏移 */
    unsigned long code_word_bit_size) /* 码字位长度 */
{
  CODEWORD  cw;
  LPCODEWORD  p_code_word;

  assert(code_word_bit_size <= MAXGIFBITS);

  /*
  * 初始化lcode = 0 否则，就会有问题哦。add by menghui 2003.06.18
   */
  cw.lcode = 0;

  p_code_word = (LPCODEWORD)(unsigned char*)(code_stream + offset_byte);

  /* 12位，取3个字节即可 */
  if ((offset_byte + 0) < code_stream_len) {
    cw.bcode[0] = p_code_word->bcode[0];
  }

  if ((offset_byte + 1) < code_stream_len) {
    cw.bcode[1] = p_code_word->bcode[1];
  }

  if ((offset_byte + 2) < code_stream_len) {
    cw.bcode[2] = p_code_word->bcode[2];
  }

  /* 将数据移至字低端 */
  cw.lcode >>= offset_bits;
  cw.lcode  &= ((1 << code_word_bit_size) - 1);

  return (unsigned short)cw.lcode;
}



/* 读取图像位数据 */
GIF_API int CALLAGREEMENT gif_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  unsigned long stream_length;
  void*      ptmp = (void*)0;
  int       result;

  GIFINSIDEINFO inside;
  LPSUBIMGBLOCK subimg_tmp;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&gif_load_img_critical);

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

      /* 获取流长度 */
      stream_length = isio_length(pfile);

      /* 申请内存，用于保存整个GIF流 */
      ptmp = isirw_malloc(stream_length);

      if (!ptmp) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读取整个GIF流到内存 */
      if (isio_read(ptmp, stream_length, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 获取图像信息 */
      if (_get_imginfo((unsigned char*)ptmp, stream_length, &inside) == -1) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      inside.img_count = (inside.img_count > 1) ? inside.img_count + 1 : 1;

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        /* 图像流类型 */
        pinfo_str->imgtype  = (inside.img_count > 1) ? IMT_RESDYN : IMT_RESSTATIC;
        pinfo_str->imgformat = IMF_GIF;     /* 图像流格式（后缀名） */
        pinfo_str->compression = ICS_GIFLZW;  /* 图像压缩方式 */

        /* 如果是动态GIF，则取用逻辑屏的数据 */
        if (pinfo_str->imgtype == IMT_RESDYN) {
          pinfo_str->width  = inside.scr_width;
          pinfo_str->height = inside.scr_height;
          pinfo_str->order  = 0;      /* GIF图像为正向 */

          /* 如果没有全局调色板数据，则取首幅图像对应数据 */
          if (inside.scr_bitcount == 0) {
            pinfo_str->bitcount = inside.first_img_bitcount;
          }
          else {
            pinfo_str->bitcount = inside.scr_bitcount;
          }
        }
        else {
          pinfo_str->width  = inside.first_img_width;
          pinfo_str->height = inside.first_img_height;
          pinfo_str->order  = 0;      /* GIF图像为正向 */
          pinfo_str->bitcount = inside.first_img_bitcount;
        }

        /* GIF都为索引图，所以没有掩码数据 */
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;

        /* 设定数据包状态 */
        pinfo_str->data_state = 1;
      }


      /* 读入GIF图像 */
      if (pinfo_str->imgtype == IMT_RESSTATIC) { /* 静态 */
        result = _load_static_gif(pinfo_str, ptmp, stream_length, &inside);
      }
      else if (pinfo_str->imgtype == IMT_RESDYN) { /* 动态 */
        result = _load_dyn_gif(pinfo_str, ptmp, stream_length, &inside);
      }
      else {
        assert(0);  /* 设计错误 */
      }


      switch (result) {
      case  0:
        break;

      case  -1:
        b_status = ER_BADIMAGE;
        __leave;
        break;

      case  -2:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  -3:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  -4:
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
      if (ptmp) {
        isirw_free(ptmp);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        /* 释放子图像链表 */
        if (pinfo_str->psubimg != 0) {
          while (subimg_tmp = pinfo_str->psubimg->next) {
            _free_SUBIMGBLOCK(pinfo_str->psubimg);
            pinfo_str->psubimg = subimg_tmp;
          }

          _free_SUBIMGBLOCK(pinfo_str->psubimg);
          pinfo_str->psubimg = 0;
        }

        /* 释放主图像 */
        if (pinfo_str->pp_line_addr != 0) {
          isirw_free(pinfo_str->pp_line_addr);
        }

        if (pinfo_str->p_bit_data != 0) {
          isirw_free(pinfo_str->p_bit_data);
        }

        pinfo_str->pp_line_addr = 0;
        pinfo_str->p_bit_data   = 0;
        pinfo_str->pal_count    = 0;

        pinfo_str->imgnumbers = 0;

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
          /* 释放子图像链表 */
          if (pinfo_str->psubimg != 0) {
            while (subimg_tmp = pinfo_str->psubimg->next) {
              _free_SUBIMGBLOCK(pinfo_str->psubimg);
              pinfo_str->psubimg = subimg_tmp;
            }

            _free_SUBIMGBLOCK(pinfo_str->psubimg);
            pinfo_str->psubimg = 0;
          }

          /* 释放主图像 */
          if (pinfo_str->pp_line_addr != 0) {
            isirw_free(pinfo_str->pp_line_addr);
          }

          if (pinfo_str->p_bit_data != 0) {
            isirw_free(pinfo_str->p_bit_data);
          }

          pinfo_str->pp_line_addr = 0;
          pinfo_str->p_bit_data   = 0;
          pinfo_str->pal_count    = 0;

          pinfo_str->imgnumbers = 0;

          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1;  /* 自动降级 */
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&gif_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}

