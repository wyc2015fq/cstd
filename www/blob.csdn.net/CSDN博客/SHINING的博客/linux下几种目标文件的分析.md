# linux下几种目标文件的分析 - SHINING的博客 - CSDN博客
2012年12月21日 15:18:01[snsn1984](https://me.csdn.net/snsn1984)阅读数：16875
本文中用到的命令：
gcc -c addvec.c 
生成可重定位目标文件addvec.o
readelf addvec.o -a
读取可重定位目标文件addvec.o
gcc -O2 -c main.c
生成可重定位目标文件main.o
gcc -static -o vecadd addvec.o main.o
链接目标文件addvec.o,生成可执行文件vecadd
gcc -shared -fPIC -o libvector.so addvec.c
从addvec.c生成共享目标文件libvector.so
gcc -o vecadd2 main.c ./libvector.so 
链接共享目标文件libvector.so生成vecadd2.
正文：
目标文件分为可执行目标文件，可重定位目标文件和共享目标文件。
也有对应的成为可执行文件，目标文件，共享库。但是只是说法不同，指的都是同样的东西。
有不少资料介绍这些知识，我们这里看具体看看它们到底是什么样的。
首先写一个简单的函数：
void addvec(int *x, int *y, int *z, int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        z[i] = x[i] + y[i];
    }
}
存放在addvec.c文件中。
使用gcc -c addvec.c命令，可以得到一个addvec.o文件。
这里的addvec.o文件就是可重定位目标文件。
这个文件里面到底有些什么。我们可以通过readelf命令来看下。
readelf是GNU Binutils这个工具包里的工具。如果输入readelf -v查看不了这个工具的版本，
可以使用命令sudo apt-get install binutils去安装。
readelf addvec.o -a
可以得到以下的输出：
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          344 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         11
  Section header string table index: 8
Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       000000000000005b  0000000000000000  AX       0     0     4
  [ 2] .data             PROGBITS         0000000000000000  0000009c
       0000000000000000  0000000000000000  WA       0     0     4
  [ 3] .bss              NOBITS           0000000000000000  0000009c
       0000000000000000  0000000000000000  WA       0     0     4
  [ 4] .comment          PROGBITS         0000000000000000  0000009c
       000000000000002b  0000000000000001  MS       0     0     1
  [ 5] .note.GNU-stack   PROGBITS         0000000000000000  000000c7
       0000000000000000  0000000000000000           0     0     1
  [ 6] .eh_frame         PROGBITS         0000000000000000  000000c8
       0000000000000038  0000000000000000   A       0     0     8
  [ 7] .rela.eh_frame    RELA             0000000000000000  00000508
       0000000000000018  0000000000000018           9     6     8
  [ 8] .shstrtab         STRTAB           0000000000000000  00000100
       0000000000000054  0000000000000000           0     0     1
  [ 9] .symtab           SYMTAB           0000000000000000  00000418
       00000000000000d8  0000000000000018          10     8     8
  [10] .strtab           STRTAB           0000000000000000  000004f0
       0000000000000011  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
There are no section groups in this file.
There are no program headers in this file.
Relocation section '.rela.eh_frame' at offset 0x508 contains 1 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000020  000200000002 R_X86_64_PC32     0000000000000000 .text + 0
There are no unwind sections in this file.
Symbol table '.symtab' contains 9 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS addvec.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    2 
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 
     5: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 
     6: 0000000000000000     0 SECTION LOCAL  DEFAULT    6 
     7: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 
     8: 0000000000000000    91 FUNC    GLOBAL DEFAULT    1 addvec
No version information found in this file.
重新建立一个main.c文件：
#include <stdio.h>
#include "vector.h"
int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];
int main()
{
    addvec(x, y, z, 2);
    printf("z = [%d %d]\n", z[0], z[1]);
    return 0;
}
建立一个vector.h:
void addvec(int *x, int *y, int *z, int n);
然后运行gcc -O2 -c main.c,得到可重定位目标main.o.
运行readelf main.o -a：
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          376 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         14
  Section header string table index: 11
Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       0000000000000000  0000000000000000  AX       0     0     4
  [ 2] .data             PROGBITS         0000000000000000  00000040
       0000000000000010  0000000000000000  WA       0     0     4
  [ 3] .bss              NOBITS           0000000000000000  00000050
       0000000000000000  0000000000000000  WA       0     0     4
  [ 4] .rodata.str1.1    PROGBITS         0000000000000000  00000050
       000000000000000d  0000000000000001 AMS       0     0     1
  [ 5] .text.startup     PROGBITS         0000000000000000  00000060
       0000000000000041  0000000000000000  AX       0     0     16
  [ 6] .rela.text.startu RELA             0000000000000000  000006a0
       00000000000000c0  0000000000000018          12     5     8
  [ 7] .comment          PROGBITS         0000000000000000  000000a1
       000000000000002b  0000000000000001  MS       0     0     1
  [ 8] .note.GNU-stack   PROGBITS         0000000000000000  000000cc
       0000000000000000  0000000000000000           0     0     1
  [ 9] .eh_frame         PROGBITS         0000000000000000  000000d0
       0000000000000030  0000000000000000   A       0     0     8
  [10] .rela.eh_frame    RELA             0000000000000000  00000760
       0000000000000018  0000000000000018          12     9     8
  [11] .shstrtab         STRTAB           0000000000000000  00000100
       0000000000000076  0000000000000000           0     0     1
  [12] .symtab           SYMTAB           0000000000000000  000004f8
       0000000000000180  0000000000000018          13    10     8
  [13] .strtab           STRTAB           0000000000000000  00000678
       0000000000000027  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
There are no section groups in this file.
There are no program headers in this file.
Relocation section '.rela.text.startup' at offset 0x6a0 contains 8 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
00000000000a  000b0000000a R_X86_64_32       0000000000000004 z + 0
00000000000f  000c0000000a R_X86_64_32       0000000000000000 y + 0
000000000014  000d0000000a R_X86_64_32       0000000000000008 x + 0
000000000019  000e00000002 R_X86_64_PC32     0000000000000000 addvec - 4
00000000001f  000b00000002 R_X86_64_PC32     0000000000000004 z + 0
000000000025  000b00000002 R_X86_64_PC32     0000000000000004 z - 4
00000000002a  00050000000a R_X86_64_32       0000000000000000 .rodata.str1.1 + 0
000000000036  000f00000002 R_X86_64_PC32     0000000000000000 __printf_chk - 4
Relocation section '.rela.eh_frame' at offset 0x760 contains 1 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000020  000600000002 R_X86_64_PC32     0000000000000000 .text.startup + 0
There are no unwind sections in this file.
Symbol table '.symtab' contains 16 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    2 
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 
     5: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 
     6: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 
     7: 0000000000000000     0 SECTION LOCAL  DEFAULT    8 
     8: 0000000000000000     0 SECTION LOCAL  DEFAULT    9 
     9: 0000000000000000     0 SECTION LOCAL  DEFAULT    7 
    10: 0000000000000000    65 FUNC    GLOBAL DEFAULT    5 main
    11: 0000000000000004     8 OBJECT  GLOBAL DEFAULT  COM z
    12: 0000000000000000     8 OBJECT  GLOBAL DEFAULT    2 y
    13: 0000000000000008     8 OBJECT  GLOBAL DEFAULT    2 x
    14: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND addvec
    15: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND __printf_chk
No version information found in this file.
然后运行命令：gcc -static -o vecadd addvec.o main.o
参数static是告诉编译器驱动程序，链接器应该构建一个完全链接的可执行目标文件，它可以
加载到存储器并运行。并且在加载时无需更进一步的链接了。命令执行完后，得到一个可执行
目标文件vecadd.
运行可执行目标文件vecadd,可以得到运行完的结果：z = [4 6]
我们依然使用readelf vecadd -a来看看这个可执行目标程序里面到底有什么。
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 03 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - GNU
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x4010a8
  Start of program headers:          64 (bytes into file)
  Start of section headers:          800888 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         6
  Size of section headers:           64 (bytes)
  Number of section headers:         33
  Section header string table index: 30
Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .note.ABI-tag     NOTE             0000000000400190  00000190
       0000000000000020  0000000000000000   A       0     0     4
  [ 2] .note.gnu.build-i NOTE             00000000004001b0  000001b0
       0000000000000024  0000000000000000   A       0     0     4
  [ 3] .rela.plt         RELA             00000000004001d8  000001d8
       0000000000000120  0000000000000018   A       0     5     8
  [ 4] .init             PROGBITS         00000000004002f8  000002f8
       0000000000000018  0000000000000000  AX       0     0     4
  [ 5] .plt              PROGBITS         0000000000400310  00000310
       00000000000000c0  0000000000000000  AX       0     0     16
  [ 6] .text             PROGBITS         00000000004003d0  000003d0
       0000000000094a08  0000000000000000  AX       0     0     16
  [ 7] __libc_thread_fre PROGBITS         0000000000494de0  00094de0
       00000000000000a8  0000000000000000  AX       0     0     16
  [ 8] __libc_freeres_fn PROGBITS         0000000000494e90  00094e90
       000000000000181c  0000000000000000  AX       0     0     16
  [ 9] .fini             PROGBITS         00000000004966ac  000966ac
       000000000000000e  0000000000000000  AX       0     0     4
  [10] .rodata           PROGBITS         00000000004966c0  000966c0
       000000000001d364  0000000000000000   A       0     0     32
  [11] __libc_thread_sub PROGBITS         00000000004b3a28  000b3a28
       0000000000000008  0000000000000000   A       0     0     8
  [12] __libc_subfreeres PROGBITS         00000000004b3a30  000b3a30
       0000000000000058  0000000000000000   A       0     0     8
  [13] __libc_atexit     PROGBITS         00000000004b3a88  000b3a88
       0000000000000008  0000000000000000   A       0     0     8
  [14] .eh_frame         PROGBITS         00000000004b3a90  000b3a90
       000000000000d684  0000000000000000   A       0     0     8
  [15] .gcc_except_table PROGBITS         00000000004c1114  000c1114
       0000000000000181  0000000000000000   A       0     0     1
  [16] .tdata            PROGBITS         00000000006c1ef0  000c1ef0
       0000000000000020  0000000000000000 WAT       0     0     16
  [17] .tbss             NOBITS           00000000006c1f10  000c1f10
       0000000000000038  0000000000000000 WAT       0     0     16
  [18] .init_array       INIT_ARRAY       00000000006c1f10  000c1f10
       0000000000000008  0000000000000000  WA       0     0     8
  [19] .fini_array       FINI_ARRAY       00000000006c1f18  000c1f18
       0000000000000008  0000000000000000  WA       0     0     8
  [20] .ctors            PROGBITS         00000000006c1f20  000c1f20
       0000000000000010  0000000000000000  WA       0     0     8
  [21] .dtors            PROGBITS         00000000006c1f30  000c1f30
       0000000000000010  0000000000000000  WA       0     0     8
  [22] .jcr              PROGBITS         00000000006c1f40  000c1f40
       0000000000000008  0000000000000000  WA       0     0     8
  [23] .data.rel.ro      PROGBITS         00000000006c1f50  000c1f50
       0000000000000080  0000000000000000  WA       0     0     16
  [24] .got              PROGBITS         00000000006c1fd0  000c1fd0
       0000000000000010  0000000000000008  WA       0     0     8
  [25] .got.plt          PROGBITS         00000000006c1fe8  000c1fe8
       0000000000000078  0000000000000008  WA       0     0     8
  [26] .data             PROGBITS         00000000006c2060  000c2060
       0000000000001690  0000000000000000  WA       0     0     32
  [27] .bss              NOBITS           00000000006c3700  000c36f0
       0000000000002bc8  0000000000000000  WA       0     0     32
  [28] __libc_freeres_pt NOBITS           00000000006c62d0  000c36f0
       0000000000000048  0000000000000000  WA       0     0     16
  [29] .comment          PROGBITS         0000000000000000  000c36f0
       000000000000002a  0000000000000001  MS       0     0     1
  [30] .shstrtab         STRTAB           0000000000000000  000c371a
       000000000000015b  0000000000000000           0     0     1
  [31] .symtab           SYMTAB           0000000000000000  000c40b8
       000000000000c1f8  0000000000000018          32   876     8
  [32] .strtab           STRTAB           0000000000000000  000d02b0
       0000000000007a3e  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
There are no section groups in this file.
Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x00000000000c1295 0x00000000000c1295  R E    200000
  LOAD           0x00000000000c1ef0 0x00000000006c1ef0 0x00000000006c1ef0
                 0x0000000000001800 0x0000000000004428  RW     200000
  NOTE           0x0000000000000190 0x0000000000400190 0x0000000000400190
                 0x0000000000000044 0x0000000000000044  R      4
  TLS            0x00000000000c1ef0 0x00000000006c1ef0 0x00000000006c1ef0
                 0x0000000000000020 0x0000000000000058  R      10
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     8
  GNU_RELRO      0x00000000000c1ef0 0x00000000006c1ef0 0x00000000006c1ef0
                 0x0000000000000110 0x0000000000000110  R      1
 Section to Segment mapping:
  Segment Sections...
   00     .note.ABI-tag .note.gnu.build-id .rela.plt .init .plt .text __libc_thread_freeres_fn __libc_freeres_fn .fini .rodata __libc_thread_subfreeres __libc_subfreeres __libc_atexit .eh_frame .gcc_except_table 
   01     .tdata .init_array .fini_array .ctors .dtors .jcr .data.rel.ro .got .got.plt .data .bss __libc_freeres_ptrs 
   02     .note.ABI-tag .note.gnu.build-id 
   03     .tdata .tbss 
   04     
   05     .tdata .init_array .fini_array .ctors .dtors .jcr .data.rel.ro .got 
