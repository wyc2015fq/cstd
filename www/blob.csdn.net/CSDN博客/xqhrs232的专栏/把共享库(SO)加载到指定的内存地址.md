# 把共享库(SO)加载到指定的内存地址 - xqhrs232的专栏 - CSDN博客
2016年10月21日 17:35:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：534
原文地址::[http://blog.chinaunix.net/uid-28978412-id-3867012.html](http://blog.chinaunix.net/uid-28978412-id-3867012.html)
from：[http://blog.csdn.net/absurd/article/details/860140](http://blog.csdn.net/absurd/article/details/860140)   
 又是一个牛人的博客
一位朋友最近遇到一个棘手的问题，希望把共享库(SO)加载到指定的内存地址，目的可能是想通过prelink来加快应用程序的起动速度。他问我有没有什么方法。我知道Windows下是可以的，比如在VC6里设置/base的值就行了，所以相信在linux下也是可行的。
      VC有编译选项可以设置，猜想gcc也应该有吧。gcc本身只是一个外壳，链接工作是由于ld完成的，当然是应该去阅读ld命令行选项文档。很快发现ld有个—image-base选项，可以设置动态库的加载地址。
    通过Xlinker把这个参数传递给ld，但是ld不能识别这个选项：
gcc -g -shared test.c -Xlinker --image-base -Xlinker 0x00c00000 -o libtest.so
/usr/bin/ld: unrecognized option '--image-base'
/usr/bin/ld: use the --help option for usage information
collect2: ld returned 1 exit status
    再仔细看手册，原来这个选项只适用于PE文件，PE文件是Windows下专用的，在linux下自然用不了，看来得另想办法。
    我知道ld script可以控制ld的行为，于是研究ld
 script的写法，按照手册里的说明，写了一个简单的ld script:
SECTIONS
{
. = 0x00c00000;
.text : { *(.text) }
.data : { *(.data) }
.bss : { *(.bss) }
}
按下列方式编译：
gcc -shared -g -Xlinker --script -Xlinker ld.s test.c -o libtest.so
gcc -g main.c -L./ -ltest -o test.exe
用ldd查看，加载地址正确。
[root@localhost lds]# ldd test.exe
linux-gate.so.1 =>(0x00aff000)
libtest.so => ./libtest.so (0x00c00000)
libc.so.6 => /lib/libc.so.6 (0x007fa000)
/lib/ld-linux.so.2 (0x007dd000)
但运行时会crash:
[root@localhost lds]# ./test.exe
# Segmentation fault
调试运行可以发现：
(gdb) r
Starting program: /work/test/lds/test.exe
Reading symbols from shared object read from target memory...done.
Loaded system supplied DSO at 0x452000
Program received signal SIGSEGV, Segmentation fault.
0x007e7a10 in _dl_relocate_object () from /lib/ld-linux.so.2
(gdb) bt
#00x007e7a10 in _dl_relocate_object () from /lib/ld-linux.so.2
#10x007e0833 in dl_main () from /lib/ld-linux.so.2
#20x007f056b in _dl_sysdep_start () from /lib/ld-linux.so.2
#30x007df48f in _dl_start () from /lib/ld-linux.so.2
#40x007dd847 in _start () from /lib/ld-linux.so.2
猜想可能是ld.s写得不全，导致一些信息不正确。于是用ld
 –verbose导出一个默认的ld script。不出所料，默认的ld
 script非常冗长，下面是开头一段：
/* Script for -z combreloc: combine and sort reloc sections */
OUTPUT_FORMAT("elf32-i386", "elf32-i386",
"elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
SEARCH_DIR("/usr/i386-redhat-linux/lib"); SEARCH_DIR("/usr/local/lib"); SEARCH_DIR("/lib"); SEARCH_DIR("/usr/lib");
SECTIONS
{
/* Read-only sections, merged into text segment: */
PROVIDE (__executable_start = 0x08048000); . = 0x08048000 + SIZEOF_HEADERS;
.interp: { *(.interp) }
.hash: { *(.hash) }
.dynsym: { *(.dynsym) }
.dynstr: { *(.dynstr) }
.gnu.version: { *(.gnu.version) }
.gnu.version_d: { *(.gnu.version_d) }
.gnu.version_r: { *(.gnu.version_r) }
按照ld script的语法，我把它修改为(红色部分为新增行)：
/* Script for -z combreloc: combine and sort reloc sections */
OUTPUT_FORMAT("elf32-i386", "elf32-i386",
"elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
SEARCH_DIR("/usr/i386-redhat-linux/lib"); SEARCH_DIR("/usr/local/lib"); SEARCH_DIR("/lib"); SEARCH_DIR("/usr/lib");
SECTIONS
{
/* Read-only sections, merged into text segment: */
PROVIDE (__executable_start = 0x08048000); . = 0x08048000 + SIZEOF_HEADERS;
. = 0x00c00000;
.interp: { *(.interp) }
.hash: { *(.hash) }
.dynsym: { *(.dynsym) }
.dynstr: { *(.dynstr) }
.gnu.version: { *(.gnu.version) }
.gnu.version_d: { *(.gnu.version_d) }
.gnu.version_r: { *(.gnu.version_r) }
用这个ld script再次测试，一切正常。又验证多个共享库的情况，也正常，下面是测试数据：
test.c
int test(intn)
{
returnn;
}
test1.c
inttest1(intn)
{
returnn;
}
main.c
externinttest(intn);
externinttest1(intn);
#include <stdio.h>
intmain(intargc, char* argv[])
{
printf("Hello:
 %d %d/n", test(100), test1(200));
getchar();
return 0;
}
Makefile
all:
gcc -shared -g -Xlinker --script -Xlinker ld.s test.c -o libtest.so
gcc -shared -g -Xlinker --script -Xlinker ld1.s test1.c -o libtest1.so
gcc -g main.c -L./ -ltest -ltest1 -o test.exe
clean:
rm -f *.so *.exe
libtest.so的加载地址为：0x00c00000
libtest1.so的加载地址为：0x00d00000
ldd显示结果：
linux-gate.so.1 =>(0x00aa3000)
libtest.so => ./libtest.so (0x00c00000)
libtest1.so => ./libtest1.so (0x00d00000)
libc.so.6 => /lib/libc.so.6 (0x007fa000)
/lib/ld-linux.so.2 (0x007dd000)
maps的内容为：
007dd000-007f6000 r-xp 00000000 03:01 521466/lib/ld-2.4.so
007f6000-007f7000 r-xp 00018000 03:01 521466/lib/ld-2.4.so
007f7000-007f8000 rwxp 00019000 03:01 521466/lib/ld-2.4.so
007fa000-00926000 r-xp 00000000 03:01 523579/lib/libc-2.4.so
00926000-00929000 r-xp 0012b000 03:01 523579/lib/libc-2.4.so
00929000-0092a000 rwxp 0012e000 03:01 523579/lib/libc-2.4.so
0092a000-0092d000 rwxp 0092a000 00:00 0
00c00000-00c01000 r-xp 00001000 03:03 16370/work/test/ldsex/libtest.so
00c01000-00c02000 rwxp 00001000 03:03 16370/work/test/ldsex/libtest.so
00cf1000-00cf2000 r-xp 00cf1000 00:00 0[vdso]
00d00000-00d01000 r-xp 00001000 03:03 16373/work/test/ldsex/libtest1.so
00d01000-00d02000 rwxp 00001000 03:03 16373/work/test/ldsex/libtest1.so
08048000-08049000 r-xp 00000000 03:03 16374/work/test/ldsex/test.exe
08049000-0804a000 rw-p 00000000 03:03 16374/work/test/ldsex/test.exe
b7fdf000-b7fe0000 rw-p b7fdf000 00:00 0
b7fed000-b7ff0000 rw-p b7fed000 00:00 0
bf8db000-bf8f0000 rw-p bf8db000 00:00 0[stack]
[](http://blog.chinaunix.net/uid-28978412-id-3867012.html#)[](http://blog.chinaunix.net/uid-28978412-id-3867012.html#)[](http://blog.chinaunix.net/uid-28978412-id-3867012.html#)[](http://blog.chinaunix.net/uid-28978412-id-3867012.html#)[](http://blog.chinaunix.net/uid-28978412-id-3867012.html#)
