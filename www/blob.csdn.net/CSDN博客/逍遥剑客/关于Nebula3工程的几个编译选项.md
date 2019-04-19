# 关于Nebula3工程的几个编译选项 - 逍遥剑客 - CSDN博客
2008年10月15日 23:05:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1847
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
研究一下人家是怎么通过编译选项来优化性能的
**DEBUG:**
|C++/Code Generation/Enable String Pooling: Yes (/GF)|[该选项使编译器能够为执行过程中程序映像和内存中的相同字符串创建单个副本，从而得到较小的程序，这种优化称为字符串池](http://msdn.microsoft.com/zh-cn/library/s0s0asdt(VS.80).aspx)|
|----|----|
|C++/Code Generation/Enable C++ Exceptions: No|[禁用C++异常](http://msdn.microsoft.com/zh-cn/library/d42ws1f6(VS.80).aspx)|
|C++/Code Generation/Enable Function-Level Linking: Yes (/Gy)|[此选项允许编译器以封装函数 (COMDAT) 的形式将各个函数打包](http://msdn.microsoft.com/zh-cn/library/xsa71f43(VS.80).aspx)|
|C++/Code Generation/Enable Enhanced Instruction Set: Streaming SIMD Extensions (/arch:SSE)|[使用流式处理 SIMD 扩展 (SSE)  指令指定生成代码的结构](http://msdn.microsoft.com/zh-cn/library/7t5yh4fd(VS.80).aspx)|
|C++/Code Generation/Floating Point Model: Fast (/fp:fast)|[指定源代码文件中的浮点行为: 在多数情况下，创建执行速度最快的代码](http://http//msdn.microsoft.com/zh-cn/library/e7s85ffb(VS.80).aspx)|
|C++/Advanced/Calling Convertion: __fastcall (/Gr)|[为除 C++ 成员函数和标记为 **__cdecl** 或 **__stdcall** 的函数以外的所有函数指定 **__fastcall **调用约定](http://msdn.microsoft.com/zh-cn/library/46t77ak2(VS.80).aspx)|
**RELEASE:**
|C++/Optimization/Inline Function Expansion: Any Suitable (/Ob2)|[只展开标记为 inline, __inline, __forceinline 或 __inline 的函数，或在类声明内定义的 C++ 成员函数中的函数](http://msdn.microsoft.com/zh-cn/library/47238hez(VS.80).aspx)|
|----|----|
|C++/Optimization/Enable Instrinsic Fuctions: Yes (/Oi)|[用有助于应用程序更快运行的内部函数或其他特殊形式的函数替换某些函数调用](http://msdn.microsoft.com/zh-cn/library/f99tchzc(VS.80).aspx)|
|C++/Optimization/Favor Size or Speed: Favor Small Code (/Os)|[（代码大小优先）通过指示编译器优选大小而非速度来最小化 EXE 和 DLL 的大小。编译器可以将许多 C 和 C++ 构造缩小为功能类似的机器码序列。这些差异有时在大小和速度之间提供了折中](http://msdn.microsoft.com/zh-cn/library/f9534wye(VS.80).aspx)|
|C++/Optimization/Omit Frame Pointers: Yes (/Oy)|[此选项可以加快函数调用的速度，因为无需设置和移除任何框架指针。它还可以使一个或多个寄存器（Intel 386 或更高版本中的 EBP）空闲出来，将其用于存储频繁使用的变量和子表达式](http://msdn.microsoft.com/zh-cn/library/2kxx5t2c(VS.80).aspx)|
|C++/Code Generation/Enable String Pooling: Yes (/GF)|[该选项使编译器能够为执行过程中程序映像和内存中的相同字符串创建单个副本，从而得到较小的程序，这种优化称为字符串池](http://msdn.microsoft.com/zh-cn/library/s0s0asdt(VS.80).aspx)|
|C++/Code Generation/Enable C++ Exceptions: No|[禁用C++异常](http://msdn.microsoft.com/zh-cn/library/d42ws1f6(VS.80).aspx)|
|C++/Code Generation/Buffer Security Check: No (/GS-)|[不检测某些改写返回地址的缓冲区溢出，这是一种利用不强制缓冲区大小限制的代码的常用技术](http://msdn.microsoft.com/zh-cn/library/8dbf701c(VS.80).aspx)|
|C++/Code Generation/Enable Enhanced Instruction Set: Streaming SIMD Extensions (/arch:SSE)|[使用流式处理 SIMD 扩展 (SSE)  指令指定生成代码的结构](http://msdn.microsoft.com/zh-cn/library/7t5yh4fd(VS.80).aspx)|
|C++/Code Generation/Floating Point Model: Fast (/fp:fast)|[指定源代码文件中的浮点行为: 在多数情况下，创建执行速度最快的代码](http://http//msdn.microsoft.com/zh-cn/library/e7s85ffb(VS.80).aspx)|
|C++/Advanced/Calling Convertion: __fastcall (/Gr)|[为除 C++ 成员函数和标记为 **__cdecl** 或 **__stdcall** 的函数以外的所有函数指定 **__fastcall **调用约定](http://msdn.microsoft.com/zh-cn/library/46t77ak2(VS.80).aspx)|
/Os好像是为了适应console和移动设备而选择了大小优先, 我觉得在PC平台上可以改成速度优先
不过对于频繁更新的程序来说(如网络游戏), 还是大小优先吧
