# DSP TMS320C6000基础学习（3）——CCS v5软件开发环境搭建 - xiahouzuoxin - CSDN博客





2013年07月25日 20:53:37[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：16149







==================================================
***DSP CCS工程文件构成***
==================================================
1. 源文件(*.c *.asm)
2. 头文件(*.h *.inc)
3. cmd文件
   分为2种。
   一种是用来分配RAM空间的，用来将程序下载到RAM内调试，因为开发过程中大部分
   时间在调试程序，因此多用这类cmd；
   另一种是分配Flash空间的，当程序调试完毕后，需要将其烧写到Flash中固化。

4. 库文件(*.lib *.dll)
   库文件包含了寄存器的地址和对应标示符的定义，还包括了标准C/C++运行支持库函数。
   *.lib为静态库文件，在编译时调用，调用时直接加载到内存；
   *.dll为动态库文件，在运行时调用。


==================================================
***创建DSP TMS320C6713工程***
==================================================   

==================== 准备工作 ====================
1. 从http://www.ti.com/lsds/ti/dsp/c6000_dsp/c67x/toolsw.page下载C6XCSL及其文档 
   C6XCSL为DSP C6000系列的芯片支持库（TMS320C6000 Chip Support Library）

   安装C6XCSL，点击.exe文件（其实就是解压），选择路径，本文为设为
   C6XCSL_ROOT=E:\MyDesigner\dsp\C6XCSL

2. 下载并安装破解CCS 5.3，安装路径不要有中文字符


==================== 新建工程 ====================
1. File -> New Project -> CCS Project 
![](https://img-blog.csdn.net/20130725204954203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   工程路径不要有中文字符。


2. 添加库文件
   工程右键 -> Add Files...
   转到C6XCSL_ROOT\lib_3x下。这里有2个6713系列文件：csl6713.lib和csl6713e.lib。

   两者区别是csl6713.lib用于小端(little endian)格式DSP，
   而csl6713e.lib用于大端(little endian)格式DSP。
   我们新建工程时选择了小端，因此这里选择csl6713.lib。
   然后弹出提示，选择“copy to files”将库文件拷贝一份到当前工程。

3. 添加头文件
   基本头文件是与库文件对应的。
   将C6XCSL_ROOT\include\文件夹拷贝到当前工程目录。
   工程右键 -> Properties, Build -> C6000 Compiler -> Include Options，
   添加../include头文件搜索目录。
![](https://img-blog.csdn.net/20130725205118312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4. 添加.cmd文件，这个可以参考C6XCSL_ROOT\example\下的例子，下面是Timer的例子，

```
/* Memory Map 0 - the default */
   MEMORY
   {
   PMEM:o = 00000000hl = 00010000h
   BMEM:o = 00010000hl = 00030000h 
   }
   
   SECTIONS
   {    
       .text       >   PMEM
       .csldata> PMEM
       .stack>   PMEM
       .far>   PMEM
       .switch>   BMEM 
       .tables >   BMEM
       .data   >   BMEM
       .bss>   BMEM
       .sysmem>   BMEM
       .cinit>   PMEM
       .const>   BMEM
       .cio    >   BMEM 
   }
```



5. 新建mian.c，这个不用多说了吧

```cpp
#include <csl.h>
	 void main(void) {
		 while (1) {
	
		 }
	 }
```


==================== 附属问题 ====================
1. 新建工程结束，右键 -> Build Project，出现如下error，
   fatal error #35: #error NO CHIP DEFINED (use -dCHIP_XXXX where XXXX is chip number, i.e. 6201)
   原因：使用C6XCSL库需要定义芯片信号，错误提示没定义芯片型号
   解决：一种方法，在main.c中的#inclde <csl.h>前添加#define CHIP_6713；
         另一种方法，工程右键 -> Properties，Build -> C6000 Compiler -> Predefined Symbols，
         在“Pre-define NAME”中添加CHIP_6713

![](https://img-blog.csdn.net/20130725205141062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2. 右键 -> Build Project，出现如下warning，
   warning #10210-D: creating ".stack" section with default size of 0x400; use the
      -stack option to change the default size
   warning #10210-D: creating ".sysmem" section with default size of 0x400; use
      the -heap option to change the default size
   原因：没有设置堆大小(stack size)和栈大小(heap size)
   解决：工程右键 -> Properties，Build -> C6000 Linker -> Basic Options，
         设置stack size和heap size都为0x3FF。

![](https://img-blog.csdn.net/20130725205204765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






