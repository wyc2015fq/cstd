# windows下用VC编译IJG库/libjpeg - xqhrs232的专栏 - CSDN博客
2013年07月08日 15:54:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1580
原文地址::[http://www.liujiaojiao.com/post/9.html](http://www.liujiaojiao.com/post/9.html)
一、下载源代码。下载地址：http://www.ijg.org/。注意：要下载 Windows format： jpegsr8d.zip
二、怎么编译 jpeg.lib ？
        1、解压源代码 jpegsr8d.zip
        2、右键单击计算机（我的电脑），进入属性，高级系统设置，环境变量，添加环境变量PATH，C:\Program Files\Microsoft Visual Studio\VC98\Bin （VC6安装路径）若已有PATH，加分        号，C:\Program Files\Microsoft Visual Studio\VC98\Bin；
        3、 修改makefile.vc，将 Pull in standard variable definitions下面的一行换为：!include <C:\Program Files\Microsoft Visual Studio\VC98\Include\win32.mak>；
        4、进入命令提示环境下，D:\test\jpegsr8d\jpeg-8d>NMAKE /f makefile.vc  setup-vc6
        上面的步骤会在源程序目录中生成很多 dsp 和 dsw 文件，选择其中的 jpeg.dsw 用 VC6 打开。
        编译之后就会在 Release 目录生成 jpeg.lib 静态库文件。
三、怎样在 VC6 中使用 IJG 的 jpeg 库？
        为了方便以后使用，可创建一个名为 libjpeg 的目录，将下列文件复制到其中：
        jpeg.lib
        jpeglib.h
        jconfig.h
        jmorecfg.h
        以后在使用的时候就只需要将 libjpeg 目录复制到工程所在目录中，与 jpeg-8d 下一大堆文件无关了。
        在使用时，使用到该库的 cpp 文件应加上如下内容：
        Cpp代码  ![收藏代码](http://coding1688.iteye.com/images/icon_star.png)
- extern"C" {  
-     #include "jpeglib.h"
- }  
- 
- #pragma comment(lib, "libjpeg/jpeg.lib")
        因为那些头文件是放置在 libjpeg 目录中的，因此需要增加 附加包含路径 libjpeg，操作如下：
        在菜单“工程”中选择“设置”，切到“C/C++”，将“分类”选择为“预处理器”，在“附加包含路径”中加入 libjpeg 。
        使用时需要 将 MFC 库作为静态库使用，更改方式：
        在菜单“工程”中选择“设置”，切到“常规”，就会看到 Microsoft 基础类的设置，改成“把MFC作为静态链接库”。
        否则，在链接时报一大堆错误
本文出自：[我做的事](http://www.liujiaojiao.com/)，地址：[http://www.liujiaojiao.com/post/9.html](http://www.liujiaojiao.com/post/9.html)，转载须注明！
