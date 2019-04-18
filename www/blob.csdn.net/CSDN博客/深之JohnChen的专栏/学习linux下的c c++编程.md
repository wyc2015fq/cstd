# 学习linux下的c/c++编程 - 深之JohnChen的专栏 - CSDN博客

2009年03月05日 14:16:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：15120


** 学习linux下的c/c++编程**

1，先有linux环境
搭minGW和cygwin都有点麻烦，最最简单的办法还是装个真正的linux，用虚拟机也好，在网络上的另一台机器也好。这样不仅快，而且你有了真正的环境。
2.会C/C++语言（估计你会的）
3.入门阶段熟悉gcc命令行，最基本的参数，如，-g,-W,-O,-o,-c 建议看man gcc(很大找想要的)
4.编译第一个helloworld程序: 基本命令 gcc helloworld.c -o helloworld
前面四个阶段的时间估计只要1-2天就可以了
5.提高阶段，你需要开始了解，并熟悉ld,gdb,编写基本的Makefile，了解make程序，编程方面学习*nix系统IPC,信号行为（该阶段非常长估计要至少几个月-一年，看个人悟性和所花时间了）
到目前为止，你已经是linux软件工程师了，下面的高级是可选的:-)
6. 高级阶段：编程方面学习Linux内核源码结构,驱动开发，进而能精简内核，了解各种library的作用和用法。编译器方面：了解系统各个主要库如 libstdc++,glibc2(libc.so),ld-linux.so.2(动态链接器),了解gcc其它各种option.ld各种， option.编写复杂的Makefile，熟悉并能使用automake和autoconf（这个阶段，你最好自己至少编译和安装一次glibc, gcc,甚至生成cross-reference编译系统）（该阶段基本上没有止境，编译系统和内核熟悉之后，你基本上已经是专家了） 

备注:

编辑器vi,emacs   
编译器gcc   
集成开发环境kdevelop等   
函数库gtk+,QT等   
inux发版本都差不多的,哪个版本都一样

linux发版本都差不多，内核都一样，都是用gcc/g++做编译器，gdb调试 
界面编程，可以用gtk、qt等，另外wxWidgets也可以 
应用编程，只要用c/c++库，文件、内存、网络和windows下没有区别，进程和线程，有些不同，但原理类似，很容易入手，数据库的话，轻量级的sqlite，复杂的有mysql等，这些都是跨平台的， 
linux下驱动编程，相对于windows驱动来说，还是比较简单的，没有什么复杂的框架.

