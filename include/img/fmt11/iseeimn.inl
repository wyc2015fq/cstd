/********************************************************************

  iseeimn.c

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
  本文件用途：  图像读写引擎－插件管理实现文件
  本文件编写人：
          YZ        yzfree##sina.com

  本文件版本：  30517
  最后修改于：  2003-5-17

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
    地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2003-5    ismf_update()函数增加返回信息结构
          修正调入优先级的计算方法
    2003-2    修正空闲命令发送方式中存在的BUG，以防空闲队列溢出
    2002-8    第一个测试版发布

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


/*************************************************************************/
static LPIRWMF_HEADER ismf_info = 0;          /* 插件信息表 */
static CRITICAL_SECTION ismf_cs;            /* 插件信息表访问关键段 */

static char     _ismf_openstr[ISIRW_OPENSTR_SIZE];  /* 用于Windows平台OPEN对话框的扩展名过滤串 */
/* 串格式请参见MSDN中有关CFileDialog构造函数第5个参数的文档说明 */
static char     _ismf_shellstr[ISIRW_OPENSTR_SIZE]; /* 通用扩展名过滤串*/
/* 串格式为："*.bmp,*.gif,*.xxx,....." */

static unsigned long  _ismf_idle_counter = 0;     /* 空闲处理包抛送计数 */



/*************************************************************************/
LPIRWMF_INFOBLOCK _ismf_alloc_IRWMF_INFOBLOCK(void);
void        _ismf_free_IRWMF_INFOBLOCK(LPIRWMF_INFOBLOCK pt);
LPIRWMF_HEADER    _ismf_alloc_IRWMF_HEADER(void);
void        _ismf_free_IRWMF_HEADER(LPIRWMF_HEADER pt);

int         _create_mini(char* path_str, LPIRWMF_HEADER ispt);
int         _load_mini(FILE* fp, LPIRWMF_HEADER ispt);
void        _verify_mini(LPIRWMF_HEADER phead, char* path, LPUPDATAINFO rinfo);

LPIRWMF_INFOBLOCK _remove_node(LPIRWMF_HEADER phead, int type, LPIRWMF_INFOBLOCK pnode);

int         _search_plugin_info_block(LPIRWMF_INFOBLOCK* ppblock, const char* path);
LPIRWMF_INFOBLOCK _ismf_search_at_filename(const char* ss);
int         _isirw_update_filtrate_str(void);
char* CALLAGREEMENT _ismf_get_openstr(void);
char* CALLAGREEMENT _ismf_get_shellstr(void);

void        _ismf_idle(void);



/*************************************************************************/


/* 初始化插件信息表（内存）*/
int CALLAGREEMENT ismf_init(char* path,/*插件INI文件所在路径*/ char* fname/*优先处理的图象文件*/)
{
  FILE*    fp = 0;
  char    pfull[MAX_PATH], path2[MAX_PATH], path3[MAX_PATH];
  int     result = 0, i = 0, actcnt = IRWMF_ACTIVE_INIT_CNT, j;

  LPIRWMF_INFOBLOCK ptmp;
  LPIRWMF_INFOBLOCK phig;


  assert(path);
  assert(ismf_info == 0);               /* 本函数不能重复调用 */


  __try {
    __try {
      /* 初始化信息表访问关键段 */
      InitializeCriticalSection(&ismf_cs);
      EnterCriticalSection(&ismf_cs);

      /* 清空过滤串缓冲区 */
      memset((void*)_ismf_openstr, 0, ISIRW_OPENSTR_SIZE);
      memset((void*)_ismf_shellstr, 0, ISIRW_OPENSTR_SIZE);

      /* 申请新的的表头结构 */
      if ((ismf_info = _ismf_alloc_IRWMF_HEADER()) == 0) {
        result = -1;
        __leave;         /* 内存不足 */
      }

      /* 合成信息文件的全路径 */
      strcpy((char*)pfull, (const char*)path);

      /* 确保路径的最后一个字符是"\" */
      if (pfull[strlen((const char*)pfull) - 1] != '\\') {
        strcat((char*)pfull, (const char*)"\\");
      }

      /* 合成INI文件全路径 */
      strcpy((char*)path2, (const char*)pfull);
      strcat((char*)pfull, (const char*)IRWMF_FILENAME);

      /* 试探着打开INI文件 */
      if ((fp = fopen((const char*)pfull, "rb")) == 0) {
        /* 没有找到INI文件，试着创建它 */
        switch (_create_mini(path2, ismf_info)) {
        case  0:                /* 创建成功 */
          break;

        case  1:                /* 无可用插件 */
          result = -2;
          __leave;
          break;

        case  2:                /* 文件读写错误 */
          result = -3;
          __leave;
          break;

        case  3:                /* 内存不足 */
          result = -1;
          __leave;
          break;

        default:                /* 系统异常 */
          result = -5;
          __leave;
          break;
        }

        assert(ismf_info->plugin_num > 0);    /* 至少应该有一个插件 */

        ismf_info->build = IRWMF_HEADER_BUD_CREATE;   /* 表创建的方式 */
      }
      else {
        /* 找到了INI文件，试着读入其中的内容 */
        switch (_load_mini(fp, ismf_info)) {
        case  0:                /* 读入成功 */
          break;

        case  1:                /* 文件读写错误 */
          result = -3;
          __leave;
          break;

        case  2:                /* INI文件内容非法 */
          result = -4;
          __leave;
          break;

        case  3:                /* 内存不足 */
          result = -1;
          __leave;
          break;

        default:                /* 系统异常 */
          result = -5;
          __leave;
          break;
        }

        assert(ismf_info->plugin_num > 0);

        /* 校验读入的INI内容 */
        _verify_mini(ismf_info, path2, 0);

        /* 检测插件个数是否为0 */
        if (ismf_info->plugin_num == 0) {
          result = -2;
          __leave;       /* 没有插件可用 */
        }

        /* 设置表创建的方式 */
        ismf_info->build = IRWMF_HEADER_BUD_LOAD;
      }

      /* 如果调用者指定了某个图象文件名，则先激活对应的插件 */
      if (fname != 0) {
        /* 根据文件名搜索对应插件 */
        if (_search_plugin_info_block(&ptmp, (const char*)fname) > 0) {
          assert(ptmp && ptmp->use && (ptmp->act == 0));

          /* 合成插件全路径 */
          strcpy((char*)path3, (const char*)path2);
          strcat((char*)path3, (const char*)ptmp->path);

          /* 读入插件 */
          if ((ptmp->handle = LoadLibrary((LPCTSTR)path3)) == 0) {
            result = -3;
            __leave;     /* 文件读写错误 */
          }

          /* 取得接口函数地址 */
          if (((ptmp->fn_get_img_info = (IRWP_API_GET_IMG_INFO)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), get_image_info)) == 0) || \
              ((ptmp->fn_load_img = (IRWP_API_LOAD_IMG)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), load_image)) == 0) || \
              ((ptmp->fn_save_img = (IRWP_API_SAVE_IMG)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), save_image)) == 0)) {
            /* 插件文件遭到破坏，舍弃该插件 */
            FreeLibrary(ptmp->handle);
            ptmp->handle = 0;
            result = -4;
            __leave;
          }

          ptmp->act = 1;            /* 设置激活标志 */

          assert(actcnt > 0);
          actcnt--;
        }
      }

      /* 根据插件优先级激活插件 */
      for (i = 0; i < actcnt; i++) {
        phig = 0;

        /* 搜索录用链中优先级最高的插件 */
        for (j = 0, ptmp = ismf_info->puh; j < ismf_info->uc; j++, ptmp = ptmp->next) {
          assert(ptmp);

          /* 如果该插件已经被激活，则继续搜索下一个插件 */
          if (ptmp->act > 0) {
            continue;
          }

          /* 冒泡排序法找到优先级最高的插件 */
          if ((phig == 0) || (ptmp->pr > phig->pr)) {
            phig = ptmp;
          }
        }

        /* 如果已经没有可激活插件了，则退出循环 */
        if (phig == 0) {
          break;
        }

        ptmp = phig;

        assert(ptmp && ptmp->use && (ptmp->act == 0));

        /* 合成插件全路径 */
        strcpy((char*)path3, (const char*)path2);
        strcat((char*)path3, (const char*)ptmp->path);

        /* 读入插件 */
        if ((ptmp->handle = LoadLibrary((LPCTSTR)path3)) == 0) {
          result = -3;
          __leave;         /* 文件读写错误 */
        }

        /* 取得接口函数地址 */
        if (((ptmp->fn_get_img_info = (IRWP_API_GET_IMG_INFO)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), get_image_info)) == 0) || \
            ((ptmp->fn_load_img = (IRWP_API_LOAD_IMG)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), load_image)) == 0) || \
            ((ptmp->fn_save_img = (IRWP_API_SAVE_IMG)isirw_get_api_addr(ptmp->handle, &(ptmp->ifo), save_image)) == 0)) {
          /* 插件文件遭到破坏，舍弃该插件 */
          FreeLibrary(ptmp->handle);
          ptmp->handle = 0;
          result = -4;
          __leave;
        }

        ptmp->act = 1;                /* 设置激活标志 */
      }

      _isirw_update_filtrate_str();         /* 填写扩展名过滤串 */

      ismf_info->mark = 1;              /* 表头信息有效标记 */
    }
    __finally {
      if ((result != 0) || (AbnormalTermination())) {
        if (ismf_info) {
          /* 如果有插件处于激活状态，则释放该插件 */
          for (i = 0, ptmp = ismf_info->puh; i < (int)ismf_info->uc; i++, ptmp = ptmp->next) {
            assert(ptmp);
            assert(ptmp->act <= 1);     /* 该插件现在还不可能被消息泵使用 */

            /* 如果插件已被激活，则释放它 */
            if (ptmp->act == 1) {
              FreeLibrary(ptmp->handle);

              ptmp->handle          = 0;
              ptmp->fn_get_img_info = 0;
              ptmp->fn_load_img     = 0;
              ptmp->fn_save_img     = 0;
              ptmp->act = 0;
            }
          }

          _ismf_free_IRWMF_HEADER(ismf_info);
          ismf_info = 0;
        }

        LeaveCriticalSection(&ismf_cs);
        DeleteCriticalSection(&ismf_cs);
      }
      else {
        LeaveCriticalSection(&ismf_cs);
      }

      if (fp) {
        fclose(fp);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = -5;                  /* 系统异常 */
  }

  return result;
}


