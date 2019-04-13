
# Redis管理基本操作 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:54:36[尹成](https://me.csdn.net/yincheng01)阅读数：16个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[](https://blog.csdn.net/yincheng01/article/category/7618299)



**@概述**
本节列举一些Redis中常见的非数据操作，包括帮助、数据库切换、状态监视等；
**@启动服务**
`redis-server.exe redis.windows.conf // 使用配置启动redis服务`**@查看服务状态（非redis命令模式下）**
`redis-cli --stat -a 123456 // 查看服务状态`显示如下：共有15个键，占用内存715K，有3个客户端连接（含DesktopManager和Python端）
![这里写图片描述](https://img-blog.csdn.net/20180403072124129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**@启动客户端**
`redis-cli.exe // 打开客户端
redis-cli --help // 查看客户端帮助
redis-cli -h 127.0.0.1 -p 6379 -a 123456 // 使用密码访问远程redis
auth 123456 // 使用密码登录
ping // 测试服务端连接状态`**@查看客户端命令帮助**
`help @connection // 查看连接类命令
help @server // 查看服务端相关命令`如图，客户端罗列出了所有相关命令和摘要
![这里写图片描述](https://img-blog.csdn.net/20180403072908269?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**@连接类命令**
`select 15 // 切换数据库（共提供16个数据库）
quit // 退出客户端`**@服务类命令**
`flushdb //清空当前库数据，谨慎使用
flushall // 清空所有数据库，谨慎使用
monitor // 监视服务端收到的信息`监视效果如图，当其它客户端访问redis时，所有操作都被monitor所记录了：
![这里写图片描述](https://img-blog.csdn.net/20180403073728963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**@允许远程连接**
编译配置文件
Linux下：/etc/redis/redis.conf
Windows下：安装文件夹/redis.windows.conf
`sudo gedit /etc/redis/redis.conf`做如下几处修改：
注释掉bind 127.0.0.1，取消和本机的绑定
修改protected-mode yes为protected-mode no，取消保护模式
解注释requirepass foobared，将密码修改为自定义的密码如requirepass 123456
注意，关闭或配置防火墙，允许远程访问Redis：
`sudo systemctl stop firewalld`远程连接
`redis-cli -h 192.168.23.181 -p 6379 -a 123456`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

