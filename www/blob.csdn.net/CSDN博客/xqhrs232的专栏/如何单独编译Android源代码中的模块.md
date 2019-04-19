# 如何单独编译Android源代码中的模块 - xqhrs232的专栏 - CSDN博客
2012年03月05日 11:04:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：418标签：[android																[脚本																[ubuntu																[google																[build																[email](https://so.csdn.net/so/search/s.do?q=email&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/luoshengyang/article/details/6566662](http://blog.csdn.net/luoshengyang/article/details/6566662)
 第一次下载好Android源代码工程后，我们通常是在Android源代码工程目录下执行make命令，经过漫长的等待之后，就可以得到Android系统镜像system.img了。以后如果我们修改了Android源代码中的某个模块或者在Android源代码工程新增一个自己的模块，是不是还是执行make命令呢？答案是否定的，Google为我们准备了另外的命令来支持编译单独的模块，以及重新打包system.img的命令。在继续学习Android源代码之前，就让我们先来看看这个命令吧。
      一. 首先在Android源代码目录下的build目录下，有个脚本文件envsetup.sh，执行这个脚本文件后，就可以获得一些有用的工具：
**USER-NAME@MACHINE-NAME:~/Android$ .  ./build/envsetup.sh**
注意，这是一个source命令，执行之后，就会有一些额外的命令可以使用：
**      - croot: Changes directory to the top of the tree.**
**      - m: Makes from the top of the tree.**
**      - mm: Builds all of the modules in the current directory.**
**      - mmm: Builds all of the modules in the supplied directories.**
**      - cgrep: Greps on all local C/C++ files.**
**      - jgrep: Greps on all local Java files.**
**      - resgrep: Greps on all local res/*.xml files.**
**      - godir: Go to the directory containing a file.**
      这些命令的具体用法，可以在命令的后面加-help来查看，这里我们只关注mmm命令，也就是可以用它来编译指定目录的所有模块，通常这个目录只包含一个模块。
      二. 使用mmm命令来编译指定的模块，例如Email应用程序：
**USER-NAME@MACHINE-NAME:~/Android$ mmm packages/apps/Email/**
编译完成之后，就可以在out/target/product/generic/system/app目录下看到Email.apk文件了。Android系统自带的App都放在这具目录下。另外，Android系统的一些可执行文件，例如C编译的可执行文件，放在out/target/product/generic/system/bin目录下，动态链接库文件放在out/target/product/generic/system/lib目录下，out/target/product/generic/system/lib/hw目录存放的是硬件抽象层（HAL）接口文件，后面的文章里面，我们会陆续提及到，敬请关注。
      三. 编译好模块后，还要重新打包一下system.img文件，这样我们把system.img运行在模拟器上时，就可以看到我们的程序了。
**USER-NAME@MACHINE-NAME:~/Android$ make snod**
 四. 参照[Ubuntu上下载、编译和安装Android最新源代码](http://blog.csdn.net/luoshengyang/article/details/6559955)一文介绍的方法运行模拟器：
**USER-NAME@MACHINE-NAME:~/Android$ emulator**
这样一切就搞定了。
