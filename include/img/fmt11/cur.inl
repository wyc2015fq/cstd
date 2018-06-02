// ISee图像浏览器—CUR图像读写模块实现文件
// 读取功能：1、4、8、16、24、32位光标图象
// 保存功能：不支持



/* 光标类型定义 */
#define CURTYPE   2


/* 主、子图象类型定义 */
enum CUR_DATA_TYPE {
  CUR_PRI_IMAGE,
  CUR_SUB_IMAGE
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
} CURDIRENTRY, *LPCURDIRENTRY;


/* 光标信息头结构 */
typedef struct {
  unsigned short  reserved;   /* 保留 */
  unsigned short  type;     /* 资源类型(CURTYPE为光标) */
  unsigned short  count;      /* 图象个数 */
  CURDIRENTRY   entries[1];   /* 每一个图象的入口 */
} CURDIR, *LPCURDIR;



/*****************************************************************************/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 获取CUR头结构（包括图像个数、图像尺寸等），并进行简单的鉴别 */
int CALLAGREEMENT _read_cur_header(ISFILE* file, CURDIR** cur)
{
  CURDIR    curdir;
  int     infosize, result = 0;
  LPCURDIR  lpir = 0;

  assert((file != 0) && (cur != 0));

  do {
    /* 定位于文件首部 */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = -1;
      break;   /* 文件读写失败 */
    }

    /* 读取光标文件信息头结构 */
    if (isio_read((void*)&curdir, sizeof(CURDIR), 1, file) == 0) {
      result = -4;
      break;   /* 不是一个光标文件 */
    }

    /* 判断文件标志 */
    if ((curdir.reserved != 0) || (curdir.type != CURTYPE)) {
      result = -4;
      break;   /* 不是一个光标文件 */
    }

    /* 至少会存在一个图象 */
    if (curdir.count < 1) {
      result = -2;
      break;   /* 光标文件受损 */
    }


    /* 计算光标信息块的尺寸 */
    infosize = sizeof(CURDIR) + (curdir.count - 1) * sizeof(CURDIRENTRY);

    /* 分配用于存放光标信息的缓冲区内存块 */
    if ((lpir = (LPCURDIR)isirw_malloc(infosize)) == 0) {
      result = -3;
      break;   /* 内存不足 */
    }

    /* 读取光标信息 */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = -1;
      break;
    }

    if (isio_read((void*)lpir, infosize, 1, file) == 0) {
      result = -2;
      break;   /* 光标文件受损（也有可能是文件读写错，但图象受损的可能性更大） */
    }
  } while(0);
  
  if (result != 0) {
    if (lpir) {
      isirw_free(lpir);
      lpir = 0;/* 当发生错误或有异常时，*cur将被置 0 */
    }
    
    if (result == 0) {
      result = -1;  /* 大多数异常可能都是由文件读写失败引起的 */
    }
  }
  *cur = lpir;  

  return result;  /* 返回 0 成功，非 0 值失败。*cur指向的内存块在不需要时应释放。 */
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

  return result;  /* 0-成功， -1－非法的图像格式 */
}


/* 读取一个光标的象素数据块（包含BITMAPINFOHEADER结构、XOR数据和AND数据）*/
int CALLAGREEMENT _read_image_block(ISFILE* file, unsigned long offset, unsigned long len, unsigned char** image)
{
  unsigned char*  tmp = 0;
  int       result = 0;
  int err = 1;

  assert((file != 0) && (image != 0));

  /* 数据合法性检查 */
  if ((offset == 0) || (len == 0)) {
    return -2;    /* 光标文件受损（数据不合法）*/
  }

  do {
    /* 定位读写位置 */
    if (isio_seek(file, offset, SEEK_SET) == -1) {
      result = -1;
      break;   /* 文件读写失败 */
    }

    if ((tmp = (unsigned char*)isirw_malloc(len)) == 0) {
      result = -3;
      break;   /* 内存不足 */
    }

    /* 读取图象数据 */
    if (isio_read((void*)tmp, len, 1, file) == 0) {
      result = -2;
      break;   /* 光标文件受损 */
    }

    if (((wBITMAPINFOHEADER*)tmp)->biSize != sizeof(wBITMAPINFOHEADER)) {
      result = -2;
      break;   /* 光标文件受损 */
    }
    err = 0;
  } while(0);

  if (err) {
    if (result != 0) {
      if (tmp) {
        isirw_free(tmp);
        tmp = 0;
      }

      if (result == 0) {
        result = -1;      /* 文件读写失败 */
      }
    }
  }

  *image = tmp;

  return result;  /* 返回 0 成功，非 0 值失败。*tmp指向的内存块在不需要时应释放。 */
}


