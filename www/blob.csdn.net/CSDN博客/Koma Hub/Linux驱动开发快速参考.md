# Linux驱动开发快速参考 - Koma Hub - CSDN博客
2019年02月26日 22:57:04[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：9
个人分类：[Linux kernel																[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)](https://blog.csdn.net/Rong_Toa/article/category/7348890)
本节总结了我们在本章接触到的内核函数, 变量, 宏定义, 和 /proc 文件. 它的用意是作为一个参考. 每一项列都在相关头文件的后面, 如果有. 从这里开始, 在几乎每章的结尾会有类似一节, 总结一章中介绍的新符号. 本节中的项通常以在本章中出现的顺序排列:
用户空间工具, 加载模块到运行中的内核以及去除它们.
> - insmod
- modprobe
- rmmod
指定模块的初始化和清理函数的宏定义.
```cpp
#include <linux/init.h>
module_init(init_function);
module_exit(cleanup_function);
```
> - __init
- __initdata
- __exit
- __exitdata
函数( __init 和 __exit )和数据 (__initdata 和 __exitdata)的标记, 只用在模块初始化或者清理时间. 为初始化所标识的项可能会在初始化完成后丢弃; 退出的项可能被丢弃如果内核没有配置模块卸载. 这些标记通过使相关的目标在可执行文件的特定的 ELF 节里被替换来工作.
```cpp
#include <linux/sched.h>
```
最重要的头文件中的一个. 这个文件包含很多驱动使用的内核 API 的定义, 包括睡眠函数和许多变量声明.
```cpp
struct task_struct *current;
```
当前进程.
```cpp
current->pid
current->comm
```
进程 ID 和 当前进程的命令名.
> - obj-m
一个 makefile 符号, 内核建立系统用来决定当前目录下的哪个模块应当被建立.
> 
/sys/module
/proc/modules
/sys/module 是一个 sysfs 目录层次, 包含当前加载模块的信息. /proc/moudles是旧式的, 那种信息的单个文件版本. 其中的条目包含了模块名, 每个模块占用的内存数量, 以及使用计数. 另外的字串追加到每行的末尾来指定标志, 对这个模块当前是活动的.
> 
vermagic.o
来自内核源码目录的目标文件, 描述一个模块为之建立的环境.
> 
#include <linux/module.h>
必需的头文件. 它必须在一个模块源码中包含.
> 
#include <linux/version.h>
头文件, 包含在建立的内核版本信息.
> 
LINUX_VERSION_CODE
整型宏定义, 对 #ifdef 版本依赖有用.
> 
EXPORT_SYMBOL (symbol);
EXPORT_SYMBOL_GPL (symbol);
宏定义, 用来输出一个符号给内核. 第 2 种形式输出没有版本信息, 第 3 种限制输出给 GPL 许可的模块.
> 
MODULE_AUTHOR(author);
MODULE_DESCRIPTION(description);
MODULE_VERSION(version_string);
MODULE_DEVICE_TABLE(table_info);
MODULE_ALIAS(alternate_name);
放置文档在目标文件的模块中.
> 
module_init(init_function);
module_exit(exit_function);
宏定义, 声明一个模块的初始化和清理函数.
> 
#include <linux/moduleparam.h>
module_param(variable, type, perm);
宏定义, 创建模块参数, 可以被用户在模块加载时调整( 或者在启动时间, 对于内
嵌代码). 类型可以是 bool, charp, int, invbool, short, ushort, uint, ulong,
或者 intarray.
> 
#include <linux/kernel.h>
int printk(const char * fmt, ...);
内核代码的 printf 类似物.

