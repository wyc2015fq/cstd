/********************************************************************

  iseeio.h

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
  本文件用途：  ISee I/O 定义文件（兼容标准：ANSI C 流I/O）
  本文件编写人：
          YZ      yzfree##sina.com
          流浪者    chenjm##netease.com
          lzp     zlan##corp.netease.com

  本文件版本：  30912
  最后修改于：  2003-9-12

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
    地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2003-9    模块结构改为在函数指针数组的方式下运行。
          增加插入新介质接口的函数，使模块可以在二进制方式下动态支持新介质。
    2002-11   增加一种顺序读或写文件方式，以利用操作系统提供的
          文件IO高速缓冲功能，目前只支持WIN32。(流浪者)
    2002-10   修正内存流入口参数含义，及引入内存防护系统。
    2002-8    修正联络电邮地址、及编译模式
    2002-4    修正文件缺省打开模式（定为二进制模式）
    2001-6    第一个测试版发布


********************************************************************/



#ifndef __ISEEIO_INC__
#define __ISEEIO_INC__


#ifndef WIN32
#if defined(_WIN32) || defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */


#ifdef WIN32  /* Windows OS */

# ifdef ISEEIO_EXPORTS
# define ISEEIO_API __declspec(dllexport)
# else
# define ISEEIO_API __declspec(dllimport)
# endif
# define CALLAGREEMENT  __cdecl

#else     /* Linux OS */

# define ISEEIO_API
# define CALLAGREEMENT

#endif  /*WIN32*/


#define ISIO_MAX_INFACNUM   32        /* 最多可容纳32套介质接口 */
#define ISIO_OFFIC_INUM     24        /* 官方介质接口最大套数 */
#define ISIO_OEM_INUM     (ISIO_MAX_INFACNUM-ISIO_OFFIC_INUM) /* 第三方介质接口最大套数 */


/* isee I/O 流类型定义 */
typedef enum _iseeio_confer_type {
  ISEEIO_CONFER_INVALID,            /* 无效介质类型       */
  ISEEIO_CONFER_LOC_FILE,           /* 本地文件类型   @@    */
  ISEEIO_CONFER_LOC_MEMORY,         /* 本地内存类型   @@    */
  ISEEIO_CONFER_ROT_INTERNET,         /* 远程internet       */
  ISEEIO_CONFER_LOC_FILE_SEQUENTIAL,      /* 本地顺序访问文件类型 @@  */
  ISEEIO_CONFER_HTTP,             /* HTTP远程文件类型     */
  ISEEIO_CONFER_FTP,              /* FTP远程文件类型      */
  ISEEIO_CONFER_XXX,              /* 新类型 */
  ISEEIO_CONFER_MAX = ISIO_OFFIC_INUM,    /* 官方介质类型边界     */
  ISEEIO_CONFER_OEM0 = ISIO_OFFIC_INUM,   /* 第三方介质类型     */
  ISEEIO_CONFER_OEM1,
  ISEEIO_CONFER_OEM2,
  ISEEIO_CONFER_OEM3,
  ISEEIO_CONFER_OEM4,
  ISEEIO_CONFER_OEM5,
  ISEEIO_CONFER_OEM6,
  ISEEIO_CONFER_OEM7,
  ISEEIO_CONFER_OEM8,
  ISEEIO_CONFER_OEM9,
  ISEEIO_CONFER_OEMA,
  ISEEIO_CONFER_OEMB,
  ISEEIO_CONFER_OEMC,
  ISEEIO_CONFER_OEMD,
  ISEEIO_CONFER_OEME,
  ISEEIO_CONFER_OEMF
} ISEEIO_CONFER_TYPE, *PISEEIO_CONFER_TYPE;
/* 注：以上枚举值中，只有带@@注释的部分才可以被用于isio_open_param函数。*/

#define ISIO_MAX_PATH         512

