/********************************************************************

  iseeirw.h

  ----------------------------------------------------------------
    软件许可证 － GPL
  版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
  ----------------------------------------------------------------
  这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
  可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
  据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
  目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
  GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
  ----------------------------------------------------------------
  如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

      http://isee.126.com
      http://cosoft.org.cn/projects/iseeexplorer

  或发信到：

      isee##vip.163.com
  ----------------------------------------------------------------
  本文件用途：  图象读写引擎－对外接口定义文件
  本文件编写人：
          YZ        yzfree##sina.com
          xiaoyueer   xiaoyueer##263.net
          orbit     Inte2000##263.net
          Janhail     janhail##sina.com

  本文件版本：  30902
  最后修改于：  2003-9-2

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
  地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2003-9    增加LCD插件格式定义
    2003-7    增加isirw_add_idle()和isirw_get_idlesse_count()两个内用函数接口
    2003-5    增加UPDATAINFO结构定义
    2003-5    修改isirw_get_eng_info()函数原型，增加获取插件路径功能
    2003-4    修改isirw_s_read_img_c()、isirw_cnv()接口函数原型
    2003-4    增加isirw_orgpix_malloc()、isirw_cnvpix_malloc()内用接口
    2003-2    增加错误类型ER_WAITOUTTIME及专业读写接口isirw_s_wait()
    2002-10   增加有关新版读写引擎的结构及宏定义
    2002-6    增加YUV转换宏、字对齐计算宏、及几种新图象格式定义
    2002-5    增加JBIG压缩方式定义
    2002-4    增加一些各插件都能使用宏定义
    2002-3    增加播放顺序表。修正相关的INFOSTR分配及释放函数
    2002-3    增加数种新图象格式定义，增加数据结构移植定义
    2002-1    修正静态图像文件不能包含多幅图像的错误注释
    2001-6    修改新版接口
    2000-7    添加图像压缩类型
    2000-6    第一个测试版发布

********************************************************************/


#ifndef __ISEE_IRW_INC__
#define __ISEE_IRW_INC__


#ifdef WIN32  /* Windows */

# ifdef ISEEIRW_EXPORTS
# define ISEEIRW_API __declspec(dllexport)
# else
# define ISEEIRW_API __declspec(dllimport)
# endif
# define CALLAGREEMENT  __cdecl

#else     /* Linux */

# define ISEEIRW_API
# define CALLAGREEMENT

typedef try     __try
    typedef finally   __finally
      typedef leave   __leave
      typedef except    __except

#endif  /*WIN32*/



/* 开发者信息结构 */
struct _tag_author_info;

typedef struct _tag_author_info  AUTHOR_INFO;
typedef struct _tag_author_info*  LPAUTHOR_INFO;


/* 插件能处理的图像文件类型信息结构 */
struct _tag_irwp_desc_info;
typedef struct _tag_irwp_desc_info  IRWP_DESC_INFO;
typedef struct _tag_irwp_desc_info*  LPIRWP_DESC_INFO;


/* 插件自身信息结构 */
struct _tag_irwp_info;
typedef struct _tag_irwp_info IRWP_INFO;
typedef struct _tag_irwp_info* LPIRWP_INFO;


/* 保存功能参数设定描述 */
struct _tag_irw_save_para_desc;
typedef struct _tag_irw_save_para_desc  IRW_SAVE_PARA_DESC;
typedef struct _tag_irw_save_para_desc*  LPIRW_SAVE_PARA_DESC;


/* 保存功能描述 */
struct _tag_irw_save_desc;
typedef struct _tag_irw_save_desc IRW_SAVE_DESC;
typedef struct _tag_irw_save_desc* LPIRW_SAVE_DESC;


/* 多桢图像的子图像块结构（主要用于动画类图像文件）*/
struct _tag_subimage_block;
typedef struct _tag_subimage_block  SUBIMGBLOCK;
typedef struct _tag_subimage_block*  LPSUBIMGBLOCK;


/* 图像读写数据包结构 */
struct _tag_infostr;
typedef struct _tag_infostr   INFOSTR;
typedef struct _tag_infostr*   LPINFOSTR;


