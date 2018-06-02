
#ifndef __ISEE_IRW_INC__
#define __ISEE_IRW_INC__

/* ISee图象读写引擎当前版本 */
#define ISEEIRW_ENG_VER     20

/* 当前的ISee图象读写引擎使用的插件接口版本 */
#define ISEEIRW_ENG_PLUGIN_VER  22

/* ISee图象读写插件支持的最大位深值 */
#define ISEEIRW_MAX_BITCOUNT  32

/* ISee图象读写引擎支持的最大插件数目 */
#define ISEEIRW_MAX_PLUGIN_CNT  128

/* 引擎过滤串缓冲区最大尺寸（每个插件的过滤名占用24个字节）*/
#define ISIRW_OPENSTR_SIZE    (ISEEIRW_MAX_PLUGIN_CNT*24)

/* 线程切换等待的空闲时间长度（ms）*/
#define ISIRW_THREAD_IDLE   10


#define __LEAVE if (ER_SUCCESS != b_status) { break; }
#define IF_LEAVE(X) if (ER_SUCCESS != (b_status=(X))) { break; }
/* 图像读写模块执行结果 */
typedef enum EXERESULT {
  ER_SUCCESS,       /* 执行成功 */
  ER_USERBREAK,     /* 操作被用户中断 */
  ER_BADIMAGE,      /* 指定的文件是一个受损的图像文件 */
  ER_NONIMAGE,      /* 指定的文件不是一个图像文件 */
  ER_MEMORYERR,     /* 内存不足，导致操作失败 */
  ER_FILERWERR,     /* 文件在读写过程中发生错误（系统、硬件等原因引起文件读写异常） */
  ER_SYSERR,        /* 操作系统不稳定，无法执行命令 */
  ER_NOTSUPPORT,    /* 不支持的命令 */
  ER_NSIMGFOR,      /* 保存功能不支持的图像格式 */
  ER_WAITOUTTIME,   /* 读写操作超时引发的退出动作 */
  /*ER_XXXX,*/      /* 新的错误信息在此插入 */
  ER_MAX = 64       /* 边界值 */
} EXERESULT;


/* 图像类型 */
typedef enum {
  IMT_NULL,         /* 无效图像类型 */
  IMT_RESSTATIC,    /* 光栅、静态图像 */
  IMT_RESDYN,       /* 光栅、动态图像 */
  IMT_VECTORSTATIC, /* 矢量、静态图像 */
  IMT_VECTORDYN,    /* 矢量、动态图像 */
  IMT_CNV,          /* 转换而来的图像 */
  /*IMT_XXXXX,*/    /* 在此插入新的图像类型 */
  IMT_MAX = 64      /* 边界值 */
} IMGTYPE;


/* 图像格式 */
typedef enum {
  IMF_NULL,     /* 未知或无效的图像 */
  IMF_BMP,      /* BMP格式的图像 */
  IMF_CUR,      /* .... */
  IMF_ICO,
  IMF_PCX,
  IMF_DCX,
  IMF_GIF,
  IMF_PNG,
  IMF_TGA,
  IMF_WMF,
  IMF_EMF,
  IMF_TIFF,
  IMF_JPEG,     /* ... */
  IMF_PSD,      /* PSD格式的图像 */
  IMF_XBM,
  IMF_XPM,
  IMF_PCD,
  IMF_RAW,
  IMF_RAS,
  IMF_PPM,
  IMF_PGM,
  IMF_PBM,
  IMF_IFF,
  IMF_JP2,      /* JPEG2000格式图像 */
  IMF_LBM,
  IMF_BW,
  IMF_PIX,
  IMF_RLE,
  IMF_XWD,
  IMF_DXF,
  IMF_WBMP,
  IMF_ANI,
  IMF_ART,
  IMF_FPX,
  IMF_ICN,
  IMF_KDC,
  IMF_LDF,
  IMF_LWF,
  IMF_MAG,
  IMF_PIC,
  IMF_PCT,
  IMF_PDF,
  IMF_PS,
  IMF_PSP,
  IMF_RSB,
  IMF_SGI,
  IMF_RLA,
  IMF_RLB,
  IMF_RLC,
  IMF_JBIG,
  IMF_APM,
  IMF_MNG,
  IMF_CGM,
  IMF_PAL,
  IMF_YUV,
  IMF_DJVU,
  IMF_DICOM,
  IMF_LCD,
  /*IMF_XXX*/     /* 新增格式在此处插入 */
  IMF_MAX = 511   /* 边界值 */
} IMGFORMAT;