There is no dynamic section in this file.
Relocation section '.rela.plt' at offset 0x1d8 contains 12 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
0000006c2058  000000000025 R_X86_64_IRELATIV                    000000000042df20
0000006c2050  000000000025 R_X86_64_IRELATIV                    0000000000482a80
0000006c2048  000000000025 R_X86_64_IRELATIV                    000000000042e090
0000006c2040  000000000025 R_X86_64_IRELATIV                    000000000040a1f0
0000006c2038  000000000025 R_X86_64_IRELATIV                    00000000004325d0
0000006c2030  000000000025 R_X86_64_IRELATIV                    0000000000409db0
0000006c2028  000000000025 R_X86_64_IRELATIV                    0000000000445d50
0000006c2020  000000000025 R_X86_64_IRELATIV                    0000000000437ff0
0000006c2018  000000000025 R_X86_64_IRELATIV                    0000000000432460
0000006c2010  000000000025 R_X86_64_IRELATIV                    00000000004305f0
0000006c2008  000000000025 R_X86_64_IRELATIV                    00000000004302c0
0000006c2000  000000000025 R_X86_64_IRELATIV                    00000000004324b0
There are no unwind sections in this file.
Symbol table '.symtab' contains 2069 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000400190     0 SECTION LOCAL  DEFAULT    1 
     2: 00000000004001b0     0 SECTION LOCAL  DEFAULT    2 
     3: 00000000004001d8     0 SECTION LOCAL  DEFAULT    3 
     4: 00000000004002f8     0 SECTION LOCAL  DEFAULT    4 
     5: 0000000000400310     0 SECTION LOCAL  DEFAULT    5 
     6: 00000000004003d0     0 SECTION LOCAL  DEFAULT    6 
     7: 0000000000494de0     0 SECTION LOCAL  DEFAULT    7 
     8: 0000000000494e90     0 SECTION LOCAL  DEFAULT    8 
     9: 00000000004966ac     0 SECTION LOCAL  DEFAULT    9 
    10: 00000000004966c0     0 SECTION LOCAL  DEFAULT   10 
    11: 00000000004b3a28     0 SECTION LOCAL  DEFAULT   11 
    12: 00000000004b3a30     0 SECTION LOCAL  DEFAULT   12 
    13: 00000000004b3a88     0 SECTION LOCAL  DEFAULT   13 
    14: 00000000004b3a90     0 SECTION LOCAL  DEFAULT   14 
    15: 00000000004c1114     0 SECTION LOCAL  DEFAULT   15 
    16: 00000000006c1ef0     0 SECTION LOCAL  DEFAULT   16 
    17: 00000000006c1f10     0 SECTION LOCAL  DEFAULT   17 
    18: 00000000006c1f10     0 SECTION LOCAL  DEFAULT   18 
    19: 00000000006c1f18     0 SECTION LOCAL  DEFAULT   19 
    20: 00000000006c1f20     0 SECTION LOCAL  DEFAULT   20 
    21: 00000000006c1f30     0 SECTION LOCAL  DEFAULT   21 
    22: 00000000006c1f40     0 SECTION LOCAL  DEFAULT   22 
    23: 00000000006c1f50     0 SECTION LOCAL  DEFAULT   23 
    24: 00000000006c1fd0     0 SECTION LOCAL  DEFAULT   24 
    25: 00000000006c1fe8     0 SECTION LOCAL  DEFAULT   25 
    26: 00000000006c2060     0 SECTION LOCAL  DEFAULT   26 
    27: 00000000006c3700     0 SECTION LOCAL  DEFAULT   27 
    28: 00000000006c62d0     0 SECTION LOCAL  DEFAULT   28 
    29: 0000000000000000     0 SECTION LOCAL  DEFAULT   29 
    30: 00000000004010d4     0 FUNC    LOCAL  DEFAULT    6 call_gmon_start
    31: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    32: 00000000006c1f20     0 OBJECT  LOCAL  DEFAULT   20 __CTOR_LIST__
    33: 00000000006c1f30     0 OBJECT  LOCAL  DEFAULT   21 __DTOR_LIST__
    34: 00000000004b3a90     0 OBJECT  LOCAL  DEFAULT   14 __EH_FRAME_BEGIN__
    35: 00000000006c1f40     0 OBJECT  LOCAL  DEFAULT   22 __JCR_LIST__
    36: 00000000004010f0     0 FUNC    LOCAL  DEFAULT    6 __do_global_dtors_aux
    37: 00000000006c3700     1 OBJECT  LOCAL  DEFAULT   27 completed.6036
    38: 00000000006c3708     8 OBJECT  LOCAL  DEFAULT   27 dtor_idx.6038
    39: 0000000000401170     0 FUNC    LOCAL  DEFAULT    6 frame_dummy
    40: 00000000006c3720    48 OBJECT  LOCAL  DEFAULT   27 object.6048
    41: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    42: 00000000006c1f28     0 OBJECT  LOCAL  DEFAULT   20 __CTOR_END__
    43: 00000000004c1110     0 OBJECT  LOCAL  DEFAULT   14 __FRAME_END__
    44: 00000000006c1f40     0 OBJECT  LOCAL  DEFAULT   22 __JCR_END__
    45: 0000000000494da0     0 FUNC    LOCAL  DEFAULT    6 __do_global_ctors_aux
    46: 00000000004003d0   113 FUNC    LOCAL  DEFAULT    6 check_one_fd.part.0
    47: 0000000000402540   258 FUNC    LOCAL  DEFAULT    6 ptmalloc_lock_all
    48: 00000000006c4950     4 OBJECT  LOCAL  DEFAULT   27 list_lock
    49: 0000000000000030     8 TLS     LOCAL  DEFAULT   17 __libc_tsd_MALLOC
    50: 000000000040940f    31 FUNC    LOCAL  DEFAULT    6 _L_lock_30
    51: 00000000006c20e0  2184 OBJECT  LOCAL  DEFAULT   26 main_arena
    52: 000000000040942e    27 FUNC    LOCAL  DEFAULT    6 _L_lock_48
    53: 0000000000407860   281 FUNC    LOCAL  DEFAULT    6 malloc_atfork
    54: 00000000006c4920     8 OBJECT  LOCAL  DEFAULT   27 save_malloc_hook
    55: 0000000000404ef0   138 FUNC    LOCAL  DEFAULT    6 free_atfork
    56: 00000000006c4930     8 OBJECT  LOCAL  DEFAULT   27 save_free_hook
    57: 00000000006c4910     8 OBJECT  LOCAL  DEFAULT   27 save_arena
    58: 00000000006c4960     4 OBJECT  LOCAL  DEFAULT   27 atfork_recursive_cntr
    59: 0000000000402650   145 FUNC    LOCAL  DEFAULT    6 ptmalloc_unlock_all2
    60: 00000000006c4940     8 OBJECT  LOCAL  DEFAULT   27 free_list
    61: 00000000004026f0   180 FUNC    LOCAL  DEFAULT    6 get_free_list
    62: 0000000000409449    31 FUNC    LOCAL  DEFAULT    6 _L_lock_136
    63: 0000000000409468    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_148
    64: 0000000000409487    27 FUNC    LOCAL  DEFAULT    6 _L_lock_156
    65: 0000000000494de0   112 FUNC    LOCAL  DEFAULT    7 arena_thread_freeres
    66: 0000000000494e50    28 FUNC    LOCAL  DEFAULT    7 _L_lock_193
    67: 0000000000494e6c    28 FUNC    LOCAL  DEFAULT    7 _L_unlock_199
    68: 00000000004027b0   656 FUNC    LOCAL  DEFAULT    6 mem2chunk_check
    69: 00000000006c2980    88 OBJECT  LOCAL  DEFAULT   26 mp_
    70: 0000000000402a40   106 FUNC    LOCAL  DEFAULT    6 __malloc_assert
    71: 0000000000402ab0   491 FUNC    LOCAL  DEFAULT    6 new_heap
    72: 00000000006c48c0     8 OBJECT  LOCAL  DEFAULT   27 aligned_heap_area
    73: 0000000000402ca0  1114 FUNC    LOCAL  DEFAULT    6 mi_arena.12891
    74: 00000000004094a2    27 FUNC    LOCAL  DEFAULT    6 _L_lock_854
    75: 00000000004094bd    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_1086
    76: 0000000000403100   312 FUNC    LOCAL  DEFAULT    6 mremap_chunk
    77: 0000000000496b7c    13 OBJECT  LOCAL  DEFAULT   10 __func__.12382
    78: 0000000000403240   178 FUNC    LOCAL  DEFAULT    6 ptmalloc_unlock_all
    79: 00000000004094d8    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_1441
    80: 00000000004094f3    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_1450
    81: 0000000000403300   215 FUNC    LOCAL  DEFAULT    6 sYSTRIm.isra.1
    82: 00000000004033e0   214 FUNC    LOCAL  DEFAULT    6 malloc_printerr
    83: 00000000004034c0   341 FUNC    LOCAL  DEFAULT    6 top_check
    84: 00000000006c29e0     4 OBJECT  LOCAL  DEFAULT   26 check_action
    85: 0000000000403620   802 FUNC    LOCAL  DEFAULT    6 malloc_consolidate.part.3
    86: 0000000000497dd0    19 OBJECT  LOCAL  DEFAULT   10 __func__.12714
    87: 0000000000400441    20 FUNC    LOCAL  DEFAULT    6 munmap_chunk.part.4
    88: 0000000000403950   754 FUNC    LOCAL  DEFAULT    6 arena_get2.isra.5.part.6
    89: 00000000006c4970     8 OBJECT  LOCAL  DEFAULT   27 narenas_limit.12118
    90: 00000000006c29f0     8 OBJECT  LOCAL  DEFAULT   26 narenas
    91: 00000000006c4980     8 OBJECT  LOCAL  DEFAULT   27 next_to_use.12103
    92: 0000000000409512    27 FUNC    LOCAL  DEFAULT    6 _L_lock_2449
    93: 00000000006c48b0     8 OBJECT  LOCAL  DEFAULT   27 arena_mem
    94: 000000000040952d    27 FUNC    LOCAL  DEFAULT    6 _L_lock_2627
    95: 0000000000409548    31 FUNC    LOCAL  DEFAULT    6 _L_lock_2632
    96: 0000000000409567    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_2639
    97: 00000000006c4890     8 OBJECT  LOCAL  DEFAULT   27 global_max_fast
    98: 0000000000403c50   505 FUNC    LOCAL  DEFAULT    6 mALLOPt
    99: 0000000000409586    31 FUNC    LOCAL  DEFAULT    6 _L_lock_2727
   100: 00000000004095a5    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_2757
   101: 00000000006c48a0     4 OBJECT  LOCAL  DEFAULT   27 perturb_byte
   102: 0000000000403e50   993 FUNC    LOCAL  DEFAULT    6 ptmalloc_init.part.8
   103: 00000000006c48e0    48 OBJECT  LOCAL  DEFAULT   27 atfork_mem
   104: 00000000006c4870     4 OBJECT  LOCAL  DEFAULT   27 disallow_malloc_check
   105: 00000000006c4880     4 OBJECT  LOCAL  DEFAULT   27 using_malloc_checking
   106: 00000000004063e0   258 FUNC    LOCAL  DEFAULT    6 malloc_check
   107: 0000000000404f80   346 FUNC    LOCAL  DEFAULT    6 free_check
   108: 0000000000406cc0   889 FUNC    LOCAL  DEFAULT    6 realloc_check
   109: 0000000000406780   306 FUNC    LOCAL  DEFAULT    6 memalign_check
   110: 0000000000404240   375 FUNC    LOCAL  DEFAULT    6 mALLINFo
   111: 00000000004043c0  2862 FUNC    LOCAL  DEFAULT    6 _int_free
   112: 00000000004095c4    28 FUNC    LOCAL  DEFAULT    6 _L_lock_4222
   113: 00000000004095e0    28 FUNC    LOCAL  DEFAULT    6 _L_unlock_4349
   114: 0000000000496b72    10 OBJECT  LOCAL  DEFAULT   10 __func__.12066
   115: 00000000004095fc    28 FUNC    LOCAL  DEFAULT    6 _L_lock_4776
   116: 0000000000409618    28 FUNC    LOCAL  DEFAULT    6 _L_unlock_4789
   117: 0000000000409634    28 FUNC    LOCAL  DEFAULT    6 _L_unlock_4827
   118: 0000000000496b68    10 OBJECT  LOCAL  DEFAULT   10 __func__.12667
   119: 0000000000409650    31 FUNC    LOCAL  DEFAULT    6 _L_lock_5477
   120: 000000000040966f    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_5498
   121: 000000000040968e    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_5520
   122: 00000000004096ad    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_5579
   123: 0000000000496b5b    13 OBJECT  LOCAL  DEFAULT   10 __func__.12371
   124: 00000000004050e0  4860 FUNC    LOCAL  DEFAULT    6 _int_malloc
   125: 0000000000496b45    12 OBJECT  LOCAL  DEFAULT   10 __func__.12628
   126: 0000000000496b51    10 OBJECT  LOCAL  DEFAULT   10 __func__.12340
   127: 00000000004096cc    31 FUNC    LOCAL  DEFAULT    6 _L_lock_8454
   128: 00000000004096eb    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_8470
   129: 00000000004064f0   650 FUNC    LOCAL  DEFAULT    6 _int_memalign
   130: 0000000000496b37    14 OBJECT  LOCAL  DEFAULT   10 __func__.12760
   131: 000000000040970a    31 FUNC    LOCAL  DEFAULT    6 _L_lock_8909
   132: 0000000000409729    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_8927
   133: 00000000004068c0  1011 FUNC    LOCAL  DEFAULT    6 _int_realloc
   134: 0000000000496b89    13 OBJECT  LOCAL  DEFAULT   10 __func__.12739
   135: 0000000000409748    31 FUNC    LOCAL  DEFAULT    6 _L_lock_9620
   136: 0000000000409767    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_9630
   137: 0000000000409786    31 FUNC    LOCAL  DEFAULT    6 _L_lock_9659
   138: 00000000004097a5    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_9701
   139: 00000000004097c4    31 FUNC    LOCAL  DEFAULT    6 _L_lock_10069
   140: 00000000004097e3    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_10366
   141: 0000000000497df0    19 OBJECT  LOCAL  DEFAULT   10 __func__.12310
   142: 0000000000409802    27 FUNC    LOCAL  DEFAULT    6 _L_lock_10509
   143: 000000000040981d    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_10531
   144: 0000000000409838    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_10589
   145: 0000000000409853    31 FUNC    LOCAL  DEFAULT    6 _L_lock_10596
   146: 0000000000409872    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_10605
   147: 0000000000409891    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_10687
   148: 00000000004098b0    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_10699
   149: 0000000000496b96    14 OBJECT  LOCAL  DEFAULT   10 __func__.12407
   150: 00000000004098cb    31 FUNC    LOCAL  DEFAULT    6 _L_lock_10777
   151: 00000000004098ea    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_10780
   152: 0000000000407980    51 FUNC    LOCAL  DEFAULT    6 malloc_hook_ini
   153: 0000000000409909    31 FUNC    LOCAL  DEFAULT    6 _L_lock_10965
   154: 0000000000409928    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_11086
   155: 0000000000409947    27 FUNC    LOCAL  DEFAULT    6 _L_lock_11351
   156: 0000000000409962    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_11362
   157: 0000000000496ba4    15 OBJECT  LOCAL  DEFAULT   10 __func__.12439
   158: 0000000000408050    70 FUNC    LOCAL  DEFAULT    6 realloc_hook_ini
   159: 000000000040997d    28 FUNC    LOCAL  DEFAULT    6 _L_lock_11793
   160: 0000000000409999    28 FUNC    LOCAL  DEFAULT    6 _L_unlock_11811
   161: 00000000004099b5    31 FUNC    LOCAL  DEFAULT    6 _L_lock_11888
   162: 00000000004099d4    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_11898
   163: 0000000000497e10    16 OBJECT  LOCAL  DEFAULT   10 __func__.12467
   164: 00000000004099f3    28 FUNC    LOCAL  DEFAULT    6 _L_unlock_12002
   165: 0000000000408310    57 FUNC    LOCAL  DEFAULT    6 memalign_hook_ini
   166: 0000000000409a0f    27 FUNC    LOCAL  DEFAULT    6 _L_lock_12142
   167: 0000000000409a2a    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_12172
   168: 0000000000409a45    31 FUNC    LOCAL  DEFAULT    6 _L_lock_12287
   169: 0000000000409a64    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_12296
   170: 0000000000409a83    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_12406
   171: 0000000000496bb3    14 OBJECT  LOCAL  DEFAULT   10 __func__.12492
   172: 0000000000409a9e    27 FUNC    LOCAL  DEFAULT    6 _L_lock_12550
   173: 0000000000409ab9    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_12586
   174: 0000000000409ad4    31 FUNC    LOCAL  DEFAULT    6 _L_lock_12706
   175: 0000000000409af3    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_12715
   176: 0000000000409b12    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_12823
   177: 0000000000496bc1    15 OBJECT  LOCAL  DEFAULT   10 __func__.12519
   178: 0000000000409b2d    28 FUNC    LOCAL  DEFAULT    6 _L_lock_12976
   179: 0000000000409b49    28 FUNC    LOCAL  DEFAULT    6 _L_unlock_13018
   180: 0000000000409b65    31 FUNC    LOCAL  DEFAULT    6 _L_lock_13110
   181: 0000000000409b84    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_13119
   182: 0000000000409ba3    28 FUNC    LOCAL  DEFAULT    6 _L_lock_13259
   183: 0000000000409bbf    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_13277
   184: 0000000000409bde    28 FUNC    LOCAL  DEFAULT    6 _L_unlock_13289
   185: 0000000000496bd0    14 OBJECT  LOCAL  DEFAULT   10 __func__.12547
   186: 0000000000409bfa    27 FUNC    LOCAL  DEFAULT    6 _L_lock_13455
   187: 0000000000409c15    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_13634
   188: 0000000000496bde     6 OBJECT  LOCAL  DEFAULT   10 __func__.12783
   189: 0000000000409c30    27 FUNC    LOCAL  DEFAULT    6 _L_lock_14013
   190: 0000000000409c4b    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_14049
   191: 0000000000409c66    31 FUNC    LOCAL  DEFAULT    6 _L_lock_14139
   192: 0000000000409c85    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_14145
   193: 00000000004b3a28     8 OBJECT  LOCAL  DEFAULT   11 __elf_set___libc_thread_s
   194: 000000000041a710   211 FUNC    LOCAL  DEFAULT    6 _IO_helper_overflow
   195: 000000000041a7f0   514 FUNC    LOCAL  DEFAULT    6 buffered_vfprintf
   196: 000000000049b920   168 OBJECT  LOCAL  DEFAULT   10 _IO_helper_jumps
   197: 000000000041ff9f    27 FUNC    LOCAL  DEFAULT    6 _L_lock_189
   198: 000000000041ffba    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_238
   199: 0000000000400455   258 FUNC    LOCAL  DEFAULT    6 group_number
   200: 0000000000400557   485 FUNC    LOCAL  DEFAULT    6 _i18n_number_rewrite
   201: 000000000041ffd5    27 FUNC    LOCAL  DEFAULT    6 _L_lock_1109
   202: 000000000041fff0    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_1164
   203: 000000000049b1c0    91 OBJECT  LOCAL  DEFAULT   10 jump_table.13210
   204: 000000000049b220   240 OBJECT  LOCAL  DEFAULT   10 step0_jumps.13228
   205: 000000000049b9d0    22 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.13225
   206: 000000000049b820   240 OBJECT  LOCAL  DEFAULT   10 step4_jumps.13414
   207: 000000000049b320   240 OBJECT  LOCAL  DEFAULT   10 step1_jumps.13259
   208: 000000000049b520   240 OBJECT  LOCAL  DEFAULT   10 step3a_jumps.13261
   209: 000000000049b420   240 OBJECT  LOCAL  DEFAULT   10 step2_jumps.13260
   210: 000000000049b620   240 OBJECT  LOCAL  DEFAULT   10 step4_jumps.13264
   211: 000000000049b720   240 OBJECT  LOCAL  DEFAULT   10 step3b_jumps.13263
   212: 000000000049b0eb     7 OBJECT  LOCAL  DEFAULT   10 null
   213: 0000000000420010   340 FUNC    LOCAL  DEFAULT    6 hack_digit.14698
   214: 000000000040073c   485 FUNC    LOCAL  DEFAULT    6 _i18n_number_rewrite
   215: 000000000044e9e0   192 FUNC    LOCAL  DEFAULT    6 is_dst
   216: 000000000044eaa0   960 FUNC    LOCAL  DEFAULT    6 fillin_rpath
   217: 000000000049d43a     3 OBJECT  LOCAL  DEFAULT   10 curwd.11054
   218: 00000000006c1f90     8 OBJECT  LOCAL  DEFAULT   23 ncapstr
   219: 00000000006c5840     8 OBJECT  LOCAL  DEFAULT   27 max_dirnamelen
   220: 000000000049f960    66 OBJECT  LOCAL  DEFAULT   10 system_dirs
   221: 0000000000400921   249 FUNC    LOCAL  DEFAULT    6 is_trusted_path_normalize
   222: 000000000049f940    32 OBJECT  LOCAL  DEFAULT   10 system_dirs_len
   223: 0000000000400a1a   335 FUNC    LOCAL  DEFAULT    6 print_search_path
   224: 00000000006c1f98     8 OBJECT  LOCAL  DEFAULT   23 max_capstrlen
   225: 00000000006c1fa0     8 OBJECT  LOCAL  DEFAULT   23 capstr
   226: 000000000044ee60   117 FUNC    LOCAL  DEFAULT    6 lose
   227: 000000000044eee0   211 FUNC    LOCAL  DEFAULT    6 add_name_to_object.isra.3
   228: 000000000049f9f0    19 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11035
   229: 000000000044efc0  3686 FUNC    LOCAL  DEFAULT    6 _dl_map_object_from_fd
   230: 000000000049fa50    21 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10918
   231: 000000000049fa30    23 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11169
   232: 000000000044fe30  1177 FUNC    LOCAL  DEFAULT    6 open_verify.isra.4
   233: 000000000049f9b0    16 OBJECT  LOCAL  DEFAULT   10 expected.11238
   234: 000000000049f9d0    16 OBJECT  LOCAL  DEFAULT   10 expected_note.11244
   235: 000000000049f9c0     9 OBJECT  LOCAL  DEFAULT   10 expected2.11237
   236: 00000000004502d0  1037 FUNC    LOCAL  DEFAULT    6 open_path.isra.5
   237: 00000000006c1fb0    16 OBJECT  LOCAL  DEFAULT   23 rtld_search_dirs
   238: 00000000004506e0   203 FUNC    LOCAL  DEFAULT    6 add_path.11355.isra.2.con
   239: 0000000000450b50   505 FUNC    LOCAL  DEFAULT    6 expand_dynamic_string_tok
   240: 000000000049fa10    28 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11013
   241: 0000000000450d50   367 FUNC    LOCAL  DEFAULT    6 decompose_rpath
   242: 00000000006c1fc0    16 OBJECT  LOCAL  DEFAULT   23 env_path_list
   243: 000000000049f90d    15 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11117
   244: 000000000049f91c    15 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11317
   245: 000000000049f9e0     4 OBJECT  LOCAL  DEFAULT   10 dummy_bucket.11344
   246: 0000000000400b69   112 FUNC    LOCAL  DEFAULT    6 strip
   247: 00000000004a17e0    52 OBJECT  LOCAL  DEFAULT   10 codeset_idx.10648
   248: 00000000004a1750    16 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10653
   249: 000000000046e8d0   257 FUNC    LOCAL  DEFAULT    6 _IO_helper_overflow
   250: 0000000000400bd9   202 FUNC    LOCAL  DEFAULT    6 group_number
   251: 0000000000400ca3   291 FUNC    LOCAL  DEFAULT    6 _i18n_number_rewrite
   252: 0000000000473ff2    27 FUNC    LOCAL  DEFAULT    6 _L_lock_647
   253: 000000000047400d    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_727
   254: 0000000000473de0   530 FUNC    LOCAL  DEFAULT    6 buffered_vfprintf
   255: 00000000004affa0    91 OBJECT  LOCAL  DEFAULT   10 jump_table.13236
   256: 00000000004b0000   240 OBJECT  LOCAL  DEFAULT   10 step0_jumps.13254
   257: 00000000004aff70    14 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.13251
   258: 00000000004b0600   240 OBJECT  LOCAL  DEFAULT   10 step4_jumps.13431
   259: 00000000004b0100   240 OBJECT  LOCAL  DEFAULT   10 step1_jumps.13285
   260: 00000000004b0200   240 OBJECT  LOCAL  DEFAULT   10 step2_jumps.13286
   261: 00000000004b0300   240 OBJECT  LOCAL  DEFAULT   10 step3a_jumps.13287
   262: 00000000004b0400   240 OBJECT  LOCAL  DEFAULT   10 step4_jumps.13290
   263: 00000000004b07b0    28 OBJECT  LOCAL  DEFAULT   10 null
   264: 00000000004b0500   240 OBJECT  LOCAL  DEFAULT   10 step3b_jumps.13289
   265: 00000000004b0700   168 OBJECT  LOCAL  DEFAULT   10 _IO_helper_jumps
   266: 0000000000474028    27 FUNC    LOCAL  DEFAULT    6 _L_lock_13380
   267: 0000000000474043    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_13432
   268: 00000000004822e0    51 FUNC    LOCAL  DEFAULT    6 init
   269: 00000000006c5b20    32 OBJECT  LOCAL  DEFAULT   27 last_result
   270: 00000000006c5b40     8 OBJECT  LOCAL  DEFAULT   27 static_buf
   271: 0000000000482530   133 FUNC    LOCAL  DEFAULT    6 free_key_mem
   272: 00000000006c5b04     4 OBJECT  LOCAL  DEFAULT   27 key
   273: 00000000006c5b00     4 OBJECT  LOCAL  DEFAULT   27 once
   274: 00000000004824c0   101 FUNC    LOCAL  DEFAULT    6 check_free
   275: 0000000000400dd0    10 FUNC    LOCAL  DEFAULT    6 fini
   276: 00000000006c3680   104 OBJECT  LOCAL  DEFAULT   26 _dlfcn_hooks
   277: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.c
   278: 000000000040e8c0    16 FUNC    LOCAL  DEFAULT    6 intel_02_known_compare
   279: 000000000040e8d0   383 FUNC    LOCAL  DEFAULT    6 handle_amd
   280: 0000000000498fd9    11 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.4179
   281: 000000000040ea50   720 FUNC    LOCAL  DEFAULT    6 intel_check_word
   282: 0000000000499160   544 OBJECT  LOCAL  DEFAULT   10 intel_02_known
   283: 0000000000499380    17 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.4110
   284: 000000000040ed20   255 FUNC    LOCAL  DEFAULT    6 handle_intel
   285: 0000000000400e30   632 FUNC    LOCAL  DEFAULT    6 init_cacheinfo
   286: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS addvec.c
   287: 00000000006c3760  1024 OBJECT  LOCAL  DEFAULT   27 static_dtv
   288: 00000000006c3b60  1128 OBJECT  LOCAL  DEFAULT   27 static_map
   289: 00000000006c3fe0  1040 OBJECT  LOCAL  DEFAULT   27 static_slotinfo
   290: 00000000006c4400     4 OBJECT  LOCAL  DEFAULT   27 lock
   291: 0000000000402055    31 FUNC    LOCAL  DEFAULT    6 _L_lock_50
   292: 0000000000402074    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_164
   293: 00000000004967b5    13 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.9516
   294: 0000000000402093    31 FUNC    LOCAL  DEFAULT    6 _L_lock_324
   295: 00000000004020b2    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_426
   296: 00000000006c4420  1040 OBJECT  LOCAL  DEFAULT   27 initial
   297: 0000000000409df0  1024 FUNC    LOCAL  DEFAULT    6 __memcmp_sse2
   298: 00000000006c4990     4 OBJECT  LOCAL  DEFAULT   27 miss_F_GETOWN_EX
   299: 000000000040f370    37 FUNC    LOCAL  DEFAULT    6 cancel_handler
   300: 00000000006c49b4     4 OBJECT  LOCAL  DEFAULT   27 syslog_lock
   301: 0000000000410002    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_7
   302: 000000000040f3a0  1081 FUNC    LOCAL  DEFAULT    6 openlog_internal
   303: 00000000006c49a0     8 OBJECT  LOCAL  DEFAULT   27 LogTag
   304: 00000000006c49b0     4 OBJECT  LOCAL  DEFAULT   27 LogStat
   305: 00000000006c2ab0     4 OBJECT  LOCAL  DEFAULT   26 LogFile
   306: 00000000006c49c0     4 OBJECT  LOCAL  DEFAULT   27 connected
   307: 00000000006c49e0   110 OBJECT  LOCAL  DEFAULT   27 SyslogAddr
   308: 00000000006c2a90     4 OBJECT  LOCAL  DEFAULT   26 LogFacility
   309: 00000000006c2aa0     4 OBJECT  LOCAL  DEFAULT   26 LogType
   310: 00000000006c2a80     4 OBJECT  LOCAL  DEFAULT   26 LogMask
   311: 0000000000410021    31 FUNC    LOCAL  DEFAULT    6 _L_lock_625
   312: 0000000000410040    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_650
   313: 000000000041005f    31 FUNC    LOCAL  DEFAULT    6 _L_lock_1183
   314: 000000000041007e    31 FUNC    LOCAL  DEFAULT    6 _L_lock_1244
   315: 0000000000410120   216 FUNC    LOCAL  DEFAULT    6 phys_pages_info
   316: 0000000000410200   470 FUNC    LOCAL  DEFAULT    6 next_line
   317: 000000000049945d    10 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11532
   318: 00000000006c4a50     8 OBJECT  LOCAL  DEFAULT   27 timestamp.11536
   319: 00000000006c4a60     4 OBJECT  LOCAL  DEFAULT   27 cached_result.11535
   320: 0000000000494e90   133 FUNC    LOCAL  DEFAULT    8 free_mem
   321: 0000000000494f15    28 FUNC    LOCAL  DEFAULT    8 _L_lock_9
   322: 00000000006c4aa0  2312 OBJECT  LOCAL  DEFAULT   27 fork_handler_pool
   323: 0000000000494f31    28 FUNC    LOCAL  DEFAULT    8 _L_unlock_20
   324: 0000000000410b78    31 FUNC    LOCAL  DEFAULT    6 _L_lock_80
   325: 0000000000410b97    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_167
   326: 00000000004b3a30     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   327: 0000000000410d5d    27 FUNC    LOCAL  DEFAULT    6 _L_lock_55
   328: 0000000000410d78    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_123
   329: 0000000000410d93    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_170
   330: 0000000000410ec3    27 FUNC    LOCAL  DEFAULT    6 _L_lock_41
   331: 0000000000410ede    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_104
   332: 0000000000410ef9    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_153
   333: 0000000000410f20   158 FUNC    LOCAL  DEFAULT    6 backtrace_helper
   334: 0000000000499600   261 OBJECT  LOCAL  DEFAULT   10 unsecure_envvars.10813
   335: 00000000006c53e0     1 OBJECT  LOCAL  DEFAULT   27 buf.10830
   336: 00000000006c53d0    16 OBJECT  LOCAL  DEFAULT   27 result.10829
   337: 000000000049992a     2 OBJECT  LOCAL  DEFAULT   10 not_available
   338: 000000000049a1a0   256 OBJECT  LOCAL  DEFAULT   10 collseqmb
   339: 000000000049a2a0  1052 OBJECT  LOCAL  DEFAULT   10 collseqwc
   340: 000000000049a6f0    19 OBJECT  LOCAL  DEFAULT   10 errstr.12278
   341: 00000000004122d0   477 FUNC    LOCAL  DEFAULT    6 plural_eval
   342: 0000000000494f50   169 FUNC    LOCAL  DEFAULT    8 free_mem
   343: 00000000006c5478     8 OBJECT  LOCAL  DEFAULT   27 root
   344: 00000000006c54b0     8 OBJECT  LOCAL  DEFAULT   27 transmem_list
   345: 00000000004124b0   118 FUNC    LOCAL  DEFAULT    6 transcmp
   346: 00000000006c54c0     4 OBJECT  LOCAL  DEFAULT   27 output_charset_cached.112
   347: 00000000006c54d0     8 OBJECT  LOCAL  DEFAULT   27 output_charset_cache.1128
   348: 00000000006c5480     4 OBJECT  LOCAL  DEFAULT   27 lock.11232
   349: 00000000004138c4    31 FUNC    LOCAL  DEFAULT    6 _L_lock_1258
   350: 00000000006c5490     8 OBJECT  LOCAL  DEFAULT   27 freemem.11243
   351: 00000000006c54a0     8 OBJECT  LOCAL  DEFAULT   27 freemem_size.11244
   352: 00000000004138e3    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_1405
   353: 0000000000413902    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_1480
   354: 0000000000413921    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_1492
   355: 0000000000413940    31 FUNC    LOCAL  DEFAULT    6 _L_lock_2074
   356: 000000000041395f    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_2081
   357: 00000000006c5440    56 OBJECT  LOCAL  DEFAULT   27 tree_lock.10916
   358: 00000000004b3a38     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   359: 00000000006c54e0    56 OBJECT  LOCAL  DEFAULT   27 lock.11429
   360: 00000000006c5520     8 OBJECT  LOCAL  DEFAULT   27 _nl_loaded_domains
   361: 00000000006c5530    16 OBJECT  LOCAL  DEFAULT   27 lock.10964
   362: 000000000041531c    31 FUNC    LOCAL  DEFAULT    6 _L_lock_37
   363: 000000000041533b    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_75
   364: 0000000000415360  1188 FUNC    LOCAL  DEFAULT    6 read_alias_file
   365: 00000000006c5550     8 OBJECT  LOCAL  DEFAULT   27 nmap
   366: 00000000006c5560     8 OBJECT  LOCAL  DEFAULT   27 maxmap
   367: 00000000006c62d0     8 OBJECT  LOCAL  DEFAULT   28 map
   368: 00000000006c5570     8 OBJECT  LOCAL  DEFAULT   27 string_space_act
   369: 00000000006c5580     8 OBJECT  LOCAL  DEFAULT   27 string_space_max
   370: 00000000006c62e0     8 OBJECT  LOCAL  DEFAULT   28 string_space
   371: 0000000000415810    16 FUNC    LOCAL  DEFAULT    6 alias_compare
   372: 00000000006c5540     4 OBJECT  LOCAL  DEFAULT   27 lock
   373: 0000000000415955    31 FUNC    LOCAL  DEFAULT    6 _L_lock_532
   374: 00000000006c2ba0     8 OBJECT  LOCAL  DEFAULT   26 locale_alias_path.11381
   375: 0000000000415974    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_613
   376: 00000000004159a0  1236 FUNC    LOCAL  DEFAULT    6 _nl_make_l10nflist.constp
   377: 0000000000416ea0   253 FUNC    LOCAL  DEFAULT    6 new_exp
   378: 000000000049ac60    27 OBJECT  LOCAL  DEFAULT   10 yypact
   379: 000000000049ac80   263 OBJECT  LOCAL  DEFAULT   10 yytranslate
   380: 000000000049ada0    55 OBJECT  LOCAL  DEFAULT   10 yycheck
   381: 000000000049ae20    27 OBJECT  LOCAL  DEFAULT   10 yydefact
   382: 000000000049ae3b    14 OBJECT  LOCAL  DEFAULT   10 yyr2
   383: 000000000049ae49    14 OBJECT  LOCAL  DEFAULT   10 yyr1
   384: 000000000049ae57     3 OBJECT  LOCAL  DEFAULT   10 yypgoto
   385: 000000000049ae5a     3 OBJECT  LOCAL  DEFAULT   10 yydefgoto
   386: 000000000049ade0    55 OBJECT  LOCAL  DEFAULT   10 yytable
   387: 000000000049aea0    32 OBJECT  LOCAL  DEFAULT   10 plvar
   388: 000000000049aec0    32 OBJECT  LOCAL  DEFAULT   10 plone
   389: 00000000006c55a0    16 OBJECT  LOCAL  DEFAULT   27 lock
   390: 0000000000417bf7    31 FUNC    LOCAL  DEFAULT    6 _L_lock_18
   391: 00000000006c55b0     4 OBJECT  LOCAL  DEFAULT   27 stage
   392: 0000000000417c16    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_141
   393: 0000000000417c35    31 FUNC    LOCAL  DEFAULT    6 _L_lock_160
   394: 0000000000417cf0   762 FUNC    LOCAL  DEFAULT    6 msort_with_tmp.part.0
   395: 0000000000417ff0   795 FUNC    LOCAL  DEFAULT    6 qsort_r.constprop.1
   396: 00000000006c55c0     4 OBJECT  LOCAL  DEFAULT   27 pagesize.9193
   397: 00000000006c55d0     8 OBJECT  LOCAL  DEFAULT   27 phys_pages.9192
   398: 0000000000418730  1093 FUNC    LOCAL  DEFAULT    6 __add_to_environ.constpro
   399: 00000000006c55e0     4 OBJECT  LOCAL  DEFAULT   27 envlock
   400: 0000000000419217    31 FUNC    LOCAL  DEFAULT    6 _L_lock_44
   401: 00000000006c55f0     8 OBJECT  LOCAL  DEFAULT   27 last_environ
   402: 00000000006c55f8     8 OBJECT  LOCAL  DEFAULT   27 known_values
   403: 0000000000419236    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_238
   404: 0000000000419255    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_294
   405: 0000000000419274    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_340
   406: 0000000000419293    31 FUNC    LOCAL  DEFAULT    6 _L_lock_559
   407: 00000000004192b2    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_668
   408: 00000000004192d1    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_820
   409: 00000000004192f0    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_869
   410: 000000000041930f    31 FUNC    LOCAL  DEFAULT    6 _L_lock_1156
   411: 000000000041932e    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_1194
   412: 000000000041934d    31 FUNC    LOCAL  DEFAULT    6 _L_lock_1263
   413: 000000000041936c    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_1272
   414: 0000000000495130    42 FUNC    LOCAL  DEFAULT    8 free_mem
   415: 00000000004b3a40     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   416: 00000000006c5600     4 OBJECT  LOCAL  DEFAULT   27 lock
   417: 0000000000422b55    31 FUNC    LOCAL  DEFAULT    6 _L_lock_23
   418: 0000000000422b74    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_44
   419: 000000000049ba54    15 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.14127
   420: 0000000000495160    97 FUNC    LOCAL  DEFAULT    8 free_mem
   421: 00000000006c5610     4 OBJECT  LOCAL  DEFAULT   27 next_bit
   422: 00000000006c5614     4 OBJECT  LOCAL  DEFAULT   27 lock
   423: 0000000000424831    31 FUNC    LOCAL  DEFAULT    6 _L_lock_135
   424: 0000000000424850    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_182
   425: 00000000004b3a48     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   426: 00000000006c5618     4 OBJECT  LOCAL  DEFAULT   27 lock
   427: 0000000000424935    31 FUNC    LOCAL  DEFAULT    6 _L_lock_14
   428: 00000000006c2bb0     4 OBJECT  LOCAL  DEFAULT   26 pa_next_type
   429: 0000000000424954    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_38
   430: 0000000000424bf2    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_16
   431: 000000000049bf5d    11 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12827
   432: 00000000004256df    27 FUNC    LOCAL  DEFAULT    6 _L_lock_37
   433: 00000000004256fa    31 FUNC    LOCAL  DEFAULT    6 _L_lock_101
   434: 0000000000425719    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_110
   435: 0000000000425738    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_195
   436: 0000000000425753    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_224
   437: 0000000000425868    27 FUNC    LOCAL  DEFAULT    6 _L_lock_32
   438: 0000000000425883    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_80
   439: 000000000042589e    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_138
   440: 0000000000425c2e    27 FUNC    LOCAL  DEFAULT    6 _L_lock_39
   441: 0000000000425c49    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_101
   442: 0000000000425c64    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_163
   443: 0000000000425dc0    27 FUNC    LOCAL  DEFAULT    6 _L_lock_31
   444: 0000000000425ddb    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_157
   445: 0000000000425df3    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_186
   446: 0000000000425f9d    27 FUNC    LOCAL  DEFAULT    6 _L_lock_50
   447: 0000000000425fb8    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_204
   448: 0000000000425fd0    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_233
   449: 000000000049bf70    16 OBJECT  LOCAL  DEFAULT   10 blanks
   450: 000000000049bf80    16 OBJECT  LOCAL  DEFAULT   10 zeroes
   451: 00000000004262d7    27 FUNC    LOCAL  DEFAULT    6 _L_lock_196
   452: 00000000004262f2    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_245
   453: 000000000042630a    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_291
   454: 00000000004263c0    70 FUNC    LOCAL  DEFAULT    6 _IO_mem_sync
   455: 0000000000426410   109 FUNC    LOCAL  DEFAULT    6 _IO_mem_finish
   456: 000000000049bfa0   168 OBJECT  LOCAL  DEFAULT   10 _IO_mem_jumps
   457: 000000000049c060    64 OBJECT  LOCAL  DEFAULT   10 blanks
   458: 000000000049c0a0    64 OBJECT  LOCAL  DEFAULT   10 zeroes
   459: 0000000000426a60   553 FUNC    LOCAL  DEFAULT    6 save_for_wbackup.isra.0
   460: 000000000042901b    27 FUNC    LOCAL  DEFAULT    6 _L_lock_643
   461: 0000000000429036    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_1027
   462: 000000000049c350    20 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11594
   463: 0000000000429051    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_1255
   464: 0000000000428760    40 FUNC    LOCAL  DEFAULT    6 _IO_wfile_underflow_maybe
   465: 0000000000428790   312 FUNC    LOCAL  DEFAULT    6 _IO_wfile_underflow_mmap
   466: 0000000000429070    41 FUNC    LOCAL  DEFAULT    6 _IO_file_seekoff_maybe_mm
   467: 00000000004290a0    90 FUNC    LOCAL  DEFAULT    6 _IO_file_sync_mmap
   468: 0000000000429110   368 FUNC    LOCAL  DEFAULT    6 decide_maybe_mmap
   469: 0000000000429280    73 FUNC    LOCAL  DEFAULT    6 _IO_file_xsgetn_maybe_mma
   470: 00000000004292f0   446 FUNC    LOCAL  DEFAULT    6 mmap_remap_check
   471: 00000000004294b0   276 FUNC    LOCAL  DEFAULT    6 _IO_file_xsgetn_mmap
   472: 00000000004297b0   305 FUNC    LOCAL  DEFAULT    6 new_do_write
   473: 000000000049c5d0    19 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12660
   474: 000000000042b432    27 FUNC    LOCAL  DEFAULT    6 _L_lock_4530
   475: 000000000042b44d    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_4570
   476: 000000000042b468    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_4744
   477: 000000000042b490   143 FUNC    LOCAL  DEFAULT    6 flush_cleanup
   478: 00000000006c5630     8 OBJECT  LOCAL  DEFAULT   27 run_fp
   479: 000000000042d480    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_24
   480: 00000000006c5620    16 OBJECT  LOCAL  DEFAULT   27 list_all_lock
   481: 000000000042d49b    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_38
   482: 00000000004951d0    96 FUNC    LOCAL  DEFAULT    8 buffer_free
   483: 00000000006c5650     8 OBJECT  LOCAL  DEFAULT   27 freeres_list
   484: 00000000006c5644     1 OBJECT  LOCAL  DEFAULT   27 dealloc_buffers
   485: 000000000042b520   463 FUNC    LOCAL  DEFAULT    6 save_for_backup
   486: 000000000042d4ba    31 FUNC    LOCAL  DEFAULT    6 _L_lock_402
   487: 000000000042d4d9    27 FUNC    LOCAL  DEFAULT    6 _L_lock_437
   488: 000000000042d4f4    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_508
   489: 00000000006c5640     4 OBJECT  LOCAL  DEFAULT   27 _IO_list_all_stamp
   490: 000000000042d513    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_561
   491: 000000000042d52e    31 FUNC    LOCAL  DEFAULT    6 _L_lock_619
   492: 000000000042d54d    27 FUNC    LOCAL  DEFAULT    6 _L_lock_658
   493: 000000000042d568    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_698
   494: 000000000042d587    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_754
   495: 000000000042d5a2    31 FUNC    LOCAL  DEFAULT    6 _L_lock_2750
   496: 000000000042d5c1    27 FUNC    LOCAL  DEFAULT    6 _L_lock_2821
   497: 000000000042d5dc    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_2873
   498: 000000000042d5f7    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_2952
   499: 000000000042d616    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_3203
   500: 000000000042d631    31 FUNC    LOCAL  DEFAULT    6 _L_lock_3271
   501: 000000000042d650    27 FUNC    LOCAL  DEFAULT    6 _L_lock_3335
   502: 000000000042d66b    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_3369
   503: 000000000042d686    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_3403
   504: 000000000042d6a5    31 FUNC    LOCAL  DEFAULT    6 _L_lock_4117
   505: 000000000042d6c4    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_4145
   506: 00000000004b3a88     8 OBJECT  LOCAL  DEFAULT   13 __elf_set___libc_atexit_e
   507: 00000000004b3a50     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   508: 00000000006c5660    16 OBJECT  LOCAL  DEFAULT   27 _IO_stdfile_2_lock
   509: 00000000006c2e80   328 OBJECT  LOCAL  DEFAULT   26 _IO_wide_data_2
   510: 00000000006c5670    16 OBJECT  LOCAL  DEFAULT   27 _IO_stdfile_1_lock
   511: 00000000006c2fe0   328 OBJECT  LOCAL  DEFAULT   26 _IO_wide_data_1
   512: 00000000006c5680    16 OBJECT  LOCAL  DEFAULT   27 _IO_stdfile_0_lock
   513: 00000000006c3140   328 OBJECT  LOCAL  DEFAULT   26 _IO_wide_data_0
   514: 000000000042d940   428 FUNC    LOCAL  DEFAULT    6 enlarge_userbuf.part.0
   515: 000000000049c6b0    16 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11664
   516: 000000000042df60   124 FUNC    LOCAL  DEFAULT    6 __strchr_sse2
   517: 000000000042dfe0   165 FUNC    LOCAL  DEFAULT    6 __strchr_sse42
   518: 000000000042f510  3497 FUNC    LOCAL  DEFAULT    6 __strcmp_sse42
   519: 000000000042e0d0  5177 FUNC    LOCAL  DEFAULT    6 __strcmp_sse2
   520: 0000000000430300   220 FUNC    LOCAL  DEFAULT    6 __strcpy_sse2
   521: 0000000000430620   109 FUNC    LOCAL  DEFAULT    6 __strnlen_sse2
   522: 00000000004324f0   220 FUNC    LOCAL  DEFAULT    6 __stpcpy_sse2
   523: 00000000004348b0  7062 FUNC    LOCAL  DEFAULT    6 __strcasecmp_l_sse42
   524: 0000000000432670  8742 FUNC    LOCAL  DEFAULT    6 __strcasecmp_l_sse2
   525: 0000000000432660    16 FUNC    LOCAL  DEFAULT    6 __strcasecmp_sse2
   526: 0000000000436460  5718 FUNC    LOCAL  DEFAULT    6 __strcasecmp_l_avx
   527: 0000000000438240    79 FUNC    LOCAL  DEFAULT    6 __rawmemchr_sse42
   528: 0000000000438030   516 FUNC    LOCAL  DEFAULT    6 __rawmemchr_sse2
   529: 0000000000445530   321 FUNC    LOCAL  DEFAULT    6 critical_factorization
   530: 0000000000445680   850 FUNC    LOCAL  DEFAULT    6 two_way_long_needle
   531: 00000000006c5690     8 OBJECT  LOCAL  DEFAULT   27 state
   532: 000000000049d15a    10 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11481
   533: 00000000006c5698     8 OBJECT  LOCAL  DEFAULT   27 state
   534: 000000000049d22e    12 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11487
   535: 00000000006c56a0     8 OBJECT  LOCAL  DEFAULT   27 state
   536: 000000000049d2a0    13 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11488
   537: 000000000049d300   104 OBJECT  LOCAL  DEFAULT   10 to_wc
   538: 000000000049d380   104 OBJECT  LOCAL  DEFAULT   10 to_mb
   539: 0000000000446c70    52 FUNC    LOCAL  DEFAULT    6 compute_offset
   540: 0000000000446cb0   718 FUNC    LOCAL  DEFAULT    6 compute_change
   541: 0000000000495230    64 FUNC    LOCAL  DEFAULT    8 free_mem
   542: 00000000006c56e0     8 OBJECT  LOCAL  DEFAULT   27 tzstring_list
   543: 00000000006c56f0     8 OBJECT  LOCAL  DEFAULT   27 old_tz
   544: 00000000006c5700    96 OBJECT  LOCAL  DEFAULT   27 tz_rules
   545: 00000000004479e0   422 FUNC    LOCAL  DEFAULT    6 tzset_internal.part.0
   546: 00000000006c56ec     4 OBJECT  LOCAL  DEFAULT   27 is_initialized.11109
   547: 00000000006c56e8     4 OBJECT  LOCAL  DEFAULT   27 tzset_lock
   548: 0000000000447fa1    31 FUNC    LOCAL  DEFAULT    6 _L_lock_1991
   549: 0000000000447fc0    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_2008
   550: 0000000000447fdf    31 FUNC    LOCAL  DEFAULT    6 _L_lock_2118
   551: 0000000000447ffe    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_2135
   552: 000000000044801d    31 FUNC    LOCAL  DEFAULT    6 _L_lock_2170
   553: 000000000044803c    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_2249
   554: 00000000004b3a58     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   555: 00000000006c6300     8 OBJECT  LOCAL  DEFAULT   28 transitions
   556: 00000000006c5760     8 OBJECT  LOCAL  DEFAULT   27 tzfile_ino
   557: 00000000006c5770     8 OBJECT  LOCAL  DEFAULT   27 tzfile_dev
   558: 00000000006c5780     8 OBJECT  LOCAL  DEFAULT   27 tzfile_mtime
   559: 00000000006c5790     8 OBJECT  LOCAL  DEFAULT   27 num_transitions
   560: 00000000006c57a0     8 OBJECT  LOCAL  DEFAULT   27 num_types
   561: 00000000006c57b0     8 OBJECT  LOCAL  DEFAULT   27 num_leaps
   562: 000000000049dfd0    20 OBJECT  LOCAL  DEFAULT   10 default_tzdir.5686
   563: 00000000006c57d0     8 OBJECT  LOCAL  DEFAULT   27 types
   564: 00000000006c57e0     8 OBJECT  LOCAL  DEFAULT   27 zone_names
   565: 00000000006c57c0     8 OBJECT  LOCAL  DEFAULT   27 type_idxs
   566: 00000000006c57f0     8 OBJECT  LOCAL  DEFAULT   27 leaps
   567: 00000000006c5800     8 OBJECT  LOCAL  DEFAULT   27 tzspec
   568: 00000000006c5810     8 OBJECT  LOCAL  DEFAULT   27 rule_dstoff
   569: 00000000006c5820     8 OBJECT  LOCAL  DEFAULT   27 rule_stdoff
   570: 000000000049d47a    14 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.5850
   571: 000000000049dff0    17 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.5887
   572: 0000000000449800  7835 FUNC    LOCAL  DEFAULT    6 __strftime_internal
   573: 000000000044b6c0   164 FUNC    LOCAL  DEFAULT    6 _nl_init_alt_digit
   574: 000000000044ba50   509 FUNC    LOCAL  DEFAULT    6 _nl_init_era_entries.part
   575: 000000000044be80   121 FUNC    LOCAL  DEFAULT    6 __alloc_dir.constprop.1
   576: 000000000044c187    28 FUNC    LOCAL  DEFAULT    6 _L_lock_20
   577: 000000000044c1a3    25 FUNC    LOCAL  DEFAULT    6 _L_unlock_89
   578: 000000000049e874    12 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.13007
   579: 000000000044c5c0   271 FUNC    LOCAL  DEFAULT    6 __sysconf_check_spec
   580: 000000000049f0d5     9 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.9153
   581: 000000000049f17b    14 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10536
   582: 000000000044d8c0   150 FUNC    LOCAL  DEFAULT    6 trecurse
   583: 000000000044d960   277 FUNC    LOCAL  DEFAULT    6 maybe_split_for_insert.is
   584: 000000000044da80  1426 FUNC    LOCAL  DEFAULT    6 tdestroy_recurse
   585: 00000000006c5850     8 OBJECT  LOCAL  DEFAULT   27 cache
   586: 00000000006c5860     8 OBJECT  LOCAL  DEFAULT   27 cachesize
   587: 00000000006c5870     8 OBJECT  LOCAL  DEFAULT   27 cache_new
   588: 000000000049fae0    22 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10588
   589: 00000000004524d0   417 FUNC    LOCAL  DEFAULT    6 check_match.11143
   590: 000000000049fbcd    12 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11146
   591: 0000000000452680  2259 FUNC    LOCAL  DEFAULT    6 do_lookup_x
   592: 000000000049fd90    19 OBJECT  LOCAL  DEFAULT   10 undefined_msg
   593: 000000000049fd70    20 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11310
   594: 000000000049fbd9    15 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11360
   595: 00000000004a02d0    29 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10772
   596: 00000000004a0320    59 OBJECT  LOCAL  DEFAULT   10 errstring.10964
   597: 00000000004a0280    76 OBJECT  LOCAL  DEFAULT   10 msg.10971
   598: 00000000004a02f0    26 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10872
   599: 00000000004a03ad    14 OBJECT  LOCAL  DEFAULT   10 _dl_out_of_memory
   600: 00000000006c5880     8 OBJECT  LOCAL  DEFAULT   27 receiver
   601: 00000000004558f0  1378 FUNC    LOCAL  DEFAULT    6 _dl_debug_vdprintf
   602: 00000000004a0440    19 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10726
   603: 00000000004a0460   120 OBJECT  LOCAL  DEFAULT   10 primes.10803
   604: 00000000004a0610    19 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11158
   605: 00000000004a0630    22 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11191
   606: 00000000004a0650    20 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11230
   607: 00000000004a0685    15 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10548
   608: 0000000000000050     8 TLS     LOCAL  DEFAULT   17 data.10507
   609: 00000000004a06c8     8 OBJECT  LOCAL  DEFAULT   10 internal_trans_names.1046
   610: 00000000004a06f4     8 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10926
   611: 0000000000495270   203 FUNC    LOCAL  DEFAULT    8 free_derivation
   612: 00000000004a07b0    16 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11102
   613: 0000000000457de0    79 FUNC    LOCAL  DEFAULT    6 derivation_compare
   614: 0000000000495340  1981 FUNC    LOCAL  DEFAULT    8 free_modules_db
   615: 0000000000495b00   175 FUNC    LOCAL  DEFAULT    8 free_mem
   616: 00000000006c58a0     8 OBJECT  LOCAL  DEFAULT   27 known_derivations
   617: 00000000004a0790    21 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11110
   618: 0000000000457ee0  2718 FUNC    LOCAL  DEFAULT    6 find_derivation
   619: 00000000006c5890     4 OBJECT  LOCAL  DEFAULT   27 once
   620: 0000000000458dc6    31 FUNC    LOCAL  DEFAULT    6 _L_lock_3954
   621: 0000000000458de5    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_3968
   622: 0000000000458e04    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_4064
   623: 0000000000458e23    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_4086
   624: 0000000000458e42    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_4198
   625: 0000000000458e61    31 FUNC    LOCAL  DEFAULT    6 _L_lock_4226
   626: 0000000000458e80    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_4258
   627: 00000000004b3a60     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   628: 0000000000495bb0    34 FUNC    LOCAL  DEFAULT    8 free_mem
   629: 00000000004a0960    16 OBJECT  LOCAL  DEFAULT   10 empty_path_elem
   630: 0000000000458ea0   220 FUNC    LOCAL  DEFAULT    6 insert_module
   631: 0000000000458f80   793 FUNC    LOCAL  DEFAULT    6 add_module.isra.1
   632: 00000000004a092d     4 OBJECT  LOCAL  DEFAULT   10 gconv_module_ext
   633: 00000000006c58a8     4 OBJECT  LOCAL  DEFAULT   27 lock.12832
   634: 0000000000459e0f    31 FUNC    LOCAL  DEFAULT    6 _L_lock_676
   635: 0000000000459e2e    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_685
   636: 00000000004a0940    17 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12850
   637: 00000000006c32c0   672 OBJECT  LOCAL  DEFAULT   26 builtin_modules
   638: 00000000004a0980   999 OBJECT  LOCAL  DEFAULT   10 builtin_aliases
   639: 00000000006c58ac     4 OBJECT  LOCAL  DEFAULT   27 modcounter.12812
   640: 00000000004b3a68     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   641: 00000000004a0f20    26 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10503
   642: 00000000004a0da0   384 OBJECT  LOCAL  DEFAULT   10 map
   643: 00000000004a1080    32 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11615
   644: 00000000004a10a0    32 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11711
   645: 00000000004a10c0    34 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11796
   646: 00000000004a1100    34 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11890
   647: 00000000004a1340    21 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11835
   648: 00000000004a1140    33 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11980
   649: 00000000004a1180    33 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12108
   650: 00000000004a1360    27 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12065
   651: 00000000004a11c0    32 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12252
   652: 00000000004a1380    26 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12209
   653: 00000000004a13fa     5 OBJECT  LOCAL  DEFAULT   10 inmask.12333
   654: 00000000004a11e0    32 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12410
   655: 00000000004a13a0    26 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12364
   656: 00000000004a1200    32 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12536
   657: 00000000004a13c0    26 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12493
   658: 00000000004a1220    32 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12666
   659: 00000000004a13e0    26 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12623
   660: 00000000004a1240    39 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12794
   661: 00000000004a1280    33 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12751
   662: 00000000004a12c0    39 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12930
   663: 00000000004a1300    33 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12887
   664: 000000000045fc90   184 FUNC    LOCAL  DEFAULT    6 open_translit
   665: 000000000045fd50    11 FUNC    LOCAL  DEFAULT    6 trans_compare
   666: 00000000006c58b0     4 OBJECT  LOCAL  DEFAULT   27 lock
   667: 000000000046047d    31 FUNC    LOCAL  DEFAULT    6 _L_lock_691
   668: 00000000006c58b8     8 OBJECT  LOCAL  DEFAULT   27 search_tree
   669: 000000000046049c    31 FUNC    LOCAL  DEFAULT    6 _L_unlock_719
   670: 00000000004a1470    22 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11101
   671: 00000000004604c0   224 FUNC    LOCAL  DEFAULT    6 find_module_idx
   672: 00000000006c58c0     8 OBJECT  LOCAL  DEFAULT   27 gconv_cache
   673: 00000000006c58d0     8 OBJECT  LOCAL  DEFAULT   27 cache_size
   674: 00000000004605a0   262 FUNC    LOCAL  DEFAULT    6 find_module
   675: 0000000000495be0    50 FUNC    LOCAL  DEFAULT    8 free_mem
   676: 00000000006c58e0     4 OBJECT  LOCAL  DEFAULT   27 cache_malloced
   677: 00000000004b3a70     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   678: 0000000000495c20    37 FUNC    LOCAL  DEFAULT    8 free_mem
   679: 00000000006c58f0     8 OBJECT  LOCAL  DEFAULT   27 loaded
   680: 0000000000495c50    27 FUNC    LOCAL  DEFAULT    8 do_release_all
   681: 0000000000460de0    11 FUNC    LOCAL  DEFAULT    6 known_compare
   682: 0000000000460df0   117 FUNC    LOCAL  DEFAULT    6 do_release_shlib
   683: 00000000006c5900     8 OBJECT  LOCAL  DEFAULT   27 release_handle
   684: 00000000004a1540    17 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11113
   685: 00000000004a1520    19 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11105
   686: 00000000004b3a78     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   687: 0000000000461070   652 FUNC    LOCAL  DEFAULT    6 new_composite_name
   688: 0000000000495c70   173 FUNC    LOCAL  DEFAULT    8 free_category
   689: 00000000004a1620   104 OBJECT  LOCAL  DEFAULT   10 _nl_current_used
   690: 00000000004a16a0   104 OBJECT  LOCAL  DEFAULT   10 _nl_category_postload
   691: 00000000004a1c80   104 OBJECT  LOCAL  DEFAULT   10 _nl_category_num_items
   692: 00000000004a1d00   104 OBJECT  LOCAL  DEFAULT   10 _nl_value_types
   693: 00000000004a2280    23 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10041
   694: 00000000004a1d80   284 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_CTYPE
   695: 00000000004a1ea0    24 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_NUMERIC
   696: 00000000004a1ec0   444 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_TIME
   697: 00000000004a2080    76 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_COLLATE
   698: 00000000004a20e0   184 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_MONETAR
   699: 00000000004a21a0    20 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_MESSAGE
   700: 00000000004a21b4    12 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_PAPER
   701: 00000000004a21c0    28 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_NAME
   702: 00000000004a21e0    52 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_ADDRESS
   703: 00000000004a2220    20 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_TELEPHO
   704: 00000000004a2234     8 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_MEASURE
   705: 00000000004a2240    64 OBJECT  LOCAL  DEFAULT   10 _nl_value_type_LC_IDENTIF
   706: 00000000006c5960     8 OBJECT  LOCAL  DEFAULT   27 archloaded
   707: 00000000006c5970     8 OBJECT  LOCAL  DEFAULT   27 archmapped
   708: 00000000006c5980    24 OBJECT  LOCAL  DEFAULT   27 headmap
   709: 00000000006c59a0   144 OBJECT  LOCAL  DEFAULT   27 archive_stat
   710: 00000000004a22f0    31 OBJECT  LOCAL  DEFAULT   10 archfname
   711: 00000000004a2310    29 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10163
   712: 00000000004a2330    23 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10211
   713: 00000000004a4020  5420 OBJECT  LOCAL  DEFAULT   10 translit_from_idx
   714: 00000000004a5560 10840 OBJECT  LOCAL  DEFAULT   10 translit_from_tbl
   715: 00000000004a7fc0  5420 OBJECT  LOCAL  DEFAULT   10 translit_to_idx
   716: 00000000004a9500 19220 OBJECT  LOCAL  DEFAULT   10 translit_to_tbl
   717: 0000000000462ec0     0 FUNC    LOCAL  DEFAULT    6 __restore_rt
   718: 000000000046e897    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_1323
   719: 000000000046e8b2    27 FUNC    LOCAL  DEFAULT    6 _L_lock_1379
   720: 00000000004aff30    21 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.13048
   721: 0000000000474b37    27 FUNC    LOCAL  DEFAULT    6 _L_lock_60
   722: 0000000000474b52    27 FUNC    LOCAL  DEFAULT    6 _L_unlock_265
   723: 0000000000474b6d    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_377
   724: 0000000000474e90    32 FUNC    LOCAL  DEFAULT    6 do_encoding
   725: 0000000000474eb0     3 FUNC    LOCAL  DEFAULT    6 do_always_noconv
   726: 0000000000474ec0     8 FUNC    LOCAL  DEFAULT    6 do_max_length
   727: 0000000000474ed0   209 FUNC    LOCAL  DEFAULT    6 do_in
   728: 00000000004b1e40    32 OBJECT  LOCAL  DEFAULT   10 CSWTCH.11
   729: 0000000000474fb0   187 FUNC    LOCAL  DEFAULT    6 do_unshift
   730: 00000000004b1e60    32 OBJECT  LOCAL  DEFAULT   10 CSWTCH.13
   731: 0000000000475070   227 FUNC    LOCAL  DEFAULT    6 do_out
   732: 00000000004b1e80    32 OBJECT  LOCAL  DEFAULT   10 CSWTCH.15
   733: 0000000000475160   184 FUNC    LOCAL  DEFAULT    6 do_length
   734: 00000000004b1d12    10 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.13040
   735: 0000000000475561    27 FUNC    LOCAL  DEFAULT    6 _L_lock_36
   736: 000000000047557c    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_83
   737: 0000000000475594    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_127
   738: 00000000004756f0    27 FUNC    LOCAL  DEFAULT    6 _L_lock_31
   739: 000000000047570b    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_157
   740: 0000000000475723    24 FUNC    LOCAL  DEFAULT    6 _L_unlock_186
   741: 00000000006c5a30     8 OBJECT  LOCAL  DEFAULT   27 internal
   742: 00000000006c5a38     8 OBJECT  LOCAL  DEFAULT   27 state
   743: 00000000004b1ec9    10 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11484
   744: 00000000006c5a40     8 OBJECT  LOCAL  DEFAULT   27 state
   745: 00000000004b1ee1    14 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11487
   746: 00000000004771c0   258 FUNC    LOCAL  DEFAULT    6 ranged_convert
   747: 00000000004772d0   417 FUNC    LOCAL  DEFAULT    6 guess_time_tm.isra.0
   748: 0000000000477480   237 FUNC    LOCAL  DEFAULT    6 ranged_convert.constprop.
   749: 00000000006c5a50     8 OBJECT  LOCAL  DEFAULT   27 localtime_offset
   750: 00000000004795ee    25 FUNC    LOCAL  DEFAULT    6 _L_lock_13
   751: 0000000000479607    25 FUNC    LOCAL  DEFAULT    6 _L_unlock_24
   752: 00000000004b1f68    10 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11866
   753: 00000000004b1fa0    18 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11909
   754: 00000000006c5a70     4 OBJECT  LOCAL  DEFAULT   27 running
   755: 00000000006c5aa0     4 OBJECT  LOCAL  DEFAULT   27 log_hashfraction
   756: 00000000006c5a80     8 OBJECT  LOCAL  DEFAULT   27 lowpc
   757: 00000000006c5a90     8 OBJECT  LOCAL  DEFAULT   27 textsize
   758: 00000000006c5ab0     4 OBJECT  LOCAL  DEFAULT   27 fromlimit
   759: 00000000006c5ab8     8 OBJECT  LOCAL  DEFAULT   27 narcsp
   760: 00000000006c5ac0     8 OBJECT  LOCAL  DEFAULT   27 data
   761: 00000000006c5ac8     8 OBJECT  LOCAL  DEFAULT   27 tos
   762: 00000000006c5a60     4 OBJECT  LOCAL  DEFAULT   27 fromidx
   763: 00000000006c5ad0     8 OBJECT  LOCAL  DEFAULT   27 froms
   764: 00000000006c5ae0     4 OBJECT  LOCAL  DEFAULT   27 narcs
   765: 000000000047a520     5 FUNC    LOCAL  DEFAULT    6 do_dlclose
   766: 000000000047a530    71 FUNC    LOCAL  DEFAULT    6 do_dlsym
   767: 000000000047a580   107 FUNC    LOCAL  DEFAULT    6 do_dlsym_private
   768: 000000000047a5f0    65 FUNC    LOCAL  DEFAULT    6 do_dlopen
   769: 00000000006c3650    24 OBJECT  LOCAL  DEFAULT   26 _dl_open_hook
   770: 0000000000496110  1031 FUNC    LOCAL  DEFAULT    8 free_slotinfo
   771: 0000000000496520   396 FUNC    LOCAL  DEFAULT    8 free_mem
   772: 00000000004b3a80     8 OBJECT  LOCAL  DEFAULT   12 __elf_set___libc_subfreer
   773: 000000000047adc0   491 FUNC    LOCAL  DEFAULT    6 round_and_return
   774: 000000000047afb0   726 FUNC    LOCAL  DEFAULT    6 str_to_mpn.isra.0
   775: 00000000004b2117    11 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12480
   776: 00000000004b2500    22 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12540
   777: 00000000004b2520    64 OBJECT  LOCAL  DEFAULT   10 nbits.12637
   778: 000000000047d510   526 FUNC    LOCAL  DEFAULT    6 round_and_return
   779: 000000000047d720   726 FUNC    LOCAL  DEFAULT    6 str_to_mpn.isra.0
   780: 00000000004b2117    11 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12469
   781: 00000000004b26e0    22 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12529
   782: 00000000004b2700    64 OBJECT  LOCAL  DEFAULT   10 nbits.12626
   783: 000000000047fd40   477 FUNC    LOCAL  DEFAULT    6 round_and_return
   784: 000000000047ff20   734 FUNC    LOCAL  DEFAULT    6 str_to_mpn.isra.0
   785: 00000000004b2117    11 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12469
   786: 00000000004b28c0    23 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12529
   787: 00000000004b28e0    64 OBJECT  LOCAL  DEFAULT   10 nbits.12626
   788: 0000000000482760   202 FUNC    LOCAL  DEFAULT    6 dlinfo_doit
   789: 0000000000482860   104 FUNC    LOCAL  DEFAULT    6 dlmopen_doit
   790: 00000000006c6310     8 OBJECT  LOCAL  DEFAULT   28 buf
   791: 0000000000485170  8118 FUNC    LOCAL  DEFAULT    6 __strncasecmp_l_sse42
   792: 0000000000482b20  9782 FUNC    LOCAL  DEFAULT    6 __strncasecmp_l_sse2
   793: 0000000000482b10    16 FUNC    LOCAL  DEFAULT    6 __strncasecmp_sse2
   794: 0000000000487140  6550 FUNC    LOCAL  DEFAULT    6 __strncasecmp_l_avx
   795: 000000000048b0a0    85 FUNC    LOCAL  DEFAULT    6 profil_counter
   796: 00000000006c5c50     8 OBJECT  LOCAL  DEFAULT   27 pc_offset
   797: 00000000006c5c60     4 OBJECT  LOCAL  DEFAULT   27 pc_scale
   798: 00000000006c5c40     8 OBJECT  LOCAL  DEFAULT   27 nsamples
   799: 00000000006c5b60     8 OBJECT  LOCAL  DEFAULT   27 samples
   800: 00000000006c5b80    32 OBJECT  LOCAL  DEFAULT   27 otimer.9614
   801: 00000000006c5ba0   152 OBJECT  LOCAL  DEFAULT   27 oact.9613
   802: 000000000048b350   486 FUNC    LOCAL  DEFAULT    6 add_to_global
   803: 000000000048b950  2351 FUNC    LOCAL  DEFAULT    6 dl_open_worker
   804: 00000000004b2c2f     9 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12060
   805: 00000000004b2c38    15 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11975
   806: 000000000048c280   314 FUNC    LOCAL  DEFAULT    6 remove_slotinfo
   807: 00000000004b3030    16 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11932
   808: 00000000006c5c70     4 OBJECT  LOCAL  DEFAULT   27 dl_close_state.11944
   809: 00000000004b3010    17 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.11958
   810: 00000000004b2ea8    10 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.12047
   811: 000000000048d1b0   120 FUNC    LOCAL  DEFAULT    6 dlopen_doit
   812: 000000000048d280     5 FUNC    LOCAL  DEFAULT    6 dlclose_doit
   813: 000000000048d2b0    26 FUNC    LOCAL  DEFAULT    6 dlsym_doit
   814: 000000000048d330    30 FUNC    LOCAL  DEFAULT    6 dlvsym_doit
   815: 000000000048d400    56 FUNC    LOCAL  DEFAULT    6 openaux
   816: 000000000048d440  1178 FUNC    LOCAL  DEFAULT    6 _dl_build_local_scope
   817: 00000000004b3240    20 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10772
   818: 000000000048ead0   187 FUNC    LOCAL  DEFAULT    6 call_init.part.0
   819: 000000000048ed50   526 FUNC    LOCAL  DEFAULT    6 _dl_sort_fini.constprop.0
   820: 00000000004b32ad     9 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10580
   821: 000000000048f4a0   893 FUNC    LOCAL  DEFAULT    6 match_symbol
   822: 00000000004b3434    13 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10717
   823: 00000000004b3450    23 OBJECT  LOCAL  DEFAULT   10 __PRETTY_FUNCTION__.10796
   824: 000000000048fce0    62 FUNC    LOCAL  DEFAULT    6 call_dl_lookup
   825: 000000000048fd20   837 FUNC    LOCAL  DEFAULT    6 do_sym
   826: 0000000000490160    70 FUNC    LOCAL  DEFAULT    6 read_sleb128
   827: 00000000004901b0   332 FUNC    LOCAL  DEFAULT    6 read_encoded_value_with_b
   828: 0000000000490300   120 FUNC    LOCAL  DEFAULT    6 init_dwarf_reg_size_table
   829: 00000000006c5c80    18 OBJECT  LOCAL  DEFAULT   27 dwarf_reg_size_table
   830: 0000000000490380  2260 FUNC    LOCAL  DEFAULT    6 execute_cfa_program
   831: 0000000000490c60  1816 FUNC    LOCAL  DEFAULT    6 execute_stack_op
   832: 0000000000491380  1768 FUNC    LOCAL  DEFAULT    6 uw_frame_state_for
   833: 0000000000491a70   353 FUNC    LOCAL  DEFAULT    6 uw_install_context_1
   834: 0000000000491be0   880 FUNC    LOCAL  DEFAULT    6 uw_update_context_1
   835: 0000000000491f50   173 FUNC    LOCAL  DEFAULT    6 uw_update_context
   836: 0000000000492000   201 FUNC    LOCAL  DEFAULT    6 _Unwind_ForcedUnwind_Phas
   837: 00000000004920d0   156 FUNC    LOCAL  DEFAULT    6 _Unwind_RaiseException_Ph
   838: 0000000000492170   513 FUNC    LOCAL  DEFAULT    6 uw_init_context_1
   839: 00000000006c5c94     4 OBJECT  LOCAL  DEFAULT   27 once_regsizes.8575
   840: 00000000004925f0     1 FUNC    LOCAL  DEFAULT    6 _Unwind_DebugHook
   841: 0000000000492af0   175 FUNC    LOCAL  DEFAULT    6 frame_downheap
   842: 0000000000492ba0   196 FUNC    LOCAL  DEFAULT    6 frame_heapsort
   843: 0000000000492c70   103 FUNC    LOCAL  DEFAULT    6 size_of_encoded_value
   844: 0000000000492ce0   332 FUNC    LOCAL  DEFAULT    6 read_encoded_value_with_b
   845: 0000000000492e30   292 FUNC    LOCAL  DEFAULT    6 get_cie_encoding
   846: 0000000000492f60    27 FUNC    LOCAL  DEFAULT    6 fde_unencoded_compare
   847: 0000000000492f80   106 FUNC    LOCAL  DEFAULT    6 base_from_object.isra.3
   848: 0000000000492ff0   373 FUNC    LOCAL  DEFAULT    6 classify_object_over_fdes
   849: 0000000000493170   335 FUNC    LOCAL  DEFAULT    6 add_fdes
   850: 00000000004932c0   176 FUNC    LOCAL  DEFAULT    6 fde_single_encoding_compa
   851: 0000000000493370   396 FUNC    LOCAL  DEFAULT    6 linear_search_fdes
   852: 0000000000493500   106 FUNC    LOCAL  DEFAULT    6 base_from_cb_data.isra.4
   853: 0000000000493570  1693 FUNC    LOCAL  DEFAULT    6 search_object
   854: 0000000000493c10   204 FUNC    LOCAL  DEFAULT    6 fde_mixed_encoding_compar
   855: 00000000006c5e88     8 OBJECT  LOCAL  DEFAULT   27 marker.8218
   856: 00000000006c5e80     8 OBJECT  LOCAL  DEFAULT   27 terminator.8324
   857: 0000000000493ce0  1159 FUNC    LOCAL  DEFAULT    6 _Unwind_IteratePhdrCallba
   858: 00000000006c36e8     8 OBJECT  LOCAL  DEFAULT   26 adds.8480
   859: 00000000006c5d00   384 OBJECT  LOCAL  DEFAULT   27 frame_hdr_cache
   860: 00000000006c5cd8     8 OBJECT  LOCAL  DEFAULT   27 subs.8481
   861: 00000000006c5ce0     8 OBJECT  LOCAL  DEFAULT   27 frame_hdr_cache_head
   862: 00000000006c5ca0    40 OBJECT  LOCAL  DEFAULT   27 object_mutex
   863: 00000000006c5cc8     8 OBJECT  LOCAL  DEFAULT   27 unseen_objects
   864: 00000000006c5cd0     8 OBJECT  LOCAL  DEFAULT   27 seen_objects
   865: 0000000000494630   332 FUNC    LOCAL  DEFAULT    6 read_encoded_value_with_b
   866: 0000000000494bf0    21 FUNC    LOCAL  DEFAULT    6 cancel_handler
   867: 00000000004002f8     0 NOTYPE  LOCAL  DEFAULT    3 __rela_iplt_end
   868: 00000000006c1f20     0 NOTYPE  LOCAL  DEFAULT   19 __fini_array_end
   869: 00000000004001d8     0 NOTYPE  LOCAL  DEFAULT    3 __rela_iplt_start
   870: 00000000006c1f18     0 NOTYPE  LOCAL  DEFAULT   19 __fini_array_start
   871: 00000000006c1f18     0 NOTYPE  LOCAL  DEFAULT   18 __init_array_end
   872: 00000000006c1f10     0 NOTYPE  LOCAL  DEFAULT   17 __preinit_array_end
   873: 00000000006c1fe8     0 OBJECT  LOCAL  DEFAULT   25 _GLOBAL_OFFSET_TABLE_
   874: 00000000006c1f10     0 NOTYPE  LOCAL  DEFAULT   18 __init_array_start
   875: 00000000006c1f10     0 NOTYPE  LOCAL  DEFAULT   17 __preinit_array_start
   876: 00000000004a23c0    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE
   877: 000000000040f7e0  1461 FUNC    GLOBAL DEFAULT    6 __vsyslog_chk
   878: 00000000004324b0    60 IFUNC   WEAK   DEFAULT    6 stpcpy
   879: 00000000004a2a80    76 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_prin
   880: 000000000044e020   292 FUNC    WEAK   DEFAULT    6 tsearch
   881: 00000000006c20d8     8 OBJECT  GLOBAL DEFAULT   26 __morecore
   882: 000000000044d890    35 FUNC    GLOBAL DEFAULT    6 __getdtablesize
   883: 000000000042cfd0    63 FUNC    GLOBAL DEFAULT    6 _IO_remove_marker
   884: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_COLLATE_us
   885: 0000000000462ed0   499 FUNC    GLOBAL DEFAULT    6 __libc_sigaction
   886: 0000000000462db0    63 FUNC    GLOBAL DEFAULT    6 __isnanl
   887: 0000000000476730    28 FUNC    WEAK   DEFAULT    6 mbrlen
   888: 00000000004302c0    60 IFUNC   GLOBAL DEFAULT    6 strcpy
   889: 0000000000427620   337 FUNC    GLOBAL DEFAULT    6 _IO_wdefault_xsgetn
   890: 0000000000426830     5 FUNC    GLOBAL DEFAULT    6 __fcloseall
   891: 000000000040fe30   151 FUNC    GLOBAL DEFAULT    6 __syslog
   892: 0000000000490070   224 FUNC    GLOBAL DEFAULT    6 _dl_vsym
   893: 0000000000453a80   177 FUNC    GLOBAL DEFAULT    6 _dl_setup_hash
   894: 000000000042b920   503 FUNC    GLOBAL DEFAULT    6 _IO_link_in
   895: 00000000006c56d0     4 OBJECT  GLOBAL DEFAULT   27 __daylight
   896: 0000000000494430   508 FUNC    GLOBAL HIDDEN     6 _Unwind_Find_FDE
   897: 0000000000419060   286 FUNC    WEAK   DEFAULT    6 unsetenv
   898: 00000000006c20d0     8 OBJECT  WEAK   DEFAULT   26 __malloc_hook
   899: 0000000000455f10   162 FUNC    GLOBAL DEFAULT    6 _dl_debug_printf
   900: 0000000000462e40   100 FUNC    WEAK   DEFAULT    6 gsignal
   901: 000000000042c7e0    61 FUNC    GLOBAL DEFAULT    6 _IO_sputbackc
   902: 00000000004a2cc0    72 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_uppe
   903: 000000000042c750   124 FUNC    GLOBAL DEFAULT    6 _IO_default_finish
   904: 0000000000409db0    62 IFUNC   WEAK   DEFAULT    6 bcmp
   905: 000000000048f820  1090 FUNC    GLOBAL DEFAULT    6 _dl_check_map_versions
   906: 0000000000438030   516 FUNC    GLOBAL DEFAULT    6 __GI___rawmemchr
   907: 0000000000492440    21 FUNC    GLOBAL HIDDEN     6 _Unwind_GetIPInfo
   908: 000000000045cd70  3495 FUNC    GLOBAL DEFAULT    6 __gconv_transform_utf8_in
   909: 00000000006c4860     8 OBJECT  WEAK   DEFAULT   27 __malloc_initialize_hook
   910: 0000000000409cb0    27 FUNC    GLOBAL DEFAULT    6 __default_morecore
   911: 00000000006c60b8     4 OBJECT  GLOBAL DEFAULT   27 __libc_argc
   912: 0000000000411e00   582 FUNC    GLOBAL DEFAULT    6 __init_cpu_features
   913: 0000000000462df0    80 FUNC    GLOBAL DEFAULT    6 __longjmp
   914: 0000000000455800   127 FUNC    GLOBAL DEFAULT    6 _dl_receive_error
   915: 000000000042a520   116 FUNC    WEAK   DEFAULT    6 _IO_file_finish
   916: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_TELEPHONE
   917: 00000000004a26c0    92 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_width
   918: 000000000044d740    20 FUNC    WEAK   DEFAULT    6 getrlimit
   919: 0000000000495050   214 FUNC    GLOBAL DEFAULT    8 _nl_unload_domain
   920: 000000000044d7c0   151 FUNC    WEAK   DEFAULT    6 writev
   921: 0000000000410789    20 FUNC    GLOBAL DEFAULT    6 __connect_nocancel
   922: 0000000000482830    47 FUNC    GLOBAL DEFAULT    6 __dlinfo
   923: 0000000000412050    21 FUNC    GLOBAL DEFAULT    6 __get_cpu_features
   924: 0000000000492430     8 FUNC    GLOBAL HIDDEN     6 _Unwind_GetIP
   925: 0000000000446c60     8 FUNC    GLOBAL DEFAULT    6 __GI_time
   926: 0000000000463c90   384 FUNC    GLOBAL DEFAULT    6 __mpn_impn_mul_n_basecase
   927: 0000000000427290   208 FUNC    GLOBAL DEFAULT    6 _IO_wdoallocbuf
   928: 000000000044c5a0     8 FUNC    WEAK   DEFAULT    6 getgid
   929: 000000000044c540    54 FUNC    GLOBAL DEFAULT    6 __getpid
   930: 000000000042d3c0    78 FUNC    GLOBAL DEFAULT    6 _IO_list_lock
   931: 0000000000424530   366 FUNC    GLOBAL DEFAULT    6 __register_printf_modifie
   932: 000000000040fda0   138 FUNC    GLOBAL DEFAULT    6 __syslog_chk
   933: 000000000044c6d0  1817 FUNC    WEAK   DEFAULT    6 sysconf
   934: 000000000047ad60    16 FUNC    GLOBAL DEFAULT    6 __strtod_internal
   935: 00000000006c3290     8 OBJECT  GLOBAL DEFAULT   26 stdout
   936: 0000000000426100   224 FUNC    GLOBAL DEFAULT    6 _IO_seekoff_unlocked
   937: 0000000000413c10  5900 FUNC    GLOBAL DEFAULT    6 _nl_load_domain
   938: 00000000006c56d0     4 OBJECT  WEAK   DEFAULT   27 daylight
   939: 000000000042c410   156 FUNC    GLOBAL DEFAULT    6 _IO_default_doallocate
   940: 00000000006c2b80     4 OBJECT  GLOBAL DEFAULT   26 __libc_multiple_libcs
   941: 000000000044d890    35 FUNC    WEAK   DEFAULT    6 getdtablesize
   942: 000000000041a030   709 FUNC    WEAK   DEFAULT    6 __strtoull_l
   943: 0000000000479620   137 FUNC    WEAK   DEFAULT    6 fdopendir
   944: 0000000000437ac0   293 FUNC    GLOBAL DEFAULT    6 _wordcopy_fwd_aligned
   945: 00000000004117d0    35 FUNC    GLOBAL DEFAULT    6 _dl_important_hwcaps
   946: 0000000000429f70   610 FUNC    GLOBAL DEFAULT    6 _IO_new_file_xsputn
   947: 00000000004540c0   209 FUNC    GLOBAL DEFAULT    6 _dl_reloc_bad_type
   948: 00000000004459e0   878 FUNC    GLOBAL DEFAULT    6 __strstr_sse2
   949: 0000000000426c90    53 FUNC    GLOBAL DEFAULT    6 _IO_least_wmarker
   950: 000000000042c740     3 FUNC    GLOBAL DEFAULT    6 _IO_default_sync
   951: 0000000000410780    81 FUNC    WEAK   DEFAULT    6 connect
   952: 0000000000494210    38 FUNC    GLOBAL HIDDEN     6 __register_frame
   953: 000000000042a280   191 FUNC    WEAK   DEFAULT    6 _IO_file_sync
   954: 0000000000447cd0   138 FUNC    GLOBAL DEFAULT    6 __tzset
   955: 00000000004193e0    16 FUNC    GLOBAL DEFAULT    6 __strtoull_internal
   956: 00000000004641f0   385 FUNC    GLOBAL DEFAULT    6 __mpn_impn_sqr_n_basecase
   957: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_once
   958: 000000000041a030   709 FUNC    WEAK   DEFAULT    6 strtoull_l
   959: 0000000000427b20   204 FUNC    GLOBAL DEFAULT    6 _IO_seekwmark
   960: 0000000000425770   248 FUNC    GLOBAL DEFAULT    6 _IO_fflush
   961: 0000000000464970   197 FUNC    GLOBAL DEFAULT    6 __mpn_extract_long_double
   962: 000000000049c2a0   168 OBJECT  GLOBAL DEFAULT   10 _IO_wfile_jumps
   963: 00000000004a2b40    76 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_xdig
   964: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_mutex_lock
   965: 00000000004298f0   145 FUNC    WEAK   DEFAULT    6 _IO_file_write
   966: 0000000000482920   180 FUNC    GLOBAL DEFAULT    6 strerror
   967: 000000000045b3d0  1104 FUNC    GLOBAL DEFAULT    6 __gconv_transform_ascii_i
   968: 0000000000410700    87 FUNC    GLOBAL DEFAULT    6 __init_misc
   969: 0000000000464730   173 FUNC    GLOBAL DEFAULT    6 __mpn_sub_n
   970: 0000000000446aa0   121 FUNC    GLOBAL DEFAULT    6 __wcsmbs_clone_conv
   971: 00000000004011b4    91 FUNC    GLOBAL DEFAULT    6 addvec
   972: 0000000000475830    97 FUNC    WEAK   DEFAULT    6 strndup
   973: 000000000044c590     8 FUNC    WEAK   DEFAULT    6 geteuid
   974: 000000000044c1c0   131 FUNC    GLOBAL DEFAULT    6 __getdents
   975: 00000000006c5f20     8 OBJECT  GLOBAL DEFAULT   27 _dl_profile_output
   976: 00000000004630f0    59 FUNC    GLOBAL DEFAULT    6 __mpn_cmp
   977: 0000000000476730    28 FUNC    GLOBAL DEFAULT    6 __mbrlen
   978: 00000000004079c0   591 FUNC    WEAK   DEFAULT    6 malloc_get_state
   979: 00000000006c2078     8 OBJECT  GLOBAL DEFAULT   26 x
   980: 0000000000476090   206 FUNC    WEAK   DEFAULT    6 argz_add_sep
   981: 00000000006c2a70     8 OBJECT  GLOBAL DEFAULT   26 __x86_64_data_cache_size_
   982: 000000000047ac40   235 FUNC    GLOBAL DEFAULT    6 __mpn_addmul_1
   983: 000000000044cfa0    30 FUNC    GLOBAL DEFAULT    6 __openat_2
   984: 00000000004305f0    41 IFUNC   GLOBAL DEFAULT    6 __strnlen
   985: 0000000000457b10   534 FUNC    GLOBAL DEFAULT    6 __gconv
   986: 0000000000407c10   227 FUNC    GLOBAL DEFAULT    6 __cfree
   987: 0000000000432460    71 IFUNC   GLOBAL DEFAULT    6 memmove
   988: 000000000045db20  1934 FUNC    GLOBAL DEFAULT    6 __gconv_transform_ucs2_in
   989: 00000000006c6140     8 OBJECT  GLOBAL DEFAULT   27 __printf_modifier_table
   990: 0000000000479770   164 FUNC    GLOBAL DEFAULT    6 __tcgetattr
   991: 0000000000453bf0   756 FUNC    GLOBAL DEFAULT    6 _dl_new_object
   992: 0000000000492930   241 FUNC    GLOBAL HIDDEN     6 _Unwind_Resume_or_Rethrow
   993: 00000000004088b0  1034 FUNC    GLOBAL DEFAULT    6 __calloc
   994: 0000000000456bf0    84 FUNC    GLOBAL DEFAULT    6 _dl_make_stack_executable
   995: 000000000042c1a0   276 FUNC    GLOBAL DEFAULT    6 _IO_default_xsgetn
   996: 00000000004100c0    20 FUNC    WEAK   DEFAULT    6 munmap
   997: 00000000006c1f58     8 OBJECT  GLOBAL DEFAULT   23 __libc_stack_end
   998: 0000000000475440    36 FUNC    WEAK   DEFAULT    6 fileno_unlocked
   999: 00000000004a1740    16 OBJECT  GLOBAL DEFAULT   10 _nl_default_locale_path
  1000: 00000000004592a0   966 FUNC    GLOBAL DEFAULT    6 __gconv_get_path
  1001: 0000000000422a50   253 FUNC    GLOBAL DEFAULT    6 __register_printf_specifi
  1002: 00000000006c2b58     4 OBJECT  GLOBAL DEFAULT   26 _dl_debug_fd
  1003: 0000000000499c00    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_NAME
  1004: 000000000044e020   292 FUNC    GLOBAL DEFAULT    6 __tsearch
  1005: 0000000000426550   395 FUNC    GLOBAL DEFAULT    6 _IO_vasprintf
  1006: 0000000000419410  1188 FUNC    GLOBAL DEFAULT    6 ____strtol_l_internal
  1007: 00000000004755b0   320 FUNC    WEAK   DEFAULT    6 ftello64
  1008: 0000000000429e30   232 FUNC    GLOBAL DEFAULT    6 _IO_file_seekoff_mmap
  1009: 000000000040f150   354 FUNC    GLOBAL DEFAULT    6 __libc_fcntl
  1010: 0000000000416740  1875 FUNC    GLOBAL DEFAULT    6 __gettext_free_exp
  1011: 0000000000462d30    36 FUNC    GLOBAL DEFAULT    6 __isnan
  1012: 0000000000451e50  1589 FUNC    GLOBAL DEFAULT    6 _dl_load_cache_lookup
  1013: 0000000000000001     0 NOTYPE  GLOBAL DEFAULT  ABS _nl_current_LC_NUMERIC_us
  1014: 000000000040f000    81 FUNC    WEAK   DEFAULT    6 __write
  1015: 00000000004259f0   227 FUNC    WEAK   DEFAULT    6 _IO_fopen64
  1016: 0000000000417720   259 FUNC    GLOBAL DEFAULT    6 __gettext_extract_plural
  1017: 0000000000409040   434 FUNC    WEAK   DEFAULT    6 malloc_stats
  1018: 000000000042c190    13 FUNC    GLOBAL DEFAULT    6 _IO_sgetn
  1019: 00000000004100a0    23 FUNC    GLOBAL DEFAULT    6 __mmap
  1020: 00000000004100e0    20 FUNC    GLOBAL DEFAULT    6 __mprotect
  1021: 00000000006c2b70     8 OBJECT  GLOBAL DEFAULT   26 _dl_use_load_bias
  1022: 00000000006c6120     8 OBJECT  GLOBAL DEFAULT   27 _nl_domain_bindings
  1023: 00000000006c6230     8 OBJECT  GLOBAL DEFAULT   27 __gconv_path_envvar
  1024: 0000000000492480     8 FUNC    GLOBAL HIDDEN     6 _Unwind_GetRegionStart
  1025: 0000000000424aa0   143 FUNC    GLOBAL DEFAULT    6 __dprintf
  1026: 0000000000418b80  1115 FUNC    GLOBAL DEFAULT    6 __add_to_environ
  1027: 00000000006c5f30    16 OBJECT  GLOBAL DEFAULT   27 _dl_initial_searchlist
  1028: 0000000000418650   219 FUNC    GLOBAL DEFAULT    6 getenv
  1029: 0000000000429100     8 FUNC    GLOBAL DEFAULT    6 _IO_file_seek
  1030: 00000000004762f0   664 FUNC    WEAK   DEFAULT    6 wcslen
  1031: 0000000000474060  1729 FUNC    GLOBAL DEFAULT    6 __parse_one_specwc
  1032: 000000000041a560   203 FUNC    GLOBAL DEFAULT    6 _itoa_word
  1033: 00000000006c2a10     8 OBJECT  GLOBAL DEFAULT   26 __x86_64_raw_shared_cache
  1034: 0000000000000020     4 TLS     GLOBAL DEFAULT   17 errno
  1035: 000000000047ada0    18 FUNC    WEAK   DEFAULT    6 strtold
  1036: 0000000000447c10   190 FUNC    GLOBAL DEFAULT    6 __tz_compute
  1037: 000000000044c5b0     8 FUNC    WEAK   DEFAULT    6 getegid
  1038: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_rwlock_init
  1039: 000000000044e600   103 FUNC    GLOBAL DEFAULT    6 __tdestroy
  1040: 0000000000437ff0    54 IFUNC   GLOBAL DEFAULT    6 __rawmemchr
  1041: 00000000004799c0   472 FUNC    GLOBAL DEFAULT    6 _dl_profile_fixup
  1042: 000000000044d040  1785 FUNC    GLOBAL DEFAULT    6 __getcwd
  1043: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_IDENTIFICA
  1044: 00000000004769e0   983 FUNC    GLOBAL DEFAULT    6 __mbsrtowcs_l
  1045: 0000000000492a50   155 FUNC    GLOBAL HIDDEN     6 _Unwind_Backtrace
  1046: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_key_create
  1047: 000000000042cf70    86 FUNC    GLOBAL DEFAULT    6 _IO_init_marker
  1048: 00000000004193b0    16 FUNC    GLOBAL DEFAULT    6 __strtol_internal
  1049: 00000000004a156d    13 OBJECT  GLOBAL DEFAULT   10 _nl_category_name_idxs
  1050: 0000000000417c60   138 FUNC    GLOBAL DEFAULT    6 bsearch
  1051: 0000000000487130    16 FUNC    GLOBAL DEFAULT    6 __strncasecmp_avx
  1052: 0000000000445e60     9 FUNC    WEAK   DEFAULT    6 wmempcpy
  1053: 00000000006c32a0    16 OBJECT  GLOBAL DEFAULT   26 __tzname
  1054: 00000000004270a0    71 FUNC    GLOBAL DEFAULT    6 __woverflow
  1055: 00000000006c2cc0   224 OBJECT  GLOBAL DEFAULT   26 _IO_2_1_stdout_
  1056: 0000000000422b50     5 FUNC    GLOBAL DEFAULT    6 __register_printf_functio
  1057: 0000000000426330   141 FUNC    WEAK   DEFAULT    6 vsscanf
  1058: 00000000004646c0   110 FUNC    GLOBAL DEFAULT    6 __mpn_mul_n
  1059: 000000000042a340    60 FUNC    GLOBAL DEFAULT    6 _IO_new_file_init
  1060: 000000000044d860    46 FUNC    WEAK   DEFAULT    6 getpagesize
  1061: 000000000044c540    54 FUNC    WEAK   DEFAULT    6 getpid
  1062: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_rwlock_wrlock
  1063: 00000000004395d0 11111 FUNC    GLOBAL DEFAULT    6 __memmove_ssse3
  1064: 00000000004822d0    10 FUNC    WEAK   DEFAULT    6 __strtold_l
  1065: 0000000000460920  1174 FUNC    GLOBAL DEFAULT    6 __gconv_lookup_cache
  1066: 0000000000456180   110 FUNC    GLOBAL DEFAULT    6 _dl_higher_prime_number
  1067: 000000000044cec0   214 FUNC    WEAK   DEFAULT    6 __openat64
  1068: 00000000004a2960    76 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_cntr
  1069: 0000000000418640     5 FUNC    GLOBAL DEFAULT    6 qsort
  1070: 00000000004092a0   118 FUNC    GLOBAL DEFAULT    6 __posix_memalign
  1071: 000000000042cd50   532 FUNC    GLOBAL DEFAULT    6 _IO_flush_all_linebuffere
  1072: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_TELEPHONE_
  1073: 00000000004254d0   527 FUNC    WEAK   DEFAULT    6 _IO_fclose
  1074: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_PAPER
  1075: 00000000006c6210     8 OBJECT  GLOBAL DEFAULT   27 __gconv_modules_db
  1076: 00000000004193b0    16 FUNC    GLOBAL DEFAULT    6 __strtoll_internal
  1077: 0000000000415820   309 FUNC    GLOBAL DEFAULT    6 _nl_expand_alias
  1078: 00000000004288d0   294 FUNC    GLOBAL DEFAULT    6 _IO_wdo_write
  1079: 0000000000474870   711 FUNC    WEAK   DEFAULT    6 __getdelim
  1080: 000000000040efa0    81 FUNC    WEAK   DEFAULT    6 __read
  1081: 00000000004769b0    34 FUNC    GLOBAL DEFAULT    6 __wcschrnul
  1082: 000000000042c020     6 FUNC    GLOBAL DEFAULT    6 _IO_default_underflow
  1083: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _dl_rtld_map
  1084: 0000000000424bc0    50 FUNC    GLOBAL DEFAULT    6 _IO_funlockfile
  1085: 000000000044d740    20 FUNC    WEAK   DEFAULT    6 getrlimit64
  1086: 000000000048eb90   434 FUNC    GLOBAL DEFAULT    6 _dl_init
  1087: 00000000004606c0   454 FUNC    GLOBAL DEFAULT    6 __gconv_load_cache
  1088: 0000000000409200   130 FUNC    GLOBAL DEFAULT    6 __mallinfo
  1089: 000000000045ae30  1437 FUNC    GLOBAL DEFAULT    6 __gconv_transform_ucs4le_
  1090: 00000000006c5f40     8 OBJECT  GLOBAL DEFAULT   27 _dl_platformlen
  1091: 00000000006c5ea0     8 OBJECT  GLOBAL DEFAULT   27 _dl_tls_static_used
  1092: 0000000000427420   125 FUNC    GLOBAL DEFAULT    6 _IO_switch_to_wget_mode
  1093: 00000000006c2a60     8 OBJECT  GLOBAL DEFAULT   26 __x86_64_data_cache_size
  1094: 0000000000446c40    13 FUNC    GLOBAL DEFAULT    6 __localtime_r
  1095: 00000000006c20c0     8 OBJECT  WEAK   DEFAULT   26 __realloc_hook
  1096: 000000000048b040    82 FUNC    GLOBAL DEFAULT    6 __strncasecmp_l_nonascii
  1097: 00000000004923d0     8 FUNC    GLOBAL HIDDEN     6 _Unwind_GetCFA
  1098: 00000000006c2090     8 OBJECT  GLOBAL DEFAULT   26 __exit_funcs
  1099: 0000000000416fa0  1907 FUNC    GLOBAL DEFAULT    6 __gettextparse
  1100: 000000000040b120  1125 FUNC    GLOBAL DEFAULT    6 memcpy
  1101: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND pthread_cancel
  1102: 000000000048d3e0    20 FUNC    WEAK   DEFAULT    6 setitimer
  1103: 0000000000482ac0    79 IFUNC   GLOBAL DEFAULT    6 __strncasecmp
  1104: 000000000042c060   290 FUNC    GLOBAL DEFAULT    6 _IO_default_xsputn
  1105: 0000000000463570   259 FUNC    GLOBAL DEFAULT    6 __mpn_lshift
  1106: 00000000004623e0   721 FUNC    GLOBAL DEFAULT    6 _nl_load_locale
  1107: 0000000000438290    57 FUNC    WEAK   DEFAULT    6 argz_count
  1108: 0000000000420170 10385 FUNC    GLOBAL DEFAULT    6 ___printf_fp
  1109: 0000000000425e10   397 FUNC    GLOBAL DEFAULT    6 _IO_fwrite
  1110: 000000000042c2c0   295 FUNC    GLOBAL DEFAULT    6 _IO_default_setbuf
  1111: 000000000042c820    54 FUNC    GLOBAL DEFAULT    6 _IO_sungetc
  1112: 0000000000453ef0   183 FUNC    GLOBAL DEFAULT    6 _dl_try_allocate_static_t
  1113: 000000000048d2d0    91 FUNC    GLOBAL DEFAULT    6 __dlsym
  1114: 00000000004606b0     8 FUNC    GLOBAL DEFAULT    6 __gconv_get_cache
  1115: 0000000000411b70    83 FUNC    GLOBAL DEFAULT    6 _dl_addr_inside_object
  1116: 0000000000460e70   479 FUNC    GLOBAL DEFAULT    6 __gconv_find_shlib
  1117: 0000000000475220   542 FUNC    GLOBAL DEFAULT    6 _IO_fwide
  1118: 0000000000410780    81 FUNC    WEAK   DEFAULT    6 __connect_internal
  1119: 00000000004198c0   747 FUNC    WEAK   DEFAULT    6 strtoll_l
  1120: 00000000004626c0    87 FUNC    GLOBAL DEFAULT    6 _nl_unload_locale
  1121: 00000000006c1f38     0 OBJECT  GLOBAL HIDDEN    21 __DTOR_END__
  1122: 000000000042a380   411 FUNC    GLOBAL DEFAULT    6 _IO_new_file_close_it
  1123: 00000000006c5f48     4 OBJECT  GLOBAL DEFAULT   27 _dl_debug_mask
  1124: 0000000000428da0   635 FUNC    GLOBAL DEFAULT    6 _IO_wfile_overflow
  1125: 00000000004080a0   614 FUNC    GLOBAL DEFAULT    6 __libc_memalign
  1126: 0000000000445d90    50 FUNC    GLOBAL DEFAULT    6 __strcasecmp_l_nonascii
  1127: 00000000004601a0   733 FUNC    GLOBAL DEFAULT    6 __gconv_translit_find
  1128: 000000000047a860   130 FUNC    GLOBAL DEFAULT    6 __libc_dlsym_private
  1129: 000000000042bc80    33 FUNC    GLOBAL DEFAULT    6 __overflow
  1130: 0000000000476750   570 FUNC    WEAK   DEFAULT    6 mbrtowc
  1131: 0000000000476590   414 FUNC    GLOBAL DEFAULT    6 __btowc
  1132: 0000000000463790   982 FUNC    GLOBAL DEFAULT    6 __mpn_mul
  1133: 000000000049af20   280 OBJECT  GLOBAL DEFAULT   10 __strtol_ul_max_tab
  1134: 00000000004114b0   791 FUNC    GLOBAL DEFAULT    6 _dl_non_dynamic_init
  1135: 000000000044c580     8 FUNC    WEAK   DEFAULT    6 getuid
  1136: 0000000000401ff0   101 FUNC    GLOBAL DEFAULT    6 __internal_atexit
  1137: 0000000000479590    94 FUNC    GLOBAL DEFAULT    6 rewinddir
  1138: 0000000000462cf0    53 FUNC    GLOBAL DEFAULT    6 __isinf
  1139: 00000000004080a0   614 FUNC    GLOBAL DEFAULT    6 __memalign
  1140: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_MEASUREMEN
  1141: 00000000004647e0   235 FUNC    GLOBAL DEFAULT    6 __mpn_submul_1
  1142: 0000000000429990     8 FUNC    GLOBAL DEFAULT    6 _IO_file_close
  1143: 00000000004382d0    67 FUNC    WEAK   DEFAULT    6 argz_stringify
  1144: 0000000000408cc0   817 FUNC    GLOBAL DEFAULT    6 __malloc_trim
  1145: 0000000000482720     9 FUNC    GLOBAL DEFAULT    6 __dladdr
  1146: 00000000006c2b90     8 OBJECT  GLOBAL DEFAULT   26 _nl_current_default_domai
  1147: 00000000006c6128     4 OBJECT  GLOBAL DEFAULT   27 _nl_msg_cat_cntr
  1148: 0000000000407600   600 FUNC    GLOBAL DEFAULT    6 malloc
  1149: 00000000004b1d20    40 OBJECT  GLOBAL DEFAULT   10 __libio_translit
  1150: 000000000040ef40    81 FUNC    WEAK   DEFAULT    6 __open
  1151: 000000000042d100   106 FUNC    GLOBAL DEFAULT    6 _IO_unsave_markers
  1152: 00000000004a3d20   768 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class
  1153: 0000000000479750    25 FUNC    WEAK   DEFAULT    6 isatty
  1154: 000000000047b290  8807 FUNC    GLOBAL DEFAULT    6 ____strtof_l_internal
  1155: 00000000006c5f50     8 OBJECT  GLOBAL DEFAULT   27 _dl_load_adds
  1156: 000000000049ae80    32 OBJECT  GLOBAL DEFAULT   10 __gettext_germanic_plural
  1157: 000000000044e690    81 FUNC    WEAK   DEFAULT    6 __llseek
  1158: 00000000004467e0    82 FUNC    GLOBAL DEFAULT    6 __wcsmbs_getfct
  1159: 00000000006c2da0   224 OBJECT  GLOBAL DEFAULT   26 _IO_2_1_stdin_
  1160: 000000000045a0a0  1021 FUNC    GLOBAL DEFAULT    6 __gconv_transform_interna
  1161: 000000000044ce10    39 FUNC    GLOBAL DEFAULT    6 __get_child_max
  1162: 0000000000443d70  1571 FUNC    GLOBAL DEFAULT    6 __strcpy_sse2_unaligned
  1163: 0000000000454060    90 FUNC    GLOBAL DEFAULT    6 _dl_protect_relro
  1164: 000000000044cec0   214 FUNC    WEAK   DEFAULT    6 openat64
  1165: 0000000000430440   421 FUNC    GLOBAL DEFAULT    6 __strerror_r
  1166: 0000000000424a10   143 FUNC    GLOBAL DEFAULT    6 __asprintf
  1167: 0000000000476590   414 FUNC    WEAK   DEFAULT    6 btowc
  1168: 0000000000446840   603 FUNC    GLOBAL DEFAULT    6 __wcsmbs_load_conv
  1169: 00000000004193c0    18 FUNC    WEAK   DEFAULT    6 strtoll
  1170: 000000000044b6a0    27 FUNC    GLOBAL DEFAULT    6 __strftime_l
  1171: 0000000000464380   827 FUNC    GLOBAL DEFAULT    6 __mpn_impn_sqr_n
  1172: 00000000004b1878     4 OBJECT  WEAK   DEFAULT   10 sys_nerr
  1173: 0000000000424530   366 FUNC    WEAK   DEFAULT    6 register_printf_modifier
  1174: 0000000000426480   207 FUNC    GLOBAL DEFAULT    6 open_memstream
  1175: 0000000000499cc0    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_ADDRESS
  1176: 00000000006c5f58     8 OBJECT  GLOBAL DEFAULT   27 _dl_wait_lookup_done
  1177: 0000000000457220    12 FUNC    GLOBAL DEFAULT    6 _dl_mcount_wrapper
  1178: 00000000004567c0   123 FUNC    GLOBAL DEFAULT    6 _dl_deallocate_tls
  1179: 00000000004a2a20    76 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_grap
  1180: 0000000000463e10   981 FUNC    GLOBAL DEFAULT    6 __mpn_impn_mul_n
  1181: 0000000000462cd0    23 FUNC    GLOBAL DEFAULT    6 __current_locale_name
  1182: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_rwlock_rdlock
  1183: 00000000004a2d20  1536 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_tolower
  1184: 00000000006c5f60     8 OBJECT  GLOBAL DEFAULT   27 _dl_profile
  1185: 0000000000410db0   275 FUNC    GLOBAL DEFAULT    6 ___vfprintf_chk
  1186: 00000000004193f0    18 FUNC    GLOBAL DEFAULT    6 strtoul
  1187: 00000000006c2068     0 OBJECT  GLOBAL HIDDEN    26 __dso_handle
  1188: 000000000048d100    36 FUNC    GLOBAL DEFAULT    6 __mpn_construct_float
  1189: 0000000000475f00   133 FUNC    GLOBAL DEFAULT    6 __strsep
  1190: 0000000000401e80   361 FUNC    GLOBAL DEFAULT    6 __new_exitfn
  1191: 00000000004108b0    66 FUNC    GLOBAL DEFAULT    6 __libc_alloca_cutoff
  1192: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_NAME_used
  1193: 000000000048f1a0   756 FUNC    GLOBAL DEFAULT    6 _dl_fini
  1194: 00000000004822d0    10 FUNC    WEAK   DEFAULT    6 strtold_l
  1195: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __nptl_deallocate_tsd
  1196: 0000000000426cd0    49 FUNC    GLOBAL DEFAULT    6 _IO_switch_to_main_wget_a
  1197: 00000000004122c0    15 FUNC    GLOBAL DEFAULT    6 __dcgettext
  1198: 0000000000401b40    51 FUNC    GLOBAL DEFAULT    6 __libc_csu_fini
  1199: 0000000000000001     0 NOTYPE  GLOBAL DEFAULT  ABS _nl_current_LC_CTYPE_used
  1200: 0000000000425c80   320 FUNC    GLOBAL DEFAULT    6 _IO_ftell
  1201: 000000000044b6a0    27 FUNC    WEAK   DEFAULT    6 strftime_l
  1202: 000000000042de60   147 FUNC    GLOBAL DEFAULT    6 _IO_str_init_readonly
  1203: 00000000004299e0  1039 FUNC    WEAK   DEFAULT    6 _IO_file_seekoff
  1204: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_TIME
  1205: 0000000000411c20   270 FUNC    GLOBAL DEFAULT    6 _dl_discover_osversion
  1206: 000000000044be30    66 FUNC    GLOBAL DEFAULT    6 _nl_cleanup_time
  1207: 000000000040b600  5694 FUNC    GLOBAL DEFAULT    6 __memcmp_sse4_1
  1208: 0000000000411bd0    69 FUNC    GLOBAL DEFAULT    6 __libc_init_secure
  1209: 0000000000410890    20 FUNC    WEAK   DEFAULT    6 socket
  1210: 0000000000453fe0   127 FUNC    GLOBAL DEFAULT    6 _dl_nothread_init_static_
  1211: 00000000004924e0   269 FUNC    GLOBAL HIDDEN     6 __frame_state_for
  1212: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _pthread_cleanup_pop_rest
  1213: 0000000000476dc0  1019 FUNC    GLOBAL DEFAULT    6 __offtime
  1214: 000000000044c0b0   215 FUNC    WEAK   DEFAULT    6 readdir
  1215: 0000000000427a10    68 FUNC    GLOBAL DEFAULT    6 _IO_adjust_wcolumn
  1216: 00000000004193e0    16 FUNC    GLOBAL DEFAULT    6 __strtoul_internal
  1217: 000000000042daf0   503 FUNC    GLOBAL DEFAULT    6 _IO_str_seekoff
  1218: 0000000000408600   679 FUNC    WEAK   DEFAULT    6 pvalloc
  1219: 0000000000412260    87 FUNC    GLOBAL DEFAULT    6 __ctype_init
  1220: 000000000044c5a0     8 FUNC    GLOBAL DEFAULT    6 __getgid
  1221: 00000000004796b0    69 FUNC    WEAK   DEFAULT    6 _lxstat
  1222: 000000000044ce40    69 FUNC    WEAK   DEFAULT    6 _xstat
  1223: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_rwlock_unlock
  1224: 0000000000429df0    52 FUNC    WEAK   DEFAULT    6 _IO_file_setbuf
  1225: 000000000044e690    81 FUNC    WEAK   DEFAULT    6 __lseek64
  1226: 000000000042a670  2184 FUNC    GLOBAL DEFAULT    6 _IO_new_file_fopen
  1227: 000000000040acc0  1106 FUNC    WEAK   DEFAULT    6 mempcpy
  1228: 0000000000409200   130 FUNC    GLOBAL DEFAULT    6 __libc_mallinfo
  1229: 0000000000425770   248 FUNC    WEAK   DEFAULT    6 fflush
  1230: 00000000004259f0   227 FUNC    GLOBAL DEFAULT    6 _IO_new_fopen
  1231: 00000000006c4988     8 OBJECT  WEAK   DEFAULT   27 _environ
  1232: 00000000006c5f68     8 OBJECT  GLOBAL DEFAULT   27 _dl_cpuclock_offset
  1233: 000000000045a090    16 FUNC    GLOBAL DEFAULT    6 __gconv_btwoc_ascii
  1234: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_MESSAGES
  1235: 00000000004762f0   664 FUNC    GLOBAL DEFAULT    6 __wcslen
  1236: 000000000042d350     3 FUNC    GLOBAL DEFAULT    6 _IO_default_write
  1237: 0000000000411de3     0 NOTYPE  GLOBAL DEFAULT    6 __syscall_error_1
  1238: 000000000040efa0    81 FUNC    GLOBAL DEFAULT    6 __libc_read
  1239: 0000000000425370   339 FUNC    GLOBAL DEFAULT    6 __fxprintf
  1240: 0000000000447b90   114 FUNC    GLOBAL DEFAULT    6 __tzname_max
  1241: 00000000006c5f04     4 OBJECT  GLOBAL DEFAULT   27 __have_sock_cloexec
  1242: 00000000004109b0    89 FUNC    GLOBAL HIDDEN     6 __libc_disable_asynccance
  1243: 0000000000458a70   676 FUNC    GLOBAL DEFAULT    6 __gconv_find_transform
  1244: 0000000000494780  1123 FUNC    GLOBAL HIDDEN     6 __gcc_personality_v0
  1245: 000000000044ce40    69 FUNC    GLOBAL DEFAULT    6 __xstat64
  1246: 00000000004299a0    45 FUNC    GLOBAL DEFAULT    6 _IO_file_close_mmap
  1247: 000000000042df60   124 FUNC    GLOBAL DEFAULT    6 __GI_strchr
  1248: 00000000004562f0   313 FUNC    GLOBAL DEFAULT    6 _dl_allocate_tls_storage
  1249: 000000000040eed0    19 FUNC    GLOBAL DEFAULT    6 __exit_thread
  1250: 000000000044e690    81 FUNC    WEAK   DEFAULT    6 lseek
  1251: 0000000000407d00   844 FUNC    GLOBAL DEFAULT    6 __libc_realloc
  1252: 0000000000445dd0     9 FUNC    WEAK   DEFAULT    6 wmemcpy
  1253: 0000000000000040     8 TLS     GLOBAL DEFAULT   17 __libc_tsd_CTYPE_TOLOWER
  1254: 000000000045ebe0  1915 FUNC    GLOBAL DEFAULT    6 __gconv_transform_ucs2rev
  1255: 0000000000419180   151 FUNC    WEAK   DEFAULT    6 clearenv
  1256: 00000000006c5eb0     8 OBJECT  GLOBAL DEFAULT   27 _dl_tls_static_align
  1257: 0000000000456b20   208 FUNC    GLOBAL DEFAULT    6 _dl_scope_free
  1258: 00000000006c4988     8 OBJECT  GLOBAL DEFAULT   27 __environ
  1259: 00000000004100a0    23 FUNC    WEAK   DEFAULT    6 mmap
  1260: 0000000000482ac0    79 IFUNC   WEAK   DEFAULT    6 strncasecmp
  1261: 000000000040ee80    80 FUNC    WEAK   DEFAULT    6 _Exit
  1262: 00000000004198c0   747 FUNC    WEAK   DEFAULT    6 strtol_l
  1263: 00000000004620b0   801 FUNC    GLOBAL DEFAULT    6 _nl_intern_locale_data
  1264: 0000000000452f60  2835 FUNC    GLOBAL DEFAULT    6 _dl_lookup_symbol_x
  1265: 0000000000446790    66 FUNC    GLOBAL DEFAULT    6 _nl_cleanup_ctype
  1266: 00000000006c5ec0     8 OBJECT  GLOBAL DEFAULT   27 _dl_tls_max_dtv_idx
  1267: 00000000004107e0   171 FUNC    WEAK   DEFAULT    6 send
  1268: 00000000004a27e0   168 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_map_touppe
  1269: 00000000004a2900    76 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_punc
  1270: 0000000000417950   679 FUNC    GLOBAL DEFAULT    6 abort
  1271: 00000000006c5920    56 OBJECT  GLOBAL DEFAULT   27 __libc_setlocale_lock
  1272: 00000000004178f0    38 FUNC    GLOBAL DEFAULT    6 __sigjmp_save
  1273: 000000000048d080   126 FUNC    GLOBAL DEFAULT    6 _dl_close
  1274: 0000000000420170 10385 FUNC    GLOBAL DEFAULT    6 __printf_fp
  1275: 00000000006c32a0    16 OBJECT  WEAK   DEFAULT   26 tzname
  1276: 00000000006c5f70     4 OBJECT  GLOBAL DEFAULT   27 _dl_bind_not
  1277: 00000000006c1f70     4 OBJECT  GLOBAL DEFAULT   23 __libc_enable_secure
  1278: 0000000000426970   235 FUNC    GLOBAL DEFAULT    6 _IO_wpadn
  1279: 0000000000462c50   114 FUNC    GLOBAL DEFAULT    6 _nl_postload_ctype
  1280: 000000000044e1a0  1088 FUNC    WEAK   DEFAULT    6 tdelete
  1281: 0000000000425ae0   334 FUNC    GLOBAL DEFAULT    6 _IO_fputs
  1282: 000000000045a4a0  1437 FUNC    GLOBAL DEFAULT    6 __gconv_transform_ucs4_in
  1283: 000000000040ef49    20 FUNC    GLOBAL DEFAULT    6 __open_nocancel
  1284: 00000000004002f8     0 FUNC    GLOBAL DEFAULT    4 _init
  1285: 00000000004a2ba0    68 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_digi
  1286: 000000000042d770    27 FUNC    GLOBAL DEFAULT    6 _IO_str_pbackfail
  1287: 0000000000428a00   547 FUNC    GLOBAL DEFAULT    6 _IO_wfile_xsputn
  1288: 00000000006c6240     8 OBJECT  GLOBAL DEFAULT   27 __gconv_max_path_elem_len
  1289: 000000000042d370     2 FUNC    GLOBAL DEFAULT    6 _IO_default_imbue
  1290: 0000000000463130  1088 FUNC    GLOBAL DEFAULT    6 __mpn_divrem
  1291: 00000000004193c0    18 FUNC    WEAK   DEFAULT    6 strtoq
  1292: 00000000004193c0    18 FUNC    GLOBAL DEFAULT    6 strtol
  1293: 0000000000417890    87 FUNC    GLOBAL DEFAULT    6 __sigsetjmp
  1294: 000000000044e690    81 FUNC    WEAK   DEFAULT    6 __libc_lseek64
  1295: 00000000006c2a00     8 OBJECT  GLOBAL DEFAULT   26 __x86_64_raw_shared_cache
  1296: 00000000004828d0    74 FUNC    GLOBAL DEFAULT    6 __dlmopen
  1297: 0000000000411020   685 FUNC    GLOBAL DEFAULT    6 __backtrace_symbols_fd
  1298: 00000000004305f0    41 IFUNC   WEAK   DEFAULT    6 strnlen
  1299: 0000000000437ff0    54 IFUNC   GLOBAL DEFAULT    6 rawmemchr
  1300: 0000000000401a20   129 FUNC    GLOBAL DEFAULT    6 __libc_csu_irel
  1301: 00000000004796b0    69 FUNC    GLOBAL DEFAULT    6 __lxstat
  1302: 000000000044c250    20 FUNC    WEAK   DEFAULT    6 uname
  1303: 00000000004324f0   220 FUNC    GLOBAL DEFAULT    6 __GI_stpcpy
  1304: 0000000000413980   645 FUNC    GLOBAL DEFAULT    6 _nl_find_domain
  1305: 000000000042d340     8 FUNC    GLOBAL DEFAULT    6 _IO_default_read
  1306: 00000000004942d0    26 FUNC    GLOBAL HIDDEN     6 __register_frame_table
  1307: 000000000042a380   411 FUNC    WEAK   DEFAULT    6 _IO_file_close_it
  1308: 00000000004b1878     4 OBJECT  GLOBAL DEFAULT   10 __sys_nerr_internal
  1309: 00000000004b1878     4 OBJECT  WEAK   DEFAULT   10 _sys_nerr
  1310: 00000000006c5f80     8 OBJECT  GLOBAL DEFAULT   27 _dl_platform
  1311: 000000000042d380     8 FUNC    GLOBAL DEFAULT    6 _IO_iter_begin
  1312: 000000000047da00  9004 FUNC    GLOBAL DEFAULT    6 ____strtod_l_internal
  1313: 00000000004a3920  1024 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class32
  1314: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND pthread_setcancelstate
  1315: 00000000004562d0    21 FUNC    GLOBAL DEFAULT    6 _dl_get_tls_static_info
  1316: 0000000000446390   827 FUNC    WEAK   DEFAULT    6 mbsnrtowcs
  1317: 0000000000431ee0   159 FUNC    GLOBAL DEFAULT    6 strrchr
  1318: 0000000000412240    17 FUNC    GLOBAL DEFAULT    6 __ctype_tolower_loc
  1319: 0000000000401460   158 FUNC    GLOBAL DEFAULT    6 __libc_check_standard_fds
  1320: 00000000006c4840     8 OBJECT  WEAK   DEFAULT   27 __after_morecore_hook
  1321: 000000000048d130    58 FUNC    GLOBAL DEFAULT    6 __mpn_construct_double
  1322: 00000000004088b0  1034 FUNC    WEAK   DEFAULT    6 calloc
  1323: 00000000004b3a88     0 NOTYPE  GLOBAL DEFAULT  ABS __start___libc_atexit
  1324: 000000000048d3e0    20 FUNC    GLOBAL DEFAULT    6 __setitimer
  1325: 00000000004325d0    60 IFUNC   WEAK   DEFAULT    6 strcasecmp_l
  1326: 00000000006c60b0     4 OBJECT  GLOBAL DEFAULT   27 __libc_enable_secure_deci
  1327: 00000000004299d0    16 FUNC    GLOBAL DEFAULT    6 _IO_file_stat
  1328: 0000000000411dd0     9 FUNC    GLOBAL DEFAULT    6 _dl_start
  1329: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_mutex_unlock
  1330: 0000000000409000    52 FUNC    WEAK   DEFAULT    6 malloc_usable_size
  1331: 0000000000424b30   143 FUNC    GLOBAL DEFAULT    6 __sscanf
  1332: 000000000047ad90    16 FUNC    GLOBAL DEFAULT    6 __strtold_internal
  1333: 000000000044e600   103 FUNC    WEAK   DEFAULT    6 tdestroy
  1334: 0000000000448f80   495 FUNC    GLOBAL DEFAULT    6 __tzfile_default
  1335: 0000000000494170   130 FUNC    GLOBAL HIDDEN     6 __register_frame_info_bas
  1336: 0000000000428c30   357 FUNC    GLOBAL DEFAULT    6 _IO_wfile_sync
  1337: 0000000000408600   679 FUNC    GLOBAL DEFAULT    6 __libc_pvalloc
  1338: 00000000004198c0   747 FUNC    WEAK   DEFAULT    6 __strtoll_l
  1339: 0000000000456c50    97 FUNC    GLOBAL DEFAULT    6 _dl_runtime_resolve
  1340: 000000000047ad70    18 FUNC    WEAK   DEFAULT    6 strtod
  1341: 00000000004651f0 38556 FUNC    GLOBAL DEFAULT    6 _IO_vfscanf_internal
  1342: 0000000000462cf0    53 FUNC    WEAK   DEFAULT    6 isinf
  1343: 000000000044e699    20 FUNC    GLOBAL DEFAULT    6 __lseek_nocancel
  1344: 0000000000431ee0   159 FUNC    WEAK   DEFAULT    6 rindex
  1345: 000000000044e7e0   444 FUNC    GLOBAL DEFAULT    6 __readonly_area
  1346: 000000000047a980   412 FUNC    GLOBAL DEFAULT    6 _dl_tlsdesc_resolve_rela_
  1347: 0000000000422a10    63 FUNC    GLOBAL DEFAULT    6 __guess_grouping
  1348: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_getspecific
  1349: 000000000040f000    81 FUNC    WEAK   DEFAULT    6 write
  1350: 0000000000408350   677 FUNC    GLOBAL DEFAULT    6 __libc_valloc
  1351: 000000000047fd30    10 FUNC    WEAK   DEFAULT    6 __strtod_l
  1352: 0000000000410fc0    92 FUNC    WEAK   DEFAULT    6 backtrace
  1353: 00000000004a2720   168 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_map_tolowe
  1354: 00000000006c61c0     8 OBJECT  GLOBAL DEFAULT   27 __fork_generation_pointer
  1355: 0000000000410fc0    92 FUNC    GLOBAL DEFAULT    6 __backtrace
  1356: 0000000000495d20   510 FUNC    GLOBAL DEFAULT    8 _nl_locale_subfreeres
  1357: 00000000006c4988     8 OBJECT  WEAK   DEFAULT   27 environ
  1358: 0000000000413060  2148 FUNC    GLOBAL DEFAULT    6 __dcigettext
  1359: 0000000000424980   143 FUNC    GLOBAL DEFAULT    6 fprintf
  1360: 0000000000447030  2479 FUNC    GLOBAL DEFAULT    6 __tzset_parse_tz
  1361: 00000000006c5e90     8 OBJECT  GLOBAL DEFAULT   27 z
  1362: 00000000004268f0   123 FUNC    GLOBAL DEFAULT    6 fputs_unlocked
  1363: 0000000000453b40   167 FUNC    GLOBAL DEFAULT    6 _dl_add_to_namespace_list
  1364: 000000000048d170    63 FUNC    GLOBAL DEFAULT    6 __mpn_construct_long_doub
  1365: 0000000000494d10   135 FUNC    GLOBAL DEFAULT    6 dl_iterate_phdr
  1366: 000000000049c600   168 OBJECT  GLOBAL DEFAULT   10 _IO_str_jumps
  1367: 000000000042d740    43 FUNC    GLOBAL DEFAULT    6 _IO_str_finish
  1368: 0000000000416370   262 FUNC    GLOBAL DEFAULT    6 _nl_normalize_codeset
  1369: 00000000004122c0    15 FUNC    WEAK   DEFAULT    6 dcgettext
  1370: 00000000006c2080     8 OBJECT  GLOBAL DEFAULT   26 _dl_tls_static_size
  1371: 0000000000455fc0   162 FUNC    GLOBAL DEFAULT    6 _dl_debug_printf_c
  1372: 000000000042d360     6 FUNC    GLOBAL DEFAULT    6 _IO_default_showmanyc
  1373: 000000000047d500    10 FUNC    WEAK   DEFAULT    6 strtof_l
  1374: 00000000004103e0   603 FUNC    GLOBAL DEFAULT    6 __get_nprocs
  1375: 0000000000479750    25 FUNC    GLOBAL DEFAULT    6 __isatty
  1376: 00000000006c5400    56 OBJECT  GLOBAL DEFAULT   27 _nl_state_lock
  1377: 000000000048b340     7 FUNC    GLOBAL DEFAULT    6 __profile_frequency
  1378: 00000000006c5f88     4 OBJECT  GLOBAL DEFAULT   27 _dl_lazy
  1379: 0000000000455880     2 FUNC    GLOBAL DEFAULT    6 _dl_debug_state
  1380: 000000000045b820  2258 FUNC    GLOBAL DEFAULT    6 __gconv_transform_interna
  1381: 00000000004324b0    60 IFUNC   GLOBAL DEFAULT    6 __stpcpy
  1382: 00000000004100a0    23 FUNC    WEAK   DEFAULT    6 __mmap64
  1383: 000000000044b940   267 FUNC    GLOBAL DEFAULT    6 _nl_parse_alt_digit
  1384: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND pthread_once
  1385: 000000000042d790   426 FUNC    GLOBAL DEFAULT    6 _IO_str_overflow
  1386: 00000000004943f0     5 FUNC    GLOBAL HIDDEN     6 __deregister_frame_info
  1387: 0000000000457260    16 FUNC    GLOBAL DEFAULT    6 _dl_initial_error_catch_t
  1388: 0000000000410100    20 FUNC    GLOBAL DEFAULT    6 madvise
  1389: 0000000000407600   600 FUNC    GLOBAL DEFAULT    6 __malloc
  1390: 0000000000432670  8742 FUNC    GLOBAL DEFAULT    6 __GI___strcasecmp_l
  1391: 000000000044ce90    43 FUNC    WEAK   DEFAULT    6 __openat64_nocancel
  1392: 00000000004107e0   171 FUNC    WEAK   DEFAULT    6 __send
  1393: 0000000000450ec0   828 FUNC    GLOBAL DEFAULT    6 _dl_init_paths
  1394: 00000000004295d0   473 FUNC    GLOBAL DEFAULT    6 _IO_file_xsgetn
  1395: 000000000042cb10   553 FUNC    GLOBAL DEFAULT    6 _IO_cleanup
  1396: 0000000000417830    59 FUNC    GLOBAL DEFAULT    6 __hash_string
  1397: 00000000006c1f60     8 OBJECT  GLOBAL DEFAULT   23 _dl_argv
  1398: 000000000042c3f0    17 FUNC    GLOBAL DEFAULT    6 _IO_default_seekpos
  1399: 0000000000457380  1936 FUNC    GLOBAL DEFAULT    6 __gconv_open
  1400: 0000000000492860   193 FUNC    GLOBAL HIDDEN     6 _Unwind_Resume
  1401: 0000000000407c10   227 FUNC    GLOBAL DEFAULT    6 __free
  1402: 000000000048d290    29 FUNC    GLOBAL DEFAULT    6 __dlclose
  1403: 0000000000492a30    26 FUNC    GLOBAL HIDDEN     6 _Unwind_DeleteException
  1404: 00000000006c2ba8     2 OBJECT  GLOBAL DEFAULT   26 __fpu_control
  1405: 000000000045e2b0  2349 FUNC    GLOBAL DEFAULT    6 __gconv_transform_interna
  1406: 00000000006c5f00     4 OBJECT  GLOBAL DEFAULT   27 __x86_64_prefetchw
  1407: 0000000000410bc0   413 FUNC    GLOBAL DEFAULT    6 __printf_chk
  1408: 0000000000475470   241 FUNC    GLOBAL DEFAULT    6 fseek
  1409: 0000000000410760    23 FUNC    WEAK   DEFAULT    6 mremap
  1410: 000000000044d740    20 FUNC    GLOBAL DEFAULT    6 __getrlimit
  1411: 000000000042afa0    35 FUNC    GLOBAL DEFAULT    6 _IO_new_do_write
  1412: 000000000042e0d0  5177 FUNC    GLOBAL DEFAULT    6 __GI_strcmp
  1413: 0000000000000008     8 TLS     GLOBAL DEFAULT   16 _nl_current_LC_CTYPE
  1414: 000000000044c0b0   215 FUNC    GLOBAL DEFAULT    6 __readdir64
  1415: 000000000042afd0   568 FUNC    WEAK   DEFAULT    6 _IO_file_underflow
  1416: 0000000000474870   711 FUNC    WEAK   DEFAULT    6 getdelim
  1417: 0000000000480200  8385 FUNC    GLOBAL DEFAULT    6 ____strtold_l_internal
  1418: 0000000000461050    24 FUNC    GLOBAL DEFAULT    6 __gconv_release_shlib
  1419: 0000000000499940    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_MONETARY
  1420: 000000000040efa9    20 FUNC    GLOBAL DEFAULT    6 __read_nocancel
  1421: 0000000000415e80  1261 FUNC    GLOBAL DEFAULT    6 _nl_make_l10nflist
  1422: 0000000000425900   239 FUNC    GLOBAL DEFAULT    6 __fopen_internal
  1423: 000000000043c140     9 FUNC    GLOBAL DEFAULT    6 __memmove_chk_ssse3_back
  1424: 000000000042c620   284 FUNC    GLOBAL DEFAULT    6 _IO_no_init
  1425: 000000000040b590   111 FUNC    GLOBAL DEFAULT    6 __strchrnul
  1426: 000000000047a8f0   134 FUNC    GLOBAL DEFAULT    6 __libc_register_dl_open_h
  1427: 00000000004b2920   160 OBJECT  GLOBAL DEFAULT   10 _tens_in_limb
  1428: 0000000000425ff0   267 FUNC    GLOBAL DEFAULT    6 _IO_padn
  1429: 000000000042b210   546 FUNC    WEAK   DEFAULT    6 _IO_file_overflow
  1430: 0000000000431f80   836 FUNC    GLOBAL DEFAULT    6 memchr
  1431: 0000000000474cf0   405 FUNC    GLOBAL DEFAULT    6 _IO_getline_info
  1432: 000000000040fed0    16 FUNC    GLOBAL DEFAULT    6 __vsyslog
  1433: 00000000004017a0   630 FUNC    WEAK   DEFAULT    6 __pthread_initialize_mini
  1434: 000000000044e7d0    14 FUNC    GLOBAL DEFAULT    6 __chk_fail
  1435: 000000000044c0b0   215 FUNC    GLOBAL DEFAULT    6 __readdir
  1436: 0000000000424c10  1881 FUNC    GLOBAL DEFAULT    6 __parse_one_specmb
  1437: 00000000006c3298     8 OBJECT  GLOBAL DEFAULT   26 stdin
  1438: 000000000044e150    72 FUNC    WEAK   DEFAULT    6 tfind
  1439: 0000000000411020   685 FUNC    WEAK   DEFAULT    6 backtrace_symbols_fd
  1440: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_TIME_used
  1441: 0000000000456cc0  1373 FUNC    GLOBAL DEFAULT    6 _dl_runtime_profile
  1442: 000000000042dda0   182 FUNC    GLOBAL DEFAULT    6 _IO_str_init_static
  1443: 00000000006c3290     8 OBJECT  GLOBAL HIDDEN    26 _IO_stdout
  1444: 0000000000450850   757 FUNC    GLOBAL DEFAULT    6 _dl_dst_substitute
  1445: 00000000004aea20   312 OBJECT  GLOBAL DEFAULT   10 _fpioconst_pow10
  1446: 00000000006c5ed0     8 OBJECT  GLOBAL DEFAULT   27 _dl_tls_dtv_slotinfo_list
  1447: 0000000000424aa0   143 FUNC    GLOBAL DEFAULT    6 dprintf
  1448: 0000000000456430   519 FUNC    GLOBAL DEFAULT    6 _dl_allocate_tls_init
  1449: 00000000006c61a0     8 OBJECT  GLOBAL DEFAULT   27 __tzname_cur_max
  1450: 0000000000457d30   155 FUNC    GLOBAL DEFAULT    6 __gconv_close
  1451: 0000000000445e70   548 FUNC    GLOBAL DEFAULT    6 __wcrtomb
  1452: 00000000004785b0  4053 FUNC    GLOBAL DEFAULT    6 mktime
  1453: 00000000006c2ab8     8 OBJECT  GLOBAL DEFAULT   26 __progname
  1454: 00000000006c56c0     8 OBJECT  WEAK   DEFAULT   27 timezone
  1455: 00000000004010a8     0 FUNC    GLOBAL DEFAULT    6 _start
  1456: 00000000004942f0   256 FUNC    GLOBAL HIDDEN     6 __deregister_frame_info_b
  1457: 00000000004b3a90     0 NOTYPE  GLOBAL DEFAULT  ABS __stop___libc_atexit
  1458: 000000000042cd40    10 FUNC    GLOBAL DEFAULT    6 _IO_flush_all
  1459: 0000000000445d50    55 IFUNC   GLOBAL DEFAULT    6 strstr
  1460: 00000000004254d0   527 FUNC    GLOBAL DEFAULT    6 _IO_new_fclose
  1461: 000000000042d3b0     4 FUNC    GLOBAL DEFAULT    6 _IO_iter_file
  1462: 000000000042c860    64 FUNC    GLOBAL DEFAULT    6 _IO_adjust_column
  1463: 000000000042c8a0   615 FUNC    GLOBAL DEFAULT    6 _IO_flush_all_lockp
  1464: 00000000004755b0   320 FUNC    GLOBAL DEFAULT    6 ftello
  1465: 0000000000000020     4 TLS     GLOBAL DEFAULT   17 __libc_errno
  1466: 00000000004070a0  1363 FUNC    WEAK   DEFAULT    6 malloc_set_state
  1467: 000000000041a300   597 FUNC    GLOBAL DEFAULT    6 __correctly_grouped_prefi
  1468: 0000000000411d30   156 FUNC    GLOBAL DEFAULT    6 __libc_init_first
  1469: 000000000040efa0    81 FUNC    WEAK   DEFAULT    6 read
  1470: 00000000006c32b8     8 OBJECT  GLOBAL DEFAULT   26 _dl_error_catch_tsd
  1471: 000000000040fee0   102 FUNC    GLOBAL DEFAULT    6 openlog
  1472: 0000000000455670   149 FUNC    GLOBAL DEFAULT    6 _dl_signal_cerror
  1473: 00000000004648d0   156 FUNC    GLOBAL DEFAULT    6 __mpn_extract_double
  1474: 000000000040ff50   154 FUNC    GLOBAL DEFAULT    6 closelog
  1475: 0000000000438290    57 FUNC    GLOBAL DEFAULT    6 __argz_count
  1476: 0000000000430690  6217 FUNC    GLOBAL DEFAULT    6 strncmp
  1477: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_PAPER_used
  1478: 000000000043ec70  8502 FUNC    GLOBAL DEFAULT    6 __strcasecmp_l_ssse3
  1479: 000000000049a0c0    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_COLLATE
  1480: 0000000000479700    77 FUNC    GLOBAL DEFAULT    6 __fxstatat
  1481: 0000000000424980   143 FUNC    WEAK   DEFAULT    6 _IO_fprintf
  1482: 0000000000419390    19 FUNC    GLOBAL DEFAULT    6 __secure_getenv
  1483: 0000000000416480   697 FUNC    GLOBAL DEFAULT    6 _nl_explode_name
  1484: 000000000046e9e0 21493 FUNC    GLOBAL DEFAULT    6 _IO_vfwprintf
  1485: 0000000000427360   179 FUNC    GLOBAL DEFAULT    6 _IO_wdefault_doallocate
  1486: 0000000000457290   111 FUNC    GLOBAL HIDDEN     6 _dl_tlsdesc_resolve_rela
  1487: 00000000004460a0   752 FUNC    WEAK   DEFAULT    6 wcsrtombs
  1488: 0000000000401b90   254 FUNC    GLOBAL DEFAULT    6 __run_exit_handlers
  1489: 0000000000407600   600 FUNC    GLOBAL DEFAULT    6 __libc_malloc
  1490: 0000000000410b50    40 FUNC    GLOBAL DEFAULT    6 __linkin_atfork
  1491: 000000000040a220  2705 FUNC    GLOBAL DEFAULT    6 __memset_sse2
  1492: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __nptl_set_robust
  1493: 0000000000445de0   109 FUNC    GLOBAL DEFAULT    6 wmemset
  1494: 00000000004106f0    10 FUNC    WEAK   DEFAULT    6 get_avphys_pages
  1495: 000000000042d020    47 FUNC    GLOBAL DEFAULT    6 _IO_marker_delta
  1496: 0000000000407c10   227 FUNC    GLOBAL DEFAULT    6 __libc_free
  1497: 0000000000418fe0   126 FUNC    WEAK   DEFAULT    6 setenv
  1498: 0000000000429f20    71 FUNC    GLOBAL DEFAULT    6 _IO_file_underflow_mmap
  1499: 00000000004279c0    65 FUNC    GLOBAL DEFAULT    6 _IO_sungetwc
  1500: 00000000006c2ab8     8 OBJECT  WEAK   DEFAULT   26 program_invocation_short_
  1501: 0000000000432610    79 IFUNC   WEAK   DEFAULT    6 strcasecmp
  1502: 0000000000437e90   342 FUNC    GLOBAL DEFAULT    6 _wordcopy_bwd_dest_aligne
  1503: 000000000044bf40    62 FUNC    GLOBAL DEFAULT    6 __opendir
  1504: 000000000042df00    18 FUNC    GLOBAL DEFAULT    6 _IO_str_count
  1505: 00000000006c62e8     8 OBJECT  GLOBAL DEFAULT   28 __printf_arginfo_table
  1506: 000000000048b540   749 FUNC    GLOBAL DEFAULT    6 _dl_open
  1507: 0000000000424bc0    50 FUNC    WEAK   DEFAULT    6 funlockfile
  1508: 00000000004292d0    26 FUNC    GLOBAL DEFAULT    6 _IO_file_underflow_maybe_
  1509: 0000000000410bc0   413 FUNC    GLOBAL DEFAULT    6 ___printf_chk
  1510: 0000000000408600   679 FUNC    GLOBAL DEFAULT    6 __pvalloc
  1511: 0000000000407d00   844 FUNC    GLOBAL DEFAULT    6 realloc
  1512: 00000000004a2ae0    68 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_spac
  1513: 000000000044c5b0     8 FUNC    GLOBAL DEFAULT    6 __getegid
  1514: 0000000000410a10   319 FUNC    GLOBAL DEFAULT    6 __register_atfork
  1515: 0000000000426830     5 FUNC    WEAK   DEFAULT    6 fcloseall
  1516: 000000000049c120   168 OBJECT  GLOBAL DEFAULT   10 _IO_wfile_jumps_maybe_mma
  1517: 000000000048fc70   100 FUNC    GLOBAL DEFAULT    6 _dl_check_all_versions
  1518: 0000000000455890    89 FUNC    GLOBAL DEFAULT    6 _dl_debug_initialize
  1519: 0000000000447d60   577 FUNC    GLOBAL DEFAULT    6 __tz_convert
  1520: 0000000000475f90   246 FUNC    GLOBAL DEFAULT    6 __argz_create_sep
  1521: 00000000004303e0    89 FUNC    GLOBAL DEFAULT    6 __strdup
  1522: 00000000006c5ed8     1 OBJECT  GLOBAL DEFAULT   27 _dl_tls_dtv_gaps
  1523: 0000000000457dd0    11 FUNC    GLOBAL DEFAULT    6 __gconv_alias_compare
  1524: 0000000000401cb0   449 FUNC    GLOBAL DEFAULT    6 __cxa_atexit
  1525: 000000000040d150  5987 FUNC    GLOBAL DEFAULT    6 __memcmp_ssse3
  1526: 00000000004107e0   171 FUNC    GLOBAL DEFAULT    6 __libc_send
  1527: 0000000000445e50     9 FUNC    GLOBAL DEFAULT    6 __wmemmove
  1528: 0000000000429f70   610 FUNC    WEAK   DEFAULT    6 _IO_file_xsputn
  1529: 000000000044d760    86 FUNC    GLOBAL DEFAULT    6 __brk
  1530: 000000000044c0b0   215 FUNC    WEAK   DEFAULT    6 readdir64
  1531: 00000000004a1760   104 OBJECT  GLOBAL DEFAULT   10 _nl_C
  1532: 0000000000427ad0    68 FUNC    GLOBAL DEFAULT    6 _IO_wmarker_delta
  1533: 0000000000430300   220 FUNC    GLOBAL DEFAULT    6 __GI_strcpy
  1534: 00000000004466d0   177 FUNC    WEAK   DEFAULT    6 wcsnlen
  1535: 0000000000422a50   253 FUNC    WEAK   DEFAULT    6 register_printf_specifier
  1536: 0000000000409290     5 FUNC    GLOBAL DEFAULT    6 __libc_mallopt
  1537: 000000000044e780    66 FUNC    WEAK   DEFAULT    6 towctrans
  1538: 000000000042d330     6 FUNC    GLOBAL DEFAULT    6 _IO_default_stat
  1539: 000000000042a280   191 FUNC    GLOBAL DEFAULT    6 _IO_new_file_sync
  1540: 0000000000409db0    62 IFUNC   GLOBAL DEFAULT    6 memcmp
  1541: 000000000049c3a0   168 OBJECT  GLOBAL DEFAULT   10 _IO_file_jumps_maybe_mmap
  1542: 000000000048b100   572 FUNC    GLOBAL DEFAULT    6 __profil
  1543: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_MESSAGES_u
  1544: 000000000047ab90   173 FUNC    GLOBAL DEFAULT    6 __mpn_add_n
  1545: 0000000000408cc0   817 FUNC    WEAK   DEFAULT    6 malloc_trim
  1546: 0000000000000018     8 TLS     GLOBAL DEFAULT   16 _nl_current_LC_NUMERIC
  1547: 000000000044c270   713 FUNC    WEAK   DEFAULT    6 fork
  1548: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_ADDRESS
  1549: 0000000000419bb0  1151 FUNC    GLOBAL DEFAULT    6 ____strtoul_l_internal
  1550: 0000000000424b30   143 FUNC    GLOBAL DEFAULT    6 sscanf
  1551: 00000000004a3320  1536 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_toupper
  1552: 0000000000492600   358 FUNC    GLOBAL HIDDEN     6 _Unwind_RaiseException
  1553: 000000000044cdf0    20 FUNC    GLOBAL DEFAULT    6 __sched_yield
  1554: 00000000004325d0    60 IFUNC   GLOBAL DEFAULT    6 __strcasecmp_l
  1555: 00000000004afa00   144 OBJECT  GLOBAL DEFAULT   10 _itowa_lower_digits
  1556: 000000000042d010     7 FUNC    GLOBAL DEFAULT    6 _IO_marker_difference
  1557: 0000000000456990   387 FUNC    GLOBAL DEFAULT    6 _dl_get_origin
  1558: 00000000004630d0    31 FUNC    WEAK   DEFAULT    6 sigaction
  1559: 00000000006c5f90     8 OBJECT  GLOBAL DEFAULT   27 _dl_phdr
  1560: 0000000000476160   388 FUNC    GLOBAL DEFAULT    6 wcschr
  1561: 0000000000427900   113 FUNC    GLOBAL DEFAULT    6 _IO_free_wbackup_area
  1562: 00000000006c20a0     4 OBJECT  GLOBAL DEFAULT   26 __libc_malloc_initialized
  1563: 0000000000456110   107 FUNC    GLOBAL DEFAULT    6 _dl_name_match_p
  1564: 0000000000462050    82 FUNC    GLOBAL DEFAULT    6 _nl_remove_locale
  1565: 000000000044d860    46 FUNC    GLOBAL DEFAULT    6 __getpagesize
  1566: 0000000000476750   570 FUNC    GLOBAL DEFAULT    6 __mbrtowc
  1567: 000000000048d230    69 FUNC    GLOBAL DEFAULT    6 __dlopen
  1568: 000000000042bc20    94 FUNC    GLOBAL DEFAULT    6 _IO_free_backup_area
  1569: 0000000000411de0    19 FUNC    GLOBAL DEFAULT    6 __syscall_error
  1570: 00000000004ae640    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_TIME
  1571: 000000000042a340    60 FUNC    WEAK   DEFAULT    6 _IO_file_init
  1572: 000000000040f2c0   161 FUNC    WEAK   DEFAULT    6 sbrk
  1573: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_MEASUREMEN
  1574: 000000000049b040    36 OBJECT  GLOBAL DEFAULT   10 _itoa_lower_digits
  1575: 000000000044cfc0    81 FUNC    GLOBAL DEFAULT    6 __libc_close
  1576: 00000000004303e0    89 FUNC    WEAK   DEFAULT    6 strdup
  1577: 00000000004997c0   232 OBJECT  GLOBAL DEFAULT   10 _nl_C_locobj
  1578: 000000000042bcb0   288 FUNC    GLOBAL DEFAULT    6 __underflow
  1579: 0000000000459e50   570 FUNC    GLOBAL DEFAULT    6 __gconv_get_builtin_trans
  1580: 00000000006c5f98     8 OBJECT  GLOBAL DEFAULT   27 _dl_nns
  1581: 0000000000479700    77 FUNC    GLOBAL DEFAULT    6 __fxstatat64
  1582: 0000000000492460     8 FUNC    GLOBAL HIDDEN     6 _Unwind_SetIP
  1583: 0000000000401ab0   137 FUNC    GLOBAL DEFAULT    6 __libc_csu_init
  1584: 00000000006c1f50     8 OBJECT  GLOBAL DEFAULT   23 _dl_random
  1585: 00000000006c5590     8 OBJECT  GLOBAL DEFAULT   27 __abort_msg
  1586: 000000000047ab70    25 FUNC    GLOBAL DEFAULT    6 _dl_unmap
  1587: 00000000006c5fa0     8 OBJECT  GLOBAL DEFAULT   27 _dl_scope_free_list
  1588: 0000000000410640   156 FUNC    GLOBAL DEFAULT    6 __get_nprocs_conf
  1589: 0000000000457e50   144 FUNC    GLOBAL DEFAULT    6 __gconv_release_step
  1590: 00000000004193f0    18 FUNC    WEAK   DEFAULT    6 strtoull
  1591: 000000000042df20    61 IFUNC   WEAK   DEFAULT    6 index
  1592: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _pthread_cleanup_push_def
  1593: 00000000004259f0   227 FUNC    WEAK   DEFAULT    6 fopen
  1594: 00000000006c36f0     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_start
  1595: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_unwind
  1596: 000000000040ef40    81 FUNC    GLOBAL DEFAULT    6 __libc_open
  1597: 00000000004270f0   416 FUNC    GLOBAL DEFAULT    6 _IO_wdefault_xsputn
  1598: 000000000045c100  3184 FUNC    GLOBAL DEFAULT    6 __gconv_transform_interna
  1599: 0000000000446c50    15 FUNC    GLOBAL DEFAULT    6 localtime
  1600: 000000000042c030    36 FUNC    GLOBAL DEFAULT    6 _IO_default_uflow
  1601: 000000000040a1f0    41 IFUNC   GLOBAL DEFAULT    6 memset
  1602: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_rwlock_destroy
  1603: 0000000000445e60     9 FUNC    GLOBAL DEFAULT    6 __wmempcpy
  1604: 00000000004198c0   747 FUNC    WEAK   DEFAULT    6 __strtol_l
  1605: 0000000000400de0    65 FUNC    GLOBAL DEFAULT    6 main
  1606: 000000000040cc40  1281 FUNC    GLOBAL DEFAULT    6 __memset_x86_64
  1607: 0000000000479bb0  1783 FUNC    GLOBAL DEFAULT    6 _dl_start_profile
  1608: 00000000006c5fa8     8 OBJECT  GLOBAL DEFAULT   27 _dl_origin_path
  1609: 00000000004466d0   177 FUNC    GLOBAL DEFAULT    6 __wcsnlen
  1610: 000000000049d2e0    32 OBJECT  GLOBAL DEFAULT   10 __wcsmbs_gconv_fcts_c
  1611: 00000000006c60e0    52 OBJECT  GLOBAL DEFAULT   27 __cpu_features
  1612: 0000000000000001     0 NOTYPE  GLOBAL DEFAULT  ABS _nl_current_LC_MONETARY_u
  1613: 00000000004b1440  1080 OBJECT  WEAK   DEFAULT   10 _sys_errlist
  1614: 0000000000425c80   320 FUNC    WEAK   DEFAULT    6 ftell
  1615: 000000000042a520   116 FUNC    GLOBAL DEFAULT    6 _IO_new_file_finish
  1616: 0000000000401730    99 FUNC    GLOBAL DEFAULT    6 _dl_tls_setup
  1617: 00000000006c5ee0     8 OBJECT  GLOBAL DEFAULT   27 _dl_tls_generation
  1618: 00000000006c6218     4 OBJECT  GLOBAL DEFAULT   27 __gconv_lock
  1619: 00000000004106e0    10 FUNC    WEAK   DEFAULT    6 get_phys_pages
  1620: 000000000046e9e0 21493 FUNC    WEAK   DEFAULT    6 vfwprintf
  1621: 0000000000479700    77 FUNC    GLOBAL DEFAULT    6 __GI___fxstatat64
  1622: 0000000000476990    28 FUNC    WEAK   DEFAULT    6 mbsrtowcs
  1623: 000000000042af00   148 FUNC    GLOBAL DEFAULT    6 _IO_new_file_attach
  1624: 00000000004324f0   220 FUNC    GLOBAL DEFAULT    6 __GI___stpcpy
  1625: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __nptl_nthreads
  1626: 00000000004b1ea0    31 OBJECT  GLOBAL DEFAULT   10 ___m128i_shift_right
  1627: 0000000000409290     5 FUNC    WEAK   DEFAULT    6 mallopt
  1628: 00000000004254d0   527 FUNC    WEAK   DEFAULT    6 fclose
  1629: 0000000000410db0   275 FUNC    GLOBAL DEFAULT    6 __vfprintf_chk
  1630: 000000000044e9a0    55 FUNC    GLOBAL DEFAULT    6 __fortify_fail
  1631: 00000000006c5fb0     4 OBJECT  GLOBAL DEFAULT   27 _dl_clktck
  1632: 0000000000451d50   244 FUNC    GLOBAL DEFAULT    6 _dl_cache_libcmp
  1633: 00000000004b1f20    52 OBJECT  GLOBAL DEFAULT   10 __mon_yday
  1634: 000000000040ef40    81 FUNC    WEAK   DEFAULT    6 open64
  1635: 00000000004541a0  4991 FUNC    GLOBAL DEFAULT    6 _dl_relocate_object
  1636: 00000000006c5f08     4 OBJECT  GLOBAL DEFAULT   27 __have_pipe2
  1637: 0000000000409320   239 FUNC    GLOBAL DEFAULT    6 malloc_info
  1638: 0000000000479770   164 FUNC    WEAK   DEFAULT    6 tcgetattr
  1639: 000000000044d7c0   151 FUNC    GLOBAL DEFAULT    6 __libc_writev
  1640: 00000000004b1440  1080 OBJECT  WEAK   DEFAULT   10 sys_errlist
  1641: 00000000006c5fb4     4 OBJECT  GLOBAL DEFAULT   27 _dl_dynamic_weak
  1642: 000000000041aa00 21919 FUNC    GLOBAL DEFAULT    6 _IO_vfprintf_internal
  1643: 00000000004758a0  1620 FUNC    GLOBAL DEFAULT    6 __strstr_sse42
  1644: 0000000000446c60     8 FUNC    GLOBAL DEFAULT    6 time
  1645: 000000000044bf40    62 FUNC    WEAK   DEFAULT    6 opendir
  1646: 00000000004274a0   382 FUNC    GLOBAL DEFAULT    6 __wunderflow
  1647: 00000000006c2070     8 OBJECT  GLOBAL DEFAULT   26 y
  1648: 000000000042bdd0   288 FUNC    GLOBAL DEFAULT    6 __uflow
  1649: 0000000000494240   114 FUNC    GLOBAL HIDDEN     6 __register_frame_info_tab
  1650: 000000000044bdc0   103 FUNC    GLOBAL DEFAULT    6 _nl_select_era_entry
  1651: 00000000004507b0   156 FUNC    GLOBAL DEFAULT    6 _dl_dst_count
  1652: 0000000000424b30   143 FUNC    GLOBAL DEFAULT    6 _IO_sscanf
  1653: 00000000004121c0    62 FUNC    GLOBAL DEFAULT    6 __assert_fail
  1654: 000000000049d433     2 OBJECT  GLOBAL DEFAULT   10 _nl_C_name
  1655: 000000000042bb20    37 FUNC    GLOBAL DEFAULT    6 _IO_least_marker
  1656: 0000000000412530  2858 FUNC    GLOBAL DEFAULT    6 _nl_find_msg
  1657: 0000000000426d10    49 FUNC    GLOBAL DEFAULT    6 _IO_switch_to_wbackup_are
  1658: 000000000040fe30   151 FUNC    GLOBAL DEFAULT    6 syslog
  1659: 000000000042d460    32 FUNC    GLOBAL DEFAULT    6 _IO_list_resetlock
  1660: 00000000004769b0    34 FUNC    WEAK   DEFAULT    6 wcschrnul
  1661: 00000000004322d0   391 FUNC    GLOBAL DEFAULT    6 __memmove_sse2
  1662: 00000000006c6160    56 OBJECT  GLOBAL DEFAULT   27 _tmbuf
  1663: 0000000000426330   141 FUNC    WEAK   DEFAULT    6 __vsscanf
  1664: 0000000000479ba0     2 FUNC    GLOBAL DEFAULT    6 _dl_call_pltexit
  1665: 000000000048d350   143 FUNC    GLOBAL DEFAULT    6 __dlvsym
  1666: 000000000044e690    81 FUNC    WEAK   DEFAULT    6 llseek
  1667: 000000000044e690    81 FUNC    WEAK   DEFAULT    6 __lseek
  1668: 000000000049a790    18 OBJECT  GLOBAL DEFAULT   10 _nl_default_dirname
  1669: 000000000049a781     6 OBJECT  GLOBAL DEFAULT   10 _nl_POSIX_name
  1670: 000000000044e5e0    28 FUNC    GLOBAL DEFAULT    6 __twalk
  1671: 0000000000474b90   340 FUNC    GLOBAL DEFAULT    6 _IO_getline
  1672: 0000000000453fb0    43 FUNC    GLOBAL DEFAULT    6 _dl_allocate_static_tls
  1673: 0000000000440db0  6050 FUNC    GLOBAL DEFAULT    6 __strcpy_ssse3
  1674: 0000000000475740    93 FUNC    GLOBAL DEFAULT    6 fread_unlocked
  1675: 00000000006c2a20     8 OBJECT  GLOBAL DEFAULT   26 __x86_64_shared_cache_siz
  1676: 000000000042e090    60 IFUNC   GLOBAL DEFAULT    6 strcmp
  1677: 0000000000427070    40 FUNC    GLOBAL DEFAULT    6 _IO_wdefault_uflow
  1678: 0000000000463680   259 FUNC    GLOBAL DEFAULT    6 __mpn_rshift
  1679: 0000000000499e00    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_MEASUREMENT
  1680: 0000000000457e40     8 FUNC    GLOBAL DEFAULT    6 __gconv_get_alias_db
  1681: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND pthread_mutex_unlock
  1682: 0000000000457300   114 FUNC    GLOBAL HIDDEN     6 _dl_tlsdesc_resolve_hold
  1683: 00000000006c2060     0 NOTYPE  WEAK   DEFAULT   26 data_start
  1684: 0000000000461bc0  1158 FUNC    GLOBAL DEFAULT    6 _nl_find_locale
  1685: 0000000000431f80   836 FUNC    GLOBAL DEFAULT    6 __memchr
  1686: 0000000000407040    83 FUNC    GLOBAL DEFAULT    6 __malloc_check_init
  1687: 00000000006c61d0     8 OBJECT  GLOBAL DEFAULT   27 __fork_handlers
  1688: 0000000000476990    28 FUNC    GLOBAL DEFAULT    6 __mbsrtowcs
  1689: 0000000000422b50     5 FUNC    WEAK   DEFAULT    6 register_printf_function
  1690: 00000000006c6130     8 OBJECT  GLOBAL DEFAULT   27 __printf_function_table
  1691: 000000000044b820   276 FUNC    GLOBAL DEFAULT    6 _nl_get_walt_digit
  1692: 000000000041a030   709 FUNC    WEAK   DEFAULT    6 strtoul_l
  1693: 00000000004258c0    59 FUNC    GLOBAL DEFAULT    6 __fopen_maybe_mmap
  1694: 0000000000451b00   589 FUNC    GLOBAL DEFAULT    6 _dl_rtld_di_serinfo
  1695: 000000000044d040  1785 FUNC    WEAK   DEFAULT    6 getcwd
  1696: 00000000006c5fc0     8 OBJECT  GLOBAL DEFAULT   27 _dl_sysinfo_dso
  1697: 0000000000499d80    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_TELEPHONE
  1698: 0000000000410950    86 FUNC    GLOBAL HIDDEN     6 __libc_enable_asynccancel
  1699: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _dl_starting_up
  1700: 00000000004a28a0    76 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_alnu
  1701: 0000000000494400    33 FUNC    GLOBAL HIDDEN     6 __deregister_frame
  1702: 000000000042bef0   126 FUNC    GLOBAL DEFAULT    6 _IO_setb
  1703: 0000000000494c10   244 FUNC    GLOBAL DEFAULT    6 __dl_iterate_phdr
  1704: 00000000004966ac     0 FUNC    GLOBAL DEFAULT    9 _fini
  1705: 0000000000424870   197 FUNC    GLOBAL DEFAULT    6 __register_printf_type
  1706: 000000000042a670  2184 FUNC    WEAK   DEFAULT    6 _IO_file_fopen
  1707: 000000000040f009    20 FUNC    GLOBAL DEFAULT    6 __write_nocancel
  1708: 0000000000482730    37 FUNC    GLOBAL DEFAULT    6 __dladdr1
  1709: 00000000004443a0  1703 FUNC    GLOBAL DEFAULT    6 __stpcpy_sse2_unaligned
  1710: 00000000004080a0   614 FUNC    WEAK   DEFAULT    6 memalign
  1711: 000000000040acc0  1106 FUNC    GLOBAL DEFAULT    6 __mempcpy
  1712: 0000000000452490    49 FUNC    GLOBAL DEFAULT    6 _dl_unload_cache
  1713: 0000000000419410  1188 FUNC    GLOBAL DEFAULT    6 ____strtoll_l_internal
  1714: 0000000000424a10   143 FUNC    WEAK   DEFAULT    6 asprintf
  1715: 0000000000429df0    52 FUNC    GLOBAL DEFAULT    6 _IO_new_file_setbuf
  1716: 0000000000430440   421 FUNC    WEAK   DEFAULT    6 strerror_r
  1717: 0000000000427c80  1270 FUNC    GLOBAL DEFAULT    6 _IO_wfile_seekoff
  1718: 000000000047ad40    18 FUNC    WEAK   DEFAULT    6 strtof
  1719: 0000000000428180  1494 FUNC    GLOBAL DEFAULT    6 _IO_wfile_underflow
  1720: 000000000047fd30    10 FUNC    WEAK   DEFAULT    6 strtod_l
  1721: 0000000000474730   307 FUNC    GLOBAL DEFAULT    6 _IO_file_doallocate
  1722: 00000000004460a0   752 FUNC    GLOBAL DEFAULT    6 __wcsrtombs
  1723: 0000000000437bf0   294 FUNC    GLOBAL DEFAULT    6 _wordcopy_fwd_dest_aligne
  1724: 0000000000460890   143 FUNC    GLOBAL DEFAULT    6 __gconv_compare_alias_cac
  1725: 00000000004ae9f8     5 OBJECT  GLOBAL DEFAULT   10 _libc_intl_domainname
  1726: 0000000000482a80    60 IFUNC   WEAK   DEFAULT    6 strncasecmp_l
  1727: 00000000006c6250     8 OBJECT  GLOBAL DEFAULT   27 __gconv_path_elem
  1728: 00000000006c5f0c     4 OBJECT  GLOBAL DEFAULT   27 __libc_multiple_threads
  1729: 00000000004aeb60  3568 OBJECT  GLOBAL DEFAULT   10 __tens
  1730: 0000000000427a60   102 FUNC    GLOBAL DEFAULT    6 _IO_init_wmarker
  1731: 0000000000461300  2238 FUNC    GLOBAL DEFAULT    6 setlocale
  1732: 0000000000000048     8 TLS     GLOBAL DEFAULT   17 __libc_tsd_CTYPE_B
  1733: 000000000044e670    17 FUNC    GLOBAL DEFAULT    6 __getclktck
  1734: 00000000004924d0     8 FUNC    GLOBAL HIDDEN     6 _Unwind_GetTextRelBase
  1735: 000000000042a1e0    32 FUNC    GLOBAL DEFAULT    6 _IO_file_read
  1736: 00000000006c3288     8 OBJECT  GLOBAL DEFAULT   26 stderr
  1737: 00000000004100a0    23 FUNC    WEAK   DEFAULT    6 mmap64
  1738: 00000000004a29c0    68 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_blan
  1739: 000000000044cfa0    30 FUNC    WEAK   DEFAULT    6 __openat64_2
  1740: 0000000000410890    20 FUNC    GLOBAL DEFAULT    6 __socket
  1741: 00000000004796b0    69 FUNC    GLOBAL DEFAULT    6 __lxstat64
  1742: 000000000040fff0    18 FUNC    GLOBAL DEFAULT    6 setlogmask
  1743: 0000000000401500   548 FUNC    GLOBAL DEFAULT    6 __libc_setup_tls
  1744: 000000000049c520   168 OBJECT  GLOBAL DEFAULT   10 _IO_file_jumps
  1745: 0000000000424a10   143 FUNC    GLOBAL DEFAULT    6 ___asprintf
  1746: 000000000048b100   572 FUNC    WEAK   DEFAULT    6 profil
  1747: 0000000000475f00   133 FUNC    WEAK   DEFAULT    6 strsep
  1748: 0000000000407c10   227 FUNC    WEAK   DEFAULT    6 cfree
  1749: 0000000000485160    16 FUNC    GLOBAL DEFAULT    6 __strncasecmp_sse42
  1750: 000000000047d500    10 FUNC    WEAK   DEFAULT    6 __strtof_l
  1751: 0000000000462d30    36 FUNC    WEAK   DEFAULT    6 isnan
  1752: 000000000044c270   713 FUNC    GLOBAL DEFAULT    6 __libc_fork
  1753: 000000000044cfc9    20 FUNC    GLOBAL DEFAULT    6 __close_nocancel
  1754: 0000000000426330   141 FUNC    GLOBAL DEFAULT    6 _IO_vsscanf
  1755: 00000000006c2b68     8 OBJECT  GLOBAL DEFAULT   26 _dl_init_static_tls
  1756: 00000000004785b0  4053 FUNC    WEAK   DEFAULT    6 timelocal
  1757: 00000000006c53b0     8 OBJECT  GLOBAL DEFAULT   27 _dl_hwcap_mask
  1758: 0000000000442560  6147 FUNC    GLOBAL DEFAULT    6 __stpcpy_ssse3
  1759: 00000000006c5ef8     8 OBJECT  GLOBAL DEFAULT   27 __new_exitfn_called
  1760: 0000000000439580    60 FUNC    GLOBAL DEFAULT    6 __strend_sse4
  1761: 00000000006c4a80     4 OBJECT  GLOBAL DEFAULT   27 __fork_lock
  1762: 000000000040f060   236 FUNC    GLOBAL DEFAULT    6 __fcntl_nocancel
  1763: 0000000000492490    36 FUNC    GLOBAL HIDDEN     6 _Unwind_FindEnclosingFunc
  1764: 0000000000475f00   133 FUNC    GLOBAL DEFAULT    6 __strsep_g
  1765: 000000000042dcf0   174 FUNC    GLOBAL DEFAULT    6 _IO_str_init_static_inter
  1766: 0000000000495000    73 FUNC    GLOBAL DEFAULT    8 _nl_finddomain_subfreeres
  1767: 0000000000408350   677 FUNC    WEAK   DEFAULT    6 valloc
  1768: 000000000044e6f0   133 FUNC    GLOBAL DEFAULT    6 __wctrans
  1769: 00000000006c2b50     4 OBJECT  GLOBAL DEFAULT   26 _dl_stack_flags
  1770: 00000000004a1560    13 OBJECT  GLOBAL DEFAULT   10 _nl_category_name_sizes
  1771: 000000000044b770   166 FUNC    GLOBAL DEFAULT    6 _nl_get_alt_digit
  1772: 0000000000462d60    78 FUNC    WEAK   DEFAULT    6 isinfl
  1773: 000000000047a2b0   617 FUNC    GLOBAL DEFAULT    6 _dl_mcount
  1774: 000000000044e690    81 FUNC    GLOBAL DEFAULT    6 __libc_lseek
  1775: 00000000004561f0   216 FUNC    GLOBAL DEFAULT    6 _dl_next_tls_modid
  1776: 0000000000462db0    63 FUNC    WEAK   DEFAULT    6 isnanl
  1777: 00000000004246a0   204 FUNC    GLOBAL DEFAULT    6 __handle_registered_modif
  1778: 00000000004259f0   227 FUNC    WEAK   DEFAULT    6 _IO_fopen
  1779: 0000000000457230    34 FUNC    GLOBAL DEFAULT    6 _dl_mcount_wrapper_check
  1780: 0000000000426fd0   152 FUNC    GLOBAL DEFAULT    6 _IO_wdefault_finish
  1781: 00000000004298f0   145 FUNC    GLOBAL DEFAULT    6 _IO_new_file_write
  1782: 0000000000424870   197 FUNC    WEAK   DEFAULT    6 register_printf_type
  1783: 0000000000409200   130 FUNC    WEAK   DEFAULT    6 mallinfo
  1784: 00000000006c3288     8 OBJECT  GLOBAL HIDDEN    26 _IO_stderr
  1785: 0000000000412200    17 FUNC    GLOBAL DEFAULT    6 __ctype_b_loc
  1786: 0000000000410760    23 FUNC    GLOBAL DEFAULT    6 __mremap
  1787: 0000000000422ba0  6541 FUNC    GLOBAL DEFAULT    6 __printf_fphex
  1788: 0000000000410780    81 FUNC    WEAK   DEFAULT    6 __connect
  1789: 0000000000492470     8 FUNC    GLOBAL HIDDEN     6 _Unwind_GetLanguageSpecif
  1790: 0000000000475830    97 FUNC    GLOBAL DEFAULT    6 __strndup
  1791: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_NAME
  1792: 00000000006c5fc8     8 OBJECT  GLOBAL DEFAULT   27 _dl_init_all_dirs
  1793: 0000000000456640   377 FUNC    GLOBAL DEFAULT    6 _dl_allocate_tls
  1794: 0000000000446c40    13 FUNC    WEAK   DEFAULT    6 localtime_r
  1795: 00000000006c5ef0     8 OBJECT  GLOBAL DEFAULT   27 _dl_tls_static_nelem
  1796: 0000000000457e30     8 FUNC    GLOBAL DEFAULT    6 __gconv_get_modules_db
  1797: 0000000000449170  1665 FUNC    GLOBAL DEFAULT    6 __tzfile_compute
  1798: 000000000044c250    20 FUNC    GLOBAL DEFAULT    6 __uname
  1799: 0000000000427980    59 FUNC    GLOBAL DEFAULT    6 _IO_sputbackwc
  1800: 000000000044bf00    54 FUNC    GLOBAL DEFAULT    6 __opendirat
  1801: 0000000000459670  1951 FUNC    GLOBAL DEFAULT    6 __gconv_read_conf
  1802: 000000000047a640   102 FUNC    GLOBAL DEFAULT    6 __libc_dlclose
  1803: 000000000044e5e0    28 FUNC    WEAK   DEFAULT    6 twalk
  1804: 0000000000458d20   166 FUNC    GLOBAL DEFAULT    6 __gconv_close_transform
  1805: 0000000000456840   117 FUNC    GLOBAL DEFAULT    6 _dl_tls_get_addr_soft
  1806: 000000000042af00   148 FUNC    WEAK   DEFAULT    6 _IO_file_attach
  1807: 0000000000475f90   246 FUNC    WEAK   DEFAULT    6 argz_create_sep
  1808: 00000000006c56c0     8 OBJECT  GLOBAL DEFAULT   27 __timezone
  1809: 00000000004b1878     4 OBJECT  GLOBAL DEFAULT   10 _sys_nerr_internal
  1810: 0000000000499b00    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_NUMERIC
  1811: 0000000000445e50     9 FUNC    WEAK   DEFAULT    6 wmemmove
  1812: 0000000000427bf0   137 FUNC    GLOBAL DEFAULT    6 _IO_unsave_wmarkers
  1813: 000000000042a5a0   208 FUNC    GLOBAL DEFAULT    6 _IO_file_open
  1814: 0000000000451200  2297 FUNC    GLOBAL DEFAULT    6 _dl_map_object
  1815: 0000000000446390   827 FUNC    GLOBAL DEFAULT    6 __mbsnrtowcs
  1816: 0000000000495f20   495 FUNC    GLOBAL DEFAULT    8 _nl_archive_subfreeres
  1817: 0000000000000000     8 TLS     GLOBAL DEFAULT   16 __libc_tsd_LOCALE
  1818: 0000000000425e10   397 FUNC    WEAK   DEFAULT    6 fwrite
  1819: 000000000042d410    68 FUNC    GLOBAL DEFAULT    6 _IO_list_unlock
  1820: 000000000044cfc0    81 FUNC    WEAK   DEFAULT    6 __close
  1821: 000000000040eef0    69 FUNC    GLOBAL DEFAULT    6 __fxstat64
  1822: 0000000000463b70   281 FUNC    GLOBAL DEFAULT    6 __mpn_mul_1
  1823: 000000000044d020    20 FUNC    WEAK   DEFAULT    6 access
  1824: 000000000044c580     8 FUNC    GLOBAL DEFAULT    6 __getuid
  1825: 000000000049b080    36 OBJECT  GLOBAL DEFAULT   10 _itoa_upper_digits
  1826: 0000000000492770   230 FUNC    GLOBAL HIDDEN     6 _Unwind_ForcedUnwind
  1827: 00000000006c36f0     0 NOTYPE  GLOBAL DEFAULT  ABS _edata
  1828: 000000000044ce40    69 FUNC    GLOBAL DEFAULT    6 __xstat
  1829: 00000000006c2b20    40 OBJECT  GLOBAL DEFAULT   26 _dl_load_lock
  1830: 0000000000418310   811 FUNC    GLOBAL DEFAULT    6 qsort_r
  1831: 000000000042bbb0   106 FUNC    GLOBAL DEFAULT    6 _IO_switch_to_get_mode
  1832: 00000000006c6318     0 NOTYPE  GLOBAL DEFAULT  ABS _end
  1833: 0000000000479820   405 FUNC    GLOBAL DEFAULT    6 _dl_fixup
  1834: 00000000004651f0 38556 FUNC    GLOBAL DEFAULT    6 _IO_vfscanf
  1835: 000000000042afa0    35 FUNC    WEAK   DEFAULT    6 _IO_do_write
  1836: 000000000041a630   221 FUNC    GLOBAL DEFAULT    6 _fitoa_word
  1837: 0000000000479620   137 FUNC    GLOBAL DEFAULT    6 __fdopendir
  1838: 000000000047ad30    16 FUNC    GLOBAL DEFAULT    6 __strtof_internal
  1839: 00000000006c6260   104 OBJECT  GLOBAL DEFAULT   27 _nl_locale_file_list
  1840: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_COLLATE
  1841: 0000000000474870   711 FUNC    GLOBAL DEFAULT    6 _IO_getdelim
  1842: 0000000000482b20  9782 FUNC    GLOBAL DEFAULT    6 __GI___strncasecmp_l
  1843: 000000000046e890     7 FUNC    WEAK   DEFAULT    6 vfscanf
  1844: 000000000040eef0    69 FUNC    WEAK   DEFAULT    6 _fxstat
  1845: 0000000000460dc0    18 FUNC    GLOBAL DEFAULT    6 __gconv_release_cache
  1846: 00000000004193f0    18 FUNC    WEAK   DEFAULT    6 strtouq
  1847: 0000000000448060  3871 FUNC    GLOBAL DEFAULT    6 __tzfile_read
  1848: 00000000004254d0   527 FUNC    GLOBAL DEFAULT    6 __new_fclose
  1849: 0000000000427780   374 FUNC    GLOBAL DEFAULT    6 __wuflow
  1850: 000000000044c6d0  1817 FUNC    GLOBAL DEFAULT    6 __sysconf
  1851: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND pthread_mutex_lock
  1852: 00000000004630d0    31 FUNC    GLOBAL DEFAULT    6 __sigaction
  1853: 00000000004382d0    67 FUNC    GLOBAL DEFAULT    6 __argz_stringify
  1854: 00000000004088b0  1034 FUNC    GLOBAL DEFAULT    6 __libc_calloc
  1855: 0000000000488ae0    16 FUNC    GLOBAL DEFAULT    6 __strncasecmp_ssse3
  1856: 0000000000462d60    78 FUNC    GLOBAL DEFAULT    6 __isinfl
  1857: 00000000006c5830     8 OBJECT  GLOBAL DEFAULT   27 __curbrk
  1858: 0000000000458980   240 FUNC    GLOBAL DEFAULT    6 __gconv_compare_alias
  1859: 00000000004395c0     9 FUNC    GLOBAL DEFAULT    6 __memmove_chk_ssse3
  1860: 000000000046e9e0 21493 FUNC    GLOBAL DEFAULT    6 __vfwprintf
  1861: 000000000044e150    72 FUNC    GLOBAL DEFAULT    6 __tfind
  1862: 00000000006c3560   232 OBJECT  GLOBAL DEFAULT   26 _nl_global_locale
  1863: 000000000042c7d0     8 FUNC    GLOBAL DEFAULT    6 _IO_default_seekoff
  1864: 00000000006c5fd0     4 OBJECT  GLOBAL DEFAULT   27 _dl_verbose
  1865: 0000000000456070   148 FUNC    GLOBAL DEFAULT    6 _dl_dprintf
  1866: 0000000000482a80    60 IFUNC   GLOBAL DEFAULT    6 __strncasecmp_l
  1867: 000000000042bf70   172 FUNC    GLOBAL DEFAULT    6 _IO_doallocbuf
  1868: 0000000000455520   329 FUNC    GLOBAL DEFAULT    6 _dl_signal_error
  1869: 00000000006c5fe0     8 OBJECT  GLOBAL DEFAULT   27 _dl_phnum
  1870: 000000000042cd50   532 FUNC    WEAK   DEFAULT    6 _flushlbf
  1871: 00000000006c1f80     4 OBJECT  GLOBAL DEFAULT   23 __stack_prot
  1872: 000000000049aee0    35 OBJECT  GLOBAL DEFAULT   10 __strtol_ul_rem_tab
  1873: 00000000004b1d60   208 OBJECT  GLOBAL DEFAULT   10 __libio_codecvt
  1874: 000000000044c080    45 FUNC    GLOBAL DEFAULT    6 __closedir
  1875: 00000000004020e0  1077 FUNC    GLOBAL DEFAULT    6 __libc_message
  1876: 00000000004103e0   603 FUNC    WEAK   DEFAULT    6 get_nprocs
  1877: 00000000006c5fe8     8 OBJECT  GLOBAL DEFAULT   27 _dl_profile_map
  1878: 000000000042bb80    43 FUNC    GLOBAL DEFAULT    6 _IO_switch_to_backup_area
  1879: 0000000000482320   402 FUNC    GLOBAL DEFAULT    6 __dlerror
  1880: 0000000000401c90    19 FUNC    GLOBAL DEFAULT    6 exit
  1881: 00000000004923e0    80 FUNC    GLOBAL HIDDEN     6 _Unwind_SetGR
  1882: 00000000006c4850     8 OBJECT  WEAK   DEFAULT   27 __free_hook
  1883: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _nl_current_LC_ADDRESS_us
  1884: 000000000045aa40  1002 FUNC    GLOBAL DEFAULT    6 __gconv_transform_interna
  1885: 0000000000419bb0  1151 FUNC    GLOBAL DEFAULT    6 ____strtoull_l_internal
  1886: 00000000004100c0    20 FUNC    GLOBAL DEFAULT    6 __munmap
  1887: 000000000044d7c0   151 FUNC    GLOBAL DEFAULT    6 __writev
  1888: 0000000000000038     8 TLS     GLOBAL DEFAULT   17 __libc_tsd_CTYPE_TOUPPER
  1889: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __pthread_setspecific
  1890: 0000000000409000    52 FUNC    GLOBAL DEFAULT    6 __malloc_usable_size
  1891: 000000000045fd60  1076 FUNC    GLOBAL DEFAULT    6 __gconv_transliterate
  1892: 0000000000432610    79 IFUNC   GLOBAL DEFAULT    6 __strcasecmp
  1893: 000000000044cec0   214 FUNC    GLOBAL DEFAULT    6 __openat
  1894: 0000000000445210   789 FUNC    GLOBAL DEFAULT    6 __strchr_sse2_no_bsf
  1895: 00000000004b1440  1080 OBJECT  GLOBAL DEFAULT   10 _sys_errlist_internal
  1896: 000000000040eef0    69 FUNC    GLOBAL DEFAULT    6 __fxstat
  1897: 000000000041a030   709 FUNC    WEAK   DEFAULT    6 __strtoul_l
  1898: 00000000006c3298     8 OBJECT  GLOBAL HIDDEN    26 _IO_stdin
  1899: 000000000049c1e0   168 OBJECT  GLOBAL DEFAULT   10 _IO_wfile_jumps_mmap
  1900: 0000000000426d50   131 FUNC    GLOBAL DEFAULT    6 _IO_wsetb
  1901: 0000000000424980   143 FUNC    GLOBAL DEFAULT    6 __fprintf
  1902: 000000000044d760    86 FUNC    WEAK   DEFAULT    6 brk
  1903: 0000000000446f80   162 FUNC    GLOBAL DEFAULT    6 __tzstring
  1904: 00000000004998c0    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_MESSAGES
  1905: 000000000041aa00 21919 FUNC    GLOBAL DEFAULT    6 _IO_vfprintf
  1906: 0000000000446b20   276 FUNC    GLOBAL DEFAULT    6 __wcsmbs_named_conv
  1907: 00000000006c2a40     8 OBJECT  GLOBAL DEFAULT   26 __x86_64_raw_data_cache_s
  1908: 00000000004261e0   247 FUNC    GLOBAL DEFAULT    6 _IO_seekoff
  1909: 00000000004112d0   465 FUNC    GLOBAL DEFAULT    6 _dl_aux_init
  1910: 00000000006c53c0     8 OBJECT  GLOBAL DEFAULT   27 _dl_hwcap
  1911: 00000000004757a0   139 FUNC    GLOBAL DEFAULT    6 _IO_wfile_doallocate
  1912: 00000000004af960   144 OBJECT  GLOBAL DEFAULT   10 _itowa_upper_digits
  1913: 0000000000412070   321 FUNC    GLOBAL DEFAULT    6 __assert_fail_base
  1914: 000000000043ec60    16 FUNC    GLOBAL DEFAULT    6 __strcasecmp_ssse3
  1915: 00000000006c61b0     4 OBJECT  GLOBAL DEFAULT   27 __use_tzfile
  1916: 00000000004a1580   137 OBJECT  GLOBAL DEFAULT   10 _nl_category_names
  1917: 000000000044cec0   214 FUNC    WEAK   DEFAULT    6 openat
  1918: 000000000047ab20    68 FUNC    GLOBAL DEFAULT    6 _dl_tlsdesc_resolve_hold_
  1919: 00000000004ae9fd    15 OBJECT  GLOBAL DEFAULT   10 _nl_C_codeset
  1920: 00000000006c5ff0     8 OBJECT  GLOBAL DEFAULT   27 _dl_initfirst
  1921: 0000000000475440    36 FUNC    GLOBAL DEFAULT    6 fileno
  1922: 0000000000417870    30 FUNC    GLOBAL DEFAULT    6 __setfpucw
  1923: 000000000040fed0    16 FUNC    GLOBAL DEFAULT    6 vsyslog
  1924: 000000000042d6f0    76 FUNC    GLOBAL DEFAULT    6 _IO_str_underflow
  1925: 0000000000417920    43 FUNC    GLOBAL DEFAULT    6 __sigprocmask
  1926: 0000000000401b80     7 FUNC    GLOBAL DEFAULT    6 _setjmp
  1927: 0000000000426840   169 FUNC    GLOBAL DEFAULT    6 fgets_unlocked
  1928: 0000000000412220    17 FUNC    GLOBAL DEFAULT    6 __ctype_toupper_loc
  1929: 0000000000424bc0    50 FUNC    GLOBAL DEFAULT    6 __funlockfile
  1930: 0000000000438320  4697 FUNC    GLOBAL DEFAULT    6 __strcmp_ssse3
  1931: 00000000004966c0     4 OBJECT  GLOBAL DEFAULT   10 _IO_stdin_used
  1932: 000000000040ee80    80 FUNC    GLOBAL DEFAULT    6 _exit
  1933: 00000000006c2ae0    40 OBJECT  GLOBAL DEFAULT   26 _dl_load_write_lock
  1934: 0000000000457270     5 FUNC    GLOBAL HIDDEN     6 _dl_tlsdesc_return
  1935: 00000000004070a0  1363 FUNC    GLOBAL DEFAULT    6 __malloc_set_state
  1936: 000000000044bf80   255 FUNC    GLOBAL DEFAULT    6 __alloc_dir
  1937: 00000000004348a0    16 FUNC    GLOBAL DEFAULT    6 __strcasecmp_sse42
  1938: 0000000000436450    16 FUNC    GLOBAL DEFAULT    6 __strcasecmp_avx
  1939: 000000000044c1c0   131 FUNC    WEAK   DEFAULT    6 __getdents64
  1940: 0000000000492380    72 FUNC    GLOBAL HIDDEN     6 _Unwind_GetGR
  1941: 000000000049a787     9 OBJECT  GLOBAL DEFAULT   10 _nl_default_default_domai
  1942: 00000000006c60c0     8 OBJECT  GLOBAL DEFAULT   27 __libc_argv
  1943: 0000000000444a50  1972 FUNC    GLOBAL DEFAULT    6 __strnlen_sse2_no_bsf
  1944: 0000000000401210   590 FUNC    GLOBAL DEFAULT    6 __libc_start_main
  1945: 0000000000410900    39 FUNC    GLOBAL HIDDEN     6 __lll_lock_wait_private
  1946: 0000000000409cd0   221 FUNC    GLOBAL DEFAULT    6 strlen
  1947: 000000000044e690    81 FUNC    WEAK   DEFAULT    6 lseek64
  1948: 00000000006c2a30     8 OBJECT  GLOBAL DEFAULT   26 __x86_64_shared_cache_siz
  1949: 000000000040ef40    81 FUNC    WEAK   DEFAULT    6 open
  1950: 00000000006c2ac0     8 OBJECT  WEAK   DEFAULT   26 program_invocation_name
  1951: 000000000047a6b0   130 FUNC    GLOBAL DEFAULT    6 __libc_dlsym
  1952: 000000000048b830   276 FUNC    GLOBAL DEFAULT    6 _dl_show_scope
  1953: 000000000040f000    81 FUNC    GLOBAL DEFAULT    6 __libc_write
  1954: 000000000046e890     7 FUNC    GLOBAL DEFAULT    6 __vfscanf
  1955: 000000000040f150   354 FUNC    WEAK   DEFAULT    6 __fcntl
  1956: 00000000004266e0   324 FUNC    WEAK   DEFAULT    6 vdprintf
  1957: 000000000042c4b0   183 FUNC    GLOBAL DEFAULT    6 _IO_init
  1958: 000000000045f360  2341 FUNC    GLOBAL DEFAULT    6 __gconv_transform_interna
  1959: 000000000044c270   713 FUNC    WEAK   DEFAULT    6 __fork
  1960: 00000000004a2c60    72 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_lowe
  1961: 00000000006c5ff8     8 OBJECT  GLOBAL DEFAULT   27 _dl_all_dirs
  1962: 0000000000418fe0   126 FUNC    GLOBAL DEFAULT    6 __setenv
  1963: 0000000000419180   151 FUNC    GLOBAL DEFAULT    6 __clearenv
  1964: 000000000042df20    61 IFUNC   GLOBAL DEFAULT    6 strchr
  1965: 00000000004568c0   208 FUNC    GLOBAL DEFAULT    6 _dl_add_to_slotinfo
  1966: 0000000000407d00   844 FUNC    GLOBAL DEFAULT    6 __realloc
  1967: 00000000006c6220     8 OBJECT  GLOBAL DEFAULT   27 __gconv_alias_db
  1968: 000000000042d390     3 FUNC    GLOBAL DEFAULT    6 _IO_iter_end
  1969: 0000000000409290     5 FUNC    GLOBAL DEFAULT    6 __mallopt
  1970: 0000000000425ae0   334 FUNC    WEAK   DEFAULT    6 fputs
  1971: 0000000000464a40  1966 FUNC    GLOBAL DEFAULT    6 _quicksort
  1972: 00000000004924c0     8 FUNC    GLOBAL HIDDEN     6 _Unwind_GetDataRelBase
  1973: 000000000042afd0   568 FUNC    GLOBAL DEFAULT    6 _IO_new_file_underflow
  1974: 00000000006c2060     0 NOTYPE  GLOBAL DEFAULT   26 __data_start
  1975: 00000000004825c0   305 FUNC    GLOBAL DEFAULT    6 _dlerror_run
  1976: 00000000004079c0   591 FUNC    GLOBAL DEFAULT    6 __malloc_get_state
  1977: 0000000000490150    13 FUNC    GLOBAL DEFAULT    6 _dl_sym
  1978: 0000000000402520    30 FUNC    GLOBAL DEFAULT    6 __libc_fatal
  1979: 00000000004106e0    10 FUNC    GLOBAL DEFAULT    6 __get_phys_pages
  1980: 000000000040f2c0   161 FUNC    GLOBAL DEFAULT    6 __sbrk
  1981: 00000000004100e0    20 FUNC    WEAK   DEFAULT    6 mprotect
  1982: 000000000042d320     8 FUNC    GLOBAL DEFAULT    6 _IO_default_seek
  1983: 000000000044e1a0  1088 FUNC    GLOBAL DEFAULT    6 __tdelete
  1984: 000000000044d020    20 FUNC    GLOBAL DEFAULT    6 __access
  1985: 00000000006c61e0    40 OBJECT  GLOBAL DEFAULT   27 _r_debug
  1986: 00000000006c62f0     8 OBJECT  GLOBAL DEFAULT   28 __printf_va_arg_table
  1987: 0000000000409040   434 FUNC    GLOBAL DEFAULT    6 __malloc_stats
  1988: 000000000044c080    45 FUNC    WEAK   DEFAULT    6 closedir
  1989: 0000000000426de0   485 FUNC    GLOBAL DEFAULT    6 _IO_wdefault_pbackfail
  1990: 00000000004b1440  1080 OBJECT  GLOBAL DEFAULT   10 __sys_errlist_internal
  1991: 00000000006c6000     4 OBJECT  GLOBAL DEFAULT   27 _dl_osversion
  1992: 00000000006c2bc0     8 OBJECT  GLOBAL DEFAULT   26 _IO_list_all
  1993: 0000000000430620   109 FUNC    GLOBAL DEFAULT    6 __GI_strnlen
  1994: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _Jv_RegisterClasses
  1995: 0000000000476090   206 FUNC    GLOBAL DEFAULT    6 __argz_add_sep
  1996: 000000000042b210   546 FUNC    GLOBAL DEFAULT    6 _IO_new_file_overflow
  1997: 000000000047a740   282 FUNC    GLOBAL DEFAULT    6 __libc_dlopen_mode
  1998: 0000000000419060   286 FUNC    GLOBAL DEFAULT    6 __unsetenv
  1999: 00000000004299e0  1039 FUNC    GLOBAL DEFAULT    6 _IO_new_file_seekoff
  2000: 0000000000477570  4157 FUNC    GLOBAL DEFAULT    6 __mktime_internal
  2001: 0000000000426550   395 FUNC    WEAK   DEFAULT    6 vasprintf
  2002: 00000000006c2a50     8 OBJECT  GLOBAL DEFAULT   26 __x86_64_raw_data_cache_s
  2003: 000000000046e890     7 FUNC    GLOBAL DEFAULT    6 ___vfscanf
  2004: 0000000000455e60   172 FUNC    GLOBAL DEFAULT    6 _dl_sysdep_read_whole_fil
  2005: 000000000040b590   111 FUNC    WEAK   DEFAULT    6 strchrnul
  2006: 0000000000000010     8 TLS     GLOBAL DEFAULT   16 _nl_current_LC_MONETARY
  2007: 000000000044ce90    43 FUNC    GLOBAL DEFAULT    6 __openat_nocancel
  2008: 000000000040f150   354 FUNC    WEAK   DEFAULT    6 fcntl
  2009: 0000000000447cd0   138 FUNC    WEAK   DEFAULT    6 tzset
  2010: 000000000044cdf0    20 FUNC    WEAK   DEFAULT    6 sched_yield
  2011: 0000000000411800   868 FUNC    GLOBAL DEFAULT    6 _dl_addr
  2012: 00000000004106f0    10 FUNC    GLOBAL DEFAULT    6 __get_avphys_pages
  2013: 0000000000424770   193 FUNC    GLOBAL DEFAULT    6 __handle_registered_modif
  2014: 000000000040ef40    81 FUNC    WEAK   DEFAULT    6 __open64
  2015: 0000000000499b80    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_PAPER
  2016: 0000000000455710   226 FUNC    GLOBAL DEFAULT    6 _dl_catch_error
  2017: 000000000042b6f0   551 FUNC    GLOBAL DEFAULT    6 _IO_un_link
  2018: 00000000004942c0     9 FUNC    GLOBAL HIDDEN     6 __register_frame_info_tab
  2019: 000000000042a200   113 FUNC    GLOBAL DEFAULT    6 _IO_file_setbuf_mmap
  2020: 00000000006c2b48     8 OBJECT  GLOBAL DEFAULT   26 _dl_make_stack_executable
  2021: 00000000006c6008     8 OBJECT  GLOBAL DEFAULT   27 _dl_inhibit_rpath
  2022: 0000000000410640   156 FUNC    WEAK   DEFAULT    6 get_nprocs_conf
  2023: 000000000042d170   417 FUNC    GLOBAL DEFAULT    6 _IO_default_pbackfail
  2024: 0000000000457280    14 FUNC    GLOBAL HIDDEN     6 _dl_tlsdesc_undefweak
  2025: 00000000004092a0   118 FUNC    WEAK   DEFAULT    6 posix_memalign
  2026: 0000000000494200     9 FUNC    GLOBAL HIDDEN     6 __register_frame_info
  2027: 00000000004266e0   324 FUNC    GLOBAL DEFAULT    6 _IO_vdprintf
  2028: 0000000000445e70   548 FUNC    WEAK   DEFAULT    6 wcrtomb
  2029: 0000000000488af0  9542 FUNC    GLOBAL DEFAULT    6 __strncasecmp_l_ssse3
  2030: 00000000006c2b54     4 OBJECT  GLOBAL DEFAULT   26 _dl_correct_cache_id
  2031: 000000000048ef60   574 FUNC    GLOBAL DEFAULT    6 _dl_sort_fini
  2032: 000000000043c150 11014 FUNC    GLOBAL DEFAULT    6 __memmove_ssse3_back
  2033: 00000000004259f0   227 FUNC    GLOBAL DEFAULT    6 __new_fopen
  2034: 000000000044cfc0    81 FUNC    WEAK   DEFAULT    6 close
  2035: 0000000000410780    81 FUNC    GLOBAL DEFAULT    6 __libc_connect
  2036: 0000000000445dd0     9 FUNC    GLOBAL DEFAULT    6 __wmemcpy
  2037: 000000000042d3a0     5 FUNC    GLOBAL DEFAULT    6 _IO_iter_next
  2038: 000000000048c3c0  3257 FUNC    GLOBAL DEFAULT    6 _dl_close_worker
  2039: 00000000006c2b60     8 OBJECT  GLOBAL DEFAULT   26 _dl_pagesize
  2040: 0000000000408350   677 FUNC    GLOBAL DEFAULT    6 __valloc
  2041: 00000000006c20b0     8 OBJECT  WEAK   DEFAULT   26 __memalign_hook
  2042: 0000000000000000     0 TLS     WEAK   DEFAULT  UND _nl_current_LC_IDENTIFICA
  2043: 000000000044c590     8 FUNC    GLOBAL DEFAULT    6 __geteuid
  2044: 0000000000437d20   360 FUNC    GLOBAL DEFAULT    6 _wordcopy_bwd_aligned
  2045: 000000000041aa00 21919 FUNC    GLOBAL DEFAULT    6 vfprintf
  2046: 00000000006c2be0   224 OBJECT  GLOBAL DEFAULT   26 _IO_2_1_stderr_
  2047: 00000000006c2ac0     8 OBJECT  GLOBAL DEFAULT   26 __progname_full
  2048: 00000000004829e0   156 FUNC    GLOBAL DEFAULT    6 strpbrk
  2049: 000000000042bb50    43 FUNC    GLOBAL DEFAULT    6 _IO_switch_to_main_get_ar
  2050: 0000000000410930    28 FUNC    GLOBAL HIDDEN     6 __lll_unlock_wake_private
  2051: 0000000000462e40   100 FUNC    GLOBAL DEFAULT    6 raise
  2052: 000000000042d050   167 FUNC    GLOBAL DEFAULT    6 _IO_seekmark
  2053: 00000000004a2c00    72 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_CTYPE_class_alph
  2054: 0000000000407c10   227 FUNC    GLOBAL DEFAULT    6 free
  2055: 000000000044e780    66 FUNC    GLOBAL DEFAULT    6 __towctrans
  2056: 000000000044bc50   365 FUNC    GLOBAL DEFAULT    6 _nl_get_era_entry
  2057: 0000000000417920    43 FUNC    WEAK   DEFAULT    6 sigprocmask
  2058: 000000000042c570   163 FUNC    GLOBAL DEFAULT    6 _IO_old_init
  2059: 000000000049c460   168 OBJECT  GLOBAL DEFAULT   10 _IO_file_jumps_mmap
  2060: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
  2061: 0000000000482700    31 FUNC    GLOBAL DEFAULT    6 __libc_register_dlfcn_hoo
  2062: 000000000048d8e0  4587 FUNC    GLOBAL DEFAULT    6 _dl_map_object_deps
  2063: 000000000049a000    64 OBJECT  GLOBAL DEFAULT   10 _nl_C_LC_IDENTIFICATION
  2064: 00000000006c6020   144 OBJECT  GLOBAL DEFAULT   27 _dl_ns
  2065: 0000000000462720  1319 FUNC    GLOBAL DEFAULT    6 _nl_load_locale_from_arch
  2066: 000000000044e6f0   133 FUNC    WEAK   DEFAULT    6 wctrans
  2067: 00000000004259f0   227 FUNC    WEAK   DEFAULT    6 fopen64
  2068: 000000000040ee20    87 FUNC    GLOBAL DEFAULT    6 __cache_sysconf
