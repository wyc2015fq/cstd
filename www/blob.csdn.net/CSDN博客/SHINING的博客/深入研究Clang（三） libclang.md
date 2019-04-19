# 深入研究Clang（三） libclang - SHINING的博客 - CSDN博客
2014年05月15日 14:22:27[snsn1984](https://me.csdn.net/snsn1984)阅读数：12406
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
                
作者：史宁宁
现在的Clang，不仅仅是一个编译器前端，同时也可以作为一个库使用。作为一个库使用的时候，可以用它去分析C/C++/ObjectC语言代码，可以分析源码得到AST，也可以获取已经分析好的AST，也可以遍历AST，还可以获取AST中基本元素的物理源码位置。这就是libclang。
libclang提供了一系列的C语言的接口，但是这些接口并不能完全提供存储在Clang C++ AST中的所有信息，只能提供部分基本信息，但是这些基本信息已经可以满足一般情况下的使用。主要目的是为了稳定，并且可以支持开发工具的基本功能。（clang/include/clang-c/Index.h中的注释原文如下： This C interface to Clang will never provide all of the information representation stored in Clang's
 C++ AST, nor should it: the intent is to maintain an API that is relatively stable from one release to the next, providing only the basic functionality needed to support development tools.）
与此同时，libclang支持和python绑定使用，也就是说你可以是用python去调用libclang。clang在clang/bindings/python/这个目录下面提供了绑定所需要的文件。目前这个目录下面除了README之外还有三个目录：clang，examples，tests。其中clang目录下面的文件是绑定所需要的，examples目录下面是绑定后测试用的两个小例子。test目录下面应该是测试需要的一些文件（该目录下面文件目前还没有认真分析）。只需要把绑定文件放到python的库文件下面（包含clang文件夹），然后确保系统安装clang到系统目录，之后就可以运行例子了。
libclang带来的优点很明显，可以在很多不同的环境下面轻松的用上libclang，只是作为一个提供了parser功能的库，非常方便快捷的嵌入自己的工具中，避免了大量的重复工作。同时也不用去分析clang内部的代码，不需要熟悉内部大量的AST相关的代码，就可以使用，这可以节省大量的时间。当然，想深挖clang的代码的同学，也可以把这里当作一个突破口。
------------------------------------------------------------转载请注明出处--------------------------------------------------------------------------------------
