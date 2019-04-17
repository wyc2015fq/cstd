# git使用小结 - xmdxcsj的专栏 - CSDN博客





2014年08月26日 21:20:46[xmucas](https://me.csdn.net/xmdxcsj)阅读数：503
个人分类：[工具](https://blog.csdn.net/xmdxcsj/article/category/2392185)









1.      Git使用

注册gitlab的账号

下载安装msysGit和TortoiseGit

在git bash执行：

ssh-keygen -t rsa -C"xmdxcsj@163.com"

将获取的秘钥复制到gitlab，gitlab服务器就可以无密码访问本地电脑。



2.      向git上传项目

首先在git新建一个项目

然后在本地项目所在的文件夹执行：



每次本地更新文件以后，需要执行以下操作：

git commit –m “”

git push

每次网站更新文件以后，需要执行：

git pull



3.从别人的项目中下载源代码

下载源码



切换分支，





修改源代码，提交








