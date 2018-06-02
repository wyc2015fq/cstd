// SGI图像读写模块实现文件
// 读取功能：8、24、32位SGI图象（包括压缩与未压缩）
// 保存功能：24位非压缩格式

/* 文件判别标志 */
#define SGI_MAGIC           0x1da

#define SGI_REV0_SIZE   4
#define SGI_REV1_SIZE   404
#define SGI_NAME_SIZE   80

/* 文件头结构定义 */
typedef struct _tag_sgi_header {
  unsigned short  magic;          /* 文件标识 */
  unsigned char storage;        /* 存储格式 */
  unsigned char bpc;          /* 每个通道象素的字节数 */
  unsigned short  dim;          /* 维数 */
  unsigned short  width;          /* 宽度 */
  unsigned short  hight;          /* 高度 */
  unsigned short  channel;        /* 通道数 */
  long      pixmin;         /* 最小象素值 */
  long      pixmax;         /* 最大象素值 */
  char      rev0[SGI_REV0_SIZE];  /* 保留 */
  char      imgname[SGI_NAME_SIZE]; /* 图象描述串 */
  long      palid;          /* 调色板ID */
  char      rev1[SGI_REV1_SIZE];  /* 保留 */
} SGIHEADER, *LPSGIHEADER;

/* SGI流头结构 */
static SGIHEADER  sgi_header;



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 判断传入的图像是否可以被保存（如不支持保存功能，可将本函数去掉） */
int CALLAGREEMENT _sgi_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* 位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(sgi_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* 不支持的位深图像 */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (sgi_irwp_info.irwp_save.img_num) {
    if (sgi_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* 图像个数不正确 */
      }
    }
  }

  return 0;
}