/* 获取插件管理系统的启用状态 */
int CALLAGREEMENT ismf_get_status()
{
  if ((ismf_info) && (ismf_info->mark)) {
    return 1;                   /* 系统已启用 */
  }
  else {
    return 0;                   /* 系统尚未启用 */
  }
}


/* 锁定插件信息表（引擎也将暂停工作）*/
LPIRWMF_HEADER CALLAGREEMENT ismf_lock()
{
  int         i;
  LPIRWMF_INFOBLOCK pii;

  assert((ismf_info) && (ismf_info->mark == 1));

  /* 设置锁定标志 */
  EnterCriticalSection(&ismf_cs);
  assert(ismf_info->locked == 0);           /* 信息表不能被嵌套锁定 */
  ismf_info->locked++;
  LeaveCriticalSection(&ismf_cs);

  /* 等待所有的插件都空闲下来 */
  for (i = 0, pii = ismf_info->puh; i < (int)ismf_info->uc; i++, pii = pii->next) {
    if ((pii->act == 0) || (pii->act == 1)) {
      continue;
    }
    else {
      while (pii->act > 1) {
        Sleep(ISIRW_THREAD_IDLE);
      }
    }
  }

  return ismf_info;
}


/* 刷新插件信息表（此时插件信息表必须处于锁定状态下）*/
int CALLAGREEMENT ismf_update(LPIRWMF_HEADER pt, int type, LPUPDATAINFO rinfo)
{
  int         i, result = 0;
  char        path[MAX_PATH], path2[MAX_PATH];
  LPIRWMF_INFOBLOCK pa;
  LPIRWMF_INFOBLOCK pifb = 0;
  LPIRWP_INFO     pii = 0;

  WIN32_FIND_DATA   file;
  HANDLE        hFind = 0;
  HMODULE       hDll  = 0;

  WIN32_FILE_ATTRIBUTE_DATA wfa;

  /* 此时信息表必须有效，并且必须处于锁定状态 */
  assert(ismf_info && (ismf_info->mark == 1) && ismf_info->locked);
  assert((ismf_info == pt) && (ismf_info->plugin_num > 0));
  assert((type == IRWE_UPDTYPE_EMPLOY) || (type == IRWE_UPDTYPE_DISMISSAL));

  __try {
    __try {
      /* 提取插件所在路径并合成搜索串 */
      strcpy((char*)path, (const char*)ismf_info->infpath);
      *(char*)(strrchr((const char*)path, '\\') + 1) = '\0';
      strcpy((char*)path2, (const char*)path);
      strcat((char*)path2, (const char*)"*.dll");

      /* 校验已存在插件信息（去除表中可能已经不存在的插件的信息块） */
      _verify_mini(ismf_info, path, rinfo);

      /* 搜索是否有新的或升级的插件 */
      hFind = FindFirstFile((LPCTSTR)path2, &file);

      if (INVALID_HANDLE_VALUE == hFind) {
        assert(ismf_info->plugin_num == 0);
        result = 1;
        __leave;              /* 没有找到图象读写插件 */
      }

      do {
        /* 合成全路径 */
        strcpy((char*)path2, (const char*)path);
        strcat((char*)path2, (const char*)(file.cFileName));

        /* 此文件是否在信息表中（存在就表示它是一个‘老’插件）*/
        if ((pa = _ismf_search_at_filename((const char*)file.cFileName)) == 0) {
          /* 不存在 */
          /* 读入此DLL文件 */
          if ((hDll = LoadLibrary((LPCTSTR)path2)) == 0) {
            result = 2;
            __leave;          /* 读入错误 */
          }

          /* 判断此DLL是否是图象读写插件 */
          if ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0) {
            FreeLibrary(hDll);
            hDll = 0;
            continue;               /* 此DLL不是插件，继续 */
          }

          /* 分配信息块缓冲区 */
          if ((pifb = _ismf_alloc_IRWMF_INFOBLOCK()) == 0) {
            result = 3;
            __leave;          /* 内存不足 */
          }

          /* 记录新增插件的信息块结构地址 */
          if (rinfo) {
            ((IRWMF_INFOBLOCK**)rinfo->newpp)[rinfo->newcnt++] = pifb;
          }

          /* 填写信息块 */
          pifb->pr    = IRWMF_INFOBLOCK_DEFPIR; /* 插件调入优先级 */
          pifb->use   = (type == IRWE_UPDTYPE_EMPLOY) ? IRWMF_INFOBLOCK_USEING : IRWMF_INFOBLOCK_NOTUSE;
          pifb->pilen   = file.nFileSizeLow;    /* 插件文件长度 */
          /* 插件文件创建时间 */
          pifb->ctimeh  = file.ftCreationTime.dwHighDateTime;
          pifb->ctimel  = file.ftCreationTime.dwLowDateTime;
          /* 插件文件最后的修改时间 */
          pifb->mtimeh  = file.ftLastWriteTime.dwHighDateTime;
          pifb->mtimel  = file.ftLastWriteTime.dwLowDateTime;

          /* 插件文件名 */
          strcpy((char*)pifb->path, (const char*)file.cFileName);
          /* 插件信息表 */
          memcpy((void*) & (pifb->ifo), (const void*)pii, sizeof(IRWP_INFO));

          pifb->act   = 0;            /* 激活标记（缺省为未激活）*/
          pifb->handle  = 0;            /* 插件句柄 */

          pifb->fn_get_img_info = 0;        /* get_image_info接口 */
          pifb->fn_load_img   = 0;        /* load_image接口 */
          pifb->fn_save_img   = 0;        /* save_image接口 */

          pifb->prev    = 0;            /* 前一个信息块的地址 */
          pifb->next    = 0;            /* 下一个信息块的地址 */
          pifb->pe    = pt;           /* 表头地址 */

          if (pifb->use == IRWMF_INFOBLOCK_USEING) {
            /* 加入表头结构中的录用链 */
            if (pt->put == 0) {           /* 第一个 */
              pt->puh = pt->put = pifb;
            }
            else {                  /* 后续 */
              pifb->prev = pt->put;
              pt->put->next = pifb;
              pt->put = pifb;
            }

            pt->uc++;
          }
          else {
            assert(pifb->use == IRWMF_INFOBLOCK_NOTUSE);

            /* 加入表头结构中的未录用链 */
            if (pt->pnt == 0) {           /* 第一个 */
              pt->pnh = pt->pnt = pifb;
            }
            else {                  /* 后续 */
              pifb->prev = pt->pnt;
              pt->pnt->next = pifb;
              pt->pnt = pifb;
            }

            pt->nc++;
          }

          /* 插件总个数累加 */
          pt->plugin_num++;

          /* 释放插件DLL */
          FreeLibrary(hDll);

          hDll = 0;
          pifb = 0;
        }
        else {                    /* 存在 */
          /* 获取插件文件的信息 */
          if (GetFileAttributesEx((LPCSTR)path2, GetFileExInfoStandard, (LPVOID)&wfa) == 0) {
            /* 对应的文件不应该找不到（前面已用校验函数校验过了） */
            result = 4;
            __leave;          /* 系统错误 */
          }
          else {
            i = 0;

            /* 如果插件文件被改动（升级），则刷新信息 */
            if (pa->pilen != wfa.nFileSizeLow) {
              i++;
              pa->pilen = wfa.nFileSizeLow;
            }

            if ((pa->ctimel != wfa.ftCreationTime.dwLowDateTime) || \
                (pa->ctimeh != wfa.ftCreationTime.dwHighDateTime)) {
              i++;
              pa->ctimel = wfa.ftCreationTime.dwLowDateTime;
              pa->ctimeh = wfa.ftCreationTime.dwHighDateTime;
            }

            if ((pa->mtimel != wfa.ftLastWriteTime.dwLowDateTime) || \
                (pa->mtimeh != wfa.ftLastWriteTime.dwHighDateTime)) {
              i++;
              pa->mtimel = wfa.ftLastWriteTime.dwLowDateTime;
              pa->mtimeh = wfa.ftLastWriteTime.dwHighDateTime;
            }

            if (i) {
              assert(pa->act == 0);       /* 激活的插件文件是不应该被操作系统改变的 */

              /* 判断此DLL是否是图象读写插件 */
              if (((hDll = LoadLibrary((LPCTSTR)path2)) == 0) || \
                  ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0)) {
                /* 插件文件遭到破坏，舍弃该插件 */
                if (hDll) {
                  FreeLibrary(hDll);
                  hDll = 0;
                }

                /* 将节点摘除 */
                _remove_node(pt, (pa->use == IRWMF_INFOBLOCK_USEING) ? 0 : 1, pa);

                assert(pa->tag == IRWMF_INFOBLOCK_TAG);

                /* 记录被删除插件的名字 */
                if (rinfo) {
                  strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(pa->ifo.irwp_name));
                }

                /* 释放信息块 */
                _ismf_free_IRWMF_INFOBLOCK(pa);
              }
              else {
                /* 取得新的插件信息 */
                memcpy((void*) & (pa->ifo), (const void*)pii, sizeof(IRWP_INFO));

                /* 记录升级插件的信息块地址 */
                if (rinfo) {
                  ((IRWMF_INFOBLOCK**)rinfo->updpp)[rinfo->updcnt++] = pa;
                }

                FreeLibrary(hDll);
                hDll = 0;
              }
            }
          }
        }
      }
      while (FindNextFile(hFind, &file));

      _isirw_update_filtrate_str();           /* 刷新扩展名过滤串 */

      if (ismf_info->plugin_num == 0) {
        result = 1;
        __leave;              /* 没有找到图象读写插件 */
      }
    }
    __finally {
      if (pifb) {
        _ismf_free_IRWMF_INFOBLOCK(pifb);
      }

      if (hDll) {
        FreeLibrary(hDll);
      }

      if (hFind) {
        FindClose(hFind);
      }

      if (ismf_info->plugin_num == 0) {
        _ismf_free_IRWMF_HEADER(ismf_info);
        ismf_info = 0;

        DeleteCriticalSection(&ismf_cs);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = 4;                       /* 系统异常 */
  }

  return result;
}


