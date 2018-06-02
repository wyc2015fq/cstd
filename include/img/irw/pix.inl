/********************************************************************
 pix.c
 本文件用途： Alias PIX图像读写模块实现文件
 读取功能：可读取8、24位RLE压缩Alias PIX图象
 保存功能：24位RLE压缩格式
 本文件编写人：
 YZ yzfree##yeah.net
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2。引入内存防护概念。
 2002-5 第一个发布版（新版）
********************************************************************/

/* 流头结构定义 */
typedef struct {
  ushort  width;      /* 图象宽、高度（象素单位）*/
  ushort  height;
  ushort  rev0;     /* 已废弃的两个字，其值无意义 */
  ushort  rev1;
  ushort  bitcount;   /* 图象位深度（只能是24或8）*/
} PIX_HEADER, *LPPIX_HEADER;

/* 内部助手函数 */
int _pix_is_valid_img(INFOSTR* pinfo_str);
static enum EXERESULT _read_line_alias24(ISFILE*, uchar*, int);
static enum EXERESULT _read_line_alias8(ISFILE*, uchar*, int);
static enum EXERESULT _write_line_alias24(ISFILE*, uchar*, int);

/* 获取图像信息 */
EXERESULT pix_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PIX_HEADER pixheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */

    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 转为Intel字序 */
      pixheader.width = EXCHANGE_WORD((pixheader.width));
      pixheader.height = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));
      /* 判断是否是有效的Alias PIX图象（这种格式非常简单，所以
      无法做更严格的判断，发生错判的可能性较其它格式稍大一些）*/
      if ((pixheader.width == 0) || (pixheader.height == 0) || \
      ((pixheader.bitcount != 24) && (pixheader.bitcount != 8))) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 填写数据包 */
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_PIX; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_RLE8;
      pinfo_str->width = (ulong)pixheader.width;
      pinfo_str->height = (ulong)pixheader.height;
      pinfo_str->order = 0; /* PIX 图都为正向图 */
      pinfo_str->bitcount = (ulong)pixheader.bitcount;
      /* 填写掩码信息 */
      switch (pixheader.bitcount) {
      case 24:
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
        break;
      case 8:
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
        break;
      default:
        assert(0);
        b_status = ER_SYSERR;
        break;
      }
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT pix_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i, linesize;
  uchar* p;
  PIX_HEADER pixheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */

    do {
      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 转为Intel字序 */
      pixheader.width = EXCHANGE_WORD((pixheader.width));
      pixheader.height = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));
      /* 如果该图像还未调用过提取信息函?蛱钚赐?像信?*/
      if (pinfo_str->data_state == 0) {
        if ((pixheader.width == 0) || (pixheader.height == 0) || \
        ((pixheader.bitcount != 24) && (pixheader.bitcount != 8))) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 填写数据包 */
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
        pinfo_str->imgformat = IMF_PIX; /* 图像文件格式（后缀名） */
        pinfo_str->compression = ICS_RLE8;
        pinfo_str->width = (ulong)pixheader.width;
        pinfo_str->height = (ulong)pixheader.height;
        pinfo_str->order = 0; /* PIX 图都为正向图 */
        pinfo_str->bitcount = (ulong)pixheader.bitcount;
        switch (pixheader.bitcount) {
        case 24:
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
          break;
        case 8:
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
          break;
        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }
        pinfo_str->data_state = 1;
      }
      /* 人工合成灰度图调色板 */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;
        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          pinfo_str->palette[i] = (ulong)((i << 16) | (i << 8) | (i << 0));
        }
      }
      else {
        pinfo_str->pal_count = 0;
      }
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
      /* 填写行首地址数组（正向）*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      /* 读哇..... */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = pinfo_str->p_bit_data + linesize * i;
        /* 逐行读取、解压缩 */
        if (pinfo_str->bitcount == 24) {
          if ((b_status = _read_line_alias24(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
            break;
          }
        }
        else {
          if ((b_status = _read_line_alias8(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
            break;
          }
        }
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
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
/* 保存图像 */
EXERESULT pix_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i;
  uchar* p;
  PIX_HEADER pixheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  do {
      /* 判断是否是受支持的图像数据 */
      if (_pix_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* 填写流头结构 */
      pixheader.width = (ushort)pinfo_str->width;
      pixheader.height = (ushort)pinfo_str->height;
      pixheader.rev0 = 0;
      pixheader.rev1 = 0;
      assert(pinfo_str->bitcount == 24);
      pixheader.bitcount = 24;
      /* 转换为MOTO字序 */
      pixheader.width = EXCHANGE_WORD((pixheader.width));
      pixheader.height = EXCHANGE_WORD((pixheader.height));
      pixheader.bitcount = EXCHANGE_WORD((pixheader.bitcount));
      /* 写入流头结构 */
      if (isio_write((const void*)&pixheader, sizeof(PIX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 逐行编码（RLE8）并写入目标流 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        p = (uchar*)pinfo_str->pp_line_addr[i];
        if ((b_status = _write_line_alias24(pfile, p, (int)pinfo_str->width)) != ER_SUCCESS) {
          break;
        }
      }
    }
    while (0);
    
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 判断传入的图像是否可以被保存 */
int _pix_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = (1UL << (24 - 1))};
  /* 位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* 不支持的位深图像 */
  }
  assert(pinfo_str->imgnumbers);
      if (pinfo_str->imgnumbers != 1) {
        return -2; /* 图像个数不正确 */
      }
  return 0;
}
/* 读取一行24位RLE压缩数据 */
static enum EXERESULT _read_line_alias24(ISFILE* pfile, uchar* pix, int size)
{
  int i;
  int count = 0;
  uchar buffer[4];
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pix && size);
  do {
    while (count < size) {
      /* 读取一组压缩数据（每组4字节，第一个字节为重复计数值，后
      三个字节为B、G、R数据 */
      if (isio_read((void*)buffer, 1, 4, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 重复计数值 */
      count += (int)(unsigned int)(uchar)(buffer[0]);
      if (count > size) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 向数据包填写重复数据 */
      for (i = 0; i < (int)(unsigned int)(uchar)buffer[0]; i++) {
        *pix++ = buffer[1];
        *pix++ = buffer[2];
        *pix++ = buffer[3];
      }
    }
  }
  while (0);
  {
    ;
  }
  return b_status;
}
/* 读取一行8位RLE压缩数据 */
static enum EXERESULT _read_line_alias8(ISFILE* pfile, uchar* pix, int size)
{
  int i;
  int count = 0;
  uchar buffer[2];
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pix && size);
  do {
    while (count < size) {
      /* 读取一组压缩数据（每组2字节，第一个字节为重复计数值，后
      一个字节为颜色索引值 */
      if (isio_read((void*)buffer, 1, 2, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 重复计数值 */
      count += (int)(unsigned int)(uchar)(buffer[0]);
      if (count > size) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 向数据包填写重复数据 */
      for (i = 0; i < (int)(unsigned int)(uchar)buffer[0]; i++) {
        *pix++ = buffer[1];
      }
    }
  }
  while (0);
  {
    ;
  }
  return b_status;
}
/* 压缩一行数据到24位RLE */
static enum EXERESULT _write_line_alias24(ISFILE* pfile, uchar* pix, int size)
{
  int i;
  int count;
  uchar buffer[4];
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pix && size);
  do {
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
          buffer[0] = (uchar)count;
          pix += 3;
        }
        else {
          /* 写入一组压缩数据 */
          if (isio_write((const void*)buffer, 1, 4, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
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
        break;
      }
    }
  }
  while (0);
  {
    ;
  }
  return b_status;
}

