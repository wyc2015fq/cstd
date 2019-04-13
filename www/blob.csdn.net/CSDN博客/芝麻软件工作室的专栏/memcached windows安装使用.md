
# memcached windows安装使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月10日 11:41:34[seven-soft](https://me.csdn.net/softn)阅读数：239个人分类：[memcache																](https://blog.csdn.net/softn/article/category/6304643)



Windows平台下Memcached安装与配置方法,Memcached
 是一个高性能的分布式内存对象缓存系统，用于动态Web应用以减轻数据库负载，下面介绍memcached windows安装使用。
   Memcached 是一个高性能的分布式内存对象缓存系统，用于动态Web应用以减轻数据库负载。它通过在内存中缓存数据和对象来减少读取数据库的次数，从而提供动态、数据库驱动网站的速度。Memcached基于一个存储键/值对的hashmap。其守护进程（daemon ）是用C写的，但是客户端可以用任何语言来编写，并通过memcached协议与守护进程通信。但是它并不提供冗余（例如，复制其hashmap条目）；当某个服务器S停止运行或崩溃了，所有存放在S上的键/值对都将丢失。
一、下载Memercached For Windows
下载地址：http://www.jb51.net/softs/44843.html
![memcached文件列表](http://www.th7.cn/d/file/p/2015/01/27/373e35365849df97763ae382b7056435.jpg)二、memcache安装步骤
1、解压到指定目录，如：C:/Memcached/memcached-win32-1.4.4-14。
2、用cmd打开命令窗口，转到解压的目录，输入 “memcached.exe -d install”。
3、打开控制面板，打开服务，可以看到memcached已经在上面可，如果没有启动，则手动启动一下。
![memcached已在控制面板服务中](http://www.th7.cn/d/file/p/2015/01/27/820963db130115f482f8aec28fdb0b83.jpg)4、 使用telnet命令 验证缓存服务器是否可用。
![使用telnet命令 验证缓存服务器是否可用](http://www.th7.cn/d/file/p/2015/01/27/081bacf76ea0f4fd63cf9fe1c7a56272.jpg)
开始什么都不显示，回车后输入命令 stats 查看统计信息，如下图，说明服务器运作正常。
三、参数介绍
1、 以上的安装和启动都是在默认环境下进行的，在安装时可设置如下参数：
-p 监听的端口
-l 连接的IP地址, 默认是本机
-d start 启动memcached服务
-d restart 重起memcached服务
-d stop|shutdown 关闭正在运行的memcached服务
-d install 安装memcached服务
-d uninstall 卸载memcached服务
-u 以的身份运行 (仅在以root运行的时候有效)
-m 最大内存使用，单位MB。默认64MB
-M 内存耗尽时返回错误，而不是删除项
-c 最大同时连接数，默认是1024
-f 块大小增长因子，默认是1.25
-n 最小分配空间，key+value+flags默认是48
-h 显示帮助
如：“memcached -d install -l 127.0.0.1 -m 1024 -c2048”。
2、 如果在安装时没有添加参数，可通过修改注册表信息进行设置，打开注册表，找
HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/memcached
在其中有一个“ImagePath”项，值为：
"C:/Memcached/memcached-win32-1.4.4-14/memcached.exe" -d runservice
在后面加上“-m 1024 -c2048”。如下图：
![修改注册表，添加memcached参数](http://www.th7.cn/d/file/p/2015/01/27/47b0c08623dcd85a6cc38b3a39f1eb1d.jpg)
3、输入stats命令后，页面出现的参数介绍。
STAT pid 4356 服务器进程IDSTAT uptime 56625 服务器运行时间，单位秒STAT time 1225249079 服务器当前的UNIX时间STAT version1.1.0服务器的版本号STAT pointer_size 64STAT rusage_user 151.845489 该进程累计的用户时间(秒:微妙)STAT rusage_system 121.667603 该进程累计的系统时间(秒:微妙)STAT ibuffer_size 4096STAT curr_connections 13 连接数量STAT total_connections 54136 服务器运行以来接受的连接总数STAT connection_structures 318 服务器分配的连接结构的数量STAT cmd_get 100595 取回请求总数STAT cmd_set 6510 存储请求总数STAT get_hits 96543 请求成功的总次数STAT get_misses 4052 请求失败的总次数STAT bytes_read 4427679 服务器从网络读取到的总字节数STAT bytes_written 6585596 服务器向网络发送的总字节数
备注：
uptime 是memcached运行的秒数，cmd_get是查询缓存的次数。 cmd_get/uptime得到平均每秒请求缓存的次数。cmd_set是设置key=>value的次数。整个memcached是个大hash，用cmd_get没有找到的内容，就会调用cmd_set写进缓存里。get_hits是缓存命中的次数，缓存命中率 = get_hits/cmd_get *100%。get_misses加上get_hits等于cmd_get。total_itemscurr_items表示现在在缓存中的键值对个数。total_items == cmd_set == get_misses，不过当可用最大内存用光时，如果memcached设置为删掉内容，上面的等式就不成立了。

