# 百万级开源MQTT消息服务器  搭建 - baidu_33570760的博客 - CSDN博客
2017年08月25日 17:44:51[carman_风](https://me.csdn.net/baidu_33570760)阅读数：8746
下载地址：http://emqtt.com/downloads
文档地址：http://emqtt.com/docs/v2/index.html
# 开始使用
## *EMQ* 2.0 消息服务器简介
*EMQ* (Erlang/Enterprise/Elastic MQTT Broker)是基于Erlang/OTP平台开发的开源物联网MQTT消息服务器。Erlang/OTP是出色的软实时(Soft-Realtime)、低延时(Low-Latency)、分布式(Distributed)的语言平台。MQTT是轻量的(Lightweight)、发布订阅模式(PubSub)的物联网消息协议。
*EMQ* 项目设计目标是承载移动终端或物联网终端海量的MQTT连接，并实现在海量物联网设备间快速低延时(Low-Latency)消息路由:
- 稳定承载大规模的MQTT客户端连接，单服务器节点支持50万到100万连接。
- 分布式节点集群，快速低延时的消息路由，单集群支持1000万规模的路由。
- 消息服务器内扩展，支持定制多种认证方式、高效存储消息到后端数据库。
- 完整物联网协议支持，MQTT、MQTT-SN、CoAP、WebSocket或私有协议支持
## Windows服务器安装
Windows平台程序包下载: [http://emqtt.com/downloads/latest/windows10](http://emqtt.com/downloads/latest/windows10)
程序包下载解压后，打开Windows命令行窗口，cd到程序目录。
控制台模式启动:
bin\emqttdconsole
如启动成功，会弹出控制台窗口。
关闭控制台窗口，停止emqttd进程，准备注册Windows服务。
警告
EMQ-2.0暂不支持服务注册
*EMQ* 注册为Windows服务:
bin\emqttdinstall
*EMQ* 服务启动:
bin\emqttdstart
*EMQ* 服务停止:
bin\emqttdstop
*EMQ* 服务卸载:
bin\emqttduninstall
其他更多版本下载与安装，请参考上面的连接，进行对应的下载与安装。
