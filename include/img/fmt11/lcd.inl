/********************************************************************

  lcd.c

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
  本文件用途：  ISee图像浏览器—LCD图像读写模块实现文件

          读取功能：读取自定义结构 -》按(8位一个字节)列对齐方式存储的
                液晶点阵数据

          保存功能：自定义结构 -》按(8位一个字节)列对齐方式存储的液晶
                点阵数据（见.h文件中定义）

  本文件编写人：  zhaojinpo (zhaojinpo##sina.com)

  本文件版本：  30902
  最后修改于：  2003-9-2

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
      地址收集软件。
    ----------------------------------------------------------------
  修正历史：

    2003-9    第一次发布

********************************************************************/


#ifndef WIN32
#if defined(_WIN32)||defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */

/*###################################################################

  移植注释：以下代码使用了WIN32系统的SEH(结构化异常处理)及多线程同步
      对象“关键段”，在移植时应转为Linux的相应语句。

  #################################################################*/


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN       /* 缩短windows.h文件的编译时间 */
#include <windows.h>
#endif /* WIN32 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lcd.h"


IRWP_INFO     lcd_irwp_info;      /* 插件信息表 */

#ifdef WIN32
CRITICAL_SECTION  lcd_get_info_critical;  /* lcd_get_image_info函数的关键段 */
CRITICAL_SECTION  lcd_load_img_critical;  /* lcd_load_image函数的关键段 */
CRITICAL_SECTION  lcd_save_img_critical;  /* lcd_save_image函数的关键段 */
#else
/* Linux对应的语句 */
#endif


/* 内部助手函数 */
void CALLAGREEMENT _init_irwp_info(LPIRWP_INFO lpirwp_info);
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */);
int CALLAGREEMENT _lcd_is_valid_img(LPINFOSTR pinfo_str);
unsigned  char CALLAGREEMENT _get_byte_bit(unsigned char my_byte, unsigned char bit);


#ifdef WIN32
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    /* 初始化插件信息表 */
    _init_irwp_info(&lcd_irwp_info);

    /* 初始化访问关键段 */
    InitializeCriticalSection(&lcd_get_info_critical);
    InitializeCriticalSection(&lcd_load_img_critical);
    InitializeCriticalSection(&lcd_save_img_critical);

    break;

  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;

  case DLL_PROCESS_DETACH:
    /* 销毁访问关键段 */
    DeleteCriticalSection(&lcd_get_info_critical);
    DeleteCriticalSection(&lcd_load_img_critical);
    DeleteCriticalSection(&lcd_save_img_critical);
    break;
  }

  return TRUE;
}
#endif  /* WIN32 */



#ifdef WIN32

LCD_API LPIRWP_INFO CALLAGREEMENT is_irw_plugin()
{
  return (LPIRWP_INFO)&lcd_irwp_info;
}

#else

LCD_API LPIRWP_INFO CALLAGREEMENT lcd_get_plugin_info()
{
  _init_irwp_info(&lcd_irwp_info);

  return (LPIRWP_INFO)&lcd_irwp_info;
}

LCD_API void CALLAGREEMENT lcd_init_plugin()
{
  /* 初始化多线程同步对象 */
}

LCD_API void CALLAGREEMENT lcd_detach_plugin()
{
  /* 销毁多线程同步对象 */
}

#endif /* WIN32 */


