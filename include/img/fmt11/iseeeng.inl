/********************************************************************

  iseeeng.c

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
  本文件用途：  图像读写引擎－消息泵模块实现文件
  本文件编写人：
          YZ        yzfree##sina.com
          达生      qing_se##163.com
          梦晖      mssoft##sina.com

  本文件版本：  30719
  最后修改于：  2003-7-19

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
  地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2003-7    修正一处BUG（在读函数中如果发生了转换行为，原来的代码没有相应的
          设置转换数据标志）
    2003-6    消息泵改用事件触发方式运行，有效减少CPU负荷（梦晖）
    2003-5    修改保存功能的数据源指向（优先保存转换后的图象数据）
    2003-2    修补保存接口中存在覆盖源图象的BUG。（达生）
    2003-2    修改消息泵工作方式，修补lock_pump()函数中的BUG
    2002-11   改进消息泵对空闲命令的处理方式（改为延迟处理）
    2002-10   第一个测试版发布

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
#include <process.h>
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
#include "iseeeng.h"
#include "queue.h"


/**********************************************************************************/
/* 内部用数据 */
static int        _isee_pump_mark    = 0;     /* 消息泵系统启用标志，0－未启用、1－启用 */
static unsigned long  _isee_pump_handle  = 0xffffffff;/* 消息泵句柄 */
static int        _isee_pump_locked  = 0;     /* 消息泵锁定标志，0－未锁、1－锁定 */
static int        _isee_pump_run_mark = 1;    /* 消息泵运行标志，0－关闭、1－运行 */
static unsigned int   _isee_pump_lccb    = 0;     /* 消息泵锁定验证标志 */
static HANDLE     _isee_pump_semap   = 0;     /* 消息信号灯 */


/**********************************************************************************/
/* 内部函数 */
static void __cdecl _isirw_pump(void* par);       /* 消息泵 */
void _isirw_pump_comm_process(void* pcomm);
void _isirw_pump_idle_process(void* pidle);
void _ipcp_get_info(LPSPEC_DATAPACK sdp);
void _ipcp_load(LPSPEC_DATAPACK sdp);
void _ipcp_save(LPSPEC_DATAPACK sdp);





/**********************************************************************************/
/* 接口函数 */

/* 打开消息泵系统 */
int CALLAGREEMENT open_pump()
{
  assert(_isee_pump_mark == 0);           /* 本函数不能重复调用 */

  /* 初始化队列 */
  init_queue();

  /* 申请队列信号灯 */
  if ((_isee_pump_semap = CreateSemaphore(0, 0, ISEEENG_QUEUE_COMM_SIZE, 0)) == 0) {
    destroy_queue();
    return -6;
  }

  /* 创建消息泵（线程）*/
  if ((_isee_pump_handle = _beginthread(_isirw_pump, 0, 0)) == 0xffffffff) {
    CloseHandle(_isee_pump_semap);
    _isee_pump_semap = 0;

    destroy_queue();
    return -6;                    /* 线程创建失败 */
  }

  /* 初始化各标志 */
  _isee_pump_locked = 0;              /* 消息泵锁定标志 */
  _isee_pump_run_mark = 1;              /* 消息泵运行标志 */
  _isee_pump_mark   = 1;              /* 消息泵系统启用标志 */
  _isee_pump_lccb     = 0;

  return 0;
}


/* 关闭消息泵系统 */
int CALLAGREEMENT close_pump()
{
  assert(_isee_pump_mark != 0);
  assert(_isee_pump_locked == 0);

  /* 先锁定消息泵，置停止标志 */
  lock_pump();

  assert(_isee_pump_locked == 1);

  _isee_pump_run_mark = 0;

  ReleaseSemaphore(_isee_pump_semap, 1, 0);     /* 增加信号灯，以使引擎能更快的停止 */

  /* 等待消息泵停止 */
  while (_isee_pump_run_mark == 0) {
    Sleep(0);
  }

  assert(get_comm_count() == 0);            /* 此时命令队列中不应该再有命令 */

  /* 销毁队列 */
  destroy_queue();

  /* 销毁队列信号灯 */
  CloseHandle(_isee_pump_semap);

  _isee_pump_mark     = 0;
  _isee_pump_run_mark = 1;
  _isee_pump_locked   = 0;
  _isee_pump_lccb     = 0;
  _isee_pump_semap    = 0;

  return 0;
}


