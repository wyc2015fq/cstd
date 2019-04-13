
# 关于valgrind的安装和内存泄露分析 - 高科的专栏 - CSDN博客

2015年01月08日 11:43:41[高科](https://me.csdn.net/pbymw8iwm)阅读数：7321


程序的安装
如果使用的是tar包安装. valgrind
\# wget http://valgrind.org/downloads/valgrind-3.9.0.tar.bz2
\# tar -jxvf valgrind-3.9.0.tar.bz2
\# cd valgrind-3.9.0
\# ./autogen.sh
\# ./configure
\# make; make install

使用命令：
valgrind --tool=memcheck --leak-check=full --show-reachable=yes --log-file=./valgrind.log.`date +%m%d%s` ./gameserver
如果后台起的话，也可以使用：
nohup valgrind --tool=memcheck --leak-check=full --show-reachable=yes --log-file=./valgrind.log.`date
 +%m%d%s` ./gameserver &
**error specification**
**一、有malloc，但未free**
code
\#include<stdio.h>
\#include<stdlib.h>

void main()
{
char*p=malloc(20);
sprintf(p,"%s","test");
fprintf(stderr,"p:%s\n",p);
}

分析：
文件后部，总体来看，有确定无疑的lost 20字节。如下：
==26512==LEAK SUMMARY:
==26512==definitely lost:20 bytesin1 blocks.

在文件之前描述的内容，细节可以看出，有1个malloc，但未去free。如下：
==26512==ERRORSUMMARY:0 errors from 0 contexts(suppressed:11 from 1)
==26512==malloc/free:inuse atexit:20 bytesin1 blocks.
==26512==malloc/free:1 allocs,0 frees,20 bytes allocated.
**二、free一个未malloc的指针**code
void main()
{
char p[]="hello";
fprintf(stderr,"p:%s\n",p);
free(p);
}
分析：
文件后部，总体来看，有1个错误，0个malloc，1个free。如下：
==26786==ERRORSUMMARY:1 errors from 1 contexts(suppressed:11 from 1)
==26786==malloc/free:inuse atexit:0 bytesin0 blocks.
==26786==malloc/free:0 allocs,1 frees,0 bytes allocated.

在文件之前描述的内容，细节可以看出，有一个无效的free。如下：
==26786==Invalid free()/delete/delete[]
==26786==at 0x402265C:free(vg_replace_malloc.c:323)
==26786==by 0x804841F:main(in/home/eric/workspace/valgrind/test/a.out)

**三、stack中，无效的读取，不会提示出错**
code
void main()
{
char p[8]="hello";//p在栈上，"hello"在常量区
fprintf(stderr,"p10:%c\n",p[10]);
}
分析：
读取stack栈中的内容，不会提示错误
==27452==ERRORSUMMARY:0 errors from 0 contexts(suppressed:11 from 1)
==27452==malloc/free:inuse atexit:0 bytesin0 blocks.
==27452==malloc/free:0 allocs,0 frees,0 bytes allocated.

其为局部变量，存贮在stack中，由于p[10]未超出栈的长度，所以没提示出错
**四、heap堆中，无效的读取，会提示出错**code
void main()
{
char*p=malloc(8);
fprintf(stderr,"p10:%c\n",p[10]);
free(p);
}
分析：
读取heap堆中的内容，会提示错误，提示无效的1个字节的读取，位置是malloc的8字节的后面第2字节
==27744==Invalid read of size 1
==27744==at 0x804842A:main(in/home/eric/workspace/valgrind/test/a.out)
==27744==Address 0x4190032is2 bytes after a block of size 8 alloc'd
==27744==at 0x4022AB8:malloc(vg_replace_malloc.c:207)
==27744==by 0x8048420:main(in/home/eric/workspace/valgrind/test/a.out)
==27744==
==27744==ERRORSUMMARY:1 errors from 1 contexts(suppressed:11 from 1)
==27744==malloc/free:inuse atexit:0 bytesin0 blocks.
==27744==malloc/free:1 allocs,1 frees,8 bytes allocated.

malloc的数据在heap堆中。
**五、stack上的，无效的写**code
void main()
{
char p[8]="hello";
p[10]='a';
}
分析：
运行时会出错，会打印Backtrace
***stack smashing detected***:./a.out terminated
=======Backtrace:=========
/lib/tls/i686/cmov/libc.so.6(__fortify_fail+0x48)[0x412d138]
/lib/tls/i686/cmov/libc.so.6(__fortify_fail+0x0)[0x412d0f0]
./a.out[0x80483d6]
/lib/tls/i686/cmov/libc.so.6(__libc_start_main+0xe0)[0x4056450]
./a.out[0x8048331]
=======Memory map:========
04000000-0401a000 r-xp 00000000 08:06 682589/lib/ld-2.7.so

总体，会提示有1个错误，内容是5个malloc，0个free
==27918==ERRORSUMMARY:1 errors from 1 contexts(suppressed:13 from 1)
==27918==malloc/free:inuse atexit:892 bytesin5 blocks.
==27918==malloc/free:5 allocs,0 frees,892 bytes allocated.
==27918==LEAK SUMMARY:
==27918==definitely lost:0 bytesin0 blocks.
==27918==possibly lost:0 bytesin0 blocks.
==27918==still reachable:892 bytesin5 blocks.

文件前面的错误信息是，有一个无效read，读了4个字节。
==27918==Invalid read of size 4
==27918==at 0x40151F3:(within/lib/ld-2.7.so)
==27918==by 0x4005C69:(within/lib/ld-2.7.so)
==27918==by 0x4007A97:(within/lib/ld-2.7.so)
==27918==by 0x4011543:(within/lib/ld-2.7.so)
==27918==by 0x400D5D5:(within/lib/ld-2.7.so)
==27918==by 0x4010F5D:(within/lib/ld-2.7.so)
==27918==by 0x414E291:(within/lib/tls/i686/cmov/libc-2.7.so)
==27918==by 0x400D5D5:(within/lib/ld-2.7.so)
==27918==by 0x414E454:__libc_dlopen_mode(in/lib/tls/i686/cmov/libc-2.7.so)
==27918==by 0x412A4D8:(within/lib/tls/i686/cmov/libc-2.7.so)
==27918==by 0x412A4D8:(within/lib/tls/i686/cmov/libc-2.7.so)
==27918==by 0x412A669:backtrace(in/lib/tls/i686/cmov/libc-2.7.so)
==27918==by 0x40A3B91:(within/lib/tls/i686/cmov/libc-2.7.so)
==27918==Address 0x4190038is16 bytes inside a block of size 19 alloc'd

这个提示的错误信息和代码中的信息，没很好的匹配上，但可以知道是已经出错了。
**六、heap堆上，无效的写**code
void main()
{
char*p=malloc(8);
p[10]='a';
free(p);
}
分析：
程序运行不会出错
写heap堆中的内容，log会提示错误，提示无效的1个字节的写，位置是malloc的8字节的后面第2字节
==28351==Invalid write of size 1
==28351==at 0x80483CA:main(in/home/eric/workspace/valgrind/test/a.out)
==28351==Address 0x4190032is2 bytes after a block of size 8 alloc'd
==28351==at 0x4022AB8:malloc(vg_replace_malloc.c:207)
==28351==by 0x80483C0:main(in/home/eric/workspace/valgrind/test/a.out)



