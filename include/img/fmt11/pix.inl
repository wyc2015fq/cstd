// Alias PIX图像读写模块实现文件
// 读取功能：可读取8、24位RLE压缩Alias PIX图象
// 保存功能：24位RLE压缩格式

/* 流头结构定义 */
typedef struct {
  unsigned short  width;      /* 图象宽、高度（象素单位）*/
  unsigned short  height;
  unsigned short  rev0;     /* 已废弃的两个字，其值无意义 */
  unsigned short  rev1;
  unsigned short  bitcount;   /* 图象位深度（只能是24或8）*/
} PIX_HEADER, *LPPIX_HEADER;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 判断传入的图像是否可以被保存 */
int CALLAGREEMENT _pix_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* 位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(pix_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* 不支持的位深图像 */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (pix_irwp_info.irwp_save.img_num)
    if (pix_irwp_info.irwp_save.img_num == 1)
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* 图像个数不正确 */
      }

  return 0;
}


/* 读取一行24位RLE压缩数据 */
static enum EXERESULT CALLAGREEMENT _read_line_alias24(ISFILE* pfile, unsigned char* pix, int size)
{
  int       i;
  int       count = 0;
  unsigned char buffer[4];

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pix && size);

  __try {
    while (count < size) {
      /* 读取一组压缩数据（每组4字节，第一个字节为重复计数值，后
         三个字节为B、G、R数据 */
      if (isio_read((void*)buffer, 1, 4, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 重复计数值 */
      count += (int)(unsigned int)(unsigned char)(buffer[0]);

      if (count > size) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* 向数据包填写重复数据 */
      for (i = 0; i < (int)(unsigned int)(unsigned char)buffer[0]; i++) {
        *pix++ = buffer[1];
        *pix++ = buffer[2];
        *pix++ = buffer[3];
      }
    }
  }
  __finally {
    ;
  }

  return b_status;
}


/* 读取一行8位RLE压缩数据 */
static enum EXERESULT CALLAGREEMENT _read_line_alias8(ISFILE* pfile, unsigned char* pix, int size)
{
  int       i;
  int       count = 0;
  unsigned char buffer[2];

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pix && size);

  __try {
    while (count < size) {
      /* 读取一组压缩数据（每组2字节，第一个字节为重复计数值，后
         一个字节为颜色索引值 */
      if (isio_read((void*)buffer, 1, 2, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 重复计数值 */
      count += (int)(unsigned int)(unsigned char)(buffer[0]);

      if (count > size) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* 向数据包填写重复数据 */
      for (i = 0; i < (int)(unsigned int)(unsigned char)buffer[0]; i++) {
        *pix++ = buffer[1];
      }
    }
  }
  __finally {
    ;
  }

  return b_status;
}


/* 压缩一行数据到24位RLE */
static enum EXERESULT CALLAGREEMENT _write_line_alias24(ISFILE* pfile, unsigned char* pix, int size)
{
  int       i;
  int       count;
  unsigned char buffer[4];

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pfile && pix && size);

  __try {
    for (count = 0, i = 0; i < size; i++) {
      if (count == 0) {
        buffer[0] = 1;
        buffer[1] = *pix++; /* B */
        buffer[2] = *pix++; /* G */
        buffer[3] = *pix++; /* R */
        count = 1;
      }
      else {
        if ((*(pix + 0) == buffer[1]) && (*(pix + 1) == buffer[2]) && \
            (*(pix + 2) == buffer[3]) && (count < 255)) {
          count++;
          buffer[0] = (unsigned char)count;
          pix += 3;
        }
        else {
          /* 写入一组压缩数据 */
          if (isio_write((const void*)buffer, 1, 4, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }

          count = 0;

          /* 空操作，循环值不变 */
          i--;
        }
      }
    }

    /* 写入剩余的数据 */
    if (count != 0) {
      if (isio_write((const void*)buffer, 1, 4, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }
    }
  }
  __finally {
    ;
  }

  return b_status;
}

/* 读取图像位数据 */
PIX_API int CALLAGREEMENT pix_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, linesize;
  unsigned char* p;

  PIX_HEADER    pixheader;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&pix_load_img_critical);

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

      if (isio_read((void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 转为Intel字序 */
      pixheader.width   = EXCHANGE_WORD((pixheader.width));
      pixheader.height  = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        if ((pixheader.width == 0) || (pixheader.height == 0) || \
            ((pixheader.bitcount != 24) && (pixheader.bitcount != 8))) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        /* 填写数据包 */
        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
        pinfo_str->imgformat  = IMF_PIX;      /* 图像文件格式（后缀名） */

        pinfo_str->compression  = ICS_RLE8;

        pinfo_str->width  = (unsigned long)pixheader.width;
        pinfo_str->height = (unsigned long)pixheader.height;
        pinfo_str->order  = 0;          /* PIX 图都为正向图 */
        pinfo_str->bitcount = (unsigned long)pixheader.bitcount;

        switch (pixheader.bitcount) {
        case  24:
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
          break;

        case  8:
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
          break;
        }

        pinfo_str->data_state = 1;
      }

      /* 人工合成灰度图调色板 */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;

        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
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

      /* 填写行首地址数组（正向）*/
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

      /* 读哇..... */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = pinfo_str->p_bit_data + linesize * i;

        /* 逐行读取、解压缩 */
        if (pinfo_str->bitcount == 24) {
          if ((b_status = _read_line_alias24(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
            __leave;
          }
        }
        else {
          if ((b_status = _read_line_alias8(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
            __leave;
          }
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

      LeaveCriticalSection(&pix_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 保存图像 */
PIX_API int CALLAGREEMENT pix_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i;
  unsigned char* p;

  PIX_HEADER    pixheader;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&pix_save_img_critical);

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
      if (_pix_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* 如果存在同名流，本函数将不进行确认提示 */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 填写流头结构 */
      pixheader.width   = (unsigned short)pinfo_str->width;
      pixheader.height  = (unsigned short)pinfo_str->height;
      pixheader.rev0    = 0;
      pixheader.rev1    = 0;
      assert(pinfo_str->bitcount == 24);
      pixheader.bitcount = 24;

      /* 转换为MOTO字序 */
      pixheader.width   = EXCHANGE_WORD((pixheader.width));
      pixheader.height  = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));


      /* 写入流头结构 */
      if (isio_write((const void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
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

      /* 逐行编码（RLE8）并写入目标流 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        p = (unsigned char*)pinfo_str->pp_line_addr[i];

        if ((b_status = _write_line_alias24(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
          __leave;
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

      LeaveCriticalSection(&pix_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



