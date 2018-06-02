/********************************************************************

  iseeimn.h

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
  本文件用途：  图像读写引擎－插件管理定义文件
  本文件编写人：
          YZ        yzfree##sina.com

  本文件版本：  30206
  最后修改于：  2003-2-6

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
    地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2003-2    修正空闲处理命令的发送闸值
    2002-8    第一个测试版发布

********************************************************************/

#ifndef __ISEE_IMN_INC__
#define __ISEE_IMN_INC__

#ifdef WIN32  /* Windows */
# define CALLAGREEMENT  __cdecl
#else     /* Linux */
# define CALLAGREEMENT
#endif  /* WIN32 */

#ifndef __ISEE_IRW_INC__
#include "iseeirw.h"                  /* 需要其中的某些结构定义 */
#endif


struct  _tag_irwmf_infoblock;
typedef struct _tag_irwmf_infoblock   IRWMF_INFOBLOCK;
typedef struct _tag_irwmf_infoblock*   LPIRWMF_INFOBLOCK;

struct  _tag_irwmf_header;
typedef struct _tag_irwmf_header    IRWMF_HEADER;
typedef struct _tag_irwmf_header*    LPIRWMF_HEADER;


#define IRWMF_INFOBLOCK_TAG       0xFAFBFBFA    /* IRWMF_INFOBLOCK结构标签 */
#define IRWMF_INFOBLOCK_DEFPIR      30        /* 插件缺省优先级 */
#define IRWMF_INFOBLOCK_USEING      1       /* 录用标记 */
#define IRWMF_INFOBLOCK_NOTUSE      0       /* 未录用标记 */


/* 插件管理信息文件中每个插件对应的信息块结构定义 */
struct _tag_irwmf_infoblock {
  unsigned long   tag;              /* 识别标签(IRWMF_INFOBLOCK_TAG) */

  unsigned long   pr;               /* 插件调入优先级 */
  unsigned long   pcr;              /* 本次使用计数 */
  int         use;              /* 录用标记（1－已录用、0－未录用）*/
  unsigned long   pilen;              /* 插件文件长度 */
  unsigned long   ctimeh;             /* 插件文件创建时间（高32位）*/
  unsigned long   ctimel;             /* 插件文件创建时间（低32位）*/
  unsigned long   mtimeh;             /* 插件文件最后的修改时间 */
  unsigned long   mtimel;             /* 插件文件最后的修改时间 */
  char        path[_MAX_FNAME];       /* 插件文件名 */
  IRWP_INFO     ifo;              /* 插件信息表 */

  int           act;            /* 激活标记 0-未激活、1-激活（*/
  /* 高16位为消息泵使用计数） */
  HMODULE         handle;           /* 插件句柄 */

  IRWP_API_GET_IMG_INFO fn_get_img_info;      /* get_image_info接口 */
  IRWP_API_LOAD_IMG   fn_load_img;        /* load_image接口 */
  IRWP_API_SAVE_IMG   fn_save_img;        /* save_image接口 */

  LPIRWMF_INFOBLOCK   prev;           /* 前一个信息块的地址 */
  LPIRWMF_INFOBLOCK   next;           /* 下一个信息块的地址 */
  LPIRWMF_HEADER      pe;             /* 表头地址 */

  char          rev[28];          /* 保留，用于将来扩展 */
};


#define IRWMF_HEADER_TAG        0xC3C2C2C3
#define IRWMF_HEADER_FILTSIZE     3200
#define IRWMF_HEADER_BUD_NULL     0       /* 无效表头类型 */
#define IRWMF_HEADER_BUD_LOAD     1       /* 读入型表头结构 */
#define IRWMF_HEADER_BUD_CREATE     2       /* 创建型表头结构 */


/* 插件管理信息文件表头结构定义 */
struct _tag_irwmf_header {
  unsigned long   tag;              /* 表头标签(IRWMF_HEADER_TAG) */

  int         mark;             /* 表头信息有效标记（0－无效、1－有效）*/
  int         plugin_num;           /* 插件总个数 */

  LPIRWMF_INFOBLOCK puh;              /* 录用链头地址 */
  LPIRWMF_INFOBLOCK put;              /* 录用链尾地址 */
  int         uc;               /* 录用链中项的个数 */

  LPIRWMF_INFOBLOCK pnh;              /* 未用链头地址 */
  LPIRWMF_INFOBLOCK pnt;              /* 未用链尾地址 */
  int         nc;               /* 未用链中项的个数 */

  int         build;              /* 表创建的方式 */
  int         locked;             /* 表锁定标志（0－未锁定、非0－锁定）*/

  char        infpath[MAX_PATH];        /* 本信息文件全路径 */

  char        rev[64];            /* 保留 */
};


#define IRWMF_FILENAME        ("iseeirw.ini")   /* 插件信息管理文件的名称 */
#define IRWMF_ACTIVE_INIT_CNT   2         /* 初始激活插件的数目 */
#define IRWMF_IDLE_MAX_COUNT    80          /* 空闲处理命令发送闸值 */


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

  int       CALLAGREEMENT ismf_init(char* path, char* fname);
  int       CALLAGREEMENT ismf_close(void);
  int       CALLAGREEMENT ismf_get_status(void);

  LPIRWMF_HEADER  CALLAGREEMENT ismf_lock(void);
  int       CALLAGREEMENT ismf_update(LPIRWMF_HEADER pt, int type, LPUPDATAINFO rinfo);
  int       CALLAGREEMENT ismf_employ(int id);
  int       CALLAGREEMENT ismf_dismissal(int id);
  int       CALLAGREEMENT ismf_unlock(LPIRWMF_HEADER pt);

  void      CALLAGREEMENT ismf_get_plugin_path(char* path);

  unsigned long CALLAGREEMENT ismf_get_plugin_heandle(const char* path);
  int       CALLAGREEMENT ismf_free_plugin_heandle(unsigned long hand);

  IRWP_API_GET_IMG_INFO CALLAGREEMENT ismf_get_info_addr(unsigned long hand);
  IRWP_API_LOAD_IMG   CALLAGREEMENT ismf_get_load_addr(unsigned long hand);
  IRWP_API_SAVE_IMG   CALLAGREEMENT ismf_get_save_addr(unsigned long hand);



#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif

#endif /* __ISEE_IMN_INC__ */
