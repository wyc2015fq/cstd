# powerpc_001_s32ds中使用汇编代码 - 小灰笔记 - CSDN博客





2018年08月27日 23:49:39[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：368








            之前曾经有过C语言比较差，我一点点学习C语言的经历。经过几次回炉之后，现在的C语言知识在工作中基本上是够用了。

            现在全心去开发底层代码，似乎PowerPC的驱动又成了我的一个障碍。没关系了，找个基础的教程继续回炉吧！点滴做起，总有突破的时候。

            手头前些年积累了几块PowerPC的开发板，正好开始启动学习。先从工具开始，今天积累第一个知识点：在S32DS中使用汇编代码。

            为什么从S32DS开始，主要还是因为免费、容易获取。这自然不一定是最好的开发工具，但是其中的道理应该是相通的。在这个环境下能够走通技术路线，其他的平台应该只是一个简单的切换而已。

            之前使用过其他的编译器，在使用汇编的时候比较简单。S32DS中还是第一次尝试，最终发现还是直接写一个汇编代码文件比较靠谱。在汇编代码中定义一个函数，之后在C语言中调用，大致应该不会出现大的问题。

            我写了我的第一段汇编代码如下：

![](https://img-blog.csdn.net/20180827234842277?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            第1、6、7行都加了一个TAB输入，基于两个原因：1，我用的emacs编辑器自动给我增加了；2，我在用其他编译器的时候遇到过类似makefile要求的用法，必须是TAB开头。

            简单的代码解释：

            L1，把这段代码放到.text区，这样后期结合链接文件的作用相应的代码会放到flash相应的区。

            L3，定义一个全局的lable，asm_demo。这个用法有点类似于C语言代码中的函数声明。

            L5，相当于C语言中的函数定义开始，6、7行是函数体。其中，第7行可以理解为C语言中的return。回到调用的位置，继续往下进行。

            还有一点值得说一下，那就是汇编指令中为什么都带有e_或者se_等前缀。这主要是因为我创建的工程是vle模式的。而这些指令则是vle版本。

            接下来是对这段代码的调用：

![](https://img-blog.csdn.net/20180827234842315?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            其中，第10行是对函数的声明，第18行是对函数的调用。

            最后看看编译的提示与结果：

![](https://img-blog.csdn.net/20180827234842393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            编译、链接全都通过且没有警告。

![](https://img-blog.csdn.net/20180827234842391?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            以上是map文件中的信息，其中第237行可以看出全局函数的名称与地址，236行能够知道存放的存储段。

            程序没有进行执行测试，这次算是简单的测试一下语法以及工具。后续再进行深一步的探索。