/* 初始化插件信息结构 */
void CALLAGREEMENT _init_irwp_info(LPIRWP_INFO lpirwp_info)
{
  assert(lpirwp_info);

  /* 初始化结构变量 */
  memset((void*)lpirwp_info, 0, sizeof(IRWP_INFO));

  /* 版本号。（十进制值，十位为主版本号，个位为副版本，*/
  lpirwp_info->irwp_version = MODULE_BUILDID;
  /* 插件名称 */
  strcpy((char*)(lpirwp_info->irwp_name), MODULE_NAME);
  /* 本模块函数前缀 */
  strcpy((char*)(lpirwp_info->irwp_func_prefix), MODULE_FUNC_PREFIX);
  /* 填写插件标识 */
  lpirwp_info->plug_id = IMF_LCD;


  /* 插件的发布类型。0－调试版插件，1－发布版插件 */
#ifdef _DEBUG
  lpirwp_info->irwp_build_set = 0;
#else
  lpirwp_info->irwp_build_set = 1;
#endif


  /* 功能标识 （##需手动修正，如不支持保存功能请将IRWP_WRITE_SUPP标识去掉） */
  lpirwp_info->irwp_function = IRWP_READ_SUPP | IRWP_WRITE_SUPP;

  /* 设置模块支持的保存位深 */
  /* ################################################################# */
  /* 各位数减一，否则32位格式无法表示。此处请用LCD正确的位深填写！*/
  lpirwp_info->irwp_save.bitcount = 1UL;
  /* ################################################################# */

  lpirwp_info->irwp_save.img_num = 1; /* 1－只能保存一副图像 */
  /* 如需更多的设定参数，可修改此值（##需手动修正） */
  lpirwp_info->irwp_save.count = 0;

  /* 开发者人数（即开发者信息中有效项的个数）（##需手动修正）*/
  lpirwp_info->irwp_author_count = 1;


  /* 开发者信息（##需手动修正） */
  /* ---------------------------------[0] － 第一组 -------------- */
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_name),
      (const char*)"zhaojinpo");
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_email),
      (const char*)"zhaojinpo##sina.com");
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_message),
      (const char*)"液晶点阵提取程式");
  /* ---------------------------------[1] － 第二组 -------------- */
  /* 后续开发者信息可加在此处。
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_name),
        (const char *)"");
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_email),
        (const char *)"@");
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_message),
        (const char *)":)");
  */
  /* ------------------------------------------------------------- */


  /* 插件描述信息（扩展名信息）*/
  strcpy((char*)(lpirwp_info->irwp_desc_info.idi_currency_name),
      (const char*)MODULE_FILE_POSTFIX);

  lpirwp_info->irwp_desc_info.idi_rev = 0;

  /* 别名个数（##需手动修正，如LCD格式有别名请参考BMP插件中别名的设置） */
  lpirwp_info->irwp_desc_info.idi_synonym_count = 0;

  /* 设置初始化完毕标志 */
  lpirwp_info->init_tag = 1;

  return;
}