/* 将光标数据块转为数据包块 */
int CALLAGREEMENT _conv_image_block(wBITMAPINFOHEADER* lpbmi, void* lpdest, enum CUR_DATA_TYPE mark)
{
  LPINFOSTR   pinfo = (LPINFOSTR)0;
  LPSUBIMGBLOCK psubinfo = (LPSUBIMGBLOCK)0;
  LPBITMAPINFO  pbi = (LPBITMAPINFO)lpbmi;

  unsigned char* p, *pxor, *pand;
  unsigned long pal_num = 0;
  int       i, linesize, linesize2;

  assert((lpbmi) && (lpdest));
  assert((mark == CUR_PRI_IMAGE) || (mark == CUR_SUB_IMAGE));

  if (mark == CUR_PRI_IMAGE) {
    pinfo = (LPINFOSTR)lpdest;

    /* 设置调色板数据 */
    if (lpbmi->biBitCount <= 8) {
      pinfo->pal_count = 1UL << pinfo->bitcount;

      pal_num = (lpbmi->biClrUsed == 0) ? pinfo->pal_count : lpbmi->biClrUsed;

      if (pal_num > pinfo->pal_count) {
        return -2;    /* 图象受损 */
      }

      memmove((void*)(pinfo->palette), (const void*)(pbi->bmiColors), sizeof(RGBQUAD)*pal_num);
    }
    else {
      pinfo->pal_count = 0;
    }

    /* XOR象素扫描行尺寸 */
    linesize  = _calcu_scanline_size(pinfo->width, pinfo->bitcount);
    /* AND象素扫描行尺寸 */
    linesize2 = _calcu_scanline_size(pinfo->width, 1);

    assert(pinfo->p_bit_data == (unsigned char*)0);

    /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
    pinfo->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo->height + 4);

    if (!pinfo->p_bit_data) {
      return -3;      /* 内存不足 */
    }

    assert(pinfo->pp_line_addr == (void**)0);
    /* 分配行首地址数组 */
    pinfo->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * pinfo->height);

    if (!pinfo->pp_line_addr) {
      isirw_free(pinfo->p_bit_data);
      pinfo->p_bit_data = 0;
      return -3;      /* 内存不足 */
    }

    /* 初始化行首地址数组（倒向） */
    for (i = 0; i < (int)(pinfo->height); i++) {
      pinfo->pp_line_addr[i] = (void*)(pinfo->p_bit_data + ((pinfo->height - i - 1) * linesize));
    }

    /* 计算XOR象素数据首地址 */
    p = (unsigned char*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(RGBQUAD);

    /* 复制XOR象素数据 */
    memmove((void*)pinfo->p_bit_data, (const void*)p, (linesize * pinfo->height));

    /* 取得 XOR 象素首地址 */
    pxor = (unsigned char*)pinfo->p_bit_data;
    /* 计算 AND 象素首地址 */
    pand = p + linesize * pinfo->height;

    /* 合成图象 */
    for (i = 0; i < (int)pinfo->height; i++) {
      if (_compose_img(pinfo->bitcount, pinfo->width, pxor, pand) != 0) {
        isirw_free(pinfo->p_bit_data);
        pinfo->p_bit_data = 0;
        isirw_free(pinfo->pp_line_addr);
        pinfo->pp_line_addr = 0;

        return -2;    /* 受损的图象 */
      }

      pxor += linesize;
      pand += linesize2;
    }
  }
  else {
    psubinfo = (LPSUBIMGBLOCK)lpdest;

    /* 填写图像信息 */
    psubinfo->width   = (int)lpbmi->biWidth;
    psubinfo->height  = (int)lpbmi->biHeight / 2; /* XOR和AND 图总高度 */
    psubinfo->order   = 1;  /* 总是倒向图 */
    psubinfo->bitcount  = (int)lpbmi->biBitCount;

    if (_get_mask(psubinfo->bitcount,
        (unsigned long*)(&psubinfo->b_mask),
        (unsigned long*)(&psubinfo->g_mask),
        (unsigned long*)(&psubinfo->r_mask),
        (unsigned long*)(&psubinfo->a_mask)) == -1) {
      return -2;    /* 图象受损 */
    }

    psubinfo->left = psubinfo->top = 0;
    psubinfo->dowith = psubinfo->userinput = psubinfo->time = 0;
    psubinfo->colorkey = -1;  /* 定为无关键色 */

    /* 设置调色板数据 */
    if (lpbmi->biBitCount <= 8) {
      psubinfo->pal_count = 1UL << psubinfo->bitcount;

      pal_num = (lpbmi->biClrUsed == 0) ? psubinfo->pal_count : lpbmi->biClrUsed;

      if (pal_num > (unsigned long)psubinfo->pal_count) {
        return -2;    /* 图象受损 */
      }

      memmove((void*)(psubinfo->palette), (const void*)(pbi->bmiColors), sizeof(RGBQUAD)*pal_num);
    }
    else {
      psubinfo->pal_count = 0;
    }


    /* 取得XOR象素扫描行尺寸 */
    linesize  = _calcu_scanline_size(psubinfo->width, psubinfo->bitcount);
    /* 计算AND象素扫描行尺寸 */
    linesize2 = _calcu_scanline_size(psubinfo->width, 1);

    assert(psubinfo->p_bit_data == (unsigned char*)0);

    /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
    psubinfo->p_bit_data = (unsigned char*)isirw_malloc(linesize * psubinfo->height + 4);

    if (!psubinfo->p_bit_data) {
      return -3;      /* 内存不足 */
    }

    assert(psubinfo->pp_line_addr == (void**)0);
    /* 分配行首地址数组 */
    psubinfo->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * psubinfo->height);

    if (!psubinfo->pp_line_addr) {
      isirw_free(psubinfo->p_bit_data);
      psubinfo->p_bit_data = 0;
      return -3;      /* 内存不足 */
    }

    /* 初始化行首地址数组（倒向） */
    for (i = 0; i < (int)(psubinfo->height); i++) {
      psubinfo->pp_line_addr[i] = (void*)(psubinfo->p_bit_data + ((psubinfo->height - i - 1) * linesize));
    }

    /* 计算象素数据首地址 */
    p = (unsigned char*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(RGBQUAD);

    /* 复制象素数据 */
    memmove((void*)psubinfo->p_bit_data, (const void*)p, (linesize * psubinfo->height));

    /* 计算 XOR 象素首地址 */
    pxor = (unsigned char*)psubinfo->p_bit_data;
    /* 计算 AND 象素首地址 */
    pand = p + linesize * psubinfo->height;

    /* 合成图象（使用XOR数据与AND数据）*/
    for (i = 0; i < psubinfo->height; i++) {
      if (_compose_img(psubinfo->bitcount, psubinfo->width, pxor, pand) != 0) {
        isirw_free(psubinfo->p_bit_data);
        psubinfo->p_bit_data = 0;
        isirw_free(psubinfo->pp_line_addr);
        psubinfo->pp_line_addr = 0;

        return -2;    /* 受损的图象 */
      }

      pxor += linesize;
      pand += linesize2;
    }
  }

  return 0;   /* 返回 0 成功，非 0 失败 */
}


