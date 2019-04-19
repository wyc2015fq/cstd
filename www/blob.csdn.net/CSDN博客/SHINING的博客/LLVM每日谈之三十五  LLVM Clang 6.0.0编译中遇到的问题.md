# LLVM每日谈之三十五   LLVM/Clang 6.0.0编译中遇到的问题 - SHINING的博客 - CSDN博客
2018年07月19日 19:45:54[snsn1984](https://me.csdn.net/snsn1984)阅读数：726
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
随着LLVM/Clang版本的不断升级，LLVM/Clang也在慢慢的变得庞大起来，这使得编译其源码的时间和空间的需求也在极速扩张。这两天在虚拟机Ubuntu18.04上编译LLVM/Clang 6.0.0的时候就遇到了这样的问题。
首先，采用了传统的cmake 、make方式编译，没有添加任何参数。这种情况下，默认编译的是Debug模式，默认编译的目标平台是所有。编译时间长，这个无非是多等一下子。在编译中出现了编译Clang的时候，报文件太大的错误。根据网上所搜集到的信息，我扩充了SWAP的空间。同时，为了节省编译时间，给cmake传递了参数“-DLLVM_TARGETS_TO_BUILD=X86”，让这次编译只编译X86平台。
其次，在编译的过程中，依然出现了问题，还是在编译clang的时候，这次出现的问题是“x86_64-linux-gun-ld: failed to set dynamic section sizes: Memory exhausted”。四处查找了一下，在stackoverflow找到了同样的问题。地址：[https://stackoverflow.com/questions/25197570/llvm-clang-compile-error-with-memory-exhausted](https://stackoverflow.com/questions/25197570/llvm-clang-compile-error-with-memory-exhausted) 根据问题下的建议，我选择了给cmake传递参数“-DLLVM_USE_LINKER=gold”使用gold代替ld。这次顺利编译成功。
最后，如果不是非要Debug版本进行调试，可以直接给cmake 传递:
“-DCMAKE_BUILD_TYPE=Release   -DLLVM_TARGETS_TO_BUILD=X86    -DLLVM_USE_LINKER=gold ”这几个参数。这样可以直接编译只针对X86平台的Release版本，并且使用gold这个linker代替ld。这样既能节省时间，也不会因为空间的问题编译不通过。
