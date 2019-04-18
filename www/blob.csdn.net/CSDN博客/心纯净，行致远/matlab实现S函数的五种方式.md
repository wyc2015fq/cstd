# matlab实现S函数的五种方式 - 心纯净，行致远 - CSDN博客





2018年03月07日 22:03:02[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：1796







Matlab允许你使用以下五种方式之一来实现S函数：

　　
A Level-1 M-file S-function provides a simple M interface to interact with a small portion of the S-function API. [Level-2](https://www.baidu.com/s?wd=Level-2&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YzPjF9njb4mWNWmhw-njDL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHTvP10YPH0kPjcLPj64rjcvn0) M-file S-functions supersede Level-1 M-file S-functions.
　　Level 1 M文件S函数----这种方式提供了一个简单的M文件接口，可以与少部分的S函数API交互。Matlab对于这种方式的支持更多的是为了保持与以前版本的兼容，现在推荐采用的是Level 2 M文件S函数。
　　
A [Level-2](https://www.baidu.com/s?wd=Level-2&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YzPjF9njb4mWNWmhw-njDL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHTvP10YPH0kPjcLPj64rjcvn0) M-file S-function provides access to a more extensive set of the S-function API and supports code generation. In most cases, use a [Level-2](https://www.baidu.com/s?wd=Level-2&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YzPjF9njb4mWNWmhw-njDL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHTvP10YPH0kPjcLPj64rjcvn0) M-file S-function when you want to implement your S-function in M.
　　Level 2 M文件S函数----支持访问更多地S函数API，支持代码生成。当你要使用M文件来实现一个S函数的时候，选择Level 2 M文件S函数。

A handwritten C MEX S-function provides the most programming flexibility. You can implement your algorithm as a C MEX S-function or write a wrapper S-function to call existing C, C++, or Fortran code. Writing a new S-function requires knowledge of the S-function API and, if you want to generate inlined code for the S-function, the Target Language Compiler (TLC).
　　手写C MEX S函数----这种方式提供了最大的编程自由度，你可以使用C MEX S函数来实现你的算法，也可以写一个wrapper S函数，调用已经存在的C，C++，Fortran代码。使用这种方式编写一个新的S函数需要对于S函数API的了解，如果你写的S函数要产生内嵌式代码，还需要熟悉TLC。

The S-Function Builder is a graphical user interface for programming a subset of S-function functionality. If you are new to writing C MEX S-functions, you can use the S-Function Builder to generate new S-functions or incorporate existing C or C++ code without interacting with the S-function API. The S-Function Builder can also generate TLC files for inlining your S-function during code generation with the Real-Time Workshop product.
　　S函数创建器----这是采用图形用户界面的方式实现部分S函数功能的方式。如果你对于编写C MEX S函数不熟悉，可以使用S函数创建器来产生新的S函数，或者将已有的C,C++代码集成到S函数中而不需要与S函数API打交道。S函数创建器可以同时生成TLC文件，这些TLC文件可以在自动代码生成时为你的S函数生成内嵌代码。

The Legacy Code Tool is a set of MATLAB commands that helps you create an S-function to incorporate legacy C or C++ code. Like the S-Function Builder, the Legacy Code Tool can generate a TLC file to inline your S-function during code generation. The Legacy Code Tool provides access to fewer of the methods in the S-function API than the S-Function Builder or a handwritten C MEX S-function.
　　旧代码工具---是一系列的Matlab命令，这些命令帮助你集成以前的C，C++代码以生成一个S函数。与S函数创建器一样，旧代码工具可以产生TLC文件，不过相比于S函数创建器和手写的C MEX S函数，旧代码工具只能访问很少的S函数API。