/* 获取图像信息 */
LCD_API int CALLAGREEMENT lcd_get_image_info(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  LCD_FILE_HEADER bfh;
  int       style;        /* 风格，0－WINDOWS，1－OS/2 */
  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */

  __try {
    __try {
      /* 进入关键段 */
      EnterCriticalSection(&lcd_get_info_critical);

      /* 打开指定流 */
      if ((pfile = isio_open((const char*)psct, "rb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read((void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (bfh.bfType == LCD_HEADER_MARKER) {
        style = 0;
      }
      else {
        b_status = ER_NONIMAGE;
        __leave;
      }

      /* 数据有效性检验 */
      if (style == 0) {
        if ((bfh.width == 0) || (bfh.hight == 0)) {
          b_status = ER_NONIMAGE;
          __leave;
        }
      }

      pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
      pinfo_str->imgformat  = IMF_LCD;      /* 图像文件格式（后缀名） */
      pinfo_str->compression  = ICS_RGB;

      pinfo_str->width  = bfh.width;
      pinfo_str->height = bfh.hight;
      pinfo_str->order  = 0;
      pinfo_str->bitcount = 1;

      pinfo_str->b_mask = 0x0;
      pinfo_str->g_mask = 0x0;
      pinfo_str->r_mask = 0x0;
      pinfo_str->a_mask = 0x0;


      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    __finally {
      if (pfile) {
        isio_close(pfile);
      }

      LeaveCriticalSection(&lcd_get_info_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    pinfo_str->data_state = 0;
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 读取图像位数据 */
LCD_API int CALLAGREEMENT lcd_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  LCD_FILE_HEADER bfh;
  int       style;          /* 风格，0－WINDOWS */
  unsigned long LCD_data_len;     /* LCD位数据尺寸 */
  int       i, linesize, line = 0; /* 当前行 */
  unsigned int  k;
  unsigned char tmp , j;
  unsigned char* pbit = (unsigned char*)0;
  unsigned char* p_pre_bit = (unsigned char*)0;
  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);    /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&lcd_load_img_critical);

      /* 调用系统回调函数 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 打开流 */
      if ((pfile = isio_open((const char*)psct, "rb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读文件定位 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read((void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = (pinfo_str->data_state == 1) ? ER_FILERWERR : ER_NONIMAGE;
        __leave;
      }

      /* 判断是否是有效的位图文件 */
      if (bfh.bfType == LCD_HEADER_MARKER) {
        style = 0;
      }
      else {
        b_status = ER_NONIMAGE;
        __leave;
      }

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        /* 数据有效性检验 */
        if (style == 0) {
          if ((bfh.width == 0) || (bfh.hight == 0)) {
            b_status = ER_NONIMAGE;
            __leave;
          }
        }

        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
        pinfo_str->imgformat  = IMF_LCD;      /* 图像文件格式（后缀名） */
        pinfo_str->compression  = ICS_RGB;

        pinfo_str->width  = bfh.width;
        pinfo_str->height = bfh.hight;
        pinfo_str->order  = 0;
        pinfo_str->bitcount = 1;

        pinfo_str->b_mask = 0x0;
        pinfo_str->g_mask = 0x0;
        pinfo_str->r_mask = 0x0;
        pinfo_str->a_mask = 0x0;

        pinfo_str->data_state = 1;
      }

      /* 设置调色板数据 */
      if (pinfo_str->bitcount <= 8) {
        pinfo_str->pal_count = 1UL << pinfo_str->bitcount;

        if (style == 0) {
          pinfo_str->palette[0] = 0xff00;   /* 绿色，最高字节置零 */
          pinfo_str->palette[1] = 0xffffff;
        }
      }
      else {
        pinfo_str->pal_count = 0;
      }

      /* 取得位数据长度 */
      LCD_data_len = isio_length(pfile) - 1;
      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* 设置图像个数 */
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg    = (LPSUBIMGBLOCK)0;


      assert(pinfo_str->p_bit_data == (unsigned char*)0);
      /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
      pinfo_str->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4);

      if (!pinfo_str->p_bit_data) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      assert(pinfo_str->pp_line_addr == (void**)0);
      /* 分配行首地址数组 */
      pinfo_str->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * pinfo_str->height);

      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 回调通知 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
      }

      /* 读取位数据到内存 */
      if (isio_seek(pfile, LCD_data_offset, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      pbit = pinfo_str->p_bit_data;

      /* 每次读取8行数据 分配8行数据缓冲区(其中列保护限制+8字节保护) */
      p_pre_bit = (unsigned char*)isirw_malloc(pinfo_str->width + 8);

      if (p_pre_bit == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      for (i = 0; i < (int)(pinfo_str->height);) {
        /* 读一组数据到缓冲区 */
        if (isio_read(p_pre_bit, pinfo_str->width, 1, pfile) == 0) {
          b_status = ER_FILERWERR;
          __leave;
        }

        j = 0;

        /* 逐行填充显示数据 */
        while (i < (int)(pinfo_str->height) && (j < 8)) {
          /* 由列抽取数据重新拼合为行数据 */
          for (k = 0; k < (int)(pinfo_str->width); k = k + 8) {
            tmp = (_get_byte_bit(*(p_pre_bit + k), j) << 7) | (_get_byte_bit(*(p_pre_bit + 1 + k), j) << 6) | (_get_byte_bit(*(p_pre_bit + 2 + k), j) << 5) | (_get_byte_bit(*(p_pre_bit + k + 3), j) << 4) | (_get_byte_bit(*(p_pre_bit + k + 4), j) << 3) | (_get_byte_bit(*(p_pre_bit + k + 5), j) << 2) | (_get_byte_bit(*(p_pre_bit + k + 6), j) << 1) | (_get_byte_bit(*(p_pre_bit + k + 7), j) << 0);
            *(unsigned char*)(pbit + (k >> 3)) = tmp;
          }

          i++;
          j++;

          /* 移动到新的位置4字节对齐 */
          pbit = (unsigned char*)(pbit + linesize);
        }
      }

      /* 清除内存 */
      isirw_free(p_pre_bit);

      p_pre_bit = 0;

      /* 一次行进度调用 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 主图象进度结束 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      pinfo_str->data_state = 2;
    }
    __finally {
      if (p_pre_bit) {
        isirw_free(p_pre_bit);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        if (pinfo_str->p_bit_data) {
          isirw_free(pinfo_str->p_bit_data);
          pinfo_str->p_bit_data = (unsigned char*)0;
        }

        if (pinfo_str->pp_line_addr) {
          isirw_free(pinfo_str->pp_line_addr);
          pinfo_str->pp_line_addr = (void**)0;
        }

        if (pinfo_str->data_state == 2) {
          pinfo_str->data_state = 1;  /* 自动降级 */
        }

        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
      }
      else {
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          break;

        case  2:
          b_status = ER_SYSERR;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }

        if (b_status != ER_SUCCESS) {
          if (pinfo_str->p_bit_data) {
            isirw_free(pinfo_str->p_bit_data);
            pinfo_str->p_bit_data = (unsigned char*)0;
          }

          if (pinfo_str->pp_line_addr) {
            isirw_free(pinfo_str->pp_line_addr);
            pinfo_str->pp_line_addr = (void**)0;
          }

          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1;
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&lcd_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 保存图像 */
LCD_API int CALLAGREEMENT lcd_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;
  LCD_FILE_HEADER bfh;
  unsigned char* p_pre_bit = (unsigned char*)0;
  int       i, k, linesize;

  typedef struct _tag_byte_bit {
    unsigned    bit0: 1;
    unsigned      bit1: 1;
    unsigned    bit2: 1;
    unsigned    bit3: 1;
    unsigned    bit4: 1;
    unsigned    bit5: 1;
    unsigned    bit6: 1;
    unsigned    bit7: 1;
  } BIT_DATA;

  union {
    unsigned char   uc_data;
    BIT_DATA    bit_data;
  } mylcd_bit;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&lcd_save_img_critical);

      /* 调用系统回调函数 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 判断是否是受支持的图像数据 */
      if (_lcd_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* 如果存在同名流，本函数将不进行确认提示 */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 填充文件头 */
      bfh.bfType = LCD_HEADER_MARKER;
      bfh.width  = (unsigned short)pinfo_str->width;
      bfh.hight  = (unsigned short)pinfo_str->height;
      bfh.Size   = pinfo_str->width * ((pinfo_str->height + 7) / 8);
      bfh.bwidth = (unsigned char)pinfo_str->width;
      bfh.bhight = (unsigned char)pinfo_str->height;
      bfh.bSize  = ((((unsigned short)bfh.Size) << 4) & 0x0f) | ((((unsigned short)bfh.Size) >> 4) & 0xf0);

      /* 写入文件头结构 */
      if (isio_write((const void*)&bfh, sizeof(LCD_FILE_HEADER), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* 行扩充为8的倍数--分配内存 */
      p_pre_bit = (unsigned char*)isirw_malloc(8L * linesize * ((pinfo_str->height + 7) / 8) + 4);

      if (p_pre_bit == 0) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      for (i = 0; i < (int)(pinfo_str->height); i++) {
        memmove((void*)(p_pre_bit + linesize * i), (const void*)(unsigned char*)(pinfo_str->pp_line_addr[i]), linesize);
      }

      /* 行-8行移动 */
      for (i = 0; i < (int)(pinfo_str->height); i = i + 8) {
        /* 列移动 */
        for (k = 0; k < (int)(pinfo_str->width); k++) {
          mylcd_bit.uc_data = 0;
          /* 由行抽取数据重新拼合为列数据 */
          mylcd_bit.bit_data.bit0 = _get_byte_bit(*(p_pre_bit + linesize * i + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit1 = _get_byte_bit(*(p_pre_bit + linesize * (i + 1) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit2 = _get_byte_bit(*(p_pre_bit + linesize * (i + 2) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit3 = _get_byte_bit(*(p_pre_bit + linesize * (i + 3) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit4 = _get_byte_bit(*(p_pre_bit + linesize * (i + 4) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit5 = _get_byte_bit(*(p_pre_bit + linesize * (i + 5) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit6 = _get_byte_bit(*(p_pre_bit + linesize * (i + 6) + k / 8) , (unsigned char)((7 - (k % 8))));
          mylcd_bit.bit_data.bit7 = _get_byte_bit(*(p_pre_bit + linesize * (i + 7) + k / 8) , (unsigned char)((7 - (k % 8))));

          if (isio_write((const void*)&mylcd_bit.uc_data, 1, 1, pfile) == 0) {
            b_status = ER_FILERWERR;
            __leave;
          }
        }
      }

      /* 清除内存 */
      isirw_free(p_pre_bit);

      p_pre_bit = 0;

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_SAVE)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }
    }
    __finally {
      if (p_pre_bit) {
        isirw_free(p_pre_bit);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&lcd_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}





/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 判断传入的图像是否可以被保存（如不支持保存功能，可将本函数去掉） */
int CALLAGREEMENT _lcd_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* 位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(lcd_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* 不支持的位深图像 */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (lcd_irwp_info.irwp_save.img_num) {
    if (lcd_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* 图像个数不正确 */
      }
    }
  }

  return 0;
}


unsigned  char CALLAGREEMENT _get_byte_bit(unsigned char my_byte, unsigned char bit)
{
  unsigned char result = 0;

  typedef struct _tag_byte_bit {
    unsigned    bit0: 1;
    unsigned      bit1: 1;
    unsigned    bit2: 1;
    unsigned    bit3: 1;
    unsigned    bit4: 1;
    unsigned    bit5: 1;
    unsigned    bit6: 1;
    unsigned    bit7: 1;
  } BIT_DATA;

  union {
    unsigned char   uc_data;
    BIT_DATA    bit_data;
  } my_bit;

  my_bit.uc_data = my_byte;

  switch (bit) {
  case 0x0:
    result = my_bit.bit_data.bit0;
    break;

  case 0x1:
    result = my_bit.bit_data.bit1;
    break;

  case 0x2:
    result = my_bit.bit_data.bit2;
    break;

  case 0x3:
    result = my_bit.bit_data.bit3;
    break;

  case 0x4:
    result = my_bit.bit_data.bit4;
    break;

  case 0x5:
    result = my_bit.bit_data.bit5;
    break;

  case 0x6:
    result = my_bit.bit_data.bit6;
    break;

  case 0x7:
    result = my_bit.bit_data.bit7;
    break;
  }

  return result;  /* 位数据 */
}