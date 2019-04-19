# makefile 中支持这四个命令：make，make install ，make clean和make uninstall - sxf_123456的博客 - CSDN博客
2017年02月25日 13:35:11[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：11241
1.makefile文件的主要结构简介：
    makefile文件里面主要有三种内容：
    1.变量声明：
        变量声明就是一种基本的严格字符替换的操作。
        比如在前面声明了：objects=program.o foo.o utils.o
        那么在后面出现的所有$(objects)或者${objects}都会被自动替换成上面的那个字符序列，而且是严格替换，即不带空格的。
    2.映射法则
    3.命令：
    映射法则和命令通常都是联合起来组成这样的结构形式：
    target...   : prerequisites..
             command
    可以简单地理解为通过prerequisites，也就是先决的依赖文件，采取后面描述的相应的命令（这里的命令都是linux里的shell命令）command之后（一般是）生成了文件target。命令的前面都要按以下tab建留一段空白来表示它是命令。
    有的target后面并没有先决条件，也就是后面的命令是无条件执行的。
    这个结构总的作用就是告诉make，target是不是out of date，具体视它的依赖文件而定，target不存在也视为out of date。然后就是如果过期了，那么更新，就是运行下面的命令来更新。
2.前面那篇文章emacs中我们建立了两个makefile文件，但是是空的，还没有任何内容。现在我们填上如下内容：（这个是一个最简单的例子，不过是一个不错的入门。）
在/project/hello的主目录下面的makefile文件的内容如下：
    prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
export prefix
export exec_prefix
export bindir
all clean  install uninstall :   
             cd src && $(MAKE) $@
.PHONY:  all clean   install uninstall
在/project/hello/src目录下面的makefile文件内容如下：
all: hello
hello: main.c
         $(CC)  -o $@ main.c
clean:
         rm hello
install:
         install -d $(bindir)
         install -m 0755 hello  $(bindir)
uninstall:
         rm $(bindir)/hello
.PHONY: all clean  install uninstall
我们等下详细说明，先看下运行情况：
![](http://www.51hei.com/UploadFiles/2014-04/huqin/psu(5).png)
现在makefile支持这四个命令：make，make install ，make clean和make uninstall。
我们从上图可以看出可以直接从终端调用我们生成的程序hello，然后返回“hello，the world！”。
    首先我们看主目录下的makefile文件，前面那部分是变量声明，这个等下还会说到。然后是export 变量，这个就是主目录下的主makefile文件可以使用这个命令，意思是将变量和子一级目录下的makefile共享。
    然后我们看到
all clean  install uninstall :
         cd src && $(MAKE) $@
.PHONY:  all clean   install uninstall
这实际上是一个跳转结构，首先介绍下.PHONY是个什么意思，前面说了每一个target默认都是一个待生成文件，但是.PHONY:后面跟着的target的意思是这些target不用生成文件。那么总的来说这里的all，clean，install，uninstall在这里是些没有依赖文件的不用生成最终target文件的执行命令了。这些执行命令就是前面说的make，make install ，make clean和make uninstall。make的意思实际上就是make all，这两个命令是没有区别的。
    然后我们看到命令，cd src这个很容易理解，就是跳转到src目录中去了。然后&&符号的意思是逻辑与，即前一命令执行完之后再执行后面的命令。而后面的$(MAKE)可能是一个具有系统默认值的变量，在这里应该就是make。而$@符号的意思则是指代当前的target。比如说我现在make clean ，那么命令最终就成了这个样子：
    cd src &&make clean
    然后下一个make clean的命令实际上是对src子目录下的makefile文件说的，我们看到这里简单的：
clean:
    rm hello
即把生成的可执行文件hello删掉即可。
然后我们再来看子目录下那个makefile文件如何处理make all 命令：
all: hello
hello: main.c
    $(CC)  -o $@ main.c
我们看到，这里首先说明all依赖于hello，然后hello依赖于main.c，而要生成或者更新hello的话需要执行下面的命令：
cc -o hello main.c
上面的$(CC)也是具有系统默认值的变量，就是cc，不过在GNU/LINUX体系下可能调用的是gcc工具。后面的选项-o的意思是编译连接然后生成后面的那个名字的文件。名字在这里就是hello。
然后我们再来看make install命令执行的情况：
install:
install -d $(bindir)
install -m 0755 hello  $(bindir)
前面说过了这个变量是继承自主目录makefile文件中的定义，然后翻译出来$(bindir)就是/usr/local/bin。linux系统在终端可以直接调用的命令存放点有两个位置，另外的一个是/usr/bin。/usr/local文件夹内部的结构和/usr内部的结构非常相似。似乎讲起来/usr是供系统内部的软件使用的空间，但这个规矩并不是死的。
uninstall命令我们看到就是一个删除对应位置hello文件的操作。所以现在就把linux中的install命令讲讲就可以了。
    第一个install命令接着-d的选项的意思就是目录确认，在这里我觉得有点多余。
    第二个install命令接着-m的选项的意思就是把hello复制到指定的目录中去，然后相应文件也就是hello文件的权限设置成为0755.关于权限我觉得在这里似乎也没有什么好说的。在这里用第二个install命令就可以搞定了，过程是复制，加权限，更新时间戳。
