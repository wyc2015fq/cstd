# 在PC环境下用VS2005编译供Windows使用的libpng和zlib库 - xqhrs232的专栏 - CSDN博客
2012年06月23日 00:09:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1705

参考网帖
1.[Visual C++ 下面编译 libpng, libjpeg 和 ImageMagick](http://blog.csdn.net/eyeblue/article/details/4445072)----[http://blog.csdn.net/eyeblue/article/details/4445072](http://blog.csdn.net/eyeblue/article/details/4445072)
1.libpng版本----最新的1.5.11----这个对zlib有要求的
2.zlib版本----最新的1.2.5----才可以配上最新的1.5.11版本的libpng库
    因为多出了下面几个操作gz的文件
     1>gzclose.c
     2>gzguts.h
     3>gzlib.c
     4>gzread.c
     5>gzwrite.c
   1.2.3版本的zlib库只有下面一个操作gz的文件
    1>gzio.c
3.把libpng库的源代码跟zlib库的源代码放一起-----可以放成两个不同文件夹-----libpng++++zlib
4.用vs2005打开libpng/projects/visualc71/libpng.sln工程，会提示要进行转换，那就进行转换吧
5.选择编译选项进行编译，先得把zlib库编译成功，有下面4种可选，看你需要了----需要DLL还是LIB文件
       1>DLL  Debug
        2>DLL  Release
       3>LIB  Debug
       4>LIB Release
   编译成功的话会在下面对应路径产生对应文件-----看你选的编译选项
    1》  libpng\projects\visualc71\Win32_DLL_Release\libpng15.dll++++libpng15.lib
    2》 libpng\projects\visualc71\Win32_DLL_Release\ZLib\zlib1.dll++++zlib1.lib
6.如果不出什么错误的话，一般都可以编译成功！！！
7.编译对应工程下自带的pngtest工程，应该就可以验证我们编译的结果是否正确了！我编译pngtest工程结果如下，应该可以表明编译的库是可用的。
3>------ 已启动全部重新生成: 项目: pngtest, 配置: DLL Release Win32 ------
3>正在删除项目“pngtest”(配置“DLL Release|Win32”)的中间文件和输出文件
3>正在编译...
3>pngtest.c
3>正在链接...
3>正在嵌入清单...
3>Testing...
3> Testing libpng version 1.5.11
3>   with zlib   version 1.2.5
3>libpng version 1.5.11 - June 14, 2012      Copyright (c) 1998-2012 Glenn Randers-Pehrson      Copyright (c) 1996-1997 Andreas Dilger      Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc. library (10511): libpng version 1.5.11 - June 14, 2012
3> pngtest (10511): libpng version 1.5.11 - June 14, 2012
3> Testing ..\..\pngtest.png:
3> Pass 0: rwrwrwrwrwrwrwrwrw
3> Pass 1: rwrwrwrwrwrwrwrwrw
3> Pass 2: rwrwrwrwrwrwrwrw
3> Pass 3: rwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrw
3> Pass 4: rwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrw
3> Pass 5: rwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrw
3>         rwrwrwrw
3> Pass 6: rwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrwrw
3>         rwrwrwrwrw
3> PASS (9782 zero samples)
3> Filter 0 was used 21 times
3> Filter 1 was used 15 times
3> Filter 2 was used 52 times
3> Filter 3 was used 10 times
3> Filter 4 was used 33 times
3> tIME = 7 Jun 1996 17:58:08 +0000
3> libpng passes test
3>生成日志保存在“file://e:\清华VS2005程序设计\png\lpng\projects\visualc71\Win32_DLL_Release\Test\BuildLog.htm”
3>pngtest - 0 个错误，0 个警告
========== 全部重新生成: 3 已成功, 0 已失败, 0 已跳过 ==========
//=============================================================================
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