/* 获取消息泵系统的状态（是否建立）*/
int CALLAGREEMENT get_pump_status()
{
  /* 0-未启用、1-启用 */
  return _isee_pump_mark;
}


/* 锁定消息泵 */
void CALLAGREEMENT lock_pump()
{
  unsigned int  ispl;

  assert(_isee_pump_mark == 1);
  assert(_isee_pump_locked == 0);   /* 不允许嵌套锁定 */

  ispl = _isee_pump_lccb;

  _isee_pump_locked = 1;        /* 置锁定标志 */

  while (ispl == _isee_pump_lccb) { /* 等待，直到获得锁定验证为止 */
    Sleep(0);
  }
}


/* 解锁消息泵 */
void CALLAGREEMENT unlock_pump()
{
  assert(_isee_pump_mark == 1);
  assert(_isee_pump_locked == 1);   /* 不允许嵌套锁定 */

  _isee_pump_locked = 0;
}


/* 加入命令 */
int CALLAGREEMENT add_pump_comm(LPSPEC_DATAPACK p)
{
  int   result;

  assert(_isee_pump_mark == 1);
  assert(p->exec_state == IRWE_SDPSTATE_IDLE);

  p->exec_state = IRWE_SDPSTATE_WAIT;

  /* 根据命令确定处理的优先级 */
  if (p->command == IRWE_SDPCOMM_GET_INFO) {
    result = add_comm((void*)p, 0);         /* 加入命令队列的尾部 */
  }
  else {
    result = add_comm((void*)p, 1);         /* 加入命令队列的头部（优先处理）*/
  }

  /* 如果命令包未被加入队列，则包的执行状态置为空闲 */
  if (result == -1) {
    p->exec_state = IRWE_SDPSTATE_IDLE;
  }

  /* 增加信号灯个数 */
  ReleaseSemaphore(_isee_pump_semap, 1, 0);

  return result;                    /* 0 :加入成功、-1 :内存不足，失败 */
}


/* 取回命令（根据参数p） */
int CALLAGREEMENT dist_pump_comm(LPSPEC_DATAPACK p)
{
  void*  pt;

  assert((_isee_pump_mark == 1) && (p));

  /* 预先减少队列信号灯个数 */
  if (WaitForSingleObject(_isee_pump_semap, 0) != WAIT_OBJECT_0) {
    return -1;                    /* 命令队列中已无命令 */
  }

  /* 搜索并提取指定包 */
  if ((pt = distill_comm((void*)p)) != 0) {
    assert(pt == p);
    assert(p->exec_state == IRWE_SDPSTATE_WAIT);  /* 刚从队列中取出时应具有的状态 */
    p->exec_state = IRWE_SDPSTATE_IDLE;       /* 置包的执行状态为空闲 */
  }
  else {
    /* 如果没能提取出指定命令，则恢复信号灯个数 */
    ReleaseSemaphore(_isee_pump_semap, 1, 0);
  }

  return (pt == 0) ? -1 : 0;  /* 0 :找到并取出，-1 :未找到指定的命令 */
}


/* 取回指定批次号（sn）的第一个命令 */
LPSPEC_DATAPACK CALLAGREEMENT dist_pump_comm_sn(unsigned long sn)
{
  void*  pt;

  assert(_isee_pump_mark == 1);

  /* 预先减少队列信号灯个数 */
  if (WaitForSingleObject(_isee_pump_semap, 0) != WAIT_OBJECT_0) {
    return 0;                   /* 0 :命令队列中已无命令 */
  }

  /* 提取指定批次的第一个包 */
  if ((pt = distill_comm_sn(sn)) != 0) {
    assert(((LPSPEC_DATAPACK)pt)->exec_state == IRWE_SDPSTATE_WAIT);
    ((LPSPEC_DATAPACK)pt)->exec_state = IRWE_SDPSTATE_IDLE;
  }
  else {
    /* 如果没有指定批次的命令，则恢复信号灯个数 */
    ReleaseSemaphore(_isee_pump_semap, 1, 0);
  }

  return (LPSPEC_DATAPACK)pt;             /* 0 :命令队列中已无sn批次的命令、非0 :提取出的命令 */
}