/* 象素数据的存放形式 */
enum IMGCOMPRESS {
  ICS_RGB,        /* 未经压缩的RGB存储方式 */
  ICS_RLE4,       /* RLE4存储方式 */
  ICS_RLE8,       /* RLE8存储方式 */
  /* @@@@@@@@@@Added By xiaoyueer 2000.8.20 */
  ICS_RLE16,        /* RLE16存储方式 */
  ICS_RLE24,        /* RLE24存储方式 */
  ICS_RLE32,        /* RLE32存储方式 */
  /* @@@@@@@@@End 2000.8.20 */
  ICS_BITFIELDS,      /* 分位存储方式 */
  ICS_PCXRLE,       /* PCX RLE存储方式 */
  ICS_GIFLZW,       /* GIF LZW存储方式 */
  ICS_GDIRECORD,      /* WMF的元文件存储方式 */
  /* @@@@@@@@@@Added By orbit 2000.8.31 */
  ICS_JPEGGRAYSCALE,    /* 灰度图象,JPEG-256级灰度 */
  ICS_JPEGRGB,      /* red/green/blue */
  ICS_JPEGYCbCr,      /* Y/Cb/Cr (also known as YUV) */
  ICS_JPEGCMYK,     /* C/M/Y/K */
  ICS_JPEGYCCK,     /* Y/Cb/Cr/K */
  /* @@@@@@@@@End 2000.8.31 */
  /* @@@@@@@@@@LibTiff: Sam Leffler 2001.1.6 */
  ICS_TIFF_NONE,      /* TIFF non-compress ABGR mode */
  ICS_TIFF_CCITTRLE,    /* CCITT modified Huffman RLE */
  ICS_TIFF_CCITTAX3,    /* CCITT Group 3 fax encoding */
  ICS_TIFF_CCITTFAX4,   /* CCITT Group 4 fax encoding */
  ICS_TIFF_LZW,     /* Lempel-Ziv  & Welch */
  ICS_TIFF_OJPEG,     /* 6.0 JPEG */
  ICS_TIFF_JPEG,      /* JPEG DCT compression */
  ICS_TIFF_NEXT,      /* NeXT 2-bit RLE */
  ICS_TIFF_CCITTRLEW,   /* 1 w/ word alignment */
  ICS_TIFF_PACKBITS,    /* Macintosh RLE */
  ICS_TIFF_THUNDERSCAN, /* ThunderScan RLE */
  /* @@@@@@@@@@Added By <dkelly@etsinc.com> */
  ICS_TIFF_IT8CTPAD,    /* IT8 CT w/padding */
  ICS_TIFF_IT8LW,     /* IT8 Linework RLE */
  ICS_TIFF_IT8MP,     /* IT8 Monochrome picture */
  ICS_TIFF_IT8BL,     /* IT8 Binary line art */
  ICS_TIFF_PIXARFILM,   /* Pixar companded 10bit LZW */
  ICS_TIFF_PIXARLOG,    /* Pixar companded 11bit ZIP */
  ICS_TIFF_DEFLATE,   /* Deflate compression */
  ICS_TIFF_ADOBE_DEFLATE, /* Deflate compression, as recognized by Adobe */
  /* @@@@@@@@@@Added By <dev@oceana.com> */
  ICS_TIFF_DCS,     /* Kodak DCS encoding */
  ICS_TIFF_JBIG,      /* ISO JBIG */
  ICS_TIFF_SGILOG,    /* SGI Log Luminance RLE */
  ICS_TIFF_SGILOG24,    /* SGI Log 24-bit packed */
  /* @@@@@@@@@End 2001.1.6 */
  /* @@@@@@@@@@Added By YZ 2001.8.3 */
  ICS_PNG_GRAYSCALE,    /* PNG灰度图 */
  ICS_PNG_PALETTE,    /* PNG索引图 */
  ICS_PNG_RGB,      /* PNG真彩图 */
  /* @@@@@@@@@End 2001.8.3 */
  ICS_XBM_TEXT,     /* 文本方式 */
  ICS_XPM_TEXT,
  /* @@@@@@@@@@Added By YZ 2002.2.3 */
  ICS_PNM_TEXT,
  ICS_PNM_BIN,
  /* @@@@@@@@@End 2002.2.3 */
  /* @@@@@@@@@@Added By YZ 2002.5.30 */
  ICS_JBIG,
  /* @@@@@@@@@End 2002.5.30 */
  /*ICS_XXXX,*/     /* 插入新的存储方式类型 */
  ICS_UNKONW = 1024,    /* 未知的压缩方式 */
  ICS_MAX = 2048      /* 边界 */
};

