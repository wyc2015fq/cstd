
# 1.02 docker环境的各种搭建方法 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:30:20[尹成](https://me.csdn.net/yincheng01)阅读数：89个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[](https://blog.csdn.net/yincheng01/article/category/7618299)



**1.docker的安装介绍**
官网：[https://docs.docker.com/](https://docs.docker.com/)
Docker 提供了两个版本：社区版 (CE) 和企业版 (EE)
社区版不同版本号的选择如下，以前是统一版本，现在分为两个版本了，当前是18年03月的版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054028128.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
docker的桌面支持平台如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054038871.png)
docker的服务支持平台如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054049222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
docker对云的支持主要有亚马逊的AWS和MA
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054059122.png)
2.docker在mac系统上安装
选择Docker for Mac (macOS)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054113146.png)
点击Download
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054120800.png)
注册
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120505413876.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054144542.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
注册时需要翻墙，这里使用蓝灯进行翻墙，安装后连接成功就可以了，不翻墙的话没有下面人机验证
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054155445.png)
注册如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054204861.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
注册需要邮箱验证
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054214405.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
注册后登录，下载docker for mac的最新稳定版
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054223983.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
打开docker.dmg
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054232440.png)
将左边的拖到右边
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054239598.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
桌面上找到docker，点击
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054246743.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
点击Open，开始启动
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054254922.png)
Next
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054301662.png)
OK
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054308947.png)
输入用户名、密码，然后点击OK
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054316789.png)
开启启动docker
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120505432313.png)
docker启动成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054330286.png)
查看docker版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054337549.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054344133.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
[](https://img-blog.csdnimg.cn/20181205054337549.png)3.docker在windows系统上安装
[](https://img-blog.csdnimg.cn/20181205054337549.png)需要Win10或者Windows Server2016
Win10需要是64位的，还要有Hyper-V available（微软实现的虚拟化技术）
选择Docker for Windows
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054353467.png)
下载最新稳定版
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054404711.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
傻瓜式安装，然后打开桌面docker快捷方式，重启，完成
不建议直接在自己的windows或mac玩docker，因为会下很多东西，清理也不是很容易
4.docker在CentOS系统上安装
选择CentOS
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054418712.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
官方文档：[https://docs.docker.com/install/linux/docker-ce/centos/\#set-up-the-repository](https://docs.docker.com/install/linux/docker-ce/centos/#set-up-the-repository)
在线yum安装
yum update 更新软件包
CentOS7上卸载旧的版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054427362.png)
安装依赖
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054440300.png)
添加repository
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054447420.png)
查看可安装的版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054456163.png)
安装指定版本docker-ce
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120505450419.png)
启动docker
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054511515.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054518825.png)
设为开机自启动
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054526454.png)
查看版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054534907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
实现HelloWorld
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054541594.png)
5.docker在Ubuntu系统上安装
apt-get -y update
apt-get -y install docker-ce
6.docker-machine的使用
Docker Machine 是 Docker 官方提供的一个工具，它可以帮助我们在远程的机器上安装 Docker，或者在虚拟机 host 上直接安装虚拟机并在虚拟机中安装 Docker
可以通过 docker-machine 命令来管理这些虚拟机和 Docker
安装方法1
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054554564.png)
安装方法2
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054609921.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054617397.png)
创建安装好docker的小巧的虚拟机
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054624661.png)
7.docker palyground
地址：[https://labs.play-with-docker.com/](https://labs.play-with-docker.com/)
直接使用云端的docker
登录，这里是用docker官网注册的用户名和密码，登录后点击start
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054640624.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
添加新的实例
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054648816.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
查看版本，共享dockder，但是有4小时的使用时间限制
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205054659962.png)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

