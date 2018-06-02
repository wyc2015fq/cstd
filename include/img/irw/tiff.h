/********************************************************************

  tiff.h


  本文件用途：  TIFF图像读写模块定义文件

          读取功能：可读以 调色板、灰度、RGB、YCbCr 方式存储的
                TIFF图象（包括多页图象），但不支持位深度
                超过32位的图象。

          保存功能：可将1、4、8、24位的单页或多页图象保存为对
                应的TIFF图象。


  本文件编写人：  YZ        yzfree##yeah.net
          xuhappy     xuxuhappy##sina.com
          zjucypher   xubo.cy##263.net

  本文件版本：  20905
  最后修改于：  2002-9-5


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

  已知 BUG：

    1、对某些含有未知标签的图象，libtiff库会产生内存泄漏。


  修正历史：

    2002-9    插件接口升级至2.2。引入内存防护概念。
    2002-5    修正读取JPEG压缩方式的TIFF图象时R、B分量颠倒的问题。
          修正不能正确读取PhotoShop导出的TIFF图象的问题。
          leye发现，YZ修正。

    2002-4    发布新版代码。支持ISeeIO，及以JPEG方式存储的TIFF
          图象。保存功能支持1、4、8、24位深度。支持多页TIFF。

    2001-2    加入了保存功能
    2001-1    修正RGBA转换函数中存在的问题，并改进了步进控制代码
    2001-1    核心码改用libtiff库
    2000-7    第一个发布版


********************************************************************/



#ifndef __TIFF_MODULE_INC__
#define __TIFF_MODULE_INC__

#ifndef __ISEEIO_INC__
/* 数据存储设备接口定义文件 */
#endif


#endif /* __TIFF_MODULE_INC__ */
