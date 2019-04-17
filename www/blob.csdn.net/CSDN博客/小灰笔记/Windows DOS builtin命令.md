# Windows DOS builtin命令 - 小灰笔记 - CSDN博客





2017年08月30日 00:20:08[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：416








       刚刚使用xcopy命令的时候发现它跟copy在命令作用行为上有很大区别，我感觉copy似乎更靠近系统内核层级一些。Where查看了一下命令的位置，发现copy找不到，但是xcopy倒是可以找到。

       迷惑了一会儿倒是想起了之前学习Linux入门教程时候遇到的builtin命令，猜测这跟当时遇到的cd等命令类似。先看Linux上的相应信息：

grey@DESKTOP-3T80NPQ:/mnt/c/Users/ThinkPad/Desktop$which cd

grey@DESKTOP-3T80NPQ:/mnt/c/Users/ThinkPad/Desktop$type -a cd

cd is a shellbuiltin

       猜测，其实那些跟系统行为相关的应该也是builtin的类似模式。查看了几个具体如下：

C:\Users\ThinkPad\Desktop>wherecd

信息: 用提供的模式无法找到文件。



C:\Users\ThinkPad\Desktop>wheredir

信息: 用提供的模式无法找到文件。



C:\Users\ThinkPad\Desktop>wherecopy

信息: 用提供的模式无法找到文件。



C:\Users\ThinkPad\Desktop>wherexcopy

C:\Windows\System32\xcopy.exe

       最后一个是我刚刚用到的xcopy，感觉功能更加复杂一点，但是距离内核的层级似乎也远了一些。



