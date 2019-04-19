# glibc 2.3分析准备 - xqhrs232的专栏 - CSDN博客
2016年10月21日 18:16:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：303
原文地址::[http://blog.chinaunix.net/uid-725631-id-253178.html](http://blog.chinaunix.net/uid-725631-id-253178.html)
本分析基于glib 2.3
1.编译安装glibc 2.3
先要安装内核头文件
例如 rpm -ivh kernel-devel-2.6.9-5.EL.i686.rpm
使用普通用户登录(这样安装时不会对系统造成不必要的破坏)
在ftp://ftp.gnu.org上下载
下载glibc-2.3.tar.bz2和glibc-linuxthreads-2.3.tar.bz2
解压
tar -xjf  glibc-2.3.tar.bz2
tar -xjf linuxthreads-2.3.tar.bz2 -C glibc-2.3
cd glibc-2.3
mkdir build(在单独的目录中编译)
cd build
../configure --prefix=~/local --enable-add-ons=yes --disable-sanity-checks (prefix为home目录下的local)
make 2>&1 |tee make.txt (将输出写到文件中)
make install 2>&1 |tee install.txt (将输出写到文件中)
2.使用工具sourceinsight看glibc2.3的代码发现,其中使用了大量的宏和条件编译，很容易使人迷失方向，失去耐心，怎么办?
我的方法是，通过分析前面的make.txt和install.txt来找到编译 该文件的命令
例如编译elf/rtld.c的命令如下
gcc rtld.c -c -O -Wall -Winline -Wstrict-prototypes -Wwrite-strings -g   -fPIC -DNOT_IN_libc -DIS_IN_rtld -Wno-uninitialized -Wno-unused   -I../include -I. -I/home/zws/glibc-2.3/build/elf
 -I.. -I../libio  -I/home/zws/glibc-2.3/build -I../sysdeps/i386/elf -I../linuxthreads/sysdeps/unix/sysv/linux/i386 -I../linuxthreads/sysdeps/unix/sysv/linux -I../linuxthreads/sysdeps/pthread -I../sysdeps/pthread -I../linuxthreads/sysdeps/unix/sysv -I../linuxthreads/sysdeps/unix
 -I../linuxthreads/sysdeps/i386/i686 -I../linuxthreads/sysdeps/i386 -I../sysdeps/unix/sysv/linux/i386 -I../sysdeps/unix/sysv/linux -I../sysdeps/gnu -I../sysdeps/unix/common -I../sysdeps/unix/mman -I../sysdeps/unix/inet -I../sysdeps/unix/sysv/i386 -I../sysdeps/unix/sysv
 -I../sysdeps/unix/i386 -I../sysdeps/unix -I../sysdeps/posix -I../sysdeps/i386/i686/fpu -I../sysdeps/i386/i686 -I../sysdeps/i386/i486 -I../sysdeps/i386/fpu -I../sysdeps/i386 -I../sysdeps/wordsize-32 -I../sysdeps/ieee754/ldbl-96 -I../sysdeps/ieee754/dbl-64 -I../sysdeps/ieee754/flt-32
 -I../sysdeps/ieee754 -I../sysdeps/generic/elf -I../sysdeps/generic  -I /lib/modules/2.4.20-8smp/build/include -D_LIBC_REENTRANT -include ../include/libc-symbols.h  -DPIC -DSHARED    -o /home/zws/glibc-2.3/build/elf/rtld.os
将最后的 -o 选项改成
-E -dD > rtld.dd.txt
这样会生成预处理过的输出文件(并且包含宏定义),对照这个文件，就很容易分析出代码的实际逻辑。
3.如何开始
glibc包含了ld.so,libc.so,libdl.so等十几动态连接库,我们选择从ld.so开始，因为它是linux下的动态链接器，非常重要。ld.so的更一般的名字是ld-linux.so.2.
4.什么是ld-linux.so.2
ld-linux.so.2是一个大家既陌生又熟悉的东西，说他陌生是因为我们平常工作中很少直接涉及，说它熟悉是因为我们无时不刻都要用到它，只是我们没有察觉到。
它就是linux中的运行时动态链接器。
运行时动态链接器的作用是在程序执行时进行加载必要的动态连接库,并完成符号链接。
linux内核在执行可执行文件时，先加载可执行文件，再查找可执行文件是否有INTERP程序头，如果有的话，里面应该指出运行时动态连接器(又称为解释器)的地址。
例如
[zws@mail zws]$ readelf  -l /bin/ls
Elf file type is EXEC (Executable file)
Entry point 0x8049690
There are 7 program headers, starting at offset 52
Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  PHDR           0x000034 0x08048034 0x08048034 0x000e0 0x000e0 R E 0x4
  INTERP         0x000114 0x08048114 0x08048114 0x00013 0x00013 R   0x1
      [Requesting program interpreter: /lib/ld-linux.so.2]
  LOAD           0x000000 0x08048000 0x08048000 0x0fa98 0x0fa98 R E 0x1000
  LOAD           0x010000 0x08058000 0x08058000 0x00348 0x006c8 RW  0x1000
  DYNAMIC        0x010114 0x08058114 0x08058114 0x000d0 0x000d0 RW  0x4
  NOTE           0x000128 0x08048128 0x08048128 0x00020 0x00020 R   0x4
  GNU_EH_FRAME   0x00f960 0x08057960 0x08057960 0x0002c 0x0002c R   0x4
 Section to Segment mapping:
  Segment Sections...
   00     
   01     .interp 
   02     .interp .note.ABI-tag .hash .dynsym .dynstr .gnu.version .gnu.version_r .rel.dyn .rel.plt .init .plt .text .fini .rodata .eh_frame_hdr .eh_frame 
   03     .data .dynamic .ctors .dtors .jcr .got .bss 
   04     .dynamic 
   05     .note.ABI-tag 
   06     .eh_frame_hdr 
这里指出ls的解释器是/lib/ld-linux.so.2
然后内核会加载/lib/ld-linux.so.2,可执行文件的入口地址(例如_start)，命令行参数，环境变量等信息传给它，跳到ld-linux.so.2的入口开始执行。
ld-linux.so.2完成加载库和链接符号后，跳到可执行文件的入口执行
ld-linux.so.2其实是一个符号链接
[zws@mail zws]$ ls -l /lib/ld-linux.so.2 
lrwxrwxrwx    1 root     root           11 Sep 24  2004 /lib/ld-linux.so.2 -> ld-2.3.2.so
5.glibc 2.3中ld.so的组成
生成ld.so的命令是
gcc   -nostdlib -nostartfiles -r -o /home/zws/glibc-2.3/build/elf/dl-allobjs.os /home/zws/glibc-2.3/build/elf/rtld.os /home/zws/glibc-2.3/build/elf/dl-load.os /home/zws/glibc-2.3/build/elf/dl-cache.os
 /home/zws/glibc-2.3/build/elf/dl-lookup.os /home/zws/glibc-2.3/build/elf/dl-object.os /home/zws/glibc-2.3/build/elf/dl-reloc.os /home/zws/glibc-2.3/build/elf/dl-deps.os /home/zws/glibc-2.3/build/elf/dl-runtime.os /home/zws/glibc-2.3/build/elf/dl-error.os /home/zws/glibc-2.3/build/elf/dl-init.os
 /home/zws/glibc-2.3/build/elf/dl-fini.os /home/zws/glibc-2.3/build/elf/dl-debug.os /home/zws/glibc-2.3/build/elf/dl-misc.os /home/zws/glibc-2.3/build/elf/dl-version.os /home/zws/glibc-2.3/build/elf/dl-profile.os /home/zws/glibc-2.3/build/elf/dl-conflict.os
 /home/zws/glibc-2.3/build/elf/dl-tls.os /home/zws/glibc-2.3/build/elf/dl-origin.os /home/zws/glibc-2.3/build/elf/dl-sysdep.os /home/zws/glibc-2.3/build/elf/dl-environ.os /home/zws/glibc-2.3/build/elf/dl-minimal.os /home/zws/glibc-2.3/build/elf/dl-xstat64.os
 /home/zws/glibc-2.3/build/elf/dl-fxstat64.os /home/zws/glibc-2.3/build/elf/dl-brk.os /home/zws/glibc-2.3/build/elf/dl-sbrk.os
gcc   -nostdlib -nostartfiles -r -o /home/zws/glibc-2.3/build/elf/librtld.os '-Wl,-(' /home/zws/glibc-2.3/build/elf/dl-allobjs.os /home/zws/glibc-2.3/build/libc_pic.a -lgcc '-Wl,-)'
gcc   -nostdlib -nostartfiles -shared            \
  -Wl,-z,combreloc -Wl,--verbose 2>&1 |            \
      sed -e '/^=========/,/^=========/!d;/^=========/d'    \
          -e 's/\. = 0 + SIZEOF_HEADERS;/& _begin = . - SIZEOF_HEADERS;/' \
      > /home/zws/glibc-2.3/build/elf/ld.so.lds
gcc   -nostdlib -nostartfiles -shared -o /home/zws/glibc-2.3/build/elf/ld.so -Wl,-z,combreloc    \
      /home/zws/glibc-2.3/build/elf/librtld.os -Wl,--version-script=/home/zws/glibc-2.3/build/ld.map        \
      -Wl,-soname=ld-linux.so.2 -T /home/zws/glibc-2.3/build/elf/ld.so.lds
dl-allobjs.os包含所有的动态连接用到的核心代码
/home/zws/glibc-2.3/build/elf/rtld.os 
/home/zws/glibc-2.3/build/elf/dl-load.os 
/home/zws/glibc-2.3/build/elf/dl-cache.os 
/home/zws/glibc-2.3/build/elf/dl-lookup.os 
/home/zws/glibc-2.3/build/elf/dl-object.os 
/home/zws/glibc-2.3/build/elf/dl-reloc.os 
/home/zws/glibc-2.3/build/elf/dl-deps.os 
/home/zws/glibc-2.3/build/elf/dl-runtime.os 
/home/zws/glibc-2.3/build/elf/dl-error.os 
/home/zws/glibc-2.3/build/elf/dl-init.os 
/home/zws/glibc-2.3/build/elf/dl-fini.os 
/home/zws/glibc-2.3/build/elf/dl-debug.os 
/home/zws/glibc-2.3/build/elf/dl-misc.os 
/home/zws/glibc-2.3/build/elf/dl-version.os 
/home/zws/glibc-2.3/build/elf/dl-profile.os 
/home/zws/glibc-2.3/build/elf/dl-conflict.os 
/home/zws/glibc-2.3/build/elf/dl-tls.os 
/home/zws/glibc-2.3/build/elf/dl-origin.os 
/home/zws/glibc-2.3/build/elf/dl-sysdep.os 
/home/zws/glibc-2.3/build/elf/dl-environ.os 
/home/zws/glibc-2.3/build/elf/dl-minimal.os 
/home/zws/glibc-2.3/build/elf/dl-xstat64.os 
/home/zws/glibc-2.3/build/elf/dl-fxstat64.os 
/home/zws/glibc-2.3/build/elf/dl-brk.os 
/home/zws/glibc-2.3/build/elf/dl-sbrk.os
libc_pic.a 包含组成libc的目标文件      
ar t libc_pic.a
init-first.os
libc-start.os
sysdep.os
version.os
check_fds.os
errno-loc.os
divdi3.os
iconv_open.os
...
dl-iteratephdr.os
dl-addr.os
dl-profstub.os
dl-libc.os
dl-sym.os
unwind-dw2-fde-glibc.os
framestate.os
总共1221个文件,libc.so也是由这些文件编译生成的
gcc   -nostdlib -nostartfiles -r -o /home/zws/glibc-2.3/build/elf/librtld.os '-Wl,-(' /home/zws/glibc-2.3/build/elf/dl-allobjs.os /home/zws/glibc-2.3/build/libc_pic.a -lgcc '-Wl,-)'
的目的是将dl-allobjs.os中引用了libc的函数静态链接到librtld.os中
ld.so.lds 链接脚本
gcc   -nostdlib -nostartfiles -shared            \
  -Wl,-z,combreloc -Wl,--verbose 2>&1 |            \
      sed -e '/^=========/,/^=========/!d;/^=========/d'    \
          -e 's/\. = 0 + SIZEOF_HEADERS;/& _begin = . - SIZEOF_HEADERS;/' \
      > /home/zws/glibc-2.3/build/elf/ld.so.lds
/* Script for --shared -z combreloc: shared library, combine & sort relocs */
OUTPUT_FORMAT("elf32-i386", "elf32-i386",
              "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
SEARCH_DIR("/usr/local/i686-pc-linux-gnu/lib"); SEARCH_DIR("/usr/local/lib"); SEARCH_DIR("/lib"); SEARCH_DIR("/usr/lib");
SECTIONS
{
  /* Read-only sections, merged into text segment: */
  . = 0 + SIZEOF_HEADERS; _begin = . - SIZEOF_HEADERS;
  .note.gnu.build-id : { *(.note.gnu.build-id) }
  .hash           : { *(.hash) }
  .gnu.hash       : { *(.gnu.hash) }
  .dynsym         : { *(.dynsym) }
  .dynstr         : { *(.dynstr) }
  .gnu.version    : { *(.gnu.version) }
  .gnu.version_d  : { *(.gnu.version_d) }
  .gnu.version_r  : { *(.gnu.version_r) }
  .rel.dyn        :
    {
      *(.rel.init)
      *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*)
      *(.rel.fini)
      *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*)
      *(.rel.data.rel.ro* .rel.gnu.linkonce.d.rel.ro.*)
      *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*)
      *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*)
      *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*)
      *(.rel.ctors)
      *(.rel.dtors)
      *(.rel.got)
      *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*)
    }
  .rela.dyn       :
    {
      *(.rela.init)
      *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
      *(.rela.fini)
      *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
      *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
      *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*)
      *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*)
      *(.rela.ctors)
      *(.rela.dtors)
      *(.rela.got)
      *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*)
    }
  .rel.plt        : { *(.rel.plt) }
  .rela.plt       : { *(.rela.plt) }
  .init           :
  {
    KEEP (*(.init))
  } =0x90909090
  .plt            : { *(.plt) }
  .text           :
  {
    *(.text .stub .text.* .gnu.linkonce.t.*)
    KEEP (*(.text.*personality*))
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
  } =0x90909090
  .fini           :
  {
    KEEP (*(.fini))
  } =0x90909090
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  .rodata1        : { *(.rodata1) }
  .eh_frame_hdr : { *(.eh_frame_hdr) }
  .eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) }
  .gcc_except_table   : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) }
  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = ALIGN (CONSTANT (MAXPAGESIZE)) - ((CONSTANT (MAXPAGESIZE) - .) & (CONSTANT (MAXPAGESIZE) - 1)); . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
  /* Exception handling  */
  .eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) }
  .gcc_except_table   : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) }
  /* Thread Local Storage sections  */
  .tdata          : { *(.tdata .tdata.* .gnu.linkonce.td.*) }
  .tbss           : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) }
  .preinit_array     :
  {
    KEEP (*(.preinit_array))
  }
  .init_array     :
  {
     KEEP (*(SORT(.init_array.*)))
     KEEP (*(.init_array))
  }
  .fini_array     :
  {
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
  }
  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  }
  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  }
  .jcr            : { KEEP (*(.jcr)) }
  .data.rel.ro : { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro* .gnu.linkonce.d.rel.ro.*) }
  .dynamic        : { *(.dynamic) }
  .got            : { *(.got) }
  . = DATA_SEGMENT_RELRO_END (12, .);
  .got.plt        : { *(.got.plt) }
  .data           :
  {
    *(.data .data.* .gnu.linkonce.d.*)
    KEEP (*(.gnu.linkonce.d.*personality*))
    SORT(CONSTRUCTORS)
  }
  .data1          : { *(.data1) }
  _edata = .; PROVIDE (edata = .);
  __bss_start = .;
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.
      FIXME: Why do we need it? When there is no .bss section, we don't
      pad the .data section.  */
   . = ALIGN(. != 0 ? 32 / 8 : 1);
  }
  . = ALIGN(32 / 8);
  . = ALIGN(32 / 8);
  _end = .; PROVIDE (end = .);
  . = DATA_SEGMENT_END (.);
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* DWARF 3 */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) }
}
最后使用该链接脚本生成ld.so
gcc   -nostdlib -nostartfiles -shared -o /home/zws/glibc-2.3/build/elf/ld.so -Wl,-z,combreloc    \
      /home/zws/glibc-2.3/build/elf/librtld.os -Wl,--version-script=/home/zws/glibc-2.3/build/ld.map        \
      -Wl,-soname=ld-linux.so.2 -T /home/zws/glibc-2.3/build/elf/ld.so.lds
