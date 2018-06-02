/********************************************************************
 lcd.c
 本文件用途： LCD图像读写模块实现文件
 读取功能：读取自定义结构 -》按(8位一个字节)列对齐方式存储的
 液晶点阵数据
 保存功能：自定义结构 -》按(8位一个字节)列对齐方式存储的液晶
 点阵数据（见.h文件中定义）
 本文件编写人： zhaojinpo (zhaojinpo##sina.com)
 本文件版本： 30902
 最后修改于： 2003-9-2
 ----------------------------------------------------------------
 修正历史：
 2003-9 第一次发布
********************************************************************/




/* 文件结构定义 */

/* MAGIC定义 */
#define LCD_HEADER_MARKER 0x0064636c          //"LCD "

/* 数据块位置 */
#define LCD_data_offset   0x010

/* 点阵图文件头结构 （Windows）*/
typedef struct _tag_lcd_file_header {
  ulong bfType;
  ulong Size;
  ushort  width;
  ushort  hight;
  ushort  bSize;                //液晶现在采用双单字节存储
  uchar bwidth;               //液晶现在采用单字节存储
  uchar bhight;               //液晶现在采用单字节存储
} LCD_FILE_HEADER;

/* 数据存储格式：数据存储1字节代表一列上8个点。

   --------lcd_file_header-------------
   ----ulong  bfType;   lcd文件标示
   ----ulong  Size;   液晶图形存储数据大小（不含头结构长度）
   ----ushort width;    图形宽度
   ----ushort hight;    图形高度
   ----ushort bSize;      我的液晶现在采用双单字节存储
   ----uchar  bwidth;     我的液晶现在采用单字节存储
   ----uchar  bhight;     我的液晶现在采用单字节存储
   ---------lcd列存储数据-------------

   ----uchar  data;       液晶单字节按列存储数据

     字节存储第1个----》第11个由列取数

     像素排列第1行 ：|0|1| 2 3 4 5 6 7 8 9 10
     |1|
     |2|
     |3|
     |4|
     |5|
     |6|
     像素排列第8行 ：|7|

*/


/* 内部助手函数 */
int _lcd_is_valid_img(INFOSTR* pinfo_str);
uchar _get_byte_bit(uchar my_byte, uchar bit);

