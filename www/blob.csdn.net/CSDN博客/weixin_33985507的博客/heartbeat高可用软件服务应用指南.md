# heartbeat高可用软件服务应用指南 - weixin_33985507的博客 - CSDN博客
2013年05月03日 18:05:07[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
**本文为《老男孩培训》待出书草稿，对应的VIP视频已录制完毕。欢迎企业及个人咨询合作。**
目录
第8章   HEARTBEAT高可用软件服务及实战配置 1
8.1 HEARTBEAT介绍 3
8.1.1 Heartbeat作用 3
8.1.2 Heartbeat工作原理 3
8.1.3 Heartbeat的心跳连接 3
8.1.4 Heartbeat裂脑 4
8.1.4.1什么是裂脑？ 4
8.1.4.2导致裂脑发生的多种原因 5
8.1.4.2防止裂脑发生的8种秘籍 5
8.1.6 Heartbeat消息类型 6
8.1.6.1心跳消息： 6
8.1.6.2 集群转换消息 6
8.1.6.3 重传请求 6
8.1.7 Heartbeat IP地址接管和故障转移 6
8.1.8 VIP/IP别名/辅助IP 7
8.1.9 Heartbeat脚本默认目录 8
8.1.10 Heartbeat配置文件 8
8.2 部署HEARTBEAT需求 9
8.2.1 业务需求描述 9
8.2.2 heartbeat部署结构图 9
8.2.3 生产环境服务器硬件配置 10
8.2.4 操作系统：Centos5.8 64bit 10
8.2.5 heartbeat服务主机资源规划 10
8.3 实施基础准备 11
8.3.1 搭建虚拟机模拟真实环境 11
8.3.2给虚拟机配置IP和主机名 11
8.3.3 SSH连接虚拟机 12
8.3.3 分别配置hosts文件 12
8.3.3 配置服务器间心跳连接： 13
8.4 开始实施部署 14
8.4.1 安装heartbeat软件 14
8.4.2 配置heartbeat服务 15
8.4.2.1 配置ha.cf文件 15
8.4.2.2 ha.cf文件详细说明 16
8.4.2.3 配置authkey文件 17
8.4.2.4 配置haresource文件 18
8.4.2.5 配置haresource说明 18
8.5 检测HEARTBEAT部署成果 19
8.5.1 启动heartbeat服务 19
8.5.2 模拟故障转移 20
8.5.3 通过heartbeat日志分析接管过程 20
8.6 HEARTBEAT实现WEB服务高可用案例 21
8.6.1 部署准备 21
8.6.2 安装配置http服务 21
8.6.3配置http服务启动脚本 21
8.6.4修改haresource配置 21
8.6.5在两台机器上分别重起heartbeat 22
8.6.5 进行heartbeat测试 22
8.6.6 有关heartbeat调用httpd脚本的说明 31
8.6.7有关heartbeat调用资源的生产场景应用 31
8.6.8 本httpd案例结论： 31
8.6.9 heartbeat案例拓展 31
8.6.10 heartbeat和keepalived的应用场景区别 31
8.6.11 heartbeat服务生产环境下维护要点 32
8.6.11.1 修改配置文件要点 32
8.6.11.2 经常查看heartbeat服务日志 33
8.6.12 heartbeat服务生产环境维护FAQ 33
问题1：一个局域网中是多组heartabeat服务同时开启的问题 33
问题2：ha+drbd脑裂问题的解决总结（准备上生产环境的实际部署） 33
附录一：晒晒生产环境HEARTBEAT所有配置文件 37
 picdata-1-1 heartbeat所有配置文件 37
 picdata-1-2 heartbeat所有配置文件 38
附录二：HEARTBEAT服务重要脚本文件解读 38
