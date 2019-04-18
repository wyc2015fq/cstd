# Blazor将.NET带回到浏览器 - weixin_33985507的博客 - CSDN博客
2017年07月16日 18:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
由Steve Sanderson开发的[Blazor](%5C)尝试使用[WebAssembly](%5C)和DotNetAnywhere将.NET带回到浏览器。它不是要成为一个像Flash或Silverlight那样的完整生产框架，而是要探索一下有什么可能性。
\\
WebAssembly将自己描述成一个“内存安全的沙箱执行环境”标准，它“甚至可以在已有的JavaScript虚拟机中实现”。虽然WebAssembly在提供原生支持的浏览器中性能更好，但较旧的浏览器可以使用转换器和[asm.js](%5C)解释WebAssembly。
\\
目前，按照设计，WebAssembly主要为C和C++应用程序提供支持。它充当clang/LLVM的编译器后台。为了从C转到C#，Blazor使用了另外一个研究项目。由Chris Bacon开发的[DotNetAnywhere](%5C)是一个基于C面向.NET公共中间语言（即IL、CIL、MSIL）的解释器。DotNetAnywhere项目大约是6年前完成的。
\\
Blazor对DotNetAnywhere做了如下修改：
\\- 支持使用Emscripten构建；\\t
- 支持通过p/invoke在.NET中调用JavaScript；\\t
- 增加其他的互操作原语，如GCHandle；\\t
- 接收从JavaScript到.NET的入站调用；\\t
- 修复了一些Bug；\\t
- 支持加载.NET Core风格的程序集。\
Blazor应用程序使用Razor模板构建，在浏览器中执行，而不是服务器。据Razor文档介绍，一个简单的“hello world”应用程序需要300KB的下载。这包括“所有的东西：一个小型的.NET运行时、核心库、应用程序代码，以及引导和与WebAssembly代码互操作所需的封装库”。
\\
现在尚未剔除没有被实际调用的代码，因此，这个下载量还可以进一步减少。
\\
想要实际地看一下效果的话，请观看NDC大会视频“[Web应用无法真正地‘那样’做，它们能吗？——Steve Sanderson](%5C)”。
\\
**查看英文原文**：[Blazor Brings .NET Back to the Browser](%5C)