/* 保存图像时用于设定参数的结构 */
struct _tag_savestr;
typedef struct _tag_savestr   SAVESTR;
typedef struct _tag_savestr*   LPSAVESTR;


/* 引擎刷新函数返回信息结构 */
struct _tag_irw_update_info;
typedef struct _tag_irw_update_info UPDATAINFO;
typedef struct _tag_irw_update_info* LPUPDATAINFO;


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


/* 图像读写模块执行结果 */
enum EXERESULT {
  ER_SUCCESS,     /* 执行成功 */
  ER_USERBREAK,   /* 操作被用户中断 */
  ER_BADIMAGE,    /* 指定的文件是一个受损的图像文件 */
  ER_NONIMAGE,    /* 指定的文件不是一个图像文件 */
  ER_MEMORYERR,   /* 内存不足，导致操作失败 */
  ER_FILERWERR,   /* 文件在读写过程中发生错误（系统、硬件等原因引起文件读写异常） */
  ER_SYSERR,      /* 操作系统不稳定，无法执行命令 */
  ER_NOTSUPPORT,    /* 不支持的命令 */
  ER_NSIMGFOR,    /* 保存功能不支持的图像格式 */
  ER_WAITOUTTIME,   /* 读写操作超时引发的退出动作 */
  /*ER_XXXX,*/    /* 新的错误信息在此插入 */
  ER_MAX = 64     /* 边界值 */
};


/* 图像类型 */
enum IMGTYPE {
  IMT_NULL,     /* 无效图像类型 */
  IMT_RESSTATIC,    /* 光栅、静态图像 */
  IMT_RESDYN,     /* 光栅、动态图像 */
  IMT_VECTORSTATIC, /* 矢量、静态图像 */
  IMT_VECTORDYN,    /* 矢量、动态图像 */
  IMT_CNV,      /* 转换而来的图像 */
  /*IMT_XXXXX,*/    /* 在此插入新的图像类型 */
  IMT_MAX = 64    /* 边界值 */
};


/* 图像格式 */
enum IMGFORMAT {
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
  IMF_IFF,      /* ... */
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
  IMF_ICN,      /* ... */
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
};



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

/* 开发者信息结构 */
struct _tag_author_info {
  char ai_name[AI_NAME_SIZE];           /* 开发者名字 */
  char ai_email[AI_EMAIL_SIZE];         /* 开发者email地址 */
  char ai_message[AI_MESSAGE_SIZE];       /* 开发者留言。（最大字数限制在159个*/
  /* 英文字符，或79个汉字）*/
};




#define IDI_CURRENCY_NAME_SIZE  16
#define IDI_SYNONYM_NAME_COUNT  4

/* 插件能处理的图像文件类型信息结构（即扩展名信息） */
struct _tag_irwp_desc_info {
  char idi_currency_name[IDI_CURRENCY_NAME_SIZE]; /* 通用扩展名，必填 */
  unsigned long idi_rev;              /* 保留 */
  unsigned long idi_synonym_count;        /* 衍生扩展名有效项个数 */
  char idi_synonym[IDI_SYNONYM_NAME_COUNT][IDI_CURRENCY_NAME_SIZE];
  /* 衍生扩展名 */
};



#define ISPD_ITEM_NUM     16
#define ISPD_STRING_SIZE    32

/* 保存功能选择参数 */
struct _tag_irw_save_para_desc {
  int     count;                /* 有效描述项个数 */
  char    desc[ISPD_STRING_SIZE];       /* 结构描述串 */

  int     value[ISPD_ITEM_NUM];       /* 值列表 */
  char    value_desc[ISPD_ITEM_NUM][ISPD_STRING_SIZE];/* 对应于值的描述 */
};  /* 612 byte */


#define ISD_ITEM_NUM      4

/* 保存功能描述 */
struct _tag_irw_save_desc {
  unsigned long bitcount;           /* 位深支持描述 */
  int       img_num;            /* 对图像个数的要求，0－无要求  1－只能保存一副图像  2－可保存多幅图像 */
  int       count;              /* 有效描述项个数 */
  IRW_SAVE_PARA_DESC    para[ISD_ITEM_NUM];
};  /* 2456 byte */