No version information found in this file.
Notes at offset 0x00000190 with length 0x00000020:
  Owner                 Data sizeDescription
  GNU                  0x00000010NT_GNU_ABI_TAG (ABI version tag)
    OS: Linux, ABI: 2.6.24
Notes at offset 0x000001b0 with length 0x00000024:
  Owner                 Data sizeDescription
  GNU                  0x00000014NT_GNU_BUILD_ID (unique build ID bitstring)
    Build ID: 5a4d2bc504ef8f7f5335d016b53e282eb6bb1c21
目前为止我们已经查看了可重定位目标文件和可执行目标文件的内部内容。
下面我们来看下共享目标文件的内容。
使用命令：gcc -shared -fPIC -o libvector.so addvec.c
可以从addvec.c得到一个共享目标文件libvector.so.
参数shared指示连接器创建一个共享的目标文件，fPIC指示编译器生成与位置无关的代码。
使用命令readelf libvector.so -a:
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x4a0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          4384 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         7
  Size of section headers:           64 (bytes)
  Number of section headers:         27
  Section header string table index: 24
Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .note.gnu.build-i NOTE             00000000000001c8  000001c8
       0000000000000024  0000000000000000   A       0     0     4
  [ 2] .gnu.hash         GNU_HASH         00000000000001f0  000001f0
       000000000000003c  0000000000000000   A       3     0     8
  [ 3] .dynsym           DYNSYM           0000000000000230  00000230
       0000000000000108  0000000000000018   A       4     2     8
  [ 4] .dynstr           STRTAB           0000000000000338  00000338
       0000000000000074  0000000000000000   A       0     0     1
  [ 5] .gnu.version      VERSYM           00000000000003ac  000003ac
       0000000000000016  0000000000000002   A       3     0     2
  [ 6] .gnu.version_r    VERNEED          00000000000003c8  000003c8
       0000000000000020  0000000000000000   A       4     1     8
  [ 7] .rela.dyn         RELA             00000000000003e8  000003e8
       0000000000000060  0000000000000018   A       3     0     8
  [ 8] .rela.plt         RELA             0000000000000448  00000448
       0000000000000018  0000000000000018   A       3    10     8
  [ 9] .init             PROGBITS         0000000000000460  00000460
       0000000000000018  0000000000000000  AX       0     0     4
  [10] .plt              PROGBITS         0000000000000480  00000480
       0000000000000020  0000000000000010  AX       0     0     16
  [11] .text             PROGBITS         00000000000004a0  000004a0
       0000000000000168  0000000000000000  AX       0     0     16
  [12] .fini             PROGBITS         0000000000000608  00000608
       000000000000000e  0000000000000000  AX       0     0     4
  [13] .eh_frame_hdr     PROGBITS         0000000000000618  00000618
       000000000000001c  0000000000000000   A       0     0     4
  [14] .eh_frame         PROGBITS         0000000000000638  00000638
       0000000000000064  0000000000000000   A       0     0     8
  [15] .ctors            PROGBITS         0000000000200e28  00000e28
       0000000000000010  0000000000000000  WA       0     0     8
  [16] .dtors            PROGBITS         0000000000200e38  00000e38
       0000000000000010  0000000000000000  WA       0     0     8
  [17] .jcr              PROGBITS         0000000000200e48  00000e48
       0000000000000008  0000000000000000  WA       0     0     8
  [18] .dynamic          DYNAMIC          0000000000200e50  00000e50
       0000000000000180  0000000000000010  WA       4     0     8
  [19] .got              PROGBITS         0000000000200fd0  00000fd0
       0000000000000018  0000000000000008  WA       0     0     8
  [20] .got.plt          PROGBITS         0000000000200fe8  00000fe8
       0000000000000020  0000000000000008  WA       0     0     8
  [21] .data             PROGBITS         0000000000201008  00001008
       0000000000000008  0000000000000000  WA       0     0     8
  [22] .bss              NOBITS           0000000000201010  00001010
       0000000000000010  0000000000000000  WA       0     0     8
  [23] .comment          PROGBITS         0000000000000000  00001010
       000000000000002a  0000000000000001  MS       0     0     1
  [24] .shstrtab         STRTAB           0000000000000000  0000103a
       00000000000000e0  0000000000000000           0     0     1
  [25] .symtab           SYMTAB           0000000000000000  000017e0
       00000000000004e0  0000000000000018          26    43     8
  [26] .strtab           STRTAB           0000000000000000  00001cc0
       000000000000016d  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