/* 在信息表中查找是否有指定的文件（以文件名）*/
LPIRWMF_INFOBLOCK _ismf_search_at_filename(const char* ss)
{
  int         ui, ni;
  LPIRWMF_INFOBLOCK pb;

  /* 此函数只能被ismf_update()函数调用，信息表必须有效，并且必须处于锁定状态 */
  assert(ismf_info && (ismf_info->mark == 1) && ismf_info->locked && ss);


  /* 在录用链中搜索 */
  for (ui = 0, pb = ismf_info->puh; ui < (int)ismf_info->uc; ui++, pb = pb->next) {
    assert(pb);

    /* 比较文件名是否相同 */
    if (_stricmp((const char*)ss, (const char*)pb->path) == 0) {
      return pb;
    }
  }

  /* 在未用链中搜索 */
  for (ni = 0, pb = ismf_info->pnh; ni < (int)ismf_info->nc; ni++, pb = pb->next) {
    assert(pb);

    if (_stricmp((const char*)ss, (const char*)pb->path) == 0) {
      return pb;
    }
  }

  /* 未找到返回0 */
  return 0;
}


/* 将指定的未用插件转至录用链表（但并不激活）*/
int CALLAGREEMENT ismf_employ(int id /*插件ID号码*/)
{
  int         ni;
  LPIRWMF_INFOBLOCK pb;

  /* 信息表必须有效，并且必须处于锁定状态 */
  assert(ismf_info && (ismf_info->mark == 1) && ismf_info->locked);

  /* 在未用链中搜索id标识的插件 */
  for (ni = 0, pb = ismf_info->pnh; ni < (int)ismf_info->nc; ni++, pb = pb->next) {
    assert(pb);

    if ((int)pb->ifo.plug_id == id) {
      break;
    }
  }

  /* 如果没有找到id标识的插件，则说明调用者的程序存在错误 */
  assert(pb);

  /* 从未用链中将插件信息块摘除 */
  _remove_node(ismf_info, 1, pb);

  /* 修正信息块内容 */
  pb->use     = IRWMF_INFOBLOCK_USEING;     /* 设置录用标记 */

  pb->act     = 0;                /* 激活标记 */
  pb->handle    = 0;                /* 插件句柄 */
  pb->fn_get_img_info = 0;              /* get_image_info接口 */
  pb->fn_load_img   = 0;              /* load_image接口 */
  pb->fn_save_img   = 0;              /* save_image接口 */

  pb->prev      = 0;              /* 前一个信息块的地址 */
  pb->next      = 0;              /* 下一个信息块的地址 */
  pb->pe        = ismf_info;          /* 表头地址 */

  /* 加入录用链尾部 */
  if (ismf_info->put == 0) {            /* 第一个 */
    ismf_info->puh = ismf_info->put = pb;
  }
  else {                      /* 后续 */
    pb->prev = ismf_info->put;
    ismf_info->put->next = pb;
    ismf_info->put = pb;
  }

  /* 录用插件个数累加 */
  ismf_info->uc++;
  ismf_info->plugin_num++;

  /* 刷新扩展名过滤串 */
  _isirw_update_filtrate_str();

  /* 成功返回0 */
  return 0;
}


/* 将指定的录用插件转至未用链表 */
int CALLAGREEMENT ismf_dismissal(int id /*插件ID号码*/)
{
  int         ui;
  LPIRWMF_INFOBLOCK pb;

  /* 信息表必须有效，并且必须处于锁定状态 */
  assert(ismf_info && (ismf_info->mark == 1) && ismf_info->locked);

  /* 在录用链中搜索id标识的插件信息块 */
  for (ui = 0, pb = ismf_info->puh; ui < (int)ismf_info->uc; ui++, pb = pb->next) {
    assert(pb);

    if ((int)pb->ifo.plug_id == id) {
      break;
    }
  }

  assert(pb);

  /* 从录用链中将插件信息块摘除 */
  _remove_node(ismf_info, 0, pb);

  /* 如果插件已被激活，则休眠（释放）它 */
  if (pb->act != 0) {
    assert(pb->act == 1);             /* act不可能大于1，因为信息表现处于锁定状态 */
    FreeLibrary(pb->handle);
  }

  /* 修正信息块内容 */
  pb->use     = IRWMF_INFOBLOCK_NOTUSE;     /* 设置未用标记 */

  pb->act     = 0;                /* 激活标记 */
  pb->handle    = 0;                /* 插件句柄 */
  pb->fn_get_img_info = 0;              /* get_image_info接口 */
  pb->fn_load_img   = 0;              /* load_image接口 */
  pb->fn_save_img   = 0;              /* save_image接口 */

  pb->prev      = 0;              /* 前一个信息块的地址 */
  pb->next      = 0;              /* 下一个信息块的地址 */
  pb->pe        = ismf_info;          /* 表头地址 */

  /* 加入未用链尾部 */
  if (ismf_info->pnt == 0) {            /* 第一个 */
    ismf_info->pnh = ismf_info->pnt = pb;
  }
  else {                      /* 后续 */
    pb->prev = ismf_info->pnt;
    ismf_info->pnt->next = pb;
    ismf_info->pnt = pb;
  }

  /* 未用插件个数累加 */
  ismf_info->nc++;
  ismf_info->plugin_num++;

  /* 刷新扩展名过滤串 */
  _isirw_update_filtrate_str();

  /* 成功返回0 */
  return 0;
}


/* 插件信息表解锁 */
int CALLAGREEMENT ismf_unlock(LPIRWMF_HEADER pt)
{
  assert((ismf_info) && (ismf_info->mark == 1) && ismf_info->locked);
  assert(pt == ismf_info);              /* 传入的参数必须是ismf_lock()传出的参数 */

  /* 设置锁定标志 */
  EnterCriticalSection(&ismf_cs);
  assert(ismf_info->locked == 1);           /* 解锁前信息表必须是已被锁定的 */
  ismf_info->locked--;
  LeaveCriticalSection(&ismf_cs);

  return 0;
}


