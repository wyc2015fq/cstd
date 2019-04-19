# Android源代码下载指南(图解) - xqhrs232的专栏 - CSDN博客
2011年03月22日 15:56:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：934标签：[android																[vmware																[ubuntu																[linux																[git																[终端](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=git&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=vmware&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://android.yaohuiji.com/archives/2652](http://android.yaohuiji.com/archives/2652)
Android学习到一定程度，就一定要多读代码多思考，Android源代码就是非常好的学习材料，本讲的内容很简单，就是把Android的源码先下载回来。我们知道Android的源代码是用Git这个分布式版本控制工具管理的，下载起来比较麻烦，我们在这里用图解的方式一步一步来，你肯定能学会。
**第一步、安装VMWare** 我假设你在Windows下，而Android源代码在Linux下下载会更方面些。那么我们就装一个虚拟机来创造一个Linux环境。不熟悉的同学去Google一下“VMware Workstation 7.1安装”
**第二步：安装Ubuntu10.04** 这一步不熟悉的同学去搜索”在VMware中安装Ubuntu10.04” 需要提醒一下，最好在图形界面方式安装，安装过程比较慢，需要等待的时候要有点耐心，我在95%的时候等了近20分钟^_^ 。
**第三步：进入Linux ，打开终端** 哦,我还是配张图吧，蛋腚，我知道这个你会的^_^
![image](http://android.yaohuiji.com/wp-content/uploads/2010/09/image_thumb14.png)
**第四步：安装git和curl **在终端窗口敲下面的命令，下同
[view source](http://writeblog.csdn.net/#viewSource)
[print](http://writeblog.csdn.net/#printSource)[?](http://writeblog.csdn.net/#about)
|`1`|```javasudo apt-get install git-core curl```|
![9VNO{$]%X$04}{4TCVY{E_T](http://android.yaohuiji.com/wp-content/uploads/2010/09/9VNOX044TCVYE_T_thumb.jpg)
如上图第一行，下面一路回车，安装完毕。
**第五步：安装repo脚本**
[view source](http://writeblog.csdn.net/#viewSource)
[print](http://writeblog.csdn.net/#printSource)[?](http://writeblog.csdn.net/#about)
|`1`|```javacurl http:``````java//android.git.kernel.org/repo >~/repo```|
![72SKWXK6Y][[5)BO)7_OY@0](http://android.yaohuiji.com/wp-content/uploads/2010/09/72SKWXK6Y5BO7_OY0_thumb.jpg)
**第六步：给repo可执行权限 **配图还是上面那张，仔细看一下
[view source](http://writeblog.csdn.net/#viewSource)
[print](http://writeblog.csdn.net/#printSource)[?](http://writeblog.csdn.net/#about)
|`1`|```javachmod a+x ~/repo```|
**第七步：新建一个目录mydroid , 然后进入该目录 **配图还是上面那张，仔细看一下
[view source](http://writeblog.csdn.net/#viewSource)
[print](http://writeblog.csdn.net/#printSource)[?](http://writeblog.csdn.net/#about)
|`1`|```javamkdir mydroid```|
[view source](http://writeblog.csdn.net/#viewSource)
[print](http://writeblog.csdn.net/#printSource)[?](http://writeblog.csdn.net/#about)
|`1`|```javacd mydroid```|
**第八步：repo初始化 **配图还是上面那张，仔细看一下
[view source](http://writeblog.csdn.net/#viewSource)
[print](http://writeblog.csdn.net/#printSource)[?](http://writeblog.csdn.net/#about)
|`1`|```java~/repo init -u git:``````java//android.git.kernel.org/platform/manifest.git```|
其间需要输入姓名和邮箱，随意输入，测试颜色选择是，最后你会得到一个类似“repo initialized in /home/nebulayao/mydroid”的提示，说明初始化成功。配图如下：
![860O8KNIQMZW)R_07Q1XI}W](http://android.yaohuiji.com/wp-content/uploads/2010/09/860O8KNIQMZWR_07Q1XIW_thumb.jpg)
**第九步：repo同步**
[view source](http://writeblog.csdn.net/#viewSource)
[print](http://writeblog.csdn.net/#printSource)[?](http://writeblog.csdn.net/#about)
|`1`|```java~/repo sync```|
![%LVLLDLN8AIEGVN{]Q[TTOX](http://android.yaohuiji.com/wp-content/uploads/2010/09/LVLLDLN8AIEGVNQTTOX_thumb.jpg)
好了，因为有大约2个多G的资料需要下载，所以等待的时间可以出去吃个饭，打几局Dota……
![~VY144N_L9T1}61RKPOEF(P](http://android.yaohuiji.com/wp-content/uploads/2010/09/VY144N_L9T161RKPOEFP_thumb.jpg)
最后你看到了这样一句 Syncing work tree: 100% ,done 表示下载完毕。
**第十步：验收成果：**下载完源代码有什么用呢？我们进入mydroid/packages/apps/Phone目录看一下，哦这不就是Android系统中打电话的那个程序的源代码吗，是的，你没有看错！到处都是宝藏 ^_^
![~75OIGIW9$EV}79Z]RQ8J]2](http://android.yaohuiji.com/wp-content/uploads/2010/09/75OIGIW9EV79ZRQ8J2_thumb.jpg)
**Take your time and enjoy it.**

