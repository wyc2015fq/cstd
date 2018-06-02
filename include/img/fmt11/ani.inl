//ANI图像读写模块实现文件
//          读取功能：1、4、8、16、24、32位ANI图象

/* 光标类型定义 */
#define ANI_CURTYPE   2


/* 主、子图象类型定义 */
enum CUR_DATA_TYPE {
  ANI_CUR_PRI_IMAGE,
  ANI_CUR_SUB_IMAGE
};


/* 光标的图象块信息头结构 */
typedef struct {
  unsigned char width;      /* 图象宽度 */
  unsigned char height;     /* 图象高度 */
  unsigned char color_count;  /* 颜色个数(如果位深度大于8时为0) */
  unsigned char reserved;
  unsigned short  x_hotspot;    /* 热点X坐标 */
  unsigned short  y_hotspot;    /* 热点Y坐标 */
  unsigned long bytes_in_res; /* 图象数据在资源中所占的字节数 */
  unsigned long image_offset; /* 图象数据的偏移 */
} ANI_CURDIRENTRY, *LPANI_CURDIRENTRY;


/* 光标信息头结构 */
typedef struct {
  unsigned short  reserved;   /* 保留 */
  unsigned short  type;     /* 资源类型(CURTYPE为光标) */
  unsigned short  count;      /* 图象个数 */
  ANI_CURDIRENTRY entries[1];   /* 每一个图象的入口 */
} ANI_CURDIR, *LPANI_CURDIR;



/* RIFF 块类型ID定义 */
#define RIFF_STR  "RIFF"
#define LIST_STR  "LIST"

#define ACON_STR  "ACON"
#define INFO_STR  "INFO"
#define INAM_STR  "INAM"
#define IART_STR  "IART"

#define FRAM_STR  "fram"
#define ANIH_STR  "anih"
#define RATE_STR  "rate"
#define SEQ_STR   "seq "
#define ICON_STR  "icon"

#define NULL_STR  "\0\0\0\0"


typedef enum {
  ANICHUNK_OUT = 0,
  ANICHUNK_RIFF,
  ANICHUNK_LIST,
  ANICHUNK_INAM,
  ANICHUNK_IART,
  ANICHUNK_ANIH,
  ANICHUNK_RATE,
  ANICHUNK_SEQ,
  ANICHUNK_ICON,
  ANICHUNK_UNKONW       /* 未知类型 */
} ANICHUNK, *LPANICHUNK;

/* RIFF 类型串长度 */
#define FORMTYPESEIZE 4


/* ANI文件头结构 */
typedef struct _tagAnihead {
  unsigned char riff[4];
  unsigned long imgsize;
  unsigned char acon[4];
} ANIHEAD, PANIHEAD;


/* ANI通用块头部结构（标记+块大小）*/
typedef  struct  _tagAnitag {
  char      ck_id[4]; /* 块标记 */
  unsigned long ck_size;  /* 块大小（字节计） */
} ANITAG, *PANITAG;


/* ANI文件信息数据区“anih”块结构 */
typedef  struct  _tagAniheader {
  unsigned long cbSizeof;   /* 块大小总是36字节 */
  unsigned long cFrames;    /* 保存的图象桢数 */
  unsigned long cSteps;     /* 显示的图象桢数 */
  unsigned long cx;       /* 图象宽度 */
  unsigned long cy;       /* 图象高度 */
  unsigned long cBitCount;    /* 颜色位数 */
  unsigned long cPlanes;
  unsigned long jifRate;    /* JIF速率 */
  unsigned long fl;       /* AF_ICON/AF_SEQUENCE设置标记 */
} ANIHEADER, *PANIHEADER;


/* 将60分之一秒的单位数据转为千分之一秒的数据 */
#define   FPS_TO_MS(r)    (r*(1000/60))


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 判断指定的数据流是否是有效的ANI数据 */
int CALLAGREEMENT _is_ani(unsigned char* pani)
{
  assert(pani);

  if ((memcmp((const char*)pani, (const char*)RIFF_STR, FORMTYPESEIZE) == 0) && \
      (memcmp((const char*)(pani + FORMTYPESEIZE + sizeof(unsigned long)), (const char*)ACON_STR, FORMTYPESEIZE) == 0)) {
    return 0; /* 是ANI数据流 */
  }
  else {
    return -1;  /* 非ANI数据 */
  }
}

