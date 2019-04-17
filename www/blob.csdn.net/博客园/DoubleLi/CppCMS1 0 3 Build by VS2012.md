# CppCMS1.0.3 Build by VS2012 - DoubleLi - 博客园






**1****、CppCMS简介**

![CppCMS1.0.3 <wbr>Build <wbr>by <wbr>VS2012](http://s9.sinaimg.cn/mw690/002I5oKhgy70ICmUcM8b8&690)

CppCMS是一个C++的Web开发框架（不是一个CMS）。它不同于大多数其他Web开发框架，如巨蟒Django ， Java的Servlets ，或C++ Wt因为它在设计和调整来处理极高的负荷，而且它的目的是发展的网站，而不是“GUI-Like” Web应用程序。[1]

CppCMS 即 C++ Web Framework （C++ 网络架构）是比较流行的一个用C++编写的开发架构。

它采用传统的网络程序开发所用的设计模式MVC。其它语言采用MVC模式的架构还有 Zend （PHP）和 Django (Python). 如果软件开发人员有基于这两种架构的开发经验并且懂得C++，那么他就可以很快熟悉CppCMS并基于它进行[网站开发](http://baike.baidu.com/view/71517.htm)。C++[网站开发](http://baike.baidu.com/view/71517.htm)通常适用于网络负载较重的公司。 C++研发通常较其他高级语言，如Java，.NET,或者[脚本语言](http://baike.baidu.com/view/76320.htm)PHP, Python要更为复杂。基于这个原因，小公司则不宜采用C++或者CppCMS进行研发。



当前版本：cppcms-1.0.3

Download: [http://cppcms.com/wikipp/en/page/main](http://cppcms.com/wikipp/en/page/main)



**2****、依赖**

ZLib 1.2.7

[http://sourceforge.net/projects/libpng/files/zlib/](http://sourceforge.net/projects/libpng/files/zlib/)



PCRE 8.32

[ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/](ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/)



Pcre for Windows(上面的PCRE 8.32给CPPCMS编译时没编译过，可能有选项没设定好，换成gunwin32 pcre库就可以了，先这样吧)

[http://gnuwin32.sourceforge.net/packages/pcre.htm](http://gnuwin32.sourceforge.net/packages/pcre.htm)



icu4c 4.8(直接下编译好的dll)

[http://site.icu-project.org/download](http://site.icu-project.org/download)



openssl-1.0.1c

[http://www.openssl.org/contrib/](http://www.openssl.org/contrib/)



**3****、环境与工具**

Win7

Python 2.7.3 (需自己配置环境变量)

ActivePerl 5.16.2

vs2012

NASM (需自己配置环境变量)

CMAKE2.8

我的编译目录G:\CPPWebframework\



**4****、编译ZLIB**

解压ZLib 1.2.7并进入解压出的目录G:\CPPWebframework\lib\zlib-1.2.7，再进入win32目录

在命令行执行：cmake .. -G" Visual Studio 11" (注意：cmake空格后有两点)

生成VS2012的工程文件ALL_BUILD.vcxproj，用VS2012打开它并编译成Release

修改源码zlib.h，将#include "zconf.h"改为#include "win32\zconf.h"



**5****、编译****PCRE**

解压PCRE 8.32并进入G:\CPPWebframework\lib\pcre-8.32

在命令行执行：cmake  -G" Visual Studio 11"

生成VS2012的工程文件ALL_BUILD.vcxproj，用VS2012打开它并编译成Release

注意：这个库编译成功，但是给CPPCMS编译时不成功，换成GUNWIN32 PCRE7.0后成功了。



**6****、编译openssl-1.0.1c**

解压openssl-1.0.1c进入目录

执行命令：perl Configure VC-WIN32 --prefix=安装目录

再执行批处理：ms\32all.bat

会在out32dll目录生成动态库文件



**7****、编译****cppcms-1.0.3**

将目录cppcms-1.0.3\booster\lib\locale\test\ 下的源文件 test_boundary.cpp和test_posix_formatting.cpp另存为UTF-8格式(源文件中有日语，VS编译时变成了乱码)



如果编译器报错，无法打开目录cppcms-1.0.3\CMakeFiles\cppcms.dir\cppcms_boost\src下的gzip.cpp和zlib.cpp，直接从目录cppcms-1.0.3\cppcms_boost\src下将两个源文件拷贝过去，这可能是生成的Makefile路径有文件，原本CMakeFiles这个目录只是存放obj文件的



在cppcms-1.0.3目录下执行：

cmake -G"NMake Makefiles" -DCMAKE_INCLUDE_PATH=G:\CPPWebframework\lib\pcre-7.0-lib\include;G:\CPPWebframework\lib\zlib-1.2.7;G:\CPPWebframework\lib\icu\include;G:\CPPWebframework\lib\openssl-1.0.1c\inc32 -DCMAKE_LIBRARY_PATH=G:\CPPWebframework\lib\pcre-7.0-lib\lib;G:\CPPWebframework\lib\zlib-1.2.7\win32\Release;G:\CPPWebframework\lib\icu\lib;G:\CPPWebframework\lib\openssl-1.0.1c\out32dll;G:\CPPWebframework\cppcms\cppcms-1.0.3\booster -DCMAKE_INSTALL_PREFIX=G:\CPPWebframework\cppcms\libcppcms -DCMAKE_BUILD_TYPE=Release

(这是本人的库路径)



生成成功后：

nmake

nmake install



如果依赖库路径变化，请重新生成vs2012的工程文件



Build Options
  -DDISABLE_STATIC=ON -- disable building of static version of cppcms library
  -DDISABLE_SHARED=ON -- disable building of shared version of cppcms library
  -DDISABLE_ICONV=ON -- disable usage of iconv (ICU would be used instead)
  -DDISABLE_GCRYPT=ON -- disable usage of gcrypt library. - -DDISABLE_OPENSSL=ON -- disable usage of OpenSSL.
  -DUSE_WINDOWS6_API=ON -- use Windows Vista, Windows 7 API if possible, allows CppCMS using native Windows Vista/7 conditional variables and other advanced API. By default disabled.
  -DLIBDIR=lib64 - use alternative name for library directory, for example use lib64 on Rad Hat based distributions.

Generic Size Optimization Options for Embedded Builds:
  -DDISABLE_FCGI=ON -- build without FastCGI Server API.
  -DDISABLE_SCGI=ON -- build without SCGI Server API.
  -DDISABLE_HTTP=ON -- build without internal HTTP server.
  -DDISABLE_ICU_LOCALE=ON -- do not use ICU for localization but rather C++ std::locale, Windows API or POSIX API based localization -- many localization features would not be available, but it may be useful for embedded builds.
  -DDISABLE_PREFORK_CACHE=ON - disable cache support for preforking modes (process shared cache)
  -DDISABLE_TCPCACHE=ON - disable distributed cache support (memcached-like solution support)
  -DDISABLE_CACHE=ON - disable caching system at all.
  -DDISABLE_GZIP=ON - disable output gzip compression support (eliminates dependency on zlib)

Generic useful CMake options:

-DCMAKE_BUILD_TYPE=(Debug|Release|RelWithDebInfo|MinSizeRel) -- release type. RelWithDebInfo is default, unless using MSVC where Debug is default.


-DCMAKE_INCLUDE_PATH=/path/to/include -- path to location of libraries headers, provide it in order to find libraries headers installed in non-standard locations. You almost always need to provide it under Windows.



-DCMAKE_LIBRARY_PATH=/path/to/lib -- path to location of libraries, provide it in order to find libraries installed in non-standard locations. You almost always need to provide it under Windows.

-DCMAKE_INSTALL_PREFIX=/usr/local -- Installation prefix (similar to autoconf --prefix). Defaults to /usr/local

参考：

http://blog.csdn.net/lee353086/article/details/7755809

http://blog.csdn.net/rydiy/article/details/7710301

http://cppcms.com/wikipp/en/page/cppcms_1x_build

转：[http://www.vimi.hk/pages/201502/150206202042911622741.html](http://www.vimi.hk/pages/201502/150206202042911622741.html)









