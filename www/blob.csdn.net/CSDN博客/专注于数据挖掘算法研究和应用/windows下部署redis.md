# windows下部署redis - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月04日 16:49:30[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2031
个人分类：[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)









Redis存储的value类型，包括string(字符串)、list(链表)、set(集合)、zset(sorted
 set --有序集合)和hash（哈希类型），支持push/pop、add/remove及取交集并集和差集的原子性操，支持各种不同方式的排序。数据缓存在内存中，周期性的把更新的数据写入磁盘。




1、下载并解压：

redis官方网站：http://redis.io/

因在windows使用，在https://github.com/MSOpenTech/redis/tree/2.6下载redis2.6版本并解压；

在\bin\release路径下选择64位压缩包解压，将其中的所有EXE文件拷贝到根目录下；


2、服务端启动：

   dos下进入redis根目录开启服务

   执行命令：redis-server.exe redis.conf

   窗口保持开启，关闭窗口即关闭服务；

![](https://img-blog.csdn.net/20160204164752274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3、客户端启动：

   新开启dos进入redis根目录

   执行命令：redis-cli.exe -h 127.0.0.1 -p 6379

   连接后测试；


![](https://img-blog.csdn.net/20160204164916776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




