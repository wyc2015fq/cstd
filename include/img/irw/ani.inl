/********************************************************************
 ani.c
 本文件用途： ANI图像读写模块实现文件
 读取功能：1、4、8、16、24、32位ANI图象
 保存功能：不支持
 本文件编写人：
 YZ
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2、引入内存防护概念。
 2002-3 发布第一个版本（新版，支持ISeeIO系统）
********************************************************************/
#ifndef WIN32
#if defined(_WIN32)||defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */
/* 光标类型定义 */
#define ANI_CURTYPE 2
/* 主、子图象类型定义 */
enum ANI_CUR_DATA_TYPE {
  ANI_CUR_PRI_IMAGE,
  ANI_CUR_SUB_IMAGE
};
#pragma pack(1)
/* 光标的图象块信息头结构 */
typedef struct {
  uchar width; /* 图象宽度 */
  uchar height; /* 图象高度 */
  uchar color_count; /* 颜色个数(如果位深度大于8时为0) */
  uchar reserved;
  ushort x_hotspot; /* 热点X坐标 */
  ushort y_hotspot; /* 热点Y坐标 */
  ulong bytes_in_res; /* 图象数据在资源中所占的字节数 */
  ulong image_offset; /* 图象数据的偏移 */
} ANI_CURDIRENTRY, *LPANI_CURDIRENTRY;
/* 光标信息头结构 */
typedef struct {
  ushort reserved; /* 保留 */
  ushort type; /* 资源类型(CURTYPE为光标) */
  ushort count; /* 图象个数 */
  ANI_CURDIRENTRY entries[1]; /* 每一个图象的入口 */
} ANI_CURDIR, *LPANI_CURDIR;
#pragma pack()
/* RIFF 块类型ID定义 */
#define RIFF_STR "RIFF"
#define LIST_STR "LIST"
#define ACON_STR "ACON"
#define INFO_STR "INFO"
#define INAM_STR "INAM"
#define IART_STR "IART"
#define FRAM_STR "fram"
#define ANIH_STR "anih"
#define RATE_STR "rate"
#define SEQ_STR "seq "
#define ICON_STR "icon"
#define NULL_STR "\0\0\0\0"
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
  ANICHUNK_UNKONW /* 未知类型 */
} ANICHUNK, *LPANICHUNK;
/* RIFF 类型串长度 */
#define FORMTYPESEIZE 4
/* ANI文件头结构 */
typedef struct _tagAnihead {
  uchar riff[4];
  ulong imgsize;
  uchar acon[4];
} ANIHEAD, PANIHEAD;
/* ANI通用块头部结构（标记+块大小）*/
typedef struct _tagAnitag {
  char ck_id[4]; /* 块标记 */
  ulong ck_size; /* 块大小（字节计） */
} ANITAG, *PANITAG;
/* ANI文件信息数据区“anih”块结构 */
typedef struct _tagAniheader {
  ulong cbSizeof; /* 块大小总是36字节 */
  ulong cFrames; /* 保存的图象桢数 */
  ulong cSteps; /* 显示的图象桢数 */
  ulong cx; /* 图象宽度 */
  ulong cy; /* 图象高度 */
  ulong cBitCount; /* 颜色位数 */
  ulong cPlanes;
  ulong jifRate; /* JIF速率 */
  ulong fl; /* AF_ICON/AF_SEQUENCE设置标记 */
} ANIHEADER, *PANIHEADER;
/* 将60分之一秒的单位数据转为千分之一秒的数据 */
#define FPS_TO_MS(r) (r*(1000/60))
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 判断指定的数据流是否是有效的ANI数据 */
int _is_ani(uchar* pani)
{
  assert(pani);
  if ((memcmp((const char*)pani, (const char*)RIFF_STR, FORMTYPESEIZE) == 0) && \
      (memcmp((const char*)(pani + FORMTYPESEIZE + sizeof(ulong)), (const char*)ACON_STR, FORMTYPESEIZE) == 0)) {
    return 0; /* 是ANI数据流 */
  }
  else {
    return -1; /* 非ANI数据 */
  }
}
/* 转换块标示串为标示符 */
ANICHUNK _str_to_value(uchar* pstr)
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
    return ANICHUNK_OUT; /* 超出有效数据范围（到达数据尾部） */
  }
  else {
    return ANICHUNK_UNKONW; /* 未知的RIFF块标志 */
  }
}
/* 跳过一个块 */
uchar* _jump_and_jump_and_jump(uchar* ptag)
{
  int size;
  assert(ptag);
  size = (int) * (ulong*)(uchar*)(ptag + FORMTYPESEIZE);
  size += size % 2; /* 块尺寸如果是奇数，其后追加一个字节（RIFF格式规定） */
  return (uchar*)(ptag + FORMTYPESEIZE + sizeof(ulong) + size);
}
/* 提取首幅光标的概要信息 */
EXERESULT _get_info(uchar* pani, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
  int mark = 0;
  ANICHUNK type;
  ANIHEADER aniheader;
  LPANI_CURDIR pdirect;
  wBITMAPINFOHEADER* pbmi;
  assert((pani != 0) && (pinfo_str != 0));
  while ((type = _str_to_value(pani)) != ANICHUNK_OUT) {
    switch (type) {
    case ANICHUNK_RIFF:
      return ER_BADIMAGE; /* 非法数据。pani已指向RIFF块内部，所以不应再出现RIFF块标志 */
    case ANICHUNK_LIST:
      pani += FORMTYPESEIZE + sizeof(ulong) + FORMTYPESEIZE;
      break;
    case ANICHUNK_INAM:
    case ANICHUNK_IART:
    case ANICHUNK_RATE:
    case ANICHUNK_SEQ:
    case ANICHUNK_UNKONW:
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_ANIH:
      if ((mark & 0x2) != 0) {
        return ER_BADIMAGE; /* 只能存在一个ANIH块 */
      }
      else {
        mark |= 0x2;
      }
      memcpy((void*)&aniheader, (const void*)(pani + FORMTYPESEIZE + sizeof(ulong)), sizeof(ANIHEADER));
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_ICON:
      if ((mark & 0x1) == 0) { /* 只提取第一幅光标图象信息 */
        /* 这里可能是光标数据，也可能是图标数据，但我们不使用这些数据，而是 */
        /* 直接读取后面BMPINFOHEADER中的参数。所以不做区分 */
        pdirect = (LPANI_CURDIR)(uchar*)(pani + FORMTYPESEIZE + sizeof(ulong));
        pbmi = (wBITMAPINFOHEADER*)(uchar*)(((uchar*)pdirect) + pdirect->entries[0].image_offset);
        if (pbmi->biSize != sizeof(wBITMAPINFOHEADER)) {
          return ER_BADIMAGE; /* 非法数据 */
        }
        /* 提取第一幅图像的信息填入数据包主帧 */
        pinfo_str->width = (ulong)pbmi->biWidth;
        pinfo_str->height = (ulong)pbmi->biHeight / 2; /* XOR和AND 图总高度 */
        pinfo_str->order = 1; /* 总是倒向图 */
        pinfo_str->bitcount = (ulong)pbmi->biBitCount;
        if ((pinfo_str->width == 0) || (pinfo_str->height == 0)) {
          return ER_BADIMAGE;
        }
        if (_get_mask(pinfo_str->bitcount,
            (&pinfo_str->b_mask),
            (&pinfo_str->g_mask),
            (&pinfo_str->r_mask),
            (&pinfo_str->a_mask)) == -1) {
          return ER_BADIMAGE;
        }
        mark |= 0x1; /* 不再提取后续图象信息 */
      }
      pani = _jump_and_jump_and_jump(pani);
      break;
    default:
      assert(0);
      return ER_SYSERR; /* 系统异常 */
      break;
    }
  }
  if (mark == 3) {
    return ER_SUCCESS; /* 成功 */
  }
  else {
    return ER_BADIMAGE; /* 图象数据受损 */
  }
}
/* 合成一个扫描行的XOR图和AND图，结果保存在XOR图中 */
int _ani_compose_img(int bitcount, int width, uchar* pxor, uchar* pand)
{
  uchar buf[32], tmp, *p = pxor, isc = 0;
  int i, j, k, l, bytesize;
  assert(bitcount > 0);
  assert(pxor && pand);
  /* 一次处理 8 个象素 */
  for (i = 0, k = 0; i < width; i += 8, k++) {
    /* 取得 8 个象素的AND数据（共 1 个字节） */
    tmp = *(pand + k);
    /* 将AND数据转为XOR象素格式 */
    switch (bitcount) {
    case 1:
      buf[0] = tmp;
      bytesize = 1; /* 实际要操作的字节数 */
      break;
    case 4:
      for (l = 0; l < 8; l += 2) {
        buf[l / 2] = ((tmp >> (7 - l)) & 0x1) ? 0xf0 : 0x0;
        buf[l / 2] |= ((tmp >> (7 - l - 1)) & 0x1) ? 0xf : 0x0;
      }
      bytesize = 4;
      break;
    case 8:
      for (l = 0; l < 8; l++) {
        buf[l] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
      }
      bytesize = ((width - i) < 8) ? (width - i) : 8;
      break;
    case 16:
      for (l = 0; l < 8; l++) {
        buf[l * 2] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 2 + 1] = buf[l * 2] & 0x7f; /* 555格式 */
      }
      bytesize = ((width - i) < 8) ? (width - i) * 2 : 16;
      break;
    case 24:
      for (l = 0; l < 8; l++) {
        buf[l * 3] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 3 + 1] = buf[l * 3];
        buf[l * 3 + 2] = buf[l * 3];
      }
      bytesize = ((width - i) < 8) ? (width - i) * 3 : 24;
      break;
    case 32:
      for (l = 0; l < 8; l++) {
        buf[l * 4] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 4 + 1] = buf[l * 4];
        buf[l * 4 + 2] = buf[l * 4];
        buf[l * 4 + 3] = 0; /* 888格式 */
      }
      bytesize = ((width - i) < 8) ? (width - i) * 4 : 32;
      break;
    default:
      return -1; /* 不支持的位深度 */
      break;
    }
    /* 合成图象（一次合成 8 个象素）*/
    for (j = 0; j < bytesize; j++) {
      *p++ ^= buf[j];
    }
  }
  return 0; /* 返回 0 表示成功，返回非 0 值表示失败 */
}
/* 将光标数据块转为数据包块 */
int _ani_conv_image_block(wBITMAPINFOHEADER* lpbmi, void* lpdest, enum ANI_CUR_DATA_TYPE mark)
{
  INFOSTR* pinfo = (INFOSTR*)0;
  SUBIMGBLOCK* psubinfo = (SUBIMGBLOCK*)0;
  wBITMAPINFO* pbi = (wBITMAPINFO*)lpbmi;
  uchar* p, *pxor, *pand;
  ulong pal_num = 0;
  int i, linesize, linesize2;
  assert((lpbmi) && (lpdest));
  assert((mark == ANI_CUR_PRI_IMAGE) || (mark == ANI_CUR_SUB_IMAGE));
  if (mark == ANI_CUR_PRI_IMAGE) {
    pinfo = (INFOSTR*)lpdest;
    /* 设置调色板数据 */
    if (lpbmi->biBitCount <= 8) {
      pinfo->pal_count = 1UL << pinfo->bitcount;
      pal_num = (lpbmi->biClrUsed == 0) ? pinfo->pal_count : lpbmi->biClrUsed;
      if (pal_num > pinfo->pal_count) {
        return -2; /* 图象受损 */
      }
      memmove((void*)(pinfo->palette), (const void*)(pbi->bmiColors), sizeof(wRGBQUAD)*pal_num);
    }
    else {
      pinfo->pal_count = 0;
    }
    /* XOR象素扫描行尺寸 */
    linesize = _calcu_scanline_size(pinfo->width, pinfo->bitcount);
    /* AND象素扫描行尺寸 */
    linesize2 = _calcu_scanline_size(pinfo->width, 1);
    assert(pinfo->p_bit_data == NULL);
    /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
    pinfo->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo->height + 4);
    if (!pinfo->p_bit_data) {
      return -3; /* 内存不足 */
    }
    assert(pinfo->pp_line_addr == NULL);
    /* 分配行首地址数组 */
    pinfo->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo->height);
    if (!pinfo->pp_line_addr) {
      isirw_free(pinfo->p_bit_data);
      pinfo->p_bit_data = 0;
      return -3; /* 内存不足 */
    }
    /* 初始化行首地址数组（倒向） */
    for (i = 0; i < (int)(pinfo->height); i++) {
      pinfo->pp_line_addr[i] = (pinfo->p_bit_data + ((pinfo->height - i - 1) * linesize));
    }
    /* 计算XOR象素数据首地址 */
    p = (uchar*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(wRGBQUAD);
    /* 复制XOR象素数据 */
    memmove((void*)pinfo->p_bit_data, (const void*)p, (linesize * pinfo->height));
    /* 取得 XOR 象素首地址 */
    pxor = (uchar*)pinfo->p_bit_data;
    /* 计算 AND 象素首地址 */
    pand = p + linesize * pinfo->height;
    /* 合成图象 */
    for (i = 0; i < (int)pinfo->height; i++) {
      if (_ani_compose_img(pinfo->bitcount, pinfo->width, pxor, pand) != 0) {
        isirw_free(pinfo->p_bit_data);
        pinfo->p_bit_data = 0;
        isirw_free(pinfo->pp_line_addr);
        pinfo->pp_line_addr = 0;
        return -2; /* 受损的图象 */
      }
      pxor += linesize;
      pand += linesize2;
    }
  }
  else {
    psubinfo = (SUBIMGBLOCK*)lpdest;
    /* 填写图像信息 */
    psubinfo->width = (int)lpbmi->biWidth;
    psubinfo->height = (int)lpbmi->biHeight / 2; /* XOR和AND 图总高度 */
    psubinfo->order = 1; /* 总是倒向图 */
    psubinfo->bitcount = (int)lpbmi->biBitCount;
    if (_get_mask(psubinfo->bitcount,
        (ulong*)(&psubinfo->b_mask),
        (ulong*)(&psubinfo->g_mask),
        (ulong*)(&psubinfo->r_mask),
        (ulong*)(&psubinfo->a_mask)) == -1) {
      return -2; /* 图象受损 */
    }
    psubinfo->left = psubinfo->top = 0;
    psubinfo->dowith = psubinfo->userinput = psubinfo->time = 0;
    psubinfo->colorkey = -1; /* 定为无关键色 */
    /* 设置调色板数据 */
    if (lpbmi->biBitCount <= 8) {
      psubinfo->pal_count = 1UL << psubinfo->bitcount;
      pal_num = (lpbmi->biClrUsed == 0) ? psubinfo->pal_count : lpbmi->biClrUsed;
      if (pal_num > (ulong)psubinfo->pal_count) {
        return -2; /* 图象受损 */
      }
      memmove((void*)(psubinfo->palette), (const void*)(pbi->bmiColors), sizeof(wRGBQUAD)*pal_num);
    }
    else {
      psubinfo->pal_count = 0;
    }
    /* 取得XOR象素扫描行尺寸 */
    linesize = _calcu_scanline_size(psubinfo->width, psubinfo->bitcount);
    /* 计算AND象素扫描行尺寸 */
    linesize2 = _calcu_scanline_size(psubinfo->width, 1);
    assert(psubinfo->p_bit_data == (uchar*)0);
    /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
    psubinfo->p_bit_data = (uchar*)isirw_malloc(linesize * psubinfo->height + 4);
    if (!psubinfo->p_bit_data) {
      return -3; /* 内存不足 */
    }
    assert(psubinfo->pp_line_addr == NULL);
    /* 分配行首地址数组 */
    psubinfo->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * psubinfo->height);
    if (!psubinfo->pp_line_addr) {
      isirw_free(psubinfo->p_bit_data);
      psubinfo->p_bit_data = 0;
      return -3; /* 内存不足 */
    }
    /* 初始化行首地址数组（倒向） */
    for (i = 0; i < (int)(psubinfo->height); i++) {
      psubinfo->pp_line_addr[i] = (psubinfo->p_bit_data + ((psubinfo->height - i - 1) * linesize));
    }
    /* 计算象素数据首地址 */
    p = (uchar*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(wRGBQUAD);
    /* 复制象素数据 */
    memmove((void*)psubinfo->p_bit_data, (const void*)p, (linesize * psubinfo->height));
    /* 计算 XOR 象素首地址 */
    pxor = (uchar*)psubinfo->p_bit_data;
    /* 计算 AND 象素首地址 */
    pand = p + linesize * psubinfo->height;
    /* 合成图象（使用XOR数据与AND数据）*/
    for (i = 0; i < psubinfo->height; i++) {
      if (_ani_compose_img(psubinfo->bitcount, psubinfo->width, pxor, pand) != 0) {
        isirw_free(psubinfo->p_bit_data);
        psubinfo->p_bit_data = 0;
        isirw_free(psubinfo->pp_line_addr);
        psubinfo->pp_line_addr = 0;
        return -2; /* 受损的图象 */
      }
      pxor += linesize;
      pand += linesize2;
    }
  }
  return 0; /* 返回 0 成功，非 0 失败 */
}
/* 设置图象显示延迟参数（只供_get_image函数调用） */
int _set_rate(int index, ulong rate, INFOSTR* pinfo_str)
{
  if (index >= (int)pinfo_str->imgnumbers) {
    return -1; /* 非法的数据 */
  }
  if (index == 0) { /* 首帧图象 */
    pinfo_str->time = FPS_TO_MS(rate);
  }
  else { /* 子图象 */
    int i;
    for (i = 0; i<pinfo_str->imgnumbers; ++i) {
      SUBIMGBLOCK* lpsub = lpsub = pinfo_str->psubimg + i;;
      if (lpsub->number != index) {
        continue;
      }
      else {
        lpsub->time = FPS_TO_MS(rate);
        break;
      }
    }
    assert(i<pinfo_str->imgnumbers); /* 不可能出现找不到对应子图象的情况 */
  }
  return 0;
}
/* 分配一个子图像节点，并初始化节点内数据 */
SUBIMGBLOCK* _ani_alloc_SUBIMGBLOCK(INFOSTR* pinfo_str)
{
  SUBIMGBLOCK* p_subimg = _alloc_SUBIMGBLOCK(pinfo_str);
  if (p_subimg) {
    p_subimg->number = 1;
    p_subimg->colorkey = -1;
  }
  return p_subimg;
}
/* 提取光标数据 */
int _get_image(uchar* pani, INFOSTR* pinfo_str)
{
  int mark = 0, imgcount = 0, i, imgnum = 0;
  ulong seqcount, ratecount;
  ulong* rate = 0; /* 速率表 */
  ANICHUNK type;
  ANIHEADER aniheader;
  LPANI_CURDIR pdirect;
  wBITMAPINFOHEADER* pbmi;
  SUBIMGBLOCK* lpsub = 0;
  assert((pani != 0) && (pinfo_str != 0));
  while ((type = _str_to_value(pani)) != ANICHUNK_OUT) {
    switch (type) {
    case ANICHUNK_RIFF:
      if (rate) {
        isirw_free(rate);
      }
      return -1; /* 非法数据。pani已指向RIFF块内部，所以不应再出现RIFF块标志 */
    case ANICHUNK_LIST:
      pani += FORMTYPESEIZE + sizeof(ulong) + FORMTYPESEIZE;
      break;
    case ANICHUNK_RATE:
      ratecount = *(ulong*)(uchar*)(pani + FORMTYPESEIZE);
      /* 分?洳シ潘俾时?*/
      rate = (ulong*)isirw_malloc(ratecount);
      if (rate == 0) {
        return -3;
      }
      /* 复制表内容 */
      memcpy((void*)rate, (const void*)(uchar*)(pani + FORMTYPESEIZE + sizeof(ulong)), ratecount);
      /* 转换为元素单位 */
      ratecount /= sizeof(ulong);
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_SEQ:
      seqcount = *(ulong*)(uchar*)(pani + FORMTYPESEIZE);
      /* 分配播放顺序表 */
      pinfo_str->play_order = (ulong*)isirw_malloc(seqcount);
      if (pinfo_str->play_order == 0) {
        if (rate) {
          isirw_free(rate);
        }
        return -3; /* 内存不足 */
      }
      /* 复制表内容 */
      memcpy((void*)pinfo_str->play_order, (const void*)(uchar*)(pani + FORMTYPESEIZE + sizeof(ulong)), seqcount);
      /* 转换为元素单位 */
      seqcount /= sizeof(ulong);
      /* 播放表中元素个数 */
      pinfo_str->play_number = seqcount;
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_INAM: /* 暂时忽略图象描述信息 */
    case ANICHUNK_IART:
    case ANICHUNK_UNKONW:
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_ANIH:
      if ((mark & 0x2) != 0) {
        if (rate) {
          isirw_free(rate);
        }
        return -1; /* 只能存在一个ANIH块 */
      }
      else {
        mark |= 0x2;
      }
      memcpy((void*)&aniheader, (const void*)(pani + FORMTYPESEIZE + sizeof(ulong)), sizeof(ANIHEADER));
      if ((aniheader.fl & 0x1) == 0) {
        if (rate) {
          isirw_free(rate);
        }
        return -1; /* 本插件将不处理以裸数据（RAW）方式存储图象的ANI */
      }
      pani = _jump_and_jump_and_jump(pani);
      break;
    case ANICHUNK_ICON:
      if ((mark & 0x2) == 0) {
        if (rate) {
          isirw_free(rate);
        }
        return -1; /* anih 块必须在ICON块的前面，否则视为非法数据 */
      }
      pdirect = (LPANI_CURDIR)(uchar*)(pani + FORMTYPESEIZE + sizeof(ulong));
      pbmi = (wBITMAPINFOHEADER*)(uchar*)(((uchar*)pdirect) + pdirect->entries[0].image_offset);
      if (pbmi->biSize != sizeof(wBITMAPINFOHEADER)) {
        if (rate) {
          isirw_free(rate);
        }
        return -1; /* 非法数据 */
      }
      imgcount = pdirect->count;
      if (imgcount < 1) { /* 至少应有一?枷?*/
        if (rate) {
          isirw_free(rate);
        }
        return -1;
      }
      if ((mark & 0x1) == 0) { /* 提取首帧光标图象 */
        switch (_ani_conv_image_block(pbmi, (void*)pinfo_str, ANI_CUR_PRI_IMAGE)) {
        case -2: /* 光标文件受损 */
          if (rate) {
            isirw_free(rate);
          }
          return -1;
        case -3: /* 内存不足 */
          if (rate) {
            isirw_free(rate);
          }
          return -3;
        case 0: /* 成功 */
          break;
        default:
          if (rate) {
            isirw_free(rate);
          }
          return -2; /* 系统异常 */
        }
        imgcount--; /* 单个光标块中图象个数计数 */
        imgnum = 0; /* 总图象个数计数 */
        mark |= 0x1;
      }
      /* 读入子图象 */
      while (imgcount) {
        if ((lpsub = _ani_alloc_SUBIMGBLOCK(pinfo_str)) == 0) {
          if (rate) {
            isirw_free(rate);
          }
          return -3;
        }
        lpsub->number = ++imgnum; /* 子图像序号由 1 开始 */
        /* 计算图象数据地址 */
        pbmi = (wBITMAPINFOHEADER*)(uchar*)(((uchar*)pdirect) + pdirect->entries[pdirect->count - imgcount].image_offset);
        /* 转换子图象到数据包中 */
        switch (_ani_conv_image_block(pbmi, (void*)lpsub, ANI_CUR_SUB_IMAGE)) {
        case -2: /* 光标文件受损 */
          if (rate) {
            isirw_free(rate);
          }
          return -1;
        case -3: /* 内存不足 */
          if (rate) {
            isirw_free(rate);
          }
          return -3;
        case 0: /* 成功 */
          break;
        default:
          if (rate) {
            isirw_free(rate);
          }
          return -2;
        }
        imgcount--;
      }
      pani = _jump_and_jump_and_jump(pani);
      break;
    default:
      assert(0);
      if (rate) {
        isirw_free(rate);
      }
      return -2; /* 系统异常 */
      break;
    }
  }
  if (mark == 3) {
    pinfo_str->imgnumbers = imgnum + 1; /* 图象总个数（包括主帧图象）*/
    /* 设置每帧图象的显示延迟时间 */
    if (rate) {
      if (pinfo_str->play_order) {
        for (i = 0; i < (int)pinfo_str->play_number; i++) {
          if (_set_rate((int)pinfo_str->play_order[i], rate[i], pinfo_str) != 0) {
            isirw_free(rate);
            return -1;
          }
        }
      }
      else {
        for (i = 0; i < (int)pinfo_str->imgnumbers; i++) {
          if (_set_rate((int)i, rate[i], pinfo_str) != 0) {
            isirw_free(rate);
            return -1;
          }
        }
      }
    }
    else {
      for (i = 0; i < (int)pinfo_str->imgnumbers; i++) {
        if (_set_rate((int)i, aniheader.jifRate, pinfo_str) != 0) {
          return -1;
        }
      }
    }
    pinfo_str->colorkey = -1;
    if (rate) {
      isirw_free(rate);
    }
    /* 转换子图象 */
    return 0; /* 成功 */
  }
  else {
    if (rate) {
      isirw_free(rate);
    }
    return -1; /* 图象数据受损 */
  }
}
/* 获取图像信息 */
EXERESULT ani_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  uchar* pani = 0, *p;
  ulong ani_len;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
  do {
    /* 获得ANI数据流长度 */
    if ((ani_len = isio_length(pfile)) < (FORMTYPESEIZE * 4)) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* 定位于文件头 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 申请内存以存放完整的ANI图象数据（+8是为了防止解码函数读操作越界） */
    if ((pani = (uchar*)isirw_malloc(ani_len + 8)) == 0) {
      b_status = ER_MEMORYERR;
      break; /* 内存不足 */
    }
    memset((void*)(pani + ani_len), 0, 8);
    /* 读取完整的ANI数据到内存中 */
    if (isio_read((void*)pani, ani_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 判断是否是ANI图象 */
    if (_is_ani(pani) != 0) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* 定位于数据区（跨过RIFF标志首部） */
    p = (uchar*)(pani + FORMTYPESEIZE + sizeof(ulong) + FORMTYPESEIZE);
    /* 提取第一幅光标的概要信息 */
    IF_LEAVE(_get_info(p, pinfo_str));
    pinfo_str->imgtype = IMT_RESDYN; /* 图像文件类型 */
    pinfo_str->imgformat = IMF_ANI; /* 图像文件格式（后缀名） */
    pinfo_str->compression = ICS_RGB; /* 图像的压缩方式 */
    /* 设定数据包状态 */
    pinfo_str->data_state = 1;
  }
  while (0);
  if (b_status != ER_SUCCESS) {
    if (pani) {
      isirw_free(pani);
    }
    if (pfile) {
    }
    pinfo_str->data_state = 0;
  }
  return b_status;
}
/* 读取图像位数据 */
EXERESULT ani_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  uchar* pani = 0, *p;
  ulong ani_len;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  do {
    /* 获得ANI数据流长度 */
    if ((ani_len = isio_length(pfile)) < (FORMTYPESEIZE * 4)) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* 定位于文件头 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 申请内存以存放完整的ANI图象数据（+8是为了防止解码函数读操作越界） */
    if ((pani = (uchar*)isirw_malloc(ani_len + 8)) == 0) {
      b_status = ER_MEMORYERR;
      break; /* 内存不足 */
    }
    memset((void*)(pani + ani_len), 0, 8);
    /* 读取完整的ANI数据到内存中 */
    if (isio_read((void*)pani, ani_len, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 判断是否是ANI图象 */
    if (_is_ani(pani) != 0) {
      b_status = ER_NONIMAGE;
      break;
    }
    /* 定位于数据区（跨过RIFF标志首部） */
    p = (uchar*)(pani + FORMTYPESEIZE + sizeof(ulong) + FORMTYPESEIZE);
    /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
    if (pinfo_str->data_state == 0) {
      /* 提取第一幅光标的概要信息 */
      IF_LEAVE(_get_info(p, pinfo_str));
      pinfo_str->imgtype = IMT_RESDYN; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_ANI; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_RGB; /* 图像的压缩方式 */
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    /* 读取图象数据 */
    switch (_get_image(p, pinfo_str)) {
    case 0: /* 成功 */
      break;
    case -1: /* 受损的图象 */
      b_status = ER_BADIMAGE;
      break;
    case -3: /* 内存不足 */
      b_status = ER_MEMORYERR;
      break;
    case -4: /* 用户中断 */
      b_status = ER_USERBREAK;
      break;
    case -2: /* 系统异常 */
    default:
      assert(0);
      b_status = ER_SYSERR;
      break;
    }
    __LEAVE;
    pinfo_str->data_state = 2;
  }
  while (0);
  if (pani) {
    isirw_free(pani);
  }
  if (b_status != ER_SUCCESS) {
    INFOSTR_free(pinfo_str);
  }
  return b_status;
}
/* 保存图像 */
EXERESULT ani_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  b_status = ER_NOTSUPPORT; /* 不支持保存功能 */
  return b_status;
}

