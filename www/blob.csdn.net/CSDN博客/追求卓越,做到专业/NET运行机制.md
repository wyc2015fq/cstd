
# .NET运行机制 - 追求卓越,做到专业 - CSDN博客


2012年07月28日 09:50:01[Waldenz](https://me.csdn.net/enter89)阅读数：2203


**.NET Framework**是一个以规格为基础的开发平台。而这份规格统称为CLI(Comm Language Infrastructure)，而最初由MicroSoft依据这个规格所自行实现的产品就是CLR(Common Language Runtime)。在CLI规格中，又细分为四个子规格。CIL(Common intermidiate Language), CTS(Common Type System),
 CTS又细分为CLS(Common Language Specification)子规格和VES(Virtual Executing System). CLS包含大多数语言都能实现的类型，CLI规范支持CLI的程序语言至少必须满足CLS的规格，这样才能达到跨语言集成的目的。
**CLR运行机制**
支持CLI的程序语言必须拥有一个编译器，该编译器必须具备将源码编译成IL code的能力，也就是CIL规格所定义的语言格式，然后交由CLR来运行，在载入IL code 之后，CLR将调用JIT Complier(Just In Time Complier)将这些IL code 转化成Native code(机器码)来运行。
![CLI运作图](https://img-my.csdn.net/uploads/201207/28/1343439661_4987.png)

JIT是.NET程序运行的重要部件之一, 全程是即时编译器. .NET程序被加载入内存以后,但某段IL代码被第一次运行的时候,JIT编译器就会将这段IL代码全部编译成本地代码,然后再执行, 这也就是为什么.NET程序第一次运行都启动很慢的原因. 随.NET库, MS还附带了一个工具,可以实现将.NET程序所有的IL代码都编译成本地代码并保存在缓冲中,运行时也就可以脱离JIT了.

