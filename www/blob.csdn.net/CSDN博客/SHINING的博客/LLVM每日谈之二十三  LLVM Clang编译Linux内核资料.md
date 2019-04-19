# LLVM每日谈之二十三   LLVM/Clang编译Linux内核资料 - SHINING的博客 - CSDN博客
2017年03月10日 09:36:56[snsn1984](https://me.csdn.net/snsn1984)阅读数：3330
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[史宁宁（snsn1984）](http://blog.csdn.net/snsn1984)
之前有朋友问到这个问题，是否有使用LLVM/Clang编译Linux内核的，随手找了一些相关资料，在这里贴出来，与大家共享。
网址：[http://llvm.linuxfoundation.org](http://llvm.linuxfoundation.org)
项目简介：
# LLVMLinux Project Overview
This project aims to fully build the [Linux kernel](http://kernel.org/) using [Clang](http://llvm.linuxfoundation.org/index.php/LLVM) which is the C front end for the [LLVM](http://llvm.linuxfoundation.org/index.php/LLVM) compiler infrastructure project. Together Clang and LLVM have many positive attributes and features which many developers and system integrators would like to take advantage of when developing and deploying the Linux Kernel as a part of their own projects.
The hope is that this project will help reduce duplicate work, as well as concentrate, and accelerate efforts around this subject matter in order to allow Linux Kernel developers, and system integrators to have the choice of a Clang compiled Linux kernel. The goal is for this project to be a testing/proving ground for these updates and to get patches that make this possible upstream to their respective projects.
The LLVMLinux project consolidates the work of the lll-project, the PAX team, and Mark Charlebois' work on the ARM kernel.
There have been several [presentations and articles](http://llvm.linuxfoundation.org/index.php/Presentations) on this topic if you wish to learn more about the reasons for this project.
Currently the work is centred around the ARM, x86, x86_64, and MIPS architectures but contributions to support other architectures are welcome. Patches accepted!
Interested parties will be holding a Bi-weekly Google Hangout to discuss project status and goals.
最新进展情况，从新闻稿中可以看出：
Features You Won't Find In The Linux 4.9 Mainline Kernel
...
LLVM Clang Building - The mainline kernel still can't build cleanly under the LLVM Clang compiler. [The LLVMLinux project slowed down a lot](http://www.phoronix.com/scan.php?page=news_item&px=Clang-The-Kernel-2016) but hopefully developers will get back to working on this support in the not too distant future. 
...
新闻链接：http://www.phoronix.com/scan.php?page=news_item&px=Linux-4.9-Stuff-Not-Mainline
至少到2016年10月19日，该新闻发布的时候，还无法正常支持主线的内核编译。同时，也号召有兴趣的朋友们积极参与该项目。
另外，该项目的邮件列表为：[llvmlinux@lists.linuxfoundation.org](mailto:llvmlinux@lists.linuxfoundation.org).
