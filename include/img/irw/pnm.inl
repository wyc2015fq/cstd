/********************************************************************
 pnm.c
 本文件用途： PBM、PGM、PPM图像读写模块实现文件
 读取功能：PBM、PGM、PPM 文本及二进制图像
 保存功能：可将1、8、24位图像保存为对应的
 PBM、PGM、PPM 文本或二进制图像格式
 本文件编写人： YZ yzfree##yeah.net
 本文件版本： 30529
 最后修改于： 2003-5-29
 ----------------------------------------------------------------
 修正历史：
 2003-5 修正保存函数中有关保存参数设定的BUG(原代码与文档描述不符)
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2。引入内存防护概念。
 2002-2 第一个发布版（新版接口），插件版本2.0
********************************************************************/

static enum pnm_type /* PNM 类型定义 */
{
  PBM = 1,
  PGM,
  PPM
};
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 判断传入的图像是否可以被保存 */
static int _pnm_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = 1UL | (1UL<<(8-1)) | (1UL<<(24-1))};
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
/* 从指定流中读取一个非负整数 */
static int _read_number(ISFILE* fp)
{
  uchar buff[4]; /* 边界对齐，实际只使用第一个字节 */
  int number = 0;
  /* 跨过空白符号（包括ACSII码0x9~0xd和0x20）*/
  do {
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
      return -1; /* 读错误或流结束 */
    }
    /* 跨过注释信息（PNM中注释信息以#号开始，直到行尾）*/
    if (buff[0] == '#') {
      do {
        if (isio_read((void*)buff, 1, 1, fp) == 0) {
          return -1;
        }
      }
      while ((buff[0] != '\r') && (buff[0] != '\n'));
    }
  }
  while (isspace((int)(buff[0])));
  /* 读取数值 */
  do {
    number *= 10;
    number += buff[0] - '0';
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
      return -1;
    }
  }
  while (isdigit(buff[0]));
  return(number); /* 返回负数（-1）表示函数读取失败 */
}
/* 写一个非负数整数（mark为后缀标志：0 后缀空格符，1后缀 \n） */
static int _write_number(ISFILE* fp, unsigned int num, int mark)
{
  char buff[32];
  assert(num >= 0);
  _itoa(num, (char*)buff, 10); /* 移植时应转换为UNIX对应函数 */
  if (mark == 1) {
    strcat((char*)buff, (const char*)"\n");
  }
  else {
    strcat((char*)buff, (const char*)" ");
  }
  if (isio_write((const void*)buff, strlen(buff), 1, fp) == 0) {
    return -1; /* 写文件失败 */
  }
  return 0; /* 成功 */
}
/* PNM 流标志判别 */
static int _is_pnm_hahaha(char* pmc)
{
  char c;
  assert(pmc != 0);
  if (*pmc++ != 'P') {
    return -1; /* 非PNM图像 */
  }
  c = (*pmc) - '0';
  /*
  * P1 PBM, ascii
  * P2 PGM, ascii
  * P3 PPM, ascii
  * P4 PBM, binary
  * P5 PGM, binary
  * P6 PPM, binary
  */
  if ((c < 1) || (c > 6)) {
    return -1; /* 非PNM图像 */
  }
  return (int)c; /* 返回PNM格式标识符 1~6 */
}
/* 获取图像信息 */
EXERESULT pnm_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char buff[4];
  int kind, ascii_mark;
  int width, height, maxval;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
  
  do {
    /* 读取流头结构 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    memset((void*)buff, 0, 4);
    if (isio_read((void*)buff, 2, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 判断流是否是一个PNM图像 */
    if ((kind = _is_pnm_hahaha(buff)) == -1) {
      b_status = ER_NONIMAGE;
      break;
    }
    if (kind > 3) {
      ascii_mark = 0; /* 二进制格式 */
      kind -= 3;
    }
    else {
      ascii_mark = 1; /* ASCII 格式 */
    }
    /* 提取图像宽、高数据 */
    if ((width = _read_number(pfile)) == -1) {
      b_status = ER_BADIMAGE;
      break;
    }
    if ((height = _read_number(pfile)) == -1) {
      b_status = ER_BADIMAGE;
      break;
    }
    /* 宽、高值检测 */
    if ((width <= 0) || (height <= 0)) {
      b_status = ER_BADIMAGE;
      break;
    }
    /* PGM和PPM都存在一个最大颜色分量变量值 */
    if (kind != PBM) {
      if ((maxval = _read_number(pfile)) == -1) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* ISee系统不支持单个颜色分量大于8位的图像 */
      if ((maxval <= 0) || (maxval > 255)) {
        b_status = ER_BADIMAGE;
        break;
      }
    }
    else {
      maxval = 255;
    }
    /* 填写图像信息 */
    pinfo_str->imgtype = IMT_RESSTATIC; /* 图像流类型 */
    if (kind == PBM) { /* 图像流格式（后缀名） */
      pinfo_str->imgformat = IMF_PBM;
    }
    else if (kind == PGM) {
      pinfo_str->imgformat = IMF_PGM;
    }
    else if (kind == PPM) {
      pinfo_str->imgformat = IMF_PPM;
    }
    else {
      assert(0);
    }
    if (ascii_mark == 1) { /* 图像数据压缩方式 */
      pinfo_str->compression = ICS_PNM_TEXT;
    }
    else if (ascii_mark == 0) {
      pinfo_str->compression = ICS_PNM_BIN;
    }
    else {
      assert(0);
    }
    pinfo_str->width = width;
    pinfo_str->height = height;
    pinfo_str->order = 0; /* 正向 */
    pinfo_str->b_mask = 0x0;
    pinfo_str->g_mask = 0x0;
    pinfo_str->r_mask = 0x0;
    pinfo_str->a_mask = 0x0;
    if (kind == PBM) { /* 图像位深 */
      pinfo_str->bitcount = 1;
    }
    else if (kind == PGM) {
      pinfo_str->bitcount = 8; /* 调色板图 */
    }
    else if (kind == PPM) {
      pinfo_str->bitcount = 24;
      pinfo_str->b_mask = 0xff;
      pinfo_str->g_mask = 0xff00;
      pinfo_str->r_mask = 0xff0000;
      pinfo_str->a_mask = 0x0;
    }
    else {
      assert(0);
    }
    /* 设定数据包状态 */
    pinfo_str->data_state = 1;
  }
  while (0);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT pnm_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char buff[4], *p;
  int kind, ascii_mark, i, y, x, r, linesize, cr, cb, cg;
  int width, height, maxval;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  
  do {
    /* 读流定位 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 读PNM图像首部标志信息 */
    if (isio_read((void*)buff, 2, 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 判断流是否是一个PNM图像 */
    if ((kind = _is_pnm_hahaha(buff)) == -1) {
      b_status = ER_NONIMAGE;
      break;
    }
    if (kind > 3) {
      ascii_mark = 0; /* 二进制格式 */
      kind -= 3;
    }
    else {
      ascii_mark = 1; /* ASCII 格式 */
    }
    /* 提取图像宽、高数据 */
    if ((width = _read_number(pfile)) == -1) {
      b_status = ER_BADIMAGE;
      break;
    }
    if ((height = _read_number(pfile)) == -1) {
      b_status = ER_BADIMAGE;
      break;
    }
    /* PGM和PPM都存在一个最大颜色分量变量值 */
    if (kind != PBM) {
      if ((maxval = _read_number(pfile)) == -1) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* ISee系统不支持单个颜色分量大于8位的图像 */
      if ((maxval <= 0) || (maxval > 255)) {
        b_status = ER_BADIMAGE;
        break;
      }
    }
    else {
      maxval = 255;
    }
    /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
    if (pinfo_str->data_state == 0) {
      /* 宽、高值检测 */
      if ((width <= 0) || (height <= 0)) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 填写图像信息 */
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像流类型 */
      if (kind == PBM) { /* 图像流格式（后缀名） */
        pinfo_str->imgformat = IMF_PBM;
      }
      else if (kind == PGM) {
        pinfo_str->imgformat = IMF_PGM;
      }
      else if (kind == PPM) {
        pinfo_str->imgformat = IMF_PPM;
      }
      else {
        assert(0);
      }
      if (ascii_mark == 1) { /* 图像数据压缩方式 */
        pinfo_str->compression = ICS_PNM_TEXT;
      }
      else if (ascii_mark == 0) {
        pinfo_str->compression = ICS_PNM_BIN;
      }
      else {
        assert(0);
      }
      pinfo_str->width = width;
      pinfo_str->height = height;
      pinfo_str->order = 0; /* 正向 */
      pinfo_str->b_mask = 0x0;
      pinfo_str->g_mask = 0x0;
      pinfo_str->r_mask = 0x0;
      pinfo_str->a_mask = 0x0;
      if (kind == PBM) { /* 图像位深 */
        pinfo_str->bitcount = 1;
      }
      else if (kind == PGM) {
        pinfo_str->bitcount = 8; /* 调色板图 */
      }
      else if (kind == PPM) {
        pinfo_str->bitcount = 24;
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
      }
      else {
        assert(0);
      }
      pinfo_str->data_state = 1;
    }
    /* 设置调色板数据 */
    switch (pinfo_str->bitcount) {
    case 1:
      pinfo_str->pal_count = 2;
      pinfo_str->palette[0] = 0xffffff;
      pinfo_str->palette[1] = 0x0;
      break;
    case 8:
      pinfo_str->pal_count = 256;
      for (i = 0; i < 256; i++) {
        pinfo_str->palette[i] = (i << 16) | (i << 8) | (i << 0);
      }
      break;
    case 24:
      pinfo_str->pal_count = 0;
      break;
    default:
      assert(0);
      pinfo_str->pal_count = 0;
      break;
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
    /* 初始化行首数组 */
    for (i = 0; i < (int)(pinfo_str->height); i++) {
      pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
    }
    /* 计算颜色分量的缩放因子 */
    r = 255 / maxval;
    /* 逐行读入 */
    if (pinfo_str->compression == ICS_PNM_TEXT) {
      for (y = 0; y < height; y++) {
        /* 取?眯惺椎刂?*/
        p = (char*)pinfo_str->pp_line_addr[y];
        if (pinfo_str->imgformat == IMF_PBM) {
          memset((void*)p, 0, linesize); /* 因为下面只有‘或’操作，所以需要先初始化为0 */
          for (x = 0; x < width; x++) {
            do {
              if (isio_read((void*)buff, 1, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
              }
            }
            while (isspace((int)buff[0]));
            /* 苛刻的源数据检验 :) */
            if ((buff[0] != '0') && (buff[0] != '1')) {
              b_status = ER_BADIMAGE;
              break;
            }
            buff[0] -= '0';
            p[x / 8] |= buff[0] << (7 - (x % 8)); /* 头象素在字节低端(ISee format) */
          }
        }
        else if (pinfo_str->imgformat == IMF_PGM) {
          for (x = 0; x < width; x++) {
            if ((i = _read_number(pfile)) == -1) {
              b_status = ER_BADIMAGE;
              break;
            }
            if (i > maxval) {
              b_status = ER_BADIMAGE;
              break;
            }
            i *= r; /* 缩放颜色分量范围至 0～255 */
            p[x] = (char)i;
          }
        }
        else if (pinfo_str->imgformat == IMF_PPM) {
          for (x = 0; x < width; x++) {
            if (((cr = _read_number(pfile)) == -1) ||
              ((cg = _read_number(pfile)) == -1) ||
              ((cb = _read_number(pfile)) == -1)) {
              b_status = ER_BADIMAGE;
              break;
            }
            if ((cr > maxval) || (cg > maxval) || (cb > maxval)) {
              b_status = ER_BADIMAGE;
              break;
            }
            /* 缩放颜色分量范围至 0～255 */
            cr *= r;
            cg *= r;
            cb *= r;
            p[x * 3] = (char)cb;
            p[x * 3 + 1] = (char)cg;
            p[x * 3 + 2] = (char)cr; /* BGR 顺序 (ISee format) */
          }
        }
        else {
          assert(0);
        }
      }
    }
    else { /* ICS_PNM_BIN */
      for (y = 0; y < height; y++) {
        /* 取得行首地址 */
        p = (char*)pinfo_str->pp_line_addr[y];
        if (pinfo_str->imgformat == IMF_PBM) {
          if (isio_read((void*)p, (size_t)((width + 7) >> 3), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else if (pinfo_str->imgformat == IMF_PGM) {
          if (isio_read((void*)p, width, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* 与缩放因子相乘，得到0～255范围的颜色分量值 */
          for (x = 0; x < width; x++) {
            p[x] *= (char)r;
          }
        }
        else if (pinfo_str->imgformat == IMF_PPM) {
          if (isio_read((void*)p, (size_t)(width * 3), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* 交换R、B分量位置，使之符合ISee格式 */
          for (x = 0; x < width; x++) {
            buff[0] = p[x * 3];
            p[x * 3] = p[x * 3 + 2];
            p[x * 3 + 2] = buff[0];
            p[x * 3] *= (char)r; /* 缩放分量 */
            p[x * 3 + 1] *= (char)r;
            p[x * 3 + 2] *= (char)r;
          }
        }
        else {
          assert(0);
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
EXERESULT pnm_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int ascii_mark; /* 存储方式 */
  int kind; /* 保存格式（后缀名）*/
  char buffer[64], *p;
  int x, y, mark = 0;
  unsigned int num;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  
  do {
    /* 判断是否是受支持的图像数据 */
    if (_pnm_is_valid_img(pinfo_str) != 0) {
      b_status = ER_NSIMGFOR;
      break;
    }
    /* 定位到流首部 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 0 代表文本方式，1代表二进制方式 */
    ascii_mark = (pinfo_str->para_value[0] == 0) ? 1 : 0;
    /* 确定保存格式 */
    switch (pinfo_str->bitcount) {
    case 1:
      kind = PBM;
      break;
    case 8:
      kind = PGM;
      break;
    case 24:
      kind = PPM;
      break;
    default:
      assert(0); /* 本模块不能保存以上三种格式以外的图像 */
      b_status = ER_NSIMGFOR;
      break;
    }
    /* 写入流头部信息结构 */
    sprintf((char*)buffer, "P%d\n#Created by ISeeExplorer PNM Plug-in 2.2\n%d %d", ((ascii_mark == 1) ? kind : (kind + 3)), (int)pinfo_str->width, (int)pinfo_str->height);
    switch (kind) {
    case PBM:
      strcat((char*)buffer, (const char*)"\n");
      break;
    case PGM:
    case PPM:
      strcat((char*)buffer, (const char*)" 255\n");
      break;
    default:
      assert(0);
      b_status = ER_NSIMGFOR;
      break;
    }
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 写入图像数据 */
    if (ascii_mark == 1) { /* 文本方式 */
      if (kind == PBM) {
        for (y = 0; y < (int)pinfo_str->height; y++) {
          p = (char*)pinfo_str->pp_line_addr[y];
          for (x = 0; x < (int)pinfo_str->width; x++) {
            mark++;
            if (_write_number(pfile, (unsigned int)(uchar)((p[x / 8] & (1 << (7 - (x % 8)))) >> (7 - (x % 8))), ((mark < 32) ? 0 : 1)) == -1) {
              b_status = ER_FILERWERR;
              break;
            }
            /* 因为PNM说明书中规定每行数据的字符数不能超过70个字符所以设置mark变量 */
            if (mark >= 32) {
              mark = 0;
            }
          }
        }
      }
      else if (kind == PGM) {
        for (y = 0; y < (int)pinfo_str->height; y++) {
          p = (char*)pinfo_str->pp_line_addr[y];
          for (x = 0; x < (int)pinfo_str->width; x++) {
            ((ulong*)buffer)[0] = pinfo_str->palette[(unsigned int)(uchar)p[x]];
            /* 将RGB数据转换为灰度数据。灰度转换公式 (3*r+6*g+b)/10 */
            num = (((unsigned int)(uchar)buffer[2]) * 3 + ((unsigned int)(uchar)buffer[1]) * 6 + ((unsigned int)(uchar)buffer[0]) * 1) / (unsigned int)10;
            mark++;
            if (_write_number(pfile, num, ((mark < 16) ? 0 : 1)) == -1) {
              b_status = ER_FILERWERR;
              break;
            }
            if (mark >= 16) {
              mark = 0;
            }
          }
        }
      }
      else { /* if (kind == PPM) */
        for (y = 0; y < (int)pinfo_str->height; y++) {
          p = (char*)pinfo_str->pp_line_addr[y];
          for (x = 0; x < (int)pinfo_str->width; x++) {
            mark++;
            /* 写入R、G、B数据 */
            if ((_write_number(pfile, (unsigned int)(uchar)p[x * 3 + 2], 0) == -1) ||
              (_write_number(pfile, (unsigned int)(uchar)p[x * 3 + 1], 0) == -1) ||
              (_write_number(pfile, (unsigned int)(uchar)p[x * 3 + 0], ((mark < 5) ? 0 : 1)) == -1)) {
              b_status = ER_FILERWERR;
              break;
            }
            if (mark >= 5) {
              mark = 0;
            }
          }
        }
      }
    }
    else { /* 二进制方式 */
      assert(ascii_mark == 0);
      /* 二进制方式对每行字符数无要求 */
      if (kind == PBM) {
        for (y = 0; y < (int)pinfo_str->height; y++) {
          p = (char*)pinfo_str->pp_line_addr[y];
          if (isio_write((const void*)p, (int)(pinfo_str->width + 7) / 8, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;/* 写文件失败 */
          }
        }
      }
      else if (kind == PGM) {
        for (y = 0; y < (int)pinfo_str->height; y++) {
          p = (char*)pinfo_str->pp_line_addr[y];
          if (isio_write((const void*)p, pinfo_str->width, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;/* 写文件失败 */
          }
        }
      }
      else { /* if (kind == PPM) */
        for (y = 0; y < (int)pinfo_str->height; y++) {
          p = (char*)pinfo_str->pp_line_addr[y];
          for (x = 0; x < (int)pinfo_str->width; x++) {
            /* 写入R、G、B数据 */
            if ((isio_write((const void*) & (p[x * 3 + 2]), 1, 1, pfile) == 0) ||
              (isio_write((const void*) & (p[x * 3 + 1]), 1, 1, pfile) == 0) ||
              (isio_write((const void*) & (p[x * 3 + 0]), 1, 1, pfile) == 0)) {
              b_status = ER_FILERWERR;
              break;
            }
          }
        }
      }
    }
    }
    while (0);
    return b_status;
}