/* 读未压缩图象数据到数据包中 */
static enum EXERESULT CALLAGREEMENT _read_uncom(ISFILE* pfile, LPINFOSTR pinfo_str, LPSGIHEADER psgi)
{
  int       i, linesize;
  unsigned char* p;
  long      fr, fg, fb, fa;
  unsigned char* pr = 0, *pg = 0, *pb = 0, *pa = 0;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pinfo_str && psgi);

  __try {
    if (isio_seek(pfile, sizeof(SGIHEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    assert(pinfo_str->pp_line_addr);

    linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
    p = pinfo_str->p_bit_data;

    switch (pinfo_str->bitcount) {
    case  8:
      assert((psgi->channel == 1) && (psgi->bpc == 1));

      /* 逐行读取象素数据 */
      for (i = 0; i < (int)psgi->hight; i++) {
        if (isio_read((void*)p, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 进度调用 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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

        p += linesize;
      }

      break;

    case  24:
      assert((psgi->channel == 3) && (psgi->bpc == 1));

      /* 申请R、G、B临时缓冲区 */
      pr = (unsigned char*)isirw_malloc(psgi->width);
      pg = (unsigned char*)isirw_malloc(psgi->width);
      pb = (unsigned char*)isirw_malloc(psgi->width);

      if (!(pr && pg && pb)) {
        b_status = ER_MEMORYERR;
        __leave;
        break;
      }

      /* 计算R、G、B分量数据在流中的偏移 */
      fr = sizeof(SGIHEADER);
      fg = fr + psgi->width * psgi->hight;
      fb = fg + psgi->width * psgi->hight;

      /* 逐行读取象素数据 */
      for (i = 0; i < (int)psgi->hight; i++) {
        /* 红色分量 */
        if (isio_seek(pfile, fr, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pr, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 绿色分量 */
        if (isio_seek(pfile, fg, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pg, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 蓝色分量 */
        if (isio_seek(pfile, fb, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pb, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        _to_rgb(pr, pg, pb, p, psgi->width);

        p += linesize;

        fr += (long)psgi->width;
        fg += (long)psgi->width;
        fb += (long)psgi->width;

        /* 进度调用 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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

      break;

    case  32:
      assert((psgi->channel == 4) && (psgi->bpc == 1));

      /* 申请R、G、B临时缓冲区 */
      pr = (unsigned char*)isirw_malloc(psgi->width);
      pg = (unsigned char*)isirw_malloc(psgi->width);
      pb = (unsigned char*)isirw_malloc(psgi->width);
      pa = (unsigned char*)isirw_malloc(psgi->width);

      if (!(pr && pg && pb && pa)) {
        b_status = ER_MEMORYERR;
        __leave;
        break;
      }

      /* 计算R、G、B分量数据在流中的偏移 */
      fr = sizeof(SGIHEADER);
      fg = fr + psgi->width * psgi->hight;
      fb = fg + psgi->width * psgi->hight;
      fa = fb + psgi->width * psgi->hight;

      /* 逐行读取象素数据 */
      for (i = 0; i < (int)psgi->hight; i++) {
        /* 红色分量 */
        if (isio_seek(pfile, fr, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pr, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 绿色分量 */
        if (isio_seek(pfile, fg, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pg, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 蓝色分量 */
        if (isio_seek(pfile, fb, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pb, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* ALPHA通道数据 */
        if (isio_seek(pfile, fa, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)pa, psgi->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        _to_rgba(pr, pg, pb, pa, p, psgi->width);

        p += linesize;

        fr += (long)psgi->width;
        fg += (long)psgi->width;
        fb += (long)psgi->width;
        fa += (long)psgi->width;

        /* 进度调用 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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

      break;

    default:
      assert(0);
      b_status = ER_SYSERR;
      __leave;
      break;
    }

  }
  __finally {
    if (pr) {
      isirw_free(pr);
    }

    if (pg) {
      isirw_free(pg);
    }

    if (pb) {
      isirw_free(pb);
    }

    if (pa) {
      isirw_free(pa);
    }
  }

  return b_status;
}


/* 读RLE8压缩图象数据到数据包中 */
static enum EXERESULT CALLAGREEMENT _read_rle8(ISFILE* pfile, LPINFOSTR pinfo_str, LPSGIHEADER psgi)
{
  int       i, linesize;
  unsigned char* p;
  unsigned char* pr = 0, *pg = 0, *pb = 0, *pa = 0, *ptmp = 0;
  unsigned long* pofftab = 0, *plentab = 0;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pinfo_str && psgi);

  __try {
    if (isio_seek(pfile, sizeof(SGIHEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    /* 分配RLE解压用临时缓冲区 */
    if ((ptmp = (unsigned char*)isirw_malloc(psgi->width * 2 + 4)) == 0) {
      b_status = ER_MEMORYERR;
      __leave;
    }

    /* 分配用于存储RLE行偏移及行长度的缓冲区 */
    pofftab = (unsigned long*)isirw_malloc(psgi->hight * psgi->channel * sizeof(long));
    plentab = (unsigned long*)isirw_malloc(psgi->hight * psgi->channel * sizeof(long));

    if (!(pofftab && plentab)) {
      b_status = ER_MEMORYERR;
      __leave;
    }

    /* 读入RLE行偏移表 */
    if (_read_long_tab(pfile, pofftab, psgi->hight * psgi->channel) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    /* 读入RLE行长度表 */
    if (_read_long_tab(pfile, plentab, psgi->hight * psgi->channel) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
    p = pinfo_str->p_bit_data;

    switch (pinfo_str->bitcount) {
    case  8:
      assert((psgi->channel == 1) && (psgi->bpc == 1));

      /* 逐行读取象素数据 */
      for (i = 0; i < (int)psgi->hight; i++) {
        if (isio_seek(pfile, pofftab[i], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 解压缩RLE行 */
        if (_uncompression_rle8(ptmp, (int)plentab[i], p, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        p += linesize;

        /* 进度调用 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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

      break;

    case  24:
      assert((psgi->channel == 3) && (psgi->bpc == 1));

      /* 申请R、G、B临时缓冲区 */
      pr = (unsigned char*)isirw_malloc(psgi->width);
      pg = (unsigned char*)isirw_malloc(psgi->width);
      pb = (unsigned char*)isirw_malloc(psgi->width);

      if (!(pr && pg && pb)) {
        b_status = ER_MEMORYERR;
        __leave;
        break;
      }

      /* 逐行读取象素数据 */
      for (i = 0; i < (int)psgi->hight; i++) {
        /* 红色分量 */
        if (isio_seek(pfile, pofftab[i], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i], pr, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* 绿色分量 */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight)], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight)], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight)], pg, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* 蓝色分量 */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight) * 2], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight) * 2], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight) * 2], pb, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        _to_rgb(pr, pg, pb, p, psgi->width);

        p += linesize;

        /* 进度调用 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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

      break;

    case  32:
      assert((psgi->channel == 4) && (psgi->bpc == 1));

      /* 申请R、G、B临时缓冲区 */
      pr = (unsigned char*)isirw_malloc(psgi->width);
      pg = (unsigned char*)isirw_malloc(psgi->width);
      pb = (unsigned char*)isirw_malloc(psgi->width);
      pa = (unsigned char*)isirw_malloc(psgi->width);

      if (!(pr && pg && pb && pa)) {
        b_status = ER_MEMORYERR;
        __leave;
        break;
      }

      /* 逐行读取象素数据 */
      for (i = 0; i < (int)psgi->hight; i++) {
        /* 红色分量 */
        if (isio_seek(pfile, pofftab[i], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i], pr, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* 绿色分量 */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight)], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight)], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight)], pg, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* 蓝色分量 */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight) * 2], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight) * 2], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight) * 2], pb, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* ALPHA通道数据 */
        if (isio_seek(pfile, pofftab[i + (int)(psgi->hight) * 3], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)ptmp, plentab[i + (int)(psgi->hight) * 3], 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (_uncompression_rle8(ptmp, (int)plentab[i + (int)(psgi->hight) * 3], pa, (int)psgi->width) == -1) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        _to_rgba(pr, pg, pb, pa, p, psgi->width);

        p += linesize;

        /* 进度调用 */
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_LOAD)) {
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

      break;

    default:
      assert(0);
      b_status = ER_SYSERR;
      __leave;
      break;
    }

  }
  __finally {
    if (pr) {
      isirw_free(pr);
    }

    if (pg) {
      isirw_free(pg);
    }

    if (pb) {
      isirw_free(pb);
    }

    if (pa) {
      isirw_free(pa);
    }

    if (ptmp) {
      isirw_free(ptmp);
    }

    if (pofftab) {
      isirw_free(pofftab);
    }

    if (plentab) {
      isirw_free(plentab);
    }
  }

  return b_status;
}


/* 组合R、G、B分量到RGB数组 */
static void _to_rgb(unsigned char* pr, unsigned char* pg, unsigned char* pb, unsigned char* p, int width)
{
  int i;

  assert(pr && pg && pb && p && width);

  for (i = 0; i < width; i++) {
    *p++ = *pb++;
    *p++ = *pg++;
    *p++ = *pr++;
  }
}


/* 组合R、G、B、A分量到RGBA数组 */
static void _to_rgba(unsigned char* pr, unsigned char* pg, unsigned char* pb, unsigned char* pa, unsigned char* p, int width)
{
  int i;

  assert(pr && pg && pb && pa && p && width);

  for (i = 0; i < width; i++) {
    *p++ = *pb++;
    *p++ = *pg++;
    *p++ = *pr++;
    *p++ = *pa++;
  }
}


/* 读取long型表（偏移表或行长度表） */
static int _read_long_tab(ISFILE* pfile, unsigned long* pl, int count)
{
  int i;

  assert(pfile && pl);

  if (isio_read((void*)pl, count * sizeof(long), 1, pfile) == 0) {
    return -1;    /* 读流出错 */
  }

  /* 转换字序 */
  for (i = 0; i < count; i++) {
    pl[i] = EXCHANGE_DWORD((pl[i]));
  }

  return 0;
}


/* 解压一行RLE数据 */
static int _uncompression_rle8(unsigned char* prle, int rlesize, unsigned char* p, int bufsize)
{
  int       bufcount = 0;
  unsigned char pix, count;

  while (1) {
    /* 计数值 */
    pix = *prle++;
    rlesize--;

    /* 提取计数值（如果计数值为0，则说明RLE行结束） */
    if ((count = (pix & 0x7f)) == 0) {
      if ((bufcount == bufsize) && (rlesize == 0)) {
        return 0;
      }
      else {
        return -1;
      }
    }

    bufcount += (int)count;

    if (bufcount > bufsize) {
      return -1;    /* 源数据受损 */
    }

    if (pix & 0x80) {
      if (rlesize < (int)count) {
        return -1;  /* 错误 */
      }
      else {
        rlesize -= (int)count;
      }

      /* 复制未压缩数据 */
      while (count--) {
        *p++ = *prle++;
      }
    }
    else {
      if (rlesize == (int)0) {
        return -1;  /* 错误 */
      }
      else {
        rlesize--;
      }

      pix = *prle++;

      /* 复制重复数据 */
      while (count--) {
        *p++ = pix;
      }
    }

    if (bufcount == bufsize) {
      if (rlesize == 0) {
        return 0;  /* 成功 */
      }
      else if ((rlesize = 1) && (((*prle) & 0x7f) == 0)) {
        return 0;  /* 成功 */
      }
      else {
        return -1;
      }
    }
  }

  return -1;        /* 错误 */
}


/* 读取图像位数据 */
SGI_API int CALLAGREEMENT sgi_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, linesize;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&sgi_load_img_critical);

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

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读取流头结构 */
      if (isio_read((void*)&sgi_header, sizeof(SGIHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 转换字序 */
      sgi_header.magic  = EXCHANGE_WORD((sgi_header.magic));
      sgi_header.dim    = EXCHANGE_WORD((sgi_header.dim));
      sgi_header.width  = EXCHANGE_WORD((sgi_header.width));
      sgi_header.hight  = EXCHANGE_WORD((sgi_header.hight));
      sgi_header.channel = EXCHANGE_WORD((sgi_header.channel));

      sgi_header.pixmin = EXCHANGE_DWORD((sgi_header.pixmin));
      sgi_header.pixmax = EXCHANGE_DWORD((sgi_header.pixmax));
      sgi_header.palid  = EXCHANGE_DWORD((sgi_header.palid));

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        /* 判断是否是有效（受支持）的SGI图象流 */
        if ((sgi_header.magic != SGI_MAGIC) ||  \
            (sgi_header.width == 0)   ||    \
            (sgi_header.hight == 0)   ||    \
            (sgi_header.channel == 0) ||    \
            (sgi_header.channel == 2) ||    \
            (sgi_header.channel > 4)  ||    \
            (sgi_header.palid != 0)   ||    \
            (sgi_header.dim == 0)   ||    \
            (sgi_header.dim > 3)    ||    \
            (sgi_header.bpc != 1)) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        /* 填写图象信息 */
        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像流类型 */
        pinfo_str->imgformat  = IMF_SGI;      /* 图像流格式（后缀名） */

        pinfo_str->compression  = (sgi_header.storage == 0) ? ICS_RGB : ICS_RLE8;

        pinfo_str->width    = (unsigned long)sgi_header.width;
        pinfo_str->height   = (unsigned long)sgi_header.hight;
        pinfo_str->order    = 1;        /* SGI 图象都为倒向图 */
        pinfo_str->bitcount   = (unsigned long)(sgi_header.bpc * sgi_header.channel * 8);

        /* 设置掩码数据 */
        switch (pinfo_str->bitcount) {
        case  8:
          pinfo_str->r_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->b_mask = 0x0;
          pinfo_str->a_mask = 0x0;
          break;

        case  24:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0x0;
          break;

        case  32:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0xff000000;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        pinfo_str->data_state = 1;
      }


      /* 合成灰度调色板 */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;

        for (i = 0; i < 256; i++) {
          pinfo_str->palette[i] = (unsigned long)((i << 16) | (i << 8) | (i << 0));
        }
      }
      else {
        pinfo_str->pal_count = 0;
      }

      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

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

      /* 初始化行首地址数组（倒向） */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
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

      /* 读取图象到数据包 */
      if (sgi_header.storage == 0) {
        b_status = _read_uncom(pfile, pinfo_str, &sgi_header);
      }
      else {
        b_status = _read_rle8(pfile, pinfo_str, &sgi_header);
      }

      if (b_status != ER_SUCCESS) {
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

      LeaveCriticalSection(&sgi_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 保存图像 */
SGI_API int CALLAGREEMENT sgi_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  unsigned char* pbuf = 0;
  int       i, j;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&sgi_save_img_critical);

      /* 调用系统回调函数 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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

      /* 判断是否是受支持的图像数据 */
      if (_sgi_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* 如果存在同名流，本函数将不进行确认提示 */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 源数据必须是标准的24位图象 */
      assert((pinfo_str->r_mask == 0xff0000) && (pinfo_str->g_mask == 0xff00) && (pinfo_str->b_mask == 0xff));

      /* 填写流头结构并转换为MOTO字序 */
      sgi_header.magic  = EXCHANGE_WORD((SGI_MAGIC));
      sgi_header.dim    = EXCHANGE_WORD((3));
      sgi_header.width  = EXCHANGE_WORD(((unsigned short)pinfo_str->width));
      sgi_header.hight  = EXCHANGE_WORD(((unsigned short)pinfo_str->height));
      sgi_header.channel = EXCHANGE_WORD((3));

      sgi_header.pixmin = EXCHANGE_DWORD((0));
      sgi_header.pixmax = EXCHANGE_DWORD((0xff));
      sgi_header.palid  = EXCHANGE_DWORD((0));

      sgi_header.storage = 0;
      sgi_header.bpc    = 1;

      memset((void*)sgi_header.rev0, 0, SGI_REV0_SIZE);
      memset((void*)sgi_header.rev1, 0, SGI_REV1_SIZE);
      memcpy((void*)sgi_header.imgname, (const void*)"iseeexplorer image.", 20);

      /* 写入文件头结构 */
      if (isio_write((const void*)&sgi_header, sizeof(SGIHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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

      /* 分配颜色分量缓冲区 */
      if ((pbuf = (unsigned char*)isirw_malloc(pinfo_str->width + 4)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 写入R分量数据 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pbuf[j] = ((unsigned char*)(pinfo_str->pp_line_addr[((int)pinfo_str->height) - i - 1]))[j * 3 + 2];
        }

        if (isio_write((const void*)pbuf, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }

      /* 写入G分量数据 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pbuf[j] = ((unsigned char*)(pinfo_str->pp_line_addr[((int)pinfo_str->height) - i - 1]))[j * 3 + 1];
        }

        if (isio_write((const void*)pbuf, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }

      /* 写入B分量数据 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pbuf[j] = ((unsigned char*)(pinfo_str->pp_line_addr[((int)pinfo_str->height) - i - 1]))[j * 3 + 0];
        }

        if (isio_write((const void*)pbuf, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }

      /* 一次性进度调用 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, (int)pinfo_str->height, IRWE_CALLBACK_FUNID_SAVE)) {
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

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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
    __finally {
      if (pbuf) {
        isirw_free(pbuf);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&sgi_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


