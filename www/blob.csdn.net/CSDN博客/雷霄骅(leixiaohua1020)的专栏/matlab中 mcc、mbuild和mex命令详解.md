# matlab中 mcc、mbuild和mex命令详解 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年08月03日 23:25:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：168
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










先简单的说说mcc、mbuild和mex到底怎么回事：

mcc将M文件转换成C/C++文件和相应的MEX包裹文件(需要Matlab编辑器)，但在特定条件下可以自动调用mbuild或者mex

mex将C/C++文件(入口函数为mexfunction())编译成MEX(DLL)文件(需要C/C++编辑器)

mbuild将C/C++文件(入口函数为main())生成可独立运行的C应用程序(需要C/C++编辑器)



### 一 mcc命令

Matlab编译器使用mcc命令能将一个M文件翻译成C文件，同时生成一个相应的包裹文件，这个包裹文件包含了编译器产生的代码和它支持的可执行文件类型之间所需的接口。利用改包裹文件提供的接口，生成的C文件能用在任何编译器所支持的可执行文件类型(如MEX文件、EXE文件、DLL文件)中，这些C文件与最终生成的可执行文件是相互独立的。
Matlab编译器(Lcc)在特定的条件下能自动调用mex和mbuild函数，尤其是mcc –x/-m/-p时。


(1)创建MEX文件
>>mcc –x filename (注意这个命令在2008a中已经去掉了)将M文件filename.m翻译成C代码，并生成一个可被Matlab直接调用的C的MEX。


(2)创建simulink S函数
>>mcc –s filename
将M文件filename.m翻译成C代码，并生成一个相应的simulink S函数，该函数的输入输出变量的大小可动态改变。


(3)创建可独立执行的C程序
>>mcc –m filename
将M文件filename.m翻译成C代码，生成的可执行文件能独立于Matlab运行环境。


(4)创建可独立运行的C++程序
>>mcc –p filename
将M文件filename.m翻译成C++代码，生成的可执行文件能独立于Matlab运行环境。


(5)创建可独立运行的C图形库函数
>>mcc –B sgl filename
如果filename.m中包含了对Matlab图形处理函数的调用，上述命令，将filename转换成为C语言，并生成一个能独立于Matlab运行环境的可执行程序。


(6) 创建可独立运行的C++图形库函数
>>mcc –B sgl cpp filename
如果filename.m中包含了对Matlab图形处理函数的调用，上述命令，将filename转换成为C++语言，并生成一个能独立于Matlab运行环境的可执行程序。


(7)创建C函数库
>>mcc –m –W lib:libfoo –T link:libfoo.m
创建一个C函数库



### 二 mex命令

用户可以在Matlab中按简单的M文件语法规则设计出来完成特定计算的算法。然后用Matlab编译器调用mex命令创建C共享库函数(DLL)或C++静态函数库，最后将它们整合到C/C++应用程序中，编译完这些C/C++应用程序，就可以用这些算法来实现特定的计算功能。当然这过程中也隐藏了算法和加快了代码的执行效率。
由于MEX文件执行效率高，故如果有文件重名的话，Matlab将优先调用MEX文件，下面以一个例子说明：



```
function theimage=gasket(numpoits)
theimage=zeros(1000);
corners=[866 1;1 500;866 1000];
startpoit=[866 1];
therand=ceil(rand(numpoits,1)*3);
for ii=1:numpoits
    startpoit=floor((corners(therand(ii),:)+startpoit)/2);
    theimage(startpoit(1),startpoit(2))=1;
end
imagesc(theimage)
colormap([1 1 1;0 0 0]);
axis equal tight
```


现在直接调用

```
>> tic ,gasket(5000000);toc
Elapsed time is 7.240129 seconds.
```

接着把它编译成mex文件在调用试试

```
>>mcc -p tic gasket;tic ,gasket(5000000);toc
Elapsed time is 1.23861 seconds.
```

### 三 mbuild命令

要建立独立运行的C应用程序，系统中需要安装Matlab、Matlab编译器、C/C++编译器以及Matlab C/C++数学库函数和图形库函数。
Matlab编译器使用mbuild命令可以直接将C/C++源代码编译为独立运行程序，在这些源代码中可以使用Matlab提供的接口来调用Matlab数学库函数。
虽然生成的可执行程序可以脱离Matlab环境运行，但是它需要Matlab C/C++数学库文件或者图形库文件的支持才能运行。但如果C/C++源代码完全是按传统C/C++源代码语法规则编写，没有包含Matlab数学库和图形库的调用，则不仅可以独立与Matlab环境，也不需要那两个库函数的支持。

对于如何在其他环境中运行那个生成的exe文件，大家看看这个帖子：http://www.matlabsky.com/thread-543-1-1.html

下面说说一些比较和区别：

1. MEX文件和EXE文件的差别

Mex文件与Matlab解释器在同一个过程空间运行，当用户调用一个MEX文件时Matlab解释器就会动态的连接到MEX文件。
可独立运行的C/C++应用程序(exe)可以独立于Matlab环境而运行，而MEX文件则不行，它需要Matlab的支持。还有EXE中可以调用MEX文件。

2.mex和mbuild编译的C/C++文件的差别



使用mbuild命令编译的C/C++源文件中，必须包含main()函数，它能独立于Matlab运行。而mex命令编译的C/C++源文件中不含main()函数，它是以mexfunction()函数作为入口的，编译后生成的是mex文件，被Matlab调用。



原文地址：[http://blog.sina.com.cn/s/blog_5928f2dd0100gf27.html](http://blog.sina.com.cn/s/blog_5928f2dd0100gf27.html)