6.glibc中目标文件
查看make.txt可以发现，glibc编译中共生成三种类型目标文件,*.o,*.op,*.os
例如
[zws@mail ~/glibc-2.3/build/elf]$ ls dl-load.*
dl-load.d  dl-load.o  dl-load.op  dl-load.os
生成这些文件的命令分别是
gcc dl-load.c -c -O -Wall -Winline -Wstrict-prototypes -Wwrite-strings -g   -Wno-unused   -I../include -I. -I/home/zws/glibc-2.3/build/elf -I.. -I../libio  -I/home/zws/glibc-2.3/build
 -I../sysdeps/i386/elf -I../linuxthreads/sysdeps/unix/sysv/linux/i386 -I../linuxthreads/sysdeps/unix/sysv/linux -I../linuxthreads/sysdeps/pthread -I../sysdeps/pthread -I../linuxthreads/sysdeps/unix/sysv -I../linuxthreads/sysdeps/unix -I../linuxthreads/sysdeps/i386/i686
 -I../linuxthreads/sysdeps/i386 -I../sysdeps/unix/sysv/linux/i386 -I../sysdeps/unix/sysv/linux -I../sysdeps/gnu -I../sysdeps/unix/common -I../sysdeps/unix/mman -I../sysdeps/unix/inet -I../sysdeps/unix/sysv/i386 -I../sysdeps/unix/sysv -I../sysdeps/unix/i386
 -I../sysdeps/unix -I../sysdeps/posix -I../sysdeps/i386/i686/fpu -I../sysdeps/i386/i686 -I../sysdeps/i386/i486 -I../sysdeps/i386/fpu -I../sysdeps/i386 -I../sysdeps/wordsize-32 -I../sysdeps/ieee754/ldbl-96 -I../sysdeps/ieee754/dbl-64 -I../sysdeps/ieee754/flt-32
 -I../sysdeps/ieee754 -I../sysdeps/generic/elf -I../sysdeps/generic  -I /lib/modules/2.4.20-8smp/build/include -D_LIBC_REENTRANT -include ../include/libc-symbols.h   -I/home/zws/glibc-2.3/build/elf/. -I/home/zws/glibc-2.3/build/csu/.   -o /home/zws/glibc-2.3/build/elf/dl-load.o
