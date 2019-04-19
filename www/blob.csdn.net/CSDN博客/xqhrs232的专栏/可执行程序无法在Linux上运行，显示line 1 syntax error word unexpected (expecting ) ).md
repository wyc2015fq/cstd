# 可执行程序无法在Linux上运行，显示line 1: syntax error: word unexpected (expecting ")" ) - xqhrs232的专栏 - CSDN博客
2011年12月22日 14:51:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1634
原文地址::[http://hi.baidu.com/serial_story/blog/item/450b060113846a067aec2cc2.html/cmtid/9e5675edc51727dbb21cb133](http://hi.baidu.com/serial_story/blog/item/450b060113846a067aec2cc2.html/cmtid/9e5675edc51727dbb21cb133)
相关网帖::华为3G模块E1750在我的Android系统下识别不了，/dev目录下不见ttyUSB*驱动产生 [http://topic.csdn.net/u/20111221/14/e9f55302-96bb-4243-b81c-b6d3c6eeab1c.html](http://topic.csdn.net/u/20111221/14/e9f55302-96bb-4243-b81c-b6d3c6eeab1c.html)
**【已解决】可执行程序无法在Linux上运行，显示line 1: syntax error: word unexpected (expecting ")")**
【问题】
用arm-linux-gcc编译出来的可执行文件clkCtl，下载到板子上，在Linux下不能运行：
./clkCtl: line 1: syntax error: word unexpected (expecting ")")
【解决过程】
1.网上有人也遇到此问题：
Syntax error: word unexpected (expecting ")")
[http://hi.baidu.com/dsfire/blog/item/5d922458886ad589800a188b.html](http://hi.baidu.com/dsfire/blog/item/5d922458886ad589800a188b.html)
其是用ftp传到板子上的，是没有传输完整，所以导致运行有问题，
但是我是用的lrz通过secureCRT传的，确保文件是完整的。
2.以为是我烧的uImage和rootfs有问题，又重新烧写一遍，
还是不行。
3.以为编译出来的文件有问题，所以换了arm-linux-uclibc-gcc去编译，结果也还是不行。4.
4.
在一块移植了linux 的开发板上运行两个测试程序出错
[http://linux.chinaunix.net/bbs/thread-1064286-1-1.html](http://linux.chinaunix.net/bbs/thread-1064286-1-1.html)
中提到用file查看一下，所以去Linux服务器上去查看了一下，看起来好像也是OK的：
file clkCtl 
clkCtl: ELF 32-bit LSB relocatable, ARM, version 1 (ARM), not stripped
甚至用了相关工具查看，好像也是OK的：
hexdump n 1 clkCtl
hexdump: n: No such file or directory
hexdump: 1: No such file or directory
0000000 457f 464c 0101 6101 0000 0000 0000 0000
0000010 0001 0028 0001 0000 0000 0000 0000 0000
0000020 0630 0000 0000 0000 0034 0000 0000 0028
。。。
objdump -a clkCtl
clkCtl:     file format elf32-little
clkCtl
objdump -f clkCtl
clkCtl:     file format elf32-little
architecture: UNKNOWN!, flags 0x00000011:
HAS_RELOC, HAS_SYMS
start address 0x00000000
objdump -h clkCtl
clkCtl:     file format elf32-little
Sections:
Idx Name          Size      VMA       LMA       File off Algn
0 .text         000003e8 00000000 00000000 00000034 2**2
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
1 .data         00000000 00000000 00000000 0000041c 2**0
                  CONTENTS, ALLOC, LOAD, DATA
2 .bss          00000004 00000000 00000000 0000041c 2**2
                  ALLOC
3 .rodata       00000190 00000000 00000000 0000041c 2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
4 .comment      00000012 00000000 00000000 000005ac 2**0
                  CONTENTS, READONLY
5 .note.GNU-stack 00000000 00000000 00000000 000005be 2**0
                  CONTENTS, READONLY
6 .ARM.attributes 00000010 00000000
5.百google度了N个帖子，都没有说出具体问题的，
只有一个帖子说的好像说到点子上了：
After updating to AIR SDK 1.5.1 all Flex AIR apps give "abc bytecode decoding" error
[http://bugs.adobe.com/jira/browse/FBE-323](http://bugs.adobe.com/jira/browse/FBE-323)
“
flex_sdk_3/bin/adl_lin: 1: Syntax error: word unexpected (expecting ")")
I've seen this error before and usually its a shell execution problem, but I'm not quite sure how to fix it here. Usually it's
something like change #!/bin/sh to #!/bin/bash, but since adl_lin is a binary file I'm not sure how to go about fixing it.
FYI: I'm running Ubuntu 8.04, if that is helpful in anyway. 
”
好像是sh的问题，
但是，去/bin目录下看了，sh好像没问题。
# sh --help 
BusyBox v1.13.2 (2009-05-13 11:38:15 CST) multi-call binary
No help available.
实在是无语了。。。。。
【解决办法】
刚看到这个帖子：
Cygwin and NMT toolchain 
[http://www.networkedmediatank.com/showthread.php?tid=20303](http://www.networkedmediatank.com/showthread.php?tid=20303)
没注意，后来才无意发现，原来其中就说明了原因和介绍了解决办法。。。
一切软件的错误，最终都能找到其原因，此处也不例外。
最后的发现，是我在写个简单的makefile的时候，在写编译命令的时候，写成了：
$(CC) -o $(OUT_FILE) -c $(SRC_FILE)
而实际应该是去掉-c选项，直接编译成可执行文件：
$(CC) -o $(OUT_FILE) $(SRC_FILE)
之前就一直搞混淆了-c参数，以为后面正好跟c文件。。。
现在又弄错了。。。
其中gcc的-c选项的意思是：
-c 
　　只激活预处理,编译,和汇编,也就是他只把程序做成obj文件 
　　例子用法: 
　　gcc -c hello.c 
　　他将生成.o的obj文件
也就是，如果不加-c，默认就直接编译生成可执行文件了
加上-c就只编译成目标obj文件，就不往下继续编译成可执行文件了。。。
我们实际的命令
arm-linux-gcc -o clkCtl -c clock_control.c
的意思是，将clock_control.c编译成目标文件，此输出的（目标）文件叫做clkCtl
其实如果此处不写-o clkCtl,一般默认输出的文件名是和C文件名一样，只是后缀是.o，此处即clock_control.o的。
arm-linux-gcc -o clkCtl clock_control.c
的意思是，将clock_control.c编译成可执行文件（就是先编译成目标文件然后再链接成可执行文件）
其中输出的文件名是clkCtl，如果不加上-o clkCtl ，那么Linux默认输出文件名就是a.out了，
容易混淆而且容易被覆盖，也没实际意义，所以一般加上-o 制定输出的可执行程序的文件名。
另外，说一下，gcc还有个参数是大写的C：
-C 
　　在预处理的时候,不删除注释信息,一般和-E使用,有时候分析程序，用这个很方便的 
注意不要和这个小写的c混淆了。。。
【附录】
gcc参数详解
[http://man.lupaworld.com/content/develop/UNIX_system_develop_gcc.htm](http://man.lupaworld.com/content/develop/UNIX_system_develop_gcc.htm)
【总结】
其他朋友们，遇到这个问题，估计不少也是和我一样，
就是在编译的时候，错误地加入了-c 选项，使得生成出来的不是可执行文件，是目标文件，
所以运行的时候，就无法运行了。
感慨一句，这么基本的基础知识都忘了，丢人啊。。。。
【后记】
其实是自己的相关知识还是不够，把目标文件的信息，看成是可执行文件的信息了，因为刚才又去用file看了真正可执行文件的信息，和之前file查看目标文件的结果是不一样的：
[crifan@localhost clock_control]$ file clock_control.o
clock_control.o: ELF 32-bit LSB **relocatable**, ARM, version 1 (ARM), not stripped
[crifan@localhost clock_control]$ file clkCtl
clkCtl: ELF 32-bit LSB **executable**, ARM, version 1 (ARM), dynamically linked (uses shared libs), not stripped
[crifan@localhost clock_control]$
如果之前就具备这些知识，那么也就可以看出毛病了。还有，如果脑子灵活点，用file去查看其他已经有的可执行文件，然后和上面的比较，也是可以看出来的。还是方法不当啊。。。
同样，用readelf工具查看也可以看清楚具体信息：
[crifan@localhost clock_control]$ **readelf -h clkCtl**
ELF Header:
Magic:   7f 45 4c 46 01 01 01 61 00 00 00 00 00 00 00 00 
Class:                             ELF32
Data:                              2's complement, little endian
Version:                           1 (current)
OS/ABI:                            ARM
ABI Version:                       0
**Type:                              EXEC (Executable file)**
Machine:                           ARM
Version:                           0x1
Entry point address:               0x854c
Start of program headers:          52 (bytes into file)
Start of section headers:          5036 (bytes into file)
Flags:                             0x2, has entry point, GNU EABI
Size of this header:               52 (bytes)
Size of program headers:           32 (bytes)
Number of program headers:         6
Size of section headers:           40 (bytes)
Number of section headers:         31
Section header string table index: 28
[crifan@localhost clock_control]$ ls
clkCtl clock_control.c clock_control.h clock_control.o clock-dev.ko makefile ReadMe.txt
[crifan@localhost clock_control]$ **readelf -h clock_control.o**ELF Header:
Magic:   7f 45 4c 46 01 01 01 61 00 00 00 00 00 00 00 00 
Class:                             ELF32
Data:                              2's complement, little endian
Version:                           1 (current)
OS/ABI:                            ARM
ABI Version:                       0
**Type:                              REL (Relocatable file)**
Machine:                           ARM
Version:                           0x1
Entry point address:               0x0
Start of program headers:          0 (bytes into file)
Start of section headers:          1584 (bytes into file)
Flags:                             0x0
Size of this header:               52 (bytes)
Size of program headers:           0 (bytes)
Number of program headers:         0
Size of section headers:           40 (bytes)
Number of section headers:         12
Section header string table index: 9
//=================================================================
备注:
 1>今天才发现我的应用程序执行不起来，不是因为编译版本不对的问题，而是要用system命令来调用，因为系统自己带的应用程序在adb下也报同样的错误
 2>简单的应用程序( 如toolbox:ls+cd+cat)直接在adb下执行就可以了，但复杂的必须用system命令调用才可以正确执行,如system("mytest")
