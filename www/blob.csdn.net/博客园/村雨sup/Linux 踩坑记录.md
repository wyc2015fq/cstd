# Linux 踩坑记录 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





1.



[Linux](http://lib.csdn.net/base/linux)系统上用火狐浏览器看视频的时候发现没有安装flash player插件，无法播放视频。简单几步安装插件，就可以快乐地使用linux系统看视频了。

步骤如下：

**一、**打开视频网站时提示无法加载视频，没有安装插件，点击视频节目中间的安装插件按钮。

**二、**点击安装后会自动打开flashplayer下载界面，需要选择安装方式。

**三、**在弹出来的界面中左下角选择安装方式，我选择的是.tar.gz方式安装，然后点击右下角的Download下载插件。下载方式都选择默认，然后点击OK

**四、**浏览器会弹出下载界面，等待下载完成。下载完成以后的文件一般是在/tmp(或者在下载目录)文件夹内，下载完成后在命令行界面用cd  /tmp命令转到tmp文件夹；

**五、**普通用户先用su - root命令转入超级用户模式，因为插件的安装涉及到更改权限问题。转入root用户后：输入下面4条命令（#号后面是注释）


         tar -zx -f install_flash_player_11_linux.x86_64.tar.gz  #解压下载好的压缩包

         cp libflashplayer.so  /usr/lib64/mozilla/plugins               #复制插件到火狐浏览器插件文件夹

         cd /usr/lib64/mozilla/plugins                                              #定位到插件目录

         chmod 755 libflashplayer.so                                              #更改插件权限

**六、**升级完毕以后打开视频网站，这个时候会提示要你升级插件。不用理会，直接点击旁边没有文字的地方，视频就可以正常播放了。


**Adobe Flash Player下载链接：https://get.adobe.com/flashplayer/?loc=cn**

**    百度百科参考链接：http://jingyan.baidu.com/article/fa4125accdeeec28ad709252.html**

    
有的同学可能会说怎么我的没有/usr/lib64/mozilla/plugins这个目录，只有/usr/lib和/usr/lib32，没有/usr/lib64及其后续目录，以前我也碰到过这个问题，当时还埋怨百度经验不准确，其实解决方法的宗旨是将libflashplayer.so
 移到火狐的插件目录下，我们要找的只是火狐的插件目录，一说到火狐浏览器，很容易想到 
mozilla，那就找相应目录吧，但是在哪里找呢？在Linux系统中，一些主要的应用程序一般都保存在/usr目录下，执行命令**find /usr -name "mozilla"**，此时可能会有好几个结果，但是又是哪一个呢？这时我们应该挨个进入目录看一下哪个目录下有**plugins**(插件的意思)这个目录，那么这个目录就是火狐浏览器的插件目录了，然后将libflashplayer.so拷贝到相应目录，再修改权限就OK啦。

2.

第一步：找到源配置文件

　　在ubuntu下，源文件的位置为： /etc/apt/source.list 。使用vim或者nano等编辑器，以管理员身份打开即可观赏代码。

第二部：找一个好用的镜像地址列表

　　我找的是阿里的一个镜像站，其他的也有很多，我觉着阿里至少在五年之内不会倒闭，所以就选它了。

　　首先将原文件备份：

```
cp ./source.list ./source.list.bak
```

　　然后开始修改 source.list 文件，将文件中所有的代码全部注释掉，使用 # 符号实现。然后将以下代码拷贝到文件中去：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# 源码
deb http://mirrors.aliyun.com/ubuntu/ xenial main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse

# 测试版源
deb http://mirrors.aliyun.com/ubuntu/ xenial-proposed main restricted universe multiverse

# 源码(src)
deb-src http://mirrors.aliyun.com/ubuntu/ xenial main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse

# 测试版源
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-proposed main restricted universe multiverse

# Canonical 合作伙伴和附加
deb http://archive.canonical.com/ubuntu/ xenial partner
deb http://extras.ubuntu.com/ubuntu/ xenial main

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

　　保存退出。至此，我们就将原来的`蜗牛`版的源替换成了`急速`版的源了。执行下update试试，是不是发现速度快了很多？













