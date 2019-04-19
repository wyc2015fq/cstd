# 用 S5PV210 学习 Linux （一） 刷机（一） - xqhrs232的专栏 - CSDN博客
2017年03月18日 23:50:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：704
原文地址::[http://www.cnblogs.com/suozhang/p/6541230.html](http://www.cnblogs.com/suozhang/p/6541230.html)
相关文章
1、[用 S5PV210 学习 Linux （二） 刷机(二)](http://www.cnblogs.com/suozhang/p/6543424.html)----[http://www.cnblogs.com/suozhang/p/6543424.html](http://www.cnblogs.com/suozhang/p/6543424.html)
2、[用 S5PV210 学习 Linux （三） SD卡下载](http://www.cnblogs.com/suozhang/p/6544931.html)----[http://www.cnblogs.com/suozhang/p/6544931.html](http://www.cnblogs.com/suozhang/p/6544931.html)
简介： 习惯了 用 keil 或者 IAR  一键下载 (烧写) 代码，S5PV210 貌似就不能这么简单用 仿真器的 方式 下载代码了，因此 学习 S5PV210 的第一步就是 学习怎么下载代码，下面就是学习过程。2017年3月13日13:11:45，SUOZHANG.
1、因为我的 电脑是 win7 64位，官方三星  DNW刷机 软件没有 很好的 支持，需要修改 签名设置 一堆等等，打算 用  Linux 系统（虚拟机） Ubuntu 进行 刷机 S5PV210.
2、首先安装 虚拟机（下载地址： [http://rj.baidu.com/soft/detail/13808.html?ald](http://rj.baidu.com/soft/detail/13808.html?ald) ），参考安装方法：[http://jingyan.baidu.com/article/0320e2c1ef9f6c1b87507bf6.html](http://jingyan.baidu.com/article/0320e2c1ef9f6c1b87507bf6.html)
3、下载Ubuntu 系统：[https://www.ubuntu.com/download](https://www.ubuntu.com/download)，下载，所有的付钱选项都选择 0 美元。。。。。。。。就可以下载了，等我 有钱了在支持你哈，Ubuntu，2017年3月13日09:42:08
4、正在安装的截图，安装完成的截图，截图如下：
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313105602182-284713846.png)
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313110005026-1546404779.png)
5、 建立 WIN7 和 虚拟机 Ubuntu  共享文件夹，这样才能 把 DNW 软件 安装到 Ubuntu上。。下面截图是参考：  [嵌入式Linux学习笔记（基于S5PV210、TQ210）.pdf](http://blog.csdn.net/zjhsucceed_329/article/details/32336019)
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313112347120-1919815212.png)
6、打开Ubuntu  的终端，就是命令行界面，参考方法：[http://jingyan.baidu.com/article/91f5db1bd345ec1c7e05e34a.html](http://jingyan.baidu.com/article/91f5db1bd345ec1c7e05e34a.html),我采用的是Ctrl+Alt+T
 在桌面空白处，截图如下：
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313113658776-988652266.png)
7、用指令：su 切到 root 用户 
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313121059323-1947378100.png)
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313121229807-738072246.png)
8、在Ubuntu 下安装 VMware tools ，安装方法如下：
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313123543463-1756329902.png)
根据 步骤 ，首先在启动 Ubuntu 的 情况下，在 VMware 菜单栏下，“虚拟机” 下 更新 VMware tools ,就会出现  一下 截图：
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313123735041-874598428.png)
对选择 的文件 进行 右键  复制到 桌面 ，并右键 提取（Extract here）到  桌面上：
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313124955791-1781749160.png)
 记住 是 在 root  用户下：
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313125307885-58523970.png)
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313125330323-174237920.png)
使用  sudo ./vmware-install.pl 指令 进行 安装 ：一路 yes 下去
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313125627291-1505728124.png)
 重启，貌似不能  自动适应 大小……………………2017年3月13日12:59:07 suozhang
 (辟谣： 为啥没有 第九步，这是因为 第八步 太长了…………，2017年3月13日13:08:24，suozhang)
10、在 root 权限下，用 cd  指令 进入 根目录下 mnt  文件夹，用指令 ll 进行查看  该文件夹下的 目录,可以 看到   hgfs 文件夹， 用 cd  指令进入这个文件夹 就可以看到  window 7 共享的 文件夹 （我 共享的 文件夹名称是  share ，2017年3月13日13:03:33， suozhang）了：
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313130302885-892176692.png)
11、 这里 才 进入 重点  ,在 Ubuntu 下 刷机 S5PV210，参考 大神 博客 ：[http://blog.csdn.net/abf1234444/article/details/50748880](http://blog.csdn.net/abf1234444/article/details/50748880)，第一步先下载
 软件 ，我已经 下载并解压好 放到 共享目录下：
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313142443526-1137223593.png)
根据 朱老师的 视频 教程： 需要 修改以下 几个文件：视频 教程 链接：[http://edu.51cto.com/lesson/id-103521.html](http://edu.51cto.com/lesson/id-103521.html)，这个 收钱的，不知道 我截图 出来  朱老师要不要我 的版权费。。。。
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313143625073-218108476.png)
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313144051979-2090756775.png)
 然后 在 Ubuntu 下 复制 文件 到 桌面，
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313145548838-1557440545.png)
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313145643479-1530980831.png)
重点：进入  dnw-linux-master\src\driver 文件下，修改 Makefile 文件 内容如下：
参考大神 博客：[http://blog.csdn.net/liduxun/article/details/50709714](http://blog.csdn.net/liduxun/article/details/50709714)
obj-m := secbulk.o
KDIR := /lib/modules/`uname -r`/build
PWD  := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -rf *.order *.symvers
![](http://images2015.cnblogs.com/blog/728723/201703/728723-20170313155552791-2079535817.png)
这个 博客 写不下了  ，写 第二篇 ，稍后给链接。
## [用 S5PV210 学习 Linux （二） 刷机(二)](http://www.cnblogs.com/suozhang/p/6543424.html)
