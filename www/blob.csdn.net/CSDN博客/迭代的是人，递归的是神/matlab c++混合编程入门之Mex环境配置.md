# matlab c++混合编程入门之Mex环境配置 - 迭代的是人，递归的是神 - CSDN博客





2012年02月24日 15:09:21[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：17689








最近一直在看文章，很多老外都提供了源代码，好多都是matlab写的。其中不少都夹杂着c++混合编程的程序。对于matlab本身就是知之甚少，又遇到混[http://www.opencv.org.cn/forum/viewto合编程，让我头大的不行啊。硬着头皮搞吧，还好，弄出来了。于是把我解决问题的过程拿出来分享一下，希望对和我一样菜的朋友有个帮助。](http://www.opencv.org.cn/forum/viewto)

一个matlab程序，编译总是不通过，说我其中混合编程编译好的dll不是win32的有效程序，让我很奇怪，以老外对于科研的认真程度，不太可能上传一个编译有问题的程序，于是我考虑应该是开发环境不同导致的。他那段代码06年写的，估计开发环境matlab7.0+vc6.0。而我使用的是matlab2008a+vs2008。（这里提一句，很多网友和我讨论一些基础的opencv的使用问题，上来就把代码一给，然后告诉我编译不能通过，问我原因。其实很多时候和开发环境，是否配置好都有很大的关系，磨刀不误砍柴工，希望大家不要遇到点问题就急着求助，自己憋一憋很有好处的）于是我决定对这个dll重新编译。根据readme文件的提示，在matlab中命令“mex
 -0 xxx.cpp”就可以编译出dll文件。

不过在此之前很重要的就是配置好环境，先安装好mex。mex安装非常简单，一般分为两种：Matlab自带的Lcc编译器和微软的vc编译器

先在matlab命令mex -setup，然后按提示键入y，接下来弹出如下提示，就是选择编译器：

Select a compiler: 

[1] Lcc-win32 C 2.4.1 in D:\PROGRA~1\MATLAB\R2008a\sys\lcc\bin 

[2] Microsoft Visual C++ 6.0 in C:\Program Files\Microsoft Visual Studio 


[0] None 


选1，就是matlab自带的lcc编译器，选2，就是使用微软的vc编译器。这里我选择1没有编译成功（不知原因），所以我选择的是2（当然选择2的时候要确定自己的电脑已经成功安装好ms vc++6.0）。接下来根据提示确定编译器路径，一般键入y即可。并提示安装成功的信息。到此，环境配置成功。

再次运行mex -0 xxx.cpp成功，可是dll文件却不再生成，取而代之的是一个xxx.mexw32。我想这就是matlab2008与之前的版本不同造成的，之前的dll不能在matlab2008下兼容正常运行。

如果还需要将opencv加入到matlab链接，可以参考[怎样把opencv和matlab衔接？](http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=13850&start=0)

很简单的问题，希望对大家有所帮助~~