/* 获取插件所在目录(path指向的缓冲区尺寸必须大于等于MAX_PATH的定义的尺寸) */
void CALLAGREEMENT ismf_get_plugin_path(char* path)
{
  assert((ismf_info) && (ismf_info->mark == 1));

  /* 抽取插件所在路径 */
  strcpy((char*)path, (const char*)ismf_info->infpath);
  *(char*)(strrchr((const char*)path, '\\')) = '\0';
}


/* 根据指定的图象文件名返回对应的插件句柄，此句柄将用于下面提取接口地址的三个函数 */
unsigned long CALLAGREEMENT ismf_get_plugin_heandle(const char* path)
{
  LPIRWMF_INFOBLOCK pib;
  char        path2[MAX_PATH];
  unsigned long   result = 0;

  assert(path);
  assert((ismf_info) && (ismf_info->mark == 1) && (ismf_info->plugin_num > 0));

  __try {
    /* 进入关键段 */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      __leave;                  /* 插件信息表被锁定 */
    }

    /* 查找能处理此文件的插件 */
    if (_search_plugin_info_block(&pib, (const char*)path) <= 0) {
      __leave;                  /* 指定的图象文件不能被任何一个插件处理 */
    }

    assert(pib && (pib->use == IRWMF_INFOBLOCK_USEING));

    /* 判断该插件是否已被激活 */
    switch (pib->act) {
    case  0:                    /* 未激活 */
      /* 合成插件全路径 */
      strcpy((char*)path2, (const char*)ismf_info->infpath);
      *(char*)(strrchr((const char*)path2, '\\') + 1) = '\0';
      strcat((char*)path2, (const char*)pib->path);

      /* 读入插件 */
      if ((pib->handle = LoadLibrary((LPCTSTR)path2)) == 0) {
        __leave;                /* 文件读写错误 */
      }

      /* 取得接口函数地址 */
      if (((pib->fn_get_img_info = (IRWP_API_GET_IMG_INFO)isirw_get_api_addr(pib->handle, &(pib->ifo), get_image_info)) == 0) || \
          ((pib->fn_load_img = (IRWP_API_LOAD_IMG)isirw_get_api_addr(pib->handle, &(pib->ifo), load_image)) == 0) || \
          ((pib->fn_save_img = (IRWP_API_SAVE_IMG)isirw_get_api_addr(pib->handle, &(pib->ifo), save_image)) == 0)) {
        /* 插件文件遭到破坏，舍弃该插件 */
        FreeLibrary(pib->handle);
        pib->handle = 0;
        __leave;                /* 插件遭到破坏 */
      }

      pib->act = 1;               /* 设置激活标志 */

      /* 此处没有break;语句，目的是让控制转向下面的计数器累加操作 */
    default:                    /* 已激活 */
      assert(pib->act >= 1);
      /* 使用计数加一（使用计数器在act成员变量的高16位，所以需要以下运算）*/
      pib->act = (((pib->act >> 16) + 1) << 16) | (pib->act & 0xffff);

      /* 本次使用计数器累加 */
      pib->pcr++;

      break;
    }

    /* 空闲处理命令发送计数器累加 */
    _ismf_idle_counter++;

    /* 如果满足了发送条件，则向消息泵发送空闲处理命令 */
    if (_ismf_idle_counter > IRWMF_IDLE_MAX_COUNT) {
      _ismf_idle_counter = 0;

      /* 当还有未处理的空闲命令时，将不再发送空闲命令（防止队列溢出）*/
      if (!get_idlesse_count()) {
        add_idlesse((void*)_ismf_idle);
      }
    }

    result = (unsigned long)pib;
  }
  __finally {
    /* 离开关键段 */
    LeaveCriticalSection(&ismf_cs);
  }

  return result;                      /* 成功找到对应的插件句柄（信息块地址）*/
}


/* 释放ismf_get_plugin_heandle()函数返回的插件句柄（这两个函数必须成对使用）*/
int CALLAGREEMENT ismf_free_plugin_heandle(unsigned long hand)
{
  LPIRWMF_INFOBLOCK pib = (LPIRWMF_INFOBLOCK)hand;
  int         result = 0;

  assert(ismf_info);
  assert((pib) && (pib->tag == IRWMF_INFOBLOCK_TAG)); /* 入口参数必须是有效的插件句柄 */

  __try {
    /* 进入关键段 */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      result = -3;
      __leave;             /* 插件信息表被锁定 */
    }

    assert(pib->act > 1);               /* 必须是激活并被使用的插件 */

    /* 使用计数减一（但并不负责使插件休眠）*/
    pib->act = (((pib->act >> 16) - 1) << 16) | (pib->act & 0xffff);
  }
  __finally {
    LeaveCriticalSection(&ismf_cs);
  }

  return result;
}


/* 根据给定的插件句柄，取得它的get_info函数接口地址 */
IRWP_API_GET_IMG_INFO CALLAGREEMENT ismf_get_info_addr(unsigned long hand)
{
  LPIRWMF_INFOBLOCK   pib = (LPIRWMF_INFOBLOCK)hand;
  IRWP_API_GET_IMG_INFO result = 0;

  assert(ismf_info);
  /* 入口参数必须是有效的插件句柄，并且必须是被激活并被引用的 */
  assert(pib && (pib->tag == IRWMF_INFOBLOCK_TAG) && (pib->act > 1));

  __try {
    /* 进入关键段 */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      result = 0;
      __leave;              /* 插件信息表被锁定 */
    }

    assert(pib->fn_get_img_info);

    result = pib->fn_get_img_info;
  }
  __finally {
    LeaveCriticalSection(&ismf_cs);
  }

  return result;
}


/* 根据给定的插件句柄，取得它的load函数接口地址 */
IRWP_API_LOAD_IMG CALLAGREEMENT ismf_get_load_addr(unsigned long hand)
{
  LPIRWMF_INFOBLOCK   pib = (LPIRWMF_INFOBLOCK)hand;
  IRWP_API_LOAD_IMG   result = 0;

  assert(ismf_info);
  /* 入口参数必须是有效的插件句柄，并且必须是被激活并被引用的 */
  assert(pib && (pib->tag == IRWMF_INFOBLOCK_TAG) && (pib->act > 1));

  __try {
    /* 进入关键段 */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      result = 0;
      __leave;              /* 插件信息表被锁定 */
    }

    assert(pib->fn_load_img);

    result = pib->fn_load_img;
  }
  __finally {
    LeaveCriticalSection(&ismf_cs);
  }

  return result;
}


/* 根据给定的插件句柄，取得它的save函数接口地址 */
IRWP_API_SAVE_IMG CALLAGREEMENT ismf_get_save_addr(unsigned long hand)
{
  LPIRWMF_INFOBLOCK   pib = (LPIRWMF_INFOBLOCK)hand;
  IRWP_API_SAVE_IMG   result = 0;

  assert(ismf_info);
  /* 入口参数必须是有效的插件句柄，并且必须是被激活并被引用的 */
  assert(pib && (pib->tag == IRWMF_INFOBLOCK_TAG) && (pib->act > 1));

  __try {
    /* 进入关键段 */
    EnterCriticalSection(&ismf_cs);

    if (ismf_info->locked > 0) {
      result = 0;
      __leave;              /* 插件信息表被锁定 */
    }

    result = pib->fn_save_img;
  }
  __finally {
    LeaveCriticalSection(&ismf_cs);
  }

  return result;
}


