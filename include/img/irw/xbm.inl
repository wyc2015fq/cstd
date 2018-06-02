/********************************************************************
 xbm.c
 本文件用途： XBM图像读写模块实现文件
 读取功能：X10、X11格式XBM图像（对注释信息具有容错性）。
 保存功能：X11格式XBM
 本文件编写人： YZ
 本文件版本： 30927
 最后修改于： 2003-9-27
 ----------------------------------------------------------------
 修正历史：
 2003-9 修改保存函数对介质类型的判断（以适应新的ISeeIO运作机制）
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-9 插件接口升级至2.2。引入内存防护概念。
 2002-1 第一次发布（新版本，起始版本号2.0）
********************************************************************/

#ifdef _WIN32
#define MAX_PATH_LEN _MAX_PATH
#define MAX_DRIVE_LEN _MAX_DRIVE
#define MAX_DIR_LEN _MAX_DIR
#define MAX_FNAME_LEN _MAX_FNAME
#define MAX_EXT_LEN _MAX_EXT
#else
#define MAX_PATH_LEN 512 /* 应修改为Linux中的相应值 */
#define MAX_DRIVE_LEN 32
#define MAX_DIR_LEN 512
#define MAX_FNAME_LEN 512
#define MAX_EXT_LEN 512
#endif
//IRWP_INFO xbm_irwp_info; /* 插件信息表 */
static int hex_table[256]; /* 字符转数字表 */

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 判断传入的图像是否可以被保存 */
int _xbm_is_valid_img(INFOSTR* pinfo_str)
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
/* 初始化字符值表 */
void _init_hex_table()
{
  int i;
  for (i = (int)'0'; i <= (int)'9'; i++) {
    hex_table[i] = i - (int)'0';
  }
  for (i = (int)'a'; i <= (int)'f'; i++) {
    hex_table[i] = i - (int)'a' + 10;
  }
  for (i = (int)'A'; i <= (int)'F'; i++) {
    hex_table[i] = i - (int)'A' + 10;
  }
  hex_table['x'] = 0;
  hex_table['X'] = 0;
  hex_table[' '] = -1;
  hex_table[','] = -1;
  hex_table['}'] = -1;
  hex_table['\n'] = -1;
  hex_table['\t'] = -1;
}
/* 读取一个整数 */
int _read_integer(ISFILE* fp)
{
  int c, flag = 0, value = 0;
  while (1) {
    if (isio_read((void*)&c, 1, 1, fp) == -1) {
      return -1; /* 读错误或文件结束 */
    }
    c &= 0xff;
    if (isxdigit(c)) {
      value = (value << 4) + hex_table[c];
      flag++;
      continue;
    }
    if ((hex_table[c] < 0) && flag) { /* ','和空格的表值小于0 */
      break;
    }
  }
  return value;
}
/* 跨过无用字符（包括注释信息），直到有效数据位置 */
int _xbm_read_until_data(ISFILE* fp)
{
  char buff[4];
  long ioloc;
  int wanlev;
  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == -1) {
      return -1; /* 读错误或文件结束 */
    }
    if (buff[0] == ' ') {
      continue;
    }
    else if (buff[0] == '\t') {
      continue;
    }
    else if (buff[0] == '\n') {
      continue;
    }
    else if (buff[0] == '\r') {
      continue;
    }
    else if (buff[0] == '/') {
      if (isio_read((void*)buff, 1, 1, fp) == -1) {
        return -1;
      }
      if (buff[0] == '*') { /* 注释信息开始 */
        wanlev = 0;
        while (1) { /* 跨过注释信息 */
          if (isio_read((void*)buff, 1, 1, fp) == -1) {
            return -1;
          }
          if (buff[0] == '*') {
            wanlev = 1;
          }
          else {
            if (buff[0] != '/') {
              wanlev = 0;
            }
            else if ((buff[0] == '/') && (wanlev == 1)) {
              break;
            }
            continue;
          }
        }
      }
      else {
        ioloc = isio_tell(fp); /* 流读写位置后退一字节 */
        ioloc --;
        if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
          return -1;
        }
      }
      continue;
    }
    else {
      ioloc = isio_tell(fp); /* 流读写位置后退一字节 */
      ioloc --;
      if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
        return -1;
      }
      break;
    }
  }
  return 0;
}
/* 读一行数据 */
int _read_line(ISFILE* fp, char* buff)
{
  int i = 0;
  while (1) {
    if (isio_read((void*) & (buff[i]), 1, 1, fp) == -1) {
      return -1; /* 读错误或文件结束 */
    }
    if (buff[i] == '\r') {
      continue;
    }
    else if (buff[i] == '\n') { /* 去掉/r/n符号 */
      buff[i++] = (uchar)0;
      break;
    }
    else {
      i++;
    }
  }
  return i;
}


