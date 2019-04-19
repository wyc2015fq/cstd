# 关闭 禁用 Redis危险命令 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月30日 10:10:06[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：911
Redis的危险命令主要有：
flushdb,清空数据库
flushall,清空所有记录，数据库
config,客户端连接后可配置服务器
keys,客户端连接后可查看所有存在的键
我们常常需要禁用以上命令来使服务器更加安全。
禁用的具体做法是，修改服务器的配置文件redis.conf，在SECURITY这一项中，我们新增以下命令：
- rename-command FLUSHALL ""
- rename-command FLUSHDB ""
- rename-command CONFIG ""
- rename-command KEYS ""
如果想要保留命令，但是不能轻易使用，可以重命名命令来设定：
- rename-command FLUSHALL joYAPNXRPmcarcR4ZDgC81TbdkSmLAzRPmcarcR
- rename-command FLUSHDB qf69aZbLAX3cf3ednHM3SOlbpH71yEXLAX3cf3e
- rename-command CONFIG FRaqbC8wSA1XvpFVjCRGryWtIIZS2TRvpFVjCRG
- rename-command KEYS eIiGXix4A2DreBBsQwY6YHkidcDjoYA2DreBBsQ
这样重启服务器后，则需要用新命令来执行操作，否则服务器会报错 unknown command。
对于FLUSHALL命令，需要设置配置文件中appendonly no，否则服务器无法启动