/* 关闭插件信息表 */
int CALLAGREEMENT ismf_close()
{
  FILE*        fp = 0;
  LPIRWMF_INFOBLOCK pii;
  int         result = 0, i;

  __try {
    __try {
      /* 如果没有内存INI，直接返回 */
      if (ismf_info == 0) {
        result = 1;
        __leave;
      }

      /* 在关闭系统前，插件信息表不能处于锁定状态 */
      assert(ismf_info->locked == 0);
      assert(ismf_info->mark == 1);

      /* 设置锁定标志－关闭过程将在锁定状态下进行 */
      EnterCriticalSection(&ismf_cs);
      ismf_info->locked++;

      /* 如果有插件处于激活状态，则使该插件休眠 */
      for (i = 0, pii = ismf_info->puh; i < (int)ismf_info->uc; i++, pii = pii->next) {
        assert(pii);

        /* 等待，直到该插件不被消息泵使用 */
        while (pii->act > 1) {
          Sleep(ISIRW_THREAD_IDLE);
        }

        /* 如果插件已被激活，则休眠（释放）它 */
        if (pii->act == 1) {
          FreeLibrary(pii->handle);

          pii->handle          = 0;
          pii->fn_get_img_info = 0;
          pii->fn_load_img     = 0;
          pii->fn_save_img     = 0;

          pii->act = 0;
        }
      }

      /* 打开文件INI */
      if ((fp = fopen((const char*)ismf_info->infpath, "wb")) == 0) {
        result = -1;
        __leave;         /* 打开文件错 */
      }

      /* 写入表头 */
      if (fwrite((const void*)ismf_info, sizeof(IRWMF_HEADER), 1, fp) == 0) {
        result = -2;
        __leave;         /* 文件写错误 */
      }

      /* 写入录用链 */
      for (i = 0, pii = ismf_info->puh; i < ismf_info->uc; i++, pii = pii->next) {
        assert(pii);

        /* 刷新调入优先级 */
        pii->pr += (pii->pcr > 0) ? 1 : 0;

        /* 写入录用项 */
        if (fwrite((const void*)pii, sizeof(IRWMF_INFOBLOCK), 1, fp) == 0) {
          result = -2;
          __leave;       /* 文件写错误 */
        }
      }

      assert(pii == 0);

      /* 写入未用链 */
      for (i = 0, pii = ismf_info->pnh; i < ismf_info->nc; i++, pii = pii->next) {
        assert(pii);

        /* 写入未用项 */
        if (fwrite((const void*)pii, sizeof(IRWMF_INFOBLOCK), 1, fp) == 0) {
          result = -2;
          __leave;       /* 文件写错误 */
        }
      }
    }
    __finally {
      if (fp) {
        fclose(fp);
      }

      /* 释放信息表 */
      _ismf_free_IRWMF_HEADER(ismf_info);
      ismf_info = 0;

      /* 删除信息表访问关键段 */
      LeaveCriticalSection(&ismf_cs);
      DeleteCriticalSection(&ismf_cs);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = -3;                  /* 系统异常 */
  }

  return result;
}


/***********************************************************************/

/* 分配并初始化一个插件表项 */
LPIRWMF_INFOBLOCK _ismf_alloc_IRWMF_INFOBLOCK()
{
  LPIRWMF_INFOBLOCK pt;

  if ((pt = (LPIRWMF_INFOBLOCK)im_malloc(sizeof(IRWMF_INFOBLOCK))) == 0) {
    return 0;
  }

  memset((void*)pt, 0, sizeof(IRWMF_INFOBLOCK));

  pt->tag = IRWMF_INFOBLOCK_TAG;

  return pt;
}


/* 释放一个插件表项 */
void _ismf_free_IRWMF_INFOBLOCK(LPIRWMF_INFOBLOCK pt)
{
  assert(pt && (pt->tag == IRWMF_INFOBLOCK_TAG));

  im_free(pt);
}


/* 申请插件信息表 */
LPIRWMF_HEADER _ismf_alloc_IRWMF_HEADER()
{
  LPIRWMF_HEADER  pt;

  if ((pt = (LPIRWMF_HEADER)im_malloc(sizeof(IRWMF_HEADER))) == 0) {
    return 0;
  }

  memset((void*)pt, 0, sizeof(IRWMF_HEADER));

  pt->tag = IRWMF_HEADER_TAG;

  return pt;
}


/* 释放插件信息表 */
void _ismf_free_IRWMF_HEADER(LPIRWMF_HEADER pt)
{
  int         i;
  LPIRWMF_INFOBLOCK p;

  assert(pt && (pt->tag == IRWMF_HEADER_TAG));

  /* 释放录用链 */
  for (i = 0; i < pt->uc; i++) {
    p = pt->puh;
    pt->puh = pt->puh->next;

    assert(p);

    p->tag = IRWMF_INFOBLOCK_TAG;         /* 使出现错误的块也能被正常释放 */

    _ismf_free_IRWMF_INFOBLOCK(p);
  }

  assert(pt->puh == 0);

  /* 释放未录用链 */
  for (i = 0; i < (int)pt->nc; i++) {
    p = pt->pnh;
    pt->pnh = pt->pnh->next;

    assert(p);

    p->tag = IRWMF_INFOBLOCK_TAG;

    _ismf_free_IRWMF_INFOBLOCK(p);
  }

  assert(pt->pnh == 0);

  /* 释放表头 */
  im_free(pt);
}


/***********************************************************************/


/* 创建新的内存INI映像 */
int _create_mini(char* path_str/* 插件路径 */, LPIRWMF_HEADER pt/* 表头结构 */)
{
  LPIRWP_INFO     pii = 0;
  LPIRWMF_INFOBLOCK pifb = 0;
  char        path[MAX_PATH];
  char        path2[MAX_PATH];
  int         result = 0, i;

  WIN32_FIND_DATA   file;
  HANDLE        hFind = 0;
  HMODULE       hDll  = 0;


  /* 调用者必须给出路径 */
  assert(strlen(path_str) > 1);
  assert(pt);

  /* 生成搜索串 */
  strcpy((char*)path, (const char*)path_str);
  strcat((char*)path, (const char*)"*.dll");

  __try {
    /* 搜索插件 */
    hFind = FindFirstFile((LPCTSTR)path, &file);

    if (INVALID_HANDLE_VALUE == hFind) {
      result = 1;
      __leave;            /* 没有找到图象读写插件 */
    }

    do {
      /* 合成插件全路径 */
      strcpy((char*)path2, (const char*)path_str);
      strcat((char*)path2, (const char*)(file.cFileName));

      /* 读入插件 */
      if ((hDll = LoadLibrary((LPCTSTR)path2)) == 0) {
        result = 2;
        __leave;          /* 插件读入错误 */
      }

      /* 判断此DLL是否是图象读写插件 */
      if ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0) {
        FreeLibrary(hDll);
        hDll = 0;
        continue;               /* 此DLL不是插件，继续 */
      }

      /* 分配临时用信息块缓冲区 */
      if ((pifb = _ismf_alloc_IRWMF_INFOBLOCK()) == 0) {
        result = 3;
        __leave;          /* 内存不足 */
      }

      /* 填写信息块 */
      pifb->pr    = IRWMF_INFOBLOCK_DEFPIR; /* 插件调入优先级 */
      pifb->use   = IRWMF_INFOBLOCK_USEING; /* 录用标记（缺省为已录用）*/
      pifb->pilen   = file.nFileSizeLow;    /* 插件文件长度 */
      /* 插件文件创建时间 */
      pifb->ctimeh  = file.ftCreationTime.dwHighDateTime;
      pifb->ctimel  = file.ftCreationTime.dwLowDateTime;
      /* 插件文件最后的修改时间 */
      pifb->mtimeh  = file.ftLastWriteTime.dwHighDateTime;
      pifb->mtimel  = file.ftLastWriteTime.dwLowDateTime;

      /* 插件文件名 */
      strcpy((char*)pifb->path, (const char*)file.cFileName);
      /* 插件信息表 */
      memcpy((void*) & (pifb->ifo), (const void*)pii, sizeof(IRWP_INFO));

      pifb->act   = 0;            /* 激活标记（缺省为未激活）*/
      pifb->handle  = 0;            /* 插件句柄 */

      pifb->fn_get_img_info = 0;        /* get_image_info接口 */
      pifb->fn_load_img   = 0;        /* load_image接口 */
      pifb->fn_save_img   = 0;        /* save_image接口 */

      pifb->prev    = 0;            /* 前一个信息块的地址 */
      pifb->next    = 0;            /* 下一个信息块的地址 */
      pifb->pe    = pt;           /* 表头地址 */

      /* 加入表头结构中的录用链 */
      if (pt->put == 0) {           /* 第一个 */
        pt->puh = pt->put = pifb;
      }
      else {                  /* 后续 */
        pifb->prev = pt->put;
        pt->put->next = pifb;
        pt->put = pifb;
      }

      pt->uc++;

      /* 释放插件DLL */
      FreeLibrary(hDll);

      hDll = 0;
      pifb = 0;

    }
    while (FindNextFile(hFind, &file));

    FindClose(hFind);

    hFind = 0;

    if (pt->uc == 0) {
      result = 1;
      __leave;            /* 无图象读写插件 */
    }

    pt->plugin_num = pt->uc;            /* 插件总个数 */

    pt->pnh     = 0;              /* 未用链头地址 */
    pt->pnt     = 0;              /* 未用链尾地址 */
    pt->nc      = 0;              /* 未用链中项的个数 */

    strcpy((char*)pt->infpath, (const char*)path_str); /* 信息文件全路径 */
    strcat((char*)pt->infpath, (const char*)IRWMF_FILENAME);

    pt->locked    = 0;              /* 初始化锁定标志 */
    pt->build   = IRWMF_HEADER_BUD_NULL;    /* 暂设为无效类型 */
    pt->mark    = 0;              /* 表头信息有效标记暂时设为0 */
  }
  __finally {
    if (pifb) {
      assert(pifb->tag == IRWMF_INFOBLOCK_TAG);
      _ismf_free_IRWMF_INFOBLOCK(pifb);
    }

    if (hDll) {
      FreeLibrary(hDll);
    }

    if (hFind) {
      FindClose(hFind);
    }

    if (AbnormalTermination() || (result != 0)) {
      /* 释放录用链 */
      for (i = 0; i < pt->uc; i++) {
        pifb = pt->puh;
        pt->puh = pt->puh->next;

        assert(pifb);
        assert(pifb->tag == IRWMF_INFOBLOCK_TAG);

        _ismf_free_IRWMF_INFOBLOCK(pifb);
      }

      assert(pt->puh == 0);

      pt->put = pt->puh = 0;
      pt->uc = 0;
      pt->plugin_num = 0;
    }
  }

  return result;
}


