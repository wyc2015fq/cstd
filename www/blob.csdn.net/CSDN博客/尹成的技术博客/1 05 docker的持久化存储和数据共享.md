
# 1.05 docker的持久化存储和数据共享 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:32:47[尹成](https://me.csdn.net/yincheng01)阅读数：81个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[](https://blog.csdn.net/yincheng01/article/category/7618299)



**1.数据持久化引入**
container可以读和写，image只读
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150550949.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
但是container写入的数据只在container内，如果将容器删除，那么数据丢失
如果是数据库的容器，数据丢失后果很严重
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150601445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
2.数据持久化方案
[](https://img-blog.csdnimg.cn/20181205150601445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)基于本地文件系统的Volume，可以执行docker create或docker run时，通过-v参数将主机的目录作为容器的数据卷，这部分功能便是基于本地文件系统的volume管理
[](https://img-blog.csdnimg.cn/20181205150601445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)基于plugin的Volume，支持第三方的存储方案，例如NAS，aws
[](https://img-blog.csdnimg.cn/20181205150601445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)3.Volume的类型
受管理的data Volume：由docker后台自动创建
绑定挂载的Volume：具体挂载位置可以由用户指定
4.数据持久化——data Volume
创建mysql容器，指定为不需要密码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150720379.png)
查进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150732141.png)
查看所有的volume
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515073839.png)
查看volume具体信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150743180.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150747651.png)
停止mysql的容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515075858.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150804190.png)
删除mysql的容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515081085.png)
查看volume数据还在
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150821554.png)
删掉所有不被引用的volume
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150826931.png)
创建容器，将容器中/var/lib/mysql下产生的数据，volume到本地重命名为mysql
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150834530.png)
查看产生的volume
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150840831.png)
交互执行，并进入mysql
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150846218.png)
创建库并退出
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150852437.png)
将mysql1容器停止并删除
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150857847.png)
进入目录查看数据
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150903620.png)
创建新的mysql容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150910243.png)
交互运行并进入mysql
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150915526.png)
查看数据，实现了数据持久化
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150920759.png)
5.数据持久化——bind mouting
创建并进入目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150926272.png)
创建index.html文件，并写入以下内容
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150931777.png)
<h1>Hello Docker! </h1>创建Dockerfile文件，并写入如下内容
FROM nginx:latest
WORKDIR /usr/share/nginx/html
COPY index.html index.html构建image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150937327.png)
查看image
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515094461.png)
起别名创建容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205150954306.png)
可以从本地访问index.html
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151002239.png)
停止并删除容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151008115.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151018280.png)
重新创建容器，将本地/home/nginx目录与容器内/usr/share/nginx/html目录映射，目录是在Dockerfile里定义的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151023680.png)
交互运行，查看目录下文件，创建一个新文件test.txt，然后exit退出
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515103125.png)
在本地查看/home/nginx目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151041517.png)
给文件添加点内容，然后再进入容器查看文件内容
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151047359.png)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

