/********************************************************************

  iseecnv.h

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
  本文件用途：  图像转换模块定义文件
  本文件编写人：
          YZ        yzfree##sina.com

  本文件版本：  20822
  最后修改于：  2002-8-22

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
  地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2002-8    修正位图转调色板图、及低位图转高位图时的BUG。
    2002-8    将转换引擎并入图象读写引擎，代码全部重写。
    2002-4    修正格式转换函数中颜色分量重叠的BUG，发现者是
          小组成员 tearshark。
    2001-12   小组成员 menghui 发现格式转换函数中缺少对多页
          图象的支持。（此BUG尚未修正）
    2001-7    发布第一个测试版

********************************************************************/


#ifndef __ISEECNV_INC__
#define __ISEECNV_INC__


#ifndef WIN32
#if defined(_WIN32) || defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */


#ifdef WIN32  /* Windows OS */

# define CALLAGREEMENT  __cdecl

#else     /* Linux OS */

# define CALLAGREEMENT

#endif  /*WIN32*/


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

  int CALLAGREEMENT  iscnv_is_std(int, unsigned long, unsigned long, unsigned long, unsigned long);
  int CALLAGREEMENT  iscnv_match(int);
  int CALLAGREEMENT  iscnv_idtobit(int);
  LPPALITEM CALLAGREEMENT iscnv_get_pal(int);

  void CALLAGREEMENT iscnv_get_mask(int, unsigned long*, unsigned long*, unsigned long*, unsigned long*);
  int CALLAGREEMENT  iscnv_cnvfun(int, int, unsigned char*, LPPALITEM, int, unsigned char*, LPPALITEM, \
      int, int, int, unsigned long, unsigned long, unsigned long, unsigned long);

  int CALLAGREEMENT  iscnv_get_id_info(int, unsigned long*, unsigned long*, unsigned long*, unsigned long*, \
      unsigned long*, int*, LPPALITEM*);

#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif


#endif /* __ISEECNV_INC__ */
