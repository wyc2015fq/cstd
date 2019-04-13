
# Redis 连接 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:26:34[seven-soft](https://me.csdn.net/softn)阅读数：380


Redis 连接命令主要是用于连接 redis 服务。
### 实例
以下实例演示了客户端如何通过密码验证连接到 redis 服务，并检测服务是否在运行：
redis127.0.0.1:6379>AUTH"password"OK
redis127.0.0.1:6379>PING
PONG
---

## Redis 连接命令
下表列出了 redis 连接的基本命令：
|序号|命令及描述|
|1|[AUTH password](http://www.runoob.com/redis/connection-auth.html)
|验证密码是否正确|
|2|[ECHO message](http://www.runoob.com/redis/connection-echo.html)
|打印字符串|
|3|[PING](http://www.runoob.com/redis/connection-ping.html)
|查看服务是否运行|
|4|[QUIT](http://www.runoob.com/redis/connection-quit.html)
|关闭当前连接|
|5|[SELECT index](http://www.runoob.com/redis/connection-select.html)
|切换到指定的数据库|

