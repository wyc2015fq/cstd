/********************************************************************
 pcd.c
 本文件用途： PCD图像读写模块实现文件
 读取功能：可读取24位PCD图象（前三幅）
 保存功能：不支持
 本文件编写人：
 YZ yzfree##yeah.net
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2。引入内存防护概念。
 2002-6 第一个发布版（新版）
********************************************************************/


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


/* 内部助手函数 */
static enum EXERESULT _read_img(void* pinfo, int img_num, ISFILE* pfile);
static SUBIMGBLOCK* _allsub(int num, INFOSTR* pinfo_str);

/* 获取图像信息 */
EXERESULT pcd_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  uchar ord;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */

    do {
      /* 确定PCD图象长度 */
      if (isio_seek(pfile, 0, SEEK_END) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 简单的通过图象长度来确定是否是PCD图象 */
      if (isio_tell(pfile) < PCD_MIN_SIZE) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 读取图象方向信息 */
      if (isio_seek(pfile, PCD_VERTICAL_LOC, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&ord, sizeof(uchar), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_PCD; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_JPEGYCbCr; /* 图像的压缩方式 */
      /* 填写图像信息 */
      pinfo_str->width = PCD_1_W; /* 取第一幅图象的尺寸数据 */
      pinfo_str->height = PCD_1_H;
      pinfo_str->order = ((ord & PCD_VERTICAL_MASK) != 8);
      pinfo_str->bitcount = PCD_DEF_BITCOUNT; /* 不支持8位深PCD */
      /* 填写掩码数据（24位）*/
      SETMASK_24(pinfo_str);
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT pcd_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int linesize, i;
  uchar ord;
  SUBIMGBLOCK* subimg_tmp;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */

    do {
      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        if (isio_seek(pfile, 0, SEEK_END) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 简单的通过图象长度来确定是否是PCD图象 */
        if (isio_tell(pfile) < PCD_MIN_SIZE) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 读取图象方向信息 */
        if (isio_seek(pfile, PCD_VERTICAL_LOC, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_read((void*)&ord, sizeof(uchar), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
        pinfo_str->imgformat = IMF_PCD; /* 图像文件格式（后缀名） */
        pinfo_str->compression = ICS_JPEGYCbCr; /* 图像的压缩方式 */
        pinfo_str->width = PCD_1_W; /* 取第一幅图象的尺寸数据 */
        pinfo_str->height = PCD_1_H;
        pinfo_str->order = ((ord & PCD_VERTICAL_MASK) != 8);
        pinfo_str->bitcount = PCD_DEF_BITCOUNT; /* 不支持8位深PCD */
        SETMASK_24(pinfo_str);
        pinfo_str->data_state = 1;
      }
      pinfo_str->pal_count = 0;
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
      /* 填写行首地址数组 */
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
      /* 读入主图象 */
      if ((b_status = _read_img((void*)pinfo_str, 0, pfile)) != ER_SUCCESS) {
        break;
      }
      /* 申请第二幅图象的子节点 */
      if ((subimg_tmp = _allsub(1, pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 读入第二幅图象 */
      if ((b_status = _read_img((void*)subimg_tmp, 1, pfile)) != ER_SUCCESS) {
        break;
      }
      /* 申请第三幅图象的子节点 */
      if ((subimg_tmp = _allsub(2, pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 读入第三幅图象 */
      if ((b_status = _read_img((void*)subimg_tmp, 2, pfile)) != ER_SUCCESS) {
        break;
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
    if (b_status != ER_SUCCESS) {
      INFOSTR_free(pinfo_str);
    }
  return b_status;
}
/* 保存图像 */
EXERESULT pcd_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  return ER_NOTSUPPORT;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 将指定主（或子）图象读入数据包 */
static enum EXERESULT _read_img(void* pinfo, int img_num, ISFILE* pfile)
{
  INFOSTR* pinfo_str = (INFOSTR*)pinfo;
  SUBIMGBLOCK* psub_str = (SUBIMGBLOCK*)pinfo;
  int w, h, bitcount, linesize, i, j;
  long pos;
  uchar* y1 = 0, *y2 = 0, *cbcr = 0, *pbits, *p;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pinfo && pfile);
  assert((img_num < 3) && (img_num >= 0)); /* 插件暂时只能读取前三幅图象 */

    do {
      /* 获取图象尺寸数据 */
      if (img_num == 0) { /* 第一幅图象（主图象）*/
        w = (int)(pinfo_str->width);
        h = (int)(pinfo_str->height);
        bitcount = (int)(pinfo_str->bitcount);
        pbits = (uchar*)(pinfo_str->p_bit_data);
      }
      else { /* 后续图象（子图象）*/
        w = (int)(psub_str->width);
        h = (int)(psub_str->height);
        bitcount = (int)(psub_str->bitcount);
        pbits = (uchar*)(psub_str->p_bit_data);
      }
      /* 计算扫描行尺寸 */
      linesize = _calcu_scanline_size(w, bitcount);
      /* 分配合成缓冲区 */
      y1 = (uchar*)isirw_malloc(w);
      y2 = (uchar*)isirw_malloc(w);
      cbcr = (uchar*)isirw_malloc(w);
      if ((y1 == 0) || (y2 == 0) || (cbcr == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 计算图象数据偏移 */
      switch (img_num) {
      case 0:
        pos = PCD_1_OFF;
        break;
      case 1:
        pos = PCD_2_OFF;
        break;
      case 2:
        pos = PCD_3_OFF;
        break;
      default:
        assert(0);
        b_status = ER_SYSERR;
        break;
      }
      /* 设定读取位置 */
      if (isio_seek(pfile, pos, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* PCD图象高度一定是偶数 */
      assert((h % 2) == 0);
      /* 逐行读入数据 */
      for (i = 0; i < h; i += 2) {
        uchar* r, *g, *b;
        /* 读取两行的YCbCr数据（CbCr数据合用） */
        if (isio_read((void*)y1, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_read((void*)y2, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_read((void*)cbcr, w, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
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
    while (0);
    
      if (y1) {
        isirw_free(y1);
      }
      if (y2) {
        isirw_free(y2);
      }
      if (cbcr) {
        isirw_free(cbcr);
      }
  return b_status;
}
/* 申请并初始化子图象节点 */
static SUBIMGBLOCK* _allsub(int num, INFOSTR* pinfo_str)
{
  SUBIMGBLOCK* subimg_tmp;
  int i, linesize;
  /* 申请节点 */
  if ((subimg_tmp = _alloc_SUBIMGBLOCK(pinfo_str)) == 0) {
    return 0;
  }
  subimg_tmp->number = 1; /* 子图象起始序号为 1 */
  subimg_tmp->colorkey = -1; /* 透明色值设为无 */
  subimg_tmp->number = num; /* 子图像序号由 1 开始 */
  //subimg_tmp->parents = pinfo_str; /* 主图象地址 */
  if (num == 1) {
    pinfo_str->psubimg = subimg_tmp;
    pinfo_str->imgnumbers = 2;
    subimg_tmp->width = PCD_2_W;
    subimg_tmp->height = PCD_2_H;
  }
  else if (num == 2) {
    pinfo_str->imgnumbers = 3;
    subimg_tmp->width = PCD_3_W;
    subimg_tmp->height = PCD_3_H;
  }
  else {
    assert(0); /* 只能处理第二幅和第三幅图象 */
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }
  subimg_tmp->order = pinfo_str->order; /* 等同与主图象方向 */
  subimg_tmp->bitcount = PCD_DEF_BITCOUNT;
  SETMASK_24(subimg_tmp);
  subimg_tmp->left = subimg_tmp->top = 0;
  subimg_tmp->dowith = subimg_tmp->userinput = subimg_tmp->time = 0;
  subimg_tmp->colorkey = -1; /* 定为无关键色 */
  subimg_tmp->pal_count = 0;
  linesize = _calcu_scanline_size(subimg_tmp->width, subimg_tmp->bitcount);
  assert(subimg_tmp->p_bit_data == (uchar*)0);
  /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
  subimg_tmp->p_bit_data = (uchar*)isirw_malloc(linesize * subimg_tmp->height + 4);
  if (!subimg_tmp->p_bit_data) {
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }
  assert(subimg_tmp->pp_line_addr == NULL);
  /* 分配行首地址数组 */
  subimg_tmp->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * subimg_tmp->height);
  if (!subimg_tmp->pp_line_addr) {
    _free_SUBIMGBLOCK(subimg_tmp);
    return 0;
  }
  /* 初始化行首地址数组（倒向） */
  if (subimg_tmp->order == 0) { /* 正向 */
    for (i = 0; i < (int)(subimg_tmp->height); i++) {
      subimg_tmp->pp_line_addr[i] = (subimg_tmp->p_bit_data + (i * linesize));
    }
  }
  else { /* 倒向 */
    for (i = 0; i < (int)(subimg_tmp->height); i++) {
      subimg_tmp->pp_line_addr[i] = (subimg_tmp->p_bit_data + ((subimg_tmp->height - i - 1) * linesize));
    }
  }
  return subimg_tmp;
}

