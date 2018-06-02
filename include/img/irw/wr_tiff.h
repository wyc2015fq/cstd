/********************************************************************

  wr_tiff.h


  本文件用途：  TIFF图像写模块辅助定义文件

          保存功能：可将1、4、8、24位的单页或多页图象保存
                为对应的TIFF图象。

  本文件编写人：  YZ        yzfree##yeah.net

  本文件版本：  20905
  最后修改于：  2002-9-5

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
      地址收集软件。
    ----------------------------------------------------------------

    引用代码编写人及版权信息：
  ----------------------------------------------------------------
  代码类别：  TIFF编解代码库(LibTiff)   目录：libtiff

  Copyright (c) 1988-1997 Sam Leffler
  Copyright (c) 1991-1997 Silicon Graphics, Inc.

  Permission to use, copy, modify, distribute, and sell this software and
  its documentation for any purpose is hereby granted without fee, provided
  that (i) the above copyright notices and this permission notice appear in
  all copies of the software and related documentation, and (ii) the names of
  Sam Leffler and Silicon Graphics may not be used in any advertising or
  publicity relating to the software without the specific, prior written
  permission of Sam Leffler and Silicon Graphics.

  THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
  EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

  IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
  ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
  LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
  OF THIS SOFTWARE.
  ----------------------------------------------------------------

  修正历史：

    2002-9    插件接口升级至2.2。引入内存防护概念。
    2002-4    第一个发布版


********************************************************************/


#ifndef __WR_TIFF_INC__
#define __WR_TIFF_INC__


#ifndef __ISEE_IRW_INC__
#include "iseeirw.h"          /* 调用者数据接口定义文件 */
#endif


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

  /* 接口函数 */
  int tiff_write_master_img(TIFF* tif, INFOSTR* pinfo_str);
  int tiff_write_servant_img(TIFF* tif, SUBIMGBLOCK* psub);


#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif

#endif /* __WR_TIFF_INC__ */
