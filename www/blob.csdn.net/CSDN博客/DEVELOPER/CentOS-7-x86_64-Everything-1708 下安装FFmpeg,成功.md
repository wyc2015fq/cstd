# CentOS-7-x86_64-Everything-1708 下安装FFmpeg,成功 - DEVELOPER - CSDN博客





2017年11月07日 14:24:43[学术袁](https://me.csdn.net/u012827205)阅读数：2094








今天20171107

这里记录一下，在CentOS系统下安装FFmpeg的过程；有一点就是，我在安装FFmpeg之前已经配置好了 JDK

安装过程有二：

首先是配置安装FFmpeg之前的准备，然后才是正式安装。

命令在这里：



```java
yum -y install epel-release
```

rpm --import http://li.nux.ro/download/nux/RPM-GPG-KEY-nux.ro



```java
rpm -Uvh http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-1.el7.nux.noarch.rpm
```



```java
yum -y install ffmpeg ffmpeg-devel
```

![](https://img-blog.csdn.net/20171107141024956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20171107141037011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171107141046361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



然后执行以下成功之后的命令：

![](https://img-blog.csdn.net/20171107141411814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171107141421382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







