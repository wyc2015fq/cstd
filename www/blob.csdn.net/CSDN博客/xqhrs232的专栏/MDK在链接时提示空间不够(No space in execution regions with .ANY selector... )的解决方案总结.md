# MDK在链接时提示空间不够(No space in execution regions with .ANY selector... )的解决方案总结 - xqhrs232的专栏 - CSDN博客
2019年03月02日 09:23:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：77
原文地址::[https://blog.csdn.net/gengyiping18/article/details/52640806/](https://blog.csdn.net/gengyiping18/article/details/52640806/)
RealView MDK具有强大的编译、链接功能。嵌入式设备拥有的资源往往有限，如果一个程序编译、链接之后的可执行文件大小略大于存储大小时，则不能生成可执行文件进行调试或固化到Flash中。在这种情况下我们一般不去从硬件上调整存储空间的大小(有些存储空间在片内，实在无法调整)，而是去修改程序，试图将程序简化、将缓存减小、减少全局变量、少用大数组多用指针等等人工方法，有些时候这些方法很有效。但如果使用的开发工具是RealView MDK，则有一系列方法，可以达到这一目的，比起手动方法只有过之而无不及。这些方法仅仅需要在编译、链接之前对某些配置做一些修改即可，使用非常简单。当然，如果将这些方法和人工修改程序的方法结合起来，效果更佳。有时单独使用以下三种方法中的一种即可解决问题，有时需要结合其中两种，甚至三种才能解决问题。下面以开发板Embest ATEB9200（采用的SOC 为AT91RM9200）的例程USARTPDC_test为例，由于片内SRAM空间比较小(大小为16KB，起始地址为0x200000)，本例程在片内SRAM中进行调试时需要使用一些技巧，下面介绍如何使用这些方法。
在默认设置下，将例程USARTPDC_test选择在片上SRAM中调试时，编译通过、在链接时会提示以下错误信息提示空间不够：
compiling init.c...
compiling interrupt_Usart.c...
compiling main.c...
linking...
.\Int_RAM\USARTPDC_test.axf: Error: L6406W: No space in execution regions with .ANY selector matching Section .text(_scanf_int.o).
.\Int_RAM\USARTPDC_test.axf: Error: L6406W: No space in execution regions with .ANY selector matching Section .text(_scanf_longlong.o).
.\Int_RAM\USARTPDC_test.axf: Error: L6406W: No space in execution regions with .ANY selector matching Section .text(aeabi_sdiv.o).
Target not created
一、使用微库
使用微库时，将以更精简短小的C库替代标准C库，减小代码大小，使用微库的方法如下：
打开Project->Options->Target，将Use MicroLIB前面的复选框勾上。如下图：
图一  使用微库
点击确定之后，重新编译、链接。发现还是会提示空间不够的错误信息，但错误信息明显比以前少了：
Build target 'USARTPDC_test Int RAM'
assembling RM9200.s...
compiling init.c...
compiling interrupt_Usart.c...
compiling main.c...
linking...
.\Int_RAM\USARTPDC_test.axf: Error: L6406W: No space in execution regions with .ANY selector matching Section .text(interrupt_usart.o).
.\Int_RAM\USARTPDC_test.axf: Error: L6406W: No space in execution regions with .ANY selector matching Section i._printf_core(printf8.o).
.\Int_RAM\USARTPDC_test.axf: Error: L6407W: Sections of aggregate size 0x86c bytes could not fit into .ANY selector(s).
Target not created
二、修改链接脚本
修改链接脚本的方法有两种一种是直接修改分散加载文件，另一种是使用默认分散加载文件，这时仅需修改Project->Options->Target中的存储空间起始和大小。为了简单直观，本例中使用后者。在使用后者时，需要在Project->Options->Linker中将Use Memory Layout from Target Dialog前面的复选框勾上。如下图：
图二 使用默认的分散加载文件
然后在Project->Options->Target中修改存储空间中只读部分和可读写部分的起始和大小，一般来说加大只读部分大小（该部分存放程序中的指令），而减小可读写部分的大小（该部分存放堆栈、局部变量等）。
根据错误提示可知，只读部分空间最少需要加大0x86c bytes，总空间固定16KB，相应地需要减小可读写部分大小。修改之前如图一，修改后如下图：
图三  增大只读部分大小，减小可读写部分大小
点击确定之后，重新编译、链接：
Build target 'USARTPDC_test Int RAM'
assembling RM9200.s...
compiling init.c...
compiling interrupt_Usart.c...
compiling main.c...
linking...
Program Size: Code=9388 RO-data=380 RW-data=4 ZI-data=1244 
".\Int_RAM\USARTPDC_test.axf" - 0 Error(s), 0 Warning(s).
编译链接成功！
三、修改优化级别，着重对空间进行优化
在编译时着重对空间进行优化，让编译器自动减小代码大小。可使用编译选项-Ospace进行编译以达到目的，另外可以选更高的选优化级别―-03如下图：
图四 选较高优化级别针对空间进行优级化以减小代码大小
