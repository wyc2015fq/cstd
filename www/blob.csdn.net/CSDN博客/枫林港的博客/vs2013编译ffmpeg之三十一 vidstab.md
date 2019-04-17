# vs2013编译ffmpeg之三十一 vidstab - 枫林港的博客 - CSDN博客





2018年07月10日 23:26:53[枫林港](https://me.csdn.net/u013601132)阅读数：174








# vidstab

对应ffmpeg configure选项—enable-libvidstab。 
[https://github.com/georgmartius/vid.stab](https://github.com/georgmartius/vid.stab)
[http://public.hronopik.de/vid.stab/](http://public.hronopik.de/vid.stab/)

libvidstab可以用cmake来产生MSVC工程：`cmake -G "Visual Studio 12 2013" ./`
打开工程，Project properties->Configuration properties->C/C++->Code generation->Runtime Library要设置成Mtd。库的类型是静态库，后缀改成.lib，库名vidstab，如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20180707002542798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
有几个错误：
- **1、invalid numeric argument ‘/Wno-pointer-sign’**

将下图中的Wno-pointer-sign去掉就可以解决： 
![这里写图片描述](https://img-blog.csdn.net/20180707002557101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

- **2、error C2065: ‘M_PI’ : undeclared identifier**
和[frei0r中M_PI_2未定义](https://blog.csdn.net/u013601132/article/details/80878863)的解决办法一样，M_PI在/mingw/include/math.h里面定义的，直接将这个定义拷贝到localmotion2transform.c里面。
- **3、Cannot open include file: ‘libgen.h’: No such file or directory**

有两个.c文件会报这个错误，直接将#include “libgen.h”这句注释掉就可以解决。
- **4、error C2054: expected ‘(’ to follow ‘inline’**

直接定义inline=__inline： 
![这里写图片描述](https://img-blog.csdn.net/20180707002613827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- **5、LINK : fatal error LNK1181: 无法打开输入文件“gomp.lib”**

vidstab.pc里面，Libs规则里面去掉-lgomp，即去掉OpenMP支持，MSVC2013支持OpenMP，可以打开：Project properties->Configuration properties->C/C++->Language->Open MP Support选项打开即可。但会编译通不过，所以没有打开。在msys下编译也没有打开。
- **6、缺符号**

```
vidstab.lib(transform.obj) : error LNK2001: 无法解析的外部符号 _interpolateBiCub

vidstab.lib(transform.obj) : error LNK2001: 无法解析的外部符号 _interpolateBiLin

vidstab.lib(transform.obj) : error LNK2001: 无法解析的外部符号 _interpolateLin

vidstab.lib(transform.obj) : error LNK2001: 无法解析的外部符号 _interpolateZero
```

vid.stab-master工程属性里面，宏定义”inline=__inline” 改成 “inline=”。







