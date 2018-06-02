/********************************************************************

  lcd.h

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
  本文件用途：  ISee图像浏览器—LCD图像读写模块定义文件

          读取功能：读取自定义结构 -》按(8位一个字节)列对齐方式存储的
                液晶点阵数据

          保存功能：自定义结构 -》按(8位一个字节)列对齐方式存储的液晶
                点阵数据

  本文件编写人：zhaojinpo (zhaojinpo##sina.com)

  本文件版本：30902
  最后修改于：2003-9-2

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
  地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2003-9    第一次发布

********************************************************************/



#ifndef __LCD_MODULE_INC__
#define __LCD_MODULE_INC__

#ifndef __ISEEIO_INC__
#include "iseeio.h"                   /* 数据存储设备接口定义文件 */
#endif


/* 定义模块版本、名称、函数前缀、默认的文件名后缀 */
#define MODULE_BUILDID          22
#define MODULE_NAME           ("LCD插件")
#define MODULE_FUNC_PREFIX        ("lcd_")
#define MODULE_FILE_POSTFIX       ("lcd")

/* 文件判别标志 */


#ifdef WIN32  /* Windows */

# ifdef  LCD_EXPORTS
# define LCD_API __declspec(dllexport)
# else
# define LCD_API __declspec(dllimport)
# endif
# define CALLAGREEMENT  __cdecl

#else     /* Linux */

# define LCD_API
# define CALLAGREEMENT

#endif  /*WIN32*/



/* 文件结构定义 */

/* MAGIC定义 */
#define LCD_HEADER_MARKER 0x0064636c          //"LCD "

/* 数据块位置 */
#define LCD_data_offset   0x010

/* 点阵图文件头结构 （Windows）*/
typedef struct _tag_lcd_file_header {
  unsigned long bfType;
  unsigned long Size;
  unsigned short  width;
  unsigned short  hight;
  unsigned short  bSize;                //液晶现在采用双单字节存储
  unsigned char bwidth;               //液晶现在采用单字节存储
  unsigned char bhight;               //液晶现在采用单字节存储
} LCD_FILE_HEADER;

/* 数据存储格式：数据存储1字节代表一列上8个点。

   --------lcd_file_header-------------
   ----unsigned long  bfType;   lcd文件标示
   ----unsigned long  Size;   液晶图形存储数据大小（不含头结构长度）
   ----unsigned short width;    图形宽度
   ----unsigned short hight;    图形高度
   ----unsigned short bSize;      我的液晶现在采用双单字节存储
   ----unsigned char  bwidth;     我的液晶现在采用单字节存储
   ----unsigned char  bhight;     我的液晶现在采用单字节存储
   ---------lcd列存储数据-------------

   ----unsigned char  data;       液晶单字节按列存储数据

     字节存储第1个----》第11个由列取数

     像素排列第1行 ：|0|1| 2 3 4 5 6 7 8 9 10
     |1|
     |2|
     |3|
     |4|
     |5|
     |6|
     像素排列第8行 ：|7|

*/


#ifndef __ISEE_IRW_INC__
#include "iseeirw.h"                  /* 调用者数据接口定义文件 */
#endif

#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

#ifdef WIN32
  extern LCD_API LPIRWP_INFO  CALLAGREEMENT is_irw_plugin(void);
#else /* Linux */
  extern LCD_API LPIRWP_INFO  CALLAGREEMENT lcd_get_plugin_info(void);
  extern LCD_API int      CALLAGREEMENT lcd_init_plugin(void);
  extern LCD_API int      CALLAGREEMENT lcd_detach_plugin(void);
#endif  /* WIN32 */

  extern LCD_API int  CALLAGREEMENT lcd_get_image_info(PISADDR psct, LPINFOSTR pinfo_str);
  extern LCD_API int  CALLAGREEMENT lcd_load_image(PISADDR psct, LPINFOSTR pinfo_str);
  extern LCD_API int  CALLAGREEMENT lcd_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave);

#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif


#endif /* __LCD_MODULE_INC__ */
