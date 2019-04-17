# 在PC上使用串口向开发板传送文件 - DoubleLi - 博客园






之前一直使用的是NFS，即网络挂载文件系统，不愁文件的传输问题，但是不幸的是 前几天网络出现故障，使这种方式没法使用 

变好的驱动程序一直没法下载到开发板里面，蛋疼欲裂啊。

于是乎折磨则么使用串口传输

下面是步骤

1.有一个文件系统 这个文件系统呢可以使用mk2yaffsimage来编译成文件镜像烧写到开发板里面使用

2使用rz sz 

网上下载rz/sz工具rzsz-3.48.tar.gz


tar zxvf  rzsz-3.48.tar.gz


进入到 src目录下

修改Makefile     cc=arm-linux-gcc   ,flags=-static  -o

上面这一步是关键，要不然在开发板上 使用rz命令的时候会什么都看不到 。懂了吧 ，主要是 -static 命令  之前这个命令没加，卡了大半天 奶奶的

使用make  posix命令  会生出  rz等可执行文件

把这些可执行文件放到 最小根文件系统的 bin目录下

 使用mk2yaffsimage  fs_mini  110.yaffs2  制作根文件系统镜像

烧写到开发板

3 在securCRT 界面里面 打开option面板  下面的x/y/modoem里面设置输出输入目录

4在串口界面点击rz便可下载东西进入文件系统了  

恼火了一天还是搞定这个恼火的问题。









