# 在VS2010下编译libjpeg及其应用例子 - L_Andy的专栏 - CSDN博客

2018年01月21日 14:32:50[卡哥](https://me.csdn.net/L_Andy)阅读数：231
个人分类：[第三方库编译](https://blog.csdn.net/L_Andy/article/category/7208429)



下载到libjpeg源码后，解压，可以看到很多文档，想在VS2010中编辑使用，主要参考文件：install.txt

在字段Microsoft Windows, Microsoft Visual C++ 2010 Developer Studio (v10):下的部分有详细说明。

下面是我参考了说明文档以及网上一些资料后的编译过程：

**编译过程：**

1，下载libjpeg的源代码  下载地址：http://www.ijg.org/  我下载到的是**jpegsr8d.zip**

2、解压出来一堆文件，打开命令行窗口（CMD），**进入代码所放的目录**下

如源码所在目录为E:\jpeg-8d  则在命令行输入E: 回车，再输入cd jpeg-8d  ，cd就是进入文件夹的意思。

3、运行命令：**NMAKE /f makefile.vc  setup-v10** 回车开始编译，然后问题就来了：

  “*nmake 不是内部或外部命令*”

    　　原因：没有安装vc或者没有选择环境变量。 

解决方法：安装vc或vs，并选择**环境变量；按以下方法设置**： 【根据VS2010的实际安装路径设置】

    . 在我的电脑 > 属性 > 高级 > 环境变量中设置“include”“lib”“path”变量的值， 如果没有这三个变量，就新建变量，如果有，就在当前的值后边添加以下值，值之间用分号隔开。

    .** include**：D:\Program Files\Microsoft Visual Studio 10.0\VC\include\ 

    . **lib**：D:\Program Files\Microsoft Visual Studio 10.0\VC\lib\ 

    . 找到Microsoft Visual Studio .NET\Vc7\bin\cl.exe的动态链接库mspdb70.dll, 将mspdb70.dll的路径加入到
**path**的值：D:\Program Files\Microsoft Visual Studio 10.0\VC\bin

    设置好后再次运行命令：NMAKE /f makefile.vc  setup-v10 回车

5、又出现问题：* “file 'win32.mak' not found”*

    　　提示找不到win32.mak文件，这个文件定义了一些必须的宏，这个文件在Windows SDK的安装目录中，打开**makefile.vc文件修改**第12行为：
**!include <C:/Program Files/Microsoft SDKs/Windows/v6.0A/Include/win32.mak>**

    　　保存，再执行上述命令。

（有些人编译到此还会继续出现错误，见本文下方引用的其他解决办法。）

其中有一段在vs2010下的说明

==================================================================

Microsoft Windows, Microsoft Visual C++ 2010 Developer Studio (v10):

We include makefiles that should work as project files in Visual Studio2010 or later.  There is a library makefile that builds the IJG library

as a static Win32 library, and application makefiles that build the sample applications as Win32 console applications.  (Even if you only want the

library, we recommend building the applications so that you can run the self-test.)

To use:

1. Open the command prompt, change to the main directory and execute the  command line

 NMAKE /f makefile.vc  setup-v10

   This will move jconfig.vc to jconfig.h and makefiles to project files.   (Note that the renaming is critical!)

2. Open the solution file jpeg.sln, build the library project.

   (If you are using Visual Studio more recent than 2010 (v10), you'll  probably get a message saying that the project files are being updated.)

3. Open the solution file apps.sln, build the application projects.

4. To perform the self-test, execute the command line

NMAKE /f makefile.vc  test-build

5. Move the application .exe files from `app`\Release to an   appropriate location on your path.

Note:

There seems to be an optimization bug in the compiler which causes the self-test to fail with the color quantization option.

We have disabled optimization for the file jquant2.c in the library project file which causes the self-test to pass properly.

=====================================

简单的操作其实也就两行，一个是生成vs2010的工程文件 NMAKE /f makefile.vc  setup-v10

运行结果如下：

Microsoft (R) Program Maintenance Utility Version 9.00.30729.01

Copyright (C) Microsoft Corporation.  All rights reserved.

        ren jconfig.vc jconfig.h

        ren makejsln.v10 jpeg.sln

        ren makeasln.v10 apps.sln

        ren makejvcx.v10 jpeg.vcxproj

        ren makejfil.v10 jpeg.vcxproj.filters

        ren makecvcx.v10 cjpeg.vcxproj

        ren makecfil.v10 cjpeg.vcxproj.filters

        ren makedvcx.v10 djpeg.vcxproj

        ren makedfil.v10 djpeg.vcxproj.filters

        ren maketvcx.v10 jpegtran.vcxproj

        ren maketfil.v10 jpegtran.vcxproj.filters

        ren makervcx.v10 rdjpgcom.vcxproj

        ren makerfil.v10 rdjpgcom.vcxproj.filters

        ren makewvcx.v10 wrjpgcom.vcxproj

        ren makewfil.v10 wrjpgcom.vcxproj.filters

然后打开jpeg.sln，直接编译就ok 

**测试程序：**

1，在测试程序中**工程属性-Linker-Input-Additional Dependencies**中添加编译得到的**jpeg.lib**，同时将jpeg.lib文件放在测试代码同级目录下；

2，将libjpeg包中的头文件放在测试程序目录下，并在测试程序中添加对jpeglib.h和jmorecfg.h的引用（#include）。

即可在测试程序用用libjpeg库生成jpeg图像。

测试代码例子：本例转自[http://blog.csdn.net/ani/article/details/2955189](http://blog.csdn.net/ani/article/details/2955189) ，我自己添加了几个参数设置，写图像数据时改了些数据

int_tmain(intargc, _TCHAR* argv[])

{

//定义一个压缩对象，这个对象用于处理主要的功能

         jpeg_compress_struct jpeg;

//用于错误信息

jpeg_error_mgr jerr;

//错误输出在绑定

jpeg.err = jpeg_std_error(&jerr);

//初始化压缩对象

jpeg_create_compress(&jpeg);

//定义压缩后的输出，这里输出到一个文件！

FILE* pFile = fopen( "c:\\tt.jpeg","wb" );

if( !pFile )

return 0;

//绑定输出

         jpeg_stdio_dest(&jpeg, pFile);

//压缩参数设置。具体请到网上找相应的文档吧，参数很多，这里只设置主要的。

//我设置为一个 24 位的 512X512大小的ＲＧＢ图片

         jpeg.image_width = 512;

         jpeg.image_height = 512;

         jpeg.input_components  = 3;

         jpeg.in_color_space = JCS_RGB;

//参数设置为默认的

jpeg_set_defaults(&jpeg);

//还可以设置些其他参数：

//// 指定亮度及色度质量

//jpeg.q_scale_factor[0] = jpeg_quality_scaling(100);

//jpeg.q_scale_factor[1] = jpeg_quality_scaling(100);

//// 图像采样率，默认为2 * 2

//jpeg.comp_info[0].v_samp_factor = 1;

//jpeg.comp_info[0].h_samp_factor = 1;

//// 设定编码jpeg压缩质量

//jpeg_set_quality (&jpeg, 100, true);

//开始压缩。执行这一行数据后，无法再设置参数了！

         jpeg_start_compress(&jpeg, TRUE);

//定义一个数组，代表图片每一行的数据。3　代表　jpeg.input_components

unsignedchar oneRowImgData[ 512 * 3 ];

for( int i=0;i<512*3;++i )

                 oneRowImgData[i] = 0;

         JSAMPROW row_pointer[1];

row_pointer[0] = oneRowImgData;

//从上到下，设置图片中每一行的像素值

for( inti=0;i<jpeg.image_height;++i )

         {

int data = 0;

for (int j = 0;j<jpeg.image_width*3;j=j+3)

                 {

oneRowImgData[j] = data%255;//R

//oneRowImgData[j+1] = data%255;//G

//oneRowImgData[j+2] = data%255;//B

                          data ++;

                 }

//将一行数据写入！

                 jpeg_write_scanlines( &jpeg,row_pointer,1 );

         }

//结束压缩

         jpeg_finish_compress(&jpeg);

//清空对象

jpeg_destroy_compress(&jpeg);

         fclose( pFile ); pFile = NULL;

return 0;

}

执行完毕后，会在C盘根目录下生成一个名为tt.jpeg的图片。

1、从[ftp://ftp.simtel.net/pub/simtelnet/msdos/graphics/jpegsr6.zip](http://blog.csdn.net/wuliqunaowuliqunao/article/details/ftp:/ftp.simtel.net/pub/simtelnet/msdos/graphics/jpegsr6.zip)下载libjpeg的源代码

2、解压出来一堆文件，打开命令行窗口（CMD），进入代码所放的目录下

3、运行命令：nmake /f makefile.vc nodebug=1
开始编译，然后问题就来了

4、“nmake
不是内部或外部命令”
　　这个执行文件在VC的安装目录中，解决方法：输入命令 path C:/Program Files/Microsoft Visual Studio 9.0/VC/bin
回车

5、makefile.vc<11>:fatal error U1052: file 'win32.mak' not found
　　提示找不到win32.mak文件，这个文件定义了一些必须的宏，这个文件在Windows SDK的安装目录中，打开makefile.vc文件修改第12行为：

    !include <C:/Program Files/Microsoft SDKs/Windows/v6.0A/Include/win32.mak>
　　保存，再执行编译命令又有新的错误。

6、NMAKE:fatal error U1073: don't know how to make 'jconfig.h'
　　这是个奇怪的问题，在网上搜了半天没有结果，最后才发现原来目录中没有这个文件。其实只要把文件jconfig.vc更名为jconig.h即可。再执行编译命令又有新的错误。

7、NMAKE: fatal error U1077: '.../cl.EXE' : return code '0xc0000135'
CL命令未能执行，我们直接在命令行提示后面输入cl回车，会有个提示“没有找到
 mspdb80.dll ”。解决方法：输入 vcvars32
回车，这是一个用来设置VC路径环境的批处理。再次执行编译命令，出现了一行行字符快速的刷新，哈哈，已经开始编译了。最后又出现一个错误，这个就不用管它了，因为我们需要的libjpeg.lib文件已经出现在了当前目录中。