#define IRWP_NAME_SIZE      32          /* 插件名称的最大长度 */
#define IRWP_AUTHOR_SIZE    16          /* 单一插件开发允许的最多参与人数 */
#define IRWP_FUNC_PREFIX_SIZE 16          /* 函数前缀的最大尺寸 */

#define IRWP_READ_SUPP      0x1         /* 读支持 */
#define IRWP_WRITE_SUPP     0x2         /* 写支持 */
#define IRWP_REREAD_SUPP    0x4         /* 重读支持（当前未实现）*/


/* 插件自身信息结构 */
struct _tag_irwp_info {
  unsigned long init_tag;             /* 初始化标签，0－表示未初始化，*/
  /* 1－表示已初始化 */
  enum IMGFORMAT plug_id;             /* 插件唯一标识符 */

  char irwp_name[IRWP_NAME_SIZE];         /* 插件名称 */
  char irwp_func_prefix[IRWP_FUNC_PREFIX_SIZE]; /* 本模块函数前缀，如BMP模块函数前缀为"bmp_" */

  unsigned long irwp_version;           /* 版本号。（十进制值，*/
  /* 十位为主版本号，个位为副版本，*/
  /* 如12，即表示1.2版） */
  unsigned long irwp_build_set;         /* 0－调试版插件，1－发布版插件 */

  unsigned long irwp_function;          /* 功能标识：*/
  /*   0位是读支持标志，*/
  /*   1位是写支持标志，*/
  /*   其它未用 */
  IRW_SAVE_DESC irwp_save;            /*保存功能描述 */
  unsigned long irwp_author_count;        /* 开发者人数（即开发者信息中有效项的个数）*/
  AUTHOR_INFO irwp_author[IRWP_AUTHOR_SIZE];    /* 开发者信息 */
  IRWP_DESC_INFO  irwp_desc_info;         /* 插件描述信息（扩展名信息）*/

};  /* size (in byte) - 4K+160+2456 byte */


/* 引擎刷新函数的刷新模式 */
#define IRWE_UPDTYPE_EMPLOY   0         /* 录用所有新插件 */
#define IRWE_UPDTYPE_DISMISSAL  1         /* 不录用新插件 */

/* 引擎刷新函数返回信息结构 */
struct _tag_irw_update_info {
  int   newcnt;                 /* 新增插件的个数 */
  void*  newpp[ISEEIRW_MAX_PLUGIN_CNT];     /* 新增插件的插件信息块结构地址（数组中每一个元素都是一个插件信息块结构的地址，需先转换为LPIRWMF_INFOBLOCK类型）*/
  int   updcnt;                 /* 刷新（已升级）插件的个数 */
  void*  updpp[ISEEIRW_MAX_PLUGIN_CNT];     /* 刷新插件的插件信息块结构地址 */
  int   delcnt;                 /* 被注销插件的个数 */
  char  delpp[ISEEIRW_MAX_PLUGIN_CNT][IRWP_NAME_SIZE];  /* 被注销插件的插件名称串 */
};


#define MAX_PALETTE_COUNT   256         /* 最大调色板项个数  */

#ifdef _DEBUG
#define INFOSTR_DBG_MARK    0x65655349      /* 图像结构标识串"ISee" */
#endif  /*_DEBUG*/



/* 保存图像时用于设定参数的结构 */
struct _tag_savestr {
  int     para_value[ISD_ITEM_NUM]; /* 保存参数对应值（见IRW_SAVE_DESC结构） */
};



/* 字、双字对齐宏（DIBSCANLINE_WIDTHBYTES因兼容目的而被保留） */
#define DIBSCANLINE_WORDALIGN(bits)   ((((bits)+15)>>4)<<1)
#define DIBSCANLINE_DWORDALIGN(bits)    ((((bits)+31)>>5)<<2)
#define DIBSCANLINE_WIDTHBYTES(bits)    ((((bits)+31)>>5)<<2)