/* 读入信息表，并形成内存INI映像 */
int _load_mini(FILE* fp/* 插件ini文件句柄 */, LPIRWMF_HEADER pt)
{
  LPIRWMF_INFOBLOCK pifb = 0;
  int         result = 0, i, uc, nc, pln;

  assert(fp && pt);

  __try {
    if (fseek(fp, 0, SEEK_SET)) {
      result = 1;
      __leave;          /* 文件错 */
    }

    /* 读表头结构 */
    if (fread((void*)pt, sizeof(IRWMF_HEADER), 1, fp) == 0) {
      result = 1;
      __leave;
    }

    /* 初始化部分参数 */
    pt->puh = pt->put = 0;
    pt->pnh = pt->pnt = 0;

    pt->locked = 0;               /* 初始化锁定标志 */
    pt->build  = IRWMF_HEADER_BUD_NULL;     /* 暂设为无效类型 */
    pt->mark   = 0;               /* 表头信息有效标记暂时设为0 */

    /* 提取插件个数信息 */
    uc  = pt->uc;
    nc  = pt->nc;
    pln = pt->plugin_num;

    pt->uc = 0;
    pt->nc = 0;
    pt->plugin_num = 0;

    /* 检验信息的合法性 */
    if ((pt->tag != IRWMF_HEADER_TAG) || (pln <= 0)) {
      result = 2;
      __leave;          /* 文件内容遭到破坏 */
    }

    /* 读取录用链 */
    for (i = 0; i < uc; i++) {
      if ((pifb = _ismf_alloc_IRWMF_INFOBLOCK()) == 0) {
        result = 3;
        __leave;        /* 内存不足*/
      }

      /* 读入一个信息块 */
      if (fread((void*)pifb, sizeof(IRWMF_INFOBLOCK), 1, fp) == 0) {
        result = 1;
        __leave;        /* 读文件错 */
      }

      /* 检查信息块内容的合法性 */
      if (pifb->tag != IRWMF_INFOBLOCK_TAG) {
        result = 2;
        __leave;
      }

      pifb->pcr     = 0;
      pifb->use     = IRWMF_INFOBLOCK_USEING;
      pifb->act     = 0;        /* 激活标记 */
      pifb->handle    = 0;        /* 插件句柄 */

      pifb->fn_get_img_info = 0;        /* get_image_info接口 */
      pifb->fn_load_img = 0;        /* load_image接口 */
      pifb->fn_save_img = 0;        /* save_image接口 */

      pifb->prev = pifb->next = 0;      /* 初始化链表地址 */

      /* 并入录用链 */
      if (pt->put == 0) {         /* 第一个信息块 */
        pt->puh = pt->put = pifb;
      }
      else {                /* 后续信息块 */
        pifb->prev = pt->put;
        pt->put->next = pifb;
        pt->put = pifb;
      }

      pifb->pe = pt;              /* 表头地址 */
      pt->uc++;
      pt->plugin_num++;           /* 插件个数累加 */

      pifb = 0;
    }

    /* 读取未用链 */
    for (i = 0; i < nc; i++) {
      if ((pifb = _ismf_alloc_IRWMF_INFOBLOCK()) == 0) {
        result = 3;
        __leave;        /* 内存不足*/
      }

      /* 读入一个信息块 */
      if (fread((void*)pifb, sizeof(IRWMF_INFOBLOCK), 1, fp) == 0) {
        result = 1;
        __leave;        /* 读文件错 */
      }

      /* 检查信息块内容的合法性 */
      if (pifb->tag != IRWMF_INFOBLOCK_TAG) {
        result = 2;
        __leave;
      }

      pifb->pcr     = 0;
      pifb->use     = IRWMF_INFOBLOCK_NOTUSE;
      pifb->act     = 0;        /* 激活标记 */
      pifb->handle    = 0;        /* 插件句柄 */

      pifb->fn_get_img_info = 0;        /* get_image_info接口 */
      pifb->fn_load_img = 0;        /* load_image接口 */
      pifb->fn_save_img = 0;        /* save_image接口 */

      pifb->prev = pifb->next = 0;      /* 初始化链表地址 */

      /* 并入未用链 */
      if (pt->pnt == 0) {         /* 第一个信息块 */
        pt->pnh = pt->pnt = pifb;
      }
      else {                /* 后续信息块 */
        pifb->prev = pt->pnt;
        pt->pnt->next = pifb;
        pt->pnt = pifb;
      }

      pifb->pe = pt;              /* 表头地址 */
      pt->nc++;
      pt->plugin_num++;           /* 插件个数累加 */

      pifb = 0;
    }
  }
  __finally {
    if (pifb) {
      pifb->tag = IRWMF_INFOBLOCK_TAG;    /* 使出现错误的块也能被正常释放 */
      _ismf_free_IRWMF_INFOBLOCK(pifb);
    }

    if ((result != 0) || AbnormalTermination()) {
      /* 释放录用链 */
      for (i = 0; i < pt->uc; i++) {
        pifb = pt->puh;
        pt->puh = pt->puh->next;

        assert(pifb);
        assert(pifb->tag == IRWMF_INFOBLOCK_TAG);

        _ismf_free_IRWMF_INFOBLOCK(pifb);
      }

      assert(pt->puh == 0);

      /* 释放未录用链 */
      for (i = 0; i < (int)pt->nc; i++) {
        pifb = pt->pnh;
        pt->pnh = pt->pnh->next;

        assert(pifb);
        assert(pifb->tag == IRWMF_INFOBLOCK_TAG);

        _ismf_free_IRWMF_INFOBLOCK(pifb);
      }

      assert(pt->pnh == 0);

      pt->put = pt->puh = 0;
      pt->uc  = 0;
      pt->pnt = pt->pnh = 0;
      pt->nc  = 0;

      pt->plugin_num = 0;
    }
  }

  return result;
}


