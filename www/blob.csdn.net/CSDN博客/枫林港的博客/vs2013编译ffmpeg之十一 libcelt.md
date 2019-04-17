# vs2013编译ffmpeg之十一 libcelt - 枫林港的博客 - CSDN博客





2018年07月08日 22:56:47[枫林港](https://me.csdn.net/u013601132)阅读数：78标签：[libcelt																[ffmpeg																[vs2013																[编译](https://so.csdn.net/so/search/s.do?q=编译&t=blog)
个人分类：[vs2013编译ffmpeg](https://blog.csdn.net/u013601132/article/category/7785737)





# vs2013编译libcelt

对应ffmpeg configure选项–enable-libcelt。 

超低延迟的音频编码器，主要用在实时高质量的语音传输应用上。2011年以后项目就没有更新了。 

从[官网上](http://www.celt-codec.org/)下载最新的代码，源代码libcelt下面有libcelt.vcxproj，但没有sln文件。先建个win32空的工程，然后将libcelt.vcxproj加进来，按下图所示更新一下工程，点“Upgrade…”： 
![这里写图片描述](https://img-blog.csdn.net/20180703002703474?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后就可以正常编译通过了。 

最后发现ffmpeg configure会失败，libcelt和opus冲突，opus包含了celt的代码，二者有很多符号是相同的。这样肯定会冲突，所以还是只能保留opus，将libcelt里面的函数改名，毕竟opus是celt的延续和扩展。
# msys编译libcelt

也是从[官网上](http://www.celt-codec.org/)下载最新的代码，代码里面只有autogen.sh，在Msys（msys.bat里面有call “C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat”）下执行报下面的错误：

```
aclocal-1.8
aclocal: macro `_LT_DECL_SED' required but not defined
aclocal: macro `_LT_FUNC_STRIPNAME_CNF' required but not defined
```

没有找到原因，解决的办法是直接去掉\MinGW\share\aclocal\libtool.m4里面的下面两个语句：

```
AC_REQUIRE([_LT_DECL_SED])
AC_REQUIRE([_LT_FUNC_STRIPNAME_CNF])dnl
```

编译完了记得改回去。autogen.sh正常产生configure文件后，再执行：
`mkdir msys_build; ./configure --prefix=$(pwd)/msys_build`
有错误：

```
./configure: line 12010: syntax error near unexpected token `tools="tools",'
./configure: line 12010: `  XIPH_PATH_OGG(tools="tools", tools="")'
```

解决的办法是直接注释掉configure里面的下面这行：
`XIPH_PATH_OGG(tools="tools", tools="")`
configure成功后可以make了。由于当时没有msys编译的ogg，所以celt就没有ogg支持，但这样test目录下的程序都能编译出来，没有测试过。](https://so.csdn.net/so/search/s.do?q=vs2013&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)](https://so.csdn.net/so/search/s.do?q=libcelt&t=blog)




