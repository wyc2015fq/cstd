# GDAL库简介以及在Windows下编译过程（64位） - L_Andy的专栏 - CSDN博客

2017年10月09日 17:39:05[卡哥](https://me.csdn.net/L_Andy)阅读数：431


GDAL(Geospatial Data Abstraction Library，地理空间数据抽象库)是一个在X/MIT许可协议下的开源栅格空间数据转换库。官网[http://www.gdal.org/index.html](http://www.gdal.org/index.html)，也可参考GitHub [https://github.com/OSGeo/gdal](https://github.com/OSGeo/gdal)，最新release版本为2.1.1.

```
GDAL是一个在X/MIT许可协议下读写空间数据(包括栅格数据和矢量数据)的开源库，它利用抽象数据模型来表达所支持的各种文件格式，还使用一系列命令行工具来进行数据转换和处理。

OGR(OGR Simple Features Library)是GDAL项目的一个分支，功能与GDAL类似，只是它提供对矢量数据的读写支持。同时它实现了一个对空间参考信息进行处理的类，用来对空间数据的空间信息进行处理。

GDAL支持的数据格式分为两种：栅格数据格式和矢量数据格式。GDAL在每次版本更新的时候,都会加入对新的格式的支持。

GDAL2.1.1源码中目录结构介绍：

alg：该目录下存放的是GDAL库中提供的一些算法的源代码；

apps：该目录下存放的是GDAL库中提供的一些命令行工具集的源代码；

bridge：该目录下存放的是用来连接GDAL抽象类的定义与GDAL自己的结构体定义及其实现的源代码；

data：该目录下存放的是GDAL库中需要用到的一些”配置文件”；

doc：该目录下存放的是用来生成GDAL帮助文档的一些dox文件；

frmts：该目录下存放的是GDAL针对不同的特定图像格式解析的源代码；

gcore：该目录下存放的是GDAL抽象类的数据集、图像、波段的读写接口等代码；

html：该目录下存放的是GDAL生成的帮助文档；

m4：该目录下存放的是以m4为后缀的文件，m4文件叫Macro Processor Library，主要是用autoconf来产生configure配置文件，继而自动生成Makefile文件；该目录在Windows下无用；

man：该目录下存放的是用来生成Linux或者其它平台下的帮助文件，Windows下无用；

ogr：该目录下存放的是OGR库源代码；

port：该目录下存放的是CPL库的源代码；CPL是一个GDAL的底层支持库，CPL库中定义了一些字符串的操作、文件处理、网页请求、数据库连接、哈希表、字符加密文件、压缩等基础的函数；

swig：该目录下存放的是swig的脚本；swig的作用就是可以将C/C++的库封装为Python、C#、Java、Perl和Ruby等其它语言的访问接口；

vb6：该目录用来将GDAL编译成一个VB6的模板；

alcocal.m4：该文件是用autoconf来产生configure配置文件；

autogen.sh：shell文件，该文件通过调用autoconf来产生configure配置文件；

COMMITERS：该文件中的内容是GDAL开发人员的信息，包括姓名、联系邮箱以及各自负责开发的模块说明等；

Doxyfile：用来生成帮助文件；

GDALmake.opt.in：该文件是Linux平台下的GDAL库编译配置文件；

gdalnightlysvn.sh：该文件是Linux平台下调用svn获取GDAL源代码的一个shell脚本；

GNUmakefile：GNU的make文件；

HOWTO-RELEASE：该文件是GDAL发布版本的一些说明；

install-sh：该文件是Linux平台下GDAL安装的shell脚本；

LICENSE.TXT：该文件是GDAL的许可说明文件；

ltmain.sh：Linux平台下libtool的shell脚本，windows下无用；

makefile.vc：GDAL的编译文件，用来将源代码编译成dll文件；

makegdal_gen.bat：用来生成VS的工程文件的批处理文件；

NEWS：该文件是介绍GDAL的新增功能以及修复的bug记录等；

nmake.opt：GDAL编译选项配置文件，在编译GDAL时，可以指定GDAL绑定的其它库；

PROVENANCE.TXT：GDAL目录说明文件；

VERSION：GDAL版本信息。

下面介绍GDAL2.1.1源码在windows vs2013中的编译步骤：
```
- 
从[http://download.osgeo.org/gdal/2.1.1/](http://download.osgeo.org/gdal/2.1.1/)下载最新稳定版本2.1.1，gdal211.zip并解压缩至E:\GitCode\GDAL_Test\src\gdal目录下；

- 
修改nmake.opt文件: MSVC_VER=1800 GDAL_HOME = 

“E:\GitCode\GDAL_Test\lib_x64_release”

- 
以管理员身份打开”VS2013 x64 兼容工具命令提示”；

- 
首先切换到VS2013的安装目录下执行：

```
C:\Program Files (x86)\Microsoft Visual
Studio12.0\VC\vcvarsall.bat  amd64；
```

- 
然后切换到GDAL源码目录E:\GitCode\GDAL_Test\src\gdal下依次执行：

[plain] view plain copy

```
nmake /f makefile.vc MSVC_VER=1800 WIN64=YES  
nmake /f makefile.vc MSVC_VER=1800 WIN64=YES install  
nmake /f makefile.vc MSVC_VER=1800 WIN64=YES devinstall
```

```
6.      执行完以上三条命令后，就会在E:\GitCode\GDAL_Test\lib_x64_release目录下生成gdal x64 release相应头文件、静态库和动态库

7.      按照以上类似步骤即可生成x64 debug、win32 release、win32 debug相应库：
```

[plain] view plain copy

```
C:\Program Files (x86)\Microsoft Visual Studio12.0\VC\vcvarsall.bat  x86  
nmake /f makefile.vc MSVC_VER=1800 DEBUG=1
```

```
如按照以上的步骤生成的库运行时提示缺失msvcr120.dll，则修改nmake.opt文件，将130行到138行，四处的/MD改成/MT即可。

按照上面的步骤只能生成相应的库，确不能生成vc工程，下面介绍下能生成vc工程的操作步骤：

1.      将nmake.opt文件中的SYM_PREFIX=_ 改为SYM_PREFIX=

2.      将makefile.vc中的内容
```

[plain] view plain copy

```
BASE_INCLUDE =  /INCLUDE:_GDALSimpleImageWarp@36 \  
        /INCLUDE:_GDALReprojectImage@48 \  
        /INCLUDE:_GDALComputeMedianCutPCT@32 \  
        /INCLUDE:_GDALDitherRGB2PCT@28 \  
        /INCLUDE:_OCTNewCoordinateTransformation@8 $(VB6_SAFEARRAYSYM)
```

```
调整为：
```

[plain] view plain copy

```
BASE_INCLUDE =  /INCLUDE:$(SYM_PREFIX)GDALSimpleImageWarp \  
        /INCLUDE:$(SYM_PREFIX)GDALReprojectImage \  
        /INCLUDE:$(SYM_PREFIX)GDALComputeMedianCutPCT \  
        /INCLUDE:$(SYM_PREFIX)GDALDitherRGB2PCT \  
        /INCLUDE:$(SYM_PREFIX)OCTNewCoordinateTransformation $(VB6_SAFEARRAYSYM)
```

```
3.      在刚才打开的命令行窗口中输入：makegdal_gen.bat 10.00 64 > makegdal10.vcproj ，执行完后会生成makegdal10.vcproj；
4.      打开vs2013，将其makegdal10.vcproj加入到工程中，会升级到vs2013；

创建一个控制台工程GDAL_Test，测试生成库的正确性，测试代码如下：
```

[cpp] view plain copy

```cpp
#include <iostream>  
#include <gdal_priv.h>  
#include <ogrsf_frmts.h>  
#include <gdal_alg.h>  

int main()  
{  
    const char* image_name = "E:/GitCode/GDAL_Test/test_images/a.tif";  
    GDALAllRegister();  
    GDALDataset* poSrc = (GDALDataset*)GDALOpen(image_name, GA_ReadOnly);  
    if (poSrc == nullptr) {  
        std::cout << "input image error" << std::endl;  
        return -1;  
    }  

    int width_src = poSrc->GetRasterXSize();  
    int height_src = poSrc->GetRasterYSize();  
    int band_count_src = poSrc->GetRasterCount();  
    fprintf(stderr, "width: %d, height: %d, bandCount: %d\n", width_src, height_src, band_count_src);  
    GDALDataType  gdal_data_type = poSrc->GetRasterBand(1)->GetRasterDataType();  
    int depth = GDALGetDataTypeSize((GDALDataType)gdal_data_type);  
    fprintf(stderr, "depth: %d\n", depth);  

    GDALClose((GDALDatasetH)poSrc);  

    std::cout << "ok" << std::endl;  

    return 0;  
}
```

```
结果如下：
由于编译GDAL源码需要较长的时间，这里把windows下x86/x64 debug/release编译好的库都放在了GitHub上，有需要的可以去下载。
```

