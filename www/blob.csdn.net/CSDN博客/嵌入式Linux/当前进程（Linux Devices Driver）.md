
# 当前进程（Linux Devices Driver） - 嵌入式Linux - CSDN博客

2015年11月16日 14:45:11[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：397个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



尽管内核模块不像应用程序一样顺序执行，内核做的大部分动作是代表一个特定进程的，内核代码可以引用当前进程，通过存取全局项current,它在asm/current.h中定义，它产生一个指针指向结构task_struct,在linux/sched.h定义，current指针指向当前在运行的进程，在一个系统调用执行期间，例如open 或者read,当前进程是发出调用的进程，内核代码可以通过使用current来使用进程特定的信息，如果它需要这样，这种技术的一个例子在第六章展示。
实际上, current 不真正地是一个全局变量. 支持 SMP 系统的需要强迫内核开发者去开发一种机制, 在相关的 CPU 上来找到当前进程. 这种机制也必须快速, 因为对 current 的引用非常频繁地发生. 结果就是一个依赖体系的机制, 常常, 隐藏了一个指向task_struct 的指针在内核堆栈内. 实现的细节对别的内核子系统保持隐藏, 一个设备驱动可以只包含linux/sched.h 并且引用当前进程. 例如, 下面的语句打印了当前进程的进程 ID 和命令名称, 通过存取结构 task_struct 中的某些字段.printk(KERN_INFO “The process is \”%s\” (pid %i)\n”, current->comm, current->pid);存于 current->comm 的命令名称是由当前进程执行的程序文件的基本名称( 截短到 15 个字符, 如果需要 ).
代码修改如下：
```python
--- a/mediatek/custom/mid713l_lp_lvds/kernel/touchpanel/GSLX68
```
```python
0
```
```python
/mtk_gslX68
```
```python
0
```
```python
.c
+++ b/mediatek/custom/mid713l_lp_lvds/kernel/touchpanel/GSLX68
```
```python
0
```
```python
/mtk_gslX68
```
```python
0
```
```python
.c
```
```python
@@
```
```python
-
```
```python
687
```
```python
,
```
```python
8
```
```python
+
```
```python
687
```
```python
,
```
```python
8
```
```python
@@
```
```python
static void record_point(u16
```
```python
x
```
```python
, u16
```
```python
y
```
```python
, u8 id)
 void tpd_down(
```
```python
int
```
```python
id,
```
```python
int
```
```python
x
```
```python
,
```
```python
int
```
```python
y
```
```python
,
```
```python
int
```
```python
p) 
 {
-       print_info(
```
```python
"============tpd_down id:
```
```python
%d
```
```python
, x:
```
```python
%d
```
```python
, y:
```
```python
%d
```
```python
============ \n"
```
```python
, id,
```
```python
x
```
```python
,
```
```python
y
```
```python
);
-
+       printk(
```
```python
"============tpd_down id:
```
```python
%d
```
```python
, x:
```
```python
%d
```
```python
, y:
```
```python
%d
```
```python
============ \n"
```
```python
, id,
```
```python
x
```
```python
,
```
```python
y
```
```python
);
+       printk(
```
```python
"weiqifa The process is -
```
```python
%s
```
```python
pid -
```
```python
%i
```
```python
\n"
```
```python
,current->comm,current->pid);
        input_report_key(tpd->dev, BTN_TOUCH,
```
```python
1
```
```python
);
        input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR,
```
```python
1
```
```python
);
        input_report_abs(tpd->dev, ABS_MT_POSITION_X,
```
```python
x
```
```python
);
weiqifa
```
```python
@weiqifa
```
```python
-Inspiron-
```
```python
3847
```
```python
:~
```
```python
/weiqifa/new
```
```python
_tm10
```
```python
0
```
```python
/tm10
```
```python
0
```
```python
$
```
日志打印如下：
```python
<
```
```python
6
```
```python
>
```
```python
[   92.546337]-(2)[[FH]] dis ok
```
```python
<
```
```python
6
```
```python
>
```
```python
[   92.546353]-(2)[Power/clkmgr] [mt
```
```python
_disable_
```
```python
clock]: id=50, names=MFG
()mknt[oetsce
```
```python
_ls[84 <4>[   92.605413] (3)============tpd_
```
```python
down id: 1, x:683, y:210============
```
```python
<
```
```python
4
```
```python
>
```
```python
[   92.605443] (3)weiqifa The process is -mtk-tpd pid -104
223 1slc i.rnsolr 94,omsoeaj10,omaj1,sz 43 okl
4[
```
```python
9.227 1Klig'dpitpoe'(8) o_cr_d 00(o_d 8,<>  2650
```
```python
](
```
```python
) t re272Bo eafo dodluce3 98 e  92.650]() cce622Bi eo ii 91k o o
```
```python
_cr_
```
```python
d 00
```
```python
<>
```
```python
2651]() Fe eoyi 70k bv eevd[f(x0d)
```
```python
>  2654]()98dodluce3 i  o[8:dpitpoe]<>  2655]()98G]ei
```
MTk平台的不好之处在于，你什么操作都没有，都会打印一大堆日志出来，我是真心搞不懂。

