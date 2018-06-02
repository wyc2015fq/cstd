/********************************************************************
 xpm.c
 本文件用途： XPM图像读写模块实现文件
 读取功能：XPM v3格式图像（对注释信息具有容错性）。
 保存功能：1、4、8位图像（注：不保存热点信息）
 本文件编写人： YZ
 本文件版本： 30927
 最后修改于： 2003-09-27
 ----------------------------------------------------------------
 修正历史：
 2003-9 修改保存函数对介质类型的判断（以适应新的ISeeIO运作机制）
 2003-6 修正颜色字符串中有空格时出现的错误()
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-9 插件接口升级至2.2。引入内存防护概念。
 修正xpm_load_image()函数中存在的一处BUG。
 2002-1 第一次发布（新版本，起始版本号2.0）
********************************************************************/

/* XPM文件头标志串 */
#define XPM_MARK  "/* XPM */"

/* 跳过串中的空格符 */
#define SKIPSPACE(p)  do{while(((*(p))==' ')||((*(p))=='\t')) (p)++;}while(0)
/* 跳过串中的非空格符 */
#define SKIPNONSPACE(p) do{while(((*(p))!=' ')&&((*(p))!='\t')&&((*(p))!=0)&&((*(p))!='\"')) (p)++;}while(0)

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

/* XPM 图像颜色结构 */
typedef struct _tagXPMCOLOR {
  char      pix[8];   /* 象素描述符（最多为7个） */
  ulong rgb;    /* RGBA数据 */
  int       attrib;   /* 颜色属性－0：普通颜色、1：透明色 */
} XPMCOLOR;



#define CTSIZE 16 /* 颜色转换表，用于保存功能 */
static const char ct1[17] = " *.o#+@O$81ugcms";
//static ushort clbuff[256]; /* 颜色描述符表 */

