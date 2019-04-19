# 集成ffmpeg/x264:ERROR: libx264 not found的问题 - maopig的专栏 - CSDN博客
2011年07月06日 18:24:06[maopig](https://me.csdn.net/maopig)阅读数：10124标签：[header																[file																[gcc																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)
个人分类：[ffmpeg																[x264 h264](https://blog.csdn.net/maopig/article/category/847676)](https://blog.csdn.net/maopig/article/category/847674)
集成ffmpeg/x264:ERROR: libx264 not found的问题
   --拔剑
集成ffmpeg/x264碰到如下问题：
    ERROR: libx264 not found
察看config.log,详细信息如下：
      check_lib x264.h x264_encoder_encode -lx264
       check_header x264.h
       check_cpp
       BEGIN/tmp/ffconf.isuazGlg.c
          1   #include<x264.h>
          2   int x;
       END/tmp/ffconf.isuazGlg.c
       gcc -D_ISOC99_SOURCE-D_POSIX_C_SOURCE=200112 -D_FILE_OFFSET_BITS=64 -D_LARGEFIL
       E_SOURCE -U__STRICT_ANSI__-I/d/MinGW/msys/1.0/include -std=c99 -fno-common -fom
       it-frame-pointer -E -o/tmp/ffconf.ClhyDYvC.o /tmp/ffconf.isuazGlg.c
      C:/DOCUME~1/ibm/LOCALS~1/Temp/ffconf.isuazGlg.c:1:18:fatal error: x264.h: No su
       ch file or directory^M
       compilationterminated.^M
       ERROR: libx264 notfound:
所以实际原因是x264 make install 的东西ffmpegconfigure没有找到x264.h这个文件。查看我的x264
make install的结果，有：
      install -d /usr/local/bin
      install -d /usr/local/include
      install -d /usr/local/lib
      install -d /usr/local/lib/pkgconfig
      install -m 644 x264.h /usr/local/include
      install -m 644 x264_config.h/usr/local/include
      install -m 644 libx264.a /usr/local/lib
      install -m 644 x264.pc/usr/local/lib/pkgconfig
      install x264.exe /usr/local/bin
      ranlib /usr/local/lib/libx264.a
      install -m 755 libx264-115.dll/usr/local/bin
      install -m 644 libx264.dll.a/usr/local/lib
因此，只要在配制configure时，添加额外的include/link路径应该能够解决问题，即增加开关：
      --extra-cflags=-I/usr/local/include--extra-ldflags=-L/usr/local/lib
这个需要根据你x264编译、安装的实际配制修改。Finally, theconfigure command for ffmpeg is
changed to :
$./configure --enable-memalign-hack --enable-ffserver--enable-network --enable-protocols --enable-muxers --disable-yasm--enable-shared--enable-w32threads --enable-libx264 --enable-gpl--extra-cflags=-I/usr/local/include--extra-ldflags=-L/usr/local/lib
运行，一切无误！
