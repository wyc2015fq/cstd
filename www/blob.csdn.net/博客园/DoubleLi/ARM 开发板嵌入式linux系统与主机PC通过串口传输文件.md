# ARM 开发板嵌入式linux系统与主机PC通过串口传输文件 - DoubleLi - 博客园






本文转载自[http://useless20.blog.163.com/blog/static/237409982010227127576/](http://useless20.blog.163.com/blog/static/237409982010227127576/)

嵌入式linux系统与主机通过串口传输文件

     我想如果要从PC机下载东西到开发板的嵌入式linux系统里面，很多人首先会想到用tftp sftp等网络工具从网口下载。但如果网络用不了，只能通过串口下载怎么办呢？这个时候有两个工具能帮到你：一个是zmrx、zmtx，另外一个是lsz、lrz。个人觉得zmrx/zmtx没有lsz/lrz稳定，建议还是用后者。下面介绍一下lsz/lrz的使用方法。



      一、编译lrzsz并下载到开发板上

     从http://download.chinaunix.net/download/0007000/6293.shtml下一个lrzsz的tar包，解压缩后输入./configure，然后进入了src文件夹中，修改了src文件夹中的Makefile文件，配置为与手机对应的交叉编译器，运行make，ok！得到两个可执行文件sz，rz（或者 lsz，lrz），把它们下载到开发板linux系统的/bin目录下。



      二、情况1: PC机用windows操作系统

     如果PC机用的是windows操作系统，串口通信工具可以用系统自带的超级终端。下面介绍一下传输文件的方法。

      1、开发板-->PC机

         在开发板上输入 sz filename。

         在PC机上点击超级终端的菜单“传送”-> “接收文件”，选择下载的位置和Zmodem 与崩溃恢复协议，点击接收即可。

      2、PC机-->开发板

         在开发板上先进入/tmp 目录，然后输入 rz。

         在PC机上点击超级终端的菜单“传送”-> “发送文件”，选择目标文件和Zmodem 与崩溃恢复协议，点击发送即可。









