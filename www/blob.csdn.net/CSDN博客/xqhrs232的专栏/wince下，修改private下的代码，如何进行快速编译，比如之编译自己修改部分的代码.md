# wince下，修改private下的代码，如何进行快速编译，比如之编译自己修改部分的代码 - xqhrs232的专栏 - CSDN博客
2013年01月23日 22:40:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：615
原文地址::[http://blog.csdn.net/bbw2008/article/details/5516055](http://blog.csdn.net/bbw2008/article/details/5516055)
1.      vs2005的命令行编译方式下，找到你所修改代码所属的lib，或者，dll的目录进入你修改的代码目录，用sysgen_capture 模块名
2.      生成source文件
3.      拷贝要修改的代码目录，拷贝自己bsp下，删除掉原来的source文件，将刚才在原始目录里生成的，你所需要的模块名的那个source文件改成source
例如：源来：source.deviceàsource
4.      vs2005的命令行编译方式下，找到你自己bsp目录下，然后，build
 –c
5.      vs2005的命令行编译模式下，进入你的D:/WINCE600/OSDesigns/Seeker2ws/RelDir/seeker2_ARMV4I_Release，使用romimage
 ce.bib,和makebinfs
6.      结束。
//=======================================
备注::
1、自己还没亲自实验过，还不知道行不行，先记录在这里了
2、WINCE60 HELP帮助文件里面有这方面的介绍----Using the Sysgen Capture Tool