#define AI_NAME_SIZE    32
#define AI_EMAIL_SIZE   64
#define AI_MESSAGE_SIZE   160

#define IDI_CURRENCY_NAME_SIZE  16
#define IDI_SYNONYM_NAME_COUNT  4

#define ISPD_ITEM_NUM     16
#define ISPD_STRING_SIZE    32

#define ISD_ITEM_NUM      4

#define IRWP_NAME_SIZE      32          /* 插件名称的最大长度 */
#define IRWP_AUTHOR_SIZE    16          /* 单一插件开发允许的最多参与人数 */
#define IRWP_FUNC_PREFIX_SIZE 16          /* 函数前缀的最大尺寸 */

#define IRWP_READ_SUPP      0x1         /* 读支持 */
#define IRWP_WRITE_SUPP     0x2         /* 写支持 */
#define IRWP_REREAD_SUPP    0x4         /* 重读支持（当前未实现）*/

/* 引擎刷新函数的刷新模式 */
#define IRWE_UPDTYPE_EMPLOY   0         /* 录用所有新插件 */
#define IRWE_UPDTYPE_DISMISSAL  1         /* 不录用新插件 */

#define MAX_PALETTE_COUNT   256         /* 最大调色板项个数  */

#ifdef _DEBUG
#define INFOSTR_DBG_MARK    0x65655349      /* 图像结构标识串"ISee" */
#endif  /*_DEBUG*/


/* 字、双字对齐宏（DIBSCANLINE_WIDTHBYTES因兼容目的而被保留） */
#define DIBSCANLINE_WORDALIGN(bits)   ((((bits)+15)>>4)<<1)
#define DIBSCANLINE_DWORDALIGN(bits)    ((((bits)+31)>>5)<<2)
#define DIBSCANLINE_WIDTHBYTES(bits)    ((((bits)+31)>>5)<<2)

/* 计算扫描行尺寸(四字节对齐) */
#define _calcu_scanline_size(w/* 宽度 */, bit/* 位深 */)  DIBSCANLINE_WIDTHBYTES(w * bit)

/* 68K字序与X86字序互换宏 */
#define EXCHANGE_DWORD(dword) (((dword&0xff)<<24)|((dword&0xff00)<<8)|((dword&0xff0000)>>8)|((dword&0xff000000)>>24))
#define EXCHANGE_WORD(word)   (((word&0xff)<<8)|((word&0xff00)>>8))


/* 1、2、4位象素字节内位置对调宏 */
#define CVT_BITS4(c)   { c = (uchar)((((uchar)c)>>4) | (((uchar)c)<<4)); }

#define CVT_BITS2(c)  {\
    c = (uchar)( ((((uchar)c)>>6)&0x3) | ((((uchar)c)>>2)&0xc) | \
        ((((uchar)c)<<2)&0x30)| ((((uchar)c)<<6)&0xc0)  );\
  }

