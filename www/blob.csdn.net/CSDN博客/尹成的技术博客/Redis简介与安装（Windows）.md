
# Redis简介与安装（Windows） - 尹成的技术博客 - CSDN博客

2018年11月16日 10:02:26[尹成](https://me.csdn.net/yincheng01)阅读数：15个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**@简介**
redis是一种高速高性能的内存数据库，存储的是键值对数据；
由于极高的存储和访问速度，redis常被用来作为数据缓存服务器；
另外redis是支持数据持久化的（即redis会自动将数据写入磁盘）——这是redis大杀四方的重要原因；
redis支持多个slave端同时向master端进行写入，这经常被用在分布式爬虫等场景中；
2010年以后redis开始大面积流行，关系型数据库（核心数据）+redis（缓存）是常规的、常见的数据解决方案；
**@支持数据类型**
redis可以存储的值的数据类型包括：
字符串（单个value最大支持512M）；
哈希hash（存储对象） ；
列表list ；
集合set ；
有序集合zset；
**@下载地址和参考手册**
windows版redis下载地址：[https://github.com/ServiceStack/redis-windows](https://github.com/ServiceStack/redis-windows)
可视化工具RedisDesktopManager下载地址：[https://redisdesktop.com/download](https://redisdesktop.com/download)
中文版命令参考手册：[http://redis.cn/commands.html](http://redis.cn/commands.html)
**@安装redis**
下载到的压缩包中包含多个不同历史版本，这里我们选择解压Redis-x64-3.2.100到任意目录（路径不能含有中文）；
![-](https://img-blog.csdn.net/20180402212335400?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用管理员权限对redis.windows.conf进行修改，添加最大内存和密码两项设置，这里我配置的是最大1073741824字节（即1G存）和以“123456”作为密码：
maxmemory 1073741824
requirepass 123456将命令行切换到上图所示的redis解压目录，执行如下命令，即使用redis.windows.conf作为配置文件启动redis服务：
redis-server.exe redis.windows.conf可以发现redis的服务处于运行状态，监听在端口6379
![这里写图片描述](https://img-blog.csdn.net/20180402213151280?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
不要关闭刚才的redis服务窗口，在新打开的命令行窗口中进入redis的解压目录，使用如下命令启动redis客户端：
redis-cli.exe这时命令行窗口已进入redis命令模式，输入密码进行登录后，ping一下自己，收到PONG的响应！
![这里写图片描述](https://img-blog.csdn.net/2018040221362849?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
写入一个简单的键值对数据进行验证：
set name 'hellosteve'
get name![这里写图片描述](https://img-blog.csdn.net/20180402214021321?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
正确地读到了写出的键值对数据，至此安装完成！
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

