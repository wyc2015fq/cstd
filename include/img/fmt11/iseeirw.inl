/********************************************************************

  iseeirw.c

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
  本文件用途：  图象读写引擎－对外接口实现文件
  本文件编写人：
          YZ        yzfree##sina.com

  本文件版本：  30723
  最后修改于：  2003-7-23

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
  地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2003-7    增加isirw_add_idle()和isirw_get_idlesse_count()两个内用函数接口
    2003-7    修正isirw_cnv中存在的两处BUG（原来对自动转换的支持有问题）
    2003-5    修改isirw_update()函数原型，增加返回信息结构
          修改isirw_get_eng_info()函数原型，增加获取插件路径功能
    2003-4    改进isirw_s_read_img_c()接口函数的线程安全性能。
          修正isirw_cnv()接口函数，增加了转换行为控制。
    2003-4    增加isirw_orgpix_malloc()、isirw_cnvpix_malloc()内用接口。
    2003-2    增加isirw_s_wait()接口函数，改进多线程特性。
    2002-12   修正简易读写接口中的两处BUG。感谢网友易秋超！
    2002-10   新版图象读写引擎编写完毕。
    2002-8    图象读写引擎及图象转换引擎并入本模块。
          插件接口升级至2.2。
          引入内存防护概念。
    2002-3    增加播放顺序表。修正相关的INFOSTR分配及释放函数
    2002-1    修正静态图像文件不能包含多幅图像的错误注释
    2001-6    第一个测试版发布

********************************************************************/

#ifndef WIN32
#if defined(_WIN32) || defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#ifdef WIN32
#include <pmalloc.h>
#else
#include <mem.h>
#endif

#include "iseemem.h"
#include "iseeirw.h"
#include "iseecnv.h"
#include "iseeimn.h"
#include "queue.h"
#include "iseeeng.h"



/**********************************************************************************/
/* 内部用数据 */

#ifdef _DEBUG
static unsigned long sct_count = 0;           /* 结构实体计数（数据包的个数）*/
static CRITICAL_SECTION  sct_access;          /* 计数器访问许可标识 */
#endif

static int      _isirw_mark = 0;          /* 引擎系统启用标志 */
static int      _isirw_lock = 0;          /* 引擎锁定标志 */
static int      _isirw_init_result = -10;     /* 引擎初始化的结果（见isirw_init()函数的返回值）*/

static int      _s_last_err = ISIRW_LASTERR_SUCCESS;/* 专业型接口最后一次操作的错误信息 */
static int      _f_last_err = ISIRW_LASTERR_SUCCESS;/* 简易型接口最后一次操作的错误信息 */

static unsigned int _f_img_order = 1;         /* 简易型数据包图象的上下顺序，缺省为倒向 */
/* 如需要可手动修改它的值，以改变引擎行为 */
/* 0－正向、1－倒向 */

static unsigned int   _isirw_buff_mark = 0;     /* 内用缓冲区开启标记（IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV）*/
static unsigned char*  _isirw_porg     = 0;     /* 原始象素缓冲区地址 */
static unsigned char*  _isirw_pcnv     = 0;     /* 转换象素缓冲区地址 */

#ifdef WIN32
#pragma pack(push)
#pragma pack(4)
#endif

static int      _isirw_org_used = 0;        /* 原始象素缓冲区使用标志（非0－使用）*/
static int      _isirw_cnv_used = 0;        /* 转换象素缓冲区使用标志 */

#ifdef WIN32
#pragma pack(pop)
#endif

static char* IRWP_API_NAME_STRING[4] = { "", "get_image_info", "load_image", "save_image"};
/* 读写插件接口函数通用名 */




/**********************************************************************************/
/* 内部辅助函数 */

static void CALLAGREEMENT _free_subimg(LPINFOSTR pinfo);
static int  CALLAGREEMENT _defalt_process(LPSPEC_DATAPACK, PFUN_COMM, int, int, int, int);
ISEEIRW_API IRWPFUN CALLAGREEMENT isirw_set_pfun(LPINFOSTR pinfo, IRWPFUN new_pfun);

ISEEIRW_API LPSUBIMGBLOCK CALLAGREEMENT isirw_alloc_SUBIMGBLOCK();
ISEEIRW_API void      CALLAGREEMENT isirw_free_SUBIMGBLOCK(LPSUBIMGBLOCK p_node);

int     CALLAGREEMENT _isirw_get_last_sert_image_info(LPINFOSTR pinfo, LPSUBIMGBLOCK* ppsi);
int     CALLAGREEMENT _isirw_get_sert_image_info(LPINFOSTR pinfo, int sid, LPSUBIMGBLOCK* ppsi);

LPINFOSTR         CALLAGREEMENT _isirw_build_mi(LPSPEC_DATAPACK sdp, int fmt);
LPSUBIMGBLOCK       CALLAGREEMENT _isirw_build_si(LPSUBIMGBLOCK psub_str, int fmt);






/**********************************************************************************/
/* 管理接口函数 */
/**********************************************************************************/