#define CVT_BITS1(c)   {\
    c = (uchar)( ((((uchar)c)>>7)&0x1) | ((((uchar)c)>>5)&0x2) | \
        ((((uchar)c)>>3)&0x4) | ((((uchar)c)>>1)&0x8) | \
        ((((uchar)c)<<1)&0x10)| ((((uchar)c)<<3)&0x20)| \
        ((((uchar)c)<<5)&0x40)| ((((uchar)c)<<7)&0x80) );\
  }


/* 掩码设置宏（p -> LPINFOSTR） */
#define SETMASK_32(p)   \
  {\
    p->b_mask = 0xff;   \
    p->g_mask = 0xff00;   \
    p->r_mask = 0xff0000; \
    p->a_mask = 0xff000000; \
  }
#define SETMASK_24(p)   \
  {\
    p->b_mask = 0xff;   \
    p->g_mask = 0xff00;   \
    p->r_mask = 0xff0000; \
    p->a_mask = 0x0;    \
  }
#define SETMASK_8(p)    \
  {\
    p->b_mask = 0x0;    \
    p->g_mask = 0x0;    \
    p->r_mask = 0x0;    \
    p->a_mask = 0x0;    \
  }


/* CMYK色转RGB宏（极粗糙的转换）*/
#define CVT_CMYK_TO_RGB(c, m, y, k, r, g, b)  \
  {\
    ushort k1 = 255 - (ushort)(uchar)k; \
    r = (uchar)((c > k1) ? (c-k1) : 0); \
    g = (uchar)((m > k1) ? (m-k1) : 0); \
    b = (uchar)((y > k1) ? (y-k1) : 0); \
  }

/* YUV转RGB所需的常量定义 */
#define irw_yuv_cr1   ((double)((double) 0.0054980  * 256.0))
#define irw_yuv_cr2   ((double)((double) 0.0000001  * 256.0))
#define irw_yuv_cr3   ((double)((double) 0.0051681  * 256.0))
#define irw_yuv_cg1   ((double)((double) 0.0054980  * 256.0))
#define irw_yuv_cg2   ((double)((double)-0.0015446  * 256.0))
#define irw_yuv_cg3   ((double)((double)-0.0026325  * 256.0))
#define irw_yuv_cb1   ((double)((double) 0.0054980  * 256.0))
#define irw_yuv_cb2   ((double)((double) 0.0079533  * 256.0))
#define irw_yuv_cb3   ((double)((double) 0.0000001  * 256.0))



/* YUV(YCbCr)转RGB宏 */
#define YUV2RGB(y, cb, cr, pr, pg, pb)    \
  {\
    double  t;  \
    t = floor(irw_yuv_cr1 * y + irw_yuv_cr2 * (cb - 156) + irw_yuv_cr3 * (cr - 137) + 0.5);\
    *pr = (uchar)((t < 0) ? 0 : (t > 255) ? 255 : t);\
    t = floor(irw_yuv_cg1 * y + irw_yuv_cg2 * (cb - 156) + irw_yuv_cg3 * (cr - 137) + 0.5);\
    *pg = (uchar)((t < 0) ? 0 : (t > 255) ? 255 : t);\
    t = floor(irw_yuv_cb1 * y + irw_yuv_cb2 * (cb - 156) + irw_yuv_cb3 * (cr - 137) + 0.5);\
    *pb = (uchar)((t < 0) ? 0 : (t > 255) ? 255 : t);\
  }

/* 调色板项结构 */
typedef struct _tag_palette_item {
  uchar blue;
  uchar green;
  uchar red;
  uchar reserved;
} PALITEM;

/////////////////////////////////////////////////////////////////////////
/* Windows 平台数据结构移植 */
typedef const char* LPCTSTR;
typedef uchar BYTE;
typedef ushort WORD;
typedef ulong DWORD;
typedef long LONG;
#pragma pack(1)
typedef struct {
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} wBITMAPINFOHEADER;
typedef struct {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
} wRGBQUAD;
typedef struct {
  wBITMAPINFOHEADER bmiHeader;
  wRGBQUAD bmiColors[1];
} wBITMAPINFO;
#pragma pack()