/* 68K字序与X86字序互换宏 */
#define EXCHANGE_DWORD(dword) (((dword&0xff)<<24)|((dword&0xff00)<<8)|((dword&0xff0000)>>8)|((dword&0xff000000)>>24))
#define EXCHANGE_WORD(word)   (((word&0xff)<<8)|((word&0xff00)>>8))


/* 1、2、4位象素字节内位置对调宏 */
#define CVT_BITS4(c) \
  {\
    c = (unsigned char)((((unsigned char)c)>>4) | (((unsigned char)c)<<4));\
  }

#define CVT_BITS2(c) \
  {\
    c = (unsigned char)( ((((unsigned char)c)>>6)&0x3) | ((((unsigned char)c)>>2)&0xc) | \
        ((((unsigned char)c)<<2)&0x30)| ((((unsigned char)c)<<6)&0xc0)  );\
  }

#define CVT_BITS1(c) \
  {\
    c = (unsigned char)( ((((unsigned char)c)>>7)&0x1) | ((((unsigned char)c)>>5)&0x2) | \
        ((((unsigned char)c)>>3)&0x4) | ((((unsigned char)c)>>1)&0x8) | \
        ((((unsigned char)c)<<1)&0x10)| ((((unsigned char)c)<<3)&0x20)| \
        ((((unsigned char)c)<<5)&0x40)| ((((unsigned char)c)<<7)&0x80) );\
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
    unsigned short k1 = 255 - (unsigned short)(unsigned char)k; \
    r = (unsigned char)((c > k1) ? (c-k1) : 0); \
    g = (unsigned char)((m > k1) ? (m-k1) : 0); \
    b = (unsigned char)((y > k1) ? (y-k1) : 0); \
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

#include <math.h>

/* YUV(YCbCr)转RGB宏 */
#define YUV2RGB(y, cb, cr, pr, pg, pb)    \
  {\
    double  t;  \
    t = floor(irw_yuv_cr1 * y + irw_yuv_cr2 * (cb - 156) + irw_yuv_cr3 * (cr - 137) + 0.5);\
    *pr = (unsigned char)((t < 0) ? 0 : (t > 255) ? 255 : t);\
    t = floor(irw_yuv_cg1 * y + irw_yuv_cg2 * (cb - 156) + irw_yuv_cg3 * (cr - 137) + 0.5);\
    *pg = (unsigned char)((t < 0) ? 0 : (t > 255) ? 255 : t);\
    t = floor(irw_yuv_cb1 * y + irw_yuv_cb2 * (cb - 156) + irw_yuv_cb3 * (cr - 137) + 0.5);\
    *pb = (unsigned char)((t < 0) ? 0 : (t > 255) ? 255 : t);\
  }




/* 调色板项结构 */
typedef struct _tag_palette_item {
  unsigned char blue;
  unsigned char green;
  unsigned char red;
  unsigned char reserved;
} PALITEM, *LPPALITEM;



/* Windows 平台数据结构移植 */
#ifndef WIN32

typedef struct _tag_rgb_quad {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD, LPRGBQUAD;

typedef struct _tag_bitmap_info_header {
  unsigned long biSize;
  long      biWidth;
  long      biHeight;
  unsigned short  biPlanes;
  unsigned short  biBitCount;
  unsigned long biCompression;
  unsigned long biSizeImage;
  long      biXPelsPerMeter;
  long      biYPelsPerMeter;
  unsigned long biClrUsed;
  unsigned long biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER;

typedef struct _tag_bitmap_info {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD          bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO;

#endif /* WIN32 */




typedef struct _tag_faci_datapack   FACI_DATAPACK;
typedef struct _tag_faci_datapack*   LPFACI_DATAPACK;

typedef struct _tag_spec_datapack   SPEC_DATAPACK;
typedef struct _tag_spec_datapack*   LPSPEC_DATAPACK;


#define DATAPACK_MARK_FACILITY      0x44504D46    /* 简易数据包标签（DPMF）*/
#define DATAPACK_MARK_SPECIALTY     0x44504D53    /* 专业数据包标签（DPMS）*/


/* 简易数据包原型 */
struct _tag_faci_datapack {
  unsigned long dbg_mark;   /* 结构标识，等于DATAPACK_MARK_FACILITY */

  unsigned long width;      /* 图像宽度 */
  unsigned long height;     /* 图像高度 */
  unsigned long bitcount;   /* 每个像素所占的位数（位深度） */
  int       std_id;     /* 所属的标准格式ID号 */

  unsigned long palette[MAX_PALETTE_COUNT];   /* 调色板数据 */
  unsigned char* pbits;     /* 图像数据缓冲区 */
};




/*********************************************************************************/


/* 多桢（页）图像的子图像块结构 */
struct _tag_subimage_block {
  int   number;         /* 子图像块的序列号 */

  int   left, top;        /* 子图像块的显示位置（左上角）*/
  int   width, height;      /* 子图像块的图像宽度及高度 */
  int   bitcount;       /* 子图像块位深 */
  int   order;          /* 图像的上下顺序 （0－正向，1－倒向）*/

  int   dowith;         /* 处置方法（只针对动态图像）*/
  int   userinput;        /* 用户输入（只针对动态图像）*/
  int   time;         /* 显示的延迟时间（只针对动态图像） */

  unsigned long r_mask;     /* 各颜色分量的掩码值 */
  unsigned long g_mask;     /* 注：1~8位的图像无掩码数据 */
  unsigned long b_mask;
  unsigned long a_mask;

  long      colorkey;   /* 透明色分量值 */

  void**      pp_line_addr; /* 标准图像位数据中从0行到n-1行的每一行的首地址 */
  unsigned char* p_bit_data;  /* 标准图像位数据缓冲区首地址 */
  int       pal_count;    /* 调色板有效项的个数 */
  unsigned long palette[MAX_PALETTE_COUNT];   /* 调色板数据 */

  LPINFOSTR   parents;    /* 第一桢图像数据的结构地址（所属数据包结构的地址）*/
  LPSUBIMGBLOCK prev;     /* 前一桢子图像数据的结构地址 */
  LPSUBIMGBLOCK next;     /* 后一桢子图像数据的结构地址（NULL表示这是最后一副图像）*/
};


/* 步进回调命令定义 */
typedef enum _tag_pfun_command {
  PFC_NULL,           /* 空命令 */
  PFC_INTO_PLUG,          /* 开始执行命令 */
  PFC_BUILD_MAST_IMAGE,     /* 创立主图象 */
  PFC_BUILD_SERT_IMAGE,     /* 创立新的子图象 */
  PFC_PROCESS,          /* 步进 */
  PFC_OVER_SIR,         /* 结束一副图象的操作（主图象或子图象）*/
  PFC_OK,             /* 命令被成功执行 */
  PFC_BREAK,            /* 命令被中断 */
} PFUN_COMM, *LPPFUN_COMM;


/*
  步进回调函数原型定义
  LPSPEC_DATAPACK －专业数据包地址
  PFUN_COMM   －命令类型（见PFUN_COMM的定义注释）
  int       －图象序号（0为主图象，其他为子图象序号）
  int       －起始行号（以缓冲区基地址为基准）
  int       －连续行数
  int       －调用者标识（0－读函数、1－写函数）
*/
typedef int (*IRWPFUN)(LPSPEC_DATAPACK, PFUN_COMM, int, int, int, int);


/* 图像读写数据包结构（核心结构） */
struct _tag_infostr {
#ifdef _DEBUG
  unsigned long sct_mark;   /* 结构的标识，用于调试，等于INFOSTR_DBG_MARK */
#endif

  enum IMGTYPE  imgtype;    /* 图像文件类型 */
  enum IMGFORMAT  imgformat;    /* 图像文件格式（插件标识） */
  enum IMGCOMPRESS compression; /* 图像文件的压缩方式 */

  unsigned long width;      /* 图像宽度 */
  unsigned long height;     /* 图像高度 */
  unsigned long bitcount;   /* 每个像素所占的位数（位深度） */
  unsigned long order;      /* 图像的上下顺序 （0－正向，1－倒向）*/

  unsigned long r_mask;     /* 各颜色分量的掩码值 */
  unsigned long g_mask;     /* 注：1~8位的图像无掩码数据 */
  unsigned long b_mask;
  unsigned long a_mask;

  void**      pp_line_addr; /* 图像位数据中从0行到n-1行每一行的首地址 */
  unsigned char* p_bit_data;  /* 图像位数据缓冲区首地址 */
  unsigned int  pal_count;    /* 调色板有效项的个数 */
  unsigned long palette[MAX_PALETTE_COUNT];   /* 调色板数据 */

  unsigned long play_number;  /* 播放顺序表尺寸（以元素为单位） */
  unsigned long* play_order;  /* 播放顺序表 */
  unsigned long time;     /* 首帧停留时间 */
  long      colorkey;   /* 主图象透明色（-1表示无透明色） */

  unsigned long imgnumbers;   /* 该文件中图像的个数 */
  LPSUBIMGBLOCK psubimg;    /* 子图像数据链地址 */

  unsigned short  data_state;   /* 数据包当前的数据状态：
                      0 － 空的数据包，没有任何变量数据是有效的
                      1 － 存在图像的描述信息
                      2 － 存在图像数据
                  */
  CRITICAL_SECTION  access;   /* 访问许可标志 */

  LPSPEC_DATAPACK   pater;    /* 所属的专业数据包地址 */
  IRWPFUN       irwpfun;  /* 回调函数 */

  int         omnipotence;/* 多用途变量。在转换包中存放标准格式ID号 */

  long        rev[28];  /* 保留 */
};


/* 专业数据包执行状态 */
typedef enum {
  IRWE_SDPSTATE_WAIT = 0,
  IRWE_SDPSTATE_EXEC = 1,
  IRWE_SDPSTATE_IDLE = 2
} SDP_EXEC_STATE, *LPSDPSTATE;


/* 专业数据包数据状态 */
typedef unsigned long       SDP_DATA_STATE;

#define SDP_DATA_STATE_PATH     0x1
#define SDP_DATA_STATE_ORG_INFO   0x2
#define SDP_DATA_STATE_ORG_PIX    0x4
#define SDP_DATA_STATE_CNV      0x8


/* 专业数据包可执行命令的类型 */
typedef enum {
  IRWE_SDPCOMM_NULL,
  IRWE_SDPCOMM_GET_INFO,
  IRWE_SDPCOMM_LOAD,
  IRWE_SDPCOMM_SAVE
} SDP_COMM_TYPE, *LPSDP_COMM_TYPE;


/* 专业数据包转换开关值定义 */
typedef enum _tag_cnvmark {
  IRWE_CNVMARK_NOT    = -2,     /* 不进行任何转换 */
  IRWE_CNVMARK_AUTO   = -1,     /* 自动转为最匹配的标准格式 */
  IRWE_CNVMARK_1      = 0,      /* 强制转为 1 位调色板图 */
  IRWE_CNVMARK_4      = 1,      /* 强制转为 4 位调色板图 */
  IRWE_CNVMARK_8      = 2,      /* 强制转为 8 位调色板图 */
  IRWE_CNVMARK_16_555 = 3,      /* 强制转为 16 位(555)格式位图 */
  IRWE_CNVMARK_16_565 = 4,      /* 强制转为 16 位(565)格式位图 */
  IRWE_CNVMARK_24     = 5,      /* 强制转为 24 位位图 */
  IRWE_CNVMARK_32     = 6       /* 强制转为 32 位位图(带Alpha掩码) */
} IRWE_CNVMARK, *LPIRWE_CNVMARK;


/* 回调函数类型ID */
#define IRWE_CALLBACK_FUNID_LOAD  0 /* 读取函数 */
#define IRWE_CALLBACK_FUNID_SAVE  1 /* 保存函数 */


#ifndef __ISEEIO_INC__
#include "iseeio.h"         /* ISADDR结构的定义 */
#endif


/* 专业数据包原型 */
struct _tag_spec_datapack {
  unsigned long dbg_mark;   /* 结构的标识，用于调试，等于DATAPACK_MARK_SPECIALTY */

  PISADDR     path;     /* 源图象地址信息 */
  PISADDR     despath;    /* 目标图象地址信息 */
  LPSAVESTR   savestr;    /* 保存参数描述 */

  LPINFOSTR   org_img;    /* 原始图象数据 */
  LPINFOSTR   cnv_img;    /* 转换后的图象数据 */

  SDP_COMM_TYPE command;    /* 当前要执行的命令 */
  SDP_EXEC_STATE  exec_state;   /* 执行状态 */
  SDP_DATA_STATE  data_state;   /* 数据状态 */
  enum EXERESULT  last_err;   /* 最后一次操作的结果信息 */

  unsigned long s_total;    /* 总进度值（累进式）*/
  unsigned long s_current;    /* 当前进度值 */
  int       s_state;    /* 进度阶段（图象号） */
  int       s_break;    /* 终止操作标志（非0终止） */

  IRWE_CNVMARK  cnvmark;    /* 用户设定的转换开关（参见IRWE_CNVMARK定义）*/
  IRWPFUN     callback;   /* 用户回调函数 */

  unsigned long sn;       /* 批次号 */

  unsigned long lab1;     /* 留给引擎使用者的变量，其含义由使用者自行定义 */
  unsigned long lab2;

  char      rev[60];    /* 保留 */
};



/* 插件API原型定义 */

/* 获得ISee图像读写插件内部信息函数的接口指针类型（内部使用） */
typedef LPIRWP_INFO(CALLAGREEMENT* IRWP_ISX)(void);
/* get_image_info接口指针类型 */
typedef int (CALLAGREEMENT* IRWP_API_GET_IMG_INFO)(PISADDR, LPINFOSTR);
/* load_image接口指针类型 */
typedef int (CALLAGREEMENT* IRWP_API_LOAD_IMG)(PISADDR, LPINFOSTR);
/* save_image接口指针类型 */
typedef int (CALLAGREEMENT* IRWP_API_SAVE_IMG)(PISADDR, LPINFOSTR, LPSAVESTR);

/* 定义IRWP识别标签 */
#define IRWP_TAG  "is_irw_plugin"

/* 常用长度单位 */
#define IRWE_KB             1024
#define IRWE_MB             (IRWE_KB*IRWE_KB)

/* 引擎内部缓冲区最大长度 */
#define IRWE_MAX_BUFFSIZE       (10*IRWE_MB)  /* 缺省为10MB */

/* 引擎内部缓冲开始类型 */
#define IRWE_BUFTYPE_ORG        1       /* 原始象素缓冲区 */
#define IRWE_BUFTYPE_CNV        2       /* 转换象素缓冲区 */


/* IRWP API 类型定义 */
enum IRWP_API_NAME {
  get_image_info = 1,
  load_image,
  save_image
};


/* 引擎的接口操作中最后一次操作的错误值定义 */
#define ISIRW_LASTERR_SUCCESS     0
#define ISIRW_LASTERR_SYSCLOSE      1       /* 引擎系统未开启 */
#define ISIRW_LASTERR_MEM       2       /* 内存不足 */
#define ISIRW_LASTERR_RW        3       /* 介质读写错误 */
#define ISIRW_LASTERR_USER        4       /* 操作被用户中断 */
#define ISIRW_LASTERR_IMGERR      5       /* 图象错误(受损或非图象) */
#define ISIRW_LASTERR_OSERR       10        /* 操作系统错误 */


struct  _tag_irwmf_infoblock;
typedef struct _tag_irwmf_infoblock   IRWMF_INFOBLOCK;
typedef struct _tag_irwmf_infoblock*   LPIRWMF_INFOBLOCK;

struct  _tag_irwmf_header;
typedef struct _tag_irwmf_header    IRWMF_HEADER;
typedef struct _tag_irwmf_header*    LPIRWMF_HEADER;


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif


#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif

#endif  //__ISEE_IRW_INC__