There are no section groups in this file.
Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x000000000000069c 0x000000000000069c  R E    200000
  LOAD           0x0000000000000e28 0x0000000000200e28 0x0000000000200e28
                 0x00000000000001e8 0x00000000000001f8  RW     200000
  DYNAMIC        0x0000000000000e50 0x0000000000200e50 0x0000000000200e50
                 0x0000000000000180 0x0000000000000180  RW     8
  NOTE           0x00000000000001c8 0x00000000000001c8 0x00000000000001c8
                 0x0000000000000024 0x0000000000000024  R      4
  GNU_EH_FRAME   0x0000000000000618 0x0000000000000618 0x0000000000000618
                 0x000000000000001c 0x000000000000001c  R      4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     8
  GNU_RELRO      0x0000000000000e28 0x0000000000200e28 0x0000000000200e28
                 0x00000000000001d8 0x00000000000001d8  R      1
 Section to Segment mapping:
  Segment Sections...
   00     .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .plt .text .fini .eh_frame_hdr .eh_frame 
   01     .ctors .dtors .jcr .dynamic .got .got.plt .data .bss 
   02     .dynamic 
   03     .note.gnu.build-id 
   04     .eh_frame_hdr 
   05     
   06     .ctors .dtors .jcr .dynamic .got 
Dynamic section at offset 0xe50 contains 20 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000000c (INIT)               0x460
 0x000000000000000d (FINI)               0x608
 0x000000006ffffef5 (GNU_HASH)           0x1f0
 0x0000000000000005 (STRTAB)             0x338
 0x0000000000000006 (SYMTAB)             0x230
 0x000000000000000a (STRSZ)              116 (bytes)
 0x000000000000000b (SYMENT)             24 (bytes)
 0x0000000000000003 (PLTGOT)             0x200fe8
 0x0000000000000002 (PLTRELSZ)           24 (bytes)
 0x0000000000000014 (PLTREL)             RELA
 0x0000000000000017 (JMPREL)             0x448
 0x0000000000000007 (RELA)               0x3e8
 0x0000000000000008 (RELASZ)             96 (bytes)
 0x0000000000000009 (RELAENT)            24 (bytes)
 0x000000006ffffffe (VERNEED)            0x3c8
 0x000000006fffffff (VERNEEDNUM)         1
 0x000000006ffffff0 (VERSYM)             0x3ac
 0x000000006ffffff9 (RELACOUNT)          1
 0x0000000000000000 (NULL)               0x0
