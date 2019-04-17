# 基于 Websocket 协议的压力测试 - zhusongziye的博客 - CSDN博客





2018年11月09日 20:46:07[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：161








**背景**

最近客户端实现了一个基于websocket的消息推送功能:长连接网络开关下发开后启动，然后保持链接，每隔15分钟发一次心跳包，服务器有数据时直接下发生效，因此需要对服务器进行压测。

本次通过写python脚本的方法模拟并发进行压力测试，相关测试方法如下。

**什么是websocket协议**

WebSocket是一种在单个TCP连接上进行全双工通讯的协议。WebSocket通信协议于2011年被IETF定为标准RFC 6455，并由RFC7936补充规范。

WebSocket使得客户端和服务器之间的数据交换变得更加简单，允许服务端主动向客户端推送数据。在WebSocketAPI中，浏览器和服务器只需要完成一次握手，两者之间就直接可以创建持久性的连接，并进行双向数据传输。



**测试场景**

1.并发创建长连接，3min内建立连接22w，总连接数维持在22w左右，间歇下发通知，查看服务器的稳定性。

2.并发创建长连接，间歇性断开并重连，保证同时在线的长连接在22w左右，运行12H,查看服务器的稳定性。



**测试步骤**

1.编写脚本实现并发请求并发



![](https://img-blog.csdnimg.cn/20181109204454277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018110920451277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



2.搭建测试环境



①找1台liunx机器，搭建python运行环境，安装脚本支持的包、模块



②运行websocket脚本，保证脚本正常运行



③ 编写run_test.sh脚本，运行多个websocket.py,单机大约可同时并发大约5w~6w的tcp链接，视linux 系统内核而定（因为本次需要同时并发22w，共4台liunx机器，所以单台并发6w左右）。



![](https://img-blog.csdnimg.cn/20181109204523324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)




**问题总结**

1.有些linux系统最大tcp链接数比较小，无法支持同时并发6w量的tcp链接。

2.原因：

最大tcp连接数和系统允许打开的最大文件数，用户允许打开的最大文件数，TCP网络连接可用的端口范围有关，取上述的最小值。

3.一般的设置规则为：系统最大文件数>可用端口>用户最大文件数



