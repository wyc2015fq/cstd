# vs2013编译ffmpeg之三十四 xz、zlib - 枫林港的博客 - CSDN博客





2018年07月10日 23:33:51[枫林港](https://me.csdn.net/u013601132)阅读数：163








# xz

xz库即lzma的压缩库。ffmpeg会自动检测是否有lzma相关的库和头文件，没有enable项。

[http://www.7-zip.org/sdk.html](http://www.7-zip.org/sdk.html)

[http://tukaani.org/xz/](http://tukaani.org/xz/)

高压缩率，高解压速度，低内存消耗。代码在tukaani.org上下载，xz用的lzma库是7z的lzma SDK，所以这个模块要从tukaani.org上下载。7z上下载的lzma或7z源代码里面没有lzma.h，ffmpeg的configure会判断lzma.h。ShiftMediaProject里面有liblzma，也是从tukaani.org上下载，但没有使用这个，直接使用从tukaani.org上下载xz-5.2.2。

工程文件xz-5.2.2\windows\xz_win.sln打开，里面可以编译静态库和动态库，使用静态库，工程属性里面MDd改成MTd，生成的库名字改成lzma.lib，因为ffmpeg configure用的是lzma.lib。

ffmpeg configure检查lzma的语句是：
`disabled  lzma || check_lib2  lzma.h lzma_version_number -llzma || disable lzma`
这个语句检查失败了后并不会暂停下来，在configure执行完了后要检查一下config.log，或确认一下config.h（CONFIG_LZMA的值要为1）。

configure的时候会有下面的错误：
`error LNK2019: 无法解析的外部符号 implzma_version_number，该符号在函数 _check_lzma_version_number 中被引用`
**imp**前缀是因为加了__declspec(dllimport)导致的，这个只应该出现在动态库中。查看lzma.h，里面有：

```
#ifndef LZMA_API_IMPORT

#   if !defined(LZMA_API_STATIC) && defined(_WIN32) && !defined(GNUC)

#       define LZMA_API_IMPORT __declspec(dllimport)

#   else

#       define LZMA_API_IMPORT

#   endif

#endif
```

所以解决的办法就是在configure检测lzma的时候加上-DLZMA_API_STATIC，即：
`disabled  lzma || check_lib2  lzma.h lzma_version_number -llzma -DLZMA_API_STATIC || disable lzma`
# zlib

ffmpeg会自动检测是否有zlib相关的库和头文件，没有enable项。

zlib的编译要按下面的来编译：

[http://blog.csdn.net/x_iya/article/details/11826647](http://blog.csdn.net/x_iya/article/details/11826647)

[http://blog.sinzy.net/hjk41/entry/22891](http://blog.sinzy.net/hjk41/entry/22891)

zlib工程里面的zlibstat工程生成的lib要改名zlib.lib，否则ffmpeg configure会设别不到zlib。

## 编译方法

1、  打开vs2013的Command Prompt，在zlib-1.2.8\contrib\masmx86目录执行bld_ml32.bat。

2、  打开zlib-1.2.8\contrib\vstudio\vc10\zlibvc.sln，然后编译。

zlibstat.vcxproj是静态库的工程，zlibvc.vcxproj是动态库的工程，不要用错了。

## 编译错误

### 1、   错误1：

> 
error LNK2026: module unsafe for SAFESEH image


link->advance->Image Has Safe Exception Handlers设置为`No (/SAFESEH:NO)`，如下所示： 
![这里写图片描述](https://img-blog.csdn.net/2018070700360810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2、   错误2：

> 
LNK1118: syntax error in ‘VERSION’ statement


将”zlibvc.def” 里面的”VERSION 1.2.8”改成”VERSION 1.2”。

### 3、   错误3：

Ffmpeg-3.0 configure gnutls的时候出现下面的错误：

> 
libgnutlsd.lib(gnutls_compress.obj) : error LNK2019: 无法解析的外部符号_deflate，该符号在函数 __gnutls_compress 中被引用

libgnutlsd.lib(gnutls_compress.obj) : error LNK2019: 无法解析的外部符号_deflateEnd，该符号在函数__gnutls_comp_deinit中被引用

libgnutlsd.lib(gnutls_compress.obj) : error LNK2019: 无法解析的外部符号_inflate，该符号在函数 __gnutls_decompress 中被引用

libgnutlsd.lib(gnutls_compress.obj) : error LNK2019: 无法解析的外部符号_inflateEnd，该符号在函数__gnutls_comp_deinit中被引用

libgnutlsd.lib(gnutls_compress.obj) : error LNK2019: 无法解析的外部符号*deflateInit2*，该符号在函数__gnutls_comp_init中被引用

libgnutlsd.lib(gnutls_compress.obj) : error LNK2019: 无法解析的外部符号*inflateInit2*，该符号在函数__gnutls_comp_init中被引用


和gnutls工程相关联的是zlibstat.vcxproj，即zlib的静态库，将工程设置和其他工程做对比，设置改成和其他工程一样也不能解决问题。用命令`dumpbin.exe -SYMBOLS  zlibstat.lib`导出符号表看，deflateEnd在符号表里面是_deflateEnd@4，多了@4，这个形式的应该是`__stdcall(/Gz)`设置才会产生的函数名修饰，但工程里面配置的是__cdecl。在源代码里面找到deflateEnd的定义，在zlib.h里面：ZEXTERN int ZEXPORT deflateEnd OF((z_streamp strm)); 前面有两个宏ZEXTERN和ZEXPORT，找到这两个宏的定义的地方，直接将这两个宏定义搞成空，问题就解决了，具体是由#    define ZEXPORT WINAPI导致的，只有定义了ZLIB_WINAPI才会定义这个宏，ZLIB_WINAPI是在工程设置里面定义的，所以去掉ZLIB_WINAPI定义就可以解决问题。