Relocation section '.rela.dyn' at offset 0x3e8 contains 4 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000201008  000000000008 R_X86_64_RELATIVE                    0000000000201008
000000200fd0  000200000006 R_X86_64_GLOB_DAT 0000000000000000 __gmon_start__ + 0
000000200fd8  000300000006 R_X86_64_GLOB_DAT 0000000000000000 _Jv_RegisterClasses + 0
000000200fe0  000400000006 R_X86_64_GLOB_DAT 0000000000000000 __cxa_finalize + 0
Relocation section '.rela.plt' at offset 0x448 contains 1 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000201000  000400000007 R_X86_64_JUMP_SLO 0000000000000000 __cxa_finalize + 0
There are no unwind sections in this file.
Symbol table '.dynsym' contains 11 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000460     0 SECTION LOCAL  DEFAULT    9 
     2: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     3: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _Jv_RegisterClasses
     4: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@GLIBC_2.2.5 (2)
     5: 0000000000201010     0 NOTYPE  GLOBAL DEFAULT  ABS _edata
     6: 0000000000201020     0 NOTYPE  GLOBAL DEFAULT  ABS _end
     7: 0000000000201010     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_start
     8: 0000000000000460     0 FUNC    GLOBAL DEFAULT    9 _init
     9: 0000000000000608     0 FUNC    GLOBAL DEFAULT   12 _fini
    10: 000000000000056c    91 FUNC    GLOBAL DEFAULT   11 addvec
