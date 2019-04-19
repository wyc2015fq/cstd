# 在PC环境下用VS2005编译供Wince6.0使用的libpng和zlib库 - xqhrs232的专栏 - CSDN博客
2012年06月29日 21:26:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1375标签：[wince																[byte																[测试																[dll																[function](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
移植libpng和zlib库到WINCE6.0系统编译的相关网帖
1.wince下的libpng!----[http://topic.csdn.net/u/20100429/13/3055dad4-ac93-4d0c-820d-b04f07df390d.html](http://topic.csdn.net/u/20100429/13/3055dad4-ac93-4d0c-820d-b04f07df390d.html)
2.libpng问题----[http://topic.csdn.net/u/20090811/13/f8f60a8a-b209-4b61-ba7c-de0df5b20f30.html](http://topic.csdn.net/u/20090811/13/f8f60a8a-b209-4b61-ba7c-de0df5b20f30.html)
编译的基本步骤如下
    1》先去相关网页下载供WINCE使用的libpng和zlib库源代码
[http://www.libpng.org/pub/png/libpng.html](http://www.libpng.org/pub/png/libpng.html)
[http://www.tenik.co.jp/~adachi/wince/](http://www.tenik.co.jp/~adachi/wince/)-------------提供有WINCE使用的版本下载
    其实也可以去下载CXIMAGE的WINCE版本，因为里面也有libpng和zlib这两个工程----CXIMAGE的 WINCE下载地址：：[http://www.xdp.it/download.htm](http://www.xdp.it/download.htm)
   有关CXIAMGE库移植到WINCE可以参考网帖----[教程：CXIMAGE
 移植 CE 平台](http://blog.csdn.net/xqhrs232/article/details/5339575)----[http://blog.csdn.net/xqhrs232/article/details/5339575](http://blog.csdn.net/xqhrs232/article/details/5339575)
    2》源代码里面没有针对VS2005的工程，那就自己对应创建吧-----创建2个针对SMART设备的WIN32空DLL工程
   3》把libpng和zlib库源代码加进工程里面去，再编译，查看编译错误对应解决-----把不需要编译进DLL的源文件从工程中排除掉
      libpng project----delete pngtest.c    zlib project----delete maketree.c /minigzip.c(这两个文件里面有main( )函数,所以删除了，但没想明白为什么)
   4》这样就可以得到libpng和zlib库的DLL/LIB文件了
   5》写一个测试工程测试编译出来的DLL/LIB文件-----网上有很多怎么去读取PNG文件的文章可以借鉴
[使用libpng读取png图像](http://blog.csdn.net/xqhrs232/article/details/7685296)----[http://blog.csdn.net/xqhrs232/article/details/7685296](http://blog.csdn.net/xqhrs232/article/details/7685296)
      有关LIBPNG库里面的高级与低级函数的联系与区别可以参考----[图像解码之二——使用libpng解码png图片](http://blog.csdn.net/xqhrs232/article/details/7703281)----[http://blog.csdn.net/xqhrs232/article/details/7703281](http://blog.csdn.net/xqhrs232/article/details/7703281)
     还有就是LIBPNG库里面自带的文档
   6》利用应用工程调试ibpng和zlib库的DLL/LIB文件----对应的配置文件要不要修改/宏编译要禁止还是使能
       A.在这个地方有一个特别要注意的地方----你的PNG文件的打开与操作的函数要很好地统一起来----fopen/fread/fclose++++createfile/readfile/closehandle最好配对使用
      网上有很多因为这个问题而出现的帖子，本文引用的那两个问题帖也是因为这个原因引起的！！！下面的一段文字引自pngrio.c
#if !defined(PNG_NO_STDIO)
/* This is the function that does the actual reading of data.  If you are
   not reading from a standard C stream, you should create a replacement
   read_data function and use it at run time with png_set_read_fn(), rather
   than changing the library. */
#ifndef USE_FAR_KEYWORD
void PNGAPI
png_default_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;
   if(png_ptr == NULL) return;
   /* fread() returns 0 on error, so it is OK to store this in a png_size_t
    * instead of an int, which is what fread() actually returns.
    */
#if defined(_WIN32_WCE)
   if ( !ReadFile((HANDLE)(png_ptr->io_ptr), data, length, &check, NULL) )
      check = 0;
#else
   check = (png_size_t)fread(data, (png_size_t)1, length,
      (png_FILE_p)png_ptr->io_ptr);
#endif
   if (check != length)
      png_error(png_ptr, "Read Error");
}
#else
......
#endif
       B.看看pngconf.h要不要加入如下定义
  /* abort() function is not supported on WindowsCE */
#  define PNG_ABORT() exit(-1)
       C.有问题最好追踪一下代码，看看具体调的是那个函数，在新的平台是不是要换成支持的来完成
   7》测试通过的DLL/LIB文件就可以发布使用了
//==========================================================================================================
备注::
   1>注意BMP数据反向的问题，那个是第一行，那个是最后一行，怎么进行反向----因为由解码出来的PIXELDATA直接去形成HBITMAP贴图，图片的颜色是不对的。
   一般解码出来的是RGBA的格式，这个图片用于产生用于OPENGL的纹理是可以直接使用的，因为OPENGL直接支持`GL_RGBA格式的数据直接产生纹理。但要用于形成HBITMAP贴图则要注意必要的变换。`
  2>PC上不支持PNG的ALPHABLEND？我的透明PNG怎么显示出来有问题啊----不知道我的解码是不是存在问题，得仔细看看。
3>怎么进行必要的格式转换-----可以用于产生HBITMAP贴图
[加载png图片并转换为DIB HBITMAP](http://blog.csdn.net/xqhrs232/article/details/7704506)----[http://blog.csdn.net/xqhrs232/article/details/7704506](http://blog.csdn.net/xqhrs232/article/details/7704506)
- **for** (y=0; y<info_ptr->height; y++)  
-             {  
- **for** (x=0; x<info_ptr->width; x++)  
-                 {  
-                     i=(info_ptr->height-1-y)*info_ptr->width*4+x*4;  
- 
-                     r=pBits[i+0];  
-                     g=pBits[i+1];  
-                     b=pBits[i+2];  
-                     a=pBits[i+3];  
- 
- // RGBA -> BGRA and to AlphaBend() Colors 
-                     pBits[i+0]=**BYTE**(b * a / 255);  
-                     pBits[i+1]=**BYTE**(g * a / 255);  
-                     pBits[i+2]=**BYTE**(r * a / 255);  
-                 }  
- 
-             }  

