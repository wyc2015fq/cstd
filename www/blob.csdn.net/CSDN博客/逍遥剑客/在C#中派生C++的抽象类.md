# 在C#中派生C++的抽象类 - 逍遥剑客 - CSDN博客
2012年04月11日 09:56:46[xoyojank](https://me.csdn.net/xoyojank)阅读数：2178
目前看到了几种选择:
- [SWIG](http://www.swig.org/Doc1.3/CSharp.html#csharp_directors_implementation)- 这个叫做"director"的机制不是真正的派生, 只是看起来像而已
- C#里的类只是包装了一个C++对象的指针, 所有调用都是通过P/Invoke完成的, 实际上是两个不同的对象
- [CXXI](http://tirania.org/blog/archive/2011/Dec-19.html)- 通过改变C++对象的vtable实现派生
- 通过DLL(so)的导出方法名来取得函数指针
- 通过解析GCCXML来生成C++类的C#声明
- MSVC的支持还没有完成, 可以参考这个去完善: [Inheriting From a Native C++ Class in C#](http://blogs.msdn.com/b/vcblog/archive/2008/12/08/inheriting-from-a-native-c-class-in-c.aspx)
- [Hack]- 嵌入mono后可以通过C++类里调用C#那边同名类的method来实现
- 也是假的派生, 实际上是两个对象
- 可以在C++这边针对C#方法的调用做异常处理, 防止崩溃
