# vs2013编译ffmpeg之四 fontconfig、freetype、libiconv、libxml2、fribidi - 枫林港的博客 - CSDN博客





2018年07月07日 22:42:57[枫林港](https://me.csdn.net/u013601132)阅读数：252








# fontconfig

对应configure选项–enable-libfontconfig，字幕功能需要这个模块。 

最近编译ffmpeg用的是[ShiftMediaProject上的fontconfig](https://github.com/ShiftMediaProject/fontconfig)，编译没有什么问题。fontconfig依赖于freetype、libxml2、libiconv。

## freetype

下载[freetype-2.5.3](https://download.savannah.gnu.org/releases/free)，用MSVC编译，sln路径：freetype-2.5.3\builds\windows\vc2010\freetype.sln。 

MSVC2013+ffmpeg-3.0，编译的时候出现下面的错误：

```
libcryptod.lib(md5_dgst.obj) : error LNK2005: _MD5_Init 已经在 freetype253_D.lib(ftbase.obj) 中定义
libcryptod.lib(md5_dgst.obj) : error LNK2005: _MD5_Update 已经在 freetype253_D.lib(ftbase.obj) 中定义
libcryptod.lib(md5_dgst.obj) : error LNK2005: _MD5_Final 已经在 freetype253_D.lib(ftbase.obj) 中定义
```

两个模块都定义了这个函数。解决的办法是直接将freetype里面的MD5_Init重命名成freetype_MD5_Init，因为修改freetype改动要小一些。 

下面的错误出现在ffmpeg configure的时候：

```
下面的错误出现在ffmpeg-3.0 configure的时候：
libfontconfigd.lib(fcfreetype.obj) : error LNK2001: 无法解析的外部符号 _FT_Get_BDF_Property
./test.exe : fatal error LNK1120: 1 个无法解析的外部命令
这个是因为freetype的库缺少这个符号，在freetype的sln工程里面增加freetype-2.5.3\src\base\ftbdf.c就可以解决。
```

这个是因为freetype的库缺少这个符号，在freetype的sln工程里面增加freetype-2.5.3\src\base\ftbdf.c就可以解决。

## libiconv

下载libiconv-1.9.2-vc10.7z，不记得是从哪里下载的了，打开libiconv-1.9.2\build\iconv\ iconv.sln后编译，iconv.sln里面的iconv工程生成的库名设置，改成iconv.lib，ffmpeg检测的是iconv.lib，编译过程没有报错。ShiftMediaProject也有此模块。

## libxml2

libxml2用的是[官网上](http://xmlsoft.org/downloads.html)的libxml2-2.9.1，里面有MSVC工程。 

打开时可能会找不到iconv的vc工程，libxml2.vcxproj里面有iconv的路径，这里使用的iconv的版本是1.9.2，和libxml2-2.9.1同级有iconv模块所在的目录libiconv-1.9.2（libiconv-1.9.2-vc10.7z解压的），直接删除iconv工程，重新添加runsuite工程就可以了。 

下面这个错误：
```
1>h:\mingw\msys\1.0\home\aa\contribute\msvc\libxml2-2.9.1\nanoftp.c(778): error C2065: 'SEND_ARG2_CAST' : undeclared identifier
1>h:\mingw\msys\1.0\home\aa\contribute\msvc\libxml2-2.9.1\nanoftp.c(931): error C2065: 'GETHOSTBYNAME_ARG_CAST' : undeclared identifier
```

直接将这两个宏定义在libxml.h里面：

```
#ifndef GETHOSTBYNAME_ARG_CAST
#define GETHOSTBYNAME_ARG_CAST (char *)
#endif
#ifndef SEND_ARG2_CAST
#define SEND_ARG2_CAST (char *)
#endif
```

XML里面的runsuite工程编译时有下面的错误，暂时不用管，编译的时候会生成相应的库：

```
3>libxml2.lib(encoding.obj) : error LNK2019: unresolved external symbol _libiconv_close referenced in function _xmlFindCharEncodingHandler
3>libxml2.lib(encoding.obj) : error LNK2019: unresolved external symbol _libiconv_open referenced in function _xmlFindCharEncodingHandler
3>libxml2.lib(encoding.obj) : error LNK2019: unresolved external symbol _libiconv referenced in function _xmlIconvWrapper
```

## fribidi

fribidi是freetype、libass依赖的库，从官网上下载fribidi-0.19.6.tar.bz2编译过，遇到不少问题，最后用的是[ShiftMediaProject上的](https://github.com/ShiftMediaProject/fribidi)。