gcc dl-load.c -c -O -Wall -Winline -Wstrict-prototypes -Wwrite-strings -g  -pg -Wno-unused   -I../include -I. -I/home/zws/glibc-2.3/build/elf -I.. -I../libio  -I/home/zws/glibc-2.3/build
 -I../sysdeps/i386/elf -I../linuxthreads/sysdeps/unix/sysv/linux/i386 -I../linuxthreads/sysdeps/unix/sysv/linux -I../linuxthreads/sysdeps/pthread -I../sysdeps/pthread -I../linuxthreads/sysdeps/unix/sysv -I../linuxthreads/sysdeps/unix -I../linuxthreads/sysdeps/i386/i686
 -I../linuxthreads/sysdeps/i386 -I../sysdeps/unix/sysv/linux/i386 -I../sysdeps/unix/sysv/linux -I../sysdeps/gnu -I../sysdeps/unix/common -I../sysdeps/unix/mman -I../sysdeps/unix/inet -I../sysdeps/unix/sysv/i386 -I../sysdeps/unix/sysv -I../sysdeps/unix/i386
 -I../sysdeps/unix -I../sysdeps/posix -I../sysdeps/i386/i686/fpu -I../sysdeps/i386/i686 -I../sysdeps/i386/i486 -I../sysdeps/i386/fpu -I../sysdeps/i386 -I../sysdeps/wordsize-32 -I../sysdeps/ieee754/ldbl-96 -I../sysdeps/ieee754/dbl-64 -I../sysdeps/ieee754/flt-32
 -I../sysdeps/ieee754 -I../sysdeps/generic/elf -I../sysdeps/generic  -I /lib/modules/2.4.20-8smp/build/include -D_LIBC_REENTRANT -include ../include/libc-symbols.h  -DPROF  -I/home/zws/glibc-2.3/build/elf/. -I/home/zws/glibc-2.3/build/csu/.   -o /home/zws/glibc-2.3/build/elf/dl-load.op
