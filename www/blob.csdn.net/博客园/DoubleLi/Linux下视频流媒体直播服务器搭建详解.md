# Linux下视频流媒体直播服务器搭建详解 - DoubleLi - 博客园






# 目标：

搭建网络直播流媒体服务器系统（[Linux](http://lib.csdn.net/base/linux)[操作系统](http://lib.csdn.net/base/operatingsystem)）



# 背景：

用于OTT-TV大并发的直播和点播的一套流媒体服务器系统。支持N x 24小时录制回看和直播的服务器端解决方案。



# 解决方案：

l  微软的Windows Media Services

l  服务端软件：Windows Media Server

l  平台：Windows

l  文件格式：ASF, WMV

--------------------------------------------------------------------

l  苹果公司的Quick Time系列

l  服务端软件：QuickTime Streaming Server (开源版本为Darwin Streaming Server)

l  平台：MacOS X, [linux](http://lib.csdn.net/base/linux), Windows

l  文件格式：MOV, MP4

-----------------------------------------------------------------------

l  Adobe公司的Adobe Media Server

l  服务端软件：Adobe Media Server

l  平台：Windows，Linux

l  文件格式：FLV

------------------------------------------------------------------------

l  八百里公司的800Li Media Server (国产方案)

l  服务端软件：八百里流媒体服务器系统

l  平台：Windows，Linux (CentOS 和Redhat)

l  文件格式：FLV

# 下面给大家介绍如何在Linux下搭建流媒体服务器系统（以八百里流媒体直播系统为例） 

# 第一步：安装前

1、  用管理员账号权限登陆linux的服务器；

2、  关闭防火墙；（需要开启一些端口）

3、  服务器要配置固定IP，不能设置成自动获取IP；

4、  确认操作系统为Centos或Redhat6版本以上。

# 第二步：安装前的准备工作

打开终端，建立4个模块所需要的缓存数据文件夹分别为：edge、live、storage、transcode（路径设置为/home/workspace/live），具体步骤如下图：

![](http://www.800li.net/Uploads/Editor/2017-05-05/590c35be18082.jpg)

# 第三步：安装步骤

一、进入到放置有安装包的文件夹目录里（/opt），解压缩各模块的安装文件

![](http://www.800li.net/Uploads/Editor/2017-05-05/590c35da09ba1.jpg)

二、进入到解压缩过后文件所在的文件夹进行安装

![](http://www.800li.net/Uploads/Editor/2017-05-05/590c35f7dbe4e.jpg)

根据提示进行相关设置（设置端口，一般只需默认即可）

![](http://www.800li.net/Uploads/Editor/2017-05-05/590c360baf8be.jpg)

![](http://www.800li.net/Uploads/Editor/2017-05-05/590c36221086f.jpg)

在安装过程中会提示建立一个账户登录名，以800li为例。安装结束后为新建立的800li账户设置密码：

![](http://www.800li.net/Uploads/Editor/2017-05-05/590c3638b3ae1.jpg)

三、安装结束后打开并通过浏览器访问http://本机的IP地址：web管理服务器端口，如：[http://192.168.1.160:9085/vms](http://192.168.1.160:9085/vms)

![](http://www.800li.net/Uploads/Editor/2017-05-05/590c365a13bec.jpg)

# Linux的常用操作命令

1、cd 进入文件夹

2、cd . 进入当前文件夹

3、cd .. 进入上一文件夹

4、mkdir 建立文件夹

5、ll 查看详细目录内容

6、ls 查看所有目录名称

7、tar –zxf 待解压缩文件名   解压缩文件

8、./install 安装文件

9、df 查看分区目录

10、rm –r –f  删除目录及下属文件

11、向上方向箭头  显示上一条命令

12、alt+F1  在服务器上创建终端窗口

13、ctrl+C 重新安装文件

14、cp 源文件 目标文件夹  把文件拷贝到目标文件夹

15、cp-R 源目录 目标目录  把源文件夹拷贝到目标文件夹

16、mv源目录 目标目录  把源文件夹剪切到目标文件夹

17、pwd 显示当前整个目录名

18、cd ~ 回到根目录

19、netstat –ltno  查看某些端口在不在监听

20、service iptables stop 关闭防火墙

21、给帐户名换密码命令：passwd 帐户名 （如passwd 800li）









