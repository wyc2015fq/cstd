# vs2013编译ffmpeg之十七 libwebp、mfx - 枫林港的博客 - CSDN博客





2018年07月08日 23:21:52[枫林港](https://me.csdn.net/u013601132)阅读数：312








# libwebp

对应ffmpeg configure选项–enable-libwebp。 

目前用的是libwebp-0.5.1，[官网](http://downloads.webmproject.org/releases/webp/)上下载的，最新的版本是1.0.0。 

libwebp-0.5.1从源代码编译，启动msvc的prompt，在源代码路径下执行：`nmake -f Makefile.vc CFG=debug-static RTLIBCFG=static OBJDIR=output ARCH=x86 all`
libwebp-0.5.1才支持libwebp_anim_encoder，ffmpeg的configure执行完后需要检查一下config.h，CONFIG_LIBWEBP_ANIM_ENCODER值要为1。

# mfx

ffmpeg-3.1.3开始已经不支持–enable-libmfx选项了。 

从[https://github.com/lu-zero/mfx_dispatch](https://github.com/lu-zero/mfx_dispatch)下载，打开libmfx.sln后，工程需要添加两个文件：mfx_load_plugin.cpp、mfx_plugin_hive.cpp，不知道原始的工程里面为何没有这两个文件，不然ffmpeg在configure的时候会报错。




