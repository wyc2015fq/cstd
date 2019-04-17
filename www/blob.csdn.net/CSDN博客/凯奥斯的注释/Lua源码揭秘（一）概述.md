# Lua源码揭秘（一）概述 - 凯奥斯的注释 - CSDN博客





2017年10月28日 15:33:32[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：852
所属专栏：[Lua源码揭秘](https://blog.csdn.net/column/details/17856.html)









由于工作原因，需要撸一波lua源码。相关的文章有很多，其中就有云风未完成的[《Lua源码鉴赏》](https://www.codingnow.com/temp/readinglua.pdf)，以及[《Lua GC 的源码剖析》](https://blog.codingnow.com/2011/04/lua_gc_6.html)，也是我阅读源码过程中的重要参考。“纸上得来终觉浅，绝知此事要躬行”，于是开一个专栏，专门记录此间的心得。 

lua版本是5.1.5，(⊙o⊙)…我们项目里实际上使用的是luajit-2.1(⊙o⊙)…莫名的觉得在读luajit之前应当先读一下对应的原生lua版本，于是就走上了一条奇怪的岔路。~~（感觉我的技能点越点越偏了，以后还能找到工作吗？/(ㄒoㄒ)/）~~

# 文件划分
- 虚拟机运转的核心功能 
[lapi.c](http://blog.csdn.net/ecidevilin/article/details/78694915) C 语言接口 

ldebug.c Debug 接口 

ldo.c 函数调用以及栈管理 

lfunc.c 函数原型及闭包管理 

lgc.c 垃圾回收 

lmem.c 内存管理接口 

lobject.c 对象操作的一些函数 

lopcodes.c 虚拟机的字节码定义 

lstate.c 全局状态机 

lstring.c 字符串池 

ltable.c 表类型的相关操作 

ltm.c 元方法 

lvm.c 虚拟机 

lzio.c 输入流接口- 源代码解析以及预编译字节码 

lcode.c 代码生成器 

ldump.c 序列化预编译的Lua 字节码 

llex.c 词法分析器 

lparser.c 解析器 

lundump.c 还原预编译的字节码- 内嵌库 
[lauxlib.c](http://blog.csdn.net/ecidevilin/article/details/78694915) 库编写用到的辅助函数库 

lbaselib.c 基础库 

ldblib.c Debug 库 

linit.c 内嵌库的初始化 

liolib.c IO 库 
[lmathlib.c](http://blog.csdn.net/ecidevilin/article/details/78377365) 数学库 

loadlib.c 动态扩展库管理 

loslib.c OS 库 
[lstrlib.c](http://blog.csdn.net/ecidevilin/article/details/78460589) 字符串库 
[ltablib.c](http://blog.csdn.net/ecidevilin/article/details/78397419) 表处理库- 可执行的解析器，字节码编译器 

lua.c 解释器 

luac.c 字节码编译器- 其他 

print.c 打印，貌似是开发阶段辅助调试用的。

与云风的分类大致相同，针对不同版本做了些许调整（云风当时的Lua版本为5.2.2）。

*本文的最主要的作用是作为一个目录，方便索引后续文章，所以暂时未有太多原创内容~*