/*********************************************************************************/

typedef struct _SUBIMGBLOCK SUBIMGBLOCK;
/* 多桢图像的子图像块结构（主要用于动画类图像文件）*/
struct _SUBIMGBLOCK {
  int   left, top;        /* 子图像块的显示位置（左上角）*/
  int   width, height;      /* 子图像块的图像宽度及高度 */
  int   bitcount;       /* 子图像块位深 */
  int stride;
  int   number;         /* 子图像块的序列号 */
  int   order;          /* 图像的上下顺序 （0－正向，1－倒向）*/
  int   dowith;         /* 处置方法（只针对动态图像）*/
  int   userinput;        /* 用户输入（只针对动态图像）*/
  int   time;         /* 显示的延迟时间（只针对动态图像） */
  ulong r_mask;     /* 各颜色分量的掩码值 */
  ulong g_mask;     /* 注：1~8位的图像无掩码数据 */
  ulong b_mask;
  ulong a_mask;
  
  long      colorkey;   /* 透明色分量值 */
  
  uchar** pp_line_addr; /* 标准图像位数据中从0行到n-1行的每一行的首地址 */
  uchar* p_bit_data;  /* 标准图像位数据缓冲区首地址 */
  int    pal_count;    /* 调色板有效项的个数 */
  COLOR palette[MAX_PALETTE_COUNT];   /* 调色板数据 */
  
  //INFOSTR*   parents;    /* 第一桢图像数据的结构地址（所属数据包结构的地址）*/
  //SUBIMGBLOCK* prev;     /* 前一桢子图像数据的结构地址 */
  //SUBIMGBLOCK* next;     /* 后一桢子图像数据的结构地址（NULL表示这是最后一副图像）*/
};

/* 图像读写数据包结构（核心结构） */
typedef struct {
  //char* name;
  ulong width;      /* 图像宽度 */
  ulong height;     /* 图像高度 */
  ulong bitcount;   /* 每个像素所占的位数（位深度） */
  ulong order;      /* 图像的上下顺序 （0－正向，1－倒向）*/
  int stride;
  
  ulong r_mask;     /* 各颜色分量的掩码值 */
  ulong g_mask;     /* 注：1~8位的图像无掩码数据 */
  ulong b_mask;
  ulong a_mask;

  int     para_value[ISD_ITEM_NUM]; /* 保存参数对应值（见IRW_SAVE_DESC结构） */
  IMGTYPE  imgtype;    /* 图像文件类型 */
  IMGFORMAT  imgformat;    /* 图像文件格式（插件标识） */
  IMGCOMPRESS compression; /* 图像文件的压缩方式 */
  PixelFormat pixfmt;
  int lockmode;
  int numlocks;
  int lockx, locky;

  uchar** pp_line_addr; /* 图像位数据中从0行到n-1行每一行的首地址 */
  uchar* p_bit_data;  /* 图像位数据缓冲区首地址 */
  unsigned int  pal_count;    /* 调色板有效项的个数 */
  COLOR palette[MAX_PALETTE_COUNT];   /* 调色板数据 */

  ulong play_number;  /* 播放顺序表尺寸（以元素为单位） */
  ulong* play_order;  /* 播放顺序表 */
  ulong time;     // 首帧停留时间
  long colorkey;   // 主图象透明色（-1表示无透明色）

  int current_frame;
  int imgnumbers;   // 该文件中图像的个数
  SUBIMGBLOCK* psubimg;    // 子图像数据链地址
//     数据包当前的数据状态：
//       0 － 空的数据包，没有任何变量数据是有效的
//       1 － 存在图像的描述信息
//       2 － 存在图像数据
  ushort  data_state;
} INFOSTR;

typedef INFOSTR Bitmap;
typedef INFOSTR Image;

#endif  //__ISEE_IRW_INC__
