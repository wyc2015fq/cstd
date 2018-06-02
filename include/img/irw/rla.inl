/********************************************************************
 rla.c
 本文件用途： Alias/Wavefront RLA图像读写模块实现文件
 读取功能：可读取3通道24位压缩rgb色系的图象，不支持matte、
 aux通道
 保存功能：可将24位图象保存为3通道RLE8压缩的rgb色系图象
 本文件编写人：
 YZ yzfree##yeah.net
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-6 发现有头结构中未填写版本信息的图像文件
 暂时去掉对版本信息的判断，如果今后发现问题再行处理()
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-9 插件接口升级至2.2。引入内存防护概念。
 2002-5 第一个发布版（新版）
********************************************************************/

/* 文件判别标志 */
#define RLA_REVISION          0xfffe

/* 窗口坐标结构 */
typedef struct {
  short left;
  short right;
  short bottom;
  short top;
} RLA_WINDOW_S, *LPRLA_WINDOW_S;


/* RLA流头结构定义 */
typedef struct {
  RLA_WINDOW_S window;      /* 背景窗口坐标 */
  RLA_WINDOW_S active_window;   /* 图象窗口坐标 */
  short frame;          /* 图象帧数 */
  short storage_type;       /* 图象数据压缩类型 */
  short num_chan;         /* 图象通道数（不包含matte、aux通道），一般为3 */
  short num_matte;        /* matte通道数，一般为1 */
  short num_aux;          /* 辅助通道数 */
  short revision;         /* 格式修订号，当前为0xfffe */
  char  gamma[16];        /* gamma值，一般为2.2 */
  char  red_pri[24];
  char  green_pri[24];
  char  blue_pri[24];
  char  white_pt[24];
  long  job_num;
  char  name[128];
  char  desc[128];        /* 图象内容描述 */
  char  program[64];        /* 创建图象的软件名称 */
  char  machine[32];        /* 创建图象的机器名称 */
  char  user[32];
  char  date[20];         /* 创建图象的日期、时间 */
  char  aspect[24];
  char  aspect_ratio[8];
  char  chan[32];         /* 所用色系，一般为"rgb" */
  short field;
  char  time[12];
  char  filter[32];
  short chan_bits;        /* 每通道的位深度 */
  short matte_type;
  short matte_bits;       /* matte通道位深度 */
  short aux_type;
  short aux_bits;         /* 辅助通道位深度 */
  char  aux[32];
  char  space[36];
  long  next;           /* 下一副子图象的数据偏移 */
} RLA_HEADER, *LPRLA_HEADER;

/* 内部助手函数 */
int _rla_is_valid_img(INFOSTR* pinfo_str);
static void _swap_byte(LPRLA_HEADER prla);
static int _decode_scanline(uchar*, int, uchar*, int);
static int _encode_scanline(uchar*, int, uchar*);

