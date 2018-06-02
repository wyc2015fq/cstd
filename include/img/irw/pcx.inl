/********************************************************************
 pcx.c
 本文件用途： PCX图像读写模块实现文件
 读取功能：1、4、8、24位图像
 保存功能：1、4、8、24位图像
 本文件编写人： freedvlp feelfree##263.net（读代码编写人）
 ChK ChK##163.net（写代码编写人）
 YZ （改版工作）
 (代码修改)
 本文件版本： 30813
 最后修改于： 2003-8-13
 ----------------------------------------------------------------
 修正历史：
 2003-8 修正4位图未处理1位面板的错误()
 2003-7 读接口增加容错处理代码（Ehomsoft webmaster##ehom.net）
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2。引入内存防护概念。
 2001-8 发布新版代码。并扩展了该模块的保存功能，
 增强了模块容错性，提高了模块的读写速度。
 2001-4 完成了图像的保存功能（24位）
 2000-8 完成了读取图像部分的全部代码
 2000-6 第一个测试版发布
********************************************************************/


/* 文件判别标志 */
#define PCX_FLAG            0xa

/* PCX调色板项结构 */
typedef struct _tag_pcx_palette_item {
  uchar red;
  uchar green;
  uchar blue;
} PCX_PALITEM, *LPPCX_PALITEM;


