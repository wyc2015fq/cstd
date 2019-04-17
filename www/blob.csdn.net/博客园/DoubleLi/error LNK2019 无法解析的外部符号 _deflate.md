# error LNK2019: 无法解析的外部符号 _deflate - DoubleLi - 博客园






我的环境为: Win764 + VS2005 + zlib1.2.8

zlib1.2.8我使用VS2010来编译。

-------------------------------------------------------------------------------------------------

我导入zlib库的代码如下:

#include "zlib128/zlib.h"
// #pragma comment(lib, "zlib128/zlibwapi.lib")  // 链接zlib动态库
#pragma comment(lib, "zlib128/zlibstat.lib")  // 链接zlib静态库



编译使用zlib的工程，出现错误信息如下：

1>Gzip.obj : error LNK2019: 无法解析的外部符号 _deflateEnd 


1>Gzip.obj : error LNK2019: 无法解析的外部符号 _deflate
1>Gzip.obj : error LNK2019: 无法解析的外部符号 _deflateInit2_
1>Gzip.obj : error LNK2019: 无法解析的外部符号 _inflateEnd
1>Gzip.obj : error LNK2019: 无法解析的外部符号 _inflate
1>Gzip.obj : error LNK2019: 无法解析的外部符号 _inflateInit2_

fatal error LNK1120: 6 个无法解析的外部命令error LNK2019: 无法解析的外部符号 _deflate



从错误信息可以看出，在我使用的每个zlib库函数的前面都多出了一个下划线，如 _deflate,

通过查看zlibwapi.dll的导出函数，发现相应的函数前面并没有下划线，也就难道编译会链接错误了。

------------------------------------------------------------------------------------------------------------------------------------------------------

解决思路：

zlib.h头文件，定义了zlib库的导出函数原型，如：ZEXTERN intZEXPORT deflateEnd OF((z_streamp strm));

注意红色的ZEXPORT ，其原型在zconf.h文件里面定义：

#ifndef ZEXPORT
#  define ZEXPORT 
#endif

看到了吗，默认的ZEXPORT宏，只是个空符号罢了，而实际是其应该表示函数的调用方式。

--------------------------------------------------------------------------------------------------------------------------------------------------------------

解决方法：

在zconf.h文件将ZEXPORT宏的定义改成下面的形式即可：

#ifndef ZEXPORT
#  define ZEXPORT WINAPI
#endif

-----------------------------------------------------------------------------------------------------------------------------------------------------------









