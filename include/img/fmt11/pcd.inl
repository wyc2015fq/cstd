// PCD图像读写模块实现文件
// 读取功能：可读取24位PCD图象（前三幅）
// 保存功能：不支持


/* 文件判别标志（PCD无详细的格式文档，柯达有意没有发布这些
 * 文档，所以只能通过判断图象最小长度来大概确定它是否是一个
 * PCD图象。当然，这种方式是很不精确的）
 */
#define PCD_MIN_SIZE          (1048576UL)
/* PCD图象方向信息位置 */
#define PCD_VERTICAL_LOC        72
#define PCD_VERTICAL_MASK       (0x3f)
/* PCD子图象尺寸（PCD图象是固定尺寸的）*/
#define PCD_1_W             192
#define PCD_1_H             128
#define PCD_2_W             384
#define PCD_2_H             256
#define PCD_3_W             768
#define PCD_3_H             512
#define PCD_4_W             1536
#define PCD_4_H             1024
#define PCD_5_W             3072
#define PCD_5_H             2048
/* PCD缺省图象位深度 */
#define PCD_DEF_BITCOUNT        24
/* PCD子图象数据在图象中的偏移（后两幅图象的偏移柯达公司没有公布） */
#define PCD_1_OFF           0x2000
#define PCD_2_OFF           0xb800
#define PCD_3_OFF           0x30000




/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 将指定主（或子）图象读入数据包 */
static enum EXERESULT CALLAGREEMENT _read_img(void* pinfo, int img_num, ISFILE* pfile)
{
  LPINFOSTR   pinfo_str = (LPINFOSTR)pinfo;
  LPSUBIMGBLOCK psub_str  = (LPSUBIMGBLOCK)pinfo;

  int       w, h, bitcount, linesize, i, j;
  long      pos;
  unsigned char* y1 = 0, *y2 = 0, *cbcr = 0, *pbits, *p;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(pinfo && pfile);
  assert((img_num < 3) && (img_num >= 0));  /* 插件暂时只能读取前三幅图象 */

