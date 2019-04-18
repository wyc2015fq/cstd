# python 在升级到python2.7之后安装相关插件 - weixin_33985507的博客 - CSDN博客
2017年03月01日 11:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
```
# yum update
# yum install centos-release-SCL
# yum search all python27
在搜索出的列表中发现python27-python-devel.x86_64，然后执行如下命令即可：
yum install python27-python-devel.x86_64
不然的话安装的python-devel都是2.6.6版本的
```
