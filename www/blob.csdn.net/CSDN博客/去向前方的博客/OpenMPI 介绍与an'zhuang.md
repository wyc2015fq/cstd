# OpenMPI:介绍与an'zhuang - 去向前方的博客 - CSDN博客





2018年07月12日 11:05:02[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：702








# 前言

在跑GLowdemo时发现没装OpenMPI，因此，只有装了吧。

# 介绍

OpenMPI [1]  是一种高性能消息传递库，最初是作为融合的技术和资源从其他几个项目（FT- MPI, LA-MPI, LAM/MPI, 以及 PACX-MPI），它是MPI-2标准的一个开源实现，由一些科研机构和企业一起开发和维护。因此，OpenMPI能够从高性能社区中获得专业技术、工业技术和资源支持，来创建最好的MPI库。OpenMPI提供给系统和软件供应商、程序开发者和研究人员很多便利。易于使用，并运行本身在各种各样的操作系统，网络互连，以及一批/调度系统。

# 安装

首先指明博主的电脑，**华硕飞行堡垒+win10**

## 1.关于下载：

[http://www.mcs.anl.gov/research/projects/mpich2/downloads/tarballs/1.4.1p1/mpich2-1.4.1p1-win-ia32.msi](http://www.mcs.anl.gov/research/projects/mpich2/downloads/tarballs/1.4.1p1/mpich2-1.4.1p1-win-ia32.msi)

戳这里（下载完成可能不是.msi，关于这个，手动改了就好） 

重点内容 

然后就是正常的安装过程，安装中会有一个步骤让你填写Phrase。当然默认什么都不改的话就是behappy，你可以设置为你自己的喜欢的内容，记住就好。
## 2.配置：

安装完毕后查看开始菜单看看是否有一个MPICH2的文件夹，没有的话直接去到安装根目录下找到bin文件夹也是一样的，总之都是要找到这个东西： 

这里写图片描述

```
以及这个bin目录需要添加到系统的path变量下，右键此电脑，属性，高级系统设置，环境变量，找到path项将bin目录路径添加到变量中

打开之后输入你当前电脑有效的本地管理员账户名称和密码，为什么说是本地，win10中你可以用你的hotmail账户登录系统，这个不是本地账户，win10下你必须要切换至本地账户登录，才能成功配置MPI，而且配置完不能再切回在线登陆，否则会失效
输入完成之后点击register，ok退出

接下来是最重要的一步，很多同学也就是卡在这里：
以管理员身份运行cmd，注意是管理员身份，否则会报错，诸君可以自己试试- -
输入 smpd -install-phrase [你的关键字，默认是behappy]，之后会提示服务已注册，这一步实际是启用MPI服务，不启用的话你的MPI装上但是无法使用
```

（提示没有这个命令的童鞋检查你环境变量是否添加正确）   

## 3.验证安装：

怎么才能知道你安装成没成功呢，方法很多：

### -1

```
同样在bin目录下找到wmpiconfig.exe打开，点击gethost，再点击scan for version,这时候会出现你安装的MPI版本，这就说明MPI服务能够正常的和你的系统通信，你的安装是成功的，否则。。你没装对回去再看看吧- -

 ![这里写图片描述](https://img-blog.csdn.net/20180712105719411?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```

### - 2

打开bin目录下的mpiexec.exe,进程数一下，浏览到安装目录下的example下的cpi.exe这是自带的关于PI值计算的并行小程序，勾上run in a seprate window(在独立窗口中运行)，点击execute,看程序能否正常运行：  
![这里写图片描述](https://img-blog.csdn.net/201807121101494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
到此安装完成，需要使用ide编写MPI代码的话需要将lib文件夹里的.lib内容拷贝到你ide安装目录下的lib文件夹内， 

include文件夹内.h内容拷贝到ide安装目录下的include文件夹下 

安装完成以**管理员身份**打开命令提示符(cmd.exe)
cd \Program Files\MPICH2\bin

C:\Program Files\MPICH2\bin>smpd -install -phrase behappy ::<此处behappy与之前phrase对应>

然后C:\Program Files\MPICH2\bin>smpd –status查看是否成功，成功则显示smpd running on xxxxxx

注册用户C:\Program Files\MPICH2\bin>mpiexec –register

显示account (domain\user) xxxx : 直接回车默认当前用户

显示password: 输入操作系统当前用户密码

confirm password:

验证是否成功C:\Program Files\MPICH2\bin>mpiexec -validate（窗口提示success则表明注册成功）

测试示例程序

技术分享C:\Program Files\MPICH2\bin>cd ../examples

技术分享C:\Program Files\MPICH2\examples>..\bin\mpiexec.exe -n 4 cpi

(-n 后面参数为所用处理器核心数) 
![这里写图片描述](https://img-blog.csdn.net/20180712110414107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 参考

1[windows mpi安装](https://blog.csdn.net/baidu_33258926/article/details/51372904)

2[windows mpi使用](http://www.mamicode.com/info-detail-1137790.html)