/* 初始化图象读写引擎 */
ISEEIRW_API int CALLAGREEMENT isirw_init(char* path, char* fname)
{
  int     result = 0;

  assert(_isirw_mark == 0);             /* 只能初始化一次 */

  __try {
    __try {
      /* 建立插件管理模块 */
      if ((result = ismf_init(path, fname)) != 0) {
        __leave;
      }

      /* 建立消息泵模块 */
      if ((result = open_pump()) != 0) {
        __leave;
      }

      /* 设置引擎启用标志 */
      _isirw_mark = 1;
    }
    __finally {
      if ((result != 0) || (AbnormalTermination())) {
        if (ismf_get_status()) {
          ismf_close();
        }

        if (get_pump_status()) {
          close_pump();
        }
      }

      /* 初始化象素缓冲区参数 */
      _isirw_buff_mark = 0;           /* 内用缓冲区开启标记 */
      _isirw_porg      = 0;           /* 原始象素缓冲区地址 */
      _isirw_pcnv      = 0;           /* 转换象素缓冲区地址 */
      _isirw_org_used  = 0;           /* 原始象素缓冲区使用标志（非0－使用）*/
      _isirw_cnv_used  = 0;           /* 转换象素缓冲区使用标志 */
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = -5;
  }

  _isirw_init_result = result;

  /*  0 :成功 */
  /* -1 :内存不足 */
  /* -2 :无可用插件 */
  /* -3 :文件读写错误 */
  /* -4 :INI文件内容非法 */
  /* -5 :系统异常 */
  /* -6 :消息泵线程创建失败 */
  return result;
}


/* 关闭图象读写引擎，并释放有关资源 */
ISEEIRW_API void CALLAGREEMENT isirw_close()
{
  assert(_isirw_mark == 1);
  assert(get_pump_status());
  assert(ismf_get_status());

  __try {
    /* 关闭消息泵及插件管理模块 */
    close_pump();
    ismf_close();
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    ;
  }

  /* 如果调用者开启了象素缓冲区，则关闭它 */
  isirw_dibuffer(IRWE_BUFTYPE_ORG | IRWE_BUFTYPE_CNV);

  _isirw_mark = 0;

  return;
}


/* 锁定图象读写引擎 */
ISEEIRW_API LPIRWMF_HEADER CALLAGREEMENT isirw_lock()
{
  LPIRWMF_HEADER result = 0;

  assert(_isirw_mark == 1);

  __try {
    /* 锁定消息泵 */
    lock_pump();

    /* 锁定插件信息表，并返回表基址 */
    result = ismf_lock();

    _isirw_lock = 1;
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 0;
  }

  /* 成功－锁定系统并返回插件信息表基址 */
  /* 失败－返回0 */
  return result;
}


/* 刷新插件信息表 */
ISEEIRW_API int CALLAGREEMENT isirw_update(LPIRWMF_HEADER pt, int type, LPUPDATAINFO rinfo)
{
  int result = 0;

  /* 刷新期间，系统必须处于锁定状态 */
  assert(pt);
  assert(_isirw_mark == 1);
  assert(_isirw_lock == 1);
  assert((type == IRWE_UPDTYPE_EMPLOY) || (type == IRWE_UPDTYPE_DISMISSAL));

  __try {
    if (rinfo) {
      /* 重置返回信息结构 */
      memset((void*)rinfo, 0, sizeof(UPDATAINFO));
    }

    /* 刷新插件信息表 */
    result = ismf_update(pt, type, rinfo);
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 4;
  }

  /* 0: 刷新成功 */
  /* 1: 引擎中已没有任何图象读写插件（引擎将无法正常工作）*/
  /* 2: 读写错误 */
  /* 3: 内存不足 */
  /* 4: 系统异常 */
  return result;
}


/* 录用指定插件 */
ISEEIRW_API int CALLAGREEMENT isirw_employ(int id /*待录用插件的ID号码*/)
{
  int result = 0;

  /* 刷新期间，系统必须处于锁定状态 */
  assert(_isirw_mark == 1);
  assert(_isirw_lock == 1);

  __try {
    /* 将指定的未用插件转至录用链表（但并不激活）*/
    ismf_employ(id);
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 1;
  }

  /* 0: 成功 */
  /* 1: 系统异常 */
  return result;
}


/* 屏蔽指定插件（取消录用）*/
ISEEIRW_API int CALLAGREEMENT isirw_dismissal(int id /*待‘解雇’插件的ID号码*/)
{
  int result = 0;

  /* 刷新期间，系统必须处于锁定状态 */
  assert(_isirw_mark == 1);
  assert(_isirw_lock == 1);

  __try {
    /* 将指定的录用插件转至未用链表（如果该插件已被激活，则先使其休眠）*/
    ismf_dismissal(id);
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 1;
  }

  /* 0: 成功 */
  /* 1: 系统异常 */
  return result;
}


/* 解锁图象读写引擎 */
ISEEIRW_API int CALLAGREEMENT isirw_unlock(LPIRWMF_HEADER pt)
{
  int result = 0;

  assert(_isirw_mark == 1);
  assert(pt && (pt->tag == IRWMF_HEADER_TAG));

  __try {
    /* 插件信息表解锁 */
    ismf_unlock(pt);

    /* 消息泵解锁 */
    unlock_pump();

    _isirw_lock = 0;
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 1;
  }

  /* 0: 解锁成功 */
  /* 1: 系统异常 */
  return result;
}


/* 开启内用象素缓冲区 */
ISEEIRW_API int CALLAGREEMENT isirw_enbuffer(int type)
{
  int iRet = 0;

  assert(_isirw_mark == 1);

  /* 分配原始象素缓冲区 */
  if (type & IRWE_BUFTYPE_ORG) {
    if (!(_isirw_buff_mark & IRWE_BUFTYPE_ORG)) {
      if ((_isirw_porg = (unsigned char*)im_malloc(IRWE_MAX_BUFFSIZE)) != 0) {
        _isirw_buff_mark |= IRWE_BUFTYPE_ORG;
      }
      else {
        iRet = -1;                /* 内存不足 */
      }
    }
  }

  /* 分配转换象素缓冲区 */
  if (type & IRWE_BUFTYPE_CNV) {
    if (!(_isirw_buff_mark & IRWE_BUFTYPE_CNV)) {
      if ((_isirw_pcnv = (unsigned char*)im_malloc(IRWE_MAX_BUFFSIZE)) != 0) {
        _isirw_buff_mark |= IRWE_BUFTYPE_CNV;
      }
      else {
        iRet = -1;                /* 内存不足 */
      }
    }
  }

  return iRet;
}


/* 关闭内用象素缓冲区 */
ISEEIRW_API int CALLAGREEMENT isirw_dibuffer(int type)
{
  int iRet = 0;

  assert(_isirw_mark == 1);

  /* 关闭原始象素缓冲区 */
  if (type & IRWE_BUFTYPE_ORG) {
    if (_isirw_buff_mark & IRWE_BUFTYPE_ORG) {
      assert(_isirw_porg);
      assert(_isirw_org_used == 0);       /* 释放缓冲区时，该缓冲区必须处于空闲状态 */

      im_free(_isirw_porg);
      _isirw_porg = 0;
      _isirw_buff_mark &= ~IRWE_BUFTYPE_ORG;
    }
  }

  /* 关闭转换象素缓冲区 */
  if (type & IRWE_BUFTYPE_CNV) {
    if (_isirw_buff_mark & IRWE_BUFTYPE_CNV) {
      assert(_isirw_pcnv);
      assert(_isirw_cnv_used == 0);

      im_free(_isirw_pcnv);
      _isirw_pcnv = 0;
      _isirw_buff_mark &= ~IRWE_BUFTYPE_CNV;
    }
  }

  return iRet;
}


/* 获取引擎自身的一些信息（包括引擎版本、插件接口版本、调试状态）*/
ISEEIRW_API void CALLAGREEMENT isirw_get_eng_info(
    int* peng_ver,    /* 返回当前引擎的版本号 */
    int* pplug_ver,   /* 返回当前引擎所使用的读写插件接口版本号 */
    int* pdebug,    /* 返回系统是否是调试版本（0－调试状态、1－发布状态）*/
    char* path)     /* 返回插件所在的路径 */
{
  assert(_isirw_mark == 1);

  if (peng_ver) {
    *peng_ver = ISEEIRW_ENG_VER;          /* 引擎版本 */
  }

  if (pplug_ver) {
    *pplug_ver = ISEEIRW_ENG_PLUGIN_VER;      /* 插件接口版本 */
  }

  if (pdebug) {
#ifdef _DEBUG
    *pdebug = 0;                  /* 调试状态 */
#else
    *pdebug = 1;                  /* 发布状态 */
#endif /* _DEBUG */
  }

  if (path) {
    ismf_get_plugin_path(path);           /* 获取插件所在路径（尾部不带'\'符号）*/
  }
}


/* 获取引擎忙碌程度的信息 */
ISEEIRW_API int CALLAGREEMENT isirw_is_busy()
{
  unsigned long cnt;
  int       result;

  assert(_isirw_mark == 1);

  /* 获取消息泵中还有多少个待处理的命令 */
  cnt = get_comm_count();

  if (cnt == 0) {
    result = 0;                   /* 闲 */
  }
  else if (cnt < 10) {
    result = 1;                   /* 忙 */
  }
  else {
    result = 2;                   /* 繁忙 */
  }

  if (_isirw_lock == 1) {
    result = 3;                   /* 系统被锁定 */
  }

  return result;
}


/* （插件管理模块内用接口）－根据提供的文件路径串来搜索对应插件的信息块地址 */
int CALLAGREEMENT _search_plugin_info_block(LPIRWMF_INFOBLOCK* ppblock, const char* path);

/* 判断指定的图象是否能被引擎处理（以扩展名判断）*/
ISEEIRW_API int CALLAGREEMENT isirw_is_supimg(const char* path)
{
  LPIRWMF_INFOBLOCK piib;             /* 占位符 */

  assert(_isirw_mark == 1);

  /* 1 - 可以被引擎处理 */
  /* 0 - 不能被引擎处理 */
  return (_search_plugin_info_block(&piib, path) == 1) ? 1 : 0;
}


/* 插件管理模块中获取过滤串的内用接口 */
char* CALLAGREEMENT _ismf_get_openstr(void);
char* CALLAGREEMENT _ismf_get_shellstr(void);

/* 获取用于Windows平台Open对话框的文件扩展名过滤串 */
/* 串格式请参见MSDN中有关OPENFILENAME结构的文档（其中的lpstrFilter成员）*/
ISEEIRW_API char* CALLAGREEMENT isirw_get_openstr()
{
  assert(_isirw_mark == 1);

  return _ismf_get_openstr();
}


/* 获取通用文件扩展名过滤串 */
/* 串格式为："*.bmp,*.gif,*.xxx,....." */
ISEEIRW_API char* CALLAGREEMENT isirw_get_shellstr()
{
  assert(_isirw_mark == 1);

  return _ismf_get_shellstr();
}


/* 判断引擎是否建立成功 */
ISEEIRW_API int CALLAGREEMENT isirw_is_ok()
{
  /*  0 :成功创建 */
  /* -1 :内存不足 */
  /* -2 :无可用插件 */
  /* -3 :文件读写错误 */
  /* -4 :INI文件内容非法 */
  /* -5 :系统异常 */
  /* -6 :消息泵线程创建失败 */
  /* -10:引擎还未被初始化 */
  return _isirw_init_result;
}




/**********************************************************************************/
/* 专业图象读写接口 */
/**********************************************************************************/

/* 获得图象的宽、高、位深度等信息 */
ISEEIRW_API LPSPEC_DATAPACK CALLAGREEMENT isirw_s_get_img_info(ISEEIO_CONFER_TYPE type, const char* name, unsigned long par1, unsigned long par2, unsigned long sn)
{
  LPSPEC_DATAPACK   psdp = 0;

  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(name);

  /* 初始化最后操作变量 */
  _s_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* 申请并初始化一个专业数据包 */
      if ((psdp = isirw_alloc_SPEC_DATAPACK(type, name, par1, par2, 0)) == 0) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* 填写命令参数 */
      psdp->command     = IRWE_SDPCOMM_GET_INFO;
      psdp->data_state |= SDP_DATA_STATE_PATH;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->sn          = sn;
      psdp->callback    = 0;

      /* 送入消息泵 */
      if (add_pump_comm(psdp) == -1) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }
    }
    __finally {
      if ((_s_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        if (psdp) {
          isirw_free_SPEC_DATAPACK(psdp);
          psdp = 0;
        }
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _s_last_err = ISIRW_LASTERR_OSERR;
  }

  return psdp;
}


/* 获得图象的象素数据（如有必要可进行位深度转换）*/
ISEEIRW_API LPSPEC_DATAPACK CALLAGREEMENT isirw_s_read_img(LPSPEC_DATAPACK psdp, IRWE_CNVMARK cnv, void* fn)
{
  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(psdp);

  /* 初始化最后操作变量 */
  _s_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* 填写命令参数 */
      psdp->command     = IRWE_SDPCOMM_LOAD;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->cnvmark     = cnv;
      psdp->s_break     = 0;
      psdp->callback    = (IRWPFUN)fn;

      /* 送入消息泵 */
      if (add_pump_comm(psdp) == -1) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }
    }
    __finally {
      if ((_s_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        psdp->last_err   = ER_MEMORYERR;
        psdp->exec_state = IRWE_SDPSTATE_IDLE;
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _s_last_err = ISIRW_LASTERR_OSERR;
  }

  return psdp;
}


/* 直接获得图象的象素数据（如有必要可进行位深度转换）（不需要事先调用isirw_s_get_img_info函数）*/
ISEEIRW_API int CALLAGREEMENT isirw_s_read_img_c(ISEEIO_CONFER_TYPE type, const char* name, unsigned long par1, unsigned long par2, unsigned long sn, IRWE_CNVMARK cnv, void* fn, LPSPEC_DATAPACK* pppack)
{
  LPSPEC_DATAPACK   psdp = 0;

  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(name);

  /* 初始化最后操作变量 */
  _s_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      *pppack = 0;

      /* 申请并初始化一个专业数据包 */
      if ((psdp = isirw_alloc_SPEC_DATAPACK(type, name, par1, par2, cnv)) == 0) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* 填写命令参数 */
      psdp->command     = IRWE_SDPCOMM_LOAD;
      psdp->data_state |= SDP_DATA_STATE_PATH;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->sn          = sn;
      psdp->callback    = (IRWPFUN)fn;

      /* 先回送专业包地址，以避免引擎线程通过回调函数操作主程序相关
         变量（此时本函数可能还未来得及返回）。*/
      *pppack = psdp;

      /* 送入消息泵 */
      if (add_pump_comm(psdp) == -1) {
        _s_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }
    }
    __finally {
      if ((_s_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        if (psdp) {
          isirw_free_SPEC_DATAPACK(psdp);
          psdp = 0;
        }

        *pppack = 0;
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _s_last_err = ISIRW_LASTERR_OSERR;
  }

  return _s_last_err;
}


/* 将源专业数据包中的图象数据保存到指定的介质中 */
ISEEIRW_API int CALLAGREEMENT isirw_s_write_img(ISEEIO_CONFER_TYPE type, const char* name, unsigned long par1, unsigned long par2, LPSPEC_DATAPACK psdp, LPSAVESTR savedec, void* fn)
{
  int   result = 0;

  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(psdp);

  /* 初始化最后操作变量 */
  _s_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* 填写命令参数 */
      psdp->command     = IRWE_SDPCOMM_SAVE;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->callback    = (IRWPFUN)fn;

      /* 复制保存参数 */
      memcpy((void*)psdp->savestr->para_value, (const void*)savedec->para_value, sizeof(int)*ISD_ITEM_NUM);

      /* 填写目标图象路径 */
      isio_open_param(psdp->despath, type, name, par1, par2);

      /* 送入消息泵 */
      if (add_pump_comm(psdp) == -1) {
        _s_last_err = ISIRW_LASTERR_MEM;
        result = 1;               /* 内存不足 */
        __leave;
      }
    }
    __finally {
      if ((_s_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        psdp->last_err   = ER_MEMORYERR;
        psdp->exec_state = IRWE_SDPSTATE_IDLE;
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 2;                   /* 操作系统不稳定 */
    _s_last_err = ISIRW_LASTERR_OSERR;
  }

  return result;
}


/* 废弃指定批次的所有命令（但已经执行的命令无法废弃）*/
ISEEIRW_API int CALLAGREEMENT isirw_dist_sn(unsigned long sn)
{
  int   count = 0;

  assert(_isirw_mark == 1);

  /* 废弃所有还没有执行的sn批次的命令 */
  while (dist_pump_comm_sn(sn)) {
    count++;                    /* 被废弃的命令个数 */
  }

  return count;
}


/* 获取专业型读写接口的最后一次操作的错误代码 */
ISEEIRW_API int CALLAGREEMENT isirw_s_get_last_err()
{
  return _s_last_err;
}


/* 获取ISee标准格式的相关格式信息 */
ISEEIRW_API int CALLAGREEMENT isirw_s_get_id_info(int id,
    unsigned long* bit,
    unsigned long* rmask,
    unsigned long* gmask,
    unsigned long* bmask,
    unsigned long* amask,
    int* pal_size,
    LPPALITEM* pppal)
{
  return iscnv_get_id_info(id, bit, rmask, gmask, bmask, amask, pal_size, pppal);
}


/* 等待专业型数据包执行完毕 */
ISEEIRW_API enum EXERESULT CALLAGREEMENT isirw_s_wait(LPSPEC_DATAPACK psdp, int defer)
{
  clock_t ctime = clock();
  clock_t mspf  = CLOCKS_PER_SEC / 1000;      /* 粗糙的记时方法（每毫秒周期数）*/

  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(psdp);

  /* 处理中断操作 */
  if (defer == -1) {
    psdp->s_break = 1;                /* 设置中断操作标记 */

    /* 等待操作执行完毕（正常结束、错误结束、中断结束） */
    while (psdp->exec_state != IRWE_SDPSTATE_IDLE) {
      Sleep(0);                 /* 释放剩余的时间片 */
    }

    psdp->s_break = 0;                /* 恢复中断操作标记 */

    return psdp->last_err;
  }

  /* 事件等待方式处理（未完成，暂保留此接口） */
  if (defer == -2) {
    ;
  }

  /* 在指定时间内等待数据包执行完毕 */
  while (psdp->exec_state != IRWE_SDPSTATE_IDLE) {
    if ((defer) && (((clock() - ctime) / mspf) >= defer)) {
      break;                    /* 如果超时则不再等待 */
    }
    else {
      Sleep(0);                 /* 释放剩余的时间片 */
    }
  }

  if (psdp->exec_state == IRWE_SDPSTATE_IDLE) {   /* 返回操作结果 */
    return psdp->last_err;
  }
  else {                      /* 延迟时间溢出 */
    return ER_WAITOUTTIME;
  }
}


// 获取专业型数据包中指定子图象的节点地址（LPSUBIMGBLOCK）
ISEEIRW_API LPSUBIMGBLOCK CALLAGREEMENT isirw_s_get_subimg(LPSPEC_DATAPACK psdp, int subid, int type)
{
  LPSUBIMGBLOCK psub;
  int       i;

  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(psdp && psdp->org_img);
  assert(subid < (int)psdp->org_img->imgnumbers);   /* 子图象索引值必然要小于图象个数值 */
  assert(subid > 0);                  /* 本函数不能用于获取主图象的结构地址 */

  // 确定要检索的数据包（原始包或转换包）
  psub = (type == 0) ? psdp->org_img->psubimg : psdp->cnv_img->psubimg;

  assert(psub);

  // 检索子图象索引
  for (i = 1; i < (int)psdp->org_img->imgnumbers; i++, psub = psub->next) {
    if (i == subid) {
      assert(psub->number == i);          /* 校验插件对子图象序列号的设置是否正确 */
      return psub;
    }
  }

  return 0;
}



/**********************************************************************************/
/* 简易图象读写接口 */
/**********************************************************************************/

/* 读取指定的图象文件 */
ISEEIRW_API LPFACI_DATAPACK CALLAGREEMENT isirw_f_read_img(const char* filename, IRWE_CNVMARK cnv)
{
  LPFACI_DATAPACK   pfdp = 0;           /* 简易数据包 */
  LPSPEC_DATAPACK   psdp = 0;           /* 专业数据包 */

  int         linesize, i;
  unsigned char*   p = 0;

  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(filename);
  assert(cnv != IRWE_CNVMARK_NOT);          /* 简易型数据包中的图象数据必须是ISee标准格式 */

  /* 初始化最后操作变量 */
  _f_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* 申请一个简易型数据包 */
      if ((pfdp = isirw_alloc_FACI_DATAPACK()) == 0) {
        _f_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* 申请并初始化一个专业数据包 */
      if ((psdp = isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, filename, 0, 0, cnv)) == 0) {
        _f_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* 填写命令参数 */
      psdp->command     = IRWE_SDPCOMM_LOAD;
      psdp->data_state |= SDP_DATA_STATE_PATH;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->sn          = 0;            /* 简易包批次号为0 */
      psdp->callback    = (IRWPFUN)0;

      /* 送入消息泵 */
      if (add_pump_comm(psdp) == -1) {
        _f_last_err = ISIRW_LASTERR_MEM;
        __leave;
      }

      /* 等待读取命令执行完毕 */
      while (psdp->exec_state != IRWE_SDPSTATE_IDLE) {
        Sleep(0);
      }

      /* 如果读取成功则将数据从专业包转入简易包 */
      if (psdp->last_err == ER_SUCCESS) {
        assert(psdp->org_img->p_bit_data);

        /* 如果发生了位深度转换，则取转换后的象素数据 */
        if (psdp->cnv_img) {
          linesize = DIBSCANLINE_DWORDALIGN(psdp->cnv_img->bitcount * psdp->cnv_img->width);

          pfdp->width    = psdp->cnv_img->width;
          pfdp->height   = psdp->cnv_img->height;
          pfdp->bitcount = psdp->cnv_img->bitcount;
          memcpy((void*)pfdp->palette, (const void*)psdp->cnv_img->palette, sizeof(unsigned long)*MAX_PALETTE_COUNT);

          /* 象素数据直接从转换包中摘取 */
          pfdp->pbits    = psdp->cnv_img->p_bit_data;
          psdp->cnv_img->p_bit_data = 0;

          /* 填写标准格式ID号 */
          pfdp->std_id = iscnv_is_std(pfdp->bitcount, psdp->cnv_img->r_mask, psdp->cnv_img->g_mask, psdp->cnv_img->b_mask, psdp->cnv_img->a_mask);
          assert(pfdp->std_id != -1);

          /* 如果读入的图象上下顺序与简易包的固有顺序不同，则颠倒之 */
          if (_f_img_order != psdp->cnv_img->order) {
            /* 分配颠倒用缓冲区 */
            if ((p = (unsigned char*)im_malloc(linesize)) == 0) {
              _f_last_err = ISIRW_LASTERR_MEM;
              __leave;
            }

            /* 颠倒图象上下顺序 */
            for (i = 0; i < (int)psdp->cnv_img->height / 2; i++) {
              memcpy((void*)p, (const void*)psdp->cnv_img->pp_line_addr[i], linesize);
              memcpy((void*)psdp->cnv_img->pp_line_addr[i], (void*)psdp->cnv_img->pp_line_addr[(int)psdp->cnv_img->height - i - 1], linesize);
              memcpy((void*)psdp->cnv_img->pp_line_addr[(int)psdp->cnv_img->height - i - 1], (const void*)p, linesize);
            }
          }
        }
        else {
          linesize = DIBSCANLINE_DWORDALIGN(psdp->org_img->bitcount * psdp->org_img->width);

          pfdp->width    = psdp->org_img->width;
          pfdp->height   = psdp->org_img->height;
          pfdp->bitcount = psdp->org_img->bitcount;
          memcpy((void*)pfdp->palette, (const void*)psdp->org_img->palette, sizeof(unsigned long)*MAX_PALETTE_COUNT);

          /* 象素数据直接从原始包中摘取 */
          pfdp->pbits    = psdp->org_img->p_bit_data;
          psdp->org_img->p_bit_data = 0;

          pfdp->std_id = iscnv_is_std(pfdp->bitcount, psdp->org_img->r_mask, psdp->org_img->g_mask, psdp->org_img->b_mask, psdp->org_img->a_mask);
          assert(pfdp->std_id != -1);

          if (_f_img_order != psdp->org_img->order) {
            if ((p = (unsigned char*)im_malloc(linesize)) == 0) {
              _f_last_err = ISIRW_LASTERR_MEM;
              __leave;
            }

            for (i = 0; i < (int)psdp->org_img->height / 2; i++) {
              memcpy((void*)p, (const void*)psdp->org_img->pp_line_addr[i], linesize);
              memcpy((void*)psdp->org_img->pp_line_addr[i], (void*)psdp->org_img->pp_line_addr[(int)psdp->org_img->height - i - 1], linesize);
              memcpy((void*)psdp->org_img->pp_line_addr[(int)psdp->org_img->height - i - 1], (const void*)p, linesize);
            }
          }
        }
      }
      else {
        switch (psdp->last_err) {
        case  ER_BADIMAGE:    /* 指定的文件是一个受损的图像文件 */
        case  ER_NONIMAGE:    /* 指定的文件不是一个图像文件 */
          _f_last_err = ISIRW_LASTERR_IMGERR;
          break;

        case  ER_MEMORYERR:   /* 内存不足，导致操作失败 */
          _f_last_err = ISIRW_LASTERR_MEM;
          break;

        case  ER_FILERWERR:   /* 文件在读写过程中发生错误（系统、硬件等原因引起文件读写异常） */
          _f_last_err = ISIRW_LASTERR_RW;
          break;

        case  ER_SYSERR:      /* 操作系统不稳定，无法执行命令 */
          _f_last_err = ISIRW_LASTERR_OSERR;
          break;

        default:
          assert(0);
          _f_last_err = ISIRW_LASTERR_OSERR;
          break;
        }
      }
    }
    __finally {
      if ((_f_last_err != ISIRW_LASTERR_SUCCESS) || (AbnormalTermination())) {
        if (pfdp) {
          isirw_free_FACI_DATAPACK(pfdp);
          pfdp = 0;
        }
      }

      /* 专业包在函数退出前已无用，释放之 */
      if (psdp) {
        isirw_free_SPEC_DATAPACK(psdp);
        psdp = 0;
      }

      if (p) {
        im_free(p);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _f_last_err = ISIRW_LASTERR_OSERR;
  }

  return pfdp;
}


/* 将图象保存到指定的文件当中 */
ISEEIRW_API int CALLAGREEMENT isirw_f_write_img(const char* new_filename, LPFACI_DATAPACK psou, LPSAVESTR savedec)
{
  LPSPEC_DATAPACK   psdp = 0;           /* 专业数据包 */

  int         result = 0, i, linesize;

  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(new_filename && psou);

  /* 初始化最后操作变量 */
  _f_last_err = ISIRW_LASTERR_SUCCESS;

  __try {
    __try {
      /* 申请并初始化一个专业数据包（此包为临时性的）*/
      if ((psdp = isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, new_filename, 0, 0, 0)) == 0) {
        _f_last_err = ISIRW_LASTERR_MEM;
        result = 4;               /* 内存不足 */
        __leave;
      }

      /* 填写命令参数 */
      psdp->command     = IRWE_SDPCOMM_SAVE;
      psdp->exec_state  = IRWE_SDPSTATE_IDLE;
      psdp->data_state  = SDP_DATA_STATE_PATH | SDP_DATA_STATE_ORG_INFO | SDP_DATA_STATE_ORG_PIX;
      psdp->last_err    = ER_SUCCESS;
      psdp->s_break     = 0;
      psdp->callback    = (IRWPFUN)0;

      /* 复制保存参数 */
      memcpy((void*)psdp->savestr->para_value, (const void*)savedec->para_value, sizeof(int)*ISD_ITEM_NUM);

      /* 填写目标图象路径 */
      isio_open_param(psdp->despath, ISEEIO_CONFER_LOC_FILE, new_filename, 0, 0);

      psdp->org_img->imgtype = IMT_RESSTATIC;   /* 图像文件类型 */
      psdp->org_img->imgformat = IMF_BMP;     /* 图像文件格式（后缀名） */
      psdp->org_img->compression = ICS_RGB;   /* 图像的压缩方式 */

      psdp->org_img->width = psou->width;     /* 图像宽度 */
      psdp->org_img->height = psou->height;   /* 图像高度 */
      psdp->org_img->order = _f_img_order;    /* 图像的上下顺序 （0－正向，1－倒向）*/
      psdp->org_img->bitcount = psou->bitcount; /* 每个像素所占的位数（对应于颜色数） */
      psdp->org_img->p_bit_data = psou->pbits;  /* 借用简易包中的象素数据 */

      /* 取各颜色分量的掩码值 */
      iscnv_get_mask(psou->std_id, &(psdp->org_img->r_mask), &(psdp->org_img->g_mask), &(psdp->org_img->b_mask), &(psdp->org_img->a_mask));

      linesize = DIBSCANLINE_DWORDALIGN(psdp->org_img->bitcount * psdp->org_img->width);

      /* 申请行首地址数组 */
      if ((psdp->org_img->pp_line_addr = (void**)isirw_malloc(sizeof(void*)*psou->height)) == 0) {
        _f_last_err = ISIRW_LASTERR_MEM;
        result = 4;               /* 内存不足 */
        __leave;
      }

      /* 初始化行首地址数组 */
      if (psdp->org_img->order == 0) {      /* 正向 */
        for (i = 0; i < (int)(psdp->org_img->height); i++) {
          psdp->org_img->pp_line_addr[i] = (void*)(psdp->org_img->p_bit_data + (i * linesize));
        }
      }
      else {                  /* 倒向 */
        for (i = 0; i < (int)(psdp->org_img->height); i++) {
          psdp->org_img->pp_line_addr[i] = (void*)(psdp->org_img->p_bit_data + ((psdp->org_img->height - i - 1) * linesize));
        }
      }

      /* 填写调色版尺寸 */
      switch (psou->std_id) {
      case  0:
        psdp->org_img->pal_count = 2;
        break;

      case  1:
        psdp->org_img->pal_count = 16;
        break;

      case  2:
        psdp->org_img->pal_count = 256;
        break;

      default:
        psdp->org_img->pal_count = 0;
        break;
      }

      /* 复制调色板数据 */
      memcpy((void*)(psdp->org_img->palette), (const void*)psou->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));

      psdp->org_img->psubimg = (LPSUBIMGBLOCK)0;  /* 子图像数据链地址 */
      psdp->org_img->imgnumbers = 1;        /* 该文件中图像的个数 */
      psdp->org_img->data_state = 2;        /* 数据包当前的数据状态 */

      psdp->org_img->play_number = 0UL;     /* 播放顺序表尺寸（以元素为单位） */
      psdp->org_img->play_order = 0UL;      /* 播放顺序表 */
      psdp->org_img->time = 0;          /* 首帧停留时间 */
      psdp->org_img->colorkey = -1;       /* 主图象透明色 */

      psdp->org_img->pater   = psdp;
      psdp->org_img->irwpfun = _defalt_process; /* 使用缺省回调函数（总之不能为空） */

      psdp->org_img->omnipotence = -1;      /* 作为转换包时使用的标准格式ID */

      /* 送入消息泵 */
      if (add_pump_comm(psdp) == -1) {
        _f_last_err = ISIRW_LASTERR_MEM;
        result = 4;               /* 内存不足 */
        __leave;
      }

      /* 等待保存命令执行完毕 */
      while (psdp->exec_state != IRWE_SDPSTATE_IDLE) {
        Sleep(0);
      }

      result = psdp->last_err;
    }
    __finally {
      if (psdp) {
        psdp->org_img->p_bit_data = 0;      /* 此图象数据区是简易包的，所以不应被释放 */
        isirw_free_SPEC_DATAPACK(psdp);
        psdp = 0;
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    _f_last_err = ISIRW_LASTERR_OSERR;        /* 原来没有本行代码，是网友 易秋超 发现了此BUG，并修补 */
    result = 6;
  }

  /* 0 - 成功保存 */
  /* 1 - 操作被用户中断 */
  /* 2 - 指定的文件是一个受损的图像文件 */
  /* 3 - 指定的文件不是一个图像文件 */
  /* 4 - 内存不足，导致操作失败 */
  /* 5 - 文件在读写过程中发生错误（系统、硬件等原因引起文件读写异常） */
  /* 6 - 操作系统不稳定，无法执行命令 */
  /* 7 - 不支持的命令 */
  /* 8 - 保存功能不支持的图像格式 */
  return result;
}


/* 获取简易接口最后一次操作的结果信息 */
ISEEIRW_API int CALLAGREEMENT isirw_f_get_last_err()
{
  return _f_last_err;
}




/**********************************************************************************/
/* 转换接口 */
/**********************************************************************************/

/* 转换原始包的图象到指定格式 */
ISEEIRW_API int CALLAGREEMENT isirw_cnv(LPSPEC_DATAPACK psdp, IRWE_CNVMARK newspec, int ctrl)
{
  int       i, result = 0;
  LPSUBIMGBLOCK psib;

  assert(newspec != IRWE_CNVMARK_NOT);
  assert(_isirw_mark != 0);             /* 引擎系统必须已经开启 */
  assert(psdp);

  /* 根据控制命令做预处理 */
  switch (ctrl) {
  case  -1:                     /* 删除转换包 */
    if (psdp->cnv_img) {
      assert(psdp->data_state & SDP_DATA_STATE_CNV);
      isirw_free_INFOSTR(psdp->cnv_img);      /* 释放转换包 */
      psdp->cnv_img = 0;
    }

    return 0;

  default:
    assert(0);

  case  0:                      /* 自然转换原则 */

    /* 判断专业包中是否存在转换包(cnv_img) */
    if (psdp->cnv_img) {
      assert(psdp->data_state & SDP_DATA_STATE_CNV);

      /* 凡转换包中的图象都是符合ISee标准格式的 */
      if (newspec == IRWE_CNVMARK_AUTO) {
        return 1;
      }

      /* 判断转换包与指定的格式是否相同 */
      if (psdp->cnv_img->bitcount == (unsigned long)iscnv_idtobit((int)newspec)) {
        return 1;               /* 已存在newspec格式的转换包 */
      }
      else {
        isirw_free_INFOSTR(psdp->cnv_img);    /* 不相同则释放转换包 */
        psdp->cnv_img = 0;
      }
    }
    else {
      /* 判断源图象是否符合ISee标准图象格式 */
      i = iscnv_is_std(psdp->org_img->bitcount,
          psdp->org_img->r_mask,
          psdp->org_img->g_mask,
          psdp->org_img->b_mask,
          psdp->org_img->a_mask);

      /* 如果原图象符合ISee标准格式，并且转换标签为自动，则不再进行进一步的处理 */
      if ((i != -1) && (newspec == IRWE_CNVMARK_AUTO)) {
        return 1;
      }
    }

    break;

  case  1:                      /* 强制转换原则 */
    if (psdp->cnv_img) {
      assert(psdp->data_state & SDP_DATA_STATE_CNV);
      isirw_free_INFOSTR(psdp->cnv_img);      /* 释放转换包 */
      psdp->cnv_img = 0;
    }

    break;
  }


  /* 设置新的转换标志 */
  psdp->cnvmark = newspec;
  psdp->s_break = 0;

  __try {
    /* 利用插件回调函数进行转换 */
    if ((result = _defalt_process(psdp, PFC_INTO_PLUG, 0, 0, 0, 0)) != 0) {
      __leave;
    }

    /* 转换主图象 */
    if ((result = _defalt_process(psdp, PFC_BUILD_MAST_IMAGE, 0, 0, 0, 0)) != 0) {
      __leave;
    }

    if ((result = _defalt_process(psdp, PFC_PROCESS, 0, 0, (int)psdp->org_img->height, 0)) != 0) {
      __leave;
    }

    if ((result = _defalt_process(psdp, PFC_OVER_SIR, 0, 0, 0, 0)) != 0) {
      __leave;
    }

    /* 转换子图象 */
    if (psdp->org_img->imgnumbers > 1) {
      psib = psdp->org_img->psubimg;

      for (i = 0; i < (int)(psdp->org_img->imgnumbers - 1); i++) {
        if ((result = _defalt_process(psdp, PFC_BUILD_SERT_IMAGE, i + 1, 0, 0, 0)) != 0) {
          __leave;
        }

        if ((result = _defalt_process(psdp, PFC_PROCESS, i + 1, 0, (int)psib->height, 0)) != 0) {
          __leave;
        }

        if ((result = _defalt_process(psdp, PFC_OVER_SIR, i + 1, 0, 0, 0)) != 0) {
          __leave;
        }

        psib = psib->next;
      }
    }

    /* 结束转换 */
    if ((result = _defalt_process(psdp, PFC_OK, 0, 0, 0, 0)) != 0) {
      __leave;
    }
  }
  __finally {
    switch (result) {
    case  0:
      break;

    case  2:
      result = -1;                /* 系统异常 */
      break;

    case  3:
      result = -2;                /* 内存不足 */
      break;

    default:
      result = -1;
      break;
    }

    if ((result < 0) && (psdp->cnv_img)) {    /* 如果转换失败，则释放转换包 */
      isirw_free_INFOSTR(psdp->cnv_img);
      psdp->cnv_img = 0;
      psdp->data_state &= ~SDP_DATA_STATE_CNV;
      psdp->cnvmark = IRWE_CNVMARK_AUTO;
    }
    else {
      psdp->data_state |= SDP_DATA_STATE_CNV;
    }
  }

  return result;
}





/**********************************************************************************/
/* 内存接口 */
/**********************************************************************************/

/* 内存分配接口 */
ISEEIRW_API void* CALLAGREEMENT isirw_malloc(size_t size)
{
  assert(size > 0);

  return im_malloc(size);
}


/* 内存重分配接口 */
ISEEIRW_API void* CALLAGREEMENT isirw_realloc(void* p, size_t size)
{
  /* 如果是内用象素缓冲区，则进行判断 */
  if (p == _isirw_porg) {
    assert(_isirw_org_used);

    if (size < IRWE_MAX_BUFFSIZE) {       /* 如果新尺寸未超出缓冲区尺寸，则仍旧使用缓冲区内存 */
      return (void*)_isirw_porg;
    }
    else {
      p = 0;
      InterlockedDecrement(&_isirw_org_used); /* 否则分配新内存（见下）*/
    }
  }
  else if (p == _isirw_pcnv) {
    assert(_isirw_cnv_used);

    if (size < IRWE_MAX_BUFFSIZE) {
      return (void*)_isirw_pcnv;
    }
    else {
      p = 0;
      InterlockedDecrement(&_isirw_cnv_used);
    }
  }

  return im_realloc(p, size);
}


/* 内存释放接口 */
ISEEIRW_API void CALLAGREEMENT isirw_free(void* memblock)
{
  assert(memblock != 0);

  /* 如果是内用象素缓冲区，则不进行释放操作 */
  if (memblock == _isirw_porg) {
    assert(_isirw_org_used);
    InterlockedDecrement(&_isirw_org_used);
  }
  else if (memblock == _isirw_pcnv) {
    assert(_isirw_cnv_used);
    InterlockedDecrement(&_isirw_cnv_used);
  }
  else {
    im_free(memblock);                /* 通用内存则直接释放 */
  }
}



/*
  功能：分配一个新的数据包

  入口：pater － 所属的专业数据包

  返回：  0   － 函数执行失败，内存不足导致的
    非0   － 函数执行成功，返回值是新数据包的首地址
          （注：由本函数申请的数据包必须由isirw_free_INFOSTR()释放
*/
ISEEIRW_API LPINFOSTR CALLAGREEMENT isirw_alloc_INFOSTR(LPSPEC_DATAPACK pater)
{
  LPINFOSTR ptmp;

  /* 分配结构空间 */
  if ((ptmp = (LPINFOSTR)isirw_malloc(sizeof(INFOSTR))) == (LPINFOSTR)0) {
    return (LPINFOSTR)0;
  }

#ifdef _DEBUG
  ptmp->sct_mark = INFOSTR_DBG_MARK;  /* 结构的标识，用于调试 */

  if (sct_count == 0) {
    InitializeCriticalSection(&(sct_access));
  }

  EnterCriticalSection(&(sct_access));
  sct_count++;            /* 结构实体个数 */
  LeaveCriticalSection(&(sct_access));
#endif

  ptmp->imgtype     = IMT_NULL;   /* 图像文件类型 */
  ptmp->imgformat   = IMF_NULL;   /* 图像文件格式（后缀名） */
  ptmp->compression = ICS_RGB;    /* 图像的压缩方式 */

  ptmp->width    = 0;         /* 图像宽度 */
  ptmp->height   = 0;         /* 图像高度 */
  ptmp->order    = 0;         /* 图像的上下顺序 （0－正向，1－倒向）*/
  ptmp->bitcount = 0;         /* 每个像素所占的位数（对应于颜色数） */

  ptmp->r_mask = 0;         /* 各颜色分量的掩码值 */
  ptmp->g_mask = 0;
  ptmp->b_mask = 0;
  ptmp->a_mask = 0;

  ptmp->pp_line_addr = (void**)0;
  ptmp->p_bit_data   = (unsigned char*)0;
  ptmp->pal_count    = 0;
  memset((void*)(ptmp->palette), 0, MAX_PALETTE_COUNT * sizeof(unsigned long));

  ptmp->psubimg = (LPSUBIMGBLOCK)0; /* 子图像数据链地址 */
  ptmp->imgnumbers = 0;       /* 该文件中图像的个数 */

  ptmp->data_state = 0;       /* 数据包当前的数据状态：
                      0 － 空的数据包，没有任何变量数据是有效的
                      1 － 存在图像的描述信息
                      2 － 存在图像数据 */
  InitializeCriticalSection(&(ptmp->access));

  ptmp->play_number = 0UL;      /* 播放顺序表尺寸（以元素为单位） */
  ptmp->play_order = 0UL;       /* 播放顺序表 */
  ptmp->time = 0;           /* 首帧停留时间 */
  ptmp->colorkey = -1;        /* 主图象透明色 */

  ptmp->pater   = pater;
  ptmp->irwpfun = _defalt_process;  /* 使用缺省回调函数（总之不能为空） */

  ptmp->omnipotence = -1;       /* 作为转换包时使用的标准格式ID */

  return ptmp;
}



/*
  功能：释放一个数据包

  入口：
    pinfo － 数据包首地址

  返回：  0   － 恒定返回值（可用于数据包指针的置零操作）
*/
ISEEIRW_API LPINFOSTR CALLAGREEMENT isirw_free_INFOSTR(LPINFOSTR pinfo)
{
  assert(pinfo);
  assert(pinfo->sct_mark == INFOSTR_DBG_MARK);

  assert(sct_count);

  EnterCriticalSection(&(pinfo->access));

#ifdef _DEBUG
  EnterCriticalSection(&(sct_access));
  sct_count--;            /* 结构实体个数 */
  LeaveCriticalSection(&(sct_access));

  if (sct_count == 0) {
    DeleteCriticalSection(&(sct_access));
  }

#endif

  if (pinfo->pp_line_addr) {
    isirw_free(pinfo->pp_line_addr);
    pinfo->pp_line_addr = (void**)0;
  }

  if (pinfo->p_bit_data) {
    isirw_free(pinfo->p_bit_data);
    pinfo->p_bit_data = (unsigned char*)0;
  }

  if (pinfo->psubimg) {
    assert(pinfo->imgnumbers > 1);
    _free_subimg(pinfo);
  }

  if (pinfo->play_order) {
    isirw_free(pinfo->play_order);
    pinfo->play_order = (unsigned long*)0;
  }

  LeaveCriticalSection(&(pinfo->access));
  DeleteCriticalSection(&(pinfo->access));

  isirw_free(pinfo);

  return (LPINFOSTR)0;
}



/* 内部函数－ 释放数据包中的子图像链表（如果有的话） */
void CALLAGREEMENT _free_subimg(LPINFOSTR pinfo)
{
  int i;
  LPSUBIMGBLOCK ptmp, pfree = pinfo->psubimg;

  /* 释放所有子图象节点 */
  for (i = 0; i < (int)(pinfo->imgnumbers - 1); i++) {
    ptmp = pfree->next;

    assert(pfree);

    isirw_free_SUBIMGBLOCK(pfree);

    pfree = ptmp;
  }

  pinfo->imgnumbers -= i;
  assert(pinfo->imgnumbers == 1);   /* 因为只剩下主帧图象了 */
  pinfo->psubimg = (LPSUBIMGBLOCK)0;
}



/*
  功能：分配一个子图像节点，并初始化节点内数据

  入口：

  返回：  0   － 函数执行失败，内存不足导致的
    非0   － 函数执行成功，返回值是子图象节点的首地址
          （注：由本函数申请的子图象节点必须由isirw_free_SUBIMGBLOCK()释放
*/
ISEEIRW_API LPSUBIMGBLOCK CALLAGREEMENT isirw_alloc_SUBIMGBLOCK()
{
  LPSUBIMGBLOCK p_subimg;

  /* 申请子图像信息节点(SUBIMGBLOCK) */
  if ((p_subimg = (LPSUBIMGBLOCK)isirw_malloc(sizeof(SUBIMGBLOCK))) == 0) {
    return 0;  /* 内存不足 */
  }

  /* 初始化为 0 */
  memset((void*)p_subimg, 0, sizeof(SUBIMGBLOCK));

  p_subimg->number   = 1;     /* 子图象起始序号为 1 */
  p_subimg->colorkey = -1;    /* 透明色值设为无 */

  return p_subimg;
}



/*
  功能：释放一个子图像节点，包括其中的位缓冲区及行首地址数组

  入口：p_node  － 待释放的子图象节点

  返回：
*/
ISEEIRW_API void CALLAGREEMENT isirw_free_SUBIMGBLOCK(LPSUBIMGBLOCK p_node)
{
  assert(p_node);
  assert(p_node->number > 0);

  if (p_node->pp_line_addr) {
    isirw_free(p_node->pp_line_addr);
  }

  if (p_node->p_bit_data) {
    isirw_free(p_node->p_bit_data);
  }

  isirw_free(p_node);
}



/*
  功能：分配一个空的专业数据包，并初始化其中的个成员变量

  入口：

  返回：  0   － 函数执行失败，内存不足导致的
    非0   － 函数执行成功
*/
ISEEIRW_API LPSPEC_DATAPACK CALLAGREEMENT isirw_alloc_SPEC_DATAPACK(
    ISEEIO_CONFER_TYPE type,
    const char* name,
    unsigned long param1,
    unsigned long param2,
    IRWE_CNVMARK cm)
{
  LPSPEC_DATAPACK p_sdp;

  /* 申请专业数据包本体 */
  if ((p_sdp = (LPSPEC_DATAPACK)isirw_malloc(sizeof(SPEC_DATAPACK))) == 0) {
    return 0;         /* 内存不足 */
  }

  /* 初始化为 0 */
  memset((void*)p_sdp, 0, sizeof(SPEC_DATAPACK));

  p_sdp->dbg_mark    = DATAPACK_MARK_SPECIALTY;
  p_sdp->exec_state  = IRWE_SDPSTATE_IDLE;
  p_sdp->data_state  = 0;
  p_sdp->last_err    = ER_SUCCESS;
  p_sdp->s_total     = 0;
  p_sdp->s_current   = 0;
  p_sdp->s_break     = 0;
  p_sdp->s_state     = -1;
  p_sdp->cnvmark     = cm;
  p_sdp->command     = IRWE_SDPCOMM_NULL;

  /* 申请数据包 */
  if ((p_sdp->org_img = (LPINFOSTR)isirw_alloc_INFOSTR(p_sdp)) == 0) {
    isirw_free(p_sdp);
    return 0;
  }

  /* 申请源图象路径包 */
  if ((p_sdp->path = (PISADDR)isirw_malloc(sizeof(ISADDR))) == 0) {
    isirw_free_INFOSTR(p_sdp->org_img);
    isirw_free(p_sdp);
    return 0;
  }

  /* 设置初始路径 */
  isio_open_param(p_sdp->path, type, name, param1, param2);

  /* 申请目标图象路径包 */
  if ((p_sdp->despath = (PISADDR)isirw_malloc(sizeof(ISADDR))) == 0) {
    isirw_free(p_sdp->path);
    isirw_free_INFOSTR(p_sdp->org_img);
    isirw_free(p_sdp);
    return 0;
  }

  /* 申请保存参数描述结构 */
  if ((p_sdp->savestr = (LPSAVESTR)isirw_malloc(sizeof(SAVESTR))) == 0) {
    isirw_free(p_sdp->despath);
    isirw_free(p_sdp->path);
    isirw_free_INFOSTR(p_sdp->org_img);
    isirw_free(p_sdp);
    return 0;
  }

  return p_sdp;
}



/*
  功能：释放一个专业数据包（包含其中包含的原始数据包、转换数据包、路径包）

  入口：p_sdp － 待释放的专业数据包

  返回：
*/
ISEEIRW_API void CALLAGREEMENT isirw_free_SPEC_DATAPACK(LPSPEC_DATAPACK p_sdp)
{
  assert(p_sdp);
  assert(p_sdp->exec_state == IRWE_SDPSTATE_IDLE);

  if (p_sdp->org_img) {
    isirw_free_INFOSTR(p_sdp->org_img);
  }

  if (p_sdp->cnv_img) {
    isirw_free_INFOSTR(p_sdp->cnv_img);
  }

  if (p_sdp->savestr) {
    isirw_free(p_sdp->savestr);
  }

  if (p_sdp->despath) {
    isirw_free(p_sdp->despath);
  }

  if (p_sdp->path) {
    isirw_free(p_sdp->path);
  }

  isirw_free(p_sdp);
}


/*
  功能：申请一个简易型数据包

  入口：－
  返回：  0   － 函数执行失败，内存不足导致的
    非0   － 函数执行成功（返回一个空的简易型数据包）
*/
ISEEIRW_API LPFACI_DATAPACK CALLAGREEMENT isirw_alloc_FACI_DATAPACK()
{
  LPFACI_DATAPACK   pfdp;

  /* 申请专业数据包本体 */
  if ((pfdp = (LPFACI_DATAPACK)isirw_malloc(sizeof(FACI_DATAPACK))) == 0) {
    return 0;         /* 内存不足 */
  }

  /* 初始化为 0 */
  memset((void*)pfdp, 0, sizeof(FACI_DATAPACK));

  pfdp->dbg_mark = DATAPACK_MARK_FACILITY;
  pfdp->std_id   = -1;
  pfdp->pbits    = 0;

  return pfdp;
}


/*
  功能：释放一个简易型数据包（包含其中包含的原始图象象素数据）

  入口：pfdp  － 待释放的简易型数据包

  返回：
*/
ISEEIRW_API void CALLAGREEMENT isirw_free_FACI_DATAPACK(LPFACI_DATAPACK pfdp)
{
  assert(pfdp);
  assert(pfdp->dbg_mark == DATAPACK_MARK_FACILITY);

  /* 如果有图象象素数据，则释放它 */
  if (pfdp->pbits) {
    isirw_free(pfdp->pbits);
  }

  isirw_free(pfdp);
}





/**********************************************************************************/
/*内用接口函数 */
/**********************************************************************************/

#ifdef WIN32

/*
  功能：测试指定DLL是否是ISee图像读写插件

  入口：
    hmode － DLL句柄

  返回：  0   － 指定的DLL不是ISee的图像读写插件
    非0   － 是

*/
ISEEIRW_API unsigned long CALLAGREEMENT isirw_test_plugin(HMODULE hmod)
{
  return (unsigned long)GetProcAddress(hmod, IRWP_TAG);
}



/*
  功能：获得ISee图像读写插件的内部信息

  入口：
    pfun_add－ 由上面的isirw_test_plugin()函数返回的值

  返回：  0   － 函数执行失败，表明该插件已受损，无法利用
    非0   － 函数执行成功，返回值是指向IRWP_INFO结构的地址，
           该结构的实体在插件内部。
*/
ISEEIRW_API LPIRWP_INFO CALLAGREEMENT isirw_get_info(unsigned long pfun_add)
{
  return (pfun_add == 0UL) ? (LPIRWP_INFO)0 : (*((IRWP_ISX)pfun_add))();
}



/*
  功能：获得ISee图像读写插件接口地址

  入口：
    hmod  － 插件模块句柄
    pinfo － 由isirw_get_info()函数返回的插件信息结构地址
    name  － 要获取的接口名称（见IRWP_API_NAME的定义）

  返回：  0   － 函数执行失败，表明该插件已受损，无法利用
    非0   － 函数执行成功，返回值是指定接口的函数地址
*/
ISEEIRW_API void* CALLAGREEMENT isirw_get_api_addr(HMODULE hmod, LPIRWP_INFO pinfo, enum IRWP_API_NAME name)
{
  char buf[64];

  assert(hmod);
  assert(pinfo);

  strcpy((char*)buf, (const char*)(pinfo->irwp_func_prefix));
  strcat((char*)buf, (const char*)IRWP_API_NAME_STRING[name]);

  return (void*)GetProcAddress(hmod, (char*)buf);
}

#endif /* WIN32 */





/*
  功能：为指定数据包设置新的回调函数

  入口：pinfo －待设定数据包
    new_pfun－新的回调函数

  返回：老的回调函数
*/
ISEEIRW_API IRWPFUN CALLAGREEMENT isirw_set_pfun(LPINFOSTR pinfo, IRWPFUN new_pfun)
{
  IRWPFUN old;

  assert(pinfo);

  if (new_pfun == 0) {
    old = _defalt_process;
    new_pfun = _defalt_process;   /* 回调函数不能为空 */
  }
  else {
    old = pinfo->irwpfun;
  }

  pinfo->irwpfun = new_pfun;

  return old;
}


/*
  功能：分配原始象素内存块。如果原始象素缓冲区可用，则返回原始象素缓冲区首地址。否则
    分配一个新的内存块。

  入口：size  －申请的内存块尺寸（字节单位）

  返回：缓冲区首地址，或新内存块首地址
*/
ISEEIRW_API void* CALLAGREEMENT isirw_orgpix_malloc(size_t size)
{
  /* 如果调用者申请的内存过大，或原始象素缓冲区并没有被分配，或者缓冲区正在被使用，则申请新的内存块 */
  if ((size > IRWE_MAX_BUFFSIZE) || (!(_isirw_buff_mark & IRWE_BUFTYPE_ORG)) || (_isirw_org_used)) {
    return im_malloc(size);
  }

  /* 累加使用计数 */
  InterlockedIncrement(&_isirw_org_used);

  assert(_isirw_porg);

  /* 返回原始象素缓冲区首地址 */
  return (void*)_isirw_porg;
}


/*
  功能：分配转换象素内存块。如果转换象素缓冲区可用，则返回转换象素缓冲区首地址。否则
    分配一个新的内存块。

  入口：size  －申请的内存块尺寸（字节单位）

  返回：缓冲区首地址，或新内存块首地址
*/
ISEEIRW_API void* CALLAGREEMENT isirw_cnvpix_malloc(size_t size)
{
  /* 如果调用者申请的内存过大，或转换象素缓冲区并没有被分配，或者缓冲区正在被使用，则申请新的内存块 */
  if ((size > IRWE_MAX_BUFFSIZE) || (!(_isirw_buff_mark & IRWE_BUFTYPE_CNV)) || (_isirw_cnv_used)) {
    return im_malloc(size);
  }

  /* 累加使用计数 */
  InterlockedIncrement(&_isirw_cnv_used);

  assert(_isirw_pcnv);

  /* 返回原始象素缓冲区首地址 */
  return (void*)_isirw_pcnv;
}


/* 向引擎加入空闲处理命令 */
ISEEIRW_API int CALLAGREEMENT isirw_add_idle(void* p/*空闲处理函数地址*/)
{
  return add_idlesse(p);                /* 队列满，返回-1，成功返回0 */
}


/* 获取待处理的空闲命令个数 */
ISEEIRW_API int CALLAGREEMENT isirw_get_idlesse_count()
{
  return get_idlesse_count();
}


/*
  功能：缺省的进度回调函数

  入口：sdp     －专业数据包地址
    type    －命令类型（见PFUN_COMM的定义注释）
    img_num   －图象序号（0为主图象，其他为子图象序号）
    star_line －起始行号（以缓冲区基地址为基准）
    line_num  －连续行数
    funid   －调用者标识（0－读函数、1－写函数）

  返回：0－继续读写图象
    1－用户中断
    2－系统异常
    3－内存不足
*/
static int CALLAGREEMENT _defalt_process(
    LPSPEC_DATAPACK sdp,
    PFUN_COMM type,
    int img_num,
    int star_line,
    int line_num,
    int funid)
{
  int       result = 0;
  int       i, j, sls, dls, fmt;
  LPINFOSTR   pinfo_str;
  LPSUBIMGBLOCK psub_str;
  LPSUBIMGBLOCK psub_tmp;
  LPSUBIMGBLOCK psub_a;
  LPSUBIMGBLOCK psub_b;

  assert(sdp);

  pinfo_str = sdp->org_img;
  psub_str  = sdp->org_img->psubimg;

  switch (type) {
  case  PFC_NULL:           /* 空命令 */
    break;

  case  PFC_INTO_PLUG:          /* 控制进入插件 */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      sdp->s_total   = 1;
      sdp->s_current = 0;

      if (sdp->s_break != 0) {
        result = 1;
        break;
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      sdp->s_total   = 1;
      sdp->s_current = 0;

      if (sdp->s_break != 0) {
        result = 1;
        break;
      }
    }
    else {
      assert(0);            /* 当前只有以上两种调用者函数类型 */
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_BUILD_MAST_IMAGE:     /* 新的主图象 */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      switch (sdp->cnvmark) {
      case  IRWE_CNVMARK_NOT:
        fmt = -1;
        break;

      case  IRWE_CNVMARK_AUTO:
        /* 判断源图象是否是ISee标准格式之一 */
        i = iscnv_is_std(sdp->org_img->bitcount, sdp->org_img->r_mask, sdp->org_img->g_mask, sdp->org_img->b_mask, sdp->org_img->a_mask);

        if (i == -1) {        /* 非标准格式 */
          /* 求得最匹配的标准格式ID号 */
          fmt = iscnv_match(sdp->org_img->bitcount);
        }
        else {            /* 标准格式 */
          fmt = -1;
        }

        break;

      case  IRWE_CNVMARK_1:
      case  IRWE_CNVMARK_4:
      case  IRWE_CNVMARK_8:
      case  IRWE_CNVMARK_16_555:
      case  IRWE_CNVMARK_16_565:
      case  IRWE_CNVMARK_24:
      case  IRWE_CNVMARK_32:
        fmt = sdp->cnvmark;
        break;

      default:
        assert(0);            /* cnvmark的值不可能超出以上范围 */
        return 2;
        break;
      }

      /* 接受用户中断请求 */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }

      /* 如果源图不是标准格式，则申请转换包 */
      if (fmt >= 0) {
        if ((pinfo_str = _isirw_build_mi(sdp, fmt)) == 0) {
          result = 3;
          break;      /* 内存不足 */
        }

        assert(sdp->cnv_img == 0);

        sdp->cnv_img = pinfo_str;
      }

      /* 设置进度及包状态变量 */
      sdp->s_total   = sdp->org_img->height;
      sdp->s_current = 0;
      sdp->s_state   = 0;
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      sdp->s_total   = sdp->org_img->height;
      sdp->s_current = 0;
      sdp->s_state   = 0;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_BUILD_SERT_IMAGE:     /* 新的子图象 */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      /* 接受用户中断请求 */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }

      if (sdp->cnv_img) {
        assert(sdp->cnvmark != IRWE_CNVMARK_NOT);

        /* 获取主图象标准格式ID */
        fmt = iscnv_is_std(sdp->cnv_img->bitcount, sdp->cnv_img->r_mask, sdp->cnv_img->g_mask, sdp->cnv_img->b_mask, sdp->cnv_img->a_mask);

        assert(fmt != -1);

        /* 获取本次原始子图象的数据包地址 */
        _isirw_get_sert_image_info(sdp->org_img, img_num, (LPSUBIMGBLOCK*)&psub_str);

        /* 申请新的子图象转换包 */
        if ((psub_tmp = _isirw_build_si(psub_str, fmt)) == 0) {
          result = 3;
          break;
        }

        psub_tmp->parents = sdp->cnv_img;

        /* 获取转换包最后一副子图象的序列号及数据包地址 */
        j = _isirw_get_last_sert_image_info(sdp->cnv_img, (LPSUBIMGBLOCK*)&psub_str);

        /* 链接至转换包子图象链的尾部 */
        if (j != 0) {
          psub_tmp->prev = psub_str;
          psub_str->next = psub_tmp;
        }
        else {
          psub_tmp->prev = 0;
          assert(sdp->cnv_img->psubimg == 0);
          sdp->cnv_img->psubimg = psub_tmp;
        }

        psub_tmp->next    = 0;

        psub_tmp->parents->imgnumbers++;

        /* 累加进度 */
        sdp->s_total     += psub_tmp->height;
        sdp->s_state      = psub_tmp->number;
      }
      else {
        if (sdp->cnvmark == IRWE_CNVMARK_NOT) {
          if (sdp->callback) {
            (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
          }

          break;
        }

        assert(sdp->cnvmark == IRWE_CNVMARK_AUTO);
        assert(sdp->org_img);

        /* 获得本次子图象数据包地址 */
        _isirw_get_sert_image_info(sdp->org_img, img_num, (LPSUBIMGBLOCK*)&psub_str);

        assert(psub_str);

        /* 如果和主图象的格式相同，在无需转换，继续下一副子图象 */
        if ((sdp->org_img->bitcount == (unsigned long)psub_str->bitcount) && \
            (sdp->org_img->r_mask == psub_str->r_mask) && \
            (sdp->org_img->g_mask == psub_str->g_mask) && \
            (sdp->org_img->b_mask == psub_str->b_mask) && \
            (sdp->org_img->a_mask == psub_str->a_mask)) {
          if (sdp->callback) {
            (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
          }

          break;
        }

        /* 如果子图象是标准格式之一，则继续下一副子图象 */
        if (iscnv_is_std(psub_str->bitcount, psub_str->r_mask, psub_str->g_mask, psub_str->b_mask, psub_str->a_mask) != -1) {
          if (sdp->callback) {
            (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
          }

          break;
        }

        /* 取得原始主图象位深值 */
        j = (int)sdp->org_img->bitcount;

        /* 指向第一幅子图象数据包 */
        psub_str = sdp->org_img->psubimg;

        /* 选出最大位深值 */
        while (psub_str) {
          if (psub_str->bitcount > j) {
            j = psub_str->bitcount;
          }

          psub_str = psub_str->next;
        }

        assert((j > 0) && (j <= 32));

        /* 求得最匹配的标准格式ID号 */
        fmt = iscnv_match(j);

        /* 先建立并进行主图象转换 */
        if ((pinfo_str = _isirw_build_mi(sdp, fmt)) == 0) {
          result = 3;
          break;      /* 内存不足 */
        }

        sdp->cnv_img = pinfo_str;

        /* 计算源、目标图象扫描行宽度 */
        sls = DIBSCANLINE_DWORDALIGN(sdp->org_img->bitcount * sdp->org_img->width);
        dls = DIBSCANLINE_DWORDALIGN(sdp->cnv_img->bitcount * sdp->cnv_img->width);

        /* 一次性转换所有的主图象行 */
        result = iscnv_cnvfun(sdp->org_img->bitcount, sdp->cnv_img->omnipotence,
            sdp->org_img->p_bit_data, (LPPALITEM)sdp->org_img->palette, sls,
            sdp->cnv_img->p_bit_data, (LPPALITEM)sdp->cnv_img->palette, dls,
            sdp->org_img->height, sdp->org_img->width, sdp->org_img->b_mask, sdp->org_img->g_mask,
            sdp->org_img->r_mask, sdp->org_img->a_mask);

        if (result == -1) {
          result = 2;
          break;
        }

        psub_str = sdp->org_img->psubimg;

        assert(psub_str);

        /* 转换子图象 */
        while (psub_str) {
          /* 申请新的子图象转换包 */
          if ((psub_tmp = _isirw_build_si(psub_str, fmt)) == 0) {
            result = 3;
            break;
          }

          psub_tmp->parents = sdp->cnv_img;

          /* 获取转换包最后一副子图象的序列号及数据包地址 */
          j = _isirw_get_last_sert_image_info(sdp->cnv_img, (LPSUBIMGBLOCK*)&psub_a);

          /* 链入转换包子图象链的尾部 */
          if (j != 0) {
            psub_tmp->prev = psub_a;
            psub_a->next   = psub_tmp;
          }
          else {
            psub_tmp->prev = 0;
            assert(sdp->cnv_img->psubimg == 0);
            sdp->cnv_img->psubimg = psub_tmp;
          }

          psub_tmp->next    = 0;

          /* 转换包的图象计数加一 */
          psub_tmp->parents->imgnumbers++;

          /* 设定进度及图象号 */
          sdp->s_total     += psub_tmp->height;
          sdp->s_state      = psub_tmp->number;

          /* 本次调用指定的子图象以前的子图象都将进行一次性转换 */
          if (psub_str->number < img_num) {
            /* 计算源图象和目标图象扫描行尺寸 */
            sls = DIBSCANLINE_DWORDALIGN(psub_str->bitcount * psub_str->width);
            dls = DIBSCANLINE_DWORDALIGN(psub_tmp->bitcount * psub_tmp->width);

            /* 一次性转换子图象 */
            result = iscnv_cnvfun(psub_str->bitcount, sdp->cnv_img->omnipotence,
                psub_str->p_bit_data, (LPPALITEM)psub_str->palette, sls,
                psub_tmp->p_bit_data, (LPPALITEM)psub_tmp->palette, dls,
                psub_str->height, psub_str->width, psub_str->b_mask, psub_str->g_mask,
                psub_str->r_mask, psub_str->a_mask);

            if (result == -1) {
              result = 2;
              break;
            }

            /* 累加进度 */
            sdp->s_current += psub_tmp->height;
          }
          else {
            break;
          }

          /* 进行下一副子图象 */
          psub_str = psub_str->next;
        }
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      /* 获取原始包本次子图象的数据包地址 */
      _isirw_get_sert_image_info(sdp->org_img, img_num, (LPSUBIMGBLOCK*)&psub_a);

      /* 累加进度 */
      sdp->s_total   += psub_a->height;
      sdp->s_state   = 0;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_PROCESS:          /* 步进 */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      if (sdp->cnv_img) {
        if (sdp->s_state == 0) {  /* 主图象 */
          sls = DIBSCANLINE_DWORDALIGN(sdp->org_img->bitcount * sdp->org_img->width);
          dls = DIBSCANLINE_DWORDALIGN(sdp->cnv_img->bitcount * sdp->cnv_img->width);

          result = iscnv_cnvfun(sdp->org_img->bitcount, sdp->cnv_img->omnipotence,
              sdp->org_img->p_bit_data + star_line * sls, (LPPALITEM)sdp->org_img->palette, sls,
              sdp->cnv_img->p_bit_data + star_line * dls, (LPPALITEM)sdp->cnv_img->palette, dls,
              line_num, sdp->org_img->width, sdp->org_img->b_mask, sdp->org_img->g_mask,
              sdp->org_img->r_mask, sdp->org_img->a_mask);

          result = (result == -1) ? 2 : 0;
        }
        else {          /* 子图象 */
          /* 获取转换包最后一副子图象的数据包地址及原始包本次子图象数据包地址 */
          _isirw_get_sert_image_info(sdp->org_img, img_num, (LPSUBIMGBLOCK*)&psub_a);
          _isirw_get_last_sert_image_info(sdp->cnv_img, (LPSUBIMGBLOCK*)&psub_b);

          assert(psub_a && psub_b);
          assert(psub_a->number == psub_b->number);

          sls = DIBSCANLINE_DWORDALIGN(psub_a->bitcount * psub_a->width);
          dls = DIBSCANLINE_DWORDALIGN(psub_b->bitcount * psub_b->width);

          result = iscnv_cnvfun(psub_a->bitcount, sdp->cnv_img->omnipotence,
              psub_a->p_bit_data + star_line * sls, (LPPALITEM)psub_a->palette, sls,
              psub_b->p_bit_data + star_line * dls, (LPPALITEM)psub_b->palette, dls,
              line_num, psub_a->width, psub_a->b_mask, psub_a->g_mask,
              psub_a->r_mask, psub_a->a_mask);

          result = (result == -1) ? 2 : 0;
        }
      }

      if (((star_line % 10) == 1) && (sdp->callback)) {
        (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
      }

      /* 累加进度 */
      sdp->s_current += line_num;

      /* 接受用户中断请求 */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      if (((star_line % 10) == 1) && (sdp->callback)) {
        (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
      }

      /* 累加进度 */
      sdp->s_current += line_num;

      /* 接受用户中断请求 */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }
    }
    else {
      assert(0);            /* 当前只有以上两种调用者函数类型 */
    }

    break;

  case  PFC_OVER_SIR:         /* 结束一副图象 */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      /* 接受用户中断请求 */
      if (sdp->s_break != 0) {
        result = 1;
        break;
      }

      if (sdp->cnv_img) {
        if (sdp->s_state == 0) {      /* 主图象 */
          sdp->cnv_img->time     = sdp->org_img->time;
          sdp->cnv_img->colorkey = sdp->org_img->colorkey;
          sdp->cnv_img->data_state = 2;
        }
        else {              /* 子图象 */
          ;
        }
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      ;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_OK:             /* 控制即将离开插件－执行成功的状态下 */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      if (sdp->cnv_img) {
        sdp->cnv_img->play_number = sdp->org_img->play_number;

        /* 复制主图象播放表 */
        if (sdp->cnv_img->play_number) {
          sdp->cnv_img->play_order  = (unsigned long*)isirw_malloc(sdp->cnv_img->play_number * sizeof(long));

          if (sdp->cnv_img->play_order == 0) {
            result = 3;
            break;
          }

          memcpy((void*)sdp->cnv_img->play_order, (const void*)sdp->org_img->play_order, (size_t)sdp->cnv_img->play_number * sizeof(long));
        }

        sdp->cnv_img->imgnumbers  = sdp->org_img->imgnumbers;
      }

      /* 当前进度等于总进度 */
      sdp->s_current = sdp->s_total;
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      sdp->s_current = sdp->s_total;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  case  PFC_BREAK:            /* 控制即将离开插件－执行失败的状态下 */
    if (funid == IRWE_CALLBACK_FUNID_LOAD) {
      if (sdp->cnv_img) {
        isirw_free_INFOSTR(sdp->cnv_img);
        sdp->cnv_img = 0;
      }
    }
    else if (funid == IRWE_CALLBACK_FUNID_SAVE) {
      ;
    }
    else {
      assert(0);
    }

    if (sdp->callback) {
      (*sdp->callback)(sdp, type, img_num, star_line, line_num, funid);
    }

    break;

  default:
    assert(0);
    result = 2;
    break;   /* 系统错误，中断插件的读写操作 */
  }

  return result;
  /* 0－继续读写，1－用户中断，2－系统异常，3－内存不足 */
}


/* 创建一个新的转换用主图象数据包 */
LPINFOSTR CALLAGREEMENT _isirw_build_mi(LPSPEC_DATAPACK sdp, int fmt/* 目标格式ID */)
{
  LPINFOSTR pinfo_str;
  int     linesize, i, j;

  if ((pinfo_str = isirw_alloc_INFOSTR(sdp)) == 0) {
    return 0;                   /* 内存不足 */
  }

  pinfo_str->imgtype    = IMT_CNV;
  pinfo_str->imgformat  = sdp->org_img->imgformat;
  pinfo_str->compression  = sdp->org_img->compression;

  pinfo_str->width    = sdp->org_img->width;
  pinfo_str->height   = sdp->org_img->height;
  pinfo_str->bitcount   = iscnv_idtobit(fmt);
  pinfo_str->order    = sdp->org_img->order;

  /* 提出标准格式的掩码数据 */
  iscnv_get_mask(fmt, &(pinfo_str->r_mask), &(pinfo_str->g_mask), &(pinfo_str->b_mask), &(pinfo_str->a_mask));

  pinfo_str->psubimg    = 0;
  pinfo_str->imgnumbers = 1;

  pinfo_str->omnipotence  = fmt;

  /* 复制调色板数据 */
  if (pinfo_str->bitcount <= 8) {
    switch (pinfo_str->bitcount) {
    case  1:
      if (sdp->org_img->bitcount <= 8) {
        ((unsigned long*)(pinfo_str->palette))[0] = ((unsigned long*)sdp->org_img->palette)[0];
        ((unsigned long*)(pinfo_str->palette))[1] = ((unsigned long*)sdp->org_img->palette)[(unsigned int)pow((int)2, (int)sdp->org_img->bitcount) / 2];
      }
      else {
        memcpy((void*)pinfo_str->palette, (const void*)iscnv_get_pal(1), 2 * sizeof(unsigned long));
      }

      break;

    case  4:
      if (sdp->org_img->bitcount <= 4) {  /* 1~4 bit */
        memcpy((void*)pinfo_str->palette, (const void*)sdp->org_img->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));
      }
      else if (sdp->org_img->bitcount <= 8) { /* 5~8 bit */
        j = (unsigned int)pow((int)2, (int)sdp->org_img->bitcount) / 16;

        for (i = 0; i < 16; i++) {
          ((unsigned long*)(pinfo_str->palette))[i] = ((unsigned long*)sdp->org_img->palette)[i * j];
        }
      }
      else {
        memcpy((void*)pinfo_str->palette, (const void*)iscnv_get_pal(4), 16 * sizeof(unsigned long));
      }

      break;

    case  8:
      if (sdp->org_img->bitcount <= 8) {
        memcpy((void*)pinfo_str->palette, (const void*)sdp->org_img->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));
      }
      else {
        memcpy((void*)pinfo_str->palette, (const void*)iscnv_get_pal(8), 256 * sizeof(unsigned long));
      }

      break;

    default:
      assert(0);
      isirw_free_INFOSTR(pinfo_str);
      return 0;
    }

    pinfo_str->pal_count = (unsigned int)pow((int)2, (int)pinfo_str->bitcount);
  }
  else {
    pinfo_str->pal_count = 0;
  }

  linesize = DIBSCANLINE_DWORDALIGN(pinfo_str->bitcount * pinfo_str->width);

  /* 申请转换区 */
  pinfo_str->p_bit_data = (unsigned char*)isirw_cnvpix_malloc(linesize * pinfo_str->height + 4);

  if (!pinfo_str->p_bit_data) {
    isirw_free_INFOSTR(pinfo_str);
    return 0;
  }

  /* 申请地址区 */
  pinfo_str->pp_line_addr = (void**)isirw_malloc(pinfo_str->height * sizeof(void*));

  if (!pinfo_str->pp_line_addr) {
    isirw_free_INFOSTR(pinfo_str);
    return 0;
  }

  /* 初始化行首地址 */
  if (pinfo_str->order == 0) {  /* 正向图 */
    for (i = 0; i < (int)pinfo_str->height; i++) {
      pinfo_str->pp_line_addr[i] = (void*)(unsigned char*)(pinfo_str->p_bit_data + i * linesize);
    }
  }
  else {            /* 倒向图 */
    for (i = 0; i < (int)pinfo_str->height; i++) {
      pinfo_str->pp_line_addr[i] = (void*)(unsigned char*)(pinfo_str->p_bit_data + ((pinfo_str->height - 1 - i) * linesize));
    }
  }

  pinfo_str->data_state = 2;

  return pinfo_str;
}


/* 创建一个新的转换用子图象数据包 */
LPSUBIMGBLOCK CALLAGREEMENT _isirw_build_si(LPSUBIMGBLOCK psub_str, int fmt/* 目标格式ID */)
{
  LPSUBIMGBLOCK psub_tmp;
  int       linesize, i, k;

  /* 申请新的子图象节点 */
  if ((psub_tmp = isirw_alloc_SUBIMGBLOCK()) == 0) {
    return 0;
  }

  /* 设置子图像节点数据 */
  psub_tmp->left  = psub_str->left;
  psub_tmp->top   = psub_str->top;
  psub_tmp->width = psub_str->width;
  psub_tmp->height = psub_str->height;

  assert((psub_tmp->width != 0) && (psub_tmp->height != 0));

  psub_tmp->bitcount = iscnv_idtobit(fmt);

  /* 扫描行尺寸 */
  linesize = DIBSCANLINE_DWORDALIGN(psub_tmp->width * psub_tmp->bitcount);

  /* 申请目标图像缓冲区（+4 － 附加的4字节缓冲区） */
  if ((psub_tmp->p_bit_data = (unsigned char*)isirw_malloc(linesize * psub_tmp->height + 4)) == 0) {
    isirw_free_SUBIMGBLOCK(psub_tmp);
    return 0;
  }

  /* 申请行地址数组 */
  if ((psub_tmp->pp_line_addr = (void**)isirw_malloc(sizeof(void*)*psub_tmp->height)) == 0) {
    isirw_free_SUBIMGBLOCK(psub_tmp);
    return 0;
  }

  psub_tmp->order = psub_str->order;

  /* 初始化行首地址 */
  if (psub_tmp->order == 0) { /* 正向图 */
    for (i = 0; i < (int)psub_tmp->height; i++) {
      psub_tmp->pp_line_addr[i] = (void*)(unsigned char*)(psub_tmp->p_bit_data + i * linesize);
    }
  }
  else {            /* 倒向图 */
    for (i = 0; i < (int)psub_tmp->height; i++) {
      psub_tmp->pp_line_addr[i] = (void*)(unsigned char*)(psub_tmp->p_bit_data + ((psub_tmp->height - 1 - i) * linesize));
    }
  }

  /* 设置掩码数据 */
  iscnv_get_mask(fmt, &(psub_tmp->r_mask), &(psub_tmp->g_mask), &(psub_tmp->b_mask), &(psub_tmp->a_mask));

  /* 处置方式 */
  psub_tmp->dowith    = psub_str->dowith;
  /* 在显示下一幅子图像前是否需要等待某一事件 */
  psub_tmp->userinput = psub_str->userinput;
  /* 延迟时间（1/1000 s）*/
  psub_tmp->time    = psub_str->time;
  /* 设置透明色 */
  psub_tmp->colorkey  = psub_str->colorkey;


  /* 复制调色板数据 */
  if (psub_tmp->bitcount <= 8) {
    switch (psub_tmp->bitcount) {
    case  1:
      if (psub_str->bitcount <= 8) {
        ((unsigned long*)(psub_tmp->palette))[0] = ((unsigned long*)psub_str->palette)[0];
        ((unsigned long*)(psub_tmp->palette))[1] = ((unsigned long*)psub_str->palette)[(unsigned int)pow((int)2, (int)psub_str->bitcount) / 2];

        if (psub_tmp->colorkey != -1) {
          if (psub_tmp->colorkey == (long)(((unsigned long*)(psub_tmp->palette))[0])) {
            psub_tmp->colorkey = (long)(((unsigned long*)(psub_tmp->palette))[0]);
          }
          else {
            psub_tmp->colorkey = (long)(((unsigned long*)(psub_tmp->palette))[1]);
          }
        }
      }
      else {
        memcpy((void*)psub_tmp->palette, (const void*)iscnv_get_pal(1), 2 * sizeof(unsigned long));

        if (psub_tmp->colorkey != -1) {
          /* 转为灰度值 */
          k = ((((psub_tmp->colorkey >> 16) & 0xff) * 3 + ((psub_tmp->colorkey >> 8) & 0xff) * 6 + (psub_tmp->colorkey & 0xff)) / 10) & 0xff;
          k = (k << 16) | (k << 8) | k;

          psub_tmp->colorkey = ((unsigned long)k >= ((unsigned long*)(psub_tmp->palette))[1]) ? ((unsigned long*)(psub_tmp->palette))[1] : ((unsigned long*)(psub_tmp->palette))[0];
        }
      }

      break;

    case  4:
      if (psub_str->bitcount <= 4) {  /* 1~4 bit */
        memcpy((void*)psub_tmp->palette, (const void*)psub_str->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));
      }
      else if (psub_str->bitcount <= 8) { /* 5~8 bit */
        k = (unsigned int)pow((int)2, (int)psub_str->bitcount) / 16;

        for (i = 0; i < 16; i++) {
          ((unsigned long*)(psub_tmp->palette))[i] = ((unsigned long*)psub_str->palette)[i * k];
        }

        if (psub_tmp->colorkey != -1) {
          /* 搜索透明色是否还存在于调色板中 */
          for (i = 0; i < 16; i++) {
            if (((unsigned long*)(psub_tmp->palette))[i] == (unsigned long)psub_tmp->colorkey) {
              break;
            }
          }

          /* 如果透明色调色板项已被废弃，则取消透明色 */
          if (i == 16) {
            psub_tmp->colorkey = -1;
          }
        }
      }
      else {
        memcpy((void*)psub_tmp->palette, (const void*)iscnv_get_pal(4), 16 * sizeof(unsigned long));

        if (psub_tmp->colorkey != -1) {
          /* 转为灰度值 */
          k = ((((psub_tmp->colorkey >> 16) & 0xff) * 3 + ((psub_tmp->colorkey >> 8) & 0xff) * 6 + (psub_tmp->colorkey & 0xff)) / 10) & 0xff;
          k = (k << 16) | (k << 8) | k;

          psub_tmp->colorkey = (long)k;
        }
      }

      break;

    case  8:
      if (psub_str->bitcount <= 8) {
        memcpy((void*)psub_tmp->palette, (const void*)psub_str->palette, MAX_PALETTE_COUNT * sizeof(unsigned long));
      }
      else {
        memcpy((void*)psub_tmp->palette, (const void*)iscnv_get_pal(8), 256 * sizeof(unsigned long));

        if (psub_tmp->colorkey != -1) {
          /* 转为灰度值 */
          k = ((((psub_tmp->colorkey >> 16) & 0xff) * 3 + ((psub_tmp->colorkey >> 8) & 0xff) * 6 + (psub_tmp->colorkey & 0xff)) / 10) & 0xff;
          k = (k << 16) | (k << 8) | k;

          psub_tmp->colorkey = (long)k;
        }
      }

      break;

    default:
      assert(0);
      return 0;
    }

    psub_tmp->pal_count = (unsigned int)pow((int)2, (int)psub_tmp->bitcount);
  }
  else {
    psub_tmp->pal_count = 0;
  }

  /* 子图象序号相等 */
  psub_tmp->number  = psub_str->number;

  return psub_tmp;
}


/* 获取最后一幅子图象的信息（序号及子图象数据包地址）*/
int CALLAGREEMENT _isirw_get_last_sert_image_info(LPINFOSTR pinfo, LPSUBIMGBLOCK* ppsi)
{
  LPSUBIMGBLOCK ptmp;

  if (pinfo->psubimg == 0) {
    *ppsi  = 0;
    return 0;             /* 没有子图象 */
  }

  ptmp = pinfo->psubimg;

  while (ptmp->next) {          /* 搜索最后一个子图象数据包 */
    ptmp = ptmp->next;
  }

  *ppsi  = ptmp;

  return (int)ptmp->number;
}


/* 获取指定序号的子图象信息（序号及子图象数据包地址）*/
int CALLAGREEMENT _isirw_get_sert_image_info(LPINFOSTR pinfo, int sid, LPSUBIMGBLOCK* ppsi)
{
  LPSUBIMGBLOCK ptmp;

  if (pinfo->psubimg == 0) {
    *ppsi  = 0;
    return 0;             /* 没有子图象 */
  }

  ptmp = pinfo->psubimg;

  while (ptmp) {            /* 搜索指定子图象数据包 */
    if (ptmp->number == sid) {
      break;
    }
    else {
      ptmp = ptmp->next;
    }
  }

  *ppsi  = ptmp;

  return (ptmp == 0) ? 0 : sid;
}


