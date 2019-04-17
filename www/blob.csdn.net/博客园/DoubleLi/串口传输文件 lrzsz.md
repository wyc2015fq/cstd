# 串口传输文件 lrzsz - DoubleLi - 博客园






假设有一种开发环境，一块板子，除了串口，没有任何外部出入输出设备，没有sd卡，没有网线，这个时候如果你想跟这块板子传输交互文件，要怎么办？

根据modem所采用的文件传输协议：xmodem,ymodem,zmodem将其分类  

xmodem:简单通用，传输信息单位是“包=128B”,传输速度慢，适合电话线路质量差的情况下用  
ymodem:由XMODEM演变来，效率可靠性高，包=128*8B；一次传输可发送或接受几个文件  

zmodem:于上两种不同，已连续的数据流发送数据，效率更高

在ubuntu主机中可以直接使用rz或者sz命令与secureCRT交互文件

在开发板中busybox提供rx命令可直接Xmodem下载文件到开发板，但是没有找到上传命令

命令：

rz 下载Zmodem

xz 上传Zmodem

rx 下载Zmodem



如果想在开发板中上传文件的话可以移植完整版的lrzsz到开发板，完整使用其功能

**一，移植lrzsz到开发板中**



lrzsz工具在配合secureCRT相当实用，现把其移植到arm-linux下

交代下环境：

pc ubuntu12.4

cross-compile arm-linux-gcc

--1--获取源码

[http://www.ohse.de/uwe/software/lrzsz.html](http://www.ohse.de/uwe/software/lrzsz.html)

获取lrzsz-0.12.20.tar.gz

--2--解压源码

tar -xzf lrzsz-0.12.20.tar.gz

--3--生成cache文件

./configure --host=arm-linux --cache-file=./arm-linux.cache

这个时候目录中应该生成了./arm-linux.cache文件

--4--修改配置

vi arm-linux.cache

将

ac_cv_prog_CC=${ac_cv_prog_CC='gcc'}

改成

ac_cv_prog_CC=${ac_cv_prog_CC='arm-linux-gcc'}

--5--使用cache文件配置源码

./configure --host=arm-linux --cache-file=./arm-linux.cache



--6--编译源码

make

**注：可能出现的问题:zglobal.h:188:22: error: libintl.h: No such file or directory**

**注：可能出现的问题:warning: incompatible implicit declaration of built-in function 'gettext'**

**解决方法：**

**vi config.h**

**修改/* Define to 1 if NLS is requested.  */#define ENABLE_NLS 1**

**为**

**#define ENABLE_NLS 0**

--6--得到工具

在src/文件夹下的lrz和lsz工具即是交叉编译成功的lrzsz工具了

--7--实用

将lrz和lsz复制到板子/bin下，即可使用，当然要配合secureCRT



一，利用lrzsz通过串口与secureCRT传输文件

--1--secureCRT设置,在串口的情况下需要打开XON/XOFF流控机制

会话选项->串行->流控->XON/XOFF 打钩，然后重启会话

![](http://img.blog.csdn.net/20140613132504218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJhZ29uMTAxNzg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

会话选项->终端->X/Y/Zmodem->目录 选择要存放的目录（我选择的是桌面）

![](http://img.blog.csdn.net/20140613133526484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJhZ29uMTAxNzg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



--2--上传文件

终端输入lrz命令后自动弹出选择文件对话框，选择之后开始传输

      $ ./lrz 

![](http://img.blog.csdn.net/20140613133027968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJhZ29uMTAxNzg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

--2--下载文件

终端中输入lsz+文件名

 $
 ./lsz <文件名>

![](http://img.blog.csdn.net/20140613133319765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJhZ29uMTAxNzg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

--3--通过BusyBox中的rx简单下载文件

如果根本完全没有任何外部接口，连lrzsz都灌不进去。或者只想下载传输文件，不需要上传的情况，可以使用Busybox中的rx命令简单下载文件

终端中输入rx+要保存的名称，比如$rx redmin.txt

然后在终端中发送文件 菜单->传输->发送Xmodem->选择redmin.txt文件发送数据

![](http://img.blog.csdn.net/20140613134141593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJhZ29uMTAxNzg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



版权声明：本文为博主原创文章，未经博主允许不得转载。










