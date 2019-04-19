# Linux下autoconf和automake使用 - 三少GG - CSDN博客
2013年02月27日 20:44:11[三少GG](https://me.csdn.net/scut1135)阅读数：711
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
[Linux下autoconf和automake使用](http://www.cnblogs.com/itech/archive/2010/11/28/1890220.html)
　　作为Linux下的程序开发人员，一定都遇到过Makefile，用make命令来编译自己写的程序确实是很方便。一般情况下，大家都是手工写一个简单Makefile，如果要想写出一个符合自由软件惯例的Makefile就不那么容易了. 在本文中，将介绍如何使用autoconf和automake两个工具来帮助我们自动地生成符合自由软件惯例的Makefile，这样就可以象常见的GNU程序一样，只要使用“./configure”，“make”，“make
 instal”就可以把程序安装到Linux系统中去了。这将特别适合想做开放源代码软件的程序开发人员，又或如果你只是自己写些小的Toy程序，那么这个文章对你也会有很大的帮助。
一、Makefile介绍
Makefile是用于自动编译和链接的，一个工程有很多文件组成，每一个文件的改变都会导致工程的重新链接，但是不是所有的文件都需要重新编译，Makefile中纪录有文件的信息，在make时会决定在链接的时候需要重新编译哪些文件。
Makefile的宗旨就是：让编译器知道要编译一个文件需要依赖其他的哪些文件。当那些依赖文件有了改变，编译器会自动的发现最终的生成文件已经过时，而重新编译相应的模块。
Makefile的基本结构不是很复杂，但当一个程序开发人员开始写Makefile时，经常会怀疑自己写的是否符合惯例，而且自己写的Makefile经常和自己的开发环境相关联，当系统环境变量或路径发生了变化后，Makefile可能还要跟着修改。这样就造成了手工书写Makefile的诸多问题，automake恰好能很好地帮助我们解决这些问题。
　　使用automake，程序开发人员只需要写一些简单的含有预定义宏的文件，由autoconf根据一个宏文件生成configure，由automake根据另一个宏文件生成Makefile.in，再使用configure依据Makefile.in来生成一个符合惯例的Makefile。下面我们将详细介绍Makefile的automake生成方法。
二、使用的环境
　　本文所提到的程序是基于Linux发行版本：Fedora Core release 1，它包含了我们要用到的autoconf，automake。
三、从helloworld入手
　　我们从大家最常使用的例子程序helloworld开始。
　　下面的过程如果简单地说来就是：
　　新建三个文件：　　helloworld.cconfigure.inMakefile.am
　　然后执行：autoscan; aclocal; autoconf; automake --add-missing; ./configure; make; ./helloworld;
　　就可以看到Makefile被产生出来，而且可以将helloworld.c编译通过。很简单吧，几条命令就可以做出一个符合惯例的Makefile，感觉如何呀。现在开始介绍详细的过程：
1、建目录
　　在你的工作目录下建一个helloworld目录，我们用它来存放helloworld程序及相关文件，如在/home/my/build下：
      $ mkdir helloword
      $ cd helloworld
2、 helloworld.c
　　然后用你自己最喜欢的编辑器写一个hellowrold.c文件，如命令：vi helloworld.c。使用下面的代码作为helloworld.c的内容。
　　int main(int argc, char** argv)
　　{
        　　printf("Hello, Linux World! ");
        　　return 0;
　　} 
　　完成后保存退出。现在在helloworld目录下就应该有一个你自己写的helloworld.c了。
3、生成configure
　　我们使用autoscan命令来帮助我们根据目录下的源代码生成一个configure.in的模板文件。
　　命令：
　　$ autoscan
　　$ ls
　　configure.scan helloworld.c 
　　执行后在hellowrold目录下会生成一个文件：configure.scan，我们可以拿它作为configure.in的蓝本。
4，生成configure.in
　  现在将configure.scan改名为configure.in，并且编辑它，按下面的内容修改，去掉无关的语句：
```
[]()
Code
```
