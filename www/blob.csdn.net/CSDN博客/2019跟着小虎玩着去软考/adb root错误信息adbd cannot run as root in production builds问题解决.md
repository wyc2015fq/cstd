# adb root错误信息adbd cannot run as root in production builds问题解决 - 2019跟着小虎玩着去软考 - CSDN博客
2011年06月16日 17:00:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：40251标签：[permissions																[手机](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)
个人分类：[Android](https://blog.csdn.net/littletigerat/article/category/613910)
**adb root****错误信息****adbd cannot run as root in production builds****问题解决**
# 一．问题描述
## 1.输入指令
>adb root
adbd cannot run as root in production builds
**错误信息**：
adbd cannot run as root in production builds
2.输入指令：
cd data
$ ls –l
ls –l
opendir failed, Permission denied
$ exit
## 错误信息：
opendir failed, Permission denied
# 二．解决办法：
1.首先在手机端安装Superuser Permissions软件；
2.然后执行执行如下命令
adb shell
su
手机上点允许
变#号就成功