/* 校验内存ini的真实性，如有必要则更新内存ini */
void _verify_mini(LPIRWMF_HEADER phead, char* path, LPUPDATAINFO rinfo)
{
  LPIRWMF_INFOBLOCK pifb;
  LPIRWMF_INFOBLOCK ptmp;
  LPIRWP_INFO     pii = 0;

  HMODULE       hDll  = 0;
  char        path2[MAX_PATH];
  int         i;

  WIN32_FILE_ATTRIBUTE_DATA wfa;


  assert(phead && path);

  __try {
    /* 获得录用链头块地址 */
    pifb = phead->puh;

    while (pifb) {
      /* 合成待校验插件文件的全路径 */
      strcpy((char*)path2, (const char*)path);
      strcat((char*)path2, (const char*)pifb->path);

      /* 获取插件文件的信息 */
      if (GetFileAttributesEx((LPCSTR)path2, GetFileExInfoStandard, (LPVOID)&wfa) == 0) {
        if (pifb->act) {
          /* 如果该插件是激活的，则先使其休眠 */
          FreeLibrary(pifb->handle);
          pifb->handle = 0;
          pifb->act = 0;
          pifb->fn_get_img_info = 0;
          pifb->fn_load_img     = 0;
          pifb->fn_save_img     = 0;
        }

        /* 对应的插件文件没有找到，可能被用户私自删除 */
        ptmp = pifb;
        pifb = pifb->next;

        /* 将节点从录用链中摘除 */
        _remove_node(phead, 0, ptmp);

        assert(ptmp->tag == IRWMF_INFOBLOCK_TAG);

        /* 记录被删除插件的名字 */
        if (rinfo) {
          strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(ptmp->ifo.irwp_name));
        }

        /* 释放信息块 */
        _ismf_free_IRWMF_INFOBLOCK(ptmp);

        continue;
      }
      else {
        i = 0;

        /* 如果插件文件被改动（升级），则刷新信息 */
        if (pifb->pilen != wfa.nFileSizeLow) {
          i++;
          pifb->pilen = wfa.nFileSizeLow;
        }

        if ((pifb->ctimel != wfa.ftCreationTime.dwLowDateTime) || \
            (pifb->ctimeh != wfa.ftCreationTime.dwHighDateTime)) {
          i++;
          pifb->ctimel = wfa.ftCreationTime.dwLowDateTime;
          pifb->ctimeh = wfa.ftCreationTime.dwHighDateTime;
        }

        if ((pifb->mtimel != wfa.ftLastWriteTime.dwLowDateTime) || \
            (pifb->mtimeh != wfa.ftLastWriteTime.dwHighDateTime)) {
          i++;
          pifb->mtimel = wfa.ftLastWriteTime.dwLowDateTime;
          pifb->mtimeh = wfa.ftLastWriteTime.dwHighDateTime;
        }

        if (i) {
          /* 调入操作系统的插件文件是不可能被修改的 */
          assert(pifb->act == 0);

          /* 判断此DLL是否是图象读写插件 */
          if (((hDll = LoadLibrary((LPCTSTR)path2)) == 0) || \
              ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0)) {
            /* 插件文件遭到破坏，舍弃该插件 */
            if (hDll) {
              FreeLibrary(hDll);
              hDll = 0;
            }

            ptmp = pifb;
            pifb = pifb->next;

            /* 将节点从录用链中摘除 */
            _remove_node(phead, 0, ptmp);

            assert(ptmp->tag == IRWMF_INFOBLOCK_TAG);

            /* 记录被删除插件的名字 */
            if (rinfo) {
              strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(ptmp->ifo.irwp_name));
            }

            /* 释放信息块 */
            _ismf_free_IRWMF_INFOBLOCK(ptmp);

            continue;
          }
          else {
            /* 取得新的插件信息 */
            memcpy((void*) & (pifb->ifo), (const void*)pii, sizeof(IRWP_INFO));

            /* 记录升级插件的信息块地址 */
            if (rinfo) {
              ((IRWMF_INFOBLOCK**)rinfo->updpp)[rinfo->updcnt++] = pifb;
            }

            FreeLibrary(hDll);
            hDll = 0;
          }
        }
      }

      pifb = pifb->next;
    }


    /* 获得未用链头块地址 */
    pifb = phead->pnh;

    while (pifb) {
      /* 合成待校验插件文件的全路径 */
      strcpy((char*)path2, (const char*)path);
      strcat((char*)path2, (const char*)pifb->path);

      /* 获取插件文件的信息 */
      if (GetFileAttributesEx((LPCSTR)path2, GetFileExInfoStandard, (LPVOID)&wfa) == 0) {
        /* 未用链中的对应插件文件是不可能被激活的 */
        assert(pifb->act == 0);

        /* 对应的插件文件没有找到，可能被用户私自删除 */
        ptmp = pifb;
        pifb = pifb->next;

        /* 将节点从未用链中摘除 */
        _remove_node(phead, 1, ptmp);

        assert(ptmp->tag == IRWMF_INFOBLOCK_TAG);

        /* 记录被删除插件的名字 */
        if (rinfo) {
          strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(ptmp->ifo.irwp_name));
        }

        /* 释放信息块 */
        _ismf_free_IRWMF_INFOBLOCK(ptmp);

        continue;
      }
      else {
        i = 0;

        /* 如果插件文件被改动（升级），则刷新信息 */
        if (pifb->pilen != wfa.nFileSizeLow) {
          i++;
          pifb->pilen = wfa.nFileSizeLow;
        }

        if ((pifb->ctimel != wfa.ftCreationTime.dwLowDateTime) || \
            (pifb->ctimeh != wfa.ftCreationTime.dwHighDateTime)) {
          i++;
          pifb->ctimel = wfa.ftCreationTime.dwLowDateTime;
          pifb->ctimeh = wfa.ftCreationTime.dwHighDateTime;
        }

        if ((pifb->mtimel != wfa.ftLastWriteTime.dwLowDateTime) || \
            (pifb->mtimeh != wfa.ftLastWriteTime.dwHighDateTime)) {
          i++;
          pifb->mtimel = wfa.ftLastWriteTime.dwLowDateTime;
          pifb->mtimeh = wfa.ftLastWriteTime.dwHighDateTime;
        }

        if (i) {
          /* 未用链中的对应插件文件是不可能被激活的 */
          assert(pifb->act == 0);

          /* 判断此DLL是否是图象读写插件 */
          if (((hDll = LoadLibrary((LPCTSTR)path2)) == 0) || \
              ((pii = isirw_get_info(isirw_test_plugin(hDll))) == 0)) {
            /* 插件文件遭到破坏，舍弃该插件 */
            if (hDll) {
              FreeLibrary(hDll);
              hDll = 0;
            }

            ptmp = pifb;
            pifb = pifb->next;

            /* 将节点从未用链中摘除 */
            _remove_node(phead, 1, ptmp);

            assert(ptmp->tag == IRWMF_INFOBLOCK_TAG);

            /* 记录被删除插件的名字 */
            if (rinfo) {
              strcpy((char*)(rinfo->delpp[rinfo->delcnt++]), (const char*)(ptmp->ifo.irwp_name));
            }

            /* 释放信息块 */
            _ismf_free_IRWMF_INFOBLOCK(ptmp);

            continue;
          }
          else {
            /* 取得新的插件信息 */
            memcpy((void*) & (pifb->ifo), (const void*)pii, sizeof(IRWP_INFO));

            /* 记录升级插件的信息块地址 */
            if (rinfo) {
              ((IRWMF_INFOBLOCK**)rinfo->updpp)[rinfo->updcnt++] = pifb;
            }

            FreeLibrary(hDll);
            hDll = 0;
          }
        }
      }

      pifb = pifb->next;
    }
  }
  __finally {
    if (hDll) {
      FreeLibrary(hDll);
    }
  }
}


/* 摘除录用链或未用链中的节点（但并不负责释放该节点） */
LPIRWMF_INFOBLOCK _remove_node(LPIRWMF_HEADER phead, int type, LPIRWMF_INFOBLOCK pnode)
{
  assert(phead && pnode);

  if (type == 0) {                  /* 录用节点 */
    if (phead->puh == pnode) {
      phead->puh = pnode->next;
    }

    if (phead->put == pnode) {
      phead->put = pnode->prev;
    }

    if (pnode->prev) {
      pnode->prev->next = pnode->next;
    }

    if (pnode->next) {
      pnode->next->prev = pnode->prev;
    }

    phead->uc --;                 /* 节点计数减1 */
    phead->plugin_num --;
  }
  else {                      /* 未用节点 */
    if (phead->pnh == pnode) {
      phead->pnh = pnode->next;
    }

    if (phead->pnt == pnode) {
      phead->pnt = pnode->prev;
    }

    if (pnode->prev) {
      pnode->prev->next = pnode->next;
    }

    if (pnode->next) {
      pnode->next->prev = pnode->prev;
    }

    phead->nc --;                 /* 节点计数减1 */
    phead->plugin_num --;
  }

  return pnode;
}


/* 根据提供的文件路径串来搜索对应插件的信息块地址 */
int _search_plugin_info_block(
    LPIRWMF_INFOBLOCK* ppblock/* out */,
    const char* path/* in */)
{
  LPIRWMF_INFOBLOCK ptmp;
  int         i, l, j;
  char        ext[_MAX_EXT];
  const char*      p = path;

  assert(ppblock && path);
  assert(ismf_info);

  /* 调用本函数时，信息表不能处于锁定状态 */
  if (ismf_info->locked > 0) {
    assert(0);
    *ppblock = 0;
    return 0;
  }

  l = strlen((const char*)path);

  /* 如果有文件扩展名的话，l值必将大于等于3(x.x)，否则直接返回 */
  if (l < 3) {
    *ppblock = 0;
    return 0;
  }

  ext[0] = '\0';

  /* 提取文件扩展名 */
  for (i = l - 1; i >= 0; i--) {
    if (*(p + i) == '.') {
      strcpy((char*)ext, (const char*)(p + i + 1)); /* 拷贝扩展名（不包括dot）*/
      break;
    }
  }

  l = strlen((const char*)ext);

  /* 如果没有找到扩展名或扩展名过长，则直接返回 */
  if ((l == 0) || (l >= IDI_CURRENCY_NAME_SIZE)) {
    *ppblock = 0;
    return 0;
  }


  /* 先搜索录用链 */
  for (i = 0, ptmp = ismf_info->puh; i < ismf_info->uc; i++, ptmp = ptmp->next) {
    assert(ptmp);

    /* 比较插件的通用扩展名 */
    if (_stricmp((const char*)ext, (const char*)ptmp->ifo.irwp_desc_info.idi_currency_name) == 0) {
      *ppblock = ptmp;
      return 1;                 /* 录用链中找到相关插件 */
    }

    /* 比较插件的衍生扩展名 */
    for (j = 0; j < (int)ptmp->ifo.irwp_desc_info.idi_synonym_count; j++) {
      if (_stricmp((const char*)ext, (const char*)ptmp->ifo.irwp_desc_info.idi_synonym[j]) == 0) {
        *ppblock = ptmp;
        return 1;
      }
    }
  }

  /* 再搜索未用链 */
  for (i = 0, ptmp = ismf_info->pnh; i < ismf_info->nc; i++, ptmp = ptmp->next) {
    assert(ptmp);

    /* 比较插件的通用扩展名 */
    if (_stricmp((const char*)ext, (const char*)ptmp->ifo.irwp_desc_info.idi_currency_name) == 0) {
      *ppblock = ptmp;
      return -1;                  /* 未用链中找到相关插件 */
    }

    /* 比较插件的衍生扩展名 */
    for (j = 0; j < (int)ptmp->ifo.irwp_desc_info.idi_synonym_count; j++) {
      if (_stricmp((const char*)ext, (const char*)ptmp->ifo.irwp_desc_info.idi_synonym[j]) == 0) {
        *ppblock = ptmp;
        return -1;
      }
    }
  }

  *ppblock = 0;

  return 0;                     /* 未能找到对应插件 */
}


