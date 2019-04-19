# LLVM每日谈之二十五  Clangd和LSP - SHINING的博客 - CSDN博客
2017年11月24日 09:33:45[snsn1984](https://me.csdn.net/snsn1984)阅读数：1465
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
1、Clangd
Clang的Extra Clang Tools中有一个ClangD。按照ClangD的定义，它是对Language Server Protocol的一个具体实现（当然是建立在Clang的基础之上的），目的是为了提供编程语言的一些智能化的特性，比如代码完成、引用查找等，主要面向的终端是C/C++的编辑器。
Clangd的官方定义：
**Clangd** is an implementation of the [**Language Server Protocol**](https://github.com/Microsoft/language-server-protocol) leveraging Clang. Clangd’s goal is to provide language “smartness” features like code completion, find references, etc. for clients such as C/C++ Editors.
Clangd官方文档地址：[http://releases.llvm.org/5.0.0/tools/clang/tools/extra/docs/clangd.html](http://releases.llvm.org/5.0.0/tools/clang/tools/extra/docs/clangd.html)
2、LSP,Language Server Protocol
Language Server Protocol是一个协议，一个语言服务协议，主要用于编辑器和语言服务器之间进行交互的协议。这个协议的消息主要分为Request、Resonse、Notification三种类型。按照之前Clangd的定义，它就是对该协议的一个具体实现。
关于该协议的详细背景和介绍，wiki地址：[https://en.wikipedia.org/wiki/Language_Server_Protocol](https://en.wikipedia.org/wiki/Language_Server_Protocol)
关于该协议，还专门有一个网站：[http://langserver.org/](http://langserver.org/)
3、Clangd和LSP
按照上面的描述，Clangd既然是说对LSP协议的一个实现，那么它应该是夹在Editor和Server之间的一个部分。但是实际上，在[http://langserver.org/](http://langserver.org/)上，Clangd被划在了Language Servers这一类型里面。仔细分析，也不无道理。Clangd显然不是只对协议进行了实现，而是基于Clang对LSP和Server都进行了实现，只需要前端对接Editor就可以了。
在[http://langserver.org/](http://langserver.org/)上面，也没有进行三个阶段的划分，而都是直接分为了Language Servers和LSP clients两类。猜测一下，应该是没有人专门去做协议的实现，本身意义不大，更多的是Language Servers本身的实现，然后支持协议。或者是Editor本身去实现协议，支持协议直接对接Language。
