# centos7下安装Python的pip - z69183787的专栏 - CSDN博客
2018年02月07日 11:16:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：108
个人分类：[其他语言-Python																[Linux-虚拟机](https://blog.csdn.net/z69183787/article/category/2281091)](https://blog.csdn.net/z69183787/article/category/6359977)

root用户使用yum install -y python-pip 时会报如下错误：
No package python-pip available
Error:Nothing to do
解决方法如下：
　　首先安装epel扩展源：
　　yum -y install epel-release
　　更新完成之后，就可安装pip：
　　yum -y install python-pip
　　安装完成之后清除cache：
　　yum clean all
这是在root用户时使用的命令，当前用户如果不具有root权限，加上sudo。
在其他Linux类似centos衍生的发行版也可以用此方法解决。
