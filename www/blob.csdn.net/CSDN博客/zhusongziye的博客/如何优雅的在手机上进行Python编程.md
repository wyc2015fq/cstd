# 如何优雅的在手机上进行Python编程 - zhusongziye的博客 - CSDN博客





2018年11月25日 10:18:59[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：120








大家都知道有安卓上有一个QPython,可以写Python,但是QPython已经好久没有更新了,官方也貌似没有更新的打算了。今天,给大家推荐另一款安卓工具。QPython支持的库也有限，不能很好的满足我们的要求。

今天,给大家推荐的这一款安卓APP叫Termux

高档终端Termux是个强壮的终端模仿器，这么的运用应当格外合适不喜欢图形界面的命令行爱好者。

首要特点：

高档终端Termux组合了强壮的终端模仿和拓宽Linux包搜集能够使用。

享用bash 和 zsh。

运用nano 和 vim修改文件。

经过ssh拜访服务器。

运用gcc和clang编译代码。

运用python控制台来作为口袋计算器。

运用git 和 subversion查看项目。

运用frotz运转根据文本的游戏。

在应用商店中都可以下载到，如果没有就百度一下。

![](https://img-blog.csdnimg.cn/20181125101721227.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

我们打开软件后首先输入命令 apt update更新一下可以安装的软件，接着输入apt list 就可以看到如下的效果

![](https://img-blog.csdnimg.cn/20181125101733954.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



然后就可以输入apt install python 如果要安装Python2就 输入apt install python2就可以了，apt list得到的列表都可以这么安装。

![](https://img-blog.csdnimg.cn/20181125101747949.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

可以看到这就安装成功了，还支持pip 安装

![](https://img-blog.csdnimg.cn/20181125101800622.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

模块

![](https://img-blog.csdnimg.cn/20181125101817924.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

Python使用：

![](https://img-blog.csdnimg.cn/20181125101827535.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

在安卓这种基于Linux的系统中写代码当然要安装vim了

![](https://img-blog.csdnimg.cn/20181125101837993.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

Termux支持Linux常见的命令，配上黑客键盘这个APP就更加完美了。

有了这款APP，妈妈再也不用不用担心我写不了代码啦。讲真确实不错，跑起来的效率也还可以。

IOS苹果用户则可以用这款APP：Pythonista在苹果的应用商店里可以下载到

![](https://img-blog.csdnimg.cn/2018112510184917.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



