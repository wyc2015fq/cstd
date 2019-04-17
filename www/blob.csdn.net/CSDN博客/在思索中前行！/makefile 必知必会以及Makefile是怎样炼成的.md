# makefile 必知必会以及Makefile是怎样炼成的 - 在思索中前行！ - CSDN博客





2014年11月25日 09:21:05[_Tham](https://me.csdn.net/txl16211)阅读数：607标签：[make																[Makefile																[make必知必会																[Makefile是怎样炼成的](https://so.csdn.net/so/search/s.do?q=Makefile是怎样炼成的&t=blog)
个人分类：[C/C++																[开发篇](https://blog.csdn.net/txl16211/article/category/2492171)](https://blog.csdn.net/txl16211/article/category/2284665)





# [Make必知必会原文链接](http://blog.csdn.net/crylearner/article/category/1773465)

# Makefile 必知必会

**Makefile的根本任务是根据规则生成目标文件。**

## 规则


一条规则包含三个：目标文件，目标文件依赖的文件，更新（或生成）目标文件的命令。

> 
> 
规则：


> 
<目标文件>:<依赖文件>


> 
         <更新目标的命令>  




PS：更新目标命令必须以tab开头，这个有点恶心。


Example     

> 
hello.o: hello.chello.h   

         gcc -c hello.c -o hello.o



      目标hello.o 依赖于hello.c,hello.h. 生成hello.o的命令时是“gcc -c hello.c -o hello.o”

## 伪目标


一般情况下目标文件是一个具体的文件，但有时候我们只需要一个标签，如目标clean。

> 
> 
声明伪目标：


> 
.PHONY:  <伪目标>




伪目标只是一个标签，这意味着伪目标的时间戳总是最新的，结果就是makefile每次都会去执行更新伪目标的命令。

## 终极目标


makefile并不会更新所有规则中的目标，它只会更新终极目标以及终极目标依赖的目标。


默认情况下makefile的第一个目标是终极目标，而且大家约定俗成的总是将all作为第一个目标。环境变量*MAKECMDGOALS*记录着终极目标。




PS：你也可以在make的命令行参数中指定终极目标。如make clean表示以clean作为终极目标。

## 多规则目标


Makefile中，一个文件可以作为多个规则的目标，这种情形就是多规则目标。


多规则目标下，以这个文件为目标的所有规则的依赖文件将会被合并成此一个依赖文件列表，但是命令不会合并，而且实际上，这多个规则中至多只能有一个规则定义了更新命令。

> 
> 
all：hello.o            


> 
all：hello.h            


> 
等价于  all: hello.o hello.h



## 什么时候更新目标


如果目标不存在或者依赖文件中至少有一个文件的时间戳比目标新，则执行目标更新命令。

## 包含其他makefile


类似于C语言中的头文件包含，makefile也可以包含其他makefile。

> 
> 
格式：


> 
include<makefile>




与C语言不同的是，包含其他makefile不只是把其他makefile中的内容导入到当前makefile中，而且它还有一个附加行为，具体参看下面makefile执行步骤：

> 
1.  依次读取变量“MAKEFILES”定义的makefile文件列表

2.  读取工作目录下的makefile文件（根据命名的查找顺序“GNUmakefile”，“makefile”，“Makefile”，首先找到那个就读取那个）

3.  依次读取工作目录makefile文件中使用指示符“include”包含的文件

4.  查找重建所有已读取的makefile文件的规则

  （如果存在一个目标是当前读取的某一个makefile文件，则执行此规则重建此makefile文件， 完成以后从第一步开始重新执行）




## 自动生成头文件依赖关系


C，C++的源文件编译依赖于头文件，手工维护这个头文件依赖列表无疑是琐碎而易错。这里可以利用gcc自带命令自动产生依赖文件列表，然后利用include命令导入即可。

> 
> 
%.o: %.c


> 
*$(GCC)* -MMD -MP -MF"*$(@:%.o=%.d)*"-MT"$@" -MT"*$(@:%.o=%.d)*" -o "$@""$<"




这个命令比较复杂，有兴趣的同学可以自己私下搜索一下。这里我只说一下结果：


如果我们要编译 hello.o,  而hello.c中包含头文件hello.h。那么执行上面命令的结果是生成hello.d,其内容是


         hello.o hello.d : hello.c hello.h


于是只要再加上include  hello.d,自然而然地就有hello.o依赖关系。](https://so.csdn.net/so/search/s.do?q=make必知必会&t=blog)](https://so.csdn.net/so/search/s.do?q=Makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=make&t=blog)




