/********************************************************************
 rle.c
 本文件用途： Utah RLE图像读写模块实现文件
 读取功能：可读取8位、24位、32位RLE图象（最多
 允许三个色彩通道，一个ALPHA通道）
 保存功能：可将8位、24位图象保存为RLE图象（不
 压缩形式）
 本文件编写人：
 YZ yzfree##yeah.net
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-9 插件接口升级至2.2。引入内存防护概念。
 2002-5 第一个发布版（新版）
********************************************************************/

/* 文件判别标志 */
#define RLE_MAGIC           ((ushort)0xcc52)

/* 长操作数判别位 */
#define     LONGOPCODE      0x40

/* RLE流操作码定义 */
#define     RLE_SKIPLINE_OP   1
#define     RLE_SETCOLOR_OP   2
#define     RLE_SKIPPIXELS_OP 3
#define     RLE_BYTEDATA_OP   5
#define     RLE_RUNDATA_OP    6
#define     RLE_EOF_OP      7

#define     RLE_CLEARFIRST_F  0x1   /* 如果设置的话，解压前将清除缓冲区（使用背景色） */
#define     RLE_NO_BACKGROUND_F 0x2   /* 如果设置的话，表明流未提供背景色信息 */
#define     RLE_ALPHA_F     0x4   /* 如果设置的话，表明流中存在ALPHA通道（通道号-1） */
#define     RLE_COMMENT_F   0x8   /* 如果设置的话，表明流中存在注释信息 */

#define   RLE_OPCODE(s)   (s[0] & ~(uchar)LONGOPCODE)
#define   RLE_LONGP(s)    (s[0] & (uchar)LONGOPCODE)
#define   RLE_DATUM(s)    (s[1] & (uchar)0xff)


/* RLE头结构定义 */
typedef struct {
  ushort  magic;        /* RLE判别标志（0xcc52） */
  ushort  xpos;       /* 图象的x、y原点坐标（左下角，第一象限）*/
  ushort  ypos;
  ushort  width;        /* 图象的宽度和高度（象素单位）*/
  ushort  height;
  uchar flags;        /* 标志位 */
  uchar channels;     /* 颜色通道数。不包括ALPHA通道 */
  uchar chan_bits;      /* 每个颜色通道的位宽度（当前有效值只有8）*/
  uchar pal_chan_count;   /* 调色板项的通道数 */
  uchar pal_chan_bits;    /* 调色板通道的位宽度 */
} RLE_HEADER, *LPRLE_HEADER;

/* 内部助手函数 */
int _rle_is_valid_img(INFOSTR* pinfo_str);
static int _rle_pack(uchar* psou, int len, uchar* pdec);

