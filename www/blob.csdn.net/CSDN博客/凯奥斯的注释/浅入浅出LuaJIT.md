# 浅入浅出LuaJIT - 凯奥斯的注释 - CSDN博客





2017年11月25日 10:18:06[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：11768








# JIT

## 什么是JIT

JIT = Just In Time即时编译，是动态编译的一种形式，是一种优化虚拟机运行的技术。 

程序运行通常有两种方式，一种是静态编译，一种是动态解释，即时编译混合了这二者。[Java](https://www.ibm.com/support/knowledgecenter/SSYKE2_7.0.0/com.ibm.java.zos.70.doc/diag/understanding/jit_overview.html)和.Net/mono中都使用了这种技术。 

然而IOS中禁止使用（不是针对JIT，而是所有的动态编译都不支持）！
## 为什么要使用JIT

解释执行：
- 效率低。
- 代码暴露。

静态编译：
- 不够灵活，无法热更新。
- 平台兼容性差。

JIT：
- 效率：高于解释执行，低于静态编译。
- 安全性：一般都会先转换成字节码。
- 热更新：无论源码还是字节码本质上都是资源文件。
- 兼容性：虚拟机会处理平台差异，对用户透明。

## JIT是如何实现的

这里讲的实际上是JIT的一个变种：自适应动态编译（adaptive dynamic compilation）。它分为两种：Method JIT和Trace JIT。 

如图所示，这是jvmjit的流程： 
![这里写图片描述](https://img-blog.csdn.net/20171125094629206)

简单来讲：- 跟踪热点函数或trace，编译成机器码执行，并缓存起来供以后使用。
- 非热点函数解释执行。

为什么只编译热点函数？ 

对只执行一次的代码而言，解释执行其实总是比JIT编译执行要快。对这些代码做JIT编译再执行，可以说是得不偿失。而对只执行少量次数的代码，JIT编译带来的执行速度的提升也未必能抵消掉最初编译带来的开销。只有对频繁执行的代码，JIT编译才能保证有正面的收益。

# LuaJIT

## vs. Lua

Lua主要由以下三部分组成：
- 语法实现。
- 库函数。
- 字节码。

LuaJIT主要由以下四部分组成：
- 语法实现。
- Trace JIT编译器。
- 库函数。 
- 原生库++（强化过的原生库）
- bit
- ffi
- jit 

- 字节码。

注：最新luajit对应lua5.1.5。

## trace jit编译器

![这里写图片描述](https://img-blog.csdn.net/20171125095635767)

 与jvmjit大致相同。 

所谓trace便是一段线性的字节码序列。热点trace被编译成机器码，非热点trace解释执行。 

注：并不是所有的代码都能被JIT。（[NYI](http://wiki.luajit.org/NYI)）
## bytecode

bytecode基本上可以认为是虚拟机的指令码（“基本上”是因为luajit使用了[uleb128](http://blog.csdn.net/roland_sun/article/details/46708061)）。 

优点：
- 减少文件大小。 
- 生成函数原型更快。 
- 增加被破解的难度。 
- 对源代码轻微的[优化](http://wiki.luajit.org/Optimizations)。

## 库函数和可执行文件

编译步骤分三步走： 
![这里写图片描述](https://img-blog.csdn.net/20171125100500409)- minilua：实际上是lua原生代码的一个子集，用来执行lua脚本并生成平台相关的指令。
- buildvm：用来生成操作码/库函数到汇编/C语言的映射，用来jit编译。
- lib
- exec：可以执行lua代码活转换字节码。

## 编码

命令行执行
`luajit –b <in> <out>。`
虚拟机会判断是否是字节码，所以无需做额外的操作。 

另外，可以混用，即：一部分文件编成字节码，另一部分保持源代码。

## iOS64位报错问题

Cannot load incompatible bytecode！ 

这个错是因为在luajit里使用gcr用来比较对象指针，在64位环境下只有47位有效值（默认用户内存不会超过128T）。其余17位中有4位保存对象类型，即一段内存中保存了两条信息。所以在函数栈操作中有些地方需要一个空值占位。因为字节码直接反映了函数栈操作，所以64位和32位字节码不同。

# 参考文献

[https://www.zhihu.com/question/37389356](https://www.zhihu.com/question/37389356)
[https://zhuanlan.zhihu.com/hllvm/19977592](https://zhuanlan.zhihu.com/hllvm/19977592)
[http://wiki.luajit.org](http://wiki.luajit.org)
[用好Lua+Unity，让性能飞起来——Lua与C#交互篇](https://blog.uwa4d.com/archives/USparkle_Lua.html)
[在windows程序中嵌入Lua脚本引擎–使用VS IDE编译Luajit脚本引擎](http://blog.csdn.net/breaksoftware/article/details/8188269)
[Luajit-2.1.0-beta1的发布和生成arm64用bytecode的解脱](https://www.cnblogs.com/quicklyslow/p/4794430.html)
[Peeking inside LuaJIT](https://pwparchive.wordpress.com/2012/10/16/peeking-inside-luajit/)