Symbol table '.symtab' contains 52 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 00000000000001c8     0 SECTION LOCAL  DEFAULT    1 
     2: 00000000000001f0     0 SECTION LOCAL  DEFAULT    2 
     3: 0000000000000230     0 SECTION LOCAL  DEFAULT    3 
     4: 0000000000000338     0 SECTION LOCAL  DEFAULT    4 
     5: 00000000000003ac     0 SECTION LOCAL  DEFAULT    5 
     6: 00000000000003c8     0 SECTION LOCAL  DEFAULT    6 
     7: 00000000000003e8     0 SECTION LOCAL  DEFAULT    7 
     8: 0000000000000448     0 SECTION LOCAL  DEFAULT    8 
     9: 0000000000000460     0 SECTION LOCAL  DEFAULT    9 
    10: 0000000000000480     0 SECTION LOCAL  DEFAULT   10 
    11: 00000000000004a0     0 SECTION LOCAL  DEFAULT   11 
    12: 0000000000000608     0 SECTION LOCAL  DEFAULT   12 
    13: 0000000000000618     0 SECTION LOCAL  DEFAULT   13 
    14: 0000000000000638     0 SECTION LOCAL  DEFAULT   14 
    15: 0000000000200e28     0 SECTION LOCAL  DEFAULT   15 
    16: 0000000000200e38     0 SECTION LOCAL  DEFAULT   16 
    17: 0000000000200e48     0 SECTION LOCAL  DEFAULT   17 
    18: 0000000000200e50     0 SECTION LOCAL  DEFAULT   18 
    19: 0000000000200fd0     0 SECTION LOCAL  DEFAULT   19 
    20: 0000000000200fe8     0 SECTION LOCAL  DEFAULT   20 
    21: 0000000000201008     0 SECTION LOCAL  DEFAULT   21 
    22: 0000000000201010     0 SECTION LOCAL  DEFAULT   22 
    23: 0000000000000000     0 SECTION LOCAL  DEFAULT   23 
    24: 00000000000004a0     0 FUNC    LOCAL  DEFAULT   11 call_gmon_start
    25: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    26: 0000000000200e28     0 OBJECT  LOCAL  DEFAULT   15 __CTOR_LIST__
    27: 0000000000200e38     0 OBJECT  LOCAL  DEFAULT   16 __DTOR_LIST__
    28: 0000000000200e48     0 OBJECT  LOCAL  DEFAULT   17 __JCR_LIST__
    29: 00000000000004c0     0 FUNC    LOCAL  DEFAULT   11 __do_global_dtors_aux
    30: 0000000000201010     1 OBJECT  LOCAL  DEFAULT   22 completed.6531
    31: 0000000000201018     8 OBJECT  LOCAL  DEFAULT   22 dtor_idx.6533
    32: 0000000000000540     0 FUNC    LOCAL  DEFAULT   11 frame_dummy
    33: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    34: 0000000000200e30     0 OBJECT  LOCAL  DEFAULT   15 __CTOR_END__
    35: 0000000000000698     0 OBJECT  LOCAL  DEFAULT   14 __FRAME_END__
    36: 0000000000200e48     0 OBJECT  LOCAL  DEFAULT   17 __JCR_END__
    37: 00000000000005d0     0 FUNC    LOCAL  DEFAULT   11 __do_global_ctors_aux
    38: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS addvec.c
    39: 0000000000200e40     0 OBJECT  LOCAL  DEFAULT   16 __DTOR_END__
    40: 0000000000201008     0 OBJECT  LOCAL  DEFAULT   21 __dso_handle
    41: 0000000000200e50     0 OBJECT  LOCAL  DEFAULT  ABS _DYNAMIC
    42: 0000000000200fe8     0 OBJECT  LOCAL  DEFAULT  ABS _GLOBAL_OFFSET_TABLE_
    43: 0000000000201010     0 NOTYPE  GLOBAL DEFAULT  ABS _edata
    44: 0000000000000608     0 FUNC    GLOBAL DEFAULT   12 _fini
    45: 000000000000056c    91 FUNC    GLOBAL DEFAULT   11 addvec
    46: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
    47: 0000000000201020     0 NOTYPE  GLOBAL DEFAULT  ABS _end
    48: 0000000000201010     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_start
    49: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _Jv_RegisterClasses
    50: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@@GLIBC_2.2
    51: 0000000000000460     0 FUNC    GLOBAL DEFAULT    9 _init