/* 获取图像信息 */
EXERESULT rla_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  RLA_HEADER rlaheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */

    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&rlaheader, sizeof(RLA_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 转换字序 */
      _swap_byte(&rlaheader);
      /* 判断是否是合法的RLA图象流 */
      if (/*(rlaheader.revision != (short)RLA_REVISION) || */\
          ((rlaheader.matte_type != 0) && (rlaheader.matte_type != 4)) || \
          ((rlaheader.storage_type != 0) && (rlaheader.storage_type != 4)) || \
          (rlaheader.window.left > rlaheader.window.right) || \
          (rlaheader.window.bottom > rlaheader.window.top) || \
          (rlaheader.active_window.left > rlaheader.active_window.right) || \
      (rlaheader.active_window.bottom > rlaheader.active_window.top)) {
        b_status = ER_NONIMAGE;
        break;
      }
      /* 只支持RGB色系的RLA图象 */
      if (strcmp((const char*)rlaheader.chan, "rgb")) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 只支持24位RLA图象，并且废弃matte通道数据（数据包暂时无法容纳matte数据）*/
      if ((rlaheader.num_chan != 3) || (rlaheader.chan_bits != 8)) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 检查图象宽、高参数是否合法 */
      if (((rlaheader.active_window.right - rlaheader.active_window.left) < 0) || \
      ((rlaheader.active_window.top - rlaheader.active_window.bottom) < 0)) {
        b_status = ER_BADIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_RLA; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_RLE8;
      pinfo_str->width = (ulong)(rlaheader.active_window.right - rlaheader.active_window.left + 1);
      pinfo_str->height = (ulong)(rlaheader.active_window.top - rlaheader.active_window.bottom + 1);
      pinfo_str->order = 1;
      pinfo_str->bitcount = 24; /* 3通道、8位 */
      pinfo_str->b_mask = 0xff;
      pinfo_str->g_mask = 0xff00;
      pinfo_str->r_mask = 0xff0000;
      pinfo_str->a_mask = 0x0;
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT rla_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i, linesize, j;
  unsigned int* poffset = 0;
  uchar* punpack = 0, *pspl = 0, *p;
  uchar* pr, *pg, *pb;
  ushort rl, gl, bl;
  RLA_HEADER rlaheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中?荒艽嬖谕枷裎皇?*/
  do {
      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read((void*)&rlaheader, sizeof(RLA_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 转换字序 */
      _swap_byte(&rlaheader);
      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        if (/* (rlaheader.revision != (short)RLA_REVISION) || */\
            ((rlaheader.matte_type != 0) && (rlaheader.matte_type != 4)) || \
            ((rlaheader.storage_type != 0) && (rlaheader.storage_type != 4)) || \
            (rlaheader.window.left > rlaheader.window.right) || \
            (rlaheader.window.bottom > rlaheader.window.top) || \
            (rlaheader.active_window.left > rlaheader.active_window.right) || \
        (rlaheader.active_window.bottom > rlaheader.active_window.top)) {
          b_status = ER_NONIMAGE;
          break;
        }
        if (strcmp((const char*)rlaheader.chan, "rgb")) {
          b_status = ER_BADIMAGE;
          break;
        }
        if ((rlaheader.num_chan != 3) || (rlaheader.chan_bits != 8)) {
          b_status = ER_BADIMAGE;
          break;
        }
        if (((rlaheader.active_window.right - rlaheader.active_window.left) < 0) || \
        ((rlaheader.active_window.top - rlaheader.active_window.bottom) < 0)) {
          b_status = ER_BADIMAGE;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图?裎募嘈?*/
        pinfo_str->imgformat = IMF_RLA; /* 图像文件格式（后缀名） */
        pinfo_str->compression = ICS_RLE8;
        pinfo_str->width = (ulong)(rlaheader.active_window.right - rlaheader.active_window.left + 1);
        pinfo_str->height = (ulong)(rlaheader.active_window.top - rlaheader.active_window.bottom + 1);
        pinfo_str->order = 1;
        pinfo_str->bitcount = 24; /* 3通道、8位 */
        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;
        pinfo_str->data_state = 1;
      }
      /* 置调色板数为 0 */
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
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }
      /* 回调通知 */
       
      /* 分配用于存储扫描行偏移表的内存 */
      if ((poffset = (uint*)isirw_malloc(4 * pinfo_str->height)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 读取偏移表 */
      if (isio_read((void*)poffset, 4 * pinfo_str->height, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 转换偏移值字序 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        poffset[i] = EXCHANGE_DWORD(poffset[i]);
      }
      /* 分配压缩数据缓冲区 */
      if ((punpack = (uchar*)isirw_malloc(pinfo_str->width * (pinfo_str->bitcount / 8) * 2)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      pr = punpack;
      pg = pr + pinfo_str->width * 2;
      pb = pg + pinfo_str->width * 2;
      /* 分配解压缓冲区 */
      if ((pspl = (uchar*)isirw_malloc(pinfo_str->width * (pinfo_str->bitcount / 8))) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 逐行读入 */
      for (p = pinfo_str->p_bit_data, i = 0; i < (int)(pinfo_str->height); i++) {
        /* 定位扫描行读取位置 */
        if (isio_seek(pfile, poffset[i], SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 只读取前3个通道数据（R、G、B），matte通道暂不读取 */
        /* 读R */
        if (isio_read((void*)&rl, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        rl = EXCHANGE_WORD(rl);
        if (isio_read((void*)pr, rl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 读G */
        if (isio_read((void*)&gl, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        gl = EXCHANGE_WORD(gl);
        if (isio_read((void*)pg, gl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 读B */
        if (isio_read((void*)&bl, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        bl = EXCHANGE_WORD(bl);
        if (isio_read((void*)pb, bl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 查验数据合法性 */
        if ((rl == 0) || (gl == 0) || (bl == 0)) {
          b_status = ER_BADIMAGE;
          break;
        }
        /* 解压缩 */
        if (_decode_scanline(pr, rl, pspl, (int)pinfo_str->width) != 0) {
          b_status = ER_BADIMAGE;
          break;
        }
        if (_decode_scanline(pg, gl, pspl + pinfo_str->width, (int)pinfo_str->width) != 0) {
          b_status = ER_BADIMAGE;
          break;
        }
        if (_decode_scanline(pb, bl, pspl + pinfo_str->width * 2, (int)pinfo_str->width) != 0) {
          b_status = ER_BADIMAGE;
          break;
        }
        p = pinfo_str->p_bit_data + (linesize * i);
        /* 合成目标象素，并写入数据包 */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          *p++ = ((uchar*)(pspl + pinfo_str->width * 2))[j];
          *p++ = ((uchar*)(pspl + pinfo_str->width))[j];
          *p++ = ((uchar*)(pspl))[j];
        }
        /* 进度调用 */
         
      }
      /* 主图象进度结束 */
       
      pinfo_str->data_state = 2;
    }
    while (0);
    
      if (pspl) {
        isirw_free(pspl);
      }
      if (punpack) {
        isirw_free(punpack);
      }
      if (poffset) {
        isirw_free(poffset);
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
EXERESULT rla_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  int i, j, off;
  unsigned int* poffset = 0;
  uchar* ppack = 0, *pspl = 0, *p;
  uchar* pr, *pg, *pb;
  ushort rl, gl, bl, l;
  RLA_HEADER rlaheader;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);
  do {
      /* 判断是否是受支持的图像数据 */
      if (_rla_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        break;
      }
      memset((void*)&rlaheader, 0, sizeof(RLA_HEADER));
      /* 填写流头结构 */
      rlaheader.window.left = 0;
      rlaheader.window.bottom = 0;
      rlaheader.window.right = (short)(pinfo_str->width - 1);
      rlaheader.window.top = (short)(pinfo_str->height - 1);
      rlaheader.active_window.left = 0;
      rlaheader.active_window.bottom = 0;
      rlaheader.active_window.right = (short)(pinfo_str->width - 1);
      rlaheader.active_window.top = (short)(pinfo_str->height - 1);
      rlaheader.revision = (short)RLA_REVISION;
      /* 3通道，R、G、B */
      strcpy(rlaheader.chan, "rgb");
      rlaheader.num_chan = 3;
      rlaheader.chan_bits = 8;
      strcpy(rlaheader.desc, "Saved by ISee RLA plug-in.");
      strcpy(rlaheader.program, "ISee Explorer.");
      /* 图象个数 */
      rlaheader.frame = 1;
      strcpy(rlaheader.gamma, "2.2");
      strcpy(rlaheader.red_pri , "0.670 0.330");
      strcpy(rlaheader.green_pri, "0.210 0.710");
      strcpy(rlaheader.blue_pri , "0.140 0.080");
      strcpy(rlaheader.white_pt , "0.310 0.316");
      strcpy(rlaheader.aspect, "user defined");
      /* 转为MOTO字序 */
      _swap_byte(&rlaheader);
      /* 写入头结构 */
      if (isio_write((const void*)&rlaheader, sizeof(RLA_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
       
      /* 分配偏移表 */
      if ((poffset = (unsigned int*)isirw_malloc(4 * pinfo_str->height)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 分配压缩数据缓冲区 */
      if ((ppack = (uchar*)isirw_malloc(pinfo_str->width * (pinfo_str->bitcount / 8) * 2)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      pr = ppack;
      pg = pr + pinfo_str->width * 2;
      pb = pg + pinfo_str->width * 2;
      /* 分?涞ネǖ阑撼迩?*/
      if ((pspl = (uchar*)isirw_malloc(pinfo_str->width + 4)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 定位通道数据写入的起始点（跨过头结构及偏移表） */
      if (isio_seek(pfile, sizeof(RLA_HEADER) + (4 * pinfo_str->height), SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      off = isio_tell(pfile);
      /* 逐行写入 */
      for (i = 0; i < (int)pinfo_str->height; i++) {
        p = (uchar*)(pinfo_str->pp_line_addr[i]);
        poffset[(int)pinfo_str->height - i - 1] = EXCHANGE_DWORD(off);
        /* 萃取 R 通道数据 */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pspl[j] = p[j * 3 + 2];
        }
        /* 压缩 R 通道数据 */
        rl = (ushort)_encode_scanline(pspl, (int)pinfo_str->width, pr);
        /* 萃取 G 通道数据 */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pspl[j] = p[j * 3 + 1];
        }
        /* 压缩 G 通道数据 */
        gl = (ushort)_encode_scanline(pspl, (int)pinfo_str->width, pg);
        /* 萃取 B 通道数据 */
        for (j = 0; j < (int)pinfo_str->width; j++) {
          pspl[j] = p[j * 3 + 0];
        }
        /* 压缩 B 通道数据 */
        bl = (ushort)_encode_scanline(pspl, (int)pinfo_str->width, pb);
        /* 写入压缩后的数据，顺序为：R、G、B */
        l = EXCHANGE_WORD(rl);
        if (isio_write((const void*)&l, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_write((const void*)pr, rl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        l = EXCHANGE_WORD(gl);
        if (isio_write((const void*)&l, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_write((const void*)pg, gl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        l = EXCHANGE_WORD(bl);
        if (isio_write((const void*)&l, 2, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_write((const void*)pb, bl, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          break;
        }
         
        /* 获取下一行的写入位置 */
        off = isio_tell(pfile);
      }
      /* 定位偏移表写入点 */
      if (isio_seek(pfile, sizeof(RLA_HEADER), SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 写入偏移表 */
      if (isio_write((const void*)poffset, 4 * pinfo_str->height, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        break;
      }
       
    }
    while (0);
    
      if (pspl) {
        isirw_free(pspl);
      }
      if (ppack) {
        isirw_free(ppack);
      }
      if (poffset) {
        isirw_free(poffset);
      }
  return b_status;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 判断传入的图像是否可以被保存（如不支持保存?δ埽山竞サ簦?*/
int _rla_is_valid_img(INFOSTR* pinfo_str)
{
  enum {bitcount = 1UL << (24 - 1)};
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
/* 对RLA_HEADER结构中的short域进行字序转换 */
static void _swap_byte(LPRLA_HEADER prla)
{
  assert(prla);
  prla->window.bottom = EXCHANGE_WORD(prla->window.bottom);
  prla->window.left = EXCHANGE_WORD(prla->window.left);
  prla->window.right = EXCHANGE_WORD(prla->window.right);
  prla->window.top = EXCHANGE_WORD(prla->window.top);
  prla->active_window.bottom = EXCHANGE_WORD(prla->active_window.bottom);
  prla->active_window.left = EXCHANGE_WORD(prla->active_window.left);
  prla->active_window.right = EXCHANGE_WORD(prla->active_window.right);
  prla->active_window.top = EXCHANGE_WORD(prla->active_window.top);
  prla->frame = EXCHANGE_WORD(prla->frame);
  prla->storage_type = EXCHANGE_WORD(prla->storage_type);
  prla->num_chan = EXCHANGE_WORD(prla->num_chan);
  prla->num_matte = EXCHANGE_WORD(prla->num_matte);
  prla->num_aux = EXCHANGE_WORD(prla->num_aux);
  prla->revision = EXCHANGE_WORD(prla->revision);
  prla->job_num = EXCHANGE_DWORD(prla->job_num);
  prla->field = EXCHANGE_WORD(prla->field);
  prla->chan_bits = EXCHANGE_WORD(prla->chan_bits);
  prla->matte_type = EXCHANGE_WORD(prla->matte_type);
  prla->matte_bits = EXCHANGE_WORD(prla->matte_bits);
  prla->aux_type = EXCHANGE_WORD(prla->aux_type);
  prla->aux_bits = EXCHANGE_WORD(prla->aux_bits);
  prla->next = EXCHANGE_DWORD(prla->next);
}
/* 解码一个RLE8的通道压缩数据（一个扫描行中的一个通道） */
static int _decode_scanline(uchar* src, int src_len, uchar* dec, int dec_len)
{
  uchar count;
  assert(src && dec && src_len && dec_len);
  while (src_len > 0) {
    /* 取计数值 */
    count = *src++;
    src_len--;
    if (src_len == 0) {
      return 2; /* 源数据不足 */
    }
    if (count < (uchar)128) {
      count++;
      /* 合法性检测 */
      if (((int)(unsigned int)count) > dec_len) {
        return 1; /* 源数据非法 */
      }
      else {
        dec_len -= (int)(unsigned int)count;
      }
      /* 重复计数次 */
      while (count-- > 0) {
        *dec++ = *src;
      }
      src++;
      src_len--;
    }
    else {
      /* 拷贝未压缩的数据 */
      for (count = (uchar)(256 - (int)(unsigned int)count); count > 0; src_len--, dec_len--, count--) {
        if (dec_len == 0) {
          return 1;
        }
        else {
          *dec++ = *src++;
        }
      }
    }
  }
  if (dec_len != 0) {
    return 3; /* 源数据有缺损 */
  }
  else {
    return 0; /* 成功解压缩 */
  }
}
/* 压缩一个通道的数据（一个扫描行中的一个通道） */
static int _encode_scanline(uchar* src, int src_len, uchar* dec)
{
  int len = 0;
  uchar count;
  assert(src && src_len && dec);
  while (src_len > 0) {
    if ((src_len > 1) && (src[0] == src[1])) {
      /* 累计重复字节个数 */
      for (count = 2; (int)(unsigned int)count < src_len; count++) {
        if (src[(int)(unsigned int)count] != src[(int)(unsigned int)count - 1]) {
          break;
        }
        if ((int)(unsigned int)count >= 127) {
          break;
        }
      }
      /* 写入计数值 */
      *dec++ = count - (uchar)1;
      len++;
      /* 写入重复内容值 */
      *dec++ = *src;
      len++;
      src_len -= (int)(unsigned int)count;
      src += (int)(unsigned int)count;
    }
    else {
      /* 累计不重复字节个数 */
      for (count = 1; (int)(unsigned int)count < src_len; count++) {
        if (((src_len - (int)(unsigned int)count) > 1) && (src[(int)(unsigned int)count] == src[(int)(unsigned int)count + 1])) {
          break;
        }
        if ((int)(unsigned int)count >= 127) {
          break;
        }
      }
      /* 写入计数值 */
      *dec++ = (uchar)((int)256 - (int)(unsigned int)count);
      len++;
      /* 拷贝未压缩数据 */
      for (; count-- > 0; len++, src_len--) {
        *dec++ = *src++;
      }
    }
  }
  return len; /* 返回压缩后的通道数据长度 */
}

