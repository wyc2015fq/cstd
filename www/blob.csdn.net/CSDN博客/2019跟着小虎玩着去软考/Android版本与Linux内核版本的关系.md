# Android版本与Linux内核版本的关系 - 2019跟着小虎玩着去软考 - CSDN博客
2011年08月03日 16:14:30[littletigerat](https://me.csdn.net/littletigerat)阅读数：19359标签：[linux内核																[android																[branch																[git																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=git&t=blog)](https://so.csdn.net/so/search/s.do?q=branch&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[Java																[手机开发																[Android](https://blog.csdn.net/littletigerat/article/category/613910)](https://blog.csdn.net/littletigerat/article/category/649495)](https://blog.csdn.net/littletigerat/article/category/781909)
Android版本与Linux内核版本的关系
# 一．Android版本与Linux内核的关系
|**英文名**|**中文名**|**Android版本**|**Linux内核版本**|
|----|----|----|----|
|No|无|1.1| |
|Cupcake|纸杯蛋糕|1.5|2.6.27|
|Donut|甜甜圈|1.6|2.6.29|
|Eclair|松饼|2.0|2.6.29|
|Eclair|松饼|2.1|2.6.29|
|Froyo|冻酸奶|2.2|2.6.32|
|Gingerbread|姜饼|2.3|2.6.35|
|Honeycomb|蜂巢|3.0|?|
# 二．下载源码分为2大部分
1.Android源码；
2.Android内核(Android  Linux Kernel)；
# 三．如何下载Android内核
git clone git://android.git.kernel.org/kernel/common.git
# 四．如何查看Android内核的版本（代码级查看）
在内核代码目录：
运行命令：git branch –a
## 运行实例
[root@localhost common]# git branch -a  （查看版本）
* android-2.6.36
  remotes/origin/HEAD -> origin/android-2.6.36
  remotes/origin/android-2.6.35
  remotes/origin/android-2.6.36
  remotes/origin/archive/android-2.6.25
  remotes/origin/archive/android-2.6.27
  remotes/origin/archive/android-2.6.29
  remotes/origin/archive/android-2.6.32
  remotes/origin/archive/android-gldfish-2.6.29
  remotes/origin/archive/android-goldfish-2.6.27
# 五．如何获得指定版本的Linux内核
要想下载2.6.29内核，只要最后执行一下（进入下载后的目录）： 
git checkout remotes/origin/archive/android-2.6.29   （即上面列出的版本分支）
此时下载到的是2.6.29版本内核了
# 六．如何获得Android某个版本的源码
例如获得Android1.5的源码
repo init -u git://android.git.kernel.org/platform/manifest.git -b cupcake
repo sync
Android 1.5：Cupcake（杯子蛋糕）
Android 1.6：Donut（甜甜圈）
Android 2.0 / 2.1：Eclair（闪电泡芙）
Android 2.2：Froyo（冷冻忧格）
Android 3.0：Gingerbread（姜饼）
Android 3.5：Honeycomb（蜂巢）
Android 4.0：Ice Cream（冰淇淋）
本质上：
想取某个branch而不是主线上的代码，我们需要用-b参数制定branch名字
# 七．如何获取Android当前主干上最新版本的所有的sourcecode
repo init -u git://android.git.kernel.org/platform/manifest.git
# 八．如何获取需要某一个project的代码
比如kernel/common，就不需要repo了，直接用Git即可
git clone git://android.git.kernel.org/kernel/common.git
# 九．列出本地所有分支 
git branch
# 十．如何获取某个branch的代码
如果需要某个branch的代码，用git checkout即可。
比如我们刚刚获取了kernel/common的代码，那就先进入到common目录，然后用下面的命令： 
git checkout origin/android-goldfish-2.6.27 -b goldfish  
这样我们就在本地建立了一个名为goldfish的android-goldfish-2.6.27分支，代码则已经与android-goldgish-2.6.27同步。