/* 获取图像信息 */
EXERESULT lcd_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LCD_FILE_HEADER bfh;
  int style; /* 风格，0－WINDOWS，1－OS/2 */
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */

    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      if (bfh.bfType == LCD_HEADER_MARKER) {
        style = 0;
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 数据有效性检验 */
      if (style == 0) {
        if ((bfh.width == 0) || (bfh.hight == 0)) {
          b_status = ER_NONIMAGE;
          break;
        }
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_LCD; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_RGB;
      pinfo_str->width = bfh.width;
      pinfo_str->height = bfh.hight;
      pinfo_str->order = 0;
      pinfo_str->bitcount = 1;
      pinfo_str->b_mask = 0x0;
      pinfo_str->g_mask = 0x0;
      pinfo_str->r_mask = 0x0;
      pinfo_str->a_mask = 0x0;
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT lcd_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LCD_FILE_HEADER bfh;
  int style; /* 风格，0－WINDOWS */
  ulong LCD_data_len; /* LCD位数据尺寸 */
  int i, linesize, line = 0; /* 当前行 */
  unsigned int k;
  uchar tmp , j;
  uchar* pbit = (uchar*)0;
  uchar* p_pre_bit = (uchar*)0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */

    do {
      /* 读文件定位 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = (pinfo_str->data_state == 1) ? ER_FILERWERR : ER_NONIMAGE;
        break;
      }
      /* 判断是否是有效的位图文件 */
      if (bfh.bfType == LCD_HEADER_MARKER) {
        style = 0;
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        /* 数据有效性检验 */
        if (style == 0) {
          if ((bfh.width == 0) || (bfh.hight == 0)) {
            b_status = ER_NONIMAGE;
            break;
          }
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件?嘈?*/
        pinfo_str->imgformat = IMF_LCD; /* 图像文件格式（后缀名） */
        pinfo_str->compression = ICS_RGB;
        pinfo_str->width = bfh.width;
        pinfo_str->height = bfh.hight;
        pinfo_str->order = 0;
        pinfo_str->bitcount = 1;
        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;
        pinfo_str->data_state = 1;
      }
      /* 设置调色板数据 */
      if (pinfo_str->bitcount <= 8) {
        pinfo_str->pal_count = 1UL << pinfo_str->bitcount;
        if (style == 0) {
          pinfo_str->palette[0] = 0xff00; /* 绿色，最高字节置零 */
          pinfo_str->palette[1] = 0xffffff;
        }
      }
      else {
        pinfo_str->pal_count = 0;
      }
      /* 取得位数据长度 */
      LCD_data_len = isio_length(pfile) - 1;
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
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      /* 读取位数据到内存 */
      if (isio_seek(pfile, LCD_data_offset, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      pbit = pinfo_str->p_bit_data;
      /* 每次读取8行数据 分配8行数据缓冲区(其中列保护限制+8字节保护) */
      p_pre_bit = (uchar*)isirw_malloc(pinfo_str->width + 8);
      if (p_pre_bit == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      for (i = 0; i < (int)(pinfo_str->height);) {
        /* 读一组数据到缓冲区 */
        if (isio_read(p_pre_bit, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        j = 0;
        /* 逐行填充显示数据 */
        while (i < (int)(pinfo_str->height) && (j < 8)) {
          /* 由列抽取数据重新拼合为行数据 */
          for (k = 0; k < (int)(pinfo_str->width); k = k + 8) {
            tmp = (_get_byte_bit(*(p_pre_bit + k), j) << 7) | (_get_byte_bit(*(p_pre_bit + 1 + k), j) << 6) | (_get_byte_bit(*(p_pre_bit + 2 + k), j) << 5) | (_get_byte_bit(*(p_pre_bit + k + 3), j) << 4) | (_get_byte_bit(*(p_pre_bit + k + 4), j) << 3) | (_get_byte_bit(*(p_pre_bit + k + 5), j) << 2) | (_get_byte_bit(*(p_pre_bit + k + 6), j) << 1) | (_get_byte_bit(*(p_pre_bit + k + 7), j) << 0);
            *(uchar*)(pbit + (k >> 3)) = tmp;
          }
          i++;
          j++;
          /* 移动到新的位置4字节对齐 */
          pbit = (uchar*)(pbit + linesize);
        }
      }
      /* 清除内存 */
      isirw_free(p_pre_bit);
      p_pre_bit = 0;
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (p_pre_bit) {
        isirw_free(p_pre_bit);
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
EXERESULT lcd_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LCD_FILE_HEADER bfh;
  uchar* p_pre_bit = (uchar*)0;
  int i, k, linesize;
  typedef struct _tag_byte_bit {
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
  } BIT_DATA;
  union {
    uchar uc_data;
    BIT_DATA bit_data;
  } mylcd_bit;
  enum EXERESULT b_status = ER_SUCCESS;
      assert(pfile && pinfo_str);
      assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
      assert(pinfo_str->p_bit_data);

    do {
      /* 判断是否是受支持的图像数据 */
      if (_lcd_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* 填充文件头 */
      bfh.bfType = LCD_HEADER_MARKER;
      bfh.width = (ushort)pinfo_str->width;
      bfh.hight = (ushort)pinfo_str->height;
      bfh.Size = pinfo_str->width * ((pinfo_str->height + 7) / 8);
      bfh.bwidth = (uchar)pinfo_str->width;
      bfh.bhight = (uchar)pinfo_str->height;
      bfh.bSize = ((((ushort)bfh.Size) << 4) & 0x0f) | ((((ushort)bfh.Size) >> 4) & 0xf0);
      /* 写入文件头结构 */
      if (isio_write((const void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* 行扩充为8的倍数--分配内存 */
      p_pre_bit = (uchar*)isirw_malloc(8L * linesize * ((pinfo_str->height + 7) / 8) + 4);
      if (p_pre_bit == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        memmove((void*)(p_pre_bit + linesize * i), (const void*)(uchar*)(pinfo_str->pp_line_addr[i]), linesize);
      }
      /* 行-8行移动 */
      for (i = 0; i < (int)(pinfo_str->height); i = i + 8) {
        /* 列移动 */
        for (k = 0; k < (int)(pinfo_str->width); k++) {
          mylcd_bit.uc_data = 0;
          /* 由行抽取数据重新拼合为列数据 */
          mylcd_bit.bit_data.bit0 = _get_byte_bit(*(p_pre_bit + linesize * i + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit1 = _get_byte_bit(*(p_pre_bit + linesize * (i + 1) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit2 = _get_byte_bit(*(p_pre_bit + linesize * (i + 2) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit3 = _get_byte_bit(*(p_pre_bit + linesize * (i + 3) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit4 = _get_byte_bit(*(p_pre_bit + linesize * (i + 4) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit5 = _get_byte_bit(*(p_pre_bit + linesize * (i + 5) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit6 = _get_byte_bit(*(p_pre_bit + linesize * (i + 6) + k / 8) , (uchar)((7 - (k % 8))));
          mylcd_bit.bit_data.bit7 = _get_byte_bit(*(p_pre_bit + linesize * (i + 7) + k / 8) , (uchar)((7 - (k % 8))));
          if (isio_write((const void*)&mylcd_bit.uc_data, 1, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      /* 清除内存 */
      isirw_free(p_pre_bit);
      p_pre_bit = 0;
    }
    while (0);
    
      if (p_pre_bit) {
        isirw_free(p_pre_bit);
      }
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 判断传入的图像是否可以被保存（如不支持保存功能，可将本函数去掉） */
int _lcd_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = 1UL};
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
uchar _get_byte_bit(uchar my_byte, uchar bit)
{
  uchar result = 0;
  typedef struct _tag_byte_bit {
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
  } BIT_DATA;
  union {
    uchar uc_data;
    BIT_DATA bit_data;
  } my_bit;
  my_bit.uc_data = my_byte;
  switch (bit) {
  case 0x0:
    result = my_bit.bit_data.bit0;
    break;
  case 0x1:
    result = my_bit.bit_data.bit1;
    break;
  case 0x2:
    result = my_bit.bit_data.bit2;
    break;
  case 0x3:
    result = my_bit.bit_data.bit3;
    break;
  case 0x4:
    result = my_bit.bit_data.bit4;
    break;
  case 0x5:
    result = my_bit.bit_data.bit5;
    break;
  case 0x6:
    result = my_bit.bit_data.bit6;
    break;
  case 0x7:
    result = my_bit.bit_data.bit7;
    break;
  }
  return result; /* 位数据 */
}

