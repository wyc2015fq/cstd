
# 无法连接上 cn.archive.ubuntu.com:80 (123.129.214.98)。 - connect (111: 拒绝连接) - 嵌入式Linux - CSDN博客

2013年09月10日 21:53:40[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：15927


今天用sudo apt-get install flex的时候出现了标题上的问题，不能连接。
先ping了一下这个域名，发现能够ping的通，就是不懂为什么不能链接。
接着从网上找相应的解决方法，有网友说是源出问题拉，权且认为是这个源的主机down了。
然后从网上搜到与OS相对应的版本的源，我的是10.04,然后更改了源，就没问题了。
具体如下：
1. sudo gedit /etc/apt/source.list
删除原有内容
2. 将下面的源中选择一个，最好是物理上位置比较近的，这样连接速度快。复制到source.list中去。
Ubuntu 10.04源列表：
网易（速度很快）
**代码:**deb http://mirrors.163.com/ubuntu/ lucid main universe restricted multiverse
deb-src http://mirrors.163.com/ubuntu/ lucid main universe restricted multiverse
deb http://mirrors.163.com/ubuntu/ lucid-security universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ lucid-security universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ lucid-updates universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ lucid-proposed universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ lucid-proposed universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ lucid-backports universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ lucid-backports universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ lucid-updates universe main multiverse restricted
搜狐
**代码:**deb http://mirrors.shlug.org/ubuntu/ lucid main universe restricted multiverse
deb-src http://mirrors.shlug.org/ubuntu/ lucid main universe restricted multiverse
deb http://mirrors.shlug.org/ubuntu/ lucid-security universe main multiverse restricted
deb-src http://mirrors.shlug.org/ubuntu/ lucid-security universe main multiverse restricted
deb http://mirrors.shlug.org/ubuntu/ lucid-updates universe main multiverse restricted
deb http://mirrors.shlug.org/ubuntu/ lucid-proposed universe main multiverse restricted
deb-src http://mirrors.shlug.org/ubuntu/ lucid-proposed universe main multiverse restricted
deb http://mirrors.shlug.org/ubuntu/ lucid-backports universe main multiverse restricted
deb-src http://mirrors.shlug.org/ubuntu/ lucid-backports universe main multiverse restricted
deb-src http://mirrors.shlug.org/ubuntu/ lucid-updates universe main multiverse restricted
SRT Sources
**代码:**deb http://ubuntu.srt.cn/ubuntu/ lucid main restricted universe multiverse
deb http://ubuntu.srt.cn/ubuntu/ lucid-security main restricted universe multiverse
deb http://ubuntu.srt.cn/ubuntu/ lucid-updates main restricted universe multiverse
deb http://ubuntu.srt.cn/ubuntu/ lucid-proposed main restricted universe multiverse
deb http://ubuntu.srt.cn/ubuntu/ lucid-backports main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ lucid main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ lucid-security main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ lucid-updates main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ lucid-proposed main restricted universe multiverse
deb-src http://ubuntu.srt.cn/ubuntu/ lucid-backports main restricted universe multiverse
ubuntu官方上海源
**代码:**deb http://mirror.rootguide.org/ubuntu/ lucid main universe restricted multiverse
deb-src http://mirror.rootguide.org/ubuntu/ lucid main universe restricted multiverse
deb http://mirror.rootguide.org/ubuntu/ lucid-security universe main multiverse restricted
deb-src http://mirror.rootguide.org/ubuntu/ lucid-security universe main multiverse restricted
deb http://mirror.rootguide.org/ubuntu/ lucid-updates universe main multiverse restricted
deb http://mirror.rootguide.org/ubuntu/ lucid-proposed universe main multiverse restricted
deb-src http://mirror.rootguide.org/ubuntu/ lucid-proposed universe main multiverse restricted
deb http://mirror.rootguide.org/ubuntu/ lucid-backports universe main multiverse restricted
deb-src http://mirror.rootguide.org/ubuntu/ lucid-backports universe main multiverse restricted
deb-src http://mirror.rootguide.org/ubuntu/ lucid-updates universe main multiverse restricted
3. 运行一下sudo apt-get update
这样问题就能解决了