/* 获取图像信息 */
EXERESULT xbm_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char rwbuff[512]; /* 读写缓冲区 */
  char agb_name[256];
  unsigned int w, h;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
  
  do {
    /* 读取文件头结构 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 去除文件首部多余的空格、注释等字符 */
    if (_xbm_read_until_data(pfile) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 读入一行数据 */
    if (_read_line(pfile, rwbuff) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 提取图像宽度值 */
    if ((sscanf(rwbuff, "#define %s %u", agb_name, &w) == 2) &&
      (strlen(agb_name) >= 6) &&
      (strcmp(agb_name + strlen(agb_name) - 6, "_width") == 0)) {
      pinfo_str->width = (ulong)w;
    }
    else {
      b_status = ER_NONIMAGE;
      break;
    }
    /* 读入一行数据 */
    if (_read_line(pfile, rwbuff) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 提取图像高度值 */
    if ((sscanf(rwbuff, "#define %s %u", agb_name, &h) == 2) &&
      (strlen(agb_name) >= 7) &&
      (strcmp(agb_name + strlen(agb_name) - 7, "_height") == 0)) {
      pinfo_str->height = (ulong)h;
    }
    else {
      b_status = ER_NONIMAGE;
      break;
    }
    if ((w == 0) || (h == 0)) {
      b_status = ER_NONIMAGE;
      break;
    }
    pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
    pinfo_str->imgformat = IMF_XBM; /* 图像文件格式（后缀名） */
    pinfo_str->compression = ICS_XBM_TEXT;
    pinfo_str->order = 0; /* 正向 */
    pinfo_str->bitcount = 1; /* XBM为单色图，位深为 1 */
    pinfo_str->b_mask = 0; /* 索引图，无掩码数据 */
    pinfo_str->g_mask = 0;
    pinfo_str->r_mask = 0;
    pinfo_str->a_mask = 0;
    /* 设定数据包状态 */
    pinfo_str->data_state = 1;
  }
  while (0);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT xbm_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char rwbuff[512]; /* 读写缓冲区 */
  char agb_name[256], *p;
  unsigned int w, h, version;
  int i, linesize, result, j;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  
  do {
    /* 读文件定位 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
    if (pinfo_str->data_state == 0) {
      /* 去除文件首部多余的空格、注释等字符 */
      if (_xbm_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 读入一行数据 */
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 提取图像宽度值 */
      if ((sscanf(rwbuff, "#define %s %u", agb_name, &w) == 2) &&
        (strlen(agb_name) >= 6) &&
        (strcmp(agb_name + strlen(agb_name) - 6, "_width") == 0)) {
        pinfo_str->width = (ulong)w;
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 读入一行数据 */
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 提取图像高度值 */
      if ((sscanf(rwbuff, "#define %s %u", agb_name, &h) == 2) &&
        (strlen(agb_name) >= 7) &&
        (strcmp(agb_name + strlen(agb_name) - 7, "_height") == 0)) {
        pinfo_str->height = (ulong)h;
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 检查图像宽、高数据是否合法 */
      if ((w == 0) || (h == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_XBM; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_XBM_TEXT;
      pinfo_str->order = 0; /* 正向 */
      pinfo_str->bitcount = 1; /* XBM为单色图，位深为 1 */
      pinfo_str->b_mask = 0; /* 索引图，无掩码数据 */
      pinfo_str->g_mask = 0;
      pinfo_str->r_mask = 0;
      pinfo_str->a_mask = 0;
      pinfo_str->data_state = 1; /* 设置标记 */
    }
    else {
      /* 跨过文件宽高信息 */
      if (_xbm_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
    }
    /* 跨过热点信息行 */
    while (1) {
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (sscanf(rwbuff, "#define %s %u", agb_name, &w) == 2) {
        continue;
      }
      else {
        break;
      }
    }
    /* 设置调色板数据 */
    pinfo_str->pal_count = 2;
    /* 实际位数据为1，索引为0，所以0索引为白色，1索引为黑色 */
    pinfo_str->palette[0] = 0xffffffUL;
    pinfo_str->palette[1] = 0UL;
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
    version = 11; /* 缺省：X11 XBM 格式 */
    /* 判断XBM版本 */
    while (1) {
      if (sscanf(rwbuff, "static short %s = {", agb_name) == 1) {
        version = 10;
      }
      else {
        if (sscanf(rwbuff, "static uchar %s = {", agb_name) == 1) {
          version = 11;
        }
        else if (sscanf(rwbuff, "static char %s = {", agb_name) == 1) {
          version = 11;
        }
        else {
          continue;
        }
      }
      p = (char*)strrchr(agb_name, '_');
      /* 检查_bits[]标记 */
      if (p == NULL) {
        p = agb_name;
      }
      else {
        p++;
      }
      if (strcmp("bits[]", (char*)p) == 0) {
        break;
      }
      /* 读入一行数据 */
      if (_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
    }
    /* 初始化转换值表 */
    _init_hex_table();
    if (version == 11) {
      /* 逐行读入 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        for (j = 0; j < (int)(pinfo_str->width); j += 8) {
          result = _read_integer(pfile);
          if (result == -1) {
            b_status = ER_FILERWERR;
            break;
          }
          CVT_BITS1(result);
          *(uchar*)(pinfo_str->p_bit_data + (i * linesize) + j / 8) = (uchar)result;
        }
      }
    }
    else {
      uchar swa, swb;
      assert(version == 10);
      /* 逐行读入 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        for (j = 0; j < (int)(pinfo_str->width); j += 16) {
          result = _read_integer(pfile);
          if (result == -1) {
            b_status = ER_FILERWERR;
            break;
          }
          swa = (uchar)result;
          swb = (uchar)(result >> 8);
          CVT_BITS1(swa);
          CVT_BITS1(swb);
          result = (int)(unsigned int)swb;
          result <<= 8;
          result |= (int)(unsigned int)swa;
          *(ushort*)(pinfo_str->p_bit_data + (i * linesize) + j / 8) = (ushort)result;
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
EXERESULT xbm_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  static char path_buffer[MAX_PATH_LEN];
  static char drive[MAX_DRIVE_LEN];
  static char fname[MAX_FNAME_LEN];
  static char ext[MAX_EXT_LEN];
  static char dir[MAX_DIR_LEN];
  static char buffer[MAX_PATH_LEN];
  int i, j, linesize;
  uchar* p;
  uchar swa;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  
  do {
    /* 判断是否是受支持的图像数据 */
    if (_xbm_is_valid_img(pinfo_str) != 0) {
      b_status = ER_NSIMGFOR;
      break;
    }
#if 0
    /* 分解出文件名，以确定XBM中宽、高变量的名字 */
    if (pinfo_str->name) {
      strcpy(path_buffer, (const char*)pinfo_str->name); /* 文件 */
    }
    else {
      strcpy(path_buffer, (const char*)"temp_name"); /* 其它介质 */
    }
    _splitpath(path_buffer, drive, dir, fname, ext);
    /* 排除文件名中多余的'.'字符（该字符不能用于变量名）*/
    p = (uchar*)strchr((const char*)fname, (int)'.');
    if (p) {
      *p = 0;
    }
#endif
    /* 定位到文件首部 */
    if (isio_seek(pfile, 0, SEEK_SET) == -1) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 写入宽、高变量描述 */
    sprintf(buffer, "#define %s_width %u\n", fname, pinfo_str->width);
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    sprintf(buffer, "#define %s_height %u\n", fname, pinfo_str->height);
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    /* 注：本模块将不保存热点信息 */
    /* 保存格式为X11 */
    sprintf(buffer, "static uchar %s_bits[] = {\n", fname);
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    strcpy(buffer, (const char*)" ");
    if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
      b_status = ER_FILERWERR;
      break;
    }
    assert(pinfo_str->bitcount == 1);
    linesize = (pinfo_str->width + 7) / 8;
    /* 写入图像数据 */
    for (i = 0; i < (int)(pinfo_str->height); i++) {
      p = (uchar*)(pinfo_str->pp_line_addr[i]);
      for (j = 0; j < linesize; j++) {
        /* 结尾数据判断 */
        if (((i + 1) == (int)(pinfo_str->height)) && ((j + 1) == linesize)) {
          swa = *p++;
          CVT_BITS1(swa);
          sprintf(buffer, "0x%02x };", (uchar)swa);
          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else {
          swa = *p++;
          sprintf(buffer, "0x%02x, ", (uchar)swa);
          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      strcpy(buffer, "\n ");
      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
    }
  }
  while (0);
  return b_status;
}
