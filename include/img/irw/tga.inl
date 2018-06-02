/********************************************************************
 tga.c
 本文件用途： TGA图像读写模块实现文件
 读取功能：未压缩的8、15、16、24、32位图像，及
 采用RLE压缩的8、15、16、24、32位图像
 保存功能：未压缩的8、15、16、24、32位图像
 本文件编写人： 晓月儿 xiaoyueer##263.net
 YZ
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-9 插件接口升级至2.2。引入内存防护概念。
 2001-12 发布新版代码（加快了图像读取速度，并增加
 了保存图像的功能）
 2001-3 为提高模块读图像速度而修改代码
 2000-8 第一个测试版发布
********************************************************************/


/**************TGA 图象类型 *************/
typedef enum {
  TGA_NULL    = 0,
    TGA_UNCPSPAL  = 1,
    TGA_UNCPSCOLOR  = 2,
    TGA_UNCPSGRAY = 3,
    TGA_RLEPAL    = 9,
    TGA_RLECOLOR  = 10,
    TGA_RLEGRAY   = 11
} TGATYPE;

/**********TGA 图象数据存储类型(与bDescriptor对应位与) ***********/
typedef enum {
  TGA_HORZMIRROR = 0x10,
    TGA_VERTMIRROR = 0x20
} TGAMIRROR;



/**************TGA 文件头描述 ***************/
typedef struct {
  uchar  bID_Length;    /* 附加信息长度 */
  uchar  bPalType;    /* 调色板信息 */
  uchar  bImageType;    /* 图象类型(0,1,2,3,9,10,11) */
  ushort wPalFirstNdx;  /* 调色板第一个索引值 */
  ushort wPalLength;    /* 调色板索引数(以调色板单元为单位) */
  uchar  bPalBits;    /* 一个调色板单位位数(15,16,24,32) */
  ushort wLeft;     /* 图象左端坐标(基本无用) */
  ushort wBottom;     /* 图象底端坐标(基本无用) */
  ushort wWidth;      /* 图象宽度 */
  ushort wDepth;      /* 图象长度 */
  uchar  bBits;     /* 一个象素位数 */
  uchar  bDescriptor;   /* 附加特性描述  */
} TGAHEADER, *LPTGAHEADER;

/**************TGA 文件尾描述 ***************/
typedef struct {
  ulong  eao;       /* 扩展区偏移 */
  ulong  ddo;       /* 开发者区偏移 */
  uchar  info[16];    /* TRUEVISION-XFILE 商标字符串 */
  uchar  period;      /* 字符"." */
  uchar  zero;      /* 0 */
} TGAFOOTER, *LPTGAFOOTER;


//IRWP_INFO tga_irwp_info; /* 插件信息表 */
char* tga_save_idinfo = "ISeeExplorer";
/* 保存图像时注入的标志性信息 */


/* 内部助手函数 */
int _decoding_line(uchar* pdest, uchar* psrc, int rleunit, int destlen);
int _tga_is_valid_img(INFOSTR* pinfo_str);