/* 获取待处理的命令个数 */
int CALLAGREEMENT get_pump_comm_count()
{
  assert(_isee_pump_mark == 1);

  return (int)get_comm_count();
}


/* 加空闲处理命令 */
int CALLAGREEMENT add_pump_idle(void* p/*空闲处理函数地址*/)
{
  assert(_isee_pump_mark == 1);

  return add_idlesse(p);                /* 0 :加入成功、-1 :队列满，失败 */
}





/**********************************************************************************/
/* ISee图象读写引擎消息泵 */

static void __cdecl _isirw_pump(void* par)
{
  void*  pcomm = 0;

  /* 等待open_pump()函数初始化完所有的标志 */
  while (_isee_pump_mark == 0) {
    Sleep(0);
  }

  /* 提取并处理命令 */
  while (_isee_pump_run_mark) {
    /* 如果泵被锁定，则不进行任何命令的处理。释放剩余的时间片 */
    if (_isee_pump_locked) {
      _isee_pump_lccb++;              /* 设置锁定验证 */
      Sleep(0);
      continue;
    }

    /* 判断是否有待处理的命令 */
    switch ((unsigned int)WaitForSingleObject(_isee_pump_semap, ISEEENG_IDLE_STIME * 1000)) {
    case  WAIT_OBJECT_0:

      /* 处理队列中的读写命令 */
      if ((pcomm = distill_comm(0)) != 0) {
        _isirw_pump_comm_process(pcomm);
      }

      break;

    case  WAIT_TIMEOUT:

      /* 超时，则进行空闲指令处理 */
      if ((pcomm = distill_idlesse()) != 0) {
        _isirw_pump_idle_process(pcomm);
      }

      break;

    default:
      break;
    }
  }

  /* 向close_pump()函数传递停止信息 */
  _isee_pump_run_mark = 1;

  /* 退出线程，并释放资源 */
  _endthread();
}


/* 命令处理 */
void _isirw_pump_comm_process(void* pcomm)
{
  LPSPEC_DATAPACK sdp = (LPSPEC_DATAPACK)pcomm;

  assert((sdp) && (sdp->dbg_mark == DATAPACK_MARK_SPECIALTY));
  assert(sdp->exec_state == IRWE_SDPSTATE_WAIT);

  /* 标志执行开始 */
  sdp->exec_state = IRWE_SDPSTATE_EXEC;

  /* 根据命令调用相应的解释函数 */
  switch (sdp->command) {
  case  IRWE_SDPCOMM_GET_INFO:            /* 读图象信息 */
    _ipcp_get_info(sdp);
    break;

  case  IRWE_SDPCOMM_LOAD:              /* 读图象 */
    _ipcp_load(sdp);
    break;

  case  IRWE_SDPCOMM_SAVE:              /* 保存图象 */
    _ipcp_save(sdp);
    break;

  default:                      /* 非法命令 */
    assert(0);
    break;
  }

  /* 标志执行结束 */
  sdp->exec_state = IRWE_SDPSTATE_IDLE;
}


/* 获取图象信息命令解释函数 */
void _ipcp_get_info(LPSPEC_DATAPACK sdp)
{
  unsigned long     handle;
  IRWP_API_GET_IMG_INFO pfn_get_info;

  /* 本命令没有进度信息要传出 */
  sdp->s_total   = 0;   /* 总进度值（累进式）*/
  sdp->s_current = 0;   /* 当前进度值 */
  sdp->s_state   = 0;   /* 进度状态（图象号） */

  /* 如果数据包中已经包含了图象信息，则直接返回 */
  if (sdp->data_state & SDP_DATA_STATE_ORG_INFO) {
    sdp->last_err = ER_SUCCESS;
    sdp->s_break   = 0;   /* 终止操作标志（非0终止） */
    return;
  }

  /* 检测用户是否要中断下面的操作 */
  if (sdp->s_break) {
    sdp->last_err = ER_USERBREAK;
    return;
  }

  /* 取得处理插件的句柄 */
  if ((handle = ismf_get_plugin_heandle((const char*)sdp->path->name)) == 0) {
    sdp->last_err = ER_NONIMAGE;
    return;
  }

  /* 取得get_info接口地址 */
  pfn_get_info = ismf_get_info_addr(handle);
  assert(pfn_get_info);

  /* 获取图象信息 */
  sdp->last_err = (*pfn_get_info)(sdp->path, sdp->org_img);

  /* 释放处理插件句柄 */
  ismf_free_plugin_heandle(handle);

  /* 标明该包已含有原始图象信息 */
  if (sdp->last_err == ER_SUCCESS) {
    sdp->data_state |= SDP_DATA_STATE_ORG_INFO;
  }

  return;
}


