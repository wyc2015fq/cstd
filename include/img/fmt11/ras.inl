// Sun RAS图像读写模块实现文件
// 读取功能：可读取1、8、24、32位压缩与未压缩RAS图象
// 保存功能：提供24位非压缩格式的保存功能

/* 文件判别标志 */
#define RAS_MAGIC           0x59a66a95

/* 文件结构定义 */
#define RT_OLD          0     /* 未压缩格式 */
#define RT_STANDARD       1     /* 未压缩格式 */
#define RT_BYTE_ENCODED     2     /* RLE压缩格式 */
#define RT_FORMAT_RGB     3     /* RGB格式 */
#define RT_FORMAT_TIFF      4
#define RT_FORMAT_IFF     5

#define RT_EXPERIMENTAL     0xFFFF

#define RMT_NONE        0     /* 无调色板数据 */
#define RMT_EQUAL_RGB     1
#define RMT_RAW         2

#define RESC          0x80    /* RLE压缩标识字 */
#define RAS_ROUNDROW      16      /* 扫描行对齐位数 */


/* RAS图象头结构 */
typedef struct _SUNHeaderinfo {
  unsigned long ras_magic;        /* 判别标志 */
  unsigned long ras_width;        /* 宽度 */
  unsigned long ras_height;       /* 高度 */
  unsigned long ras_depth;        /* 位深度 */
  unsigned long ras_length;       /* 图象数据长度（不包括头结构及可能的调色板）*/
  unsigned long ras_type;       /* 格式类型 */
  unsigned long ras_maptype;      /* 调色板数据存放方式 */
  unsigned long ras_maplength;      /* 调色板长度（字节计）*/
} SUNHeaderinfo, *LPSUNHeaderinfo;



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 判断传入的图像是否可以被保存（如不支持保存功能，可将本函数去掉） */
int CALLAGREEMENT _ras_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* 位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(ras_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* 不支持的位深图像 */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (ras_irwp_info.irwp_save.img_num) {
    if (ras_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* 图像个数不正确 */
      }
    }
  }

  return 0;
}


