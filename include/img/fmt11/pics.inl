// SoftImage PIC图像读写模块实现文件
// 读取功能：可读取24位、32位（带一个ALPHA通道）的
// 压缩与未压缩 SoftImage PIC 图象
// 保存功能：可将24位图象以不压缩的方式保存为3通道
// SoftImage PIC 图象


/* 文件判别标志 */
#define PICS_MAGIC            0x5380F634
#define PICS_PICT_STR         0x54434950    /* 'PICT' 四个字符 */

/* 文件头结构定义 */
typedef struct {
  unsigned int  magic;           /* PIC图象标签             */
  float     version;         /* 使用的格式版本号        */
  char      comment[80];     /* 图象描述字串            */
  char      id[4];           /* "PICT"四个字符          */
  unsigned short  width;           /* 图象宽度，象素单位      */
  unsigned short  height;          /* 图象高度，象素单位      */
  float     ratio;           /* 图象的宽高比            */
  unsigned short  fields;          /* 图象标志集              */
  unsigned short  padding;         /* 保留                    */
} PICS_HEADER, *LPPICS_HEADER;


/* 通道信息结构 */
typedef struct {
  unsigned char chained;
  unsigned char size;     /* 每通道位数 */
  unsigned char type;     /* 压缩方式 */
  unsigned char channel;    /* 包含哪些通道(R、G、B、A...) */
} PICS_CHANNEL_INFO, *LPPICS_CHANNEL_INFO;



/* 数据类型 */
#define PIC_UNSIGNED_INTEGER  0x00
#define PIC_SIGNED_INTEGER    0x10  /* 未实现 */
#define PIC_SIGNED_FLOAT      0x20  /* 未实现 */


/* 压缩方法 */
#define PIC_UNCOMPRESSED      0x00
#define PIC_PURE_RUN_LENGTH   0x01
#define PIC_MIXED_RUN_LENGTH  0x02

/* 通道类型 */
#define PIC_RED_CHANNEL     0x80
#define PIC_GREEN_CHANNEL   0x40
#define PIC_BLUE_CHANNEL    0x20
#define PIC_ALPHA_CHANNEL   0x10
#define PIC_SHADOW_CHANNEL    0x08  /* 未实现 */
#define PIC_DEPTH_CHANNEL   0x04  /* 未实现 */
#define PIC_AUXILIARY_1_CHANNEL 0x02  /* 未实现 */
#define PIC_AUXILIARY_2_CHANNEL 0x01  /* 未实现 */



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 判断传入的图像是否可以被保存（如不支持保存功能，可将本函数去掉） */
int CALLAGREEMENT _pics_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* 位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(pics_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* 不支持的位深图像 */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (pics_irwp_info.irwp_save.img_num) {
    if (pics_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* 图像个数不正确 */
      }
    }
  }

  return 0;
}


