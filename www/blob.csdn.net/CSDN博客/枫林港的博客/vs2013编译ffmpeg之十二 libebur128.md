# vs2013编译ffmpeg之十二 libebur128 - 枫林港的博客 - CSDN博客





2018年07月08日 23:05:07[枫林港](https://me.csdn.net/u013601132)阅读数：95标签：[libebur128																[ffmpeg																[vs2013																[编译](https://so.csdn.net/so/search/s.do?q=编译&t=blog)
个人分类：[vs2013编译ffmpeg](https://blog.csdn.net/u013601132/article/category/7785737)





# libebur128

从[https://github.com/jiixyj/libebur128](https://github.com/jiixyj/libebur128)下载代码。 

libebur128，响度归一化（Loudness Normalization）标准“EBU R 128”的库。相关资料：[http://k.ylo.ph/2016/04/04/loudnorm.html](http://k.ylo.ph/2016/04/04/loudnorm.html)。 

ffmpeg-3.0默认支持EBUR128_filter，没有–enable-libebur128选项，高版本3.1.3才支持这个选项，config.h里面CONFIG_EBUR128_FILTER的值就是1。 

libebur128的编译：
```bash
mkdir build; cd build; export PKG_CONFIG_PATH=$(pwd)/../pc_files; cmake -G "Visual Studio 12 2013" --debug-output --trace  ..
```

libebur128目录下建立pc_files目录，存放speexdsp.pc，内容如下：

```bash
# speexdsp.pc.  Generated from fribidi.pc.in by configure.

prefix=../../speexdsp-1.2rc3
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: GNU speexdsp
Description: Speexdsp
#Requires: 
Version: 1.2rc3
Libs: -L${libdir}  libspeexdsp.lib
Cflags: -I${includedir}
```

cmake会用pkg-config来搜索speexdsp的库，这里先用“export PKG_CONFIG_PATH=$(pwd)/../pc_files”来指定pkg-config的搜索路径。要仔细确认一下cmake的打印，确定检测到了speexdsp。 

产生sln文件后就可以编译了。将工程设置里面的绝对路径都改成相对路径。下面的编译错误直接注释掉出错的那行：
`ebur128.c(413): fatal error C1021: invalid preprocessor command 'warning'`](https://so.csdn.net/so/search/s.do?q=vs2013&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)](https://so.csdn.net/so/search/s.do?q=libebur128&t=blog)




