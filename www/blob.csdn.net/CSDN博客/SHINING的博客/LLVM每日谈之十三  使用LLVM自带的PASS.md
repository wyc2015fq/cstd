# LLVM每日谈之十三   使用LLVM自带的PASS - SHINING的博客 - CSDN博客
2013年05月07日 11:12:05[snsn1984](https://me.csdn.net/snsn1984)阅读数：3987
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
PS：最近一段时间，投入在LLVM上的时间有些减少。差点把对它的研究断掉，今天开始继续。对LLVM的研究需要很长一段时间的坚持不懈才可以彻底搞明白。
前面已经介绍过如何写自己的PASS，并且也针对一个简单的PASS进行了分析和介绍。相信大家也可以从LLVM源码中找到很多的PASS，这些PASS太多，他们每个到底是做什么用？如何去调用这些系统已经有的PASS？这就是这次每日谈要关注的问题。
在文档 [http://llvm.org/docs/Passes.html](http://llvm.org/docs/Passes.html) 中，列出了所有的PASS的命令行参数以及这个PASS主要是做什么用的。下面来实际进行测试下。
在编译好的bin/目录下，使用clang生成一个.bc文件作为例子。然后使用opt工具：
./opt -print-module <test.bc > /dev/null
这里选择的是-print-module，对于它的介绍如下：
This pass simply prints out the entire module when it is executed.
这个文档中的其他Pass也是类似的。自己不但可以在这个列表里找到自己需要的Pass，也可以通过这个列表来熟悉这些Pass。另外这个列表中的Pass的源码主要在llvm源码/lib/Analysis和llvm源码/lib/Transforms两个目录下，大家可以根据pass的名字自己去找。
