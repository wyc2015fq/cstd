# 重拾C#日常积累：in、ref、out类型标识的方法参数 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 16:25:31[boonya](https://me.csdn.net/boonya)阅读数：1299










为不具有 [in](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/in-parameter-modifier)、[ref](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/ref) 或 [out](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/out-parameter-modifier) 的方法声明的参数会按值传递给调用的方法。可以在方法中更改该值，但当控制传递回调用过程时，不会保留更改后的值。可以通过使用方法参数关键字更改此行为。

本部分介绍声明方法参数时可以使用的关键字：
- 
[params](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/params) 指定此参数采用可变数量的参数。

- 
[in](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/in-parameter-modifier) 指定此参数由引用传递，但只由调用方法读取。

- 
[ref](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/ref) 指定此参数由引用传递，可能由调用方法读取或写入。

- 
[out](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/out-parameter-modifier) 指定此参数由引用传递，由调用方法写入。



参考地址：[https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/method-parameters](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/method-parameters)