/* isee I/O 流的地址结构 */
typedef struct _tag_istream_address {
  ISEEIO_CONFER_TYPE  type;         /* 协议类型 */
  char        name[ISIO_MAX_PATH];  /* 流名称 */
  unsigned long   param1;         /* 附加参数1 */
  unsigned long   param2;         /* 附加参数2 */
} ISADDR, *PISADDR;


#if defined(_DEBUG)
#define ISFILE_DBG_ID ("+S+")         /* 流句柄标识字串（调试用）*/
#endif



/* isee I/O 流句柄结构 */
typedef struct _tag_iseeio_stream {
  void*        stream;
  ISEEIO_CONFER_TYPE  stream_type;
  char*        org_name;
#if defined(_DEBUG)
  char        dbg_iseeio_stream_id[4];
#endif
} ISFILE;


#ifndef _FILE_DEFINED
#include <stdio.h>
#endif


/* 本地文件流－FILE 结构已定义在 stdio.h 文件中 */
typedef FILE  FILE_STREAM;

/* 本地顺序访问文件流 - FILE_SEQUENTIAL */
typedef HANDLE  FILE_SEQUENTIAL;

/* 本地内存流－结构定义 */
#define MEMORY_STREAM_UP_SIZE   4096
#define MEMORY_STREAM_UP_STEP   1

typedef struct _tag_memory_stream {
  char*      p_start_pos; /* 内存块的首地址 */
  unsigned long len;      /* 有内容的长度 */
  unsigned long cur_pos;    /* 当前的读写位置 */
  unsigned long mode;     /* 流的打开模式 */

  unsigned long error;      /* 错误标志 */
  unsigned long total_size;   /* 内存块的总尺寸 */
  PISADDR     pisaddr;    /* 回填数据地址（ISADDR）*/
} MEMORY_STREAM;



/* 网络数据流－结构定义 */
typedef struct _tag_internet_stream {
  char*      p_address;
} INTERNET_STREAM;




/* 流模式字－位域含义表：

            32 bit (unsigned long)
  _________________________________________________________________->low bit
  |0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
                                 | |                         | | | |
                                 | bin                       | | | |
                                 text                        | | | read
                                                             | | write
                                                             | append
                               create
*/
#define ISIO_MODE_READ    0x1
#define ISIO_MODE_WRITE   0x2
#define ISIO_MODE_RDWR    0x3
#define ISIO_MODE_APPEND  0x4
#define ISIO_MODE_CREATE  0x8

#define ISIO_MODE_BIN   0x10000
#define ISIO_MODE_TXT   0x20000   /* 非ANSI标准，ISIO将不予支持 */



#ifdef  WIN32
#define ISIO_ERROR_EOF    _IOEOF
#define ISIO_ERROR_ERR    _IOERR
#else
#define ISIO_ERROR_EOF    0x10
#define ISIO_ERROR_ERR    0x20
#endif /* WIN32 */

/* ISIO 接口函数ID定义 */
typedef enum _tagISIOFNTYPE {
  ISIOFN_MIN = 0,                   /* 有效函数ID边界 */

  ISIOFN_OPEN,
  ISIOFN_CLOSE,

  ISIOFN_READ,
  ISIOFN_WRITE,
  ISIOFN_FLUSH,

  ISIOFN_SEEK,
  ISIOFN_REWIND,
  ISIOFN_TELL,
  ISIOFN_LENGTH,

  ISIOFN_EOF,
  ISIOFN_ERROR,
  ISIOFN_CLEARERR,

  ISIOFN_MAX                      /* 有效函数ID边界 */
} ISIOFN, *LPISIOFN;

/* 特定介质的读写接口 */
typedef struct _tagISIOFNSET {
  int     id;                   /* 介质标识 */
  char    name[64 - sizeof(int)];       /* 介质名称 */
  void*    pfn[ISIOFN_MAX];           /* 介质读写接口函数数组 */
} ISIOFNSET, *LPISIOFNSET;


#define _ISIOINFAC_MAGIC    0xbbaaccff        /* 结构标签 */

