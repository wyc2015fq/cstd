# ld 用法：gcc 过程最后一步-链接 - Koma Hub - CSDN博客
2019年02月10日 18:48:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：87
**目录**
[1、什么是ld？它有什么作用？](#1%E3%80%81%E4%BB%80%E4%B9%88%E6%98%AFld%EF%BC%9F%E5%AE%83%E6%9C%89%E4%BB%80%E4%B9%88%E4%BD%9C%E7%94%A8%EF%BC%9F)
[2、常用的选项](#2%E3%80%81%E5%B8%B8%E7%94%A8%E7%9A%84%E9%80%89%E9%A1%B9)
[3、链接描述脚本](#3%E3%80%81%E9%93%BE%E6%8E%A5%E6%8F%8F%E8%BF%B0%E8%84%9A%E6%9C%AC)
[（1）[Makefile]](#%EF%BC%881%EF%BC%89%5BMakefile%5D)
[（2）[arch/Makefile]](#%EF%BC%882%EF%BC%89%5Barch%2FMakefile%5D)
今天把vivi所用到的ld的用法都看懂了，常见的选项含义也清楚了。翻看了一下[kernel](https://www.baidu.com/s?wd=kernel&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的链接脚本，发现相当复杂，并且需要对全局有很好的把握，对整个的地址空间分配也必须清楚。这点暂时还做不到，属于后续工作。先把ld的基本用法总结一下，因为《Linkers and Loaders》还没有读完，所以暂时还不能作出总结。不过，对链接和加载已经有了更深入的认识。在读using ld时，很多地方自然就理解了。慢慢来，把这块知识体协理顺。
## 1、什么是ld？它有什么作用？
    ld是GNU binutils工具集中的一个，是众多Linkers（链接器）的一种。完成的功能自然也就是链接器的基本功能：把各种目标文件和库文件链接起来，并重定向它们的数据，完成符号解析。Linking其实主要就是完成四个方面的工作：storage allocation、symbol management、libraries、relocation。
    ld可以识别一种Linker command Language表示的linker scriopt文件来显式的控制链接的过程。通过BFD（Binary Format Description）库，ld可以读取和操作COFF（common object file format）、ELF（executable and linking format）、a.out等各种格式的目标文件。
## 2、常用的选项
-b TARGET  设置目标文件的文件格式
-e ADDRESS 设置目标文件的开始地址
-EB  链接big-endian的目标文件
-EL  链接small-endian的目标文件
-l LIBNAME    创建执行程序时要链接的库文件（比如某个库为test，则可以为-ltest）
-L DIRECTORY  寻找要链接的库文件时搜索的文件路径
-o FILE  设置输出文件的名字
-s  去除输出文件中的所有符号信息
-S  去除输出文件中的调试符号信息
-T FILE  读取链接描述脚本，以确定符号等的定位地址
-v  输出ld的版本信息
-x  去除所有的局部符号信息
-X  去除临时的局部符号信息，默认情况下会设置这个选项
-Bstatic   创建的输出文件链接静态链接库
-Bdynamic  创建的输出文件链接动态链接库
-Tbss ADDRESS  设置section bss的起始地址
-Tdata ADDRESS 设置section data的起始地址
-Ttext ADDRESS 设置section text的起始地址
## 3、链接描述脚本
    链接描述脚本描述了各个输入文件的各个section如何映射到输出文件的各section中，并控制输出文件中section和符号的内存布局。
    目标文件中每个section都有名字和大小，而且可以标识为loadable（表示该section可以加载到内存中）、allocatable（表示必须为这个section开辟一块空间，但是没有实际内容下载到这里）。如果不是loadable或者allocatable，则一般含有调试信息。
    每个有loadable或allocatable标识的输出section有两种地址，一种是VMA（Virtual Memory Address），这种地址是输出文件运行时section的运行地址；一种是LMA（Load Memory Address），这种地址是加载输出文件时section的加载地址。一般，这两种地址相同。但在嵌入式系统中，经常存在执行地址和加载地址不一致的情况。如把输出文件加载到开发板的flash存储器中（地址由LMA指定），但运行时，要把flash存储器中的输出文件复制到SDRAM中运行（地址有VMA指定）。
    在链接脚本中使用注释，可以用“/*...*/”。
    每个目标文件有许多符号，每个符号有一个名字和一个地址，一个符号可以是定义的，也可以是未定义的。对于普通符号，需要一个特殊的标识，因为在目标文件中，普通符号没有一个特定的输入section。链接器会把普通符号处理成好像它们都在一个叫做COMMON的section中。
下面给出vivi的ld script的内容及分析。
### （1）[Makefile]
`LINKFLAGS = -Tarch/vivi.lds -Bstatic`
    可见，链接的脚本是arch/vivi.lds，而且链接静态库。但是在arch下没有vivi.lds，而是有vivi.lds.in。看了一下vivi.lds.in的内容，
```
SECTIONS { 
			  . = TEXTADDR;
			  .text : { *(.text) }
			  .data ALIGN(4) : { *(.data) }
			  .bss ALIGN(4) : { *(.bss) *(COMMON) }
			}
```
    很明显，这个就是原始的vivi的链接脚本。但是存在一个变量TEXTADDR没有赋值，也就是说，这个量根据配置的不同是不同的，所以肯定就在Makefile中执行了生成方法。下一步就要看[arch/Makefile]
### （2）[arch/Makefile]
`LDSCRIPT = arch/vivi.lds.in`
```
ifeq ($(CONFIG_ARCH_S3C2410),y)
			MACHINE = s3c2410
			  ifeq ($(CONFIG_S3C2410_NAND_BOOT),y)
			    TEXTADDR = 0x33f00000
			  else
			    TEXTADDR = 0x00000000
			  endif
			endif
```
```
vivi: $(HEAD) arch/vivi.lds
			arch/vivi.lds: $(LDSCRIPT)
			        @sed s/TEXTADDR/$(TEXTADDR)/ $(LDSCRIPT) >$@
```
    很明显，这步主要完成的工作就是要把vivi.lds.in文件中的TEXTADDR用配置后的实际值来代替。根据我的配置，这里我的TEXTADDR就是0x33f00000.
```
SECTIONS { 
			  . = 0x33f00000;
			  .text : { *(.text) }
			  .data ALIGN(4) : { *(.data) }
			  .bss ALIGN(4) : { *(.bss) *(COMMON) }
			}
```
    SECTIONS表示段。第一行表示当前地址为0x33f00000，就是VMA，同时也是text段的起始地址。第二行用了通配符*表示所有字符，这里的意思就是说指定的每个目标文件的text section的内容都放到同一个.text中。第三行表示指定的每个目标文件的data section的内容都放到同一个.data中，而且要四字节边界对齐。第四行表示指定的每个目标文件的bss section的内容都放到同一个.bss中，所有的普通符号都放到COMMON中，而且要四字节边界对齐。
    这算是最为简单的ld scripts，不过也够用了。如果不考虑对齐等因素，则可以直接在命令行中指定-Ttext 0x33f00000，就可以完成了。当然，对[Linux](http://lib.csdn.net/base/linux) kernel等，ld scripts要处理复杂的内存分配等操作，相应的要复杂一些，读那些的方法就是查阅using ld手册，同时还要研究MCU的内存分配，这样才能作出合理的安排。
转自：[https://blog.csdn.net/rrr_obben/article/details/73556623](https://blog.csdn.net/rrr_obben/article/details/73556623)
