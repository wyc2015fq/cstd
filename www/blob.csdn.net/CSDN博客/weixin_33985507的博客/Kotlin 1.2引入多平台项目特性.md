# Kotlin 1.2引入多平台项目特性 - weixin_33985507的博客 - CSDN博客
2017年12月04日 18:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
[最新版本的Kotlin](%5C)通过多平台项目支持JVM和JavaScript平台的代码共享。除此之外，它还带来了语言和类库方面的大量改进，以及更好的编译器性能。
\\
Kotlin在1.1版本中就已引入了对[JavaScript的支持](%5C)，而最新版本的Kotlin让开发者可以很方便地管理在JVM和JavaScript平台上运行的项目。因为有了[Kotlin Native](%5C)，在未来还将支持本地二进制包。
\\
多平台项目（[multi-platform project](%5C)）的基本原理是按照一定的方式来组织项目模块，如一系列公共模块和平台特定模块。平台特定模块所包含的代码要么是运行在JVM上，要么是运行在JavaScript平台上，可以自由访问相应平台的包。每一个平台特定模块都需要一个公共模块，这些公共模块提供了所谓的“预期声明（expected declaration）”，平台特定模块必须实现这些声明。因为有了这种机制，平台独立代码可以调用平台特定代码。公共模块只能包含Kotlin代码，也只能使用Kotlin的包，而平台特定模块可以包含Java或JavaScript代码，它们可以被编译成特定的格式。
\\
下面是如何定义公共模块和相应平台模块的例子：
\\// Common module\\package org.jetbrains.foo\\expect class Foo(bar: String) {\    fun frob()\}\\fun main(args: Array\u0026lt;String\u0026gt;) {\    Foo(\"Hello\").frob()\}\\// JVM module\\package org.jetbrains.foo\\actual class Foo actual constructor(val bar: String) {\    actual fun frob() {\        println(\"Frobbing the $bar\")\    }\}\\
一些跨平台库为Kotlin在公共模块方面带来了更强大的能力，包括一个[序列化库](%5C)、一个[测试库](%5C)和一个[HTML同构库](%5C)。
\\
在语言方面，Kotlin 1.2为某些场景提供了改进的智能类型转换和类型推断，标准库中新增了kotlin.math包，序列和集合中也加入了新的标准库函数。
\\
Kotlin 1.2中的编译器变得更快了，比1.1要快25%。在后续的次要版本中，编译器性能将会有进一步的提升。
\\
Kotlin 1.2已经被绑定到[IntelliJ IDEA 2017.3](%5C)当中，可以从[官方的发布说明](%5C)中看到更多相关信息。
\\
**查看英文原文**：[Kotlin 1.2 Introduces Multi-Platform Projects](%5C)