/* 获取图像信息 */
EXERESULT rle_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  RLE_HEADER rle_header;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */

    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&rle_header, sizeof(RLE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 判断指定的图象流是否是RLE格式（只支持8、24、32位1、3、4通道图象） */
      if ((rle_header.magic != RLE_MAGIC) || \
          ((rle_header.channels != 1) && (rle_header.channels != 3)) || \
      (rle_header.chan_bits != 8)) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_RLE; /* 图像文件格式（后缀名） */
      /* 填写图像信息 */
      pinfo_str->width = rle_header.width;
      pinfo_str->height = rle_header.height;
      pinfo_str->order = 1; /* RLE图象都是倒向图（第一象限）*/
      if (rle_header.flags & RLE_ALPHA_F) {
        /* 此处未使用rle_header中的chan_bits，因为这个值总是8 */
        if (rle_header.channels == 1) {
          b_status = ER_NONIMAGE;
          break; /* 不支持带ALPHA通道的8位索引图 */
        }
        else {
          assert(rle_header.channels == 3);
          pinfo_str->bitcount = (rle_header.channels + 1) * 8;
        }
        assert(pinfo_str->bitcount == 32);
      }
      else {
        pinfo_str->bitcount = rle_header.channels * 8;
      }
      pinfo_str->compression = ICS_RLE8;
      /* 计算图像掩码数据 */
      switch (pinfo_str->bitcount) {
      case 8:
        pinfo_str->r_mask = 0;
        pinfo_str->g_mask = 0;
        pinfo_str->b_mask = 0;
        pinfo_str->a_mask = 0;
        break;
      case 24:
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->b_mask = 0xff;
        pinfo_str->a_mask = 0;
        break;
      case 32:
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->b_mask = 0xff;
        pinfo_str->a_mask = 0xff000000;
        break;
      }
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT rle_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i, j, k, linesize, end_mark;
  RLE_HEADER rle_header;
  char bg[4], filler, bgmark;
  ushort* pal_buf = 0, comm_len;
  uchar* pcomm = 0, *p, oper[2], *pbuf = 0;
  unsigned int xinc;
  ushort skiplines, skipcol, nc, cor;
  uchar currchann;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  do {
      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&rle_header, sizeof(RLE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 如果该图像还未调?霉崛⌒畔⒑蛱钚赐枷裥畔?*/
      if (pinfo_str->data_state == 0) {
        if ((rle_header.magic != RLE_MAGIC) || \
            ((rle_header.channels != 1) && (rle_header.channels != 3)) || \
        (rle_header.chan_bits != 8)) {
          b_status = ER_NONIMAGE;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
        pinfo_str->imgformat = IMF_RLE; /* 图像文件格式（后缀名） */
        /* 填写图像信息 */
        pinfo_str->width = rle_header.width;
        pinfo_str->height = rle_header.height;
        pinfo_str->order = 1;
        if (rle_header.flags & RLE_ALPHA_F) {
          if (rle_header.channels == 1) {
            b_status = ER_NONIMAGE;
            break;
          }
          else {
            assert(rle_header.channels == 3);
            pinfo_str->bitcount = (rle_header.channels + 1) * 8;
          }
          assert(pinfo_str->bitcount == 32);
        }
        else {
          pinfo_str->bitcount = rle_header.channels * 8;
        }
        pinfo_str->compression = ICS_RLE8;
        /* 计算图像掩码数据 */
        switch (pinfo_str->bitcount) {
        case 8:
          pinfo_str->r_mask = 0;
          pinfo_str->g_mask = 0;
          pinfo_str->b_mask = 0;
          pinfo_str->a_mask = 0;
          break;
        case 24:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0;
          break;
        case 32:
          pinfo_str->r_mask = 0xff0000;
          pinfo_str->g_mask = 0xff00;
          pinfo_str->b_mask = 0xff;
          pinfo_str->a_mask = 0xff000000;
          break;
        }
        pinfo_str->data_state = 1;
      }
      /* 提取背景色信息 */
      if (rle_header.flags & RLE_NO_BACKGROUND_F) {
        /* 跨过填充字节 */
        if (isio_read((void*)&filler, 1, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        bgmark = 0;
      }
      else {
        assert((rle_header.channels == 3) || (rle_header.channels == 1));
        memset((void*)bg, 0, 4);
        /* 读取背景色 */
        if (isio_read((void*)bg, rle_header.channels, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 确定是否需要事先填充背景色 */
        if (rle_header.flags & RLE_CLEARFIRST_F) {
          bgmark = 1;
        }
        else {
          bgmark = 0;
        }
      }
      /* 设置调色板数据 */
      if (pinfo_str->bitcount <= 8) {
        assert(rle_header.channels == 1);
        assert(pinfo_str->bitcount == 8); /* 当前只支持8位索引图 */
        if ((rle_header.pal_chan_count != 3) || (rle_header.pal_chan_bits != 8)) {
          b_status = ER_BADIMAGE;
          break;
        }
        /* 申请调色板数?莼撼迩?*/
        if ((pal_buf = (ushort*)isirw_malloc(256 * 3 * 2)) == 0) { /* 256项、3通道、2字节 */
          b_status = ER_MEMORYERR;
          break;
        }
        /* 读取调色板数据 */
        if (isio_read((void*)pal_buf, (256 * 3 * 2), 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 转换至数据包 */
        for (i = 0; i < 256; i++) {
          pinfo_str->palette[i] = (((ulong)(pal_buf[i] >> 8)) << 16) | \
              (((ulong)(pal_buf[256 + i] >> 8)) << 8) | \
              ((ulong)(pal_buf[512 + i] >> 8));
        }
        pinfo_str->pal_count = 256;
      }
      else {
        if (rle_header.pal_chan_count != 0) {
          b_status = ER_BADIMAGE;
          break; /* 非索引图不应存在调色板数据 */
        }
        pinfo_str->pal_count = 0;
      }
      /* 读取注释信息 */
      if (rle_header.flags & RLE_COMMENT_F) {
        /* 读取注释信息长度值（2字节）*/
        if (isio_read((void*)&comm_len, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 偶数对齐 */
        comm_len = (comm_len + 1) & (ushort)0xfffe;
        if (comm_len) {
          /* 申请注?托畔⒒撼迩?*/
          if ((pcomm = (uchar*)isirw_malloc(comm_len)) == 0) {
            b_status = ER_MEMORYERR;
            break;
          }
          /* 读注释信息 */
          if (isio_read((void*)pcomm, comm_len, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
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
      /* 填写行首地址数组（倒向图）*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }
      /* 回调通知 */
       
      /* 填充背景色 */
      if (bgmark) {
        k = (int)pinfo_str->bitcount / 8;
        for (i = 0; i < (int)pinfo_str->height; i++) {
          for (j = 0; j < (int)pinfo_str->width; j++) {
            memcpy((void*)((uchar*)pinfo_str->pp_line_addr[i] + j * k), (const void*)bg, k);
          }
        }
      }
      /* 申请扫描行缓冲区 */
      if ((pbuf = (uchar*)isirw_malloc(pinfo_str->width * 3 + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      currchann = 0;
      nc = 0;
      k = pinfo_str->bitcount / 8;
      end_mark = 0;
      /* 读入并解压象素数据至数据包 */
      for (i = 0; i < (int)pinfo_str->height; i += skiplines) {
        if (end_mark) {
          b_status = ER_BADIMAGE;
          break; /* 源图象缺少数据 */
        }
        skiplines = 0;
        xinc = 0;
        p = pinfo_str->p_bit_data + i * linesize;
        while (!skiplines) {
          /* 检查源数据是否合法 */
          if (xinc > pinfo_str->width) {
            b_status = ER_BADIMAGE;
            break;
          }
          /* 读取操作命令（包括操作码和单字节操作数）*/
          if (isio_read((void*)oper, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* 分解并执行命令 */
          switch (RLE_OPCODE(oper)) {
          case RLE_SKIPLINE_OP: /* 1 － 跳过指定行 */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&skiplines, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
                break;
              }
            }
            else {
              skiplines = (ushort)RLE_DATUM(oper);
            }
            break;
          case RLE_SETCOLOR_OP: /* 2 － 设置当前?ǖ?*/
            currchann = RLE_DATUM(oper);
            switch (k) {
            case 1:
              if (currchann != 0) { /* 只能有0号通道数据 */
                b_status = ER_BADIMAGE;
                break;
                break;
              }
              break;
            case 3:
              if (currchann == 0) { /* 将当前通道值转为象素写入位置 */
                currchann = 2;
              }
              else if (currchann == 1) {
                currchann = 1;
              }
              else if (currchann == 2) {
                currchann = 0;
              }
              else { /* 不允许有其他通道号 */
                b_status = ER_BADIMAGE;
                break;
                break;
              }
              break;
            case 4:
              if (currchann == 0) {
                currchann = 2;
              }
              else if (currchann == 1) {
                currchann = 1;
              }
              else if (currchann == 2) {
                currchann = 0;
              }
              else if (currchann == (uchar)255) { /* ALPHA通道转为象素写入位置 */
                currchann = 3;
              }
              else {
                b_status = ER_BADIMAGE;
                break;
                break;
              }
              break;
            default:
              assert(0);
              b_status = ER_SYSERR;
              break;
              break;
            }
            assert(currchann <= 3);
            xinc = 0; /* 象素索引归0 */
            break;
          case RLE_SKIPPIXELS_OP: /* 3 － 跳过指定个数的象素 */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&skipcol, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
                break;
              }
            }
            else {
              skipcol = (ushort)RLE_DATUM(oper);
            }
            /* 防止发生写溢出 */
            if ((xinc + (unsigned int)skipcol) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              break;
              break;
            }
            xinc += (unsigned int)skipcol;
            break;
          case RLE_BYTEDATA_OP: /* 5 － 未压缩数据 */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&nc, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
                break;
              }
            }
            else {
              nc = RLE_DATUM(oper);
            }
            nc++;
            /* 读入未压缩数据（如果可能，也包括填充字节） */
            if (isio_read((void*)pbuf, ((nc + 1) / 2) * 2, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
              break;
            }
            /* 防止发生写溢出 */
            if ((xinc + (unsigned int)nc) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              break;
              break;
            }
            /* 写象素至数据包 */
            for (j = 0; j < (int)(unsigned int)nc; j++) {
              p[k * (xinc + j) + (int)(unsigned int)currchann] = pbuf[j];
            }
            xinc += (unsigned int)nc;
            break;
          case RLE_RUNDATA_OP: /* 6 － 压缩数据 */
            if (RLE_LONGP(oper)) {
              if (isio_read((void*)&nc, 2, 1, pfile) == 0) {
                b_status = ER_FILERWERR;
                break;
                break;
              }
            }
            else {
              nc = RLE_DATUM(oper);
            }
            nc++;
            /* 读压缩字节（和一字节的填充码）*/
            if (isio_read((void*)&cor, 2, 1, pfile) == 0) {
              b_status = ER_FILERWERR;
              break;
              break;
            }
            /* 防止发生写溢出 */
            if ((xinc + (unsigned int)nc) > pinfo_str->width) {
              b_status = ER_BADIMAGE;
              break;
              break;
            }
            /* 写象素至数据包 */
            for (j = 0; j < (int)(unsigned int)nc; j++) {
              p[k * (xinc + j) + (int)(unsigned int)currchann] = (uchar)cor;
            }
            xinc += (unsigned int)nc;
            break;
          case RLE_EOF_OP: /* 7 － 流终结符 */
            skiplines = 1; /* 虚值，使while循环终止 */
            end_mark = 1;
            break;
          default:
            b_status = ER_BADIMAGE;
            break; /* 未定义的操作码 */
          }
        }
        /* 进度调用 */
         
      }
      /* 主图象进度结束 */
       
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pbuf) {
        isirw_free(pbuf);
      }
      if (pcomm) {
        isirw_free(pcomm);
      }
      if (pal_buf) {
        isirw_free(pal_buf);
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
EXERESULT rle_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  RLE_HEADER rle_header;
  uchar filler = 0;
  ushort commlen, skipline, chann;
  int i, j, l, k, packlen;
  uchar* psou = 0, *pdec = 0, *p;
  static char* ptag = "manufacturer=ISee"; /* 注释串，17字节 */
  static ushort pal[3][256]; /* 调色板数据 */
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  do {
      /* 判断是否是受支持的图像数据 */
      if (_rle_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      /* 填写头结构 */
      rle_header.magic = (ushort)RLE_MAGIC;
      rle_header.xpos = 0;
      rle_header.ypos = 0;
      rle_header.width = (ushort)pinfo_str->width;
      rle_header.height = (ushort)pinfo_str->height;
      rle_header.channels = (uchar)pinfo_str->bitcount / 8;
      assert((rle_header.channels == 1) || (rle_header.channels == 3)); /* 只允许8位和24位图象 */
      rle_header.chan_bits = 8;
      rle_header.flags = RLE_NO_BACKGROUND_F | RLE_COMMENT_F;
      if (pinfo_str->bitcount == 8) {
        rle_header.pal_chan_count = 3;
        rle_header.pal_chan_bits = 8;
      }
      else {
        rle_header.pal_chan_count = 0;
        rle_header.pal_chan_bits = 8;
      }
      /* 写入头结构 */
      if (isio_write((const void*)&rle_header, sizeof(RLE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
       
      /* 写入填充字节 */
      if (isio_write((const void*)&filler, 1, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 写入调色板数据 */
      if (rle_header.pal_chan_count) {
        assert(pinfo_str->bitcount == 8);
        /* 分解原调色板数据 */
        for (i = 0; i < 3; i++) {
          for (j = 0; j < 256; j++) {
            pal[i][j] = (ushort)(((pinfo_str->palette[j] >> ((2 - i) * 8)) & 0xff) << 8);
          }
        }
        /* 写入通道分离后的调色板数据 */
        for (i = 0; i < 3; i++) {
          if (isio_write((const void*)pal[i], 256 * 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
      }
      commlen = strlen(ptag) + 1; /* 计算注释串长度（包括结尾0符）*/
      /* 写入注释信息 */
      if (isio_write((const void*)&commlen, 2, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_write((const void*)ptag, commlen, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 确保偶字节对齐 */
      if (commlen & 0x1) {
        /* 写入填充字节 */
        if (isio_write((const void*)&filler, 1, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
      }
      /* 分配压缩缓冲区 */
      if ((psou = (uchar*)isirw_malloc(pinfo_str->width + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      if ((pdec = (uchar*)isirw_malloc(pinfo_str->width * 3 + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      k = pinfo_str->bitcount / 8;
      /* 逐行写入数据 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        /* 以倒向图方式保存图象 */
        p = (uchar*)pinfo_str->pp_line_addr[pinfo_str->height - i - 1];
        for (j = 0; j < k; j++) {
          chann = ((j & 0xff) << 8) | (RLE_SETCOLOR_OP & 0xff);
          /* 写入通道值 */
          if (isio_write((const void*)&chann, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
          /* 提取通道数据 */
          for (l = 0; l < (int)pinfo_str->width; l++) {
            psou[l] = p[k * l + (k - 1 - j)];
          }
          packlen = _rle_pack(psou, (int)pinfo_str->width, pdec);
          /* 写入通道数据 */
          if (isio_write((const void*)pdec, packlen, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        if ((i + 1) == (int)pinfo_str->height) { /* 最后一行已经?慈?*/
          skipline = RLE_EOF_OP;
          /* 写入EOF标志 */
          if (isio_write((const void*)&skipline, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
        else {
          skipline = (0x1 << 8) | (RLE_SKIPLINE_OP & 0xff);
          /* 写入通道值 */
          if (isio_write((const void*)&skipline, 2, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            break;
          }
        }
         
      }
       
    }
    while (0);
    
      if (psou) {
        isirw_free(psou);
      }
      if (pdec) {
        isirw_free(pdec);
      }
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 判断传入的图像是否可以被保存（如不支持保存功能，可将本函数去掉） */
int _rle_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = (1UL << (8 - 1)) | (1UL << (24 - 1))};
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
#define RLE_UNKNOW 0
#define RLE_REPEAT 1
#define RLE_NONREPEAT 2
/* 压缩一行数据 */
/* 该函数实际上并未压缩数据，因为这种压缩法很原始，做不好的话就会产生反效果 */
static int _rle_pack(uchar* psou, int len, uchar* pdec)
{
  assert(psou && len && pdec);
  *pdec++ = RLE_BYTEDATA_OP | LONGOPCODE;
  *pdec++ = 0;
  *(ushort*)pdec = (ushort)(len - 1);
  pdec += 2;
  memcpy((void*)pdec, (const void*)psou, len);
  return (len + 2 + 2 + 1) & 0xfffffffe; /* 压缩后的数据长度，偶字节对齐 */
}

