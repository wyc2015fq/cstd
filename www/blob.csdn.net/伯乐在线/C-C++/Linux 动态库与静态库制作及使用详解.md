# Linux 动态库与静态库制作及使用详解 - 文章 - 伯乐在线
原文出处： [IBM/陈鲁](http://www.ibm.com/developerworks/cn/linux/l-cn-linklib/index.html)
Linux 应用开发通常要考虑三个问题，即：1）在 Linux 应用程序开发过程中遇到过标准库链接在不同 Linux 版本下不兼容的问题； 2）在 Linux 静态库的制作过程中发现有别于 Windows 下静态库的制作方法；3）在 Linux 应用程序链接第三方库或者其他静态库的时候发现链接顺序的烦人问题。本文就这三个问题针对 Linux 下标准库链接和如何巧妙构建 achrive(*.a) 展开相关介绍。
## 两个要知道的基本知识
Linux 应用程序因为 Linux 版本的众多与各自独立性，在工程制作与使用中必须熟练掌握如下两点才能有效地工作和理想地运行。
- Linux 下标准库链接的三种方式（全静态 , 半静态 (libgcc,libstdc++), 全动态）及其各自利弊。
- Linux 下如何巧妙构建 achrive(*.a)，并且如何设置链接选项来解决 gcc 比较特别的链接库的顺序问题。
## 三种标准库链接方式选项及对比
为了演示三种不同的标准库链接方式对最终应用程序产生的区别， 这里用了一个经典的示例应用程序 HelloWorld 做演示，见 清单 1 HelloWorld。
##### 清单 1. HelloWorld


```
#include <stdio.h> 
 #include <iostream> 
 using std::cout; 
 using std::endl; 
 int main(int argc, char* argv[]) 
 { 
  printf("HelloWorld!(Printed by printf)\n"); 
  cout<<"HelloWorld!(Printed by cout)"<<endl; 
  return 0; 
 }
```
三种标准库链接方式的选项及区别见 表 1
##### 表 1. 三种标准库链接方式的选项及区别
|标准库连接方式|示例连接选项|优点|缺点|
|----|----|----|----|
|-static -pthread -lrt -ldl|不会发生应用程序在 不同 Linux 版本下的标准库不兼容问题。|生成的文件比较大，应用程序功能受限（不能调用动态库等）| |
|-pthread -lrt -ldl|生成文件是三者中最小的|比较容易发生应用程序在不同 Linux 版本下标准库依赖不兼容问题。| |
|-static-libgcc -L. -pthread -lrt -ldl|灵活度大，能够针对不同的标准库采取不同的链接策略，从而避免不兼容问题发生。结合了全静态与全动态两种链接方式的优点。|比较难识别哪些库容易发生不兼容问题，目前只有依靠经验积累。某些功能会因选择的标准库版本而丧失。| |
上述三种标准库链接方式中，比较特殊的是 **半静态**链接方式，主要在于其还需要在链接前增加额外的一个步骤：
ln -s `g++ -print-file-name=libstdc++.a`，作用是将 libstdc++.a（libstdc++ 的静态库）符号链接到本地工程链接目录。
-print-file-name 在 gcc 中的解释如下：
-print-file-name=<lib> Display the full path to library <lib>
为了区分三种不同的标准库链接方式对最终生成的可执行文件的影响，本文从两个不同的维度进行分析比较：
### 维度一：最终生成的可执行文件对标准库的依赖方式（使用 ldd 命令进行分析）
ldd 简介：该命令用于打印出某个应用程序或者动态库所依赖的动态库
涉及语法：ldd [OPTION]… FILE…
其他详细说明请参阅 man 说明。
三种标准库链接方式最终产生的应用程序的可执行文件对于标准库的依赖方式具体差异见 图 1、图 2、图 3所示：
##### 图 1. 全静态标准库链接方式
![11](http://jbcdn2.b0.upaiyun.com/2016/07/7bfc85c0d74ff05806e0b5a0fa0c1df1.png)
图 2. 全动态标准库链接方式
![2](http://jbcdn2.b0.upaiyun.com/2016/07/665f644e43731ff9db3d341da5c827e1.png)
##### 图 3. 半静态（libgcc,libstdc++) 标准库链接方式
![3](http://jbcdn2.b0.upaiyun.com/2016/07/38026ed22fc1a91d92b5d2ef93540f20.png)
通过上述三图，可以清楚的看到，当用 **全静态标准库的链接方式**时，所生成的可执行文件最终不依赖任何的动态标准库，
而 **全动态标准库的链接方式**会导致最终应用程序可执行文件依赖于所有用到的标准动态库。
区别于上述两种方式的 **半静态链接方式**则有针对性的将 libgcc 和 libstdc++ 两个标准库非动态链接。
（对比 图 2与 图 3，可见在 图 3中这两个标准库的动态依赖不见了）
从实际应用当中发现，最理想的标准库链接方式就是半静态链接，通常会选择将 libgcc 与 libstdc++ 这两个标准库静态链接，
从而避免应用程序在不同 Linux 版本间标准库依赖不兼容的问题发生。
### 维度二 : 最终生成的可执行文件大小（使用 size 命令进行分析）
size 简介：该命令用于显示出可执行文件的大小
涉及语法：size objfile…
其他详细说明请参阅 man 说明。
三种标准库链接方式最终产生的应用程序的可执行文件的大小具体差异见 图 4、图 5、图 6所示：
##### 图 4. 全静态标准库链接方式
![4](http://jbcdn2.b0.upaiyun.com/2016/07/011ecee7d295c066ae68d4396215c3d0.png)
##### 图 5. 全动态标准库链接方式
![5](http://jbcdn2.b0.upaiyun.com/2016/07/4e44f1ac85cd60e3caa56bfd4afb675e.png)
##### 图 6. 半静态（libgcc,libstdc++) 标准库链接方式
![6](http://jbcdn2.b0.upaiyun.com/2016/07/3d2f8900f2e49c02b481c2f717aa9020.png)
通过上述三图可以看出，最终可执行文件的大小随最终所依赖的标准动态库的数量增加而减小。
从实际应用当中发现，最理想的是 **半静态链接方式**，因为该方式能够在避免应用程序于
不同 Linux 版本间标准库依赖不兼容的问题发生的同时，使最终生成的可执行文件大小最小化。
### 示例链接选项中所涉及命令（引用 GCC 原文）：
-llibrary
-l library：指定所需要的额外库
-Ldir：指定库搜索路径
-static：静态链接所有库
-static-libgcc：静态链接 gcc 库
-static-libstdc++：静态链接 c++ 库
关于上述命令的详细说明，请参阅 GCC 技术手册
## Linux 下静态库（archive）的制作方式：
### 涉及命令：ar
ar 简介：处理创建、修改、提取静态库的操作
涉及选项：
t – 显示静态库的内容
r[ab][f][u] – 更新或增加新文件到静态库中
[s] – 创建文档索引
ar -M [<mri-script] – 使用 ar 脚本处理
其他详细说明请参阅 man 说明。
### 示例情景：
假设现有如 图 7所示两个库文件
##### 图 7. 示例静态库文件
![示例静态库文件](http://www.ibm.com/developerworks/cn/linux/l-cn-linklib/Lib1.png)
从 图 7中可以得知，CdtLog.a 只包含 CdtLog.o 一个对象文件 , 而 xml.a 包含 TXmlParser.o 和 xmlparser.o 两个对象文件
现将 CdtLog.o 提取出来，然后通过 图 8方式创建一个新的静态库 demo.a，可以看出，demo.a 包含的是 CdtLog.o 以及 xml.a，
而不是我们所预期的 CdtLog.o,TXmlParser.o 和 xmlparser.o。这正是区别于 Windows 下静态库的制作。
##### 图 8. 示例静态库制作方式 1
![示例静态库制作方式 1](http://www.ibm.com/developerworks/cn/linux/l-cn-linklib/Lib2.png)
这样的 demo.a 当被链接入某个工程时，所有在 TXmlParser.o 和 xmlparser.o 定义的符号都不会被发现，从而会导致链接错误，
提示无法找到对应的符号。显然，通过图 8 方式创建 Linux 静态库是不正确的。
正确的方式有两种：
- 将所有静态库中包含的对象文件提取出来然后重新打包成新的静态库文件。
- 用一种更加灵活的方式创建新的静态库文件：**ar 脚本**。
显然，方式 1 是比较麻烦的，因为涉及到太多的文件处理，可能还要通过不断创建临时目录用于保存中间文件。
推荐使用如 清单 2 createlib.sh所示的 **ar 脚本**方式进行创建：
##### 清单 2 createlib.sh

```
rm demo.a 
 rm ar.mac 
 echo CREATE demo.a > ar.mac 
 echo SAVE >> ar.mac 
 echo END >> ar.mac 
 ar -M < ar.mac 
 ar -q demo.a CdtLog.o 
 echo OPEN demo.a > ar.mac 
 echo ADDLIB xml.a >> ar.mac 
 echo SAVE >> ar.mac 
 echo END >> ar.mac 
 ar -M < ar.mac 
 rm ar.mac
```
如果想在 Linux makefile 中使用 **ar 脚本**方式进行静态库的创建，可以编写如 清单 3 BUILD_LIBRARY所示的代码：
##### 清单 3 BUILD_LIBRARY

```
define BUILD_LIBRARY 
 $(if $(wildcard $@),@$(RM) $@) 
 $(if $(wildcard ar.mac),@$(RM) ar.mac) 
 $(if $(filter %.a, $^), 
 @ echo CREATE $@ > ar.mac 
 @ echo SAVE >> ar.mac 
 @ echo echo END >> ar.mac 
 @$(AR) -M < ar.mac 
 ) 
 $(if $(filter %.o,$^),@$(AR) -q $@ $(filter %.o, $^)) 
 $(if $(filter %.a, $^), 
 @ echo OPEN $@ > ar.mac 
 $(foreach LIB, $(filter %.a, $^), 
 @ echo ADDLIB $(LIB) >> ar.mac 
 ) 
 @ echo SAVE >> ar.mac 
 @ echo END >> ar.mac 
 @$(AR) -M < ar.mac 
 @$(RM) ar.mac 
 ) 
 endef 
 $(TargetDir)/$(TargetFileName):$(OBJS) 
    $(BUILD_LIBRARY)
```
通过 图 9，我们可以看到，用这种方式产生的 demo.a 才是我们想要的结果。
##### 图 9. 巧妙创建的静态库文件结果
![巧妙创建的静态库文件结果](http://www.ibm.com/developerworks/cn/linux/l-cn-linklib/Lib3.png)
## Linux 静态库链接顺序问题及解决方法：
正如 GCC 手册中提到的那样：
It makes a difference where in the command you write this option; the linker
searches and processes libraries and object files in the order they are specified.
Thus, ‘ foo.o -lz bar.o ’ searches library ‘ z ’ after file ‘ foo.o ’ but before
‘ bar.o ’ . If ‘ bar.o ’ refers to functions in ‘ z ’ , those functions may not be loaded.
为了解决这种库链接顺序问题，我们需要增加一些链接选项 :
$(CXX) $(LINKFLAGS) $(OBJS) -Xlinker “-(” $(LIBS) -Xlinker “-)” -o $@
通过将所有需要被链接的静态库放入 -Xlinker “-(” 与 -Xlinker “-)” 之间，可以是 g++ 链接过程中， 自动循环链接所有静态库，从而解决了原本的链接顺序问题。
### 涉及链接选项：-Xlinker
-Xlinker option
Pass option as an option to the linker. You can use this to supply system-specific
linker options which GCC does not know how to recognize.
## 小结
本文介绍了 Linux 下三种标准库链接的方式及各自利弊，同时还介绍了 Linux 下静态库的制作及使用方法，相信能够给 大多数需要部署 Linux 应用程序和编写 Linux Makefile 的工程师提供有用的帮助。
