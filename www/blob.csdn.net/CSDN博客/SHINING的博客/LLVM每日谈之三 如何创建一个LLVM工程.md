# LLVM每日谈之三 如何创建一个LLVM工程 - SHINING的博客 - CSDN博客
2012年10月22日 15:53:03[snsn1984](https://me.csdn.net/snsn1984)阅读数：7358
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
阅读了文档《Creating an LLVM Project》（地址：http://llvm.org/docs/Projects.html）之后，自己照着做了一遍，记录一下自己的过程。
实际做的时候跟文档上还是有不同的。
1.首先我系统上的llvm3.1是之前已经下好，并且make过的。我打开llvm-3.1/projects目录，看到下面有一个我之前放的compiler-rt目录和另外一个本来这里就有的sample。把sample这个文件夹copy到llvm3.1的同级目录。修改文件夹名字为MyFirstLLVM。
2. 打开MyFirstLLVM/autoconf目录，打开configure.ac.
3. AC_INIT后面的括号里面有三个中括号，第一个中括号要填的是这个项目的名字，第二个是这个项目的版本号，第三个是作者的联系邮箱，可改可不改。
4. 因为现在MyFirstLLVM已经和llvm-3.1在同级目录了，所以修改接下来的LLVM_SRC_ROOT="../../llvm-3.1" LLVM_OBJ_ROOT="../../llvm-3.1/build"。llvm-3.1/build目录是我编译llvm-3.1的目录。
5. 修改AC_CONFIG_AUX_DIR后面的内容为：(["../../llvm-3.1/autoconf"])。
6. 然后保存，在terminal中运行./AutoRegen.sh。这时候系统提示：Your autoconf was not detected as being 2.5x or 2.6x。
7. 查了下，是机器上没装autoconf的缘故，使用sudo apt-get install autoconf安装之后，就可以正常运行了。
8. 然后在MyFirstLLVM目录之下，建立一个build目录，在build目录下运行../configure。
9. 在build目录下运行make，结束之后可以看到build目录下有一个Release+Asserts目录，打开这个目录，可以看到lib和bin两个目录。
10. 在bin目录之下运行./Sample得到一个输出的结果5。
这样就已经完成了一个LLVM工程的建立。这是建立LLVM工程的两种方式之一，另外一种方式就是从零开始，自己一点一点建立，这样相对来说会复杂点。
PS：
建立LLVM工程和编译LLVM是两回事，不要搞混了。
