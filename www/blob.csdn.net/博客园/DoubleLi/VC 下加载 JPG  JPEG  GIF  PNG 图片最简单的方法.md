# VC 下加载 JPG / JPEG / GIF / PNG 图片最简单的方法 - DoubleLi - 博客园







VC MFC 提供的 API LoadBitmap / LoadImage 类 CBitmap 等都只能操作 BMP 位图，图标。对于其他常用的 JPG / JPEG / GIF / PNG 格式，它无能为力。VC 下怎样才能加载各种非 BMP 格式的图片呢？ 下面介绍一种最简单的办法。用 CImage 类的 Load 函数加载图片，之后用 Detach 取得HBITMAP 句柄。取得图片的HBITMAP 句柄后就可以像操作 BMP 图片一样处理 JPG / JPEG / GIF / PNG 格式的图片了。具体代码如下：




#include “*atlimage.h”*



CImage img;

            HRESULT ret = img.Load(filename );      // filename 是要加载的文件名（包含路径）

            HBITMAP bitmap = img.Detach();

            //像操作 BMP 图片一样处理图片





- 顶
0- 踩










