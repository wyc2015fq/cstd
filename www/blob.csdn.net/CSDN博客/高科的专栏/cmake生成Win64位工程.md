
# cmake生成Win64位工程 - 高科的专栏 - CSDN博客

2019年01月04日 11:13:18[高科](https://me.csdn.net/pbymw8iwm)阅读数：400


使用cmake编译64的dll
一开始使用cmake --build .来生成了dll，在导入到java项目中使用的时候，才发现是32位的。导致程序不能正常运行，报错如下：
Exception in thread "main" java.lang.UnsatisfiedLinkError
Can't load IA 32-bit .dll on a AMD 64-bit platform12
即由于在AMD64的架构下调用了32位的dll文件，导致的安全问题。
看来需要让cmake编译出64位的dll才行啊。
下面分享一种可行的方法：
在介绍之前要先了解一个概念
cmake有两种编译的方式，一种是使用CMakeLists.txt文件，另一种是使用CMakeCache.txt文件
cmake [option]  指向含有顶级CMakeLists.txt的那个目录
cmake [option]  指向含有CMakeCache.txt的那个目录
第一种方式用于第一次生成cmake makefile，此后可以在build dir里直接cmake . 注意.表示当前目录，因为当前目录中已经有CMakeCache.txt，所以适用第二种方式。实际上cmake总是先检查指定的build dir中有没有CMakeCache.txt，如果有就按第二种方式处理；如果没有才寻找CMakeLists.txt使用第一种方式处理。
下面介绍方法：
1. 在CMakeLists.txt文件中添加
set(BUILD_USE_64BITS on)1
在你希望的输出目录下

cmake -G "Visual Studio 13 2015 Win64" ..  //这个VS的版本取决于你安装的是哪个版本的VS1
仍然在输出目录下

cmake --build .1
构建成功，得到64bit版的dll了。