/* 合成一个扫描行的XOR图和AND图，并进行ISee位流格式转换，结果保存在XOR图中 */
int CALLAGREEMENT _compose_img(int bitcount, int width, unsigned char* pxor, unsigned char* pand)
{
  unsigned char buf[32], tmp, *p = pxor, isc = 0;
  int i, j, k, l, bytesize;

  assert(bitcount > 0);
  assert(pxor && pand);

  /* 一次处理 8 个象素 */
  for (i = 0, k = 0; i < width; i += 8, k++) {
    /* 取得 8 个象素的AND数据（共 1 个字节） */
    tmp = *(pand + k);

    /* 将AND数据转为XOR象素格式 */
    switch (bitcount) {
    case  1:
      buf[0] = tmp;
      bytesize = 1;
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
      *p++ ^= buf[j];
    }
  }

  return 0; /* 返回 0 表示成功，返回非 0 值表示失败 */
}


/* 分配一个子图像节点，并初始化节点内数据 */
LPSUBIMGBLOCK CALLAGREEMENT _alloc_SUBIMGBLOCK(void)
{
  LPSUBIMGBLOCK p_subimg = 0;

  /* 申请子图像信息节点(SUBIMGBLOCK) */
  if ((p_subimg = (LPSUBIMGBLOCK)isirw_malloc(sizeof(SUBIMGBLOCK))) == 0) {
    return 0;  /* 内存不足 */
  }

  /* 初始化为 0 */
  memset((void*)p_subimg, 0, sizeof(SUBIMGBLOCK));

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


/* 读取图像位数据 */
CC_INLINE int CALLAGREEMENT cur_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  LPCURDIR    lpir = 0;
  unsigned char* lpimg = 0;
  int       i;

  wBITMAPINFOHEADER*  lpbmi;
  LPSUBIMGBLOCK   lpsub = 0, lplast;
  LPSUBIMGBLOCK   subimg_tmp;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&cur_load_img_critical);

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

      /* 读取文件头信息 */
      switch (_read_cur_header(pfile, (CURDIR**)&lpir)) {
      case  -1:   /* 文件读写失败 */
        b_status = ER_FILERWERR;
        __leave;
        break;

      case  -2:   /* 光标文件受损 */
        b_status = ER_BADIMAGE;
        __leave;
        break;

      case  -3:   /* 内存不足 */
        b_status = ER_MEMORYERR;
        __leave;
        break;

      case  -4:   /* 不是一个光标文件 */
        b_status = ER_NONIMAGE;
        __leave;
        break;

      case  0:    /* 成功 */
        break;

      default:
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 读取主图象数据块 */
      switch (_read_image_block(pfile, (unsigned long)lpir->entries[0].image_offset, (unsigned long)lpir->entries[0].bytes_in_res, &lpimg)) {
      case  -1:   /* 文件读写失败 */
        b_status = ER_FILERWERR;
        __leave;
        break;

      case  -2:   /* 光标文件受损 */
        b_status = ER_BADIMAGE;
        __leave;
        break;

      case  -3:   /* 内存不足 */
        b_status = ER_MEMORYERR;
        __leave;
        break;

      case  0:    /* 成功 */
        break;

      default:
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      lpbmi = (wBITMAPINFOHEADER*)lpimg;

      assert(lpbmi);
      assert(lpir->count > 0);

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
        pinfo_str->imgformat  = IMF_CUR;      /* 图像文件格式（后缀名） */
        pinfo_str->compression  = ICS_RGB;      /* 图像的压缩方式 */

        /* 填写图像信息 */
        pinfo_str->width  = (unsigned long)lpbmi->biWidth;
        pinfo_str->height = (unsigned long)lpbmi->biHeight / 2; /* XOR和AND 图总高度 */
        pinfo_str->order  = 1;  /* 总是倒向图 */
        pinfo_str->bitcount = (unsigned long)lpbmi->biBitCount;

        if (_get_mask(pinfo_str->bitcount,
            (unsigned long*)(&pinfo_str->b_mask),
            (unsigned long*)(&pinfo_str->g_mask),
            (unsigned long*)(&pinfo_str->r_mask),
            (unsigned long*)(&pinfo_str->a_mask)) == -1) {
          b_status = ER_BADIMAGE;
          __LEAVE;
        }

        /* 设定数据包状态 */
        pinfo_str->data_state = 1;
      }

      /* 设置图像个数 */
      pinfo_str->imgnumbers = (unsigned long)lpir->count;
      assert(pinfo_str->psubimg == 0);
      pinfo_str->psubimg  = (LPSUBIMGBLOCK)0;

      /* 读首帧图象到数据包中 */
      switch (_conv_image_block(lpbmi, (void*)pinfo_str, CUR_PRI_IMAGE)) {
      case  -2:   /* 光标文件受损 */
        b_status = ER_BADIMAGE;
        __leave;
        break;

      case  -3:   /* 内存不足 */
        b_status = ER_MEMORYERR;
        __leave;
        break;

      case  0:    /* 成功 */
        break;

      default:
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 读取的首帧图象数据已无用，释放 */
      isirw_free(lpimg);
      lpimg = (unsigned char*)lpbmi = (unsigned char*)0;

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

      /* 一次性进度调用 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
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


      lplast = 0;

      /* 读入子图象 */
      for (i = 0; i < (int)(pinfo_str->imgnumbers - 1)/*主图象不算*/; i++) {
        if ((lpsub = _alloc_SUBIMGBLOCK()) == 0) {
          b_status = ER_MEMORYERR;
          __leave;
        }

        lpsub->number = i + 1; /* 子图像序号由 1 开始 */
        lpsub->parents = pinfo_str;

        /* 加入子图象链表尾部 */
        if (lplast == 0) {
          pinfo_str->psubimg = lpsub;
          lplast = lpsub;

          lpsub->prev = 0;
          lpsub->next = 0;
        }
        else {
          lpsub->prev = lplast;
          lpsub->next = 0;
          lplast->next = lpsub;
          lplast = lpsub;
        }

        /* 读取子图象数据块 */
        switch (_read_image_block(pfile, (unsigned long)lpir->entries[i + 1].image_offset, (unsigned long)lpir->entries[i + 1].bytes_in_res, &lpimg)) {
        case  -1:   /* 文件读写失败 */
          b_status = ER_FILERWERR;
          __leave;
          break;

        case  -2:   /* 光标文件受损 */
          b_status = ER_BADIMAGE;
          __leave;
          break;

        case  -3:   /* 内存不足 */
          b_status = ER_MEMORYERR;
          __leave;
          break;

        case  0:    /* 成功 */
          break;

        default:
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        lpbmi = (wBITMAPINFOHEADER*)lpimg;

        /* 转换子图象到数据包中 */
        switch (_conv_image_block(lpbmi, (void*)lpsub, CUR_SUB_IMAGE)) {
        case  -2:   /* 光标文件受损 */
          b_status = ER_BADIMAGE;
          __leave;
          break;

        case  -3:   /* 内存不足 */
          b_status = ER_MEMORYERR;
          __leave;
          break;

        case  0:    /* 成功 */
          break;

        default:
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        isirw_free(lpimg);
        lpimg = (unsigned char*)lpbmi = (unsigned char*)0;

        /* 回调通知 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_SERT_IMAGE, lpsub->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
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

        /* 一次性进度调用 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, lpsub->number, 0, lpsub->height, IRWE_CALLBACK_FUNID_LOAD)) {
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

        /* 一次子图象进度结束 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, lpsub->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
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
      }

      pinfo_str->data_state = 2;
    }
    __finally {
      if (lpimg) {
        isirw_free(lpimg);
      }

      if (lpir) {
        isirw_free(lpir);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        /* 释放子图像 */
        if (pinfo_str->psubimg != 0) {
          while (subimg_tmp = pinfo_str->psubimg->next) {
            _free_SUBIMGBLOCK(pinfo_str->psubimg);
            pinfo_str->psubimg = subimg_tmp;
          }

          _free_SUBIMGBLOCK(pinfo_str->psubimg);
          pinfo_str->psubimg = 0;
        }

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
          if (pinfo_str->psubimg != 0) {
            while (subimg_tmp = pinfo_str->psubimg->next) {
              _free_SUBIMGBLOCK(pinfo_str->psubimg);
              pinfo_str->psubimg = subimg_tmp;
            }

            _free_SUBIMGBLOCK(pinfo_str->psubimg);
            pinfo_str->psubimg = 0;
          }

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
      }

      LeaveCriticalSection(&cur_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}
