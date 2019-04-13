
# 使用gdb - 嵌入式Linux - CSDN博客

2015年11月17日 15:39:46[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：721



## 1、gdb调试
gdb 对于看系统内部是非常有用. 在这个级别精通调试器的使用要求对 gdb 命令有信心,
需要理解目标平台的汇编代码, 以及对应源码和优化的汇编码的能力.
LINUX DEVICE DRIVERS,3RD EDITION
调试器必须把内核作为一个应用程序来调用. 除了指定内核映象的文件名之外, 你需要在
命令行提供一个核心文件的名子. 对于一个运行的内核, 核心文件是内核核心映象,
/proc/kcore. 一个典型的 gdb 调用看来如下:
**gdb /usr/src/linux/vmlinux /proc/kcore**
第一个参数是非压缩的 ELF 内核可执行文件的名子, 不是 zImage 或者 bzImage 或者给
启动环境特别编译的任何东东.
gdb 命令行的第二个参数是核心文件的名子. 如同任何 /proc 中的文件, /proc/kcore
是在被读的时候产生的. 当 read 系统调用在 /proc 文件系统中执行时, 它映射到一个
数据产生函数,而不是一个数据获取函数; 我们已经在本章”使用 /proc 文件系统”一节中
利用了这个特点. kcore 用来代表内核”可执行文件”, 以一个核心文件的形式; 它是一个
巨大的文件, 因为他代表整个的内核地址空间, 对应于所有的物理内存. 从 gdb 中, 你
可查看内核变量,通过发出标准 gdb 命令. 例如, p jiffies 打印时钟的从启动到当前时
间的嘀哒数
```python
weiqifa@weiqifa
```
```python
-Inspiron
```
```python
-
```
```python
3847
```
```python
:~/weiqifa/new_tm100/tm100$ vim wmkkernel 
weiqifa@weiqifa
```
```python
-Inspiron
```
```python
-
```
```python
3847
```
```python
:~/weiqifa/new_tm100/tm100$ find
```
```python
.
```
```python
/out
```
```python
/
```
```python
-iname
```
```python
vmlinux
```
```python
.
```
```python
/out/target/product/mid713l_lp_lvds/obj/KERNEL_OBJ/arch/arm/boot/compressed/vmlinux
```
```python
.
```
```python
/out/target/product/mid713l_lp_lvds/obj/KERNEL_OBJ/vmlinux
^C
weiqifa@weiqifa
```
```python
-Inspiron
```
```python
-
```
```python
3847
```
```python
:~/weiqifa/new_tm100/tm100$ gdb
```
```python
.
```
```python
/out/target/product/mid713l_lp_lvds/obj/KERNEL_OBJ/vmlinux /proc
```
```python
/
```
```python
GNU gdb (Ubuntu
```
```python
7.7
```
```python
.1
```
```python
-
```
```python
0
```
```python
ubuntu5~
```
```python
14.04
```
```python
.2
```
```python
)
```
```python
7.7
```
```python
.1
```
```python
Copyright (C)
```
```python
2014
```
```python
Free Software Foundation, Inc
```
```python
.
```
```python
License GPLv3
```
```python
+
```
```python
: GNU GPL version
```
```python
3
```
```python
or
```
```python
later
```
```python
<
```
```python
http:
```
```python
//gnu.org/licenses/gpl.html>
```
```python
This is free software: you are free
```
```python
to
```
```python
change
```
```python
and
```
```python
redistribute it
```
```python
.
```
```python
There is NO WARRANTY,
```
```python
to
```
```python
the extent permitted
```
```python
by
```
```python
law
```
```python
.
```
```python
Type
```
```python
"show copying"
```
```python
and
```
```python
"show warranty"
```
```python
for details
```
```python
.
```
```python
This GDB was configured as
```
```python
"x86_64-linux-gnu"
```
```python
.
```
```python
Type
```
```python
"show configuration"
```
```python
for configuration details
```
```python
.
```
```python
For bug reporting instructions, please see:
```
```python
<
```
```python
http:
```
```python
//www.gnu.org/software/gdb/bugs/>.
```
```python
Find the GDB manual
```
```python
and
```
```python
other documentation resources online at:
```
```python
<
```
```python
http:
```
```python
//www.gnu.org/software/gdb/documentation/>.
```
```python
For help,
```
```python
type
```
```python
"help"
```
```python
.
```
```python
Type
```
```python
"apropos word"
```
```python
to
```
```python
search for commands related
```
```python
to
```
```python
"word"
```
```python
...
```
```python
Reading symbols from
```
```python
.
```
```python
/out/target/product/mid713l_lp_lvds/obj/KERNEL_OBJ/vmlinux
```
```python
...
```
```python
done
```
```python
.
```
```python
"/proc/"
```
```python
is
```
```python
not
```
```python
a core dump: Is a directory
(gdb) l
```
```python
*
```
```python
tpd_down
```
```python
0xc05ca0ac
```
```python
is
```
```python
in
```
```python
tpd_down (/home/weiqifa/weiqifa/new_tm100/tm100/kernel/mediatek/custom/out/kernel/touchpanel/mtk_gslX680
```
```python
.
```
```python
c:
```
```python
689
```
```python
)
```
```python
.
```
```python
684
```
```python
685
```
```python
}
```
```python
686
```
```python
#endif
```
```python
687
```
```python
688
```
```python
void
```
```python
tpd_down( int id, int x, int y, int p)
```
```python
689
```
```python
{
```
```python
690
```
```python
print_info(
```
```python
"============tpd_down id: %d, x:%d, y:%d============ \n"
```
```python
, id, x, y);
```
```python
691
```
```python
while
```
```python
(
```
```python
1
```
```python
);
```
```python
692
```
```python
input_report_key(tpd
```
```python
->
```
```python
dev, BTN_TOUCH,
```
```python
1
```
```python
);
```
```python
693
```
```python
input_report_abs(tpd
```
```python
->
```
```python
dev, ABS_MT_TOUCH_MAJOR,
```
```python
1
```
```python
);
(gdb)
```
## ２、在内核驱动里面加上一个while(1);系统会不会挂掉
上面的代码就是我加的while(1)代码。
linux内核是进程调度的，系统肯定没有崩溃，但是你加代码的位置，这个驱动已经用不了了。