/* 获取图像信息 */
EXERESULT tga_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  ulong stream_length;
  TGAHEADER tgaHeader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
    do {
      /* 获取流长度 */
      stream_length = isio_length(pfile);
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&tgaHeader, sizeof(TGAHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 判断文件头的图象类型标志 */
      switch (tgaHeader.bImageType) {
      case TGA_RLEPAL:
      case TGA_RLECOLOR:
      case TGA_RLEGRAY:
        /* 有效的图像流长度应大于头结构尺寸（粗略判断） */
        if (stream_length < sizeof(TGAHEADER)) {
          b_status = ER_NONIMAGE;
          break;
        }
        break;
      case TGA_UNCPSPAL:
      case TGA_UNCPSCOLOR:
      case TGA_UNCPSGRAY:
        /* 未压缩图像可计算出图像数据的尺寸，所以判断的能更精确一些 */
        if (stream_length < (sizeof(TGAHEADER) + tgaHeader.bID_Length + ((tgaHeader.bBits + 7) >> 3)*tgaHeader.wDepth * tgaHeader.wWidth + tgaHeader.wPalLength * ((tgaHeader.bPalBits + 7) >> 3))) {
          b_status = ER_NONIMAGE;
          break;
        }
        break;
      case TGA_NULL: /* 未包含图像数据的文件将视为无效图像 */
      default: /* 未知文件类型 */
        b_status = ER_NONIMAGE;
        break;
      }
      /* 检验象素位数是否合法 */
      if ((tgaHeader.bBits != 8) && (tgaHeader.bBits != 15) && \
          (tgaHeader.bBits != 16) && (tgaHeader.bBits != 24) && \
      (tgaHeader.bBits != 32)) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 填写图像信息结构 */
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_TGA; /* 图像文件格式（后缀名） */
      switch (tgaHeader.bImageType) { /* 图像的压缩方式 */
      case TGA_UNCPSPAL:
      case TGA_UNCPSCOLOR:
      case TGA_UNCPSGRAY:
        pinfo_str->compression = ICS_RGB;
        break;
      case TGA_RLEPAL:
      case TGA_RLECOLOR:
      case TGA_RLEGRAY:
        switch (tgaHeader.bBits) {
        case 8:
          pinfo_str->compression = ICS_RLE8;
          break;
        case 15:
        case 16:
          pinfo_str->compression = ICS_RLE16;
          break;
        case 24:
          pinfo_str->compression = ICS_RLE24;
          break;
        case 32:
          pinfo_str->compression = ICS_RLE32;
          break;
        }
        break;
      default:
        assert(0); /* 系统崩溃 */
        b_status = ER_SYSERR;
        break;
      }
      /* 填写图像信息 */
      pinfo_str->width = (ulong)tgaHeader.wWidth;
      pinfo_str->height = (ulong)tgaHeader.wDepth;
      pinfo_str->order = (tgaHeader.bDescriptor & TGA_VERTMIRROR) ? 0 : 1;
      pinfo_str->bitcount = (ulong)(tgaHeader.bBits == 15) ? 16 : tgaHeader.bBits;
      IF_LEAVE(_get_mask(tgaHeader.bBits, (&pinfo_str->b_mask), (&pinfo_str->g_mask), (&pinfo_str->r_mask), (&pinfo_str->a_mask)));
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
  
  return b_status;
}
/* 读取图像位数据 */
EXERESULT tga_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  ulong stream_length;
  TGAHEADER tgaHeader;
  int i, k, j, linesize, decoded;
  int pallen = 0, firstindex = 0, newlinepos = 0, ile = 0;
  ushort palentry;
  uchar* pbuf = 0;
  uchar* pbits;
  uchar bpix;
  ushort wpix;
  ulong lpix;
  static uchar tmp_pal[1024]; /* 临时调色板缓冲区 */
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  
    do {
      /* 获取流长度 */
      stream_length = isio_length(pfile);
      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&tgaHeader, sizeof(TGAHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        /* 判断文件头的图象类型标志 */
        switch (tgaHeader.bImageType) {
        case TGA_RLEPAL:
        case TGA_RLECOLOR:
        case TGA_RLEGRAY:
          /* 有效的图像流长度应大于头结构尺寸（粗略判断） */
          if (stream_length < sizeof(TGAHEADER)) {
            b_status = ER_NONIMAGE;
            break;
          }
          break;
        case TGA_UNCPSPAL:
        case TGA_UNCPSCOLOR:
        case TGA_UNCPSGRAY:
          /* 未压缩图像可计算出图像数据的尺寸，所以判断的能更精确一些 */
          if (stream_length < (sizeof(TGAHEADER) + tgaHeader.bID_Length + ((tgaHeader.bBits + 7) >> 3)*tgaHeader.wDepth * tgaHeader.wWidth + tgaHeader.wPalLength * ((tgaHeader.bPalBits + 7) >> 3))) {
            b_status = ER_NONIMAGE;
            break;
          }
          break;
        case TGA_NULL: /* 未包含图像数据的文件将视为无效图像 */
        default: /* 未知文件类型 */
          b_status = ER_NONIMAGE;
          break;
        }
        /* 检验象素位数是否合法 */
        if ((tgaHeader.bBits != 8) && (tgaHeader.bBits != 15) && \
            (tgaHeader.bBits != 16) && (tgaHeader.bBits != 24) && \
        (tgaHeader.bBits != 32)) {
          b_status = ER_BADIMAGE;
          break;
        }
        /* 填写图像信息结构 */
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
        pinfo_str->imgformat = IMF_TGA; /* 图像文件格式（后缀名） */
        switch (tgaHeader.bImageType) { /* 图像的压缩方式 */
        case TGA_UNCPSPAL:
        case TGA_UNCPSCOLOR:
        case TGA_UNCPSGRAY:
          pinfo_str->compression = ICS_RGB;
          break;
        case TGA_RLEPAL:
        case TGA_RLECOLOR:
        case TGA_RLEGRAY:
          switch (tgaHeader.bBits) {
          case 8:
            pinfo_str->compression = ICS_RLE8;
            break;
          case 15:
          case 16:
            pinfo_str->compression = ICS_RLE16;
            break;
          case 24:
            pinfo_str->compression = ICS_RLE24;
            break;
          case 32:
            pinfo_str->compression = ICS_RLE32;
            break;
          }
          break;
        default:
          assert(0); /* 系统崩溃 */
          b_status = ER_SYSERR;
          break;
        }
        /* 填写图像信息 */
        pinfo_str->width = (ulong)tgaHeader.wWidth;
        pinfo_str->height = (ulong)tgaHeader.wDepth;
        pinfo_str->order = (tgaHeader.bDescriptor & TGA_VERTMIRROR) ? 0 : 1;
        pinfo_str->bitcount = (ulong)(tgaHeader.bBits == 15) ? 16 : tgaHeader.bBits;
        /* 填写掩码信息 */
        IF_LEAVE(_get_mask(tgaHeader.bBits, (&pinfo_str->b_mask), (&pinfo_str->g_mask),
            (&pinfo_str->r_mask), (&pinfo_str->a_mask)));
        pinfo_str->data_state = 1;
      }
      /* 设置调色板数据 */
      if (pinfo_str->bitcount == 8) {
        pinfo_str->pal_count = 256;
        /* 生成灰度图调色板 */
        if ((tgaHeader.bImageType == TGA_UNCPSGRAY) || (tgaHeader.bImageType == TGA_RLEGRAY)) {
          for (i = 0; i < (int)pinfo_str->pal_count; i++) {
            pinfo_str->palette[i] = (i << 16) | (i << 8) | (i);
          }
        }
        else { /* 拷贝并转换调色板数据从文件 */
          if (isio_seek(pfile, sizeof(TGAHEADER) + tgaHeader.bID_Length, SEEK_SET) == -1) {
            b_status = ER_FILERWERR;
            break;
          }
          /* TGA调色板项并不是普通的ARGB格式，所以需要下面的判断过程 */
          pallen = tgaHeader.wPalLength * ((tgaHeader.bPalBits + 7) >> 3);
          firstindex = tgaHeader.wPalFirstNdx;
          if (isio_read((void*)tmp_pal, pallen, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* 转换调色板数据 */
          switch (tgaHeader.bPalBits) {
          case 15:
          case 16: /* 转换2字节调色板数据 */
            for (i = 0; i < pallen; i += 2) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                break;
              }
              palentry = *((ushort*)(tmp_pal + i));
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->blue = (palentry & 0x1f) << 3;
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->green = ((palentry >> 5) & 0x1f) << 3;
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->red = ((palentry >> 10) & 0x1f) << 3;
              pinfo_str->palette[firstindex++] &= 0xffffff; /* 最高字节置零 */
            }
            break;
          case 24: /* 转换3字节调色板数据 */
            for (i = 0; i < pallen; i += 3) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                break;
              }
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->blue = tmp_pal[i];
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->green = tmp_pal[i + 1];
              ((PALITEM*)(&pinfo_str->palette[firstindex]))->red = tmp_pal[i + 2];
              pinfo_str->palette[firstindex++] &= 0xffffff;
            }
            break;
          case 32: /* 转换4字节调色板数据 */
            for (i = 0; i < pallen; i += 4) {
              if (firstindex >= 256) {
                b_status = ER_BADIMAGE;
                break;
              }
              pinfo_str->palette[firstindex] = *(ulong*)(tmp_pal + i);
              pinfo_str->palette[firstindex++] &= 0xffffff;
            }
            break;
          default:
            assert(0); /* 系统崩溃 */
            b_status = ER_SYSERR;
            break;
          }
        }
      }
      else {
        pinfo_str->pal_count = 0; /* 无调色板数据 */
      }
      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* 设置图像个数 */
      pinfo_str->imgnumbers = 1; /* TGA文件中只有一个图像，*/
      /* 应当还有原始图象缩图(Postage Stamp)，*/
      /* 但晓月儿还未遇见实例 */
      pinfo_str->psubimg = (SUBIMGBLOCK*)0; /* 子图像链置空 */
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
      /* 分配图像扫描行缓冲区 */
      pbuf = (uchar*)isirw_malloc(pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3) + pinfo_str->width); /* 最坏的情况是RLE压缩中没有一个重复字组 */
      if (!pbuf) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 初始化行首地址数组 */
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
      /* 读取图像数据 */
      /* 根据图像文件的位深度的不同而采用不同的读取代码 */
      switch (tgaHeader.bImageType) {
      case TGA_UNCPSPAL:
      case TGA_UNCPSGRAY:
      case TGA_UNCPSCOLOR:
        /* 下一行图像数据在文件中的起始位置 */
        newlinepos = sizeof(TGAHEADER) + tgaHeader.bID_Length + pallen;
        if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 文件扫描行长度 */
        ile = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3);
        pbits = pinfo_str->p_bit_data;
        /* 以行为单位读取图像数据 */
        if (tgaHeader.bDescriptor & TGA_HORZMIRROR) { /* 左右反置 */
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_read((void*)pbuf, ile, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            for (k = 0; k < (int)pinfo_str->width; k++) {
              switch (pinfo_str->bitcount) {
              case 8:
                ((uchar*)pbits)[k] = pbuf[pinfo_str->width - 1 - k];
                break;
              case 15:
              case 16:
                ((ushort*)pbits)[k] = ((ushort*)pbuf)[pinfo_str->width - 1 - k];
                break;
              case 24:
                for (j = 0; j < 3; j++) {
                  ((uchar*)pbits)[k * 3 + j] = pbuf[(pinfo_str->width - 1 - k) * 3 + j];
                }
                break;
              case 32:
                ((ulong*)pbits)[k] = ((ulong*)pbuf)[pinfo_str->width - 1 - k];
                break;
              default:
                assert(0); /* 系统崩溃 */
                b_status = ER_SYSERR;
                break;
                break;
              }
            }
            pbits = (uchar*)(pbits + linesize);
          }
        }
        else {
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_read((void*)pbits, ile, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            pbits = (uchar*)(pbits + linesize);
          }
        }
        break;
      case TGA_RLEGRAY:
      case TGA_RLEPAL:
      case TGA_RLECOLOR:
        /* 下一行图像数据在文件中的起始位置 */
        newlinepos = sizeof(TGAHEADER) + tgaHeader.bID_Length + pallen;
        /* 文件扫描行长度（以最坏的情况计算） */
        ile = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3) + pinfo_str->width;
        pbits = pinfo_str->p_bit_data;
        /* 以行为单位读取图像数据 */
        if (tgaHeader.bDescriptor & TGA_HORZMIRROR) { /* 左右反置 */
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
              b_status = ER_FILERWERR;
              break;
            }
            if (isio_read((void*)pbuf, 1, ile, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            /* 解压一RLE图像行 */
            decoded = _decoding_line(pbits, pbuf, (pinfo_str->bitcount + 7) >> 3, pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3));
            if (decoded == -1) {
              b_status = ER_BADIMAGE;
              break;
            }
            assert(decoded);
            /* 下一行图像数据在文件中的起始位置 */
            newlinepos += decoded;
            /* 左右颠倒图像数据 */
            for (k = 0; k < (int)pinfo_str->width; k++) {
              switch (pinfo_str->bitcount) {
              case 8:
                bpix = ((uchar*)pbits)[k];
                ((uchar*)pbits)[k] = ((uchar*)pbits)[pinfo_str->width - 1 - k];
                ((uchar*)pbits)[pinfo_str->width - 1 - k] = bpix;
                break;
              case 15:
              case 16:
                wpix = ((ushort*)pbits)[k];
                ((ushort*)pbits)[k] = ((ushort*)pbits)[pinfo_str->width - 1 - k];
                ((ushort*)pbits)[pinfo_str->width - 1 - k] = wpix;
                break;
              case 24:
                for (j = 0; j < 3; j++) {
                  bpix = ((uchar*)pbits)[k * 3 + j];
                  ((uchar*)pbits)[k * 3 + j] = ((uchar*)pbits)[(pinfo_str->width - 1 - k) * 3 + j];
                  ((uchar*)pbits)[(pinfo_str->width - 1 - k) * 3 + j] = bpix;
                }
                break;
              case 32:
                lpix = ((ulong*)pbits)[k];
                ((ulong*)pbits)[k] = ((ulong*)pbits)[pinfo_str->width - 1 - k];
                ((ulong*)pbits)[pinfo_str->width - 1 - k] = lpix;
                break;
              default:
                assert(0); /* 系统崩溃 */
                b_status = ER_SYSERR;
                break;
                break;
              }
            }
            pbits = (uchar*)(pbits + linesize);
          }
        }
        else {
          for (i = 0; i < (int)pinfo_str->height; i++) {
            if (isio_seek(pfile, newlinepos, SEEK_SET) == -1) {
              b_status = ER_FILERWERR;
              break;
            }
            if (isio_read((void*)pbuf, 1, ile, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
            }
            /* 解压一RLE图像行 */
            decoded = _decoding_line(pbits, pbuf, (pinfo_str->bitcount + 7) >> 3, pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3));
            if (decoded == -1) {
              b_status = ER_BADIMAGE;
              break;
            }
            assert(decoded);
            /* 下一行图像数据在文件中的起始位置 */
            newlinepos += decoded;
            pbits = (uchar*)(pbits + linesize);
          }
        }
        break;
      default:
        assert(0); /* 系统崩溃 */
        b_status = ER_SYSERR;
        break;
      }
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pbuf) {
        isirw_free(pbuf);
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
EXERESULT tga_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  TGAHEADER tgaHeader;
  TGAFOOTER tgaFooter;
  int i, wrlinesize;
  uchar* pbits;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  
    do {
      /* 判断是否是受支持的图像数据 */
      if (_tga_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* 附加信息长度，此处将增加一个"ISeeExplorer"字符串 */
      tgaHeader.bID_Length = (uchar)strlen((const char*)tga_save_idinfo);
      /* 调色板信息（灰度图在这里也将被看作调色板图） */
      tgaHeader.bPalType = (uchar)(pinfo_str->bitcount == 8) ? 1 : 0;
      /* 图象类型(只有1，2两种类型) */
      tgaHeader.bImageType = (uchar)(pinfo_str->bitcount == 8) ? TGA_UNCPSPAL : TGA_UNCPSCOLOR;
      /* 调色板第一个索引值 */
      tgaHeader.wPalFirstNdx = 0;
      /* 调色板索引数(以调色板单元为单位) */
      tgaHeader.wPalLength = (ushort)(pinfo_str->bitcount == 8) ? 256 : 0;
      /* 一个调色板单位位数(ISee调色板项是32位的) */
      tgaHeader.bPalBits = 32;
      /* 图象左下端坐标 */
      tgaHeader.wLeft = 0;
      tgaHeader.wBottom = 0;
      /* 图象宽、高度 */
      tgaHeader.wWidth = (ushort)pinfo_str->width;
      tgaHeader.wDepth = (ushort)pinfo_str->height;
      /* 象素位数 */
      tgaHeader.bBits = (uchar)pinfo_str->bitcount;
      /* 附加特性描??不包含A通道，一律为倒向图) */
      tgaHeader.bDescriptor = 0;
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 写入文件头结构 */
      if (isio_write((const void*)&tgaHeader, sizeof(TGAHEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 写入信息 */
      if (isio_write((const void*)tga_save_idinfo, strlen(tga_save_idinfo), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 写入调色板数据 */
      if (tgaHeader.bPalType == 1) {
        if (isio_write((const void*)pinfo_str->palette, pinfo_str->pal_count * 4, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
      /* TGA未压缩扫描行尺寸并不是4字节对齐的，所以需要按实际象素计算行尺寸 */
      wrlinesize = pinfo_str->width * ((pinfo_str->bitcount + 7) >> 3);
      /* 写入图像位数据
      注：传入的图像数据必须是ISee位流格式，否则可能会产生失真
      */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        /* 一律保存为倒向图 */
        pbits = (uchar*)(pinfo_str->pp_line_addr[pinfo_str->height - 1 - i]);
        if (isio_write((const void*)pbits, wrlinesize, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
      /* 填写文件尾部信息（新版TGA格式）*/
      tgaFooter.eao = 0;
      tgaFooter.ddo = 0;
      memcpy((void*)tgaFooter.info, (const void*)"TRUEVISION-XFILE", 16);
      tgaFooter.period = '.';
      tgaFooter.zero = 0;
      if (isio_write((const void*)&tgaFooter, sizeof(TGAFOOTER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
    }
    while (0);
    
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 行解压，返回psrc实际解压字节数，如源数据有误，返回-1 */
int _decoding_line(
    uchar* pdest, /* 目标缓冲区 */
    uchar* psrc, /* RLE源数据 */
    int rleunit, /* rleunit为解压单位(in Bytes) */
    int destlen /* destlen为解压目标长度 */
)
{
  int i, j, nCount;
  uchar* pdestEnd = pdest + destlen, *psrcOrg = psrc, *pdestOrg = pdest;
  uchar* ptmp;
  assert(psrc);
  assert(pdest);
  while (pdest < pdestEnd) {
    if (psrc[0] & 0x80) { /* 重复字组 */
      nCount = (psrc[0] & 0x7F) + 1;
      psrc++;
      for (i = 0; i < nCount; i++) {
        ptmp = psrc;
        for (j = 0; j < rleunit; j++) { /* 解压一个象素(length equ rleunit) */
          *pdest++ = *ptmp++;
        }
      }
      psrc += rleunit;
    }
    else { /* 非重复字组 */
      nCount = (psrc[0] & 0x7F) + 1;
      psrc++;
      memcpy(pdest, psrc, nCount * rleunit); /* 非重复字组的尺寸可能会比较大，所以memcpy的效率可能更高 */
      pdest += nCount * rleunit;
      psrc += nCount * rleunit;
    }
  }
  if (pdest != pdestEnd) { /* 图像数据非法，返回 -1 */
    return -1;
  }
  return psrc - psrcOrg; /* 解压成功，返回被解压字节数 */
}
/* 判断传入的图像是否可以被保存 */
int _tga_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = (1UL << (8 - 1)) | (1UL << (16 - 1)) | (1UL << (24 - 1)) | (1UL << (32 - 1))};
  /* ## 位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1; /* 不支持的位深图像 */
  }
  assert(pinfo_str->imgnumbers);
  if (pinfo_str->imgnumbers != 1) {
    return -2; /* 图像个数不正确 */
  }
  return 0;
}