/* 刷新OPEN对话框扩展名过滤串和通用扩展名过滤串 */
int _isirw_update_filtrate_str()
{
  static char     strbuf[ISIRW_OPENSTR_SIZE];
  char        buff[IDI_CURRENCY_NAME_SIZE];
  LPIRWMF_INFOBLOCK ptmp;
  int         i, j, count = 0;

  assert(ismf_info);

  /* 清空过滤串缓冲区 */
  memset((void*)_ismf_openstr, 0, ISIRW_OPENSTR_SIZE);
  memset((void*)_ismf_shellstr, 0, ISIRW_OPENSTR_SIZE);
  memset((void*)strbuf, 0, ISIRW_OPENSTR_SIZE);

  /* 只生成录用插件的扩展名串 */
  if (ismf_info->uc) {
    strcpy((char*)_ismf_openstr, (const char*)"所有图像文件 (");

    /* 只提取录用链中各插件的扩展名信息 */
    for (i = 0, ptmp = ismf_info->puh; i < ismf_info->uc; i++, ptmp = ptmp->next) {
      assert(ptmp);

      strcpy((char*)buff, (const char*)ptmp->ifo.irwp_desc_info.idi_currency_name);
      _strlwr((char*)buff);         /* 转换为小写 */

      /* 填充通用扩展名串 */
      strcat((char*)_ismf_shellstr, (const char*)"*.");
      strcat((char*)_ismf_shellstr, (const char*)buff);
      strcat((char*)_ismf_shellstr, (const char*)",");

      /* 填充OPEN对话框扩展名 */
      strcat((char*)strbuf, (const char*)"*.");
      strcat((char*)strbuf, (const char*)buff);
      strcat((char*)strbuf, (const char*)";");

      /* 累加扩展名个数 */
      count++;

      /* 处理衍生扩展名 */
      for (j = 0; j < (int)ptmp->ifo.irwp_desc_info.idi_synonym_count; j++) {
        strcpy((char*)buff, (const char*)ptmp->ifo.irwp_desc_info.idi_synonym[j]);
        _strlwr((char*)buff);

        strcat((char*)_ismf_shellstr, (const char*)"*.");
        strcat((char*)_ismf_shellstr, (const char*)buff);
        strcat((char*)_ismf_shellstr, (const char*)",");

        strcat((char*)strbuf, (const char*)"*.");
        strcat((char*)strbuf, (const char*)buff);
        strcat((char*)strbuf, (const char*)";");

        count++;
      }
    }

    /* 去掉各缓冲区末尾的';'和','。（通用串到此合成完毕）*/
    strbuf[strlen((const char*)strbuf) - 1] = '\0';
    _ismf_shellstr[strlen((const char*)_ismf_shellstr) - 1] = '\0';

    /* 继续填写OPEN串 */
    strcat((char*)_ismf_openstr, (const char*)strbuf);
    strcat((char*)_ismf_openstr, (const char*)")|");
    strcat((char*)_ismf_openstr, (const char*)strbuf);
    strcat((char*)_ismf_openstr, (const char*)"|");

    /* 填写各插件的扩展名信息 */
    for (i = 0, ptmp = ismf_info->puh; i < ismf_info->uc; i++, ptmp = ptmp->next) {
      strcpy((char*)buff, (const char*)ptmp->ifo.irwp_desc_info.idi_currency_name);
      _strlwr((char*)buff);           /* 转换为小写 */

      strcpy((char*)strbuf, (const char*)"*.");
      strcat((char*)strbuf, (const char*)buff);
      strcat((char*)strbuf, (const char*)";");

      strcat((char*)_ismf_openstr, (const char*)buff);  /* 格式："jpeg文件 (*.jpeg;*.jpg)|*.jpeg;*.jpg|" */
      strcat((char*)_ismf_openstr, (const char*)"文件 (*.");
      strcat((char*)_ismf_openstr, (const char*)buff);
      strcat((char*)_ismf_openstr, (const char*)";");

      /* 合并衍生扩展名 */
      for (j = 0; j < (int)ptmp->ifo.irwp_desc_info.idi_synonym_count; j++) {
        strcpy((char*)buff, (const char*)ptmp->ifo.irwp_desc_info.idi_synonym[j]);
        _strlwr((char*)buff);

        strcat((char*)strbuf, (const char*)"*.");
        strcat((char*)strbuf, (const char*)buff);
        strcat((char*)strbuf, (const char*)";");

        strcat((char*)_ismf_openstr, (const char*)"*.");
        strcat((char*)_ismf_openstr, (const char*)buff);
        strcat((char*)_ismf_openstr, (const char*)";");
      }

      /* 去除末尾的‘;’号 */
      strbuf[strlen((const char*)strbuf) - 1] = '\0';
      _ismf_openstr[strlen((const char*)_ismf_openstr) - 1] = '\0';

      strcat((char*)_ismf_openstr, (const char*)")|");
      strcat((char*)_ismf_openstr, (const char*)strbuf);
      strcat((char*)_ismf_openstr, (const char*)"|");
    }

    /* 填写‘所有’文件的扩展名 */
    strcat((char*)_ismf_openstr, (const char*)"所有文件 (*.*)|*.*||");

    /* 结尾双NULL */
    _ismf_openstr[strlen(_ismf_openstr) + 1] = '\0';
  }

  return count;                   /* 返回扩展名总个数 */
}


/* 获取OPEN对话框扩展名过滤串 */
char* CALLAGREEMENT _ismf_get_openstr()
{
  return (char*)_ismf_openstr;
}


/* 获取通用过滤串 */
char* CALLAGREEMENT _ismf_get_shellstr()
{
  return (char*)_ismf_shellstr;
}


/* 空闲处理函数 */
void _ismf_idle()
{
  static int      mark = 1;
  int         i, j;
  LPIRWMF_INFOBLOCK pstr;
  LPIRWMF_INFOBLOCK ptmp;
  LPIRWMF_INFOBLOCK phig;

  assert(ismf_info && (ismf_info->mark == 1));

  /* 锁定状态则不进行进一步的处理 */
  if (ismf_info->locked) {
    return;
  }

  pstr = ptmp = phig = 0;

  /* 为减少空闲函数的执行时间，分别的对录用链和未用链进行排序 */
  if (mark % 2) {
    /* 通过冒泡排序法对插件信息表录用链进行排序 */
    for (i = 0, pstr = ismf_info->puh; i < ismf_info->uc; i++) {
      for (j = i + 1, ptmp = pstr->next; j < ismf_info->uc; j++, ptmp = ptmp->next) {
        /* 根据本次使用计数排序 */
        if ((phig == 0) || (ptmp->pcr > phig->pcr)) {
          phig = ptmp;
        }
      }

      /* 排序完毕 */
      if (phig == 0) {
        break;
      }

      /* 将本次使用较多的插件信息块前提 */
      if (phig->pcr > pstr->pcr) {
        _remove_node(ismf_info, 0, phig);

        phig->prev = pstr->prev;
        phig->next = pstr;
        pstr->prev = phig;

        if (phig->prev) {
          phig->prev->next = phig;
        }

        if (ismf_info->puh == pstr) {
          ismf_info->puh = phig;
        }

        ismf_info->uc++;
        ismf_info->plugin_num++;
      }
      else {
        pstr = pstr->next;
      }

      phig = 0;
    }
  }
  else {
    /* 通过冒泡排序法对插件信息表未用链进行排序 */
    for (i = 0, pstr = ismf_info->pnh; i < ismf_info->nc; i++) {
      for (j = i + 1, ptmp = pstr->next; j < ismf_info->nc; j++, ptmp = ptmp->next) {
        /* 根据调入优先级排序 */
        if ((phig == 0) || (ptmp->pr > phig->pr)) {
          phig = ptmp;
        }
      }

      /* 排序完毕 */
      if (phig == 0) {
        break;
      }

      /* 将调入优先级较高的插件信息块前提 */
      if (phig->pr > pstr->pr) {
        _remove_node(ismf_info, 1, phig);

        phig->prev = pstr->prev;
        phig->next = pstr;
        pstr->prev = phig;

        if (phig->prev) {
          phig->prev->next = phig;
        }

        if (ismf_info->pnh == pstr) {
          ismf_info->pnh = phig;
        }

        ismf_info->nc++;
        ismf_info->plugin_num++;
      }
      else {
        pstr = pstr->next;
      }

      phig = 0;
    }
  }

  mark++;

  return;
}

