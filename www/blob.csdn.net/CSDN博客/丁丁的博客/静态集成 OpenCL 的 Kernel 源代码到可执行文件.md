
# 静态集成 OpenCL 的 Kernel 源代码到可执行文件 - 丁丁的博客 - CSDN博客


2015年06月23日 14:26:38[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：2385


在编写OpenCL代码时，为了方便起见，我们更喜欢将kernel源代码放在单独的文件中（一般为*.cl）。这各做的缺点在于，程序需要在运行时动态读入文件中的代码为字符串，然后再传递给OpenCL的RT编译、执行。因此，在可执行文件之外，我们还需要单独分发*.cl文件。
下面，我们介绍一种简单的技巧，将*.cl文件在编译时静态的包含在可执行文件中。
首先，将所有源代码由”STRINGFY()”括起来。
文件samefile.cl中
STRINGIFY(...// Lots of OpenCL code
)然后，在C/C++文件中，定义第一步用到的宏“STRINGFY()”，通过”include” 预编译命令将cl文件做为字符串（可能会很长）导入。
在文件somefile.cpp中
\#defineSTRINGIFY(src) \#srcinlineconstchar* Kernels() {staticconstchar* kernels =\#include "somefile.cl";returnkernels;
}这样kernel代码就被静态的编译到可执行文件中，无需单独分必。在需要用到kernel函数进行计算时，只需要将返回的字符串（这里为”Kernels“）传递给OpenCL相应的函数进行处理即可。
当然，我们可以同时添加更多的文件，只要每个cl文件都被”STRINGFY“括起来。C++编译器会自动完成字符串的拼接。
inlineconstchar* Kernels() {staticconstchar* kernels =\#include "utility_functions.cl"\#include "somefile.cl";returnkernels;
}对于较多短的kernel代码，我们也可以直接在cpp源文件中将其字符串化。
\#defineSTRINGIFY(src) \#srcstaticconstchar*
Kernel = STRINGIFY(// kernel definition);[1. stackoverflow讨论](http://stackoverflow.com/questions/6502173/preprocessor-tomfoolery-stringifying-a-include)
[2. BoostCompute实例](https://github.com/boostorg/compute/blob/master/include/boost/compute/utility/source.hpp)