Histogram for `.gnu.hash' bucket list length (total of 3 buckets):
 Length  Number     % of total  Coverage
      0  0          (  0.0%)
      1  0          (  0.0%)      0.0%
      2  3          (100.0%)    100.0%
Version symbols section '.gnu.version' contains 11 entries:
 Addr: 00000000000003ac  Offset: 0x0003ac  Link: 3 (.dynsym)
  000:   0 (*local*)       0 (*local*)       0 (*local*)       0 (*local*)    
  004:   2 (GLIBC_2.2.5)   1 (*global*)      1 (*global*)      1 (*global*)   
  008:   1 (*global*)      1 (*global*)      1 (*global*)   
Version needs section '.gnu.version_r' contains 1 entries:
 Addr: 0x00000000000003c8  Offset: 0x0003c8  Link: 4 (.dynstr)
  000000: Version: 1  File: libc.so.6  Cnt: 1
  0x0010:   Name: GLIBC_2.2.5  Flags: none  Version: 2
Notes at offset 0x000001c8 with length 0x00000024:
  Owner                 Data sizeDescription
  GNU                  0x00000014NT_GNU_BUILD_ID (unique build ID bitstring)
    Build ID: f0bc12375091e48c6bac8e67a4e863059e1fc59e
然后我们使用命令：gcc -o vecadd2 main.c ./libvector.so 
可以得到可执行目标程序vecadd2.
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x400560
  Start of program headers:          64 (bytes into file)
  Start of section headers:          4448 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         30
  Section header string table index: 27
Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .interp           PROGBITS         0000000000400238  00000238
       000000000000001c  0000000000000000   A       0     0     1
  [ 2] .note.ABI-tag     NOTE             0000000000400254  00000254
       0000000000000020  0000000000000000   A       0     0     4
  [ 3] .note.gnu.build-i NOTE             0000000000400274  00000274
       0000000000000024  0000000000000000   A       0     0     4
  [ 4] .gnu.hash         GNU_HASH         0000000000400298  00000298
       0000000000000038  0000000000000000   A       5     0     8
  [ 5] .dynsym           DYNSYM           00000000004002d0  000002d0
       0000000000000108  0000000000000018   A       6     1     8
  [ 6] .dynstr           STRTAB           00000000004003d8  000003d8
       000000000000008d  0000000000000000   A       0     0     1
  [ 7] .gnu.version      VERSYM           0000000000400466  00000466
       0000000000000016  0000000000000002   A       5     0     2
  [ 8] .gnu.version_r    VERNEED          0000000000400480  00000480
       0000000000000020  0000000000000000   A       6     1     8
  [ 9] .rela.dyn         RELA             00000000004004a0  000004a0
       0000000000000018  0000000000000018   A       5     0     8
  [10] .rela.plt         RELA             00000000004004b8  000004b8
       0000000000000048  0000000000000018   A       5    12     8
  [11] .init             PROGBITS         0000000000400500  00000500
       0000000000000018  0000000000000000  AX       0     0     4
  [12] .plt              PROGBITS         0000000000400520  00000520
       0000000000000040  0000000000000010  AX       0     0     16
  [13] .text             PROGBITS         0000000000400560  00000560
       0000000000000208  0000000000000000  AX       0     0     16
  [14] .fini             PROGBITS         0000000000400768  00000768
       000000000000000e  0000000000000000  AX       0     0     4
  [15] .rodata           PROGBITS         0000000000400778  00000778
       0000000000000011  0000000000000000   A       0     0     4
  [16] .eh_frame_hdr     PROGBITS         000000000040078c  0000078c
       000000000000002c  0000000000000000   A       0     0     4
  [17] .eh_frame         PROGBITS         00000000004007b8  000007b8
       00000000000000a4  0000000000000000   A       0     0     8
  [18] .ctors            PROGBITS         0000000000600e18  00000e18
       0000000000000010  0000000000000000  WA       0     0     8
  [19] .dtors            PROGBITS         0000000000600e28  00000e28
       0000000000000010  0000000000000000  WA       0     0     8
  [20] .jcr              PROGBITS         0000000000600e38  00000e38
       0000000000000008  0000000000000000  WA       0     0     8
  [21] .dynamic          DYNAMIC          0000000000600e40  00000e40
       00000000000001a0  0000000000000010  WA       6     0     8
  [22] .got              PROGBITS         0000000000600fe0  00000fe0
       0000000000000008  0000000000000008  WA       0     0     8
  [23] .got.plt          PROGBITS         0000000000600fe8  00000fe8
       0000000000000030  0000000000000008  WA       0     0     8
  [24] .data             PROGBITS         0000000000601018  00001018
       0000000000000020  0000000000000000  WA       0     0     8
  [25] .bss              NOBITS           0000000000601038  00001038
       0000000000000018  0000000000000000  WA       0     0     8
  [26] .comment          PROGBITS         0000000000000000  00001038
       000000000000002a  0000000000000001  MS       0     0     1
  [27] .shstrtab         STRTAB           0000000000000000  00001062
       00000000000000fe  0000000000000000           0     0     1
  [28] .symtab           SYMTAB           0000000000000000  000018e0
       0000000000000660  0000000000000018          29    46     8
  [29] .strtab           STRTAB           0000000000000000  00001f40
       00000000000001ff  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
There are no section groups in this file.
Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000400040 0x0000000000400040
                 0x00000000000001f8 0x00000000000001f8  R E    8
  INTERP         0x0000000000000238 0x0000000000400238 0x0000000000400238
                 0x000000000000001c 0x000000000000001c  R      1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x000000000000085c 0x000000000000085c  R E    200000
  LOAD           0x0000000000000e18 0x0000000000600e18 0x0000000000600e18
                 0x0000000000000220 0x0000000000000238  RW     200000
  DYNAMIC        0x0000000000000e40 0x0000000000600e40 0x0000000000600e40
                 0x00000000000001a0 0x00000000000001a0  RW     8
  NOTE           0x0000000000000254 0x0000000000400254 0x0000000000400254
                 0x0000000000000044 0x0000000000000044  R      4
  GNU_EH_FRAME   0x000000000000078c 0x000000000040078c 0x000000000040078c
                 0x000000000000002c 0x000000000000002c  R      4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     8
  GNU_RELRO      0x0000000000000e18 0x0000000000600e18 0x0000000000600e18
                 0x00000000000001e8 0x00000000000001e8  R      1
 Section to Segment mapping:
  Segment Sections...
   00     
   01     .interp 
   02     .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .plt .text .fini .rodata .eh_frame_hdr .eh_frame 
   03     .ctors .dtors .jcr .dynamic .got .got.plt .data .bss 
   04     .dynamic 
   05     .note.ABI-tag .note.gnu.build-id 
   06     .eh_frame_hdr 
   07     
   08     .ctors .dtors .jcr .dynamic .got 
Dynamic section at offset 0xe40 contains 21 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [./libvector.so]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000000c (INIT)               0x400500
 0x000000000000000d (FINI)               0x400768
 0x000000006ffffef5 (GNU_HASH)           0x400298
 0x0000000000000005 (STRTAB)             0x4003d8
 0x0000000000000006 (SYMTAB)             0x4002d0
 0x000000000000000a (STRSZ)              141 (bytes)
 0x000000000000000b (SYMENT)             24 (bytes)
 0x0000000000000015 (DEBUG)              0x0
 0x0000000000000003 (PLTGOT)             0x600fe8
 0x0000000000000002 (PLTRELSZ)           72 (bytes)
 0x0000000000000014 (PLTREL)             RELA
 0x0000000000000017 (JMPREL)             0x4004b8
 0x0000000000000007 (RELA)               0x4004a0
 0x0000000000000008 (RELASZ)             24 (bytes)
 0x0000000000000009 (RELAENT)            24 (bytes)
 0x000000006ffffffe (VERNEED)            0x400480
 0x000000006fffffff (VERNEEDNUM)         1
 0x000000006ffffff0 (VERSYM)             0x400466
 0x0000000000000000 (NULL)               0x0
Relocation section '.rela.dyn' at offset 0x4a0 contains 1 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000600fe0  000400000006 R_X86_64_GLOB_DAT 0000000000000000 __gmon_start__ + 0
Relocation section '.rela.plt' at offset 0x4b8 contains 3 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000601000  000100000007 R_X86_64_JUMP_SLO 0000000000000000 printf + 0
000000601008  000200000007 R_X86_64_JUMP_SLO 0000000000000000 __libc_start_main + 0
000000601010  000300000007 R_X86_64_JUMP_SLO 0000000000000000 addvec + 0
There are no unwind sections in this file.
Symbol table '.dynsym' contains 11 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND printf@GLIBC_2.2.5 (2)
     2: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@GLIBC_2.2.5 (2)
     3: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND addvec
     4: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     5: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _Jv_RegisterClasses
     6: 0000000000601038     0 NOTYPE  GLOBAL DEFAULT  ABS _edata
     7: 0000000000601050     0 NOTYPE  GLOBAL DEFAULT  ABS _end
     8: 0000000000601038     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_start
     9: 0000000000400500     0 FUNC    GLOBAL DEFAULT   11 _init
    10: 0000000000400768     0 FUNC    GLOBAL DEFAULT   14 _fini
Symbol table '.symtab' contains 68 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000400238     0 SECTION LOCAL  DEFAULT    1 
     2: 0000000000400254     0 SECTION LOCAL  DEFAULT    2 
     3: 0000000000400274     0 SECTION LOCAL  DEFAULT    3 
     4: 0000000000400298     0 SECTION LOCAL  DEFAULT    4 
     5: 00000000004002d0     0 SECTION LOCAL  DEFAULT    5 
     6: 00000000004003d8     0 SECTION LOCAL  DEFAULT    6 
     7: 0000000000400466     0 SECTION LOCAL  DEFAULT    7 
     8: 0000000000400480     0 SECTION LOCAL  DEFAULT    8 
     9: 00000000004004a0     0 SECTION LOCAL  DEFAULT    9 
    10: 00000000004004b8     0 SECTION LOCAL  DEFAULT   10 
    11: 0000000000400500     0 SECTION LOCAL  DEFAULT   11 
    12: 0000000000400520     0 SECTION LOCAL  DEFAULT   12 
    13: 0000000000400560     0 SECTION LOCAL  DEFAULT   13 
    14: 0000000000400768     0 SECTION LOCAL  DEFAULT   14 
    15: 0000000000400778     0 SECTION LOCAL  DEFAULT   15 
    16: 000000000040078c     0 SECTION LOCAL  DEFAULT   16 
    17: 00000000004007b8     0 SECTION LOCAL  DEFAULT   17 
    18: 0000000000600e18     0 SECTION LOCAL  DEFAULT   18 
    19: 0000000000600e28     0 SECTION LOCAL  DEFAULT   19 
    20: 0000000000600e38     0 SECTION LOCAL  DEFAULT   20 
    21: 0000000000600e40     0 SECTION LOCAL  DEFAULT   21 
    22: 0000000000600fe0     0 SECTION LOCAL  DEFAULT   22 
    23: 0000000000600fe8     0 SECTION LOCAL  DEFAULT   23 
    24: 0000000000601018     0 SECTION LOCAL  DEFAULT   24 
    25: 0000000000601038     0 SECTION LOCAL  DEFAULT   25 
    26: 0000000000000000     0 SECTION LOCAL  DEFAULT   26 
    27: 000000000040058c     0 FUNC    LOCAL  DEFAULT   13 call_gmon_start
    28: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    29: 0000000000600e18     0 OBJECT  LOCAL  DEFAULT   18 __CTOR_LIST__
    30: 0000000000600e28     0 OBJECT  LOCAL  DEFAULT   19 __DTOR_LIST__
    31: 0000000000600e38     0 OBJECT  LOCAL  DEFAULT   20 __JCR_LIST__
    32: 00000000004005b0     0 FUNC    LOCAL  DEFAULT   13 __do_global_dtors_aux
    33: 0000000000601038     1 OBJECT  LOCAL  DEFAULT   25 completed.6531
    34: 0000000000601040     8 OBJECT  LOCAL  DEFAULT   25 dtor_idx.6533
    35: 0000000000400620     0 FUNC    LOCAL  DEFAULT   13 frame_dummy
    36: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    37: 0000000000600e20     0 OBJECT  LOCAL  DEFAULT   18 __CTOR_END__
    38: 0000000000400858     0 OBJECT  LOCAL  DEFAULT   17 __FRAME_END__
    39: 0000000000600e38     0 OBJECT  LOCAL  DEFAULT   20 __JCR_END__
    40: 0000000000400730     0 FUNC    LOCAL  DEFAULT   13 __do_global_ctors_aux
    41: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.c
    42: 0000000000600e14     0 NOTYPE  LOCAL  DEFAULT   18 __init_array_end
    43: 0000000000600e40     0 OBJECT  LOCAL  DEFAULT   21 _DYNAMIC
    44: 0000000000600e14     0 NOTYPE  LOCAL  DEFAULT   18 __init_array_start
    45: 0000000000600fe8     0 OBJECT  LOCAL  DEFAULT   23 _GLOBAL_OFFSET_TABLE_
    46: 0000000000400720     2 FUNC    GLOBAL DEFAULT   13 __libc_csu_fini
    47: 0000000000601018     0 NOTYPE  WEAK   DEFAULT   24 data_start
    48: 0000000000601038     0 NOTYPE  GLOBAL DEFAULT  ABS _edata
    49: 0000000000601048     8 OBJECT  GLOBAL DEFAULT   25 z
    50: 0000000000601028     8 OBJECT  GLOBAL DEFAULT   24 x
    51: 0000000000400768     0 FUNC    GLOBAL DEFAULT   14 _fini
    52: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND printf@@GLIBC_2.2.5
    53: 0000000000600e30     0 OBJECT  GLOBAL HIDDEN    19 __DTOR_END__
    54: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@@GLIBC_
    55: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND addvec
    56: 0000000000601018     0 NOTYPE  GLOBAL DEFAULT   24 __data_start
    57: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
    58: 0000000000601020     0 OBJECT  GLOBAL HIDDEN    24 __dso_handle
    59: 0000000000400778     4 OBJECT  GLOBAL DEFAULT   15 _IO_stdin_used
    60: 0000000000400690   137 FUNC    GLOBAL DEFAULT   13 __libc_csu_init
    61: 0000000000601050     0 NOTYPE  GLOBAL DEFAULT  ABS _end
    62: 0000000000400560     0 FUNC    GLOBAL DEFAULT   13 _start
    63: 0000000000601030     8 OBJECT  GLOBAL DEFAULT   24 y
    64: 0000000000601038     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_start
    65: 0000000000400644    68 FUNC    GLOBAL DEFAULT   13 main
    66: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _Jv_RegisterClasses
    67: 0000000000400500     0 FUNC    GLOBAL DEFAULT   11 _init
Histogram for `.gnu.hash' bucket list length (total of 3 buckets):
 Length  Number     % of total  Coverage
      0  0          (  0.0%)
      1  1          ( 33.3%)     20.0%
      2  2          ( 66.7%)    100.0%
