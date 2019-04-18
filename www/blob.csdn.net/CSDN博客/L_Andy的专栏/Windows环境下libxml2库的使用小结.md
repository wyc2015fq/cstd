# Windows环境下libxml2库的使用小结 - L_Andy的专栏 - CSDN博客

2018年12月17日 16:34:20[卡哥](https://me.csdn.net/L_Andy)阅读数：98


(1)--环境的搭建，下载与安装LIBXML2和ICONV

          Libxml2是一个C语言的XML程序库，可以简单方便的提供对XML文档的各种操作，并且支持XPATH查询，以及部分的支持XSLT转换等功能。Libxml2的下载地址是http://xmlsoft.org/downloads.html，完全版的库是开源的，并且带有例子程序和说明文档。完全版的文件名为:libxml2-2.7.8.tar.gz。

          Libxml2中默认的内码是UTF-8，所有使用libxml2进行处理的xml文件，必须首先显式或者默认的转换为UTF-8编码才能被处理。

要在xml中使用中文，就必须能够在UTF-8和GB2312内码（较常用的一种简体中文编码）之间进行转换。Libxml2提供了默认的内码转换机制，并且在libxml2的Tutorial中有一个例子，事实证明这个例子并不适合用来转换中文。所以需要我们显式的使用ICONV来进行内码转换，libxml2本身也是ICONV使用进行转换的。ICONV是一个专门用来进行编码转换的库，基本上支持目前所有常用的编码。它是glibc库的一个部分，常常被用于UNIX系统中。当然，在windows下面使用也没有任何问题。

其下载地址是http://gnuwin32.sourceforge.net/packages/libiconv.htm，文件名为libiconv-1.9.2-1-lib.zip。

          将libiconv-1.9.2-1-lib.zip解压，将其中的iconv.h放入C:\opt\include目录中，将其中的libiconv.lib放入C:\opt\lib中，并改名为iconv.lib.(没有opt目录就新建一个)。

          解压libxml2-2.7.8.tar.gz文件到C盘根目录，在c:\libxml2-2.7.8\libxml2-2.7.8\win32目录中存放了多个Windows平台编译器的Makefile文件，我们使用vs2008，所以待会会用Makefile.msvc文件。

1.进入Visual Studio 2008 Command Prompt；

2.cd c:\libxml2-2.7.8\libxml2-2.7.8\win32

3.输入cscript configure.js compiler=msvc prefix=c:\opt  include=c:\opt\include lib=c:\opt\lib debug=yes，回车执行。

4.最后使用nmake命令进行编译，输入nmake /f Makefile.msvc并回车。

此时会出现一下错误提示，

Makefile.msvc(465) : fatal error U1020: end-of-file found before next directive

Stop.

网上找了下原因，原来是Makefile.msvc中存在错误。

+!if "$(WITH_ICU)" == "1"

+LIBS = $(LIBS) icu.lib

+!endif

将以上三行前面的+号都删掉，重新执行nmake即可。

在当前win32目录中生成了一个bin.msvc目录，其中就有开发需要的libxml2.lib和libxml2.dll文件。