/* 根据给定的象素描述符查找对应的RGB数据数组的索引值 */
static int _search_pix(XPMCOLOR* pcol, int ncol, char* pix, int cpp)
{
  int i;
  for (i = 0; i < ncol; i++) {
    if (memcmp((const void*)pcol[i].pix, (const void*)pix, cpp) == 0) {
      return i;
    }
  }
  return -1;
}

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 判断传入的图像是否可以被保存 */
int _xpm_is_valid_img(INFOSTR* pinfo_str)
{
  /* 位数减一，否则32位格式无法表示。?↙ONG为32位）*/
  enum {bitcount = 1UL | (1UL << (4 - 1)) | (1UL << (8 - 1)) };
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* 不支持的位深图像 */
  }
  assert(pinfo_str->imgnumbers);
      if (pinfo_str->imgnumbers != 1) {
        return -2; /* 图像个数不正确 */
      }
  return 0;
}
/* 跨过空白字符（包括空格、TAB符、回车换行符） */
int _skip_spec(ISFILE* fp)
{
  char buff[4];
  long ioloc;
  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
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
    else {
      ioloc = isio_tell(fp); /* 流读写位置后退一字节 */
      ioloc --;
      if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
        return -1;
      }
      break;
    }
  }
  return 0; /* 成功 */
}
/* 提取注释信息 */
int _distill_comm(ISFILE* fp, char* comm, int commsize)
{
  char buff[4];
  long ioloc;
  int wanlev, count;
  if (commsize < 5) {
    return -3; /* 给定缓冲区过小，起码需要5各字节 */
  }
  if (isio_read((void*)buff, 2, 1, fp) == 0) {
    return -1; /* 读错误或文件结束 */
  }
  buff[2] = 0;
  if (strcmp(buff, "/*")) {
    ioloc = isio_tell(fp); /* 流读写位置后退2字节 */
    ioloc -= 2;
    if (isio_seek(fp, ioloc, SEEK_SET) == -1) {
      return -1;
    }
    return -2; /* 当前位置不是注释信息 */
  }
  strcpy(comm, buff);
  wanlev = 0;
  count = 2;
  while (1) { /* 跨过注释信息 */
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
      return -1;
    }
    comm[count++] = buff[0];
    if (count == commsize) {
      return -4; /* 注释信息过长，已超出给定缓冲区尺寸 */
    }
    if (buff[0] == '*') {
      wanlev = 1;
    }
    else {
      if (buff[0] != '/') {
        wanlev = 0;
      }
      else if (wanlev == 1) {
        break;
      }
      continue;
    }
  }
  comm[count] = 0;
  return count; /* 成功 */
}
/* 跨过无用字符（包括注释信息），直到有效数据位置 */
int _read_until_data(ISFILE* fp)
{
  char buff[4];
  long ioloc;
  int wanlev;
  while (1) {
    if (isio_read((void*)buff, 1, 1, fp) == 0) {
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
      if (isio_read((void*)buff, 1, 1, fp) == 0) {
        return -1;
      }
      if (buff[0] == '*') { /* 注释信息开始 */
        wanlev = 0;
        while (1) { /* 跨过注释信息 */
          if (isio_read((void*)buff, 1, 1, fp) == 0) {
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
int _xpm_read_line(ISFILE* fp, char* buff)
{
  int i = 0;
  while (1) {
    if (isio_read((void*) & (buff[i]), 1, 1, fp) == 0) {
      return -1; /* 读错误或文件结束 */
    }
    if (buff[i] == '\r') {
      continue;
    }
    else if (buff[i] == '\n') { /* 去掉/r/n符号 */
      buff[i++] = (char)0;
      break;
    }
    else {
      i++;
    }
  }
  return i;
}
#define XPM_MONO_MASK 1
#define XPM_COLOR_MASK 2
#define XPM_GRAY_MASK 4
/* 解析颜色数据 */
int _parse_color_line(char* line, int cpp, char* colname, ulong* rgb)
{
  static char mbuf[128], cbuf[128], gbuf[128];
  ulong mrgb, crgb, grgb;
  int matt, catt, gatt;
  char* p, *coldat;
  char nametype;
  int result, bmask = 0;
  assert((line) && (strlen(line)));
  if ((p = strchr(line, (int)'\"')) == 0) {
    return -1; /* 源数据格式不正确 */
  }
  p++; /* 跨过前导引号 */
  if (strlen(p) <= (size_t)cpp) {
    return -1;
  }
  memcpy((void*)colname, (const void*)p, cpp); /* 复制象素描述符 */
  p += cpp; /* 跨过象素描述符 */
  matt = catt = gatt = 0;
  while (*p) {
    SKIPSPACE(p); /* 跳过空白符 */
    if (*p == '\"') {
      break;
    }
    nametype = *p;
    SKIPNONSPACE(p);
    SKIPSPACE(p);
    coldat = p;
    SKIPNONSPACE(p);
    switch (nametype) {
    case 'S':
    case 's': /* 跳过符号名称串 */
      continue;
    case 'M':
    case 'm':
      matt = result = _color_to_rgb(coldat, p - coldat, &mrgb);
      if ((matt == 0) || (matt == 1)) {
        bmask |= XPM_MONO_MASK;
      }
      break;
    case 'C':
    case 'c':
      if ((p = strchr(coldat, (int)'\"')) == 0) {
        assert(0);
        p = coldat;
        SKIPNONSPACE(p);
      }
      catt = result = _color_to_rgb(coldat, p - coldat, &crgb);
      if ((catt == 0) || (catt == 1)) {
        bmask |= XPM_COLOR_MASK;
      }
      break;
    case 'G':
    case 'g':
      gatt = result = _color_to_rgb(coldat, p - coldat, &grgb);
      if ((gatt == 0) || (gatt == 1)) {
        bmask |= XPM_GRAY_MASK;
      }
      break;
    default:
      return -1; /* 源数据格式不正确 */
    }
    if ((result != 1) && (result != 0) && (result != -1)) {
      assert(0); /* 系统不稳定 :) */
      return -2;
    }
  }
  /* 此处采用颜色的原则是：有彩色数据就不用灰度数据，有灰度
  数据就不用黑白数据，如果连黑白数据都没有则返回错误。*/
  if (bmask & XPM_COLOR_MASK) {
    *rgb = crgb;
    return catt;
  }
  else if (bmask & XPM_GRAY_MASK) {
    *rgb = grgb;
    return gatt;
  }
  else if (bmask & XPM_MONO_MASK) {
    *rgb = mrgb;
    return matt;
  }
  else {
    return -1; /* 源图像数据有缺损 */
  }
}
/* 读取图像文件中的所有颜色数据 */
int _read_color_line(XPMCOLOR* pcol, int ncol, int cpp, ISFILE* fp)
{
  static char buff[256];
  int i, result;
  for (i = 0; i < ncol; i++) {
    if (_xpm_read_line(fp, (char*)buff) == -1) {
      return -1; /* 文件读写错 */
    }
    result = _parse_color_line((char*)buff, cpp, pcol[i].pix, &(pcol[i].rgb));
    switch (result) {
    case -1:
      return -2; /* 文件数据错误 */
    case 0:
      pcol[i].attrib = 0;
      break;
    case 1:
      pcol[i].attrib = 1;
      break;
    case -2:
    default:
      assert(0);
      return -3; /* 系统不稳定 */
    }
  }
  return i;
}
/* 读取一行象素数据 */
int _read_pix_line(ulong* pixbuf, int width, int bitcount, XPMCOLOR* pcol, int ncol, int cpp, ISFILE* fp)
{
  static char filebuf[256], buf[8];
  char* p;
  int i, index;
  /* 去除多余的空格、注释字符 */
  if (_read_until_data(fp) == -1) {
    return -1; /* 文件读写错 */
  }
  if (_xpm_read_line(fp, filebuf) == -1) {
    return -1;
  }
  p = strchr(filebuf, '\"'); /* 定位到前导引号处 */
  if (p == 0) {
    return -2; /* 非法的象素格式 */
  }
  if (strlen(p) < 3) {
    return -3; /* 两个引号加一个象素符是最短行长度 */
  }
  p++; /* 跨过前导引号 */
  switch (bitcount) {
  case 1:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      if (index == -1) {
        return -2; /* 非法象素数据 */
      }
      assert(index < 2);
      if (i % 8) {
        ((uchar*)pixbuf)[i / 8] |= (uchar)index << (7 - (i % 8));
      }
      else {
        ((uchar*)pixbuf)[i / 8] = (uchar)((index & 0x1) << 7);
      }
      p += cpp;
    }
    break;
  case 4:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      if (index == -1) {
        return -2; /* 非法象素数据 */
      }
      assert(index < 16);
      if (i % 2) {
        ((uchar*)pixbuf)[i / 2] |= (uchar)index;
      }
      else {
        ((uchar*)pixbuf)[i / 2] = (uchar)((index & 0xf) << 4);
      }
      p += cpp;
    }
    break;
  case 8:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      if (index == -1) {
        return -2; /* 非法象素数据 */
      }
      assert(index < 256);
      ((uchar*)pixbuf)[i] = (uchar)index;
      p += cpp;
    }
    break;
  case 32:
    for (i = 0; i < width; i++) {
      index = _search_pix(pcol, ncol, p, cpp);
      if (index == -1) {
        return -2; /* 非法象素数据 */
      }
      pixbuf[i] = pcol[index].rgb;
      p += cpp;
    }
    break;
  default:
    assert(0);
    return -3; /* 系统不稳定 */
  }
  return 0; /* 成功 */
}


/* 获取图像信息 */
EXERESULT xpm_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char rwbuff[512]; /* 读写缓冲区 */
  char agb_name[256];
  int w, h, cpp, ncol, i;
  char* p;
  XPMCOLOR* pcol = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
  
    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 去除文件首部多余的空格字符 */
      if (_skip_spec(pfile) != 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 提取注释信息 */
      if (_distill_comm(pfile, rwbuff, 512) < 0) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 检查XPM文件首部标志 */
      if (strcmp(rwbuff, XPM_MARK) != 0) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 去除文件首部多余的空格、注释等字符 */
      if (_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 读入一行数据 */
      if (_xpm_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 提取图像数据变量名 */
      if (rwbuff[0] == 's') {
        if (sscanf(rwbuff, "static char * %s[] = {", agb_name) != 1) {
          b_status = ER_NONIMAGE;
          break;
        }
      }
      else if (rwbuff[0] == 'c') {
        if (sscanf(rwbuff, "char * %s[] = {", agb_name) != 1) {
          b_status = ER_NONIMAGE;
          break;
        }
      }
      else {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 清除可能携带的[]符号 */
      p = strrchr(agb_name, (int)'[');
      if (p) {
        *p = '\0';
      }
      /* 去除多余的空格、注释等字符 */
      if (_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 读入图像宽、高等数据 */
      if (_xpm_read_line(pfile, rwbuff) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 提取图像宽、高、CPP及颜色值 */
      if (sscanf(rwbuff, "\"%d %d %d %d", &w, &h, &ncol, &cpp) != 4) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 图像数据合法性检查 */
      /* 注：当CPP值大于7时本模块认为它是非法参数，因为在现实世界中
      不应该会有这样的图像CPP值。*/
      if ((w == 0) || (h == 0) || (cpp == 0) || (cpp > 7) || (ncol == 0)) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 去除?嘤嗟目崭瘛⒆⑹偷茸址?*/
      if (_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 申请图像颜色表 */
      pcol = (XPMCOLOR*)isirw_malloc(sizeof(XPMCOLOR) * ncol);
      if (!pcol) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 读取颜色信息 */
      if (_read_color_line(pcol, ncol, cpp, pfile) < 0) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 填写图像信息 */
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_XPM; /* 图像文?袷剑ê笞好?*/
      pinfo_str->compression = ICS_XPM_TEXT;
      pinfo_str->width = w;
      pinfo_str->height = h;
      pinfo_str->order = 0; /* 正向 */
      /* 搜索是否有透明色 */
      for (i = 0; i < ncol; i++)
        if (pcol[i].attrib == 1) {
          break;
        }
      if (i < ncol) { /* 有透明色则将图像转为32位，以容纳透明色 */
        pinfo_str->bitcount = 32;
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0xff000000;
      }
      else {
        if (ncol <= 2) {
          pinfo_str->bitcount = 1;
        }
        else if (ncol <= 16) {
          pinfo_str->bitcount = 4;
        }
        else if (ncol <= 256) {
          pinfo_str->bitcount = 8;
        }
        else {
          pinfo_str->bitcount = 32;
        }
        if (pinfo_str->bitcount == 32) { /* 无透明色32位图像 */
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0x0;
        }
        else { /* 调色板图像 */
          pinfo_str->b_mask = 0x0;
          pinfo_str->g_mask = 0x0;
          pinfo_str->r_mask = 0x0;
          pinfo_str->a_mask = 0x0;
        }
      }
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);

      if (pcol) {
        isirw_free(pcol);
      }
  return b_status;
}
/* 读取图像位数据 */
EXERESULT xpm_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  char rwbuff[512]; /* 读写缓冲区 */
  char agb_name[256];
  int w, h, cpp, ncol;
  int i, linesize, result;
  char* p;
  XPMCOLOR* pcol = 0;
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
        /* 去除文件首部多余的空格字符 */
        if (_skip_spec(pfile) != 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 提取注释信息 */
        if (_distill_comm(pfile, rwbuff, 512) < 0) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 检查XPM文件首部标志 */
        if (strcmp(rwbuff, XPM_MARK) != 0) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 去除文件首部多余的空格、注释等字符 */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 读入一行数据 */
        if (_xpm_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 提取图像数据变量名 */
        if (rwbuff[0] == 's') {
          if (sscanf(rwbuff, "static char * %s[] = {", agb_name) != 1) {
            b_status = ER_NONIMAGE;
            break;
          }
        }
        else if (rwbuff[0] == 'c') {
          if (sscanf(rwbuff, "char * %s[] = {", agb_name) != 1) {
            b_status = ER_NONIMAGE;
            break;
          }
        }
        else {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 清除可能携带的[]符号 */
        p = strrchr(agb_name, (int)'[');
        if (p) {
          *p = '\0';
        }
        /* 去除多余的空格、注释等字符 */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 读入一行数据 */
        if (_xpm_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 提取图像宽、高、CPP及颜色值 */
        if (sscanf(rwbuff, "\"%d %d %d %d", &w, &h, &ncol, &cpp) != 4) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 图像数据合法性检查 */
        /* 注：当CPP值大于7时本模块认为它是非法参数，因为在现实世界中
        没有这样的图像CPP值。*/
        if ((w == 0) || (h == 0) || (cpp == 0) || (cpp > 7) || (ncol == 0)) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 去除多余的空格、注释等字符 */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 申请图像颜色表 */
        pcol = (XPMCOLOR*)isirw_malloc(sizeof(XPMCOLOR) * ncol);
        if (!pcol) {
          b_status = ER_MEMORYERR;
          break;
        }
        /* 读取颜色信息 */
        if (_read_color_line(pcol, ncol, cpp, pfile) < 0) {
          b_status = ER_NONIMAGE;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
        pinfo_str->imgformat = IMF_XPM; /* 图像文件格式（后缀名） */
        pinfo_str->compression = ICS_XPM_TEXT;
        pinfo_str->width = w;
        pinfo_str->height = h;
        pinfo_str->order = 0; /* 正向 */
        /* 搜索是否有透明色 */
        for (i = 0; i < ncol; i++)
          if (pcol[i].attrib == 1) {
            break;
          }
        if (i < ncol) { /* 有透明色则将图像转为32位，以容纳透明色 */
          pinfo_str->bitcount = 32;
          pinfo_str->b_mask = 0xff;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->a_mask = 0xff000000;
        }
        else {
          if (ncol <= 2) {
            pinfo_str->bitcount = 1;
          }
          else if (ncol <= 16) {
            pinfo_str->bitcount = 4;
          }
          else if (ncol <= 256) {
            pinfo_str->bitcount = 8;
          }
          else {
            pinfo_str->bitcount = 32;
          }
          if (pinfo_str->bitcount == 32) { /* 无透明色32位图像 */
            pinfo_str->b_mask = 0xff;
            pinfo_str->g_mask = 0xff00;
            pinfo_str->r_mask = 0xff0000;
            pinfo_str->a_mask = 0x0;
          }
          else { /* 调色板图像 */
            pinfo_str->b_mask = 0x0;
            pinfo_str->g_mask = 0x0;
            pinfo_str->r_mask = 0x0;
            pinfo_str->a_mask = 0x0;
          }
        }
        pinfo_str->data_state = 1;
      }
      else {
        /* 去除文件首部多余的空格字符 */
        if (_skip_spec(pfile) != 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 提取注释信息 */
        if (_distill_comm(pfile, rwbuff, 512) < 0) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 去除文件首部多余?目崭瘛⒆⑹偷茸址?*/
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 读入变量名定义行 */
        if (_xpm_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 去除多余的空格、注释等字符 */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 读入图像宽、高、CPP等数据 */
        if (_xpm_read_line(pfile, rwbuff) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 提取图像宽、高、CPP及颜色值 */
        if (sscanf(rwbuff, "\"%d %d %d %d", &w, &h, &ncol, &cpp) != 4) {
          b_status = ER_NONIMAGE;
          break;
        }
        /* 宽、高等数据的合法性已经判断过了，所以此处将不再判断 */
        /* 去除多余的空格、注释等字符 */
        if (_read_until_data(pfile) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 申请图像颜色表 */
        pcol = (XPMCOLOR*)isirw_malloc(sizeof(XPMCOLOR) * ncol);
        if (!pcol) {
          b_status = ER_MEMORYERR;
          break;
        }
        /* 读取颜色信息 */
        if (_read_color_line(pcol, ncol, cpp, pfile) < 0) {
          b_status = ER_NONIMAGE;
          break;
        }
      }
      /* 去除?嘤嗟目崭瘛⒆⑹偷茸址ㄎ坏较笏厥荽?*/
      if (_read_until_data(pfile) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 设置调色板数据 */
      switch (pinfo_str->bitcount) {
      case 1:
        pinfo_str->pal_count = 2;
        break;
      case 4:
        pinfo_str->pal_count = 16;
        break;
      case 8:
        pinfo_str->pal_count = 256;
        break;
      default:
        pinfo_str->pal_count = 0;
        break;
      }
      if (pinfo_str->pal_count) {
        for (i = 0; i < ncol; i++) {
          pinfo_str->palette[i] = pcol[i].rgb;
        }
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

      /* 逐行读入 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        result = _read_pix_line((ulong*)pinfo_str->pp_line_addr[i], (int)pinfo_str->width, (int)pinfo_str->bitcount, pcol, ncol, cpp, pfile);
        if (result == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        else if ((result == -2) || (result == -3)) {
          b_status = ER_BADIMAGE;
          break;
        }
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pcol) {
        isirw_free(pcol);
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
            pinfo_str->data_state = 1; /* 自动降级 */
          }
        }
      }

  return b_status;
}
/* 保存图像 */
EXERESULT xpm_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  static char path_buffer[MAX_PATH_LEN];
  static char drive[MAX_DRIVE_LEN];
  static char fname[MAX_FNAME_LEN];
  static char ext[MAX_EXT_LEN];
  static char dir[MAX_DIR_LEN];
  static char buffer[MAX_PATH_LEN];
  int i, j, k, linesize, ncol, cpp, ctloc0, ctloc1;
  uchar* p, tmpc;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  
    do {
      /* 判断是否是受支持的图像数据 */
      if (_xpm_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
#if 0
      /* 分解出文件名，以确定XPM中宽、高变量的名字 */
      if (pinfo_str->name) {
        strcpy(path_buffer, (const char*)pinfo_str->name); /* 文件 */
      }
      else {
        strcpy(path_buffer, (const char*)"image"); /* 其它介质 */
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
      /* 写入文件首部标志串 */
      sprintf(buffer, "%s\n", XPM_MARK);
      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 写入变量名描述 */
      sprintf(buffer, "static char * %s_xpm[] = {\n", fname);
      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 只保存调色板图像，因为颜色数目过多的图像不适合用XPM格式保存 */
      assert(pinfo_str->pal_count <= 256);
      ncol = pinfo_str->pal_count; /* 取得颜色数 */
      cpp = (ncol <= CTSIZE) ? 1 : 2; /* 确定描述符宽度 */
      /* 写入宽、高等变量的描述 */
      sprintf(buffer, "\"%d %d %d %d\",\n", pinfo_str->width, pinfo_str->height, ncol, cpp);
      if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 写入颜色数据 */
      for (i = 0; i < ncol; i++) {
        ctloc0 = i / CTSIZE;
        ctloc1 = i % CTSIZE;
        if (cpp == 1) {
          sprintf(buffer, "\"%c c #%06X\",\n", ct1[ctloc1], pinfo_str->palette[i]);
          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else { /* cpp == 2 */
          sprintf(buffer, "\"%c%c c #%06X\",\n", ct1[ctloc0], ct1[ctloc1], pinfo_str->palette[i]);
          if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* 写入图像数据 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = (uchar*)(pinfo_str->pp_line_addr[i]);
        /* 写入前导引号 */
        buffer[0] = '\"';
        buffer[1] = '\0';
        if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 写入图像数据行 */
        switch (pinfo_str->bitcount) {
        case 1:
          assert(cpp == 1); /* 单色图CPP值必定为1 */
          for (j = 0; j < (int)pinfo_str->width; j += 8) {
            tmpc = *p++;
            for (k = 0; ((k < 8) && ((j + k) < (int)pinfo_str->width)); k++) {
              if ((tmpc >> (7 - k)) & 1) {
                buffer[0] = ct1[1];
              }
              else {
                buffer[0] = ct1[0];
              }
              buffer[1] = 0;
              if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
              }
            }
          }
          break;
        case 4:
          assert(cpp == 1);
          for (j = 0; j < (int)pinfo_str->width; j += 2) {
            tmpc = *p++;
            for (k = 0; ((k < 2) && ((j + k) < (int)pinfo_str->width)); k++) {
              buffer[0] = ct1[((tmpc >> ((1 - k) * 4)) & 0xf)];
              buffer[1] = 0;
              if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
              }
            }
          }
          break;
        case 8: /* 8位深图像CPP值为2 */
          assert(cpp == 2);
          for (j = 0; j < (int)pinfo_str->width; j++) {
            tmpc = *p++;
            buffer[0] = ct1[tmpc / CTSIZE];
            buffer[1] = ct1[tmpc % CTSIZE];
            buffer[2] = 0;
            if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
          }
          break;
        default:
          assert(0); /* 系统不稳定 */
          b_status = ER_SYSERR;
          break;
        }
        /* 结尾数据判断 */
        if ((i + 1) == (int)(pinfo_str->height)) {
          buffer[0] = '\"';
          buffer[1] = '}';
          buffer[2] = ';';
          buffer[3] = '\n';
          buffer[4] = 0;
        }
        else {
          buffer[0] = '\"';
          buffer[1] = ',';
          buffer[2] = '\n';
          buffer[3] = 0;
        }
        /* 写入后缀引号 */
        if (isio_write((const void*)buffer, strlen(buffer), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
    }
    while (0);
  return b_status;
}