/* 读取指定数目的象素数据至缓冲区*/
static enum EXERESULT CALLAGREEMENT _read_data(ISFILE* pfile,
    unsigned char* buf,
    long length,
    int rle,
    int init)
{
  static unsigned char repchar, remaining;

  enum EXERESULT  b_status = ER_SUCCESS;

  /* 初始化变量值 */
  if (init) {
    repchar = remaining = 0;  /* 静态变量，重复调用时需要适时的初始化 */

    return b_status;
  }

  __try {
    __try {
      /* 如果是RLE压缩数据，则先解码 */
      if (rle) {
        while (length--) {
          if (remaining) {
            remaining--;
            *(buf++) = repchar;
          }
          else {
            /* 读取标识字节 */
            if (isio_read((void*)&repchar, 1, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              __leave;
            }

            /* 判断是否是有压缩可能的字段 */
            if (repchar == RESC) {
              if (isio_read((void*)&remaining, 1, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                __leave;
              }

              /* 压缩内容为1字节的0x80 */
              if (remaining == 0) {
                *(buf++) = RESC;
              }
              else { /* 重复内容 */
                /* 提取重复字节 */
                if (isio_read((void*)&repchar, 1, 1, pfile) == 0) {
                  b_status = ER_FILERWERR;
                  __leave;
                }

                *(buf++) = repchar;
              }
            }
            else { /* 未压缩单字节内容 */
              *(buf++) = repchar;
            }
          }
        }
      }
      else { /* 未压缩图象 */
        if (isio_read((void*)buf, length, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }
      }
    }
    __finally {
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_FILERWERR;
  }

  return b_status;
}

/* 读取图像位数据 */
RAS_API int CALLAGREEMENT ras_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       linesize, linelength, fill, i, j, numcolors, isrle, isrgb, fillchar;
  unsigned char c, *r = 0, *g, *b, *colormap = 0, *buf = 0, *bp, *bits;
  SUNHeaderinfo header;
  LPPALITEM   pal;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&ras_load_img_critical);

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

      if (isio_read((void*)&header, sizeof(SUNHeaderinfo), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 转至X86字序 */
      header.ras_magic     = EXCHANGE_DWORD((header.ras_magic));
      header.ras_width     = EXCHANGE_DWORD((header.ras_width));
      header.ras_height    = EXCHANGE_DWORD((header.ras_height));
      header.ras_depth     = EXCHANGE_DWORD((header.ras_depth));
      header.ras_length    = EXCHANGE_DWORD((header.ras_length));
      header.ras_type      = EXCHANGE_DWORD((header.ras_type));
      header.ras_maptype   = EXCHANGE_DWORD((header.ras_maptype));
      header.ras_maplength = EXCHANGE_DWORD((header.ras_maplength));

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        if (header.ras_magic != RAS_MAGIC) {
          b_status = ER_NONIMAGE;
          __leave;
        }

        if ((header.ras_type != RT_OLD) &&
            (header.ras_type != RT_STANDARD) &&
            (header.ras_type != RT_BYTE_ENCODED) &&
            (header.ras_type != RT_FORMAT_RGB)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        if ((header.ras_width == 0) || (header.ras_height == 0) || (header.ras_depth == 0)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        switch (header.ras_depth) {
        case  1:
        case  8:
        case  24:
        case  32:
          break;

        default:
          b_status = ER_BADIMAGE;
          __leave;  /* 插件将不支持以上四种格式之外的图象*/
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
        pinfo_str->imgformat  = IMF_RAS;      /* 图像文件格式（后缀名） */

        switch (header.ras_type) {        /* 图像的压缩方式 */
        case RT_OLD:
        case RT_STANDARD:
        case RT_FORMAT_RGB:
          pinfo_str->compression = ICS_RGB;
          break;

        case RT_BYTE_ENCODED:
          pinfo_str->compression = ICS_RLE8;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
        }

        pinfo_str->width  = header.ras_width;
        pinfo_str->height = header.ras_height;
        pinfo_str->order  = 0;
        pinfo_str->bitcount = header.ras_depth;

        switch (pinfo_str->bitcount) {
        case  1:
        case  8:
          SETMASK_8(pinfo_str);
          break;

        case  24:
          SETMASK_24(pinfo_str);
          break;

        case  32:
          SETMASK_32(pinfo_str);
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          __leave;
        }

        pinfo_str->data_state = 1;
      }

      isrle = 0;
      isrgb = 0;

      /* 设定象素数据存放方式（是否压缩、及RGB排放顺序）*/
      switch (header.ras_type) {
      case RT_OLD:
      case RT_STANDARD:   /* 非压缩、BGR排列 */
        break;

      case RT_BYTE_ENCODED: /* 压缩 */
        isrle = 1;
        break;

      case RT_FORMAT_RGB:   /* 非压缩、RGB排列 */
        isrgb = 1;
        break;

      case RT_FORMAT_TIFF:  /* 不支持的存储格式 */
      case RT_FORMAT_IFF:
      default:
        b_status = ER_BADIMAGE;
        __leave;
        break;
      }


      /* 设置调色板数据 */
      switch (pinfo_str->bitcount) {
      case  1:
      case  8:
        pinfo_str->pal_count = numcolors = 1 << header.ras_depth;

        switch (header.ras_maptype) {
        case RMT_NONE:      /* 人工合成灰度调色板 */
          pal = (LPPALITEM)pinfo_str->palette;

          for (i = 0; i < numcolors; i++) {
            c = (unsigned char)((255 * i) / (numcolors - 1));

            pal[i].red   = c;
            pal[i].green = c;
            pal[i].blue  = c;
            pal[i].reserved = 0;
          }

          break;

        case RMT_EQUAL_RGB:   /* 读取调色板数据 */
          pal = (LPPALITEM)pinfo_str->palette;

          /* 判断图象调色板长度域的值是否合法 */
          if (header.ras_maplength > (3 * numcolors * sizeof(unsigned char))) {
            b_status = ER_BADIMAGE;
            __leave;
            break;
          }

          /* 按最大可能分配调色板缓冲区 */
          if ((r = (unsigned char*)isirw_malloc(3 * numcolors * sizeof(unsigned char))) == 0) {
            b_status = ER_MEMORYERR;
            __leave;
            break;
          }

          /* 计算实际调色板项个数 */
          numcolors = header.ras_maplength / 3;

          g = r + numcolors;
          b = g + numcolors;

          /* 读入调色板数据 */
          if (isio_read((void*)r, header.ras_maplength, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
            break;
          }

          for (i = 0; i < numcolors; i++) {
            pal[i].red   = r[i];
            pal[i].green = g[i];
            pal[i].blue  = b[i];
            pal[i].reserved = 0;
          }

          break;

        case RMT_RAW:     /* 跳过裸调色板数据 */

          /* 未能搞清这种调色板的格式，暂时跳过 */
          if ((colormap = (unsigned char*)isirw_malloc(header.ras_maplength * sizeof(unsigned char))) == 0) {
            b_status = ER_MEMORYERR;
            __leave;
            break;
          }

          if (isio_read((void*)colormap, header.ras_maplength, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
            break;
          }

          break;

        default:        /* 非法的调色板标签值 */
          b_status = ER_BADIMAGE;
          __leave;
          break;
        }

        break;

      case  24:
      case  32:

        /* 如果图象中存在调色板数据，则说明是一个不正常的图象 */
        if (header.ras_maptype != RMT_NONE) {
          b_status = ER_BADIMAGE;
          __leave;
          break;
        }

        pinfo_str->pal_count = 0;

        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }


      /* 取得扫描行尺寸 */
      linesize = DIBSCANLINE_WIDTHBYTES(pinfo_str->width * pinfo_str->bitcount);

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

      /* 初始化行首地址数组（RAS图像都为正向） */
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

      if (header.ras_depth == 1) {
        linelength = (int)((header.ras_width / 8) + (header.ras_width % 8 ? 1 : 0));
      }
      else {
        linelength = (int)header.ras_width;
      }

      /* RAS扫描行为双字节对齐 */
      fill = (linelength % 2) ? 1 : 0;


      /* 初始化读取函数 */
      _read_data(pfile, 0, 0, 0, 1);

      /* 逐行读入 */
      switch (pinfo_str->bitcount) {
      case 1:
      case 8:
        for (i = 0; i < (int)pinfo_str->height; i++) {
          bits = pinfo_str->p_bit_data + i * linesize;

          /* 读入一行的数据 */
          if ((b_status = _read_data(pfile, bits, linelength, isrle, 0)) != ER_SUCCESS) {
            __leave;
            break;
          }

          /* 如果有的话，读取行尾填充字节 */
          if (fill) {
            if ((b_status = _read_data(pfile, (unsigned char*)&fillchar, fill, isrle, 0)) != ER_SUCCESS) {
              __leave;
              break;
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

        break;

      case 24:

        /* 分配缓冲区 */
        if ((buf = (unsigned char*)isirw_malloc(pinfo_str->width * 3)) == 0) {
          b_status = ER_MEMORYERR;
          __leave;
          break;
        }

        for (i = 0; i < (int)pinfo_str->height; i++) {
          bits = pinfo_str->p_bit_data + i * linesize;

          if ((b_status = _read_data(pfile, buf, pinfo_str->width * 3, isrle, 0)) != ER_SUCCESS) {
            __leave;
            break;
          }

          bp = buf;

          for (j = 0; j < (int)pinfo_str->width; j++) {
            bits[2] = *bp++; /* red */
            bits[1] = *bp++; /* green */
            bits[0] = *bp++; /* blue */

            bits += 3;
          }

#if 0

          /* 下面这段代码与实测图象格式冲突，isrgb为假时象素仍为RGB排列！*/
          /* 暂时废弃这段代码 */
          if (isrgb) {  /* RGB排列 */
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[2] = *bp++; /* red */
              bits[1] = *bp++; /* green */
              bits[0] = *bp++; /* blue */

              bits += 3;
            }
          }
          else {    /* BGR排列 */
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[0] = *bp++; /* blue */
              bits[1] = *bp++; /* green */
              bits[2] = *bp++; /* red */

              bits += 3;
            }
          }

#endif

          if (fill) {
            if ((b_status = _read_data(pfile, (unsigned char*)&fillchar, fill, isrle, 0)) != ER_SUCCESS) {
              __leave;
              break;
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

        break;

      case 32:

        /* 分配缓冲区 */
        if ((buf = (unsigned char*)isirw_malloc(pinfo_str->width * 4)) == 0) {
          b_status = ER_MEMORYERR;
          __leave;
          break;
        }

        for (i = 0; i < (int)pinfo_str->height; i++) {
          bits = pinfo_str->p_bit_data + i * linesize;

          if ((b_status = _read_data(pfile, buf, pinfo_str->width * 4, isrle, 0)) != ER_SUCCESS) {
            __leave;
            break;
          }

          bp = buf;

          for (j = 0; j < (int)pinfo_str->width; j++) {
            bits[3] = *bp++; /* empty */
            bits[2] = *bp++; /* red */
            bits[1] = *bp++; /* green */
            bits[0] = *bp++; /* blue */

            bits += 4;
          }

#if 0

          /* 注释原因见上 */
          if (isrgb) {  /* RGB排列 */
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[3] = *bp++; /* empty */
              bits[2] = *bp++; /* red */
              bits[1] = *bp++; /* green */
              bits[0] = *bp++; /* blue */

              bits += 4;
            }
          }
          else {    /* BGR排列 */
            for (j = 0; j < (int)pinfo_str->width; j++) {
              bits[3] = *bp++; /* empty */
              bits[0] = *bp++; /* blue */
              bits[1] = *bp++; /* green */
              bits[2] = *bp++; /* red */

              bits += 4;
            }
          }

#endif

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

      pinfo_str->data_state = 2;
    }
    __finally {
      if (buf) {
        isirw_free(buf);
      }

      if (colormap) {
        isirw_free(colormap);
      }

      if (r) {
        isirw_free(r);
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

      LeaveCriticalSection(&ras_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 保存图像 */
RAS_API int CALLAGREEMENT ras_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;

  int       i, j, wlen;
  unsigned char* bits, *pbuf = 0;
  SUNHeaderinfo header;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&ras_save_img_critical);

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
      if (_ras_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* 如果存在同名流，本函数将不进行确认提示 */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 填写头结构 */
      header.ras_magic     = RAS_MAGIC;
      header.ras_width     = (int)pinfo_str->width;
      header.ras_height    = (int)pinfo_str->height;
      header.ras_depth     = (int)pinfo_str->bitcount;
      header.ras_length    = (header.ras_width * 3 + ((header.ras_width * 3) % 2)) * header.ras_height;
      header.ras_type      = RT_STANDARD;
      header.ras_maptype   = RMT_NONE;
      header.ras_maplength = 0;

      /* 转至68K字序 */
      header.ras_magic     = EXCHANGE_DWORD((header.ras_magic));
      header.ras_width     = EXCHANGE_DWORD((header.ras_width));
      header.ras_height    = EXCHANGE_DWORD((header.ras_height));
      header.ras_depth     = EXCHANGE_DWORD((header.ras_depth));
      header.ras_length    = EXCHANGE_DWORD((header.ras_length));
      header.ras_type      = EXCHANGE_DWORD((header.ras_type));
      header.ras_maptype   = EXCHANGE_DWORD((header.ras_maptype));
      header.ras_maplength = EXCHANGE_DWORD((header.ras_maplength));

      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 写入头结构到目标流 */
      if (isio_write((const void*)&header, sizeof(SUNHeaderinfo), 1, pfile) == 0) {
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

      wlen = pinfo_str->width * 3 + ((pinfo_str->width * 3) % 2);

      /* 申请写缓冲区（扫描行）*/
      if ((pbuf = (unsigned char*)isirw_malloc(wlen + 4)) == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 写入图像数据（传入的图像数据必须是ISee位流格式，否则可能会产生失真）*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        bits = (unsigned char*)pinfo_str->pp_line_addr[i];

        /* 颠倒R、B分量。注：文档描述与实测图象格式不符，暂以ACDSee显示效果为准 */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pbuf[j * 3 + 0] = bits[j * 3 + 2];
          pbuf[j * 3 + 1] = bits[j * 3 + 1];
          pbuf[j * 3 + 2] = bits[j * 3 + 0];
        }

        /* 写入目标流 */
        if (isio_write((const void*)pbuf, wlen, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
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
      if (pbuf) {
        isirw_free(pbuf);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&ras_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