/* PCX信息结构 */
typedef struct _tag_pcx_header {
  uchar  byManufacturer;
  uchar  byVersion;
  uchar  byEncoding;
  uchar  byBits;
  ushort wLeft;
  ushort wTop;
  ushort wRight;
  ushort wBottom;
  ushort wXResolution;
  ushort wYResolution;
  PCX_PALITEM    ptPalette[16];
  uchar  byReserved;
  uchar  byPlanes;
  ushort wLineBytes;
  ushort wPaletteType;
  ushort wScrWidth;
  ushort wScrDepth;
  uchar  byFiller[54];
}  PCX_HEADER;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 读入一行PCX_RLE数据，并解码到指定的缓冲区中 */
uchar* _pcx_read_line(char* p_buf, char* p_sou, int linesize, char* p_base, int data_size)
{
  int cur_offset = 0;
  uchar count, conv;
  while (cur_offset < linesize) {
    count = (uchar) * p_sou++;
    if ((count & 0xc0) == 0xc0) { /* 计数值 */
      count &= 0x3f;
      count = MIN(count, (linesize - cur_offset)); /* 容错处理 */
      conv = (uchar) * p_sou++; /* 内容 */
      while (count--) {
        p_buf[cur_offset++] = conv; /* 重复写入数据 */
      }
    }
    else { /* 内容 */
      p_buf[cur_offset++] = count;
    }
  }
  return (uchar*)p_sou; /* 返回下一行的读取地址，如果发生数据错，返回0 */
}
/* 写一行数据到PCX_RLE缓冲区 */
int _write_line(char* p_buf, char* p_sou, int linesize)
{
  int m = 0;
  int commsize = 0;
  uchar v, n;
  while (m < linesize) {
    v = p_sou[m];
    /* 取得相连且相同的字节数n */
    n = 1;
    while (((m + n) < linesize) && (n < 63) && (v == (uchar)(p_sou[m + n]))) {
      n++;
    }
    if (n == 1) { /* 未能?顾?*/
      if (v < 0xc0) {
        p_buf[commsize++] = v;
      }
      else {
        p_buf[commsize++] = (char)0xc1;
        p_buf[commsize++] = v;
      }
    }
    else { /* 压缩 */
      p_buf[commsize++] = (char)(n + 0xc0);
      p_buf[commsize++] = v;
    }
    m += n;
  }
  return commsize; /* 压缩后的数据行尺寸 */
}
/* 读取调色板数据 */
void _get_palette(INFOSTR* pinfo_str, PCX_HEADER* p_header, ISFILE* pfile)
{
  int i;
  PCX_PALITEM tmp_palbuf[256];
#if 0
  PALETTEENTRY palcolor[256];
  HDC hdc;
#endif
  assert(pfile);
  if (pinfo_str->bitcount <= 8) {
    pinfo_str->pal_count = 1UL << pinfo_str->bitcount;
    if (p_header->wPaletteType <= 1) { /* 彩色或单色（YZ: 0将被视为彩色） */
      if (p_header->byVersion == 3) { /* 使用系统调色板 */
#if 0
        hdc = GetDC((HWND)0);
        GetSystemPaletteEntries(hdc, 0, pinfo_str->pal_count, (LPPALETTEENTRY)palcolor);
        ReleaseDC((HWND)0, hdc);
        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          ((PALITEM*)(&pinfo_str->palette[i]))->blue = palcolor[i].peBlue;
          ((PALITEM*)(&pinfo_str->palette[i]))->green = palcolor[i].peGreen;
          ((PALITEM*)(&pinfo_str->palette[i]))->red = palcolor[i].peRed;
          ((PALITEM*)(&pinfo_str->palette[i]))->reserved = 0;
        }
#else
        /* 缺省为灰度调色板，可更改为Linux GUI系统的缺省调色板 */
        switch (pinfo_str->bitcount) {
        case 1:
          pinfo_str->palette[0] = 0;
          pinfo_str->palette[1] = 0xffffff;
          break;
        case 2:
          pinfo_str->palette[0] = 0;
          pinfo_str->palette[1] = 0x555555;
          pinfo_str->palette[2] = 0xaaaaaa;
          pinfo_str->palette[3] = 0xffffff;
          break;
        case 3:
          pinfo_str->palette[0] = 0;
          pinfo_str->palette[1] = 0x242424;
          pinfo_str->palette[2] = 0x484848;
          pinfo_str->palette[3] = 0x6c6c6c;
          pinfo_str->palette[4] = 0x909090;
          pinfo_str->palette[5] = 0xb4b4b4;
          pinfo_str->palette[6] = 0xd8d8d8;
          pinfo_str->palette[7] = 0xffffff;
          break;
        case 4:
          pinfo_str->palette[0] = 0;
          pinfo_str->palette[1] = 0x111111;
          pinfo_str->palette[2] = 0x222222;
          pinfo_str->palette[3] = 0x333333;
          pinfo_str->palette[4] = 0x444444;
          pinfo_str->palette[5] = 0x555555;
          pinfo_str->palette[6] = 0x666666;
          pinfo_str->palette[7] = 0x777777;
          pinfo_str->palette[8] = 0x888888;
          pinfo_str->palette[9] = 0x999999;
          pinfo_str->palette[10] = 0xaaaaaa;
          pinfo_str->palette[11] = 0xbbbbbb;
          pinfo_str->palette[12] = 0xcccccc;
          pinfo_str->palette[13] = 0xdddddd;
          pinfo_str->palette[14] = 0xeeeeee;
          pinfo_str->palette[15] = 0xffffff;
          break;
        case 8:
          for (i = 0; i < (int)256; i++) {
            pinfo_str->palette[i] = i | i << 8 | i << 16;
          }
          break;
        }
#endif
      }
      else { /* 自带调色板 */
        if (pinfo_str->bitcount == 8) {
          /* 读取256色调色板数据（流尾部）*/
          isio_seek(pfile, -256L * 3, SEEK_END);
          isio_read((void*)tmp_palbuf, 256 * 3, 1, pfile);
          /* 复制调色板数据（8位） */
          for (i = 0; i < (int)pinfo_str->pal_count; i++) {
            ((PALITEM*)(&pinfo_str->palette[i]))->blue = tmp_palbuf[i].blue;
            ((PALITEM*)(&pinfo_str->palette[i]))->green = tmp_palbuf[i].green;
            ((PALITEM*)(&pinfo_str->palette[i]))->red = tmp_palbuf[i].red;
            ((PALITEM*)(&pinfo_str->palette[i]))->reserved = 0;
          }
        }
        else {
          /* 复制调色板数据（1、2、3、4位） */
          for (i = 0; i < (int)pinfo_str->pal_count; i++) {
            ((PALITEM*)(&pinfo_str->palette[i]))->blue = p_header->ptPalette[i].blue;
            ((PALITEM*)(&pinfo_str->palette[i]))->green = p_header->ptPalette[i].green;
            ((PALITEM*)(&pinfo_str->palette[i]))->red = p_header->ptPalette[i].red;
            ((PALITEM*)(&pinfo_str->palette[i]))->reserved = 0;
          }
        }
      }
    }
    else { /* 灰度（2） */
      switch (pinfo_str->bitcount) {
      case 1:
        pinfo_str->palette[0] = 0;
        pinfo_str->palette[1] = 0xffffff;
        break;
      case 2:
        pinfo_str->palette[0] = 0;
        pinfo_str->palette[1] = 0x555555;
        pinfo_str->palette[2] = 0xaaaaaa;
        pinfo_str->palette[3] = 0xffffff;
        break;
      case 3:
        pinfo_str->palette[0] = 0;
        pinfo_str->palette[1] = 0x242424;
        pinfo_str->palette[2] = 0x484848;
        pinfo_str->palette[3] = 0x6c6c6c;
        pinfo_str->palette[4] = 0x909090;
        pinfo_str->palette[5] = 0xb4b4b4;
        pinfo_str->palette[6] = 0xd8d8d8;
        pinfo_str->palette[7] = 0xffffff;
        break;
      case 4:
        pinfo_str->palette[0] = 0;
        pinfo_str->palette[1] = 0x111111;
        pinfo_str->palette[2] = 0x222222;
        pinfo_str->palette[3] = 0x333333;
        pinfo_str->palette[4] = 0x444444;
        pinfo_str->palette[5] = 0x555555;
        pinfo_str->palette[6] = 0x666666;
        pinfo_str->palette[7] = 0x777777;
        pinfo_str->palette[8] = 0x888888;
        pinfo_str->palette[9] = 0x999999;
        pinfo_str->palette[10] = 0xaaaaaa;
        pinfo_str->palette[11] = 0xbbbbbb;
        pinfo_str->palette[12] = 0xcccccc;
        pinfo_str->palette[13] = 0xdddddd;
        pinfo_str->palette[14] = 0xeeeeee;
        pinfo_str->palette[15] = 0xffffff;
        break;
      case 8:
        for (i = 0; i < (int)256; i++) {
          pinfo_str->palette[i] = i | i << 8 | i << 16;
        }
        break;
      }
    }
  }
  else {
    pinfo_str->pal_count = 0; /* 无调色板数据 */
  }
}
/* 判断传入的图像是否可以被保存 */
int _pcx_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = 1UL | (1UL << (4 - 1)) | (1UL << (8 - 1)) | (1UL << (24 - 1)) };
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* 不支持的位深图像 */
  }
  assert(pinfo_str->imgnumbers);
  if (pinfo_str->imgnumbers != 1) {
    return -2; /* 图像个数不正确 */
  }
  return 0;
}