/* 转换块标示串为标示符 */
int CALLAGREEMENT _str_to_value(unsigned char* pstr)
{
  assert(pstr);

  if (memcmp((const char*)pstr, (const char*)ICON_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_ICON;
  }
  else if (memcmp((const char*)pstr, (const char*)LIST_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_LIST;
  }
  else if (memcmp((const char*)pstr, (const char*)ANIH_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_ANIH;
  }
  else if (memcmp((const char*)pstr, (const char*)RATE_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_RATE;
  }
  else if (memcmp((const char*)pstr, (const char*)SEQ_STR , FORMTYPESEIZE) == 0) {
    return ANICHUNK_SEQ;
  }
  else if (memcmp((const char*)pstr, (const char*)RIFF_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_RIFF;
  }
  else if (memcmp((const char*)pstr, (const char*)INAM_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_INAM;
  }
  else if (memcmp((const char*)pstr, (const char*)IART_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_IART;
  }
  else if (memcmp((const char*)pstr, (const char*)NULL_STR, FORMTYPESEIZE) == 0) {
    return ANICHUNK_OUT;  /* 超出有效数据范围（到达数据尾部） */
  }
  else {
    return ANICHUNK_UNKONW;  /* 未知的RIFF块标志 */
  }
}


/* 跳过一个块 */
unsigned char* CALLAGREEMENT _jump_and_jump_and_jump(unsigned char* ptag)
{
  int size;

  assert(ptag);

  size = (int) * (unsigned long*)(unsigned char*)(ptag + FORMTYPESEIZE);

  size += size % 2; /* 块尺寸如果是奇数，其后追加一个字节（RIFF格式规定） */

  return (unsigned char*)(ptag + FORMTYPESEIZE + sizeof(unsigned long) + size);
}


/* 获取各颜色分量的值 */
int CALLAGREEMENT _get_mask(int bitcount,
    unsigned long* pb_mask,
    unsigned long* pg_mask,
    unsigned long* pr_mask,
    unsigned long* pa_mask)
{
  int result = 0;

  switch (bitcount) {
  case  1:
  case  4:
  case  8:
    /* 调色板位图无掩码 */
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;

  case  16:   /* 555 格式 */
    *pr_mask = 0x7c00;
    *pg_mask = 0x3e0;
    *pb_mask = 0x1f;
    *pa_mask = 0;
    break;

  case  24:
    *pr_mask  = 0xff0000;
    *pg_mask  = 0xff00;
    *pb_mask  = 0xff;
    *pa_mask  = 0x0;
    break;

  case  32:   /* 888 格式 */
    *pr_mask  = 0xff0000;
    *pg_mask  = 0xff00;
    *pb_mask  = 0xff;
    *pa_mask  = 0x0;
    break;

  default:    /* 非法格式 */
    result = -1;
    *pr_mask = 0;
    *pg_mask = 0;
    *pb_mask = 0;
    *pa_mask = 0;
    break;
  }

  return result;  // 0-成功， -1－非法的图像格式
}

// 合成一个扫描行的XOR图和AND图，结果保存在XOR图中 
int CALLAGREEMENT _ani_compose_img(int bitcount, int width, const unsigned char* pxor, const unsigned char* pand, int cn, uchar* p_bit_data)
{
  unsigned char buf[32], tmp, isc = 0;
  const uchar* p = pxor;
  int i, j, k, l, bytesize;

  assert(bitcount > 0);
  assert(pxor && pand);

  // 一次处理 8 个象素
  for (i = 0, k = 0; i < width; i += 8, k++) {
    // 取得 8 个象素的AND数据（共 1 个字节）
    tmp = *(pand + k);

    // 将AND数据转为XOR象素格式
    switch (bitcount) {
    case  1:
      buf[0] = tmp;
      bytesize = 1; // 实际要操作的字节数
      break;

    case  4:
      for (l = 0; l < 8; l += 2) {
        buf[l / 2]  = ((tmp >> (7 - l)) & 0x1) ? 0xf0 : 0x0;
        buf[l / 2] |= ((tmp >> (7 - l - 1)) & 0x1) ? 0xf : 0x0;
      }

      bytesize = 4;
      break;

    case  8:
      for (l = 0; l < 8; l++) {
        buf[l] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
      }

      bytesize = ((width - i) < 8) ? (width - i) : 8;
      break;

    case  16:
      for (l = 0; l < 8; l++) {
        buf[l * 2] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 2 + 1] = buf[l * 2] & 0x7f; /* 555格式 */
      }

      bytesize = ((width - i) < 8) ? (width - i) * 2 : 16;
      break;

    case  24:
      for (l = 0; l < 8; l++) {
        buf[l * 3] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 3 + 1] = buf[l * 3];
        buf[l * 3 + 2] = buf[l * 3];
      }

      bytesize = ((width - i) < 8) ? (width - i) * 3 : 24;
      break;

    case  32:
      for (l = 0; l < 8; l++) {
        buf[l * 4] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 4 + 1] = buf[l * 4];
        buf[l * 4 + 2] = buf[l * 4];
        buf[l * 4 + 3] = 0;     /* 888格式 */
      }

      bytesize = ((width - i) < 8) ? (width - i) * 4 : 32;
      break;

    default:
      return -1;              /* 不支持的位深度 */
      break;
    }

    /* 合成图象（一次合成 8 个象素）*/
    for (j = 0; j < bytesize; j++) {
      buf[j] = p[j] ^ buf[j];
    }
  }

  return 0; /* 返回 0 表示成功，返回非 0 值表示失败 */
}

/* 将光标数据块转为数据包块 */
CC_INLINE int _ani_conv_image_block(LPBITMAPINFOHEADER lpbmi, INFOSTR* pinfo, int mark, int cn, uchar* p_bit_data)
{
  LPBITMAPINFO  pbi = (LPBITMAPINFO)lpbmi;

  unsigned char* p, *pxor, *pand;
  unsigned long pal_num = 0;
  int       i, linesize, linesize2;

  assert(lpbmi);
  assert((mark == ANI_CUR_PRI_IMAGE) || (mark == ANI_CUR_SUB_IMAGE));

  if (mark == ANI_CUR_PRI_IMAGE) {
    /* 设置调色板数据 */
    if (lpbmi->biBitCount <= 8) {
      pinfo->pal_count = 1UL << pinfo->bitcount;

      pal_num = (lpbmi->biClrUsed == 0) ? pinfo->pal_count : lpbmi->biClrUsed;

      if (pal_num > pinfo->pal_count) {
        return ER_BADIMAGE;   /* 图象受损 */
      }

      memmove((void*)(pinfo->palette), (const void*)(pbi->bmiColors), sizeof(RGBQUAD)*pal_num);
    }
    else {
      pinfo->pal_count = 0;
    }

    /* XOR象素扫描行尺寸 */
    linesize  = xpm_calcu_scanline_size(pinfo->width, pinfo->bitcount);
    /* AND象素扫描行尺寸 */
    linesize2 = xpm_calcu_scanline_size(pinfo->width, 1);

    /* 计算XOR象素数据首地址 */
    p = (unsigned char*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(RGBQUAD);

    /* 复制XOR象素数据 */
    //memmove((void*)p_bit_data, (const void *)p, (linesize*pinfo->height));

    /* 取得 XOR 象素首地址 */
    pxor = (unsigned char*)p;
    /* 计算 AND 象素首地址 */
    pand = p + linesize * pinfo->height;

    /* 合成图象 */
    for (i = 0; i < (int)pinfo->height; i++) {
      if (_ani_compose_img(pinfo->bitcount, pinfo->width, pxor, pand, cn, p_bit_data) != 0) {
        return ER_BADIMAGE;   /* 受损的图象 */
      }

      pxor += linesize;
      pand += linesize2;
    }
  }
  else {
    /* 填写图像信息 */
    int width   = (int)lpbmi->biWidth;
    int height  = (int)lpbmi->biHeight / 2; /* XOR和AND 图总高度 */
    int order   = 1;  /* 总是倒向图 */
    int bitcount  = (int)lpbmi->biBitCount;
    unsigned long b_mask, g_mask, r_mask, a_mask;
    int left = 0, top = 0;
    int colorkey;
    int pal_count = 0;
    unsigned long palette[MAX_PALETTE_COUNT];

    if (_get_mask(bitcount,
        (unsigned long*)(&b_mask),
        (unsigned long*)(&g_mask),
        (unsigned long*)(&r_mask),
        (unsigned long*)(&a_mask)) == -1) {
      return ER_BADIMAGE;   /* 图象受损 */
    }

    colorkey = -1;  /* 定为无关键色 */

    /* 设置调色板数据 */
    if (lpbmi->biBitCount <= 8) {
      pal_count = 1UL << bitcount;

      pal_num = (lpbmi->biClrUsed == 0) ? pal_count : lpbmi->biClrUsed;

      if (pal_num > (unsigned long)pal_count) {
        return ER_BADIMAGE;   /* 图象受损 */
      }

      memmove((void*)(palette), (const void*)(pbi->bmiColors), sizeof(RGBQUAD)*pal_num);
    }
    else {
      pal_count = 0;
    }

    /* 取得XOR象素扫描行尺寸 */
    linesize  = xpm_calcu_scanline_size(width, bitcount);
    /* 计算AND象素扫描行尺寸 */
    linesize2 = xpm_calcu_scanline_size(width, 1);

    /* 计算象素数据首地址 */
    p = (unsigned char*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(RGBQUAD);

    /* 复制象素数据 */
    memmove((void*)p_bit_data, (const void*)p, (linesize * height));

    /* 计算 XOR 象素首地址 */
    pxor = (unsigned char*)p_bit_data;
    /* 计算 AND 象素首地址 */
    pand = p + linesize * height;

    /* 合成图象（使用XOR数据与AND数据）*/
    for (i = 0; i < height; i++) {
      if (_ani_compose_img(bitcount, width, pxor, pand, cn, p_bit_data) != 0) {
        return ER_BADIMAGE;   /* 受损的图象 */
      }

      pxor += linesize;
      pand += linesize2;
    }
  }

  return ER_SUCCESS;    /* 返回 0 成功，非 0 失败 */
}

/* 设置图象显示延迟参数 */
int CALLAGREEMENT _set_rate(int index, unsigned long rate, INFOSTR* pinfo_str)
{
#if 0

  if (index >= (int)pinfo_str->imgnumbers) {
    return -1;    /* 非法的数据 */
  }

  if (index == 0) { /* 首帧图象 */
    pinfo_str->time = FPS_TO_MS(rate);
  }
  else {      /* 子图象 */
    for (lpsub = pinfo_str->psubimg; lpsub != 0; lpsub = lpsub->next) {
      if (lpsub->number != index) {
        continue;
      }
      else {
        lpsub->time = FPS_TO_MS(rate);
        break;
      }
    }

    assert(lpsub);  /* 不可能出现找不到对应子图象的情况 */
  }

#endif

  return ER_SUCCESS;
}


/* 读取图像位数据 */
CC_INLINE int CALLAGREEMENT ani_load(ISFILE* pfile, int cn_req, img_t* im)
{
  unsigned char* pani  = 0;
  unsigned long ani_len;
  INFOSTR pinfo_str[1] = {0};
  int b_status = ER_SUCCESS;
  int       mark = 0, imgcount = 0, i, imgnum = 0;
  unsigned long ratecount;

  int     type;
  ANIHEADER     aniheader;
  LPANI_CURDIR    pdirect;
  LPBITMAPINFOHEADER  pbmi;

  /* 获得ANI数据流长度 */
  if ((ani_len = isio_length(pfile)) < (FORMTYPESEIZE * 4)) {
    b_status = ER_NONIMAGE;
    return b_status;
  }

  /* 定位于文件头 */
  if (isio_seek(pfile, 0, SEEK_SET) == -1) {
    b_status = ER_FILERWERR;
    return b_status;
  }

  /* 申请内存以存放完整的ANI图象数据（+8是为了防止解码函数读操作越界） */
  if ((pani = (unsigned char*)isirw_malloc(ani_len + 8)) == 0) {
    b_status = ER_MEMORYERR;
    return b_status;  /* 内存不足 */
  }

  memset((void*)(pani + ani_len), 0, 8);

  /* 读取完整的ANI数据到内存中 */
  if (isio_read((void*)pani, ani_len, 1, pfile) == 0) {
    b_status = ER_FILERWERR;
    return b_status;
  }

  /* 判断是否是ANI图象 */
  if (_is_ani(pani) != 0) {
    b_status = ER_NONIMAGE;
    return b_status;
  }

  /* 定位于数据区（跨过RIFF标志首部） */
  pani = (unsigned char*)(pani + FORMTYPESEIZE + sizeof(unsigned long) + FORMTYPESEIZE);

  /* 提取第一幅光标的概要信息 */
  {
    int   mark = 0;
    int     type;
    ANIHEADER     aniheader;
    LPANI_CURDIR    pdirect;
    LPBITMAPINFOHEADER  pbmi;

    assert((pani != 0));

    while ((type = _str_to_value(pani)) != ANICHUNK_OUT) {
      switch (type) {
      case  ANICHUNK_RIFF:
        b_status = ER_BADIMAGE; /* 非法数据。pani已指向RIFF块内部，所以不应再出现RIFF块标志 */
        break;

      case  ANICHUNK_LIST:
        pani += FORMTYPESEIZE + sizeof(unsigned long) + FORMTYPESEIZE;
        break;

      case  ANICHUNK_INAM:
      case  ANICHUNK_IART:
      case  ANICHUNK_RATE:
      case  ANICHUNK_SEQ:
      case  ANICHUNK_UNKONW:
        pani = _jump_and_jump_and_jump(pani);
        break;

      case  ANICHUNK_ANIH:
        if ((mark & 0x2) != 0) {
          b_status = ER_BADIMAGE;     /* 只能存在一个ANIH块 */
        }
        else {
          mark |= 0x2;
        }

        memcpy((void*)&aniheader, (const void*)(pani + FORMTYPESEIZE + sizeof(unsigned long)), sizeof(ANIHEADER));

        pani = _jump_and_jump_and_jump(pani);
        break;

      case  ANICHUNK_ICON:
        if ((mark & 0x1) == 0) {
          /* 只提取第一幅光标图象信息 */
          /* 这里可能是光标数据，也可能是图标数据，但我们不使用这些数据，而是 */
          /* 直接读取后面BMPINFOHEADER中的参数。所以不做区分 */
          pdirect = (LPANI_CURDIR)(unsigned char*)(pani + FORMTYPESEIZE + sizeof(unsigned long));
          pbmi = (LPBITMAPINFOHEADER)(unsigned char*)(((unsigned char*)pdirect) + pdirect->entries[0].image_offset);

          if (pbmi->biSize != sizeof(BITMAPINFOHEADER)) {
            b_status = ER_BADIMAGE;   /* 非法数据 */
            break;
          }

          /* 提取第一幅图像的信息填入数据包主帧 */
          pinfo_str->width  = (unsigned long)pbmi->biWidth;
          pinfo_str->height = (unsigned long)pbmi->biHeight / 2; /* XOR和AND 图总高度 */
          pinfo_str->order  = 1;  /* 总是倒向图 */
          pinfo_str->bitcount = (unsigned long)pbmi->biBitCount;

          if ((pinfo_str->width == 0) || (pinfo_str->height == 0)) {
            b_status = ER_BADIMAGE;
            break;
          }

          if (_get_mask(pinfo_str->bitcount,
              (unsigned long*)(&pinfo_str->b_mask),
              (unsigned long*)(&pinfo_str->g_mask),
              (unsigned long*)(&pinfo_str->r_mask),
              (unsigned long*)(&pinfo_str->a_mask)) == -1) {
            b_status = ER_BADIMAGE;
            break;
          }

          mark |= 0x1;  /* 不再提取后续图象信息 */
        }

        pani = _jump_and_jump_and_jump(pani);
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;   /* 系统异常 */
        break;
      }
    }

    b_status = (mark == 3) ? ER_SUCCESS : ER_BADIMAGE;  /* 成功 */
  }

  if (b_status != ER_SUCCESS) {
    return b_status;
  }

  /* 提取光标数据 */
  assert((pani != 0) && (pinfo_str != 0));

  while ((type = _str_to_value(pani)) != ANICHUNK_OUT) {
    switch (type) {
    case  ANICHUNK_RIFF:
      b_status = ER_BADIMAGE; /* 非法数据。pani已指向RIFF块内部，所以不应再出现RIFF块标志 */
      return b_status;

    case  ANICHUNK_LIST:
      pani += FORMTYPESEIZE + sizeof(unsigned long) + FORMTYPESEIZE;
      break;

    case  ANICHUNK_RATE:
      ratecount = *(unsigned long*)(unsigned char*)(pani + FORMTYPESEIZE);
      /* 分配播放速率表 */

      //rate = (unsigned long *)isirw_malloc(ratecount);
      /* 复制表内容 */
      //memcpy((void*)rate, (const void *)(unsigned char *)(pani+FORMTYPESEIZE+sizeof(unsigned long)), ratecount);

      /* 转换为元素单位 */
      ratecount /= sizeof(unsigned long);

      pani = _jump_and_jump_and_jump(pani);
      break;

    case  ANICHUNK_SEQ:
#if 0
      seqcount = *(unsigned long*)(unsigned char*)(pani + FORMTYPESEIZE);
      /* 分配播放顺序表 */
      pinfo_str->play_order = (unsigned long*)isirw_malloc(seqcount);

      if (pinfo_str->play_order == 0) {
        b_status = ER_MEMORYERR;
        return b_status;      /* 内存不足 */
      }

      /* 复制表内容 */
      memcpy((void*)pinfo_str->play_order, (const void*)(unsigned char*)(pani + FORMTYPESEIZE + sizeof(unsigned long)), seqcount);

      /* 转换为元素单位 */
      seqcount /= sizeof(unsigned long);
      /* 播放表中元素个数 */
      pinfo_str->play_number = seqcount;
#endif
      pani = _jump_and_jump_and_jump(pani);
      break;

    case  ANICHUNK_INAM:    /* 暂时忽略图象描述信息 */
    case  ANICHUNK_IART:
    case  ANICHUNK_UNKONW:
      pani = _jump_and_jump_and_jump(pani);
      break;

    case  ANICHUNK_ANIH:
      if ((mark & 0x2) != 0) {
        b_status = ER_BADIMAGE;     /* 只能存在一个ANIH块 */
        return b_status;
      }
      else {
        mark |= 0x2;
      }

      memcpy((void*)&aniheader, (const void*)(pani + FORMTYPESEIZE + sizeof(unsigned long)), sizeof(ANIHEADER));

      if ((aniheader.fl & 0x1) == 0) {
        b_status = ER_BADIMAGE;     /* 本插件将不处理以裸数据（RAW）方式存储图象的ANI */
        return b_status;
      }

      pani = _jump_and_jump_and_jump(pani);
      break;

    case  ANICHUNK_ICON:
      if ((mark & 0x2) == 0) {
        b_status = ER_BADIMAGE;     /* anih 块必须在ICON块的前面，否则视为非法数据 */
        return b_status;
      }

      pdirect = (LPANI_CURDIR)(unsigned char*)(pani + FORMTYPESEIZE + sizeof(unsigned long));
      pbmi = (LPBITMAPINFOHEADER)(unsigned char*)(((unsigned char*)pdirect) + pdirect->entries[0].image_offset);

      if (pbmi->biSize != sizeof(BITMAPINFOHEADER)) {
        b_status = ER_BADIMAGE;   /* 非法数据 */
        return b_status;
      }

      imgcount = pdirect->count;

      if (imgcount < 1) { /* 至少应有一幅图象 */
        b_status = ER_BADIMAGE;
        return b_status;
      }

      imsetsize(im, pinfo_str->height, pinfo_str->width, cn_req, imgcount);

      if ((mark & 0x1) == 0) { /* 提取首帧光标图象 */
        if ((b_status = _ani_conv_image_block(pbmi, pinfo_str, ANI_CUR_PRI_IMAGE, cn_req, im->tt.data))!=0) {
          return b_status;
        }

        imgnum = 0;   /* 总图象个数计数 */
        mark |= 0x1;
      }

      /* 读入子图象 */
      for (i = 0; i < imgcount; ++i) {
        /* 计算图象数据地址 */
        pbmi = (LPBITMAPINFOHEADER)(unsigned char*)(((unsigned char*)pdirect) + pdirect->entries[pdirect->count - imgcount].image_offset);

        /* 转换子图象到数据包中 */
        if ((b_status = _ani_conv_image_block(pbmi, pinfo_str, ANI_CUR_SUB_IMAGE, cn_req, im->tt.data + i * im->h * im->s))!=0) {
          return b_status;
        }
      }

      pani = _jump_and_jump_and_jump(pani);
      break;

    default:
      assert(0);
      b_status = ER_SYSERR;
      return b_status;    /* 系统异常 */
      break;
    }
  }

  if (mark == 3) {
    pinfo_str->imgnumbers = imgnum + 1; /* 图象总个数（包括主帧图象）*/

    /* 设置每帧图象的显示延迟时间 */
    pinfo_str->colorkey = -1;
    b_status = ER_SUCCESS;  /* 成功 */
  }
  else {
    b_status = ER_BADIMAGE; /* 图象数据受损 */
    return b_status;
  }


  return b_status;
}


#define ani_load_filename(fname, cn_req, im, pbpp, palette) ani_load_file_close(fopen(fname, "rb"), cn_req, im, pbpp, palette)
CC_INLINE int ani_load_file_close(FILE* pf, int cn_req, img_t* im, int* pbpp, uchar* palette)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = ani_load(s, cn_req, im);
    fclose(pf);
  }

  return ret;
}