/* 介质读写接口结构 */
typedef struct _tagISIOINFAC {
  unsigned long magic;                /* 结构标签（等于_ISIOINFAC_MAGIC）*/
  unsigned long rev;                /* 保留 */

  int       total;                /* 介质接口的总容量 */
  int       used;               /* 有效接口套数 */

  ISIOFNSET   ifc[ISIO_MAX_INFACNUM];       /* 介质集 */
} ISIOINFAC, *LPISIOINFAC;


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

  /* ISIO 接口函数原型定义 */
  typedef ISFILE*     (CALLAGREEMENT* ISIOFNT_OPEN)(const char*, const char*);
  typedef int (CALLAGREEMENT* ISIOFNT_CLOSE)(ISFILE*);
  typedef size_t (CALLAGREEMENT* ISIOFNT_READ)(void*, size_t, size_t, ISFILE*);
  typedef size_t (CALLAGREEMENT* ISIOFNT_WRITE)(const void*, size_t, size_t, ISFILE*);
  typedef int (CALLAGREEMENT* ISIOFNT_FLUSH)(ISFILE*);
  typedef int (CALLAGREEMENT* ISIOFNT_SEEK)(ISFILE*, long, int);
  typedef void (CALLAGREEMENT* ISIOFNT_REWIND)(ISFILE*);
  typedef long(CALLAGREEMENT* ISIOFNT_TELL)(ISFILE*);
  typedef int (CALLAGREEMENT* ISIOFNT_EOF)(ISFILE*);
  typedef int (CALLAGREEMENT* ISIOFNT_ERROR)(ISFILE*);
  typedef void (CALLAGREEMENT* ISIOFNT_CLEARERR)(ISFILE*);
  typedef unsigned long(CALLAGREEMENT* ISIOFNT_LENGTH)(ISFILE*);


  /* isee I/O 辅助函数 */
  extern ISEEIO_API const char*       CALLAGREEMENT isio_open_param(PISADDR psct, ISEEIO_CONFER_TYPE type, const char* name, unsigned long param1, unsigned long param2);
  extern ISEEIO_API ISEEIO_CONFER_TYPE  CALLAGREEMENT isio_insert_oem_io(ISIOFNT_OPEN, ISIOFNT_CLOSE, ISIOFNT_READ, ISIOFNT_WRITE, ISIOFNT_FLUSH, ISIOFNT_SEEK, ISIOFNT_REWIND, ISIOFNT_TELL, ISIOFNT_EOF, ISIOFNT_ERROR, ISIOFNT_CLEARERR, ISIOFNT_LENGTH);

  /* isee I/O 流函数 */
  extern ISEEIO_API ISFILE*       CALLAGREEMENT isio_open(const char* pstream_name, const char* mode);
  extern ISEEIO_API int           CALLAGREEMENT isio_close(ISFILE* stream);

  extern ISEEIO_API size_t        CALLAGREEMENT isio_read(void* buffer, size_t size, size_t count, ISFILE* stream);
  extern ISEEIO_API size_t        CALLAGREEMENT isio_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
  extern ISEEIO_API int           CALLAGREEMENT isio_flush(ISFILE* stream);

  extern ISEEIO_API int           CALLAGREEMENT isio_seek(ISFILE* stream, long offset, int origin);
  extern ISEEIO_API void          CALLAGREEMENT isio_rewind(ISFILE* stream);
  extern ISEEIO_API long        CALLAGREEMENT isio_tell(ISFILE* stream);
  extern ISEEIO_API unsigned long CALLAGREEMENT isio_length(ISFILE* stream);

  extern ISEEIO_API int           CALLAGREEMENT isio_eof(ISFILE* stream);
  extern ISEEIO_API int           CALLAGREEMENT isio_error(ISFILE* stream);
  extern ISEEIO_API void          CALLAGREEMENT isio_clearerr(ISFILE* stream);

  /*
  新的ISIO流函数 extern ISEEIO_API xxx CALLAGREEMENT isio_xxxx();
  */

#ifdef _DEBUG
  extern ISEEIO_API int      CALLAGREEMENT __get_object_counts(void);
#endif /*_DEBUG*/

#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif


#endif /* __ISEEIO_INC__ */