/* 获取图像信息 */
EXERESULT pcx_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PCX_HEADER phr;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
  
    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&phr, sizeof(PCX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 检测文件标志 */
      if (phr.byManufacturer != (uchar)PCX_FLAG) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 数据有效性检验 */
      if (((phr.wRight - phr.wLeft) == 0) || ((phr.wBottom - phr.wTop) == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_PCX; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_PCXRLE; /* 图像的压缩方式 */
      /* 填写图像信息 */
      pinfo_str->width = (ulong)((phr.wRight - phr.wLeft) + 1);
      pinfo_str->height = (ulong)((phr.wBottom - phr.wTop) + 1);
      pinfo_str->order = 0;
      pinfo_str->bitcount = phr.byBits * phr.byPlanes;
      /* 填写掩码数据 */
      if (pinfo_str->bitcount > 8) { /* 24位 */
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
      }
      else {
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
      }
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT pcx_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PCX_HEADER phr;
  int i, j, k, linesize, bufsize;
  ulong data_len;
  uchar* pbit = NULL;
  uchar* ptmp = NULL;
  uchar* plinebuf = NULL;
  uchar* psou, *pdesline;
  uchar* pla3, *pla2, *pla1, *pla0;
  uchar pb3, pb2, pb1, pb0, tbuf, tofs;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位??*/

    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&phr, sizeof(PCX_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 检测文件标志 */
      if (phr.byManufacturer != (uchar)PCX_FLAG) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 数据有效性检验 */
      if (((phr.wRight - phr.wLeft) == 0) || ((phr.wBottom - phr.wTop) == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
        pinfo_str->imgformat = IMF_PCX; /* 图像文件格式（后缀名） */
        pinfo_str->compression = ICS_PCXRLE; /* 图像的压缩方式 */
        /* 填写图像信息 */
        pinfo_str->width = (ulong)((phr.wRight - phr.wLeft) + 1);
        pinfo_str->height = (ulong)((phr.wBottom - phr.wTop) + 1);
        pinfo_str->order = 0;
        pinfo_str->bitcount = phr.byBits * phr.byPlanes;
        /* 填写掩码数据 */
        if (pinfo_str->bitcount > 8) { /* 24位 */
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else {
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
        }
        pinfo_str->data_state = 1;
      }
      /* 设置调色板数据 */
      _get_palette(pinfo_str, &phr, pfile);
      /* 取得图像数据长度 */
      data_len = isio_length(pfile) - sizeof(PCX_HEADER);
      if ((pinfo_str->bitcount == 8) && (phr.wPaletteType == 1)) {
        data_len -= 256 * 3 + 1; /* +1 : 0xc0 */
      }
      /* 取得扫描行尺寸 */
      linesize = DIBSCANLINE_WIDTHBYTES(pinfo_str->width * pinfo_str->bitcount);
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
      /* 初始化行首地址数组（PCX图像都为正向） */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      bufsize = (int)phr.wLineBytes * phr.byPlanes;
      /* 申请用于存放临时解压数据的缓冲区 */
      plinebuf = (uchar*)isirw_malloc(bufsize);
      if (plinebuf == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 读取位数据到内存 */
      if (isio_seek(pfile, sizeof(PCX_HEADER), SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 申请临时内存块，用于存放整个PCX图像数据 */
      ptmp = (uchar*)isirw_malloc(data_len);
      if (ptmp == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 读原始图像数据 */
      if (isio_read((void*)ptmp, data_len, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      psou = ptmp;
      pbit = pinfo_str->p_bit_data;
      /* 逐行读入 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        if ((psou = _pcx_read_line((char*)plinebuf, (char*)psou, bufsize, (char*)ptmp, data_len)) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 目标缓冲区地址 */
        pdesline = (uchar*)pinfo_str->pp_line_addr[i];
        switch (pinfo_str->bitcount) {
        case 1:
          /* 1位、1面板 */
          memcpy((void*)pdesline, (const void*)plinebuf, bufsize);
          break;
        case 2:
        case 3:
          /* 待修改。（未找到相关的面板设置资料，假设为1个面板）*/
          memcpy((void*)pdesline, (const void*)plinebuf, bufsize);
          break;
        case 4:
          /* 1位、4面板 */
          /* 1位面板处理 add by yuhe */ /* who is yuhe? */ /* 就是梦晖啦，不小心写了真名了，不改??*/
          if (phr.byPlanes == 1) {
            memcpy((void*)pdesline, (const void*)plinebuf, bufsize);
            break;
          }
          pla0 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 0));
          pla1 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 1));
          pla2 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 2));
          pla3 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 3));
          for (k = 0; k < (int)pinfo_str->width; k += 8) {
            pb0 = *pla0++;
            pb1 = *pla1++;
            pb2 = *pla2++;
            pb3 = *pla3++;
            /* 高位为左边的象素 */
            for (j = 7; (j >= 0) && ((k + (7 - j)) < (int)pinfo_str->width); j--) {
              tofs = 1 << j;
              tbuf = 0;
              tbuf |= (pb0 & tofs) ? 0x1 : 0;
              tbuf |= (pb1 & tofs) ? 0x2 : 0;
              tbuf |= (pb2 & tofs) ? 0x4 : 0;
              tbuf |= (pb3 & tofs) ? 0x8 : 0;
              if (j % 2) {
                tbuf <<= 4;
                *pdesline = tbuf;
              }
              else {
                *pdesline++ |= tbuf;
              }
            }
          }
          break;
        case 8:
          /* 8位图像数据直接拷贝 */
          memcpy((void*)pdesline, (const void*)plinebuf, bufsize);
          break;
        case 24:
          /* 8位、3面板 */
          pla0 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 0));
          pla1 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 1));
          pla2 = (uchar*)(plinebuf + ((int)phr.wLineBytes * 2));
          /* BGR顺序 */
          for (k = 0; k < (int)pinfo_str->width; k++) {
            *pdesline++ = *pla2++; /* B */
            *pdesline++ = *pla1++; /* G */
            *pdesline++ = *pla0++; /* R */
          }
          break;
        default:
          b_status = ER_BADIMAGE;
          break;
        }
      }
      pinfo_str->data_state = 2;
    }
    while (0);
      if (plinebuf) {
        isirw_free(plinebuf);
      }
      if (ptmp) {
        isirw_free(ptmp);
      }
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
EXERESULT pcx_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  PCX_HEADER phr;
  int i, j, k, expsize, commsize;
  uchar* psou, *pdes;
  uchar* pexpend = (uchar*)0;
  uchar* pexpend2 = (uchar*)0;
  uchar* pdesbuf = (uchar*)0;
  LPPCX_PALITEM p8pal = (LPPCX_PALITEM)0;
  uchar* pla3, *pla2, *pla1, *pla0;
  uchar pb3, pb2, pb1, pb0, tbuf;
  int cnv_count;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  
  do {
    /* 判断是否是受支持的图像数据 */
    if (_pcx_is_valid_img(pinfo_str) != 0) {
      b_status = ER_NSIMGFOR;
      break;
    }
    /* 设置PCX图像信息结构 */
    phr.byManufacturer = (uchar)PCX_FLAG;
    phr.byVersion = 5;
    phr.byEncoding = 1;
    phr.wLeft = 0;
    phr.wTop = 0;
    assert(pinfo_str->width);
    assert(pinfo_str->height);
    phr.wRight = (ushort)(pinfo_str->width - 1);
    phr.wBottom = (ushort)(pinfo_str->height - 1);
    phr.byReserved = 0;
    /* 缺省设置值，可修改 */
    phr.wXResolution = 300; /* DPI */
    phr.wYResolution = 300;
    phr.wScrWidth = 800; /* PIXEL */
    phr.wScrDepth = 600;
    memset((void*)phr.byFiller, 0, 54);
    switch (pinfo_str->bitcount) {
    case 1:
      phr.byBits = 1;
      phr.byPlanes = 1;
      phr.wLineBytes = (ushort)(((pinfo_str->width + 15) / 16) * 2);
      phr.wPaletteType = 1; /* 单色 */
      for (i = 0; i < 2; i++) {
        phr.ptPalette[i].blue = ((PALITEM*)(&pinfo_str->palette[i]))->blue;
        phr.ptPalette[i].green = ((PALITEM*)(&pinfo_str->palette[i]))->green;
        phr.ptPalette[i].red = ((PALITEM*)(&pinfo_str->palette[i]))->red;
      }
      expsize = phr.wLineBytes * 1;
      pexpend = (uchar*)isirw_malloc(expsize);
      pexpend2 = (uchar*)0;
      pdesbuf = (uchar*)isirw_malloc(phr.wLineBytes * 1 * 2); /* 按最糟的情况来分配内存 */
      if ((pexpend == 0) || (pdesbuf == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      break;
    case 4:
      phr.byBits = 1;
      phr.byPlanes = 4;
      phr.wLineBytes = (ushort)(((pinfo_str->width + 15) / 16) * 2);
      phr.wPaletteType = 1; /* 彩色 */
      for (i = 0; i < 16; i++) {
        phr.ptPalette[i].blue = ((PALITEM*)(&pinfo_str->palette[i]))->blue;
        phr.ptPalette[i].green = ((PALITEM*)(&pinfo_str->palette[i]))->green;
        phr.ptPalette[i].red = ((PALITEM*)(&pinfo_str->palette[i]))->red;
      }
      expsize = phr.wLineBytes * 4;
      pexpend = (uchar*)isirw_malloc(expsize);
      pexpend2 = (uchar*)isirw_malloc(DIBSCANLINE_WIDTHBYTES(pinfo_str->width * 4));
      pdesbuf = (uchar*)isirw_malloc(phr.wLineBytes * 4 * 2);
      if ((pexpend == 0) || (pdesbuf == 0) || (pexpend2 == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      break;
    case 8:
      phr.byBits = 8;
      phr.byPlanes = 1;
      phr.wLineBytes = (ushort)(((pinfo_str->width + 1) / 2) * 2);
      phr.wPaletteType = 0; /* 彩色 */
      expsize = phr.wLineBytes * 1;
      pexpend = (uchar*)isirw_malloc(expsize);
      pexpend2 = (uchar*)0;
      p8pal = (LPPCX_PALITEM)isirw_malloc(sizeof(PCX_PALITEM) * 256);
      pdesbuf = (uchar*)isirw_malloc(phr.wLineBytes * 1 * 2);
      if ((pexpend == 0) || (pdesbuf == 0) || (p8pal == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 调色板数据将在写图像数据后合??*/
      break;
    case 24:
      phr.byBits = 8;
      phr.byPlanes = 3;
      phr.wLineBytes = (ushort)(((pinfo_str->width + 1) / 2) * 2);
      phr.wPaletteType = 0; /* 彩色 */
      expsize = phr.wLineBytes * 3;
      pexpend = (uchar*)isirw_malloc(expsize);
      pexpend2 = (uchar*)isirw_malloc(DIBSCANLINE_WIDTHBYTES(pinfo_str->width * 3 * 8));
      pdesbuf = (uchar*)isirw_malloc(phr.wLineBytes * 3 * 2);
      if ((pexpend == 0) || (pdesbuf == 0) || (pexpend2 == 0)) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 无调色板数据 */
      break;
    default:
      assert(0);
      break;
    }
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 写入信息结构 */
    if (isio_write((const void*)&phr, sizeof(PCX_HEADER), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 写入?枷袷?*/
    for (i = 0; i < (int)(pinfo_str->height); i++) {
      psou = (uchar*)pinfo_str->pp_line_addr[i];
      pdes = pexpend;
      /* 展开为面板格式 */
      switch (pinfo_str->bitcount) {
      case 1:
        memcpy((void*)pdes, (const void*)psou, phr.wLineBytes);
        break;
      case 4:
        memcpy((void*)pexpend2, (const void*)psou, (int)((pinfo_str->width + 1) / 2));
        /* 初始化展开缓冲区 */
        memset((void*)pexpend, 0, phr.wLineBytes * 4);
        pla0 = (uchar*)(pexpend + (phr.wLineBytes * 0));
        pla1 = (uchar*)(pexpend + (phr.wLineBytes * 1));
        pla2 = (uchar*)(pexpend + (phr.wLineBytes * 2));
        pla3 = (uchar*)(pexpend + (phr.wLineBytes * 3));
        /* 将图像数据展开为4个面板 */
        for (k = 0, j = 7; k < (int)pinfo_str->width; k++, j--) {
          tbuf = *(uchar*)(pexpend2 + k / 2);
          if (k % 2) {
            tbuf &= 0xf;
          }
          else {
            tbuf >>= 4;
          }
          pb0 = ((tbuf & 0x1) >> 0) << j;
          pb1 = ((tbuf & 0x2) >> 1) << j;
          pb2 = ((tbuf & 0x4) >> 2) << j;
          pb3 = ((tbuf & 0x8) >> 3) << j;
          *pla0 |= pb0;
          *pla1 |= pb1;
          *pla2 |= pb2;
          *pla3 |= pb3;
          if (j == 0) {
            j = 8;
            pla0++;
            pla1++;
            pla2++;
            pla3++;
          }
        }
        break;
      case 8:
        /* 8位图像直接拷贝 */
        memcpy((void*)pdes, (const void*)psou, phr.wLineBytes);
        break;
      case 24:
        /* 字节内顺序翻转 */
        for (cnv_count = 0; cnv_count < (int)pinfo_str->width; cnv_count++) {
          *(uchar*)(pexpend2 + cnv_count * 3 + 2) = *(uchar*)(psou + cnv_count * 3 + 0);
          *(uchar*)(pexpend2 + cnv_count * 3 + 1) = *(uchar*)(psou + cnv_count * 3 + 1);
          *(uchar*)(pexpend2 + cnv_count * 3 + 0) = *(uchar*)(psou + cnv_count * 3 + 2);
        }
        /* 初始化展开缓冲区 */
        memset((void*)pexpend, 0, phr.wLineBytes * 3);
        pla0 = (uchar*)(pexpend + (phr.wLineBytes * 0)); /*R*/
        pla1 = (uchar*)(pexpend + (phr.wLineBytes * 1)); /*G*/
        pla2 = (uchar*)(pexpend + (phr.wLineBytes * 2)); /*B*/
        /* 将图像数据展开为3个面板--RGB */
        for (k = 0; k < (int)pinfo_str->width; k++) {
          *pla0++ = *(uchar*)(pexpend2 + k * 3 + 0);
          *pla1++ = *(uchar*)(pexpend2 + k * 3 + 1);
          *pla2++ = *(uchar*)(pexpend2 + k * 3 + 2);
        }
        break;
      }
      /* 使用RLE算法对展开后的行数据进行压缩 */
      commsize = _write_line((char*)pdesbuf, (char*)pexpend, expsize);
      /* 写入目标流 */
      if (isio_write((const void*)pdesbuf, commsize, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 如果是8位图像，则在流的最后加入256色调色板数据 */
      if ((pinfo_str->bitcount == 8) && ((i + 1) == (int)(pinfo_str->height))) {
        pdesbuf[0] = (uchar)0xc; /* 0xc 为标志符 */
        if (isio_write((const void*)pdesbuf, 1, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        for (k = 0; k < 256; k++) {
          p8pal[k].blue = ((PALITEM*)(&pinfo_str->palette[k]))->blue;
          p8pal[k].green = ((PALITEM*)(&pinfo_str->palette[k]))->green;
          p8pal[k].red = ((PALITEM*)(&pinfo_str->palette[k]))->red;
        }
        if (isio_write((const void*)p8pal, sizeof(PCX_PALITEM) * 256, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
    }
    }
    while (0);
    
    if (p8pal) {
      isirw_free(p8pal);
    }
    if (pexpend2) {
      isirw_free(pexpend2);
    }
    if (pexpend) {
      isirw_free(pexpend);
    }
    if (pdesbuf) {
      isirw_free(pdesbuf);
    }
    return b_status;
}