/* 读取流中的通道信息（超过4个通道的流将被视为4通道图象）*/
static enum EXERESULT CALLAGREEMENT _read_channel_info(ISFILE* pfile, LPPICS_CHANNEL_INFO* ppchannel, int* channel_count)
{
  int         i, j;
  LPPICS_CHANNEL_INFO p = 0;
  LPPICS_CHANNEL_INFO ptmp;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && ppchannel && channel_count);

  __try {
    *ppchannel = p;
    *channel_count = 0;

    if (isio_seek(pfile, sizeof(PICS_HEADER), SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      __leave;
    }

    if ((p = ptmp = (LPPICS_CHANNEL_INFO)isirw_malloc(sizeof(PICS_CHANNEL_INFO) * 3)) == 0) {
      b_status = ER_MEMORYERR;
      __leave;
    }

    j = i = 0;

    do {
      /* 读一个通道数据 */
      if (isio_read((void*)ptmp, sizeof(PICS_CHANNEL_INFO), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      i++;
      j++;

      /* 检测通道数据是否合法 */
      if ((ptmp->size != 8) || (ptmp->type > 0x2) || ((ptmp->channel != 0xe0) && (ptmp->channel != 0x10))) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      if (i > 2) {    /* 只保存前4个通道的数据（RGB三个通道放在一起） */
        i--;
        ptmp--;  /* 继续读取下一个通道的数据，但不保留 */

        if (j > 6) {  /* 超过8个通道就有可能是非法的PIC图象 */
          b_status = ER_BADIMAGE;
          __leave;
        }
      }
    }
    while ((ptmp->chained != 0) && (ptmp++));

    *ppchannel    = p;  /* 传回通道数据 */
    *channel_count  = i;
  }
  __finally {
    if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
      if (p) {
        isirw_free(p);
      }
    }
  }

  return b_status;
}


/* 读取一个扫描行的数据（包括这个扫描行的所有通道）*/
static unsigned long read_scanline(ISFILE* pfile,
    unsigned char* scan,
    unsigned long width,
    LPPICS_CHANNEL_INFO channel,
    unsigned long chan_count)
{
  int       i;
  unsigned long noCol, status = 0;
  unsigned long off[4];
  unsigned long bytes;

  assert(pfile && scan && width && channel && chan_count);
  assert((chan_count == 1) || (chan_count == 2));

  bytes = (chan_count == 1) ? 3 : 4;

  for (i = 0; i < (int)chan_count; i++) {
    noCol = 0;

    /* 确定R、G、B各分量在ISee流中单个象素中的位置 */
    if (channel[i].channel & PIC_RED_CHANNEL) {
      off[noCol] = 2;
      noCol++;
    }

    if (channel[i].channel & PIC_GREEN_CHANNEL) {
      off[noCol] = 1;
      noCol++;
    }

    if (channel[i].channel & PIC_BLUE_CHANNEL) {
      off[noCol] = 0;
      noCol++;
    }

    if (channel[i].channel & PIC_ALPHA_CHANNEL) {
      off[noCol] = 3;
      noCol++;
    }

    /* 依据压缩方式调用相应的读取函数 */
    switch (channel[i].type & 0xf) {
    case PIC_UNCOMPRESSED:    /* 未压缩 */
      status = channel_read_raw(pfile, scan, width, noCol, off, bytes);
      break;

    case PIC_PURE_RUN_LENGTH: /* 标准RLE8压缩 */
      status = channel_read_pure(pfile, scan, width, noCol, off, bytes);
      break;

    case PIC_MIXED_RUN_LENGTH:  /* 混合RLE8压缩法 */
      status = channel_read_mixed(pfile, scan, width, noCol, off, bytes);
      break;

    default:          /* 系统异常 */
      assert(0);
      return 0;
    }

    if (status != 0) {      /* 如果出错则停止读取操作 */
      break;
    }
  }

  return status;
}


/* 读一未压缩行象素数据 */
static unsigned long channel_read_raw(ISFILE* file,
    unsigned char* scan,
    unsigned long width,
    unsigned long noCol,
    unsigned long* off,
    unsigned long bytes)
{
  int     i, j;

  for (i = 0; i < (int)width; i++) {
    if (isio_eof(file)) {
      return 1;   /* 源数据缺损 */
    }

    for (j = 0; j < (int)noCol; j++) {
      /* 按ISee流格式排列读入的象素分量 */
      if (isio_read((void*) & (scan[off[j]]), 1, 1, file) == 0) {
        return 2; /* 流读写异常 */
      }
    }

    scan += bytes;
  }

  return 0;
}


/* 读一标准RLE压缩行象素数据 */
static unsigned long channel_read_pure(ISFILE* file,
    unsigned char* scan,
    unsigned long width,
    unsigned long noCol,
    unsigned long* off,
    unsigned long bytes)
{
  unsigned char col[4];
  unsigned long count = 0;
  int       i, j, k;

  for (i = (int)width; i > 0;) {
    /* 读入计数值 */
    if (isio_read((void*)&count, 1, 1, file) == 0) {
      return 1;   /* 源数据缺损 */
    }

    /* 容错处理 */
    if (count > width) {
      count = width;
    }

    i -= (int)count;

    if (isio_eof(file)) {
      return 1;
    }

    /* 读入重复数据 */
    for (j = 0; j < (int)noCol; j++) {
      if (isio_read((void*) & (col[j]), 1, 1, file) == 0) {
        return 2; /* 流读写异常 */
      }
    }

    /* 展开至数据包 */
    for (k = 0; k < (int)count; k++, scan += bytes) {
      for (j = 0; j < (int)noCol; j++) {
        scan[off[j] + k] = col[j];
      }
    }
  }

  return 0;
}


/* 读一混合RLE压缩象素数据（非理论压缩法）*/
static unsigned long channel_read_mixed(ISFILE* file,
    unsigned char* scan,
    unsigned long width,
    unsigned long noCol,
    unsigned long* off,
    unsigned long bytes)
{
  unsigned long count = 0;
  int       i, j, k;
  unsigned char col[4];

  for (i = 0; i < (int)width; i += count) {
    if (isio_eof(file)) {
      return 1;     /* 源数据缺损 */
    }

    /* 读入计数值 */
    if (isio_read((void*)&count, 1, 1, file) == 0) {
      return 2;     /* 流读写异常 */
    }

    if (count >= 128) { /* 重复（被压缩）数据 */
      if (count == 128) {
        if (isio_read((void*)&count, 2, 1, file) == 0) {
          return 2;
        }

        count = (unsigned long)EXCHANGE_WORD(((unsigned short)count));
      }
      else {
        count -= 127;
      }

      if ((i + (int)count) > (int)width) {
        return 3;   /* 源数据非法 */
      }

      /* 读入压缩数据 */
      for (j = 0; j < (int)noCol; j++) {
        if (isio_read((void*) & (col[j]), 1, 1, file) == 0) {
          return 2;
        }
      }

      for (k = 0; k < (int)count; k++, scan += bytes) {
        for (j = 0; j < (int)noCol; j++) {
          scan[off[j]] = col[j];
        }
      }
    }
    else {
      /* 未能压缩的数据 */
      count++;

      if ((i + (int)count) > (int)width) {
        return 3;   /* 源数据非法 */
      }

      /* 直接复制源数据至数据包 */
      for (k = (int)count; k > 0; k--, scan += bytes) {
        for (j = 0; j < (int)noCol; j++) {
          if (isio_read((void*) & (scan[off[j]]), 1, 1, file) == 0) {
            return 2;
          }
        }
      }
    }
  }

  return 0;
}



/* 读取图像位数据 */
PICS_API int CALLAGREEMENT pics_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*        pfile = (ISFILE*)0;

  int         i, linesize;
  int         channel_count;
  unsigned char*   pb;

  PICS_HEADER     picheader;
  LPPICS_CHANNEL_INFO pchannel = 0;

  enum EXERESULT    b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&pics_load_img_critical);

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

      if (isio_read((void*)&picheader, sizeof(PICS_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 转为X86字序 */
      picheader.fields = EXCHANGE_WORD(picheader.fields);
      picheader.height = EXCHANGE_WORD(picheader.height);
      picheader.magic  = EXCHANGE_DWORD(picheader.magic);
      picheader.ratio  = (float)EXCHANGE_DWORD(((unsigned long)picheader.ratio));
      picheader.version = (float)EXCHANGE_DWORD(((unsigned long)picheader.version));
      picheader.width  = EXCHANGE_WORD(picheader.width);

      /* 判断图象流是否是SoftImage PIC格式 */
      if ((picheader.magic != PICS_MAGIC) || \
          (*((unsigned long*)(&(picheader.id))) != PICS_PICT_STR) || \
          (picheader.width == 0) || \
          (picheader.height == 0)) {
        b_status = ER_NONIMAGE;
        __leave;
      }

      /* 读取通道数据 */
      if ((b_status = _read_channel_info(pfile, &pchannel, &channel_count)) != ER_SUCCESS) {
        __leave;
      }

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        if ((channel_count != 1) && (channel_count != 2)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
        pinfo_str->imgformat  = IMF_PIC;      /* 图像文件格式（后缀名） */

        pinfo_str->compression  = ICS_RGB;      /* 象素数据压缩方法 */

        for (i = 0; i < channel_count; i++) { /* 如果有一个通道采用了RLE8，则视为RLE8压缩 */
          if (pchannel[i].type != PIC_UNCOMPRESSED) {
            pinfo_str->compression  = ICS_RLE8;
            break;
          }
        }

        pinfo_str->width  = (unsigned long)picheader.width;
        pinfo_str->height = (unsigned long)picheader.height;
        pinfo_str->order  = 0;          /* 正向图 */
        pinfo_str->bitcount = (unsigned long)(channel_count == 1) ? 24 : 32;

        if (pinfo_str->bitcount == 24) {      /* 设置掩码数据 */
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else if (pinfo_str->bitcount == 32) {
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0xff000000;
        }
        else {
          assert(0);
        }

        pinfo_str->data_state = 1;
      }

      /* 设置调色板项的个数为0 */
      pinfo_str->pal_count = 0;

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

      /* 初始化行首地址数组（PIC为正向图）*/
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

      /* 逐行读入象素数据 */
      for (pb = (unsigned char*)pinfo_str->p_bit_data, i = 0; i < (int)(pinfo_str->height); i++) {
        switch (read_scanline(pfile, pb, pinfo_str->width, pchannel, channel_count)) {
        case  0:
          break;

        case  1:
        case  3:
          b_status = ER_BADIMAGE;
          __leave;
          break;

        case  2:
          b_status = ER_FILERWERR;
          __leave;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        pb += linesize;

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
      if (pchannel) {
        isirw_free(pchannel);
      }

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

      LeaveCriticalSection(&pics_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 保存图像 */
PICS_API int CALLAGREEMENT pics_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*        pfile = (ISFILE*)0;

  int         i, j, linesize;
  unsigned char*   pb;
  unsigned char   buf[4];

  PICS_HEADER     picheader;
  PICS_CHANNEL_INFO channel_info;

  enum EXERESULT    b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&pics_save_img_critical);

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
      if (_pics_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* 如果存在同名流，本函数将不进行确认提示 */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 填写流头结构 */
      strcpy((char*)picheader.comment, (const char*)"Saved by ISee PICS plug-in.");
      *((unsigned long*) & (picheader.id[0])) = PICS_PICT_STR;
      picheader.padding = 0;

      picheader.magic  = PICS_MAGIC;
      picheader.width  = (unsigned short)pinfo_str->width;
      picheader.height = (unsigned short)pinfo_str->height;
      picheader.fields = 0x3;
      picheader.ratio  = (float)pinfo_str->width / (float)pinfo_str->height;
      picheader.version = (float)0.0;

      /* 转换至MOTO字序 */
      picheader.magic  = EXCHANGE_DWORD((picheader.magic));
      picheader.width  = EXCHANGE_WORD((picheader.width));
      picheader.height = EXCHANGE_WORD((picheader.height));
      picheader.fields = EXCHANGE_WORD((picheader.fields));
      picheader.ratio  = (float)EXCHANGE_DWORD(((unsigned long)picheader.ratio));
      picheader.version = (float)EXCHANGE_DWORD(((unsigned long)picheader.version));

      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 写入文件头结构 */
      if (isio_write((const void*)&picheader, sizeof(PICS_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      assert(pinfo_str->bitcount == 24);

      /* 填写通道信息（只有一个通道）*/
      channel_info.chained = 0;
      channel_info.channel = 0xe0;
      channel_info.size    = 8;
      channel_info.type    = PIC_UNCOMPRESSED;

      /* 写入通道信息 */
      if (isio_write((const void*)&channel_info, sizeof(PICS_CHANNEL_INFO), 1, pfile) == 0) {
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

      /* 以不压缩的方式逐行保存象素数据 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        pb = (unsigned char*)pinfo_str->pp_line_addr[i];

        for (j = 0; j < (int)pinfo_str->width; j++) {
          /* 提取出R、G、B分量值（PIC顺序与ISee流格式相反）*/
          buf[2] = *pb++;
          buf[1] = *pb++;
          buf[0] = *pb++;

          if (isio_write((const void*)buf, 3, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }
        }

        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, i, 1, IRWE_CALLBACK_FUNID_SAVE)) {
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
      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&pics_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