Version symbols section '.gnu.version' contains 11 entries:
 Addr: 0000000000400466  Offset: 0x000466  Link: 5 (.dynsym)
  000:   0 (*local*)       2 (GLIBC_2.2.5)   2 (GLIBC_2.2.5)   0 (*local*)    
  004:   0 (*local*)       0 (*local*)       1 (*global*)      1 (*global*)   
  008:   1 (*global*)      1 (*global*)      1 (*global*)   
Version needs section '.gnu.version_r' contains 1 entries:
 Addr: 0x0000000000400480  Offset: 0x000480  Link: 6 (.dynstr)
  000000: Version: 1  File: libc.so.6  Cnt: 1
  0x0010:   Name: GLIBC_2.2.5  Flags: none  Version: 2
Notes at offset 0x00000254 with length 0x00000020:
  Owner                 Data sizeDescription
  GNU                  0x00000010NT_GNU_ABI_TAG (ABI version tag)
    OS: Linux, ABI: 2.6.24
Notes at offset 0x00000274 with length 0x00000024:
  Owner                 Data sizeDescription
  GNU                  0x00000014NT_GNU_BUILD_ID (unique build ID bitstring)
    Build ID: 398c8d922411743e35f3fe6f2ceee7d226a82cca
vecadd2和vecadd1相比，vecadd2只有不到三百行，而vecadd却有两千多行，基本上vecadd2
也就是vecadd的零头。