gcc dl-load.c -c -O -Wall -Winline -Wstrict-prototypes -Wwrite-strings -g   -fPIC -DNOT_IN_libc -DIS_IN_rtld -Wno-unused   -I../include -I. -I/home/zws/glibc-2.3/build/elf -I.. -I../libio 
 -I/home/zws/glibc-2.3/build -I../sysdeps/i386/elf -I../linuxthreads/sysdeps/unix/sysv/linux/i386 -I../linuxthreads/sysdeps/unix/sysv/linux -I../linuxthreads/sysdeps/pthread -I../sysdeps/pthread -I../linuxthreads/sysdeps/unix/sysv -I../linuxthreads/sysdeps/unix
 -I../linuxthreads/sysdeps/i386/i686 -I../linuxthreads/sysdeps/i386 -I../sysdeps/unix/sysv/linux/i386 -I../sysdeps/unix/sysv/linux -I../sysdeps/gnu -I../sysdeps/unix/common -I../sysdeps/unix/mman -I../sysdeps/unix/inet -I../sysdeps/unix/sysv/i386 -I../sysdeps/unix/sysv
 -I../sysdeps/unix/i386 -I../sysdeps/unix -I../sysdeps/posix -I../sysdeps/i386/i686/fpu -I../sysdeps/i386/i686 -I../sysdeps/i386/i486 -I../sysdeps/i386/fpu -I../sysdeps/i386 -I../sysdeps/wordsize-32 -I../sysdeps/ieee754/ldbl-96 -I../sysdeps/ieee754/dbl-64 -I../sysdeps/ieee754/flt-32
 -I../sysdeps/ieee754 -I../sysdeps/generic/elf -I../sysdeps/generic  -I /lib/modules/2.4.20-8smp/build/include -D_LIBC_REENTRANT -include ../include/libc-symbols.h  -DPIC -DSHARED -I/home/zws/glibc-2.3/build/elf/. -I/home/zws/glibc-2.3/build/csu/.   -o /home/zws/glibc-2.3/build/elf/dl-load.os
三条命名的差别是
*.o 使用 -g  生成一般代码
*.op 使用 -g -pg 生成性能剖析代码
*.os 使用 -g -fPIC 生成PIC代码
编译时libc会生成三个归档文件
[zws@mail ~/glibc-2.3/build/build]$ ls *.a
libc.a  libc_nonshared.a  libc_p.a  libc_pic.a
通过ar程序可以发现
libc.a是由*.o组成
libc_p.a是由*.op组成
libc_pic.a是由*.os组成
推而广之
[zws@mail ~/local/lib]$ ls *.a
libanl.a             libc.a            libdl.a      libm.a       libpthread.a    librpcsvc_p.a
libanl_p.a           libc_nonshared.a  libdl_p.a    libmcheck.a  libpthread_p.a  librt.a
libBrokenLocale.a    libc_p.a          libg.a       libm_p.a     libresolv.a     librt_p.a
libBrokenLocale_p.a  libcrypt.a        libiberty.a  libnsl.a     libresolv_p.a   libutil.a
libbsd-compat.a      libcrypt_p.a      libieee.a    libnsl_p.a   librpcsvc.a     libutil_p.a
大家都能看出glibc被安装的各个归档文件的异同