  __try {
    __try {
      /* 获取图象尺寸数据 */
      if (img_num == 0) {       /* 第一幅图象（主图象）*/
        w = (int)(pinfo_str->width);
        h = (int)(pinfo_str->height);
        bitcount = (int)(pinfo_str->bitcount);
        pbits = (unsigned char*)(pinfo_str->p_bit_data);
      }
      else {              /* 后续图象（子图象）*/
        w = (int)(psub_str->width);
        h = (int)(psub_str->height);
        bitcount = (int)(psub_str->bitcount);
        pbits = (unsigned char*)(psub_str->p_bit_data);
      }

      /* 计算扫描行尺寸 */
      linesize  = _calcu_scanline_size(w, bitcount);

      /* 分配合成缓冲区 */
      y1   = (unsigned char*)isirw_malloc(w);
      y2   = (unsigned char*)isirw_malloc(w);
      cbcr = (unsigned char*)isirw_malloc(w);

      if ((y1 == 0) || (y2 == 0) || (cbcr == 0)) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 计算图象数据偏移 */
      switch (img_num) {
      case  0:
        pos = PCD_1_OFF;
        break;

      case  1:
        pos = PCD_2_OFF;
        break;

      case  2:
        pos = PCD_3_OFF;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 设定读取位置 */
      if (isio_seek(pfile, pos, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* PCD图象高度一定是偶数 */
      assert((h % 2) == 0);

      /* 逐行读入数据 */
      for (i = 0; i < h; i += 2) {
        unsigned char* r, *g, *b;

        /* 读取两行的YCbCr数据（CbCr数据合用） */
        if (isio_read((void*)y1, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)y2, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)cbcr, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 确定象素写入位置 */
        p = pbits + (i + 0) * linesize;

        /* 转换YUV数据至RGB数据 */
        for (j = 0; j < w; j++) {
          b = p + j * 3 + 0;
          g = p + j * 3 + 1;
          r = p + j * 3 + 2;
          YUV2RGB(y1[j], cbcr[j / 2], cbcr[(w / 2) + (j / 2)], r, g, b);
        }

        /* 指向下一行地址 */
        p = pbits + (i + 1) * linesize;

        /* 转换YUV数据至RGB数据 */
        for (j = 0; j < w; j++) {
          b = p + j * 3 + 0;
          g = p + j * 3 + 1;
          r = p + j * 3 + 2;
          YUV2RGB(y2[j], cbcr[j / 2], cbcr[(w / 2) + (j / 2)], r, g, b);
        }
      }

    }
    __finally {
      if (y1) {
        isirw_free(y1);
      }

      if (y2) {
        isirw_free(y2);
      }

      if (cbcr) {
        isirw_free(cbcr);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return b_status;
}


/* 分配一个子图像节点，并初始化节点内数据 */
static LPSUBIMGBLOCK CALLAGREEMENT _alloc_SUBIMGBLOCK()
{
  LPSUBIMGBLOCK p_subimg;

  /* 申请子图像信息节点(SUBIMGBLOCK) */
  if ((p_subimg = (LPSUBIMGBLOCK)isirw_malloc(sizeof(SUBIMGBLOCK))) == 0) {
    return 0;  /* 内存不足 */
  }

  /* 初始化为 0 */
  memset((void*)p_subimg, 0, sizeof(SUBIMGBLOCK));

  p_subimg->number   = 1;     /* 子图象起始序号为 1 */
  p_subimg->colorkey = -1;    /* 透明色值设为无 */

  return p_subimg;
}



/* 释放一个子图像节点，包括其中的位缓冲区及行首地址数组 */
static void CALLAGREEMENT _free_SUBIMGBLOCK(LPSUBIMGBLOCK p_node)
{
  assert(p_node);
  assert(p_node->number > 0);

  if (p_node->pp_line_addr) {
    isirw_free(p_node->pp_line_addr);
  }

  if (p_node->p_bit_data) {
    isirw_free(p_node->p_bit_data);
  }

  isirw_free(p_node);
}


/* 申请并初始化子图象节点 */
static LPSUBIMGBLOCK CALLAGREEMENT _allsub(int num, LPINFOSTR pinfo_str)
{
  LPSUBIMGBLOCK subimg_tmp;
  int       i, linesize;

  /* 申请节点 */
  if ((subimg_tmp = _alloc_SUBIMGBLOCK()) == 0) {
    return 0;
  }

  subimg_tmp->number  = num;      /* 子图像序号由 1 开始 */
  subimg_tmp->parents = pinfo_str;  /* 主图象地址 */

  if (num == 1) {
    subimg_tmp->prev    = 0;
    subimg_tmp->next    = 0;
    pinfo_str->psubimg  = subimg_tmp;
    pinfo_str->imgnumbers = 2;

    subimg_tmp->width = PCD_2_W;
    subimg_tmp->height  = PCD_2_H;
  }
  else if (num == 2) {
    subimg_tmp->prev    = pinfo_str->psubimg;
    subimg_tmp->next    = 0;
    pinfo_str->psubimg->next  = subimg_tmp;
    pinfo_str->imgnumbers = 3;

    subimg_tmp->width = PCD_3_W;
    subimg_tmp->height  = PCD_3_H;
  }
  else {
    assert(0);                  /* 只能处理第二幅和第三幅图象 */
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }

  subimg_tmp->order = pinfo_str->order;     /* 等同与主图象方向 */
  subimg_tmp->bitcount = PCD_DEF_BITCOUNT;

  SETMASK_24(subimg_tmp);

  subimg_tmp->left = subimg_tmp->top = 0;
  subimg_tmp->dowith = subimg_tmp->userinput = subimg_tmp->time = 0;
  subimg_tmp->colorkey = -1;            /* 定为无关键色 */
  subimg_tmp->pal_count = 0;

  linesize = _calcu_scanline_size(subimg_tmp->width, subimg_tmp->bitcount);

  assert(subimg_tmp->p_bit_data == (unsigned char*)0);

  /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
  subimg_tmp->p_bit_data = (unsigned char*)isirw_malloc(linesize * subimg_tmp->height + 4);

  if (!subimg_tmp->p_bit_data) {
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }

  assert(subimg_tmp->pp_line_addr == (void**)0);
  /* 分配行首地址数组 */
  subimg_tmp->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * subimg_tmp->height);

  if (!subimg_tmp->pp_line_addr) {
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }

  /* 初始化行首地址数组（倒向） */
  if (subimg_tmp->order == 0) { /* 正向 */
    for (i = 0; i < (int)(subimg_tmp->height); i++) {
      subimg_tmp->pp_line_addr[i] = (void*)(subimg_tmp->p_bit_data + (i * linesize));
    }
  }
  else {            /* 倒向 */
    for (i = 0; i < (int)(subimg_tmp->height); i++) {
      subimg_tmp->pp_line_addr[i] = (void*)(subimg_tmp->p_bit_data + ((subimg_tmp->height - i - 1) * linesize));
    }
  }

  return subimg_tmp;
}



/* 读取图像位数据 */
PCD_API int CALLAGREEMENT pcd_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       linesize, i;
  unsigned char ord;
  LPSUBIMGBLOCK subimg_tmp;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&pcd_load_img_critical);

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

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        if (isio_seek(pfile, 0, SEEK_END) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 简单的通过图象长度来确定是否是PCD图象 */
        if (isio_tell(pfile) < PCD_MIN_SIZE) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        /* 读取图象方向信息 */
        if (isio_seek(pfile, PCD_VERTICAL_LOC, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read((void*)&ord, sizeof(unsigned char), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
        pinfo_str->imgformat  = IMF_PCD;      /* 图像文件格式（后缀名） */
        pinfo_str->compression  = ICS_JPEGYCbCr;  /* 图像的压缩方式 */

        pinfo_str->width  = PCD_1_W;        /* 取第一幅图象的尺寸数据 */
        pinfo_str->height = PCD_1_H;
        pinfo_str->order  = ((ord & PCD_VERTICAL_MASK) != 8);
        pinfo_str->bitcount = PCD_DEF_BITCOUNT;   /* 不支持8位深PCD */

        SETMASK_24(pinfo_str);

        pinfo_str->data_state = 1;
      }

      pinfo_str->pal_count = 0;

      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* 设置图像个数 */
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg    = (LPSUBIMGBLOCK)0;

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

      /* 填写行首地址数组 */
      if (pinfo_str->order == 0) {  /* 正向 */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
        }
      }
      else {            /* 倒向 */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
        }
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

      /* 读入主图象 */
      if ((b_status = _read_img((void*)pinfo_str, 0, pfile)) != ER_SUCCESS) {
        __leave;
      }

      /* 一次行进度调用 */
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

      /* 申请第二幅图象的子节点 */
      if ((subimg_tmp = _allsub(1, pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 回调通知 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_SERT_IMAGE, subimg_tmp->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
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

      /* 读入第二幅图象 */
      if ((b_status = _read_img((void*)subimg_tmp, 1, pfile)) != ER_SUCCESS) {
        __leave;
      }

      /* 一次性进度调用 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, subimg_tmp->number, 0, subimg_tmp->height, IRWE_CALLBACK_FUNID_LOAD)) {
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
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, subimg_tmp->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
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

      /* 申请第三幅图象的子节点 */
      if ((subimg_tmp = _allsub(2, pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 回调通知 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_SERT_IMAGE, subimg_tmp->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
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

      /* 读入第三幅图象 */
      if ((b_status = _read_img((void*)subimg_tmp, 2, pfile)) != ER_SUCCESS) {
        __leave;
      }

      /* 一次性进度调用 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, subimg_tmp->number, 0, subimg_tmp->height, IRWE_CALLBACK_FUNID_LOAD)) {
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
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, subimg_tmp->number, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
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

      LeaveCriticalSection(&pcd_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}

