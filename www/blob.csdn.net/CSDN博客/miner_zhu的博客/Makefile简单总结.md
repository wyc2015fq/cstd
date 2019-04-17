# Makefile简单总结 - miner_zhu的博客 - CSDN博客





2018年10月21日 21:23:58[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：34








**目录**



[一、引入](#%E4%B8%80%E3%80%81%E5%BC%95%E5%85%A5)

[二、Makefile简介](#%E4%BA%8C%E3%80%81Makefile%E7%AE%80%E4%BB%8B)

[三、Makefile基本结构](#%E4%B8%89%E3%80%81Makefile%E5%9F%BA%E6%9C%AC%E7%BB%93%E6%9E%84)

[四、简单的Makefile实现](#%E5%9B%9B%E3%80%81%E7%AE%80%E5%8D%95%E7%9A%84Makefile%E5%AE%9E%E7%8E%B0)

[参考文章](#%E5%8F%82%E8%80%83%E6%96%87%E7%AB%A0)

# 一、引入

比如我们需要在linux中运行一个test.c文件，shell命令的常见方法是：

1.进入所在目录，编译文件中已有的c文件test.c.
`gcc test.c -o test`
2.编译完成，生成了名为test的可执行文件，直接运行
`./test`
我们可以发现这样需要多次命令操作，当需要执行多个c文件的时候，显然造成了精力的分散和浪费，也增大了出错的可能性。

因此我们引入makefile，makefile是用来编译链接文件的，实现了自动化编译。与make指令搭配，一条make指令就完成了整个工程完全自动编译，极大的提高了软件开发的效率。

在编程的时候，我们也可以把一个完整程序的每个函数分离出来，写成.c文件，最后再一起编译和链接。这样有利于程序功能模块化，也方便检查代码错误。

# 二、Makefile简介

makefile关系到了整个工程的编译规则。一个工程中的源文件不计数，其按**类型、功能、模块**分别放在若干个目录中，makefile定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作，因为makefile就像一个Shell脚本一样，其中也可以执行[**操作系统**](http://lib.csdn.net/base/operatingsystem)的命令。makefile带来的好处就是——“自动化编译”，一旦写好，只需要一个make命令，整个工程完全**自动编译**，极大的提高了软件开发的效率。make是一个命令工具，是一个解释makefile中指令的命令工具，一般来说，大多数的IDE都有这个命令，比如：Delphi的make，Visual C++的nmake，[**Linux**](http://lib.csdn.net/base/linux)下GNU的make。

我们可以简单的把makefile认为是一份**定义了源文件间依赖关系、如何编译各个源文件并生成可执行文件**的说明书。

# 三、Makefile基本结构

```
TARGET... : PREREQUISITES...
    COMMAND
    ...
    ...
```

TARGET：规则的目标，最终生成文件的名字或者是中间过程文件名，也可以是make执行的动作的名称。

PREREQUISITES：规则的依赖，生成目标所必须的文件名列表。

COMMAND：规则的命令。规则需要执行的动作

**注意**：makefile中使用的必须是TAB键，使用空格会报错！

# 四、简单的Makefile实现

我们把开头提到的案例用Makefile进行简单实现。

1.创建Makefile 文件。命令行输入**vim Makefile**命令

![](https://img-blog.csdn.net/2018102120412791?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.编译执行。命令行输入**make**命令，生成文件。
`./test  //执行程序`
![](https://img-blog.csdn.net/20181021204944717?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里简单说明一下在Makefile文件中一定要将每一个.c文件按执行顺序先编译成.o文件，再按顺序将.o文件编译成可执行文件。而每次编译过后会产生很多的.o文件，对于程序运行没什么太大意义，反而会占内存，所以我们也可以在Makefile文件中添加清除命令（clean），如:

```
.PHONY:clean
clean:                删除所有文件类型为.o的文件
    rm  –rf   *.o
```

** .PHONY:clean**

PHONY:目标并非实际的文件名，只是显示在请求时执行命令的名字。一般有两大理由使用PHONY目标：避免和文件名冲突，改善性能。如果有.PHONY:clean，外面也有clean文件，执行make clean时，里面的.o文件会被删除而外面的clean文件还在。确保了外面clean文件的安全性。如果没有.PHONY:clean 语句，外面也没有clean文件时，在执行make clean也会删除.o文件，如果外面有clean，则会报错。



# 参考文章

[makefile学习之路——makefile简介](http://blog.51cto.com/12876518/2088329)

[跟我一起写Makefile](https://seisman.github.io/how-to-write-makefile/index.html)​​​​​​​

[Linux下多个.c文件的编译和Makefile文件](https://www.cnblogs.com/wanglog/p/5555669.html)

需要更详细的Makefile文章推荐参考[跟我一起写Makefile](https://seisman.github.io/how-to-write-makefile/index.html)，内容特别详实。