/* 读图象命令解释函数 */
void _ipcp_load(LPSPEC_DATAPACK sdp)
{
  unsigned long   handle;
  IRWP_API_LOAD_IMG pfn_load;

  /* 初始化进度信息 */
  sdp->s_total   = 1;   /* 总进度值（累进式）*/
  sdp->s_current = 0;   /* 当前进度值 */
  sdp->s_state   = 0;   /* 进度状态（图象号） */

  /* 如果数据包中已经包含了图象，则直接返回 */
  if (sdp->data_state & SDP_DATA_STATE_ORG_PIX) {
    sdp->last_err = ER_SUCCESS;
    sdp->s_break   = 0;   /* 终止操作标志（非0终止） */
    return;
  }

  /* 检测用户是否要中断下面的操作 */
  if (sdp->s_break) {
    sdp->last_err = ER_USERBREAK;
    return;
  }

  /* 取得处理插件的句柄 */
  if ((handle = ismf_get_plugin_heandle((const char*)sdp->path->name)) == 0) {
    sdp->last_err = ER_NONIMAGE;
    return;
  }

  /* 取得load接口地址 */
  pfn_load = ismf_get_load_addr(handle);
  assert(pfn_load);

  /* 获取图象象素数据 */
  sdp->last_err = (*pfn_load)(sdp->path, sdp->org_img);

  /* 释放处理插件句柄 */
  ismf_free_plugin_heandle(handle);

  /* 标明该包已含有原始图象 */
  if (sdp->last_err == ER_SUCCESS) {
    sdp->data_state |= SDP_DATA_STATE_ORG_INFO;
    sdp->data_state |= SDP_DATA_STATE_ORG_PIX;

    /* 如果发生了转换，则设置转换数据标志 */
    if (sdp->cnv_img) {
      sdp->data_state |= SDP_DATA_STATE_CNV;
    }
  }

  return;
}


/* 保存图象命令解释函数 */
void _ipcp_save(LPSPEC_DATAPACK sdp)
{
  unsigned long   handle;
  IRWP_API_SAVE_IMG pfn_save;

  /* 初始化进度信息 */
  sdp->s_total   = 1;   /* 总进度值（累进式）*/
  sdp->s_current = 0;   /* 当前进度值 */
  sdp->s_state   = 0;   /* 进度状态（图象号） */

  /* 如果数据包中没有图象，则直接返回 */
  if (!(sdp->data_state & SDP_DATA_STATE_ORG_PIX)) {
    assert(0);
    sdp->last_err = ER_SYSERR;
    return;
  }

  /* 检测用户是否要中断下面的操作 */
  if (sdp->s_break) {
    sdp->last_err = ER_USERBREAK;
    return;
  }

  /* 取得处理插件的句柄 */
  if ((handle = ismf_get_plugin_heandle((const char*)sdp->despath->name)) == 0) {
    sdp->last_err = ER_NONIMAGE;
    return;
  }

  /* 取得save接口地址 */
  pfn_save = ismf_get_save_addr(handle);
  assert(pfn_save);

  /* 保存包中的图象 */
  if (sdp->cnv_img) {
    sdp->last_err = (*pfn_save)(sdp->despath, sdp->cnv_img, sdp->savestr);  /* 如果存在转换图象，则存储转换图象 */
  }
  else {
    sdp->last_err = (*pfn_save)(sdp->despath, sdp->org_img, sdp->savestr);
  }

  /* 释放处理插件句柄 */
  ismf_free_plugin_heandle(handle);

  return;
}



/* 空闲处理 */
void _isirw_pump_idle_process(void* pidle)
{
  _ISIRW_PUMP_IDLE_FN fnp = (_ISIRW_PUMP_IDLE_FN)pidle;

  (*fnp)();

  return;
